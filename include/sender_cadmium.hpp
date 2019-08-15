/** \brief This header file implements the Sender class.
*
* The sender sends messages on the output port
* and receives acknowledge on the input port.
*
* The sender goes from initial passive phase
* to active when an external signal is received.
* Once activated it sends the packet with the
* alternating bit. Every time a packet is sent,
* it waits for an acknowledgement during a specific
* wait time. If the acknowledgement does not arrive
* within the time window, the sender will resend the 
* previous packet with the alternating bit. If the 
* expected acknowledgement is received within the time
* window, the sender will send the next packet. When
* there are no more packets to send, the sender will
* go again to the passive state. 
*/
/* Cristina Ruiz Martin
* ARSLab - Carleton University
*
*/

#ifndef __SENDER_CADMIUM_HPP__
#define __SENDER_CADMIUM_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>
#include <limits>
#include <math.h> 
#include <assert.h>
#include <memory>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <limits>
#include <random>

#include "message.hpp"

using namespace cadmium;
using namespace std;

/** 
* Structure that holds input and output messages.
*/
struct sender_defs {
    struct packet_sent_out : public out_port<Message_t> {
    };
    struct ack_received_out : public out_port<Message_t> {
    };
    struct data_out : public out_port<Message_t> {
    };
    struct control_in : public in_port<Message_t> {
    };
    struct ack_in : public in_port<Message_t> {
    };
};

/** 
* The Sender class sends out messages and receives acknowledges.
*/
template<typename TIME>
class Sender {
    /** putting definitions in context */
    using defs = sender_defs;
    public:
        TIME PREPARATION_TIME;    /**< Constant that holds the time delay */
                                  /**< from acknowledge to output. */
                                  /*!<Time delay constant.*/
        TIME TIMEOUT;             /**< Constant that holds the timeout delay */
                                  /**< from output to acknowledge. */
                                  /*!<Timeout constant.*/
        
        /** 
        * Constructor for Sender class.
        * Initializes the delay constants and state structure.
        */
        Sender() noexcept {
            PREPARATION_TIME = TIME("00:00:10");
            TIMEOUT          = TIME("00:00:20");
            state.alt_bit    = 0;
            state.next_internal    = std::numeric_limits<TIME>::infinity();
            state.model_active     = false;
        }
            
        /**
        * Structure that holds the state variables.
        */
        struct state_type {
            bool ack;              /*!< Acknowledge bit: true - acknowledge.*/
            int packet_num;        /*!< Packet Number to be sent.*/
            int total_packet_num;  /*!< Total Packet Number.*/
            int alt_bit;           /*!< Alternating Bit.*/
            bool sending;          /*!< State: true - sending.*/
            bool model_active;     /*!< True - model is active.*/
            TIME next_internal;    /*!< Time of next internal transition.*/
        }; 
        state_type state;
            
        /** ports definition */
        using input_ports = std::tuple<typename defs::control_in,
            typename defs::ack_in>;
        using output_ports = std::tuple<typename defs::packet_sent_out,
        typename defs::ack_received_out, typename defs::data_out>;
        
        /**
        * Function that performs internal transition.
        * It sets the next state based on the current state.
        */
        void internal_transition() {
            if (state.ack) {
                if (state.packet_num < state.total_packet_num) {
                    state.packet_num++;
                    state.ack = false;
                    state.alt_bit = (state.alt_bit + 1) % 2;
                    state.sending = true;
                    state.model_active = true; 
                    state.next_internal = PREPARATION_TIME;   
                } 
                else {
                    state.model_active = false;
                    state.next_internal = 
                    std::numeric_limits<TIME>::infinity();
                }
            }
            else {
                if (state.sending) {
                    state.sending = false;
                    state.model_active = true;
                    state.next_internal = TIMEOUT;
                }
                else {
                    state.sending = true;
                    state.model_active = true;
                    state.next_internal = PREPARATION_TIME;    
                } 
            }   
        }

        /**
        * Function that performs external transition.
        * Retrieves the messages: if the number of messages
        * is more than 1, it asserts that only one message is
        * expected per time unit. After that it determines
        * and sets the next state based on the current state.
        * It also sets the time of next internal transition. 
        * @param e time variable
        * @param mbs message bags
        */
        void external_transition(TIME e,
            typename make_message_bags<input_ports>::type mbs) { 
            if ((get_messages<typename defs::control_in>(mbs).size()
                +get_messages<typename defs::ack_in>(mbs).size()) > 1) {
                assert(false && "one message per time uniti");
            }
            for (const auto &x :
                get_messages<typename defs::control_in>(mbs)) {
                if (state.model_active == false) {
                    state.total_packet_num = static_cast<int>(x.value);
                    if (state.total_packet_num > 0) {
                        state.packet_num = 1;
                        state.ack = false;
                        state.sending = true;
                        /** set initial alt_bit */
                        state.alt_bit = state.packet_num % 2;
                        state.model_active = true;
                        state.next_internal = PREPARATION_TIME;
                    }
                    else if (state.next_internal != 
                        std::numeric_limits<TIME>::infinity()) {
                        state.next_internal = state.next_internal - e;
                    }
                }
            }
            for (const auto &x : get_messages<typename defs::ack_in>(mbs)) {
                if (state.model_active == true) { 
                    if (state.alt_bit == static_cast<int>(x.value)) {
                        state.ack = true;
                        state.sending = false;
                        state.next_internal = TIME("00:00:00");
                    }
                    else if(state.next_internal !=
                        std::numeric_limits<TIME>::infinity()) {
                        state.next_internal = state.next_internal - e;
                    }
                }
            }                     
        }

        /** 
        * Function that calls internal transition
        * followed by external transition.
        * @param e time variable
        * @param mbs message bags
        */
        void confluence_transition(TIME e,
            typename make_message_bags<input_ports>::type mbs) {
            internal_transition();
            external_transition(TIME(), std::move(mbs));
        }

        /**
        * Function that sends the packet to the output port.
        * When in sending state, the packet is calculated as
        * packet number multiplied by 10 + alternating bit
        * and assigned to output. The output is then pushed
        * to message bags.
        * When in acknowledge state, the alt_bit is 
        * assigned to output and the output is pushed
        * to message bags.
        * @return Message bags
        */
        typename make_message_bags<output_ports>::type output() const {
            typename make_message_bags<output_ports>::type bags;
            Message_t out;
            if (state.sending) {
                out.value = state.packet_num * 10 + state.alt_bit;
                get_messages<typename defs::data_out>(bags).push_back(out);
                out.value = state.packet_num;
                get_messages<typename
                    defs::packet_sent_out>(bags).push_back(out);
            }
            else if (state.ack) {
                out.value = state.alt_bit;
                get_messages<typename
                    defs::ack_received_out>(bags).push_back(out);
            }   
            return bags;
        }

        /**
        * Function with no parameters that returns the next 
        * internal transition time.
        * @return Next internal time
        */
        TIME time_advance() const {  
            return state.next_internal;
        }

        /**
        * Function that outputs packet number and 
        * total packet number to ostring stream.
        * @param os the ostring stream
        * @param i structure state_type
        * @return os the ostring stream
        */
        friend std::ostringstream& operator<<(std::ostringstream& os,
            const typename Sender<TIME>::state_type& i) {
            os << "packetNum: " << i.packet_num << 
                " & totalPacketNum: " << i.total_packet_num; 
            return os;
        }
};     

#endif /** _SENDER_CADMIUM_HPP_ */

/** \brief This header file implements the Repeater class
 * that acts as a module that takes input from subnet and
 * after processing it send it to the receiver and get
 * acknowledgement from receiver and forward it to sender
 * after processing it.
 *
 * Repeater has 2 inputs and 2 outputs port for packet and
 * acknowledgement respectively. It adds a 10 second
 * processing time to packets and send it to respective output ports
*/
/*
 * Peter Bliznyuk-Kvitko
 * Shubham Agrawal
 * Carleton University
*/

#ifndef __REPEATER_CADMIUM_HPP__
#define __REPEATER_CADMIUM_HPP__

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
 * Structure that holds input and output messages
 * using input and output port.
*/
struct repeater_defs {
    struct packet_sent_out : public out_port<Message_t> {
    };
    struct ack_received_out : public out_port<Message_t> {
    };
    struct packet_in : public in_port<Message_t> {
    };
    struct ack_in : public in_port<Message_t> {
    };
};

/**
 * The Repeater class sends out messages and acknowledgements to receiver and sender.
*/
template<typename TIME>
class Repeater {
    // putting definitions in context
    using defs = repeater_defs;
public:
    TIME PREPARATION_TIME;    /**< Constant that holds the time delay from acknowledge to output. */ //!<Time delay constant.

    /**
     * Constructor for Repeater class.
     * Initializes the delay constants and state structure.
    */
    Repeater() noexcept {
        PREPARATION_TIME = TIME("00:00:10");
        state.ack    = 0;
        state.packet = 0;
    }

    /**
     * Structure that holds the state variables for packet
     * acknowledgement packet and to check if sending packet
     * or acknowledgement.
    */
    struct state_type {
        bool ack;
        bool sending;
        int packet;
        int ack_packet;
    };
    state_type state;

    /**
     * Defining input port and output port
    */
    using input_ports = std::tuple<typename defs::packet_in,
        typename defs::ack_in>;
    using output_ports = std::tuple<typename defs::packet_sent_out,
        typename defs::ack_received_out>;

    /**
     * Internal transition function that sets the
     * state of sending packet and acknowledgement
     * to passive.
    */
    void internal_transition() {
        state.sending = false;
        state.ack = false;
    }

    /**
     * Function that performs external transition.
     * Retrieves the messages: if the number of messages
     * is more than 1, it asserts that only one message is
     * expected per time unit. After that it determines
     * from which input port message is coming and based
     * on the message type sets the state.
     * @param e time variable
     * @param mbs message bags
    */
    void external_transition(TIME e,
                             typename make_message_bags<input_ports>::type mbs) {
        if (get_messages<typename defs::packet_in>(mbs).size() > 1) {
            assert(false && "One message at a time");
        }
        for (const auto &x : get_messages<typename defs::packet_in>(mbs)) {
            state.packet = static_cast<int>(x.value);
            state.sending = true;
        }
        if (get_messages<typename defs::ack_in>(mbs).size() > 1) {
            assert(false && "One message at a time");
        }
        for (const auto &x : get_messages<typename defs::ack_in>(mbs)) {
            state.ack_packet = static_cast<int>(x.value);
            state.ack = true;
        }
    }

    /**
     * it is the function that make call to internal_transition
     * followed by call to external transition.
     * @param e time variable
     * @param mbs message bags
    */
    void confluence_transition(TIME e,
                               typename make_message_bags<input_ports>::type mbs) {
        internal_transition();
        external_transition(TIME(), std::move(mbs));
    }

    /**
     * Funtion that sends the packet to the respective
     * output ports depending on the state type.
     * When in sending state the value of the input is assigned
     * to the packet and then sent to the mesage bags.
     * When in acknowledgement state ack packet value is
     * assignet ot the output and output is pushed to message bags.
     * @return Message bags
    */
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
        Message_t out;
        if (state.sending) {
            out.value = state.packet;
            get_messages<typename defs::packet_sent_out>(bags).push_back(out);
        }
        if (state.ack)
        {
            out.value = state.ack_packet;
            get_messages<typename defs::ack_received_out>(bags).push_back(out);
        }
        return bags;
    }

    /**
     * Function that sets the next internal transaction time
     * @return Next internal time
    */
    TIME time_advance() const {
        TIME next_internal;
        if(state.sending || state.ack)  {
            next_internal=PREPARATION_TIME;
        }
        else{
            next_internal = std::numeric_limits<TIME>::infinity();
        }
        return next_internal;
    }

    /**
     * Function that outputs packet num and ack num to ostring stream
     * @param os the ostring stream
     * @param i  the structure state type
     * @return os the ostring stream
    */
    friend std::ostringstream& operator<<(std::ostringstream& os,
                                          const typename Repeater<TIME>::state_type& i) {
        os << "packetNum: " << i.packet <<
           " & ackpacket: " << i.ack_packet;
        return os;
    }
};

#endif /**ALTERNATEBITPROTOCOLSIMULATOR_REPEATER_CADMIUM_HPP*/

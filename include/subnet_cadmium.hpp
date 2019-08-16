/** \brief This header file implements the Subnet class
 *
 * The subnet receives the message on input port
 * and transmits the same message to the output port
 * after generating some delay on output port.
 *
 * The subnet has two phases: active and passive
 * it is in active state when transmitting the
 * data and goes to passive state when no data
 * is being send or acknowledged
 *
 */
 /*
 * Cristina Ruiz Martin
 * ARSLab - Carleton University
*/

#ifndef __SUBNET_CADMIUM_HPP__
#define __SUBNET_CADMIUM_HPP__

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
struct subnet_defs {
    struct out : public out_port<Message_t> {
    };
    struct in : public in_port<Message_t> {
    };
};

/**
 * The Subnet class receives message and
 * transmits out same message with some delay.
*/
template<typename TIME>
class Subnet {
    /*putting definitions in context*/
    using defs = subnet_defs;
    public:
        /*
	 * Any Parameters to be overwritten
         * when instantiating the atomic model.
        */

        /**
         * Constructor for Subnet class.
         * Initializes the state structure
         * to control the transmitting
        */
        Subnet() noexcept {
            state.transmiting     = false;
            state.index           = 0;
        }
                
        /**
         * Structure that holds the
         * state variables to check if it is in 
	 * transmitting state using transmiting variable
	 * and also which packet and what index value 
	 * is going to be send
        */
        struct state_type {
            bool transmiting;
            int packet;
            int index;
        }; 
        state_type state;
		
        // ports definition to initialize input and output ports
        using input_ports = std::tuple<typename defs::in>;
        using output_ports = std::tuple<typename defs::out>;

        /**
         * Function that performs the internal transition of the Subnet
         * It takes no input as parameter and changes the state of subnet
         * to passive or say turn off the subnet transmitting state.
        */
        void internal_transition() {
            state.transmiting = false;  
        }

        /**
         * Function that performs external transition.
         * In this function it retrives the message and
         * checks if the number of messages are more than 1
         * and if it is more that 1 it asserts giving the message
         * that only one message is expected per unit time.
         * Else it sets the message value to the packet that is
         * going to be send and set the state to transmitting
         * @param e time variable
         * @param mbs message bags
        */
        void external_transition(TIME e,
            typename make_message_bags<input_ports>::type mbs) { 
            state.index++;
            if (get_messages<typename defs::in>(mbs).size() > 1) {
	        assert(false && "One message at a time");     
            }				
            for (const auto &x : get_messages<typename defs::in>(mbs)) {
                state.packet = static_cast<int>(x.value);
                state.transmiting = true; 
            }               
        }

        /**
         * This function calls the internal transition function
         * followed by external transition function
         * @param e time variable
         * @param mbs message bags
        */
        void confluence_transition(TIME e,
            typename make_message_bags<input_ports>::type mbs) {
            internal_transition();
            external_transition(TIME(), std::move(mbs));
        }

        /**
         * Function that transmits the message to the output port
	 * The probability of packet getting transmitted is 95%
         * compared to packet being not sent that is 5%
         * @return message bags
        */
        typename make_message_bags<output_ports>::type output() const {
            typename make_message_bags<output_ports>::type bags;
            Message_t out;
            if ((double)rand() / (double) RAND_MAX  < 0.95) {
                out.value = state.packet;
                get_messages<typename defs::out>(bags).push_back(out);
            }
            return bags;
        }

        /**
         * Function sets next internal transmission time.
         * If the current state is transmitting then the next
         * internal time is set to value generated by distribution function
	 * that provides mean of 3.0 and standard deviation of 1.0
         * otherwise it is set to infinity
         * @return next internal time
        */
        TIME time_advance() const {
            std::default_random_engine generator;
            std::normal_distribution<double> distribution(3.0, 1.0); 
            TIME next_internal;
            if (state.transmiting) {
                std::initializer_list<int>
	            time = {0, 0, static_cast<int>
		        (round(distribution(generator)))};
                // time is hour min and second
                next_internal = TIME(time);
            }
            else {
                next_internal = std::numeric_limits<TIME>::infinity();
            }    
            return next_internal;
        }

        /**
         * Function that transmits the message to the ostring stream
         * @param os ostring stream
         * @param i state type
         * @return os ostring stream
         */
        friend std::ostringstream& operator<<(std::ostringstream& os,
            const typename Subnet<TIME>::state_type& i) {
            os << "index: " << i.index << " & transmiting: " << i.transmiting; 
            return os;
        }
};    

#endif // _SUBNET_CADMIUM_HPP_

//
// Created by shubh on 2019-08-15.
//

#ifndef __REPEATER_CADMIUM_H__
#define __REPEATER_CADMIUM_H__

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
* The Sender class sends out messages and receives acknowledges.
*/
template<typename TIME>
class Repeater {
    // putting definitions in context
    using defs = repeater_defs;
public:
    TIME PREPARATION_TIME;    /**< Constant that holds the time delay from acknowledge to output. */ //!<Time delay constant.

    /**
	* Constructor for Sender class.
	* Initializes the delay constants and state structure.
	*/
    Repeater() noexcept {
        PREPARATION_TIME = TIME("00:00:10");
        state.ack    = 0;
        state.packet = 0;
        //state.next_internal    = std::numeric_limits<TIME>::infinity();
        //state.model_active     = false;
    }

    /**
* Structure that holds the state variables.
*/
    struct state_type {
        bool ack;
        bool sending;
        int packet;
        int ack_packet;
        //bool model_active;
        //TIME next_internal;
    };
    state_type state;

    using input_ports = std::tuple<typename defs::packet_in,
        typename defs::ack_in>;
    using output_ports = std::tuple<typename defs::packet_sent_out,
        typename defs::ack_received_out>;

    void internal_transition() {
        state.sending = false;
        state.ack = false;
    }

    void external_transition(TIME e,
                             typename make_message_bags<input_ports>::type mbs) {
        if ((get_messages<typename defs::packet_in>(mbs).size()
            +get_messages<typename defs::ack_in>(mbs).size()) > 1) {
            assert(false && "One message at a time");
        }
        for (const auto &x : get_messages<typename defs::packet_in>(mbs)) {
            state.packet = static_cast<int>(x.value);
            state.sending = true;
            //state.next_internal=PREPARATION_TIME;
        }
        for (const auto &x : get_messages<typename defs::ack_in>(mbs)) {
            state.ack_packet = static_cast<int>(x.value);
            state.ack = true;
            //state.next_internal=PREPARATION_TIME;
        }
    }

    void confluence_transition(TIME e,
                               typename make_message_bags<input_ports>::type mbs) {
        internal_transition();
        external_transition(TIME(), std::move(mbs));
    }

    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
        Message_t out;
        if (state.sending) {
            out.value = state.packet;
            get_messages<typename defs::packet_sent_out>(bags).push_back(out);
        }
        else if(state.ack)
        {
            out.value = state.ack_packet;
            get_messages<typename defs::ack_received_out>(bags).push_back(out);
        }

        return bags;
    }

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

    friend std::ostringstream& operator<<(std::ostringstream& os,
                                          const typename Repeater<TIME>::state_type& i) {
        os << "packetNum: " << i.packet <<
           " & ackpacket: " << i.ack_packet;
        return os;
    }
};

#endif //ALTERNATEBITPROTOCOLSIMULATOR_REPEATER_CADMIUM_H

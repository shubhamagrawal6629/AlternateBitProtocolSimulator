/** \brief This main file implements the Receiver operation
 *
 * This file sets up the application generator that
 * taken in input as filepath where the output data will be stored
 * and then generated all the log data using the cadmium and
 * destimes library. It runs according to input provided
 * in the input file and it ends the simulation at 04:00:00:000 time.
 * It creates different models to seperate all the operations
 * that are performed.
 *
 */

#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>

#include <cadmium/modeling/coupling.hpp>
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/concept/coupled_model_assert.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_atomic.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/tuple_to_ostream.hpp>
#include <cadmium/logger/common_loggers.hpp>


#include "../../../lib/vendor/nd_time.hpp"
#include "../../../lib/vendor/iestream.hpp"

#include "../../../include/message.hpp"
#include "../../../include/receiver_cadmium.hpp"

#define RECEIVER_OUTPUTFILE_PATH "../test/data/receiver/receiver_test_output.txt"
#define RECEIVER_INPUTFILE_PATH "../test/data/receiver/receiver_input_test.txt"
using namespace std;

using hclock = chrono::high_resolution_clock;
using TIME = NDTime;


/**
 * Setting input ports for messages
*/
struct inp : public cadmium::in_port<Message_t> {};

/**
 * Setting output ports for messages
*/
struct outp : public cadmium::out_port<Message_t> {};


/**
 * This is class for application generator that takes
 * the file path as parameter and waits for input
 * @param T  message
 */
template<typename T>
class ApplicationGen : public iestream_input<Message_t,T> {
    public:
        ApplicationGen() = default;
        /**
         * Its a parameterized constructor for class application generator
         * that takes input the path of the file that has the input
         * for application to run
         * @param file_path
         */
        ApplicationGen(const char* file_path) :
                iestream_input<Message_t,T>(file_path) {
    }
};


int main() {
    //to measure simulation execution time
    auto start = hclock::now();

    /**
     * Generating the log of all the operation and messages
     * that are being passed during the execution of this application
     * and storing that in the file as stated in out_data
     */

    static std::ofstream out_data(RECEIVER_OUTPUTFILE_PATH);

    /**
     * This structure calls the ostream that is the output stream
     * and it return the data that has been stored in the file
     */
    struct oss_sink_provider {
        static std::ostream& sink() {
            return out_data;
        }
    };

    /**
     * Here cadmium and Destimes library functions are used
     * to generate the log files in a formatted way and to store
     * them in variables and later just log them together to the file
     */
    using
        info = cadmium::logger::logger<cadmium::logger::logger_info,
            cadmium::dynamic::logger::formatter<TIME>,
                oss_sink_provider>;
    using
        debug = cadmium::logger::logger<cadmium::logger::logger_debug,
            cadmium::dynamic::logger::formatter<TIME>,
                oss_sink_provider>;
    using
        state = cadmium::logger::logger<cadmium::logger::logger_state,
            cadmium::dynamic::logger::formatter<TIME>,
                oss_sink_provider>;
    using
        log_messages = cadmium::logger::logger<cadmium::logger::logger_messages,
            cadmium::dynamic::logger::formatter<TIME>,
                oss_sink_provider>;
    using
        routing = cadmium::logger::logger<cadmium::logger::logger_message_routing,
            cadmium::dynamic::logger::formatter<TIME>,
                oss_sink_provider>;
    using
        global_time = cadmium::logger::logger<cadmium::logger::logger_global_time,
            cadmium::dynamic::logger::formatter<TIME>,
                oss_sink_provider>;
    using
        local_time = cadmium::logger::logger<cadmium::logger::logger_local_time,
            cadmium::dynamic::logger::formatter<TIME>,
                oss_sink_provider>;
    using
        log_all = cadmium::logger::multilogger <info,
            debug, state, log_messages, routing, global_time, local_time>;

    using logger_top = cadmium::logger::multilogger<log_messages, global_time>;



    /**
     * It gets the input text file for execution of the simulation
     * process for the receiver
     */
    string input_data_control = RECEIVER_INPUTFILE_PATH;
    const char* i_input_data_control = input_data_control.c_str();


    /**
     * here the generator as been initialized that takes into
     * consideration the output file, Time and then according to input
     * generates the output
     */
    std::shared_ptr<cadmium::dynamic::modeling::model>
        generator = cadmium::dynamic::translate::make_dynamic_atomic_model
            <ApplicationGen, TIME, const char*>
                ("generator", std::move(i_input_data_control));

    /**
     * This helps in identifying the output data is coming from receiver1
     */

    std::shared_ptr<cadmium::dynamic::modeling::model>
        receiver1 = cadmium::dynamic::translate::make_dynamic_atomic_model
            <Receiver, TIME>("receiver1");


    /**
     * All these over here are to store the values for
     * operations that have been performed for a time frame
     * and then accordingly store in output file
     */
    cadmium::dynamic::modeling::Ports iports_TOP = {};
    cadmium::dynamic::modeling::Ports oports_TOP = {
        typeid(outp)
    };
    cadmium::dynamic::modeling::Models submodels_TOP = {
        generator, receiver1
    };
    cadmium::dynamic::modeling::EICs eics_TOP = {};
    cadmium::dynamic::modeling::EOCs eocs_TOP = {
        cadmium::dynamic::translate::make_EOC
            <receiver_defs::out, outp>("receiver1")
    };
    cadmium::dynamic::modeling::ICs ics_TOP = {
        cadmium::dynamic::translate::make_IC
            <iestream_input_defs<Message_t>::out,receiver_defs::in>
                ("generator","receiver1")
    };
    std::shared_ptr<cadmium::dynamic::modeling::coupled<TIME>> TOP =
        std::make_shared<cadmium::dynamic::modeling::coupled<TIME>>(
                                                                    "TOP",
                                                                    submodels_TOP,
                                                                    iports_TOP,
                                                                    oports_TOP,
                                                                    eics_TOP,
                                                                    eocs_TOP,
                                                                    ics_TOP);

    /**
     * In this model, runner are created and also the time to create
     * them are measured. Once runners are created simulation starts and
     * simulation runs until 04:00:00:000 time
     */

    auto elapsed1 = std::chrono::duration_cast<std::chrono::duration
        <double,std::ratio<1>>> (hclock::now() - start).count();
    cout<<"Model Created. Elapsed time: "<<elapsed1<<"sec"<<endl;

    cadmium::dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    elapsed1 = std::chrono::duration_cast<std::chrono::duration
        <double,std::ratio<1>>> (hclock::now() - start).count();
    cout<<"Runner Created. Elapsed time: "<<elapsed1<<"sec"<<endl;

    cout<<"Simulation starts"<<endl;

    r.run_until(NDTime("04:00:00:000"));
    auto elapsed = std::chrono::duration_cast<std::chrono::duration
        <double, std::ratio<1>>> (hclock::now() - start).count();
    cout<<"Simulation took:"<<elapsed<<"sec"<<endl;
    return 0;
}

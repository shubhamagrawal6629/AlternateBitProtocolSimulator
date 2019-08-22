# **ALTERNATE BIT PROTOCOL USING CADMIUM**
---
### ORGANIZATION
---
#### Carleton University
### AUTHORS
---
**Originally Developed by**: Dr. [Cristina Ruiz Martin](https://github.com/cruizm)<br>
**Modified by**: [Peter Bliznyuk-Kvitko](https://github.com/p-sky) and [Shubham Agrawal](https://github.com/shubhamagrawal6629)

### TABLE OF CONTENTS
---
- [INTRODUCTION](#INTRODUCTION)
- [FILES ORGANIZATION](#FILES-ORGANIZATION)
- [STEPS TO RUN THE SIMULATOR](#STEPS-TO-RUN-THE-SIMULATOR)

### INTRODUCTION
---
Alternating Bit Protocol(ABP) is a communication protocol to ensure reliable transmission through unreliable network. In ABP protocol sender sends a packet and waits for an acknowledgement. If the acknowledgement doesn't arrive within a predefined time, the sender re-sends this packet until it receives an expected acknowledgement and then sends the next packet. In order to distinguish two consecutive packets, the sender adds an additional bit on each packet (called alternating bit because the sender uses 0 and 1 alternatively).

This software is a simulator to replicate the behavior of the Alternating Bit Protocol. The ABP simulator consists of 3 components.

- **Sender** - It sends the packet and then waits for acknowledgement from the receiver but if it does not receive the acknowledgement within time window it will resend the packet with alternating bit attached to it so that it can differentiate between the two packets. Once it receives the acknowledgement it will send the next packet until it has no more packets to send.
- **Subnets** - It just transmits the packets to next node after a time delay.
- **Receiver** - It receives the data and sends back acknowledgement after certain period of time.

We have added a new module named Repeater that that takes input from subnet and after processing it send it to the receiver and get acknowledgement from receiver and forward it to sender after processing it.

Repeater has 2 inputs and 2 outputs port for packet and acknowledgement respectively. It adds a 10 second processing time to packets and send it to respective output ports

### FILES ORGANIZATION
---
##### data [This folder contains the data files for the simulator]
1. input
    -   input_abp_0.txt
    -    input_abp_1.txt
2. output
	-	abp_output.txt
	-	abp_output_0.txt
	-	abp_output_1.txt
	-	abp_proc.txt

##### doc [This folder contains the documentation for the project]
1. alternatebitprot.pdf
2. Cadmium_Documentation_Ubuntu.pdf
3. Cadmium_Windows.pdf
4. Doxygen_files.zip
5. Doxygen_files_receiver.zip
6. doxygen_html_cadmium_headers.zip
7. doxygen_html_receiver_sender.zip

##### include[This folder contains the header files]
1. file_process.hpp
2. message.hpp
3. receiver_cadmium.hpp
4. repeater_cadmium.hpp
5. sender_cadmium.hpp
6. subnet_cadmium.hpp

##### lib [This folder contains 3rd party libraries needed in the project]
1. cadmium[This folder contains cadmium library files as submodules]
2. DESTimes[This folder contains DesTimes library files as submodules]
3. iestream.hpp


##### src [This folder contains the source files written in c++ for the project]
1. file_process.cpp
2. main.cpp
3. message.cpp

##### test [This folder the unit test for the different include files]
1. data [This folder contains the data files for test folder]

   1.1.receiver
    -    receiver_input_test.txt
	-	receiver_test_output.txt
	- 	receiver_test_proc.txt

    1.2.sender
	-	sender_input_test_ack_In.txt
	-   sender_input_test_control_In.txt
	-   sender_test_output.txt
	-   sender_test_proc.txt

    1.3.subnet
	-   subnet_input_test.txt
	-   subnet_test_output.txt
	-   subnet_test_proc.txt
2. src [This folder contains the source files written in c++ for test folder]

    2.1.receiver

	-   main.cpp

    2.2.sender
	-   main.cpp

    2.3.subnet
	-   main.cpp

### STEPS TO RUN THE SIMULATOR
---
To refer working of Alternate Bit Protocol(ABP) in project look into alternatebitprot.pdf in the [document](https://github.com/shubhamagrawal6629/AlternateBitProtocolSimulator/tree/master/doc) folder.
There are some third party libraries that needs to be installed to run the simulator and steps to install those libraries are
given in [document](https://github.com/shubhamagrawal6629/AlternateBitProtocolSimulator/tree/master/doc) folder.

**1. To download the submodule files to use those libraries perform the following steps**
1. Clone the github repository using terminal or cmd using following command
>               git clone <link to repository>
2. If the repository is not the one that contains the submodules then make a pull request to that repository using
>               git pull origin [repository-name]
3. Once in the repository with submodules type in
>               git submodule update --init --recursive

**2. Update include path for cadmium in the makefile.**

In case If planning to have a different folder structure then update the relative path to cadmium/include in the makefile. You need to take into account where you copied the folder during the installation process. In our case it is in lib folder.
>               INCLUDECADMIUM=-I lib/cadmium/include

**3. Run the unit tests**

3.1. Run subnet test

1. Open the terminal.
2. To compile the test, type in the terminal:
>               make clean; make comp; make all
3. To run the test set the command prompt in the bin folder. To do so, type in the terminal the path to this folder.
>               Example: cd bin
4. Once inside the bin folder, type in the terminal **"./NAME_OF_THE_COMPILED_FILE"**. For this specific test you need to type:
>               ./SUBNET_TEST
5. For windows system type
>               ./SUBNET_TEST.exe
6. To check the output of the test, open  **"../test/data/subnet_test_output.txt"**
7. To check the modified ouput that is more understandable, open **"../test/data/subnet_test_proc.txt"**

3.2. To run receiver and sender tests, the steps are analogous to 2.1

**4. Run the simulator**

1. Open the terminal.
2. To compile the project, type in the terminal:
>               make clean; make comp; make all
3. To run the simulation set the command prompt in the bin folder. To do so, type in the terminal the path to this folder.
>               Example: cd bin
4. Once inside Bin folder, type in the terminal **"./NAME_OF_THE_COMPILED_FILE NAME_OF_THE_INPUT_FILE"**. For this test you need to type:
>               ./ABP ../data/input/input_abp_1.txt
5. And for windows system type
>               ./ABP.exe ../data/input/input_abp_1.txt
6. To check the output of the simulation, open  **"../data/output/abp_output.txt"**
7. To check the modified ouput that is more understandable, open  **"../data/output/abp_proc.txt"**
8.  To execute the simulator with different inputs
    1. Create new .txt files with the same structure as input_abp_0.txt or input_abp_1.txt
	2. Run the simulator using the instructions in step 4
	3. If you want to keep the output, rename abp_output.txt. To do so, move to the data/output folder by typing **"cd ../data/output"** in the terminal and then type :
>                       "mv abp_output.txt NEW_NAME"
>                       Example: mv abp_output.txt abp_output_0.txt

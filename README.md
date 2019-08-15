# **ALTERNATE BIT PROTOCOL USING CADMIUM**
---
### ORGANIZATION
---
#### Carleton University
### AUTHORS
---
**Originally Developed by**: Cristina Ruiz Martin<br/>
**Modified by**: Peter Bliznyuk-Kvitko and Shubham Agrawal
### INTRODUCTION
---
Alternating Bit Protocol(ABP) is a communication protocol to ensure reliable transmission through unreliable network. In ABP protocol sender sends a packet and waits for an acknowledgement. If the acknowledgement doesn't arrive within a predefined time, the sender re-sends this packet until it receives an expected acknowledgement and then sends the next packet. In order to distinguish two consecutive packets, the sender adds an additional bit on each packet (called alternating bit because the sender uses 0 and 1 alternatively).

This software is a simulator to replicate the behavior of the Alternating Bit Protocol. The ABP simulator consists of 3 components.

- **Receiver** - It receives the data and sends back acknowledgement after certain perios of time.
- **Subnets** - It just transmits the packets to next node after a time delay.
- **Sender** - It sends the packet and then waits for acknowledgement from the receiver but if it does not receive the acknowledgement within time window it will resend the packet with alternating bit attached to it so that it can differentiate between the two packets. Once it receives the acknowledgement it will swnd the next packet until it has no more packets to send.
### FILES ORGANIZATION
---
##### data [This folder contains the data files for the simulator]
1. input
    -   input_abp_0.txt
    -    input_abp_1.txt
2. output
	-	abp_output_0.txt
	-	abp_output_1.txt

##### doc [This folder contains the documentation for the project]
1. alternatebitprot.pdf
2. Cadmium_Documentation_Ubuntu.pdf
3. Cadmium_Windows.pdf

##### include[This folder contains the header files]
1. message.hpp
2. receiver_cadmium.hpp
3. sender_cadmium.hpp
4. subnet_cadmium.hpp

##### lib [This folder contains 3rd party libraries needed in the project]
1. cadmium-master[This folder contains cadmium library files]
2. DESTimes-master[This folder contains DesTimes library files]
3. vendor[this folder contains third party vendor files]
	-	iestream.hpp
	-	nd_time.hpp

##### src [This folder contains the source files written in c++ for the project]
1. main.cpp
2. message.cpp

##### test [This folder the unit test for the different include files]
1. data [This folder contains the data files for test folder]

   1.1.receiver
    -    receiver_input_test.txt
	-	receiver_test_output.txt

    1.2.sender
	-	sender_input_test_ack_In.txt
	-   sender_input_test_control_In.txt
	-   sender_test_output.txt

    1.3.subnet
	-   subnet_input_test.txt
	-   subnet_test_output.txt
2. src [This folder contains the source files written in c++ for test folder]

    2.1.receiver

	-   main.cpp

    2.2.sender
	-   main.cpp

    2.3.subnet
	-   main.cpp

### STEPS TO RUN THE SIMULATOR
---
Document alternatebitprot.pdf in the doc folder contains the explanation of this simulator and steps to install the cadmium and destimes library are
given in [document](https://github.com/shubhamagrawal6629/AlternateBitProtocolSimulator/tree/master/doc) folder.

1.Update include paths in all the makefiles in this folder and subfolders. You need to update the following lines:

1. INCLUDECADMIUM=-I lib/cadmium-master/include
2. Update the relative path to cadmium/include from the folder where the makefile is. You need to take into account where you copied the folder during the installation process
>               Example: INCLUDECADMIUM=-I lib/cadmium-master/include

2.Run the unit tests

2.1. Run subnet test

1. Open the terminal.
2. To compile the test, type in the terminal:
>               make clean; make comp; make all
3. To run the test set the command prompt in the bin folder. To do so, type in the terminal the path to this folder.
>               Example: cd bin
4. Once inside the bin folder, type in the terminal **"./NAME_OF_THE_COMPILED_FILE"**. For this specific test you need to type:
>               ./SUBNET_TEST
5. To check the output of the test, open  **"../test/data/subnet_test_output.txt"**

2.2. To run receiver and sender tests, the steps are analogous to 2.1

3 - Run the simulator

1. Open the terminal.
2. To compile the project, type in the terminal:
>               make clean; make comp; make all
3. To run the simulation set the command prompt in the bin folder. To do so, type in the terminal the path to this folder.
>               Example: cd bin
4. Once inside Bin folder, type in the terminal **"./NAME_OF_THE_COMPILED_FILE NAME_OF_THE_INPUT_FILE"**. For this test you need to type:
>               ./ABP ../data/input/input_abp_1.txt
5. To check the output of the simulation, open  **"../data/output/abp_output.txt"**
6.  To execute the simulator with different inputs
    1. Create new .txt files with the same structure as input_abp_0.txt or input_abp_1.txt
	2. Run the simulator using the instructions in step 4
	3. If you want to keep the output, rename abp_output.txt. To do so, move to the data/output folder by typing **"cd ../data/output"** in the terminal and then type :
>                       "mv abp_output.txt NEW_NAME"
>                       Example: mv abp_output.txt abp_output_0.txt



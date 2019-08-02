This folder contains a simulator for an ALTERNATE BIT PROTOCOL implemented in Cadmium

/**************************/
/****FILES ORGANIZATION****/
/**************************/

README.txt	

data [This folder contains the data files for the simulator]
	input
		input_abp_0.txt
		input_abp_1.txt
	output
		abp_output_0.txt
		abp_output_1.txt

doc [This folder contains the documentation for the project]
	alternatebitprot.pdf
	Cadmium_Documentation_Ubuntu.pdf
	Cadmium_Windows.pdf

include [This folder contains the header files]
	message.hpp
	receiver_cadmium.hpp
	sender_cadmium.hpp
	subnet_cadmium.hpp

lib [This folder contains 3rd party libraries needed in the project]
	cadmium-master
	DESTimes-master
	vendor
		iestream.hpp
		nd_time.hpp

src [This folder contains the source files written in c++ for the project]
	main.cpp
	message.cpp



test [This folder the unit test for the different include files]
	data [This folder contains the data files for test folder]
		receiver
			receiver_input_test.txt
			receiver_test_output.txt
		sender
			sender_input_test_ack_In.txt
			sender_input_test_control_In.txt
			sender_test_output.txt
		subnet
			subnet_input_test.txt
			subnet_test_output.txt
	src [This folder contains the source files written in c++ for test folder of the project]
		receiver
			main.cpp
		sender
			main.cpp
		subnet
			main.cpp


/*************/
/****STEPS****/
/*************/

0 - doc/alternatebitprot.pdf contains the explanation of this simulator

1 - Update include paths in all the makefiles in this folder and subfolders. You need to update the following lines:
	INCLUDECADMIUM=-I lib/cadmium-master/include
    Update the relative path to cadmium/include from the folder where the makefile is. You need to take into account where you copied the folder during the installation process
	Example: INCLUDECADMIUM=-I lib/cadmium-master/include

2 - Run the unit tests
	2.1. Run subnet test
		1 - Open the terminal.
		2 - To compile the test, type in the terminal:
        			make clean; make comp; make all
		2 - To run the test set the command prompt in the bin folder. To do so, type in the terminal the path to this folder.
			Example: cd bin
		4 - Once inside the bin folder, type in the terminal "./NAME_OF_THE_COMPILED_FILE". For this specific test you need to 
		    type:
			    ./SUBNET_TEST
		5 - To check the output of the test, open  "../test/data/subnet_test_output.txt"
	2.2. To run receiver and sender tests, the steps are analogous to 2.1

3 - Run the simulator
	1 - Open the terminal.
	3 - To compile the project, type in the terminal:
    		make clean; make comp; make all
	2 - To run the simulation set the command prompt in the bin folder. To do so, type in the terminal the path to this folder.
		Example: cd bin
	4 - Once inside Bin folder, type in the terminal "./NAME_OF_THE_COMPILED_FILE NAME_OF_THE_INPUT_FILE". For this test you need to 
	    type:
		    ./ABP ../data/input/input_abp_1.txt
	5 - To check the output of the simulation, open  "../data/output/abp_output.txt"
	6 - To execute the simulator with different inputs
		6.1. Create new .txt files with the same structure as input_abp_0.txt or input_abp_1.txt
		6.2. Run the simulator using the instructions in step 4
		6.3. If you want to keep the output, rename abp_output.txt. To do so, move to the data/output folder by typing
		        cd ../data/output
		     in the terminal and then type :
		        "mv abp_output.txt NEW_NAME"
        	 Example: mv abp_output.txt abp_output_0.txt


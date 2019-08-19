CC=g++
CFLAGS=-std=c++17

build_folder := $(shell mkdir -p build)
bin_folder := $(shell mkdir -p bin)

INCLUDECADMIUM=-I lib/cadmium/include

all: build/main.o build/main_r.o build/main_s.o build/main_n.o build/file_process.o
	$(CC) -g -o bin/ABP build/main.o build/message.o build/file_process.o
	$(CC) -g -o bin/SENDER_TEST build/main_s.o build/message.o build/file_process.o
	$(CC) -g -o bin/SUBNET_TEST build/main_n.o build/message.o build/file_process.o
	$(CC) -g -o bin/RECEIVER_TEST build/main_r.o build/message.o build/file_process.o

comp: main message file_proc main_s main_n main_r

main: src/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) src/main.cpp -o build/main.o
	
message: src/message.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) src/message.cpp -o build/message.o

file_proc: src/file_process.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) src/file_process.cpp -o build/file_process.o

main_s: test/src/sender/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/src/sender/main.cpp -o build/main_s.o
	
main_n: test/src/subnet/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/src/subnet/main.cpp -o build/main_n.o
	
main_r: test/src/receiver/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/src/receiver/main.cpp -o build/main_r.o
		
clean:
	rm -f bin/* build/*


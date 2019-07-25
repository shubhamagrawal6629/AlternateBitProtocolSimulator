CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I lib/cadmium-master/include

all:
	$(CC) -g -o bin/ABP build/main.o build/message.o 
	$(CC) -g -o bin/RECEIVER_TEST build/main_r.o build/message.o
	$(CC) -g -o bin/SENDER_TEST build/main_s.o build/message.o
	$(CC) -g -o bin/SUBNET_TEST build/main_n.o build/message.o

comp:
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) src/main.cpp -o build/main.o
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) src/message.cpp -o build/message.o
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/src/receiver/main.cpp -o build/main_r.o
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/src/sender/main.cpp -o build/main_s.o
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) test/src/subnet/main.cpp -o build/main_n.o

clean:
	rm -f bin/* build/*


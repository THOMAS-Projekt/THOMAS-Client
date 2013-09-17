CPPARGS= -std=c++11 -pthread
LINKERARGS= -pthread

all: main.o THOMASException.o TCPClient.o Joystick.o
	g++ THOMASException.o TCPClient.o Joystick.o main.o -o thomas-client $(LINKERARGS)

test: test.o THOMASException.o  TCPClient.o
	g++ THOMASException.o TCPClient.o test.o -o test $(LINKERARGS)

test.o: test.cpp
	g++ -c test.cpp $(CPPARGS)

main.o: main.cpp
	g++ -c main.cpp $(CPPARGS)

TCPClient.o: TCPClient.cpp TCPClient.h
	g++ -c TCPClient.cpp $(CPPARGS)

Joystick.o: Joystick.cpp Joystick.h
	g++ -c Joystick.cpp $(CPPARGS)

THOMASException.o: THOMASException.cpp THOMASException.h
	g++ -c THOMASException.cpp $(CPPARGS)

clean:
	rm -rf ./*.o thomas-client
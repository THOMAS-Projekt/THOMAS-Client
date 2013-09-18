#include <iostream>

#include "TCPClient.h"

int main()
{
	std::cout << "ich lebe!" << std::endl;
	
	THOMAS::TCPClient *c = new THOMAS::TCPClient();
	c->Connect("localhost", 4242);
	BYTE data1[3] = {1, 2, 0};
	c->Send(data1, 3);
	BYTE data2[5] = {2, 0, 255, 0, 255};
	c->Send(data2, 5);
	delete c;
	
	return 0;
}
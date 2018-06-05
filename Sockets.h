#pragma once
#ifndef CSOCKETS_H
#define CSOCKETS_H
#include "Packet.h"

class CSockets
{
public:
	CSockets(SOCKET _socket);
	~CSockets();
	

public:
	SOCKET sock;
	int receivePacketSize;
	int sendPacketSize;
	char receivedBuffer[PACKETBUFFERSIZE];
	int retVal;

};
#endif;


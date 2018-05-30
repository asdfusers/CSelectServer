#pragma once
#ifndef CSOCKETS_H
#define CSOCKETS_H
#include "Packet.h"
#include "RecvQue.h"
#include "SendQue.h"

class CSockets
{
public:
	CSockets(SOCKET _socket);
	~CSockets();
	CSockets(const CSockets& value);
public:
	SOCKET sock;
	int receivePacketSize;
	int sendPacketSize;
	char receivedBuffer[PACKETBUFFERSIZE];
	int retVal;

	CRecvQue recvQue;
	CSendQue sendQue;

};
#endif;


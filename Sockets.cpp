#include "stdafx.h"
#include "Sockets.h"
#include "Server.h"
#include "Protocol.h"

CSockets::CSockets(SOCKET _socket) : receivePacketSize(0), sendPacketSize(0), sock(_socket)
{
	recvQue.socket = sendQue.socket = _socket;
	
}


CSockets::~CSockets()
{
	
}

CSockets::CSockets(const CSockets & value)
{
	sock = value.sock;
	 receivePacketSize = value.receivePacketSize;
	 sendPacketSize = value.sendPacketSize;
	 receivedBuffer[PACKETBUFFERSIZE] = value.receivedBuffer[PACKETBUFFERSIZE];
	 retVal=value.retVal;

	recvQue.socket = value.sock;
	sendQue.socket = value.sock;
}




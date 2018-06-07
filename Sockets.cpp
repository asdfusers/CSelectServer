#include "stdafx.h"
#include "Sockets.h"
#include "Server.h"
#include "Protocol.h"

CSockets::CSockets(SOCKET _socket) : receivePacketSize(0), sendPacketSize(0), sock(_socket), iLevel(0), eStatus(Connect), iRoomNum(0)
{
}


CSockets::~CSockets()
{
	
}

CSockets & CSockets::operator=(CSockets & rhs)
{
	sock = rhs.sock;
	receivePacketSize = rhs.receivePacketSize;
	sendPacketSize = rhs.sendPacketSize;
	receivedBuffer[PACKETBUFFERSIZE] = rhs.receivedBuffer[PACKETBUFFERSIZE];
	retVal = rhs.retVal;
	ID = rhs.ID;
	return *this;
}





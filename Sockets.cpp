#include "stdafx.h"
#include "Sockets.h"
#include "Server.h"
#include "Protocol.h"

CSockets::CSockets() : receivePacketSize(0), sock(INVALID_SOCKET)
{

}


CSockets::~CSockets()
{
	
}

CSockets::CSockets(const CSockets & val)
{
	 sock = val.sock;
	 receivePacketSize = val.receivePacketSize;
	 sendPacketSize = val.sendPacketSize;
	 receivedBuffer[PACKETBUFFERSIZE] = val.receivedBuffer[PACKETBUFFERSIZE];
	 retVal = val.retVal;
	

}

void CSockets::Init()
{
	recvQue.SetSocket(sock);
	sendQue.SetSocket(sock);
}



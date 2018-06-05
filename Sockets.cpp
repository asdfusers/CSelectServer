#include "stdafx.h"
#include "Sockets.h"
#include "Server.h"
#include "Protocol.h"

CSockets::CSockets(SOCKET _socket) : receivePacketSize(0), sendPacketSize(0), sock(_socket)
{
}


CSockets::~CSockets()
{
	
}





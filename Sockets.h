#pragma once
#ifndef CSOCKETS_H
#define CSOCKETS_H
#include "Packet.h"

class CSockets
{
public:
	CSockets(SOCKET _socket);
	~CSockets();
	
	char* getID() { return ID; }
	int getLevel() { return iLevel; }
	int getRoomidx() { return iRoomNum; }
	Status getStatus() { return eStatus; }
	


	void setID(char* _ID){ 	ID = _ID; 	}
	void setiLevel(int _ilv) { iLevel = _ilv; }
	void setStatus(Status status) {	eStatus = status;	}
	void setRoomNum(int _room) { iRoomNum = _room; }
	CSockets& operator=(CSockets& rhs);

public:
	SOCKET sock;
	int receivePacketSize;
	int sendPacketSize;
	char receivedBuffer[PACKETBUFFERSIZE];
	int retVal;

private:
	char* ID;
	int iLevel;
	int iRoomNum;
	Status eStatus;

};
#endif;


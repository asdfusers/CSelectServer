#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include "Protocol.h"
#define PACKETBUFFERSIZE 8192
#define PACKETHEADERSIZE 4

class CPacket
{

public:
	CPacket();
	CPacket(unsigned short idValue);
	CPacket(const CPacket& source);
	virtual ~CPacket();

	bool isValidHeader();
	bool isValidPacket();
	CPacket& id(unsigned short ID);
	unsigned short id();

	unsigned short getDataFieldSize();
	int getPacketSize() { return (getDataFieldSize() + PACKETHEADERSIZE); }
	int getReceivedSize() { return receiveSize; }
	char* getPacketBuffer() { return packetBuffer; }

	void copyToBuffer(char* buff, int size);
	void clear();
	void WriteData(void* buffer, int size);
	void ReadData(void* buffer, int size);

	CPacket&	operator = (CPacket& packet);

	CPacket&	operator << (bool arg);
	CPacket&	operator << (int arg);
	CPacket&	operator << (long arg);
	CPacket&	operator << (DWORD arg);
	CPacket&	operator << (__int64 arg);
	CPacket&	operator << (LPTSTR arg);
	CPacket&	operator << (CPacket& arg);
	CPacket&	operator << (std::string arg);



	CPacket&	operator >> (bool& arg);
	CPacket&	operator >> (int& arg);
	CPacket&	operator >> (long& arg);
	CPacket&	operator >> (DWORD& arg);
	CPacket&	operator >> (__int64& arg);
	CPacket&	operator >> (LPTSTR arg);
	CPacket&	operator >> (CPacket& arg);
	CPacket&	operator >> (std::string arg);

	
protected:
	typedef struct
	{
		unsigned short* protocolID;
		unsigned short* dataSize;
	}HEADER;

	HEADER packetHeader;
private:
	char packetBuffer[PACKETBUFFERSIZE];
	char* dataField;
	char* endOfDataField;
	char* readPosition, *writePosition;
	int receiveSize;
};


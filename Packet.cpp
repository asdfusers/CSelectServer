#include "stdafx.h"
#include "Packet.h"

CPacket::CPacket(unsigned short idValue) : dataField(0), readPosition(0), writePosition(0), receiveSize(0)
{
	clear();
	id(idValue);
}

CPacket::CPacket(const CPacket & source) : dataField(0), readPosition(0), writePosition(0), receiveSize(0)
{
	clear();
	memcpy(packetBuffer, source.packetBuffer, PACKETBUFFERSIZE);
	receiveSize = source.receiveSize;

	DWORD offset;
	offset = (DWORD)source.readPosition - (DWORD)source.dataField;
	readPosition += offset;

	offset = (DWORD)source.writePosition - (DWORD)source.dataField;
	writePosition += offset;

}

CPacket::CPacket() : dataField(0), readPosition(0), writePosition(0), receiveSize(0)
{
	clear();
}


CPacket::~CPacket()
{
}

bool CPacket::isValidHeader()
{
	return (getPacketSize() >= PACKETHEADERSIZE);
}

bool CPacket::isValidPacket()
{
	if (isValidHeader() == false)
		return false;

	return (getPacketSize() >= receiveSize);
}

CPacket & CPacket::id(unsigned short ID)
{
	*packetHeader.protocolID = ID;

	return *this;
}

unsigned short CPacket::id()
{
	return *packetHeader.protocolID;
}

unsigned short CPacket::getDataFieldSize()
{
	return *packetHeader.dataSize;
}

void CPacket::copyToBuffer(char * buff, int size)
{
	clear();
	memcpy(packetBuffer, buff, size);
	receiveSize += size;
}

void CPacket::clear()
{
	ZeroMemory(packetBuffer, PACKETBUFFERSIZE);
	packetHeader.dataSize = (unsigned short*)(packetBuffer + 0);
	packetHeader.protocolID = (unsigned short*)(packetBuffer + 2);
	dataField = &packetBuffer[4];
	readPosition = writePosition = dataField;
	endOfDataField = &dataField[PACKETBUFFERSIZE - 1];
}

void CPacket::WriteData(void * buffer, int size)
{
	memcpy(writePosition, buffer, size);
	writePosition += size;
	receiveSize += size;
	*packetHeader.dataSize += size;

}

void CPacket::ReadData(void * buffer, int size)
{
	memcpy(buffer, readPosition, size);
	readPosition += size;
}

CPacket & CPacket::operator=(CPacket & packet)
{
	memcpy(dataField, packet.dataField, packet.getDataFieldSize());

	*packetHeader.protocolID = packet.id();
	*packetHeader.dataSize = packet.getDataFieldSize();

	return *this;
}

CPacket & CPacket::operator<<(bool arg)
{
	WriteData(&arg, sizeof(bool));

	return *this;
}

CPacket & CPacket::operator<<(int arg)
{
	WriteData(&arg, sizeof(int));

	return *this;
}

CPacket & CPacket::operator<<(long arg)
{
	WriteData(&arg, sizeof(long));

	return *this;
}

CPacket & CPacket::operator<<(DWORD arg)
{
	WriteData(&arg, sizeof(DWORD));

	return *this;
}

CPacket & CPacket::operator<<(__int64 arg)
{
	WriteData(&arg, sizeof(__int64));

	return *this;
}

CPacket &CPacket::operator<<(LPTSTR arg)
{
	char* pStr;
	int strSize = WideCharToMultiByte(CP_ACP, 0, arg, -1, NULL, 0, NULL, NULL);
	pStr = new char[strSize];
	WideCharToMultiByte(CP_ACP, 0, arg, -1, pStr, strSize, 0, 0);

	WriteData(pStr, strSize);

	return *this;
}

CPacket & CPacket::operator<<(CPacket & arg)
{
	unsigned int idValue = arg.id();
	unsigned int size = arg.getDataFieldSize();

	WriteData(&idValue, sizeof(unsigned int));
	WriteData(&size, sizeof(unsigned int));
	WriteData(arg.dataField, size);

	return *this;
}

CPacket & CPacket::operator<<(std::string arg)
{
	int length = sizeof(arg);
	WriteData(&arg, length);

	return *this;
}



CPacket & CPacket::operator >> (bool & arg)
{
	ReadData(&arg, sizeof(bool));

	return *this;
}

CPacket & CPacket::operator >> (int & arg)
{
	ReadData(&arg, sizeof(int));

	return *this;
}

CPacket & CPacket::operator >> (long & arg)
{
	ReadData(&arg, sizeof(long));

	return *this;
}

CPacket & CPacket::operator >> (DWORD & arg)
{
	ReadData(&arg, sizeof(DWORD));

	return *this;
}

CPacket & CPacket::operator >> (__int64 & arg)
{
	ReadData(&arg, sizeof(__int64));

	return *this;
}

CPacket & CPacket::operator >> (LPTSTR arg)
{
	
	ReadData(arg, lstrlen((LPTSTR)readPosition) * sizeof(wchar_t) + sizeof(wchar_t));

	return *this;
}

CPacket & CPacket::operator >> (CPacket & arg)
{
	int idValue, size;
	char buffer[PACKETBUFFERSIZE];

	ReadData(&idValue, sizeof(int));
	ReadData(&size, sizeof(int));

	ReadData(buffer, size);

	arg.id(idValue);
	arg.WriteData(buffer, size);

	return *this;
}

CPacket & CPacket::operator >> (std::string arg)
{
	int length = sizeof(arg);
	ReadData(&arg, length);

	return *this;


}

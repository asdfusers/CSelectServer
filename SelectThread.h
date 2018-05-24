#pragma once
#ifndef CSELECTTHREAD_H
#define CSELECTTHREAD_H
#include "Thread.h"
#include "Packet.h"
class CSelectThread :
	public CThread
{
public:
	CSelectThread();
	~CSelectThread();

public:
	virtual void threadMain();
	void SetSocket(SOCKET _sock) { mListen = _sock; }
	bool RemoveSocketInfo(int nIndex);


	bool onReceive(int idx);
	bool sendMessage(CPacket packet, int idx);

	void packetParsing(CPacket& packet, int idx);


	void	onPTTestPacket1Req(CPacket& packet, int idx);
	void	onPTTestPacket2Req(CPacket& packet, int idx);
	void	onPTTestPacket3Req(CPacket& packet, int idx);

private:
	FD_SET rset;
	FD_SET wset;
	SOCKET mListen;
	SOCKADDR_IN clientAddr;

	int retVal;
};
#endif


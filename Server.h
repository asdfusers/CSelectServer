#pragma once
#ifndef CSERVER_H
#define CSERVER_H

#include "stdafx.h"
#include "Sockets.h"
#include "AcceptThread.h"
#include "SelectThread.h"
#include "CriticalSections.h"
class CServer
{
public:
	CServer();
	~CServer();


public:
	void Init(int PORT);
	void UnInit();
	SOCKET getListenSocket() { return mListen; }
	void CopySocketList();

	void Update();
	bool sendMessage(CPacket packet);
	bool broadCast(CPacket packet);
	void packetParsing(CPacket packet);
	void onLoginPacket1Req(CPacket & packet);
	void onPSelectLobbyOption(CPacket & packet);
	void onPPlayerEnterRoom(CPacket & packet);


	void DeleteUserPool(SOCKET socket);
	void ChoiceLobbyOption(int iNum, SOCKET socket);
	void ViewUserInformation(CSockets User);
private:
	SOCKET mListen;
	SOCKADDR_IN sockAddr;
	int addrLen;
	int retVal;
	CMessageQue sendMessageQue;

	CAcceptThread _AcceptThread;
	CSelectThread _SelectThread;
	CS::CriticalSection m_cs;

};

#endif

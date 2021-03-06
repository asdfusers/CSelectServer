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
	bool broadCastInRoom(CPacket packet);
	void packetParsing(CPacket packet);
	void onLoginPacket1Req(CPacket & packet);
	void onPSelectLobbyOption(CPacket & packet);
	void onPPlayerEnterRoom(CPacket & packet);
	void onPBroadCastEnterRoom(CPacket & packet);
	void onPReadyReq(CPacket & packet);
	void onPReadyResult(CPacket & packet);
	void onPGameStartReady(CPacket & packet);
	void onPGameStart(CPacket & packet);
	void onPGameInputKey(CPacket & packet);


	bool playerPositionSetting(std::string cInputKey,CPosition pos ,CPacket & packet);
	void DeleteUserPool(SOCKET socket);
	void ChoiceLobbyOption(wchar_t iNum[10], SOCKET socket);
	std::string ViewUserInformation(CSockets User);
	std::string VIewUserStatuInformation(CSockets User);

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

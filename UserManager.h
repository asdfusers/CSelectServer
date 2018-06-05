#pragma once
#include "stdafx.h"
#include "Sockets.h"

class CUserManager
{
public:
	static CUserManager* getInst();
	static void releaseInst();

private:
	static  CUserManager* m_pInst;


public:
	void insertUser(CSockets client);
	void deleteUser(CSockets client);
	CSockets findUser(SOCKET socket);

public:
	std::map<SOCKET, CSockets> clientPool;
};




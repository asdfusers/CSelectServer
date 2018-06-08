#pragma once
#include "stdafx.h"
#include "Sockets.h"
class CRoom
{
public:
	CRoom();
	~CRoom();

public:
		virtual void insertUserPool(CSockets User);
		virtual void deleteUserPool(CSockets User) = 0;
		virtual CSockets findUserPool(SOCKET socket) = 0;

};


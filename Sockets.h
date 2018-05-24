#pragma once
#ifndef CSOCKETS_H
#define CSOCKETS_H


class CSockets
{
public:
	CSockets();
	~CSockets();

public:
	SOCKET sock;
	int recvBytes;
	int sendBytes;
	char buffer[MAX_BUFFER_SIZE];
};
#endif;


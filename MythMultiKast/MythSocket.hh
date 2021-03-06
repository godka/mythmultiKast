#pragma once
#include "MythConfig.hh"
#include <string.h>
class MythSocket
{
public:
	int socket_SendStr(const char* str, int length = -2);
	int socket_ReceiveData(char* recvBuf, int recvLength, int timeout = 500);
	int socket_CloseSocket(int handle = 0);
	int socket_ReceiveDataLn2(char* recvBuf, int recvLength, char* lnstr);
	static MythSocket* CreateNew(const char* ip, int port){
		return new MythSocket(ip, port);
	}
	static MythSocket* CreateNew(){
		return new MythSocket();
	}
	void generateSock(TCPsocket msock);
	~MythSocket();
	int active;
	int hasclosed;
	int isPush;
	TCPsocket sock;
	IPaddress peer;
	void* addtionaldata;
	void* data;
	void* server;
private:
	bool isrunning;
	//char* downbuffer;
	int downlength;
	int maxlength;
	SDLNet_SocketSet socketset;
	int socket_strcmp(char* buff, char*str, int length);
protected:
	MythSocket(const char* ip, int port);
	MythSocket();
};


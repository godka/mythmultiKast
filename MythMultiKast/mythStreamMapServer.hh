#pragma once
#include "MythConfig.hh"
#include "mythStreamServer.hh"
#include "mythBaseClient.hh"
#include <map>
#include <iostream>
using namespace std;
class mythStreamMapServer :
	public mythVirtualServer//,public mythVirtualSqlite
{
public:
	map<int,mythStreamServer*> servermap;
	map<int, int> servercount;
	static mythStreamMapServer* CreateNew(int port);
	void ServerDecodeCallBack(MythSocket* people, char* data, int datalength);
	void ServerCloseCallBack(MythSocket* people);
	string showAllClients();
	int startAll(void);

#ifdef MYTH_STREAM_CLOSE
	static Uint32 TimerCallbackStatic(Uint32 interval, void *param);
	Uint32 TimerCallback(Uint32 interval);
#endif
	~mythStreamMapServer(void);
protected:

#ifdef MYTH_STREAM_CLOSE
	SDL_TimerID timerid;
#endif
	mythStreamMapServer(int port);
private:
	SDL_mutex* mapmutex;
};


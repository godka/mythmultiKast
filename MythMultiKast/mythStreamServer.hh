#pragma once
#include "MythConfig.hh"
#include "mythVirtualDecoder.hh"
#include "mythProxyDecoder.hh"
#include "mythVirtualServer.hh"
#include "mythStreamDecoder.hh"
#include "mythBaseClient.hh"
#include <vector>
using namespace std;
class mythStreamServer 
{
public:
	int getClientNumber();
	int AppendClient(mythBaseClient* client);
	int DropClient(mythBaseClient* client);
	//char* getTop( int* length );
	static int mainthreadstatic(void* data);
	int mainthread();
	static mythStreamServer* CreateNew(int cameraid,void* args = NULL);
	//mythStreamServer* CreateNew(int cameraid, void* args);
	~mythStreamServer(void);
	mythVirtualDecoder* GetDecoder(){
		return decoder;
	}
	/*
	void AddClient(PEOPLE* people);
	void DelClient(PEOPLE* people);
	int GetClientNumber();
	void ClearAllClients();
	*/
	int start(bool canthread = true);
	int stop();
	int m_cameraid;
private:
	vector<mythBaseClient*> baselist;
	int ClientNumber;
	void connect();
protected:
	mythVirtualDecoder* decoder;
	//vector<mythBaseClient*> mpeople;
	//PeopleMap mpeople;
	int PeopleAdd;
	mythStreamServer(int cameraid, void* args = NULL);
	bool FindClient(vector <mythBaseClient*>::iterator beg, vector <mythBaseClient*>::iterator end, mythBaseClient* ival);
	SDL_mutex* numbermutex;
	SDL_mutex* streamservermutex;
	SDL_Thread* streamserverthread;
	SDL_mutex* decodemutex;
	int isrunning;
	char* topchar;
	int toplength;
	void* additionalargs;
};


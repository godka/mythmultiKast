﻿/********************************************************************
Created by MythKAst
©2013 MythKAst Some rights reserved.


You can build it with vc2010,gcc.
Anybody who gets this source code is able to modify or rebuild it anyway,
but please keep this section when you want to spread a new version.
It's strongly not recommended to change the original copyright. Adding new version
information, however, is Allowed. Thanks.
For the latest version, please be sure to check my website:
Http://code.google.com/mythkast


你可以用vc2010,gcc编译这些代码
任何得到此代码的人都可以修改或者重新编译这段代码，但是请保留这段文字。
请不要修改原始版权，但是可以添加新的版本信息。
最新版本请留意：Http://code.google.com/mythkast
B
MythKAst(asdic182@sina.com), in 2013 June.
*********************************************************************/
#include "mythStreamMapServer.hh"
//#define AUTOSTART
#ifndef WIN32
#include <unistd.h>  
#endif
//#include "mythUdp.hh"
mythStreamMapServer::mythStreamMapServer(int port)
:mythVirtualServer(port)//, mythVirtualSqlite()
{
	char tmpip[256] = { 0 };
	int start = read_profile_int("config", "autostart", 0 , MYTH_INFORMATIONINI_FILE);
	if (start || MYTH_FORCE_AUTOSTART){
		startAll();
	}
	mapmutex = SDL_CreateMutex();
#ifdef MYTH_STREAM_CLOSE
	this->timerid = SDL_AddTimer(1000, TimerCallbackStatic, this);
#endif
}
int mythStreamMapServer::startAll(void){
	return 0;
}
mythStreamMapServer::~mythStreamMapServer(void)
{
#ifdef MYTH_STREAM_CLOSE
	SDL_RemoveTimer(timerid);
#endif
}

mythStreamMapServer* mythStreamMapServer::CreateNew(int port)
{
	return new mythStreamMapServer(port);
}

void mythStreamMapServer::ServerDecodeCallBack(MythSocket* people, char* data, int datalength)
{
	if (strstr(data, "GET /list")){
		string tmp = showAllClients();
		/*
		HTTP/1.1 200 OK
		Date: Sat, 31 Dec 2005 23:59:59 GMT
		Content-Type: text/html;charset=ISO-8859-1
		Content-Length: 122
		*/
		//int len = tmp.length();
		//char header[256] = { 0 };
		//sprintf(header, "HTTP/1.1 200 OK\r\nDate: Sat, 31 Dec 2005 23:59:59 GMT\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n", len);
		//people->socket_SendStr(header);
		people->socket_SendStr(tmp.c_str(),tmp.length());
		closePeople(people);
		//people->socket_CloseSocket(1);
		return;
	}
	map<int,mythStreamServer*>::iterator Iter;
	int cameraid = -1;
	char cameratype[20] = { 0 };
	SDL_sscanf(data,"GET /CameraID=%d&Type=%s ",&cameraid,cameratype);
	if(cameraid != -1){
		mythStreamServer* server = NULL;
		//find cameraid from map
		Iter = servermap.find(cameraid);
		if(Iter == servermap.end() || Iter->second == NULL){
		//if (servermap[cameraid] != NULL){
			SDL_LockMutex(mapmutex);
			server = mythStreamServer::CreateNew(cameraid);			//add a new server into map list,not found ,so create 
			servermap[cameraid] = server;
			SDL_UnlockMutex(mapmutex);
			server->start();
		}
		else{
			SDL_LockMutex(mapmutex);
			server = Iter->second;									//find an existing server from map list,then add client into server list
			SDL_UnlockMutex(mapmutex);
		}
		mythBaseClient* client = NULL;
		if(!people->addtionaldata){
			SDL_LockMutex(mapmutex);
			int usingthread = read_profile_int("config", "usethread", 0, MYTH_INFORMATIONINI_FILE);
			client = mythBaseClient::CreateNew(people, usingthread,cameratype);
			people->data = server;
			people->addtionaldata = client;
			people->server = this;
			server->AppendClient(client);
			SDL_UnlockMutex(mapmutex);
		}
	}
	else{
		SDL_sscanf(data, "PUT /CameraID=%d", &cameraid);
		if (cameraid != -1){
			people->isPush = 1;
			mythStreamServer* server = NULL;
			//find cameraid from map
			Iter = servermap.find(cameraid);
			if (Iter == servermap.end() || Iter->second == NULL){
				//if (servermap[cameraid] != NULL){
				SDL_LockMutex(mapmutex);
				server = mythStreamServer::CreateNew(cameraid,people);			//add a new server into map list,not found ,so create 
				servermap[cameraid] = server;
				SDL_UnlockMutex(mapmutex);
				server->start();
			}
			else{
				SDL_LockMutex(mapmutex);
				server = Iter->second;									//find an existing server from map list,then add client into server list
				if (server){
					((mythProxyDecoder*) server->GetDecoder())->refreshSocket(people);	//problem
				}
				SDL_UnlockMutex(mapmutex);
			}
			/*mythBaseClient* client = NULL;
			if (!people->addtionaldata){
			SDL_LockMutex(mapmutex);
			int usingthread = read_profile_int("config", "usethread", 0, MYTH_INFORMATIONINI_FILE);
			if (usingthread == 1)
			client = mythBaseClient::CreateNew(people, true);
			else
			client = mythBaseClient::CreateNew(people, false);
			people->data = server;
			people->addtionaldata = client;
			server->AppendClient(client);
			SDL_UnlockMutex(mapmutex);
			}*/
		}
		else{
			people->socket_SendStr("404");
			closePeople(people);
			//ServerCloseCallBack(people);
		}
	}
	return;
}

string mythStreamMapServer::showAllClients(){
	char tmp[20] = { 0 };
	int sum = 0; 
	int clientnum = 0;
	string str = "alive:";
	for (map<int, mythStreamServer*>::iterator Iter = servermap.begin(); Iter != servermap.end(); Iter++){
		if (Iter->second){
			int t = Iter->second->m_cameraid;
			SDL_snprintf(tmp, 20, "%d", t);
			str += tmp;
			str += ";";
		}
	}
	return str;
}

void mythStreamMapServer::ServerCloseCallBack(MythSocket* people)
{
	if (people->addtionaldata){
		SDL_LockMutex(mapmutex);
		mythBaseClient* client = (mythBaseClient*)people->addtionaldata;
		mythStreamServer* server = (mythStreamServer*) people->data;
		//people->sock = NULL;
		server->DropClient(client);
		delete client;
		client = NULL;
		SDL_UnlockMutex(mapmutex);
	}
	return;
}
//this will make a fatal error if using stream close,unknown bug
#ifdef MYTH_STREAM_CLOSE
Uint32 mythStreamMapServer::TimerCallbackStatic(Uint32 interval, void *param)
{
	mythStreamMapServer* mapserver = (mythStreamMapServer*) param;
	return mapserver->TimerCallback(interval);
}

Uint32 mythStreamMapServer::TimerCallback(Uint32 interval)
{
	char filename[256] = { 0 };
	read_profile_string("config", "listtxt", filename, 256, "clients.txt", MYTH_INFORMATIONINI_FILE);
	string tmp = showAllClients();
	FILE* file = fopen(filename, "w");
	if (file){
		fprintf(file, tmp.c_str());
		fclose(file);
	}
	return interval;
}
#endif
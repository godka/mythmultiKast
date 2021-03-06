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
#include "MythConfig.hh"
#include "mythStreamMapServer.hh"
//char* global_filename;
/*
int checkArgs(int args, char** argv){
	for (int i = 1; i < args; i++){
		if (SDL_strcmp(argv[i], "--autostart") == 0){
			if (i < args - 1){
				
			}
		}
		else{
			mythVirtualSqlite::GetInstance()->SetSQLIP(argv[1]);
			SDL_Log("sqlip is now changed to %s\n", argv[1]);
		}
	}
	return 0;
}
*/
int main(int args, char** argv)
{
	SDL_Init(SDL_INIT_EVENTS);
	SDLNet_Init();
	mythStreamMapServer* streammapserver = mythStreamMapServer::CreateNew(streamserverport);
	streammapserver->StartServer();
#ifdef _DEBUG
	char input[256];
	for (;;){
		printf(">");
		gets(input);
		if (mythcmp("exit")){
			break;
		}
		else if (mythcmp("show")){
			streammapserver->showAllClients();
		}
		else if (mythcmp("stop")){
			streammapserver->StopServer();
		}
		else if (mythcmp("startall")){
			streammapserver->startAll();
		}
		else{
			system(input);
		}
	}
#endif
	streammapserver->StopServer();
	delete streammapserver;
	SDL_Quit();
	return 0;
}

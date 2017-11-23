#pragma once
#include "Global.h"

DWORD WINAPI EchoServer(LPVOID lParam)
{	
	DWORD rc = 0;
	Contact *client = (Contact*)lParam;
	
	QueueUserAPC(ASStartMessage, client->hAcceptServer, (DWORD)client);

	client->sthread = Contact::WORK;

	CancelWaitableTimer(client->htimer);
	client->sthread = Contact::FINISH;

	QueueUserAPC(ASFinishMessage, client->hAcceptServer, (DWORD)client);
	
	ExitThread(rc);
}
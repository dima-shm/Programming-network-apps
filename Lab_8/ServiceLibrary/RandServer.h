#pragma once
#include "Global.h"

DWORD WINAPI RandServer(LPVOID lParam)
{
	DWORD rc = 0;
	Contact *client = (Contact*)lParam;

	QueueUserAPC(ASStartMessage, client->hAcceptServer, (DWORD)client);

	client->sthread = Contact::WORK;

	srand(time(NULL));
	_itoa_s(rand(), client->msg, 10);

	Sleep(15000);
	CancelWaitableTimer(client->htimer);
	client->sthread = Contact::FINISH;

	QueueUserAPC(ASFinishMessage, client->hAcceptServer, (DWORD)client);

	ExitThread(rc);
}
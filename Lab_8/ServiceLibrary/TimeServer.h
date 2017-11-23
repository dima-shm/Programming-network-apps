#pragma once
#include "Global.h"

DWORD WINAPI TimeServer(LPVOID lParam)
{
	DWORD rc = 0;
	Contact *client = (Contact*)lParam;

	QueueUserAPC(ASStartMessage, client->hAcceptServer, (DWORD)client);

	client->sthread = Contact::WORK;

	SYSTEMTIME stime;
	GetLocalTime(&stime);
	sprintf_s(client->msg, "%d.%d.%d %d:%02d:%d", stime.wDay, stime.wMonth, stime.wYear, stime.wHour, stime.wMinute, stime.wSecond);

	CancelWaitableTimer(client->htimer);
	client->sthread = Contact::FINISH;

	QueueUserAPC(ASFinishMessage, client->hAcceptServer, (DWORD)client);

	ExitThread(rc);
}
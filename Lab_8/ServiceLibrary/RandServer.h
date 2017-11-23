#pragma once
#include "Global.h"

DWORD WINAPI RandServer(LPVOID lParam)
{
	DWORD rc = 0;
	Contact *client = (Contact*)lParam;

	QueueUserAPC(ASStartMessage, client->hAcceptServer, (DWORD)client);

	try
	{
		client->sthread = Contact::WORK;
		char ibuf[50];

		while (client->TimerOff == false)
		{
			if (recv(client->s, ibuf, sizeof(ibuf), NULL) == SOCKET_ERROR)
				if (WSAGetLastError() == WSAEWOULDBLOCK)
					Sleep(100);
				else
					throw SetErrorMsgText("recv: ", WSAGetLastError());
			else
			{
				cout << "ibuf: " <<  ibuf << endl;
				if (strcmp(ibuf, "Rand") == 0)
				{
					cout << "strcmp" << endl;

					srand(time(NULL));
					int randNumber = rand();
					_itoa_s(randNumber, ibuf, 10);
					
					if ((send(client->s, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR)
						throw SetErrorMsgText("send: ", WSAGetLastError());
				}
				else
					break;
			}
		}

		if (client->TimerOff == false)
		{
			CancelWaitableTimer(client->htimer);
			client->sthread = Contact::FINISH;
			QueueUserAPC(ASFinishMessage, client->hAcceptServer, (DWORD)client);
		}
	}
	catch (string errorMsgText)
	{
		CancelWaitableTimer(client->htimer);
		cout << errorMsgText << endl;
		client->sthread = Contact::ABORT;
	}

	ExitThread(rc);
}
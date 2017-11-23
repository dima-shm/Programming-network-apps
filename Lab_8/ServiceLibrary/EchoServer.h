#pragma once
#include "Global.h"

DWORD WINAPI EchoServer(LPVOID lParam)
{	
	DWORD rc = 0;
	Contact *client = (Contact*)lParam;
	
	QueueUserAPC(ASStartMessage, client->hAcceptServer, (DWORD)client);

	try
	{
		client->sthread = Contact::WORK;
		int  bytes = 1;
		char buf[50];
		
		while(client->TimerOff == false)
		{
			if((bytes = recv(client->s, buf, sizeof(buf), NULL)) == SOCKET_ERROR)
			{
				if (WSAGetLastError() == WSAEWOULDBLOCK)
					Sleep(100);
				else
					throw SetErrorMsgText("recv: ", WSAGetLastError());
			}
			else
			{
				if(strcmp(buf, "exit") != 0)
				{
					if (client->TimerOff != false)
						break;
					
					if((send(client->s, buf, sizeof(buf), NULL)) == SOCKET_ERROR)
						throw SetErrorMsgText("send: ", WSAGetLastError());
				}
				else
					break;
			}
		}

		if(client->TimerOff == false) 
		{
			CancelWaitableTimer(client->htimer);

			if((send(client->s, "EchoServer завершил работу", sizeof("EchoServer завершил работу"), NULL)) == SOCKET_ERROR)
				throw SetErrorMsgText("send: ", WSAGetLastError());

			client->sthread = Contact::FINISH;
			QueueUserAPC(ASFinishMessage, client->hAcceptServer, (DWORD)client);
		}
	}
	catch(string errorMsgText)
	{
		std::cout << errorMsgText << std::endl;
		CancelWaitableTimer(client->htimer);
		client->sthread = Contact::ABORT;
	}
	
	ExitThread(rc);
}
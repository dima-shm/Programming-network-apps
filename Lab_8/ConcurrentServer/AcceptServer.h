#pragma once
#include "Global.h"

static void WaitClients()
{
	bool ListEmpty = false;
	while (!ListEmpty)
	{
		EnterCriticalSection(&scListContact);
		ListEmpty = listContacts.empty();
		LeaveCriticalSection(&scListContact);
		SleepEx(0, TRUE);
	}
}

bool AcceptCycle(int squirt, SOCKET* s)
{
	bool rc = false;
	Contact c(Contact::ACCEPT, "AcceptServer");
	c.hAcceptServer = hAcceptServer;

	if ((c.s = accept(*s, (sockaddr*)&c.prms, &c.lprms)) == INVALID_SOCKET)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
			throw SetErrorMsgText("Accept: ", WSAGetLastError());
	}
	else
	{
		rc = true;
		InterlockedIncrement(&Accept);
		InterlockedIncrement(&Work);
		EnterCriticalSection(&scListContact);
		listContacts.push_front(c);
		LeaveCriticalSection(&scListContact);
		SetEvent(Event);
	}
	return rc;
};

void CommandsCycle(TalkersCmd& cmd, SOCKET* s)
{
	int squirt = 0;
	while (cmd != Exit)
	{
		switch (cmd)
		{
		case Start:	cmd = Getcommand; squirt = AS_SQUIRT; break;
		case Stop:	cmd = Getcommand; squirt = 0; break;
		case Wait:	   WaitClients(); cmd = Getcommand; squirt = AS_SQUIRT; break;
		case Shutdown: WaitClients(); cmd = Exit; break;
		};

		if (cmd != Exit && squirt>Work)
		{
			if (AcceptCycle(squirt, s))
			{
				cmd = Getcommand;
			}

			SleepEx(0, TRUE);
		}
	}
};

DWORD WINAPI AcceptServer(LPVOID pPrm)
{
	cout << "AcceptServer запущен" << endl;

	DWORD rc = 0;
	WSADATA wsaData;
	SOCKET sS;
	SOCKADDR_IN serv;
	serv.sin_family = AF_INET;
	serv.sin_port = htons(tport);
	serv.sin_addr.s_addr = ADDR_ANY;

	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("startup: ", WSAGetLastError());

		if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket: ", WSAGetLastError());

		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw SetErrorMsgText("bind: ", WSAGetLastError());

		if (listen(sS, SOMAXCONN) == SOCKET_ERROR)
			throw SetErrorMsgText("listen: ", WSAGetLastError());

		// Переключения сокета в режим без блокировки
		u_long nonblk = 1;
		if (ioctlsocket(sS, FIONBIO, &(nonblk)) == SOCKET_ERROR)
			throw SetErrorMsgText("ioctlsocket: ", WSAGetLastError());

		TalkersCmd* command = (TalkersCmd*)pPrm;
		CommandsCycle(*((TalkersCmd*)command), &sS);

		if (closesocket(sS) == SOCKET_ERROR)
			throw SetErrorMsgText("closesocket: ", WSAGetLastError());

		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("cleanup: ", WSAGetLastError());
	}
	catch (string errorMsgText)
	{
		cout << errorMsgText << endl;
	}

	cout << "AcceptServer остановлен" << endl;

	ExitThread(rc); // Завершение работы потока
}
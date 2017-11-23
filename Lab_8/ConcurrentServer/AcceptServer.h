#pragma once
#include "Global.h"

static void WaitClients()
{
	bool listIsEmpty = false;
	while (!listIsEmpty)
	{
		EnterCriticalSection(&csListContact);
		listIsEmpty = listContacts.empty();
		LeaveCriticalSection(&csListContact);
		SleepEx(0, TRUE); // ������������ ����� ��� ���������� ����������� ��������
	}
}

bool AcceptCycle(SOCKET* s)
{
	bool rc = false;
	Contact c(Contact::ACCEPT, "AcceptServer");
	c.hAcceptServer = hAcceptServer;

	if ((c.s = accept(*s, (sockaddr*)&c.prms, &c.lprms)) == INVALID_SOCKET)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
			throw SetErrorMsgText("accept: ", WSAGetLastError());
	}
	else
	{
		rc = true;
		InterlockedIncrement(&sInfo.Accept); // ��������� �������� Accept �� �������
		InterlockedIncrement(&sInfo.Work);   // ��������� �������� Work �� �������
		EnterCriticalSection(&csListContact);
		listContacts.push_front(c);
		LeaveCriticalSection(&csListContact);
		SetEvent(Event);
	}
	return rc;
};

void CommandsCycle(TalkersCmd& tCmd, SOCKET* s) // ���� ��������� ������
{
	int squirt = 0;
	while (tCmd != EXIT) // ���� ��������� ������ ������� � �����������
	{
		switch (tCmd)
		{
		case START:	   
			tCmd = GETCOMMAND;  // ����������� ����������� ��������
			squirt = AS_SQUIRT; // #define AS_SQUIRT 10
			break; 
		case STOP:	   
			tCmd = GETCOMMAND; // ���������� ����������� ��������
			squirt = 0; 
			break;
		case WAIT:	   
			tCmd = GETCOMMAND; 
			squirt = AS_SQUIRT; 
			WaitClients();
			break;
		case SHUTDOWN:
			tCmd = EXIT;
			WaitClients();
			break;
		};

		if ((tCmd != EXIT) && (sInfo.Work < squirt))
		{
			if (AcceptCycle(s))
				tCmd = GETCOMMAND;

			SleepEx(0, TRUE); // ��������� ����������� ���������
		}
	}
};

DWORD WINAPI AcceptServer(LPVOID pPrm)
{
	cout << "AcceptServer �������" << endl;

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

		// ������������ ������ � ����� ��� ����������
		u_long nonblk = 1;
		if (ioctlsocket(sS, FIONBIO, &(nonblk)) == SOCKET_ERROR)
			throw SetErrorMsgText("ioctlsocket: ", WSAGetLastError());

		TalkersCmd* tCmd = (TalkersCmd*)pPrm;
		CommandsCycle(*tCmd, &sS);

		if (closesocket(sS) == SOCKET_ERROR)
			throw SetErrorMsgText("closesocket: ", WSAGetLastError());

		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("cleanup: ", WSAGetLastError());
	}
	catch (string errorMessage)
	{
		cout << errorMessage << endl;
	}

	cout << "AcceptServer ����������" << endl;

	ExitThread(rc);
}
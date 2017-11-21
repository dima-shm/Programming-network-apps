#pragma once
#include "func_ErrorMsgText.h"
#include "listContact.h"
#include <time.h>
#include "conio.h"

int tport = 2000;							  // TCP-����
int uport = 2000;							  // UDP-����
char* npname = "\\\\.\\pipe\\shm-tube";		  // ��� Named Pipe
char* call = "dima-shm";					  // ��������
char* dllname = "ServiceLibrary";			  // �������� DLL ����������

HANDLE hAcceptServer, // ���������� ������ AcceptServer
hConsolePipe,		  // ���������� ������ ConsolePipe
hGarbageCleaner,	  // ���������� ������ GarbageCleaner
hDispathServer;		  // ���������� ������ hDispathServer
HANDLE Event = CreateEvent(NULL, FALSE, FALSE, NULL);

int AS_SQUIRT = 10;

enum TalkersCmd { Start, Stop, Exit, Statistics, Wait, Shutdown, Getcommand }; // ������� �������

volatile LONG Accept = 0;
volatile LONG Fail = 0;
volatile LONG Finished = 0;
volatile LONG Work = 0;

ListContact listContacts;

CRITICAL_SECTION scListContact;

HANDLE(*ts1)(char*, LPVOID);
HMODULE st1;

void CALLBACK ASWTimer(LPVOID Lprm, DWORD, DWORD)
{
	char obuf[50] = "Close: timeout;";
	Contact *client = (Contact*)Lprm;
	EnterCriticalSection(&scListContact);
	client->TimerOff = true;
	client->sthread = Contact::TIMEOUT;
	LeaveCriticalSection(&scListContact);

	if ((send(client->s, obuf, sizeof(obuf) + 1, NULL)) == SOCKET_ERROR)
		throw SetErrorMsgText("Send: ", WSAGetLastError());

	SYSTEMTIME stt;
	GetLocalTime(&stt);

	printf("%d.%d.%d %d:%02d Timeout ", stt.wDay, stt.wMonth, stt.wYear, stt.wHour, stt.wMinute);
	cout << client->srvname << ";" << endl;
}

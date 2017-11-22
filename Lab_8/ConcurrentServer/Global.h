#pragma once
#include "func_ErrorMsgText.h"
#include "listContact.h"
#include "statsInfo.h"
#include <time.h>
#include "conio.h"
#define AS_SQUIRT 10

int tport = 2000;							  // TCP-����
int uport = 2000;							  // UDP-����
char* npname = "\\\\.\\pipe\\shm-tube";		  // ��� Named Pipe
char* call = "dima-shm";					  // ��������
char* dllname = "ServiceLibrary";			  // �������� DLL ����������

HANDLE hAcceptServer, // ���������� ������ AcceptServer
	hConsolePipe,	  // ���������� ������ ConsolePipe
	hGarbageCleaner,  // ���������� ������ GarbageCleaner
	hDispathServer;	  // ���������� ������ hDispathServer
HANDLE Event = CreateEvent(NULL, FALSE, FALSE, NULL);

enum TalkersCmd { START, STOP, EXIT, STATISTICS, WAIT, SHUTDOWN, GETCOMMAND }; // ������� �������

volatile StatsInfo sInfo; // ��������� ��� ����� ����������� ����������

ListContact listContacts;		// ������ �����������
CRITICAL_SECTION csListContact; // ����������� ������ ��� ������ �����������

HANDLE(*ts1)(char*, LPVOID);
HMODULE st1;

void CALLBACK ASStartMessage(DWORD Lprm)
{
	Contact *client = (Contact*)Lprm;

	SYSTEMTIME stime;
	GetLocalTime(&stime);

	cout << stime.wDay << "." << stime.wMonth << "." << stime.wYear << " " << stime.wHour << ":" << stime.wMinute << endl;
	cout << "������ " << client->srvname << " ����� ������" << endl;
}

void CALLBACK ASFinishMessage(DWORD Lprm)
{
	Contact *client = (Contact*)Lprm;

	SYSTEMTIME stime;
	GetLocalTime(&stime);

	cout << stime.wDay << "." << stime.wMonth << "." << stime.wYear << " " << stime.wHour << ":" << stime.wMinute << endl;
	cout << "������ " << client->srvname << " �������� ������" << endl;
}

void CALLBACK ASWTimer(LPVOID Lprm, DWORD, DWORD)
{
	char obuf[50] = "Close: timeout";
	Contact *client = (Contact*)Lprm;
	EnterCriticalSection(&csListContact);
	client->TimerOff = true;
	client->sthread = Contact::TIMEOUT;
	LeaveCriticalSection(&csListContact);

	if ((send(client->s, obuf, sizeof(obuf) + 1, NULL)) == SOCKET_ERROR)
		throw SetErrorMsgText("send: ", WSAGetLastError());

	SYSTEMTIME stime;
	GetLocalTime(&stime);

	cout << stime.wDay << "." << stime.wMonth << "." << stime.wYear << " " << stime.wHour << ":" << stime.wMinute << endl;
	cout << client->srvname << endl;
}
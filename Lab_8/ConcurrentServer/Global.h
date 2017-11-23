#pragma once
#include "func_ErrorMsgText.h"
#include "listContact.h"
#include "statsInfo.h"
#include "conio.h"
#include <time.h>
#define AS_SQUIRT 10

int   tport  = 2000,					// TCP-����   (�� ���������)
	  uport  = 2000;					// UDP-����   (�� ���������)
char* npname = "\\\\.\\pipe\\shm-tube";	// Named Pipe (�� ���������)
char* call   = "dima-shm";				// ��������   (�� ���������)

HMODULE serviceLib;			 // ���������� dll
HANDLE(*tableService)(char*, LPVOID);

HANDLE hAcceptServer, // ���������� ������ AcceptServer
	hConsolePipe,	  // ���������� ������ ConsolePipe
	hGarbageCleaner,  // ���������� ������ GarbageCleaner
	hDispathServer;	  // ���������� ������ hDispathServer
HANDLE Event = CreateEvent(NULL, FALSE, FALSE, NULL);

enum TalkersCmd { START, STOP, EXIT, STATISTICS, WAIT, SHUTDOWN, GETCOMMAND }; // ������� �������

// volatile � ������������� ���������� ���������� tCmd � ������ ��� ���������� �����������
volatile TalkersCmd tCmd = START;

volatile StatsInfo sInfo; // ��������� ��� ����� ����������� ����������

ListContact listContacts;		// ������ �����������
CRITICAL_SECTION csListContact; // ����������� ������ ��� ������ �����������

void CALLBACK ASWTimer(LPVOID Lprm, DWORD, DWORD)
{
	Contact *client = (Contact*)Lprm;
	client->TimerOff = true;
	client->sthread = Contact::TIMEOUT;

	SYSTEMTIME stime;
	GetLocalTime(&stime);

	cout << endl;
	cout << stime.wDay << "." << stime.wMonth << "." << stime.wYear << " " << stime.wHour << ":" << stime.wMinute << endl;
	cout << "TIMEOUT " << client->srvname << endl;
}
#pragma once
#include "func_ErrorMsgText.h"
#include "listContact.h"
#include "statsInfo.h"
#include "conio.h"
#include <time.h>
#define AS_SQUIRT 10

int   tport  = 2000,					// TCP-порт   (по умолчанию)
	  uport  = 2000;					// UDP-порт   (по умолчанию)
char* npname = "\\\\.\\pipe\\shm-tube";	// Named Pipe (по умолчанию)
char* call   = "dima-shm";				// Позывной   (по умолчанию)

HMODULE serviceLib;			 // Библиотека dll
HANDLE(*tableService)(char*, LPVOID);

HANDLE hAcceptServer, // Дескриптор потока AcceptServer
	hConsolePipe,	  // Дескриптор потока ConsolePipe
	hGarbageCleaner,  // Дескриптор потока GarbageCleaner
	hDispathServer;	  // Дескриптор потока hDispathServer
HANDLE Event = CreateEvent(NULL, FALSE, FALSE, NULL);

enum TalkersCmd { START, STOP, EXIT, STATISTICS, WAIT, SHUTDOWN, GETCOMMAND }; // Команды сервера

// volatile — необходимость размещения переменной tCmd в памяти без выполнения оптимизации
volatile TalkersCmd tCmd = START;

volatile StatsInfo sInfo; // Структура для сбора необходимой статистики

ListContact listContacts;		// Список подключений
CRITICAL_SECTION csListContact; // Критическая секция для списка подключений

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
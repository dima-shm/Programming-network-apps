#pragma once
#include "func_ErrorMsgText.h"
#include "listContact.h"
#include "statsInfo.h"
#include <time.h>
#include "conio.h"
#define AS_SQUIRT 10

int tport = 2000;							  // TCP-порт
int uport = 2000;							  // UDP-порт
char* npname = "\\\\.\\pipe\\shm-tube";		  // Имя Named Pipe
char* call = "dima-shm";					  // Позывной
char* dllname = "ServiceLibrary";			  // Название DLL библиотеки

HANDLE hAcceptServer, // Дескриптор потока AcceptServer
	hConsolePipe,	  // Дескриптор потока ConsolePipe
	hGarbageCleaner,  // Дескриптор потока GarbageCleaner
	hDispathServer;	  // Дескриптор потока hDispathServer
HANDLE Event = CreateEvent(NULL, FALSE, FALSE, NULL);

enum TalkersCmd { START, STOP, EXIT, STATISTICS, WAIT, SHUTDOWN, GETCOMMAND }; // Команды сервера

volatile StatsInfo sInfo; // Структура для сбора необходимой статистики

ListContact listContacts;		// Список подключений
CRITICAL_SECTION csListContact; // Критическая секция для списка подключений

HANDLE(*ts1)(char*, LPVOID);
HMODULE st1;

void CALLBACK ASStartMessage(DWORD Lprm)
{
	Contact *client = (Contact*)Lprm;

	SYSTEMTIME stime;
	GetLocalTime(&stime);

	cout << stime.wDay << "." << stime.wMonth << "." << stime.wYear << " " << stime.wHour << ":" << stime.wMinute << endl;
	cout << "Клиент " << client->srvname << " начал работу" << endl;
}

void CALLBACK ASFinishMessage(DWORD Lprm)
{
	Contact *client = (Contact*)Lprm;

	SYSTEMTIME stime;
	GetLocalTime(&stime);

	cout << stime.wDay << "." << stime.wMonth << "." << stime.wYear << " " << stime.wHour << ":" << stime.wMinute << endl;
	cout << "Клиент " << client->srvname << " завершил работу" << endl;
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
#pragma once
#include <time.h>
#include "listContact.h"
#include "func_ErrorMsgText.h"

void CALLBACK ASStartMessage(DWORD Lprm)
{
	Contact *client = (Contact*)Lprm;

	SYSTEMTIME stime;
	GetLocalTime(&stime);

	cout << endl;
	cout << stime.wDay << "." << stime.wMonth << "." << stime.wYear << " " << stime.wHour << ":" << stime.wMinute << endl;
	cout << "Клиент " << client->srvname << " начал работу" << endl;
}

void CALLBACK ASFinishMessage(DWORD Lprm)
{
	Contact *client = (Contact*)Lprm;

	SYSTEMTIME stime;
	GetLocalTime(&stime);

	cout << endl;
	cout << stime.wDay << "." << stime.wMonth << "." << stime.wYear << " " << stime.wHour << ":" << stime.wMinute << endl;
	cout << "Клиент " << client->srvname << " завершил работу" << endl;
}
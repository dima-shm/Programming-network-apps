#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <string>
#include <conio.h>

using namespace std;

void main()
{
	setlocale(0, "");

	HANDLE cH;
	char buf[26] = "Hello from Maislot-client";
	DWORD lBuf;

	try
	{
		if ((cH = CreateFile(L"\\\\.\\mailslot\\Box",
			GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE, 
			NULL, OPEN_EXISTING, 
			NULL, NULL)) == INVALID_HANDLE_VALUE)
			throw "CreateFile exception";

		if (!WriteFile(cH, buf, sizeof(buf), &lBuf, NULL))
			throw "WriteFile exception";

		CloseHandle(cH);
	}
	catch (string errorMessage)
	{
		cout << errorMessage << endl;
	}
	_getch();
}
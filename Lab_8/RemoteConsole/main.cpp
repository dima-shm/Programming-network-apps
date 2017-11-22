#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "func_ErrorMsgText.h"
#include "conio.h"

void main()
{
	setlocale(0, "");
	SetConsoleTitle(L"RemoteConsole");

	try
	{
		cout << "�������:" << endl;
		cout << "[1] � ������" << endl;
		cout << "[2] � ���������" << endl;
		cout << "[3] � �����" << endl;
		cout << "[4] � ����������" << endl;
		cout << "[5] � ��������" << endl;
		cout << "[6] � ����������" << endl;
		cout << "[7] � ����� �� RemoteConsole" << endl << endl;

		HANDLE hPipe;
		char writeBuf[10],
			 readBuf[50];
		DWORD lBuf;
		int cmd;
		
		hPipe = CreateFile(L"\\\\.\\pipe\\shm-tube",
			GENERIC_READ | GENERIC_WRITE, // ������ � ������ � �����
			NULL,
			NULL, OPEN_EXISTING,
			NULL, NULL);

		do
		{
			cout << "�������� �������: "; cin >> cmd;

			if ((cmd > 0) && (cmd < 7))
			{
				_itoa(cmd, writeBuf, 10);

				if (!WriteFile(hPipe, writeBuf, sizeof(writeBuf), &lBuf, NULL))
					cout << "������: WriteFile" << endl;

				if (ReadFile(hPipe, readBuf, sizeof(readBuf), &lBuf, NULL))
					cout << readBuf << endl << endl;
				else
					cout << "������: ReadFile" << endl;
			}
			else if (cmd > 7)
				cout << "������� �������������� ������� [" << cmd << "]" << endl << endl;
		} while (cmd != 3 && cmd != 6 && cmd != 7);

		CloseHandle(hPipe);
	}
	catch (string errorMessage)
	{
		cout << errorMessage << endl;
	}

	_getch();
}
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "func_ErrorMsgText.h"
#include "conio.h"

int main(int argc, char* argv[])
{
	setlocale(0, "");
	SetConsoleTitle(L"RemoteConsole");

	HANDLE hPipe;
	char writeBuf[1],
		 readBuf[256];
	DWORD lBuf;
	int code;

	cout << "�������:" << endl;
	cout << "[1] � ������" << endl;
	cout << "[2] � ���������" << endl;
	cout << "[3] � ����������" << endl;
	cout << "[4] � ��������" << endl;
	cout << "[5] � ����������" << endl;
	cout << "[6] � ����������" << endl;
	cout << "[7] � ����� �� RemoteConsole" << endl << endl;

	hPipe = CreateFile(L"\\\\.\\pipe\\shm-tube",
		GENERIC_READ | GENERIC_WRITE, // ������ � ������ � �����
		FILE_SHARE_READ | FILE_SHARE_WRITE, // ����� ����������� �������������
		NULL, OPEN_EXISTING,
		NULL, NULL);

	do
	{
		cout << "�������� �������: "; cin >> code;
		if ((code > 0) && (code < 7))
		{
			_itoa(code, writeBuf, 10);
			WriteFile(hPipe, writeBuf, sizeof(writeBuf), &lBuf, NULL);

			if (ReadFile(hPipe, readBuf, sizeof(readBuf), &lBuf, NULL))
				cout << "����� �� �������: " << readBuf << endl << endl;
			else
				cout << "������: ReadFile" << endl;
		}
		else
			cout << "�������� �������" << endl << endl;
	} while (code != 3 && code != 6 && code != 7);

	_getch();
	return 0;
}
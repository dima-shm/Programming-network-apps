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
		cout << "Команды:" << endl;
		cout << "[1] – Запуск" << endl;
		cout << "[2] – Остановка" << endl;
		cout << "[3] – Выход" << endl;
		cout << "[4] – Статистика" << endl;
		cout << "[5] – Ожидание" << endl;
		cout << "[6] – Выключение" << endl;
		cout << "[7] – Выйти из RemoteConsole" << endl << endl;

		HANDLE hPipe;
		char writeBuf[10],
			 readBuf[50];
		DWORD lBuf;
		int cmd;
		
		hPipe = CreateFile(L"\\\\.\\pipe\\shm-tube",
			GENERIC_READ | GENERIC_WRITE, // Чтение и запись в канал
			NULL,
			NULL, OPEN_EXISTING,
			NULL, NULL);

		do
		{
			cout << "Выберите команду: "; cin >> cmd;

			if ((cmd > 0) && (cmd < 7))
			{
				_itoa(cmd, writeBuf, 10);

				if (!WriteFile(hPipe, writeBuf, sizeof(writeBuf), &lBuf, NULL))
					cout << "Ошибка: WriteFile" << endl;

				if (ReadFile(hPipe, readBuf, sizeof(readBuf), &lBuf, NULL))
					cout << readBuf << endl << endl;
				else
					cout << "Ошибка: ReadFile" << endl;
			}
			else if (cmd > 7)
				cout << "Введена несуществующая команда [" << cmd << "]" << endl << endl;
		} while (cmd != 3 && cmd != 6 && cmd != 7);

		CloseHandle(hPipe);
	}
	catch (string errorMessage)
	{
		cout << errorMessage << endl;
	}

	_getch();
}
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

	cout << "Команды:" << endl;
	cout << "[1] – Запуск" << endl;
	cout << "[2] – Остановка" << endl;
	cout << "[3] – Выключение" << endl;
	cout << "[4] – Ожидание" << endl;
	cout << "[5] – Статистика" << endl;
	cout << "[6] – Выключение" << endl;
	cout << "[7] – Выйти из RemoteConsole" << endl << endl;

	hPipe = CreateFile(L"\\\\.\\pipe\\shm-tube",
		GENERIC_READ | GENERIC_WRITE, // Чтение и запись в канал
		FILE_SHARE_READ | FILE_SHARE_WRITE, // Режим совместного использования
		NULL, OPEN_EXISTING,
		NULL, NULL);

	do
	{
		cout << "Выберите команду: "; cin >> code;
		if ((code > 0) && (code < 7))
		{
			_itoa(code, writeBuf, 10);
			WriteFile(hPipe, writeBuf, sizeof(writeBuf), &lBuf, NULL);

			if (ReadFile(hPipe, readBuf, sizeof(readBuf), &lBuf, NULL))
				cout << "Ответ от сервера: " << readBuf << endl << endl;
			else
				cout << "Ошибка: ReadFile" << endl;
		}
		else
			cout << "Неверная команда" << endl << endl;
	} while (code != 3 && code != 6 && code != 7);

	_getch();
	return 0;
}
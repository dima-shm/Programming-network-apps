#pragma once
#include "Global.h"

DWORD WINAPI ConsolePipe(LPVOID pPrm)
{
	cout << "ConsolePipe запущен" << endl;

	DWORD rc = 0;
	HANDLE hPipe;
	TalkersCmd tCmd = *((TalkersCmd*)pPrm);

	hPipe = CreateNamedPipe(L"\\\\.\\pipe\\shm-tube",
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_WAIT,
		1, NULL, NULL,
		INFINITE, NULL);

	while (tCmd != Exit)
	{
		if (ConnectNamedPipe(hPipe, NULL))
		{
			cout << "Установлено соединение с RemoteConsole" << endl << endl;

			char readBuf[1];
			DWORD lBuf;

			while (tCmd != Exit)
			{
				ReadFile(hPipe, readBuf, sizeof(readBuf), &lBuf, NULL);

				if (lBuf > 0)
				{
					switch (atoi(readBuf))
					{
					case 1:
						tCmd = TalkersCmd::Start;
						WriteFile(hPipe, "Запуск", sizeof("Запуск"), &lBuf, NULL);
						cout << "ConsolePipe: команда " << "Запуск" << endl;
						break;
					case 2:
						tCmd = TalkersCmd::Stop;
						WriteFile(hPipe, "Остановка", sizeof("Остановка"), &lBuf, NULL);
						cout << "ConsolePipe: команда " << "Остановка" << endl;
						break;
					case 3:
						tCmd = TalkersCmd::Exit;
						WriteFile(hPipe, "Выход", sizeof("Выход"), &lBuf, NULL);
						cout << "ConsolePipe: команда " << "Выход" << endl;
						break;
					case 4:
						tCmd = TalkersCmd::Wait;
						WriteFile(hPipe, "Ожидание", sizeof("Ожидание"), &lBuf, NULL);
						cout << "ConsolePipe: команда " << "Ожидание" << endl;
						break;
					case 5:
						cout << endl;
						cout << "Подключено: " << Accept << endl;
						cout << "Успешно:    " << Finished << endl;
						cout << "Работают:   " << Work << endl;
						cout << "Ошибка:     " << Fail << endl;
						WriteFile(hPipe, "Статистика", sizeof("Статистика"), &lBuf, NULL);
						break;
					case 6:
						tCmd = TalkersCmd::Shutdown;
						WriteFile(hPipe, "Выключение", sizeof("Выключение"), &lBuf, NULL);
						cout << "ConsolePipe: команда " << "Выключение" << endl;
						break;
					}
				}
			}
		}
	}

	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	cout << "ConsolePipe остановлен" << endl;

	ExitThread(rc);
}
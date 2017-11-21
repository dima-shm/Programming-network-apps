#include "Global.h"
#include "AcceptServer.h"
#include "ConsolePipe.h"
#include "GarbageCleaner.h"
#include "DispathServer.h"

using namespace std;

bool portIsCorrect(int port) 
{
	return (port >= 0 && port <= 65535);
}

int main(int argc, char* argv[])
{
	setlocale(0, "");
	SetConsoleTitle(L"ConcurrentServer");
	srand((unsigned)time(NULL));

	if (argc > 1)
	{
		if (portIsCorrect(atoi(argv[1])))
			cout << "TCP-порт:   " << (tport = atoi(argv[1])) << endl;
		else
			cout << "Некорректный TCP-порт" << endl;
	}
	else
		cout << "TCP-порт:   " << tport << "              (default)" << endl;


	if (argc > 2)
	{
		if (portIsCorrect(atoi(argv[2])))
			cout << "UDP-порт:   " << (uport = atoi(argv[2])) << endl;
		else
			cout << "Некорректный UDP-порт" << endl;
	}
	else
		cout << "UDP-порт:   " << uport << "              (default)" << endl;


	if (argc > 3)
		dllname = argv[3];


	if (argc > 4)
		cout << "Named Pipe: " << (npname = argv[4]) << endl;
	else
		cout << "Named Pipe: " << npname << " (default)" << endl;


	if (argc > 5)
		cout << "Позывной:   " << (call = argv[5]) << endl;
	else
		cout << "Позывной:   " << call << "          (default)" << endl;



	

	InitializeCriticalSection(&scListContact);

	st1 = LoadLibrary((LPCWSTR)dllname);
	ts1 = (HANDLE(*)(char*, LPVOID))GetProcAddress(st1, "SSS");
	if (st1 == NULL)
		cout << "Не удалось загрузить DLL-библиотеку" << endl << endl;
	else
		cout << "Загружена DLL-библиотека: " << dllname << endl << endl;

	// volatile — необходимость размещения переменной cmd в памяти без выполнения оптимизации
	volatile TalkersCmd cmd = Start; // Команды сервера

	hAcceptServer = CreateThread(NULL, NULL, AcceptServer, (LPVOID)&cmd, NULL, NULL);
	hConsolePipe = CreateThread(NULL, NULL, ConsolePipe, (LPVOID)&cmd, NULL, NULL);
	hGarbageCleaner = CreateThread(NULL, NULL, GarbageCleaner, (LPVOID)&cmd, NULL, NULL);
	hDispathServer = CreateThread(NULL, NULL, DispathServer, (LPVOID)&cmd, NULL, NULL);

	// Установить приоритеты потоков
	SetThreadPriority(hAcceptServer, THREAD_PRIORITY_HIGHEST);
	SetThreadPriority(hConsolePipe, THREAD_PRIORITY_NORMAL);
	SetThreadPriority(hGarbageCleaner, THREAD_PRIORITY_BELOW_NORMAL);
	SetThreadPriority(hDispathServer, THREAD_PRIORITY_NORMAL);

	// Приостанавливить выполнение main до завершения работы потока
	WaitForSingleObject(hAcceptServer, INFINITE);
	WaitForSingleObject(hConsolePipe, INFINITE);
	WaitForSingleObject(hGarbageCleaner, INFINITE);
	WaitForSingleObject(hDispathServer, INFINITE);

	CloseHandle(hAcceptServer);
	CloseHandle(hConsolePipe);
	CloseHandle(hGarbageCleaner);
	CloseHandle(hDispathServer);

	FreeLibrary(st1);
	DeleteCriticalSection(&scListContact);

	_getch();
	return 0;
}
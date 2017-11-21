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
			cout << "TCP-����:   " << (tport = atoi(argv[1])) << endl;
		else
			cout << "������������ TCP-����" << endl;
	}
	else
		cout << "TCP-����:   " << tport << "              (default)" << endl;


	if (argc > 2)
	{
		if (portIsCorrect(atoi(argv[2])))
			cout << "UDP-����:   " << (uport = atoi(argv[2])) << endl;
		else
			cout << "������������ UDP-����" << endl;
	}
	else
		cout << "UDP-����:   " << uport << "              (default)" << endl;


	if (argc > 3)
		dllname = argv[3];


	if (argc > 4)
		cout << "Named Pipe: " << (npname = argv[4]) << endl;
	else
		cout << "Named Pipe: " << npname << " (default)" << endl;


	if (argc > 5)
		cout << "��������:   " << (call = argv[5]) << endl;
	else
		cout << "��������:   " << call << "          (default)" << endl;



	

	InitializeCriticalSection(&scListContact);

	st1 = LoadLibrary((LPCWSTR)dllname);
	ts1 = (HANDLE(*)(char*, LPVOID))GetProcAddress(st1, "SSS");
	if (st1 == NULL)
		cout << "�� ������� ��������� DLL-����������" << endl << endl;
	else
		cout << "��������� DLL-����������: " << dllname << endl << endl;

	// volatile � ������������� ���������� ���������� cmd � ������ ��� ���������� �����������
	volatile TalkersCmd cmd = Start; // ������� �������

	hAcceptServer = CreateThread(NULL, NULL, AcceptServer, (LPVOID)&cmd, NULL, NULL);
	hConsolePipe = CreateThread(NULL, NULL, ConsolePipe, (LPVOID)&cmd, NULL, NULL);
	hGarbageCleaner = CreateThread(NULL, NULL, GarbageCleaner, (LPVOID)&cmd, NULL, NULL);
	hDispathServer = CreateThread(NULL, NULL, DispathServer, (LPVOID)&cmd, NULL, NULL);

	// ���������� ���������� �������
	SetThreadPriority(hAcceptServer, THREAD_PRIORITY_HIGHEST);
	SetThreadPriority(hConsolePipe, THREAD_PRIORITY_NORMAL);
	SetThreadPriority(hGarbageCleaner, THREAD_PRIORITY_BELOW_NORMAL);
	SetThreadPriority(hDispathServer, THREAD_PRIORITY_NORMAL);

	// ���������������� ���������� main �� ���������� ������ ������
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
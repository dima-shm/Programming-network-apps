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

void main(int argc, char* argv[])
{
	setlocale(0, "");
	SetConsoleTitle(L"ConcurrentServer");

	if (argc > 1)
	{
		if (portIsCorrect(atoi(argv[1])))
			cout << "TCP-����:       " << (tport = atoi(argv[1])) << endl;
		else
			cout << "������������ TCP-����" << endl;
	}
	else
		cout << "TCP-����:       " << tport << "               (�� ���������)" << endl;
	if (argc > 2)
	{
		if (portIsCorrect(atoi(argv[2])))
			cout << "UDP-����:       " << (uport = atoi(argv[2])) << endl;
		else
			cout << "������������ UDP-����" << endl;
	}
	else
		cout << "UDP-����:       " << uport << "               (�� ���������)" << endl;
	if (argc > 3)
		cout << "Named Pipe:     " << (npname = argv[3]) << endl;
	else
		cout << "Named Pipe:     " << npname << "  (�� ���������)" << endl;
	if (argc > 4)
		cout << "��������:       " << (call = argv[4]) << endl;
	else
		cout << "��������:       " << call << "           (�� ���������)" << endl;
	if ((serviceLib = LoadLibrary(L"ServiceLibrary.dll")) == NULL)
		cout << "DLL-����������: ServiceLibrary.dll �� ���������" << endl << endl;
	else
		cout << "DLL-����������: ServiceLibrary.dll ��������� " << endl << endl;
	tableService = (HANDLE(*)(char*, LPVOID))GetProcAddress(serviceLib, "SSS");

	InitializeCriticalSection(&csListContact);

	hAcceptServer = CreateThread(NULL, NULL, AcceptServer, (LPVOID)&tCmd, NULL, NULL);
	hConsolePipe = CreateThread(NULL, NULL, ConsolePipe, (LPVOID)&tCmd, NULL, NULL);
	hGarbageCleaner = CreateThread(NULL, NULL, GarbageCleaner, (LPVOID)&tCmd, NULL, NULL);
	hDispathServer = CreateThread(NULL, NULL, DispathServer, (LPVOID)&tCmd, NULL, NULL);

	// ���������� ���������� �������
	SetThreadPriority(hAcceptServer, THREAD_PRIORITY_HIGHEST);
	SetThreadPriority(hConsolePipe, THREAD_PRIORITY_NORMAL);
	SetThreadPriority(hGarbageCleaner, THREAD_PRIORITY_BELOW_NORMAL);
	SetThreadPriority(hDispathServer, THREAD_PRIORITY_NORMAL);

	// ������������� ���������� main �� ���������� ������ �������
	WaitForSingleObject(hAcceptServer, INFINITE);
	WaitForSingleObject(hConsolePipe, INFINITE);
	WaitForSingleObject(hGarbageCleaner, INFINITE);
	WaitForSingleObject(hDispathServer, INFINITE);

	// ���������� ��������� � ������� �������
	CloseHandle(hAcceptServer);
	CloseHandle(hConsolePipe);
	CloseHandle(hGarbageCleaner);
	CloseHandle(hDispathServer);

	FreeLibrary(serviceLib);

	DeleteCriticalSection(&csListContact);

	_getch();
}
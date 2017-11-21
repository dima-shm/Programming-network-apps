#pragma once
#include "Global.h"

DWORD WINAPI ConsolePipe(LPVOID pPrm)
{
	cout << "ConsolePipe �������" << endl;

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
			cout << "����������� ���������� � RemoteConsole" << endl << endl;

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
						WriteFile(hPipe, "������", sizeof("������"), &lBuf, NULL);
						cout << "ConsolePipe: ������� " << "������" << endl;
						break;
					case 2:
						tCmd = TalkersCmd::Stop;
						WriteFile(hPipe, "���������", sizeof("���������"), &lBuf, NULL);
						cout << "ConsolePipe: ������� " << "���������" << endl;
						break;
					case 3:
						tCmd = TalkersCmd::Exit;
						WriteFile(hPipe, "�����", sizeof("�����"), &lBuf, NULL);
						cout << "ConsolePipe: ������� " << "�����" << endl;
						break;
					case 4:
						tCmd = TalkersCmd::Wait;
						WriteFile(hPipe, "��������", sizeof("��������"), &lBuf, NULL);
						cout << "ConsolePipe: ������� " << "��������" << endl;
						break;
					case 5:
						cout << endl;
						cout << "����������: " << Accept << endl;
						cout << "�������:    " << Finished << endl;
						cout << "��������:   " << Work << endl;
						cout << "������:     " << Fail << endl;
						WriteFile(hPipe, "����������", sizeof("����������"), &lBuf, NULL);
						break;
					case 6:
						tCmd = TalkersCmd::Shutdown;
						WriteFile(hPipe, "����������", sizeof("����������"), &lBuf, NULL);
						cout << "ConsolePipe: ������� " << "����������" << endl;
						break;
					}
				}
			}
		}
	}

	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	cout << "ConsolePipe ����������" << endl;

	ExitThread(rc);
}
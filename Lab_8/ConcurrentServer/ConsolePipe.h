#pragma once
#include "Global.h"

DWORD WINAPI ConsolePipe(LPVOID pPrm)
{
	cout << "ConsolePipe �������" << endl;

	DWORD rc = 0;
	HANDLE hPipe;

	try
	{
		if ((hPipe = CreateNamedPipe(L"\\\\.\\pipe\\shm-tube",
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE | PIPE_WAIT,
			1, NULL, NULL,
			INFINITE, NULL)) == INVALID_HANDLE_VALUE)
			throw SetErrorMsgText("CreateNamedPipe: ", GetLastError());

		while (*((TalkersCmd*)pPrm) != EXIT)
		{
			if (!ConnectNamedPipe(hPipe, NULL))
				throw SetErrorMsgText("ConnectNamedPipe: ", GetLastError());

			cout << endl << "RemoteConsole ����� ������" << endl << endl;

			char readBuf[10],
				writeBuf[50];
			DWORD lBuf;

			while (*((TalkersCmd*)pPrm) != EXIT)
			{
				if (!ReadFile(hPipe, readBuf, sizeof(readBuf), &lBuf, NULL))
					break;

				if (lBuf > 0)
				{
					switch (atoi(readBuf))
					{
					case 1:
						*((TalkersCmd*)pPrm) = TalkersCmd::START;
						WriteFile(hPipe, "������", sizeof("������"), &lBuf, NULL);
						cout << "ConsolePipe: ������� " << "������" << endl;
						break;
					case 2:
						*((TalkersCmd*)pPrm) = TalkersCmd::STOP;
						WriteFile(hPipe, "���������", sizeof("���������"), &lBuf, NULL);
						cout << "ConsolePipe: ������� " << "���������" << endl;
						break;
					case 3:
						*((TalkersCmd*)pPrm) = TalkersCmd::EXIT;
						WriteFile(hPipe, "�����", sizeof("�����"), &lBuf, NULL);
						cout << "ConsolePipe: ������� " << "�����" << endl;
						break;
					case 4:
						*((TalkersCmd*)pPrm) = TalkersCmd::STATISTICS;
						sprintf_s(writeBuf, "\n����������: %i\n������: %i\n�������: %i\n��������: %i", sInfo.Accept, sInfo.Fail, sInfo.Finished, sInfo.Work);
						WriteFile(hPipe, writeBuf, sizeof(writeBuf), &lBuf, NULL);
						cout << "ConsolePipe: ������� " << "����������" << endl;
						break;
					case 5:
						*((TalkersCmd*)pPrm) = TalkersCmd::WAIT;
						WriteFile(hPipe, "��������", sizeof("��������"), &lBuf, NULL);
						cout << "ConsolePipe: ������� " << "��������" << endl;
						break;
					case 6:
						*((TalkersCmd*)pPrm) = TalkersCmd::SHUTDOWN;
						WriteFile(hPipe, "����������", sizeof("����������"), &lBuf, NULL);
						cout << "ConsolePipe: ������� " << "����������" << endl;
						break;
					default:
						cout << "ConsolePipe: ����������� �������" << endl;
						break;
					}
				}
			}

			cout << endl << "RemoteConsole �������� ������" << endl;

			if (!DisconnectNamedPipe(hPipe))
				throw SetErrorMsgText("DisconnectNamedPipe: ", GetLastError());
		}

		DisconnectNamedPipe(hPipe);
		CloseHandle(hPipe);
	}
	catch (string errorMessage)
	{
		cout << errorMessage << endl;
	}

	cout << "ConsolePipe ����������" << endl;

	ExitThread(rc);
}
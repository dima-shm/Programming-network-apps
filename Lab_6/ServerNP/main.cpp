#include "func_ErrorMsgText.h"

void main()
{
	setlocale(0, "");

	HANDLE sH;
	char buf[24];
	DWORD lBuf;

	try
	{
		sH = CreateNamedPipe(L"\\\\.\\pipe\\Tube",
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE | PIPE_WAIT,
			1, NULL, NULL,
			INFINITE, NULL);

		if (ConnectNamedPipe(sH, NULL))
		{
			cout << "”становлено соединение сервера с каналом клиента" << endl << endl;
			while (true)
			{
				ReadFile(sH, buf, sizeof(buf), &lBuf, NULL);

				if (strcmp(buf, "STOP") == 0 || buf[0] == 'C')
					break;
				cout << buf << endl;

				WriteFile(sH, buf, sizeof(buf), &lBuf, NULL);
			}
		}
		else
		{
			cout << "Ќе удалось соединить сервер с каналом клиента" << endl;
		}
		DisconnectNamedPipe(sH);
		CloseHandle(sH);
	}
	catch (string errorMessage)
	{
		cout << errorMessage << endl;
	}
	_getch();
}
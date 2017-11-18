#define _CRT_SECURE_NO_WARNINGS
#include "func_ErrorMsgText.h"

void prepareString(string& str, int i)
{
	str.erase(18);
	if (i < 10)
	{
		str.append("00");
		str.append(to_string(i));
	}
	else if (i < 100)
	{
		str.append("0");
		str.append(to_string(i));
	}
	else
		str.append(to_string(i));
}

void main()
{
	setlocale(0, "");

	HANDLE cH;
	char buf[24] = "Hello from Client 000";
	string bufStr;
	DWORD lBuf;
	int numMsg;

	try
	{
		cH = CreateFile(L"\\\\.\\pipe\\Tube",
			GENERIC_READ | GENERIC_WRITE, // Чтение и запись в канал
			FILE_SHARE_READ | FILE_SHARE_WRITE, // Режим совместного использования
			NULL, OPEN_EXISTING, 
			NULL, NULL);

		DWORD dwMode = PIPE_READMODE_MESSAGE; // Открыть канал в режиме передачи сообщениями
		SetNamedPipeHandleState(cH, &dwMode, NULL, NULL); // Изменить характеристики канала

		cout << "Введите количество сообщений: "; cin >> numMsg; cout << endl;
		for (int i = 0; i < numMsg; i++)
		{
			TransactNamedPipe(cH, buf, sizeof(buf), buf, sizeof(buf), &lBuf, NULL);

			cout  << buf << endl;

			bufStr = (string)buf;
			prepareString(bufStr, i+1);
			strcpy(buf, bufStr.c_str());
		}
		WriteFile(cH, "STOP", sizeof("STOP"), &lBuf, NULL);

		CloseHandle(cH);
	}
	catch (string errorMessage)
	{
		cout << errorMessage << endl;
	}
	_getch();
}
#include "func_ErrorMsgText.h"
#include <time.h>

bool SystemMessage(char *ch)
{
	bool result = false;
	char Timeout[50] = "Close: timeout;",
		Close[50] = "Close: finish;",
		Abort[50] = "Close: Abort;";

	if (strcmp(ch, Timeout) == NULL)
		result = true;
	else if (strcmp(ch, Abort) == NULL)
		result = true;
	else if (strcmp(ch, Close) == NULL)
		result = true;

	return result;
}


bool GetServer(char* call, SOCKADDR_IN* from, int* flen, SOCKET * cC, SOCKADDR_IN * all)
{
	char ibuf[50],
		obuf[50];
	int libuf = 0,
		lobuf = 0;

	if ((lobuf = sendto(*cC, call, strlen(call), NULL, (sockaddr*)all, sizeof(*all))) == SOCKET_ERROR)
		throw SetErrorMsgText("sendto: ", WSAGetLastError());

	if ((libuf = recvfrom(*cC, ibuf, sizeof(ibuf), NULL, (sockaddr*)from, flen)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAETIMEDOUT)
			return false;
		else
			throw SetErrorMsgText("recvfrom: ", WSAGetLastError());

		if (strcmp(call, ibuf) == 0)
			return true;
		else
			return false;
	}
}

void main()
{
	setlocale(0, "");
	SetConsoleTitle(L"Client");

	WSADATA wsaData;
	SOCKET cS;
	SOCKADDR_IN serv;
	char servName[50] = "DIMA";
	char call[50] = "dima-shm";
	int port = 2000;

	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("startup: ", WSAGetLastError());

		if ((cS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket: ", WSAGetLastError());

		int ch = 0;
		bool fin = false;
		int cCall = 0;
		int lobuf = 1;
		char obuf[50];
		char Call[50];

		cout << "Введите имя сервера: "; cin >> servName;
		hostent* s = gethostbyname(servName);
		if (s == NULL)
			throw "Сервер не найден";

		cout << "Введите порт сервера: "; cin >> port;

		serv.sin_addr = *(struct in_addr *)s->h_addr_list[0];
		serv.sin_family = AF_INET;
		serv.sin_port = htons(port);

		if ((connect(cS, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
			throw "Ошибка подключения: connect";

		while (!fin)
		{
			cout << "[1] – Rand" << endl;
			cout << "[2] – Time" << endl;
			cout << "[3] – Echo" << endl;
			cin >> cCall;

			if (cCall == 1 || cCall == 2 || cCall == 3)
			{
				fin = true;
				break;
			}
			else
			{
				if (closesocket(cS) == SOCKET_ERROR)
					throw SetErrorMsgText("closesocket: ", WSAGetLastError());

				throw "Вы неправильно ввели код команду";
			}
		}

		switch (cCall)
		{
		case 1:	strcpy_s(Call, "Rand");
			cout << "Enter Rand command or other:" << endl;
			break;
		case 2: strcpy_s(Call, "Time");
			cout << "Enter Time command or other:" << endl;
			break;
		case 3:	strcpy_s(Call, "Echo");
			cout << "Enter string or exit :" << endl;
			break;
		default: strcpy_s(Call, "Echo");
		}

		if ((lobuf = send(cS, Call, sizeof(Call), NULL)) == SOCKET_ERROR)
			throw SetErrorMsgText("send: ", WSAGetLastError());

		char rCall[50];
		if ((lobuf = recv(cS, rCall, sizeof(rCall), NULL)) == SOCKET_ERROR)
			throw SetErrorMsgText("recv: ", WSAGetLastError());

		if (strcmp(Call, rCall) != 0)
			throw "Ошибка сервера";
		else
		{
			bool Check = true;
			fin = false;

			u_long nonblk = 1;
			if (SOCKET_ERROR == ioctlsocket(cS, FIONBIO, &nonblk))
				throw SetErrorMsgText("Ioctlsocket: ", WSAGetLastError());

			clock_t StartTime = clock();
			bool rcv = true;

			char iib[50];
			cin >> iib;

			while (!fin)
			{
				if (rcv)
				{
					if ((lobuf = send(cS, iib, sizeof(iib), NULL)) == SOCKET_ERROR)
						throw "Error;";

					rcv = false;
				}

				if ((recv(cS, obuf, sizeof(obuf), NULL)) == SOCKET_ERROR)
				{
					switch (WSAGetLastError())
					{
					case WSAEWOULDBLOCK: Sleep(100); break;
					default: throw SetErrorMsgText("recv: ", WSAGetLastError());
					}
				}
				else
				{
					if (SystemMessage(obuf))
					{
						printf("Сервер остановил подключение: %s\n", obuf);
						break;
					}
					else
						printf("Принято сообщение: [%s]\n", obuf);

					rcv = true;
				}
			}

			clock_t FinishTime = clock();
			printf("Время: %lf сек.\n", (double)(FinishTime - StartTime) / CLOCKS_PER_SEC);
		}

		if (closesocket(cS) == SOCKET_ERROR)
			throw SetErrorMsgText("closesocket: ", WSAGetLastError());

		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("cleanup: ", WSAGetLastError());
	}
	catch (string errorMessage)
	{
		cout << errorMessage << endl;
	}

	_getch();
}
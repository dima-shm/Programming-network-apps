#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "func_ErrorMsgText.h"
#include "Winsock2.h"
#include "consoleColors.h"


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

	SOCKET cS;       // ���������� ������ �������
	WSADATA wsaData;

	int numMsg;

	SOCKADDR_IN clnt;
	SOCKADDR_IN from;
	int sizeOfFrom = sizeof(from);

	try 
	{
		/********************************* 1 *********************************/
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("WSAStartup: ", WSAGetLastError());


		/********************************* 2 *********************************/
		if ((cS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket: ", WSAGetLastError());


		/********************************* 3 *********************************/
		clnt.sin_family = AF_INET;					   // ������������ IP-���������
		clnt.sin_port = htons(2000);				   // TCP-���� 2000
		clnt.sin_addr.s_addr = inet_addr("127.0.0.1"); // ����� �������

		cout << "������� ���������� ������������ ���������: "; cin >> numMsg; cout << endl;

		char buf[50] = "Hello from Client 000"; // ����� �����
		string bufStr;
		int libuf = 0;                          // ���������� ������������ ����
		int lobuf = 0;                          // ���������� �������� ����

		for (int i = 1; i <= numMsg; i++)
		{
			if ((libuf = sendto(cS, buf, strlen(buf), NULL, (sockaddr*)&clnt, sizeof(clnt))) == SOCKET_ERROR)
				throw SetErrorMsgText("sendto: ", WSAGetLastError());
			
			setConsoleClrBlue();
			cout << "    ���������� ����������� ���� ���������: " << libuf << endl;
			cout << "    ����� ���������:                       " << buf << endl;
			setConsoleClrWhite();


			/********************************* 4 *********************************/
			if ((lobuf = recvfrom(cS, buf, sizeof(buf), NULL, (sockaddr*)&from, &sizeOfFrom)) == SOCKET_ERROR)
				throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
			
			setConsoleClrGreen();
			cout << "���������� ��������� ���� ���������:   " << lobuf << endl;
			cout << "����� ���������:                       " << buf << endl;
			setConsoleClrWhite();
			
			bufStr = (string)buf;
			prepareString(bufStr, i);
			strcpy(buf, bufStr.c_str());
		}
		if (sendto(cS, "", 0, NULL, (sockaddr*)&clnt, sizeof(clnt)) == SOCKET_ERROR)
			throw SetErrorMsgText("sendto: ", WSAGetLastError());


		/********************************* 5 *********************************/
		if (closesocket(cS) == SOCKET_ERROR)
			throw SetErrorMsgText("closesocket: ", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("WSACleanup: ", WSAGetLastError());
	}
	catch (string errorMessage)
	{
		cout << errorMessage << endl;
	}
	_getch();
}
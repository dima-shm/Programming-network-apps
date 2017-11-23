#pragma once
#include <list>

using namespace std;

struct Contact // ������� ������ �����������
{
	enum TE {    // ��������� ������� �����������
		EMPTY,   // ������ ������� ������ �����������
		ACCEPT,  // ��������� (accept), �� �� �������������
		CONTACT  // ������� �������������� �������
	} type;      // ��� �������� ������ �����������
	enum ST {    // ��������� �������������� �������
		WORK,    // ���� ����� ������� � ��������
		ABORT,   // ������������������� ���������� �����������
		TIMEOUT, // ������������� ������ ���������� �� �������
		FINISH   // ������������� ������ ���������� ���������
	} sthread;   // ��������� �������������� ������� (������)
	SOCKET      s;              // ����� ��� ������ ������� � ��������
	SOCKADDR_IN prms;           // ��������� ������
	int         lprms;          // ����� prms
	HANDLE hthread;             // handle ������ (��� ��������)
	HANDLE htimer;              // handle �������
	char msg[50];               // ���������
	char srvname[15];           // ������������ �������������� �������
	bool TimerOff;
	HANDLE hAcceptServer;
	Contact(TE t = EMPTY, const char* namesrv = "") // �����������
	{
		memset(&prms, 0, sizeof(SOCKADDR_IN));
		lprms = sizeof(SOCKADDR_IN);
		type = t;
		strcpy_s(srvname, namesrv);
		msg[0] = 0;
		TimerOff = false;
	};
	void SetST(ST sth, const char* m = "")
	{
		sthread = sth;
		strcpy_s(msg, m);
	}
};
typedef list<Contact> ListContact; // ������ �����������
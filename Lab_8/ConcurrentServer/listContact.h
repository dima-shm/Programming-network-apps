#pragma once
#include <list>

using namespace std;

struct Contact // Элемент списка подключений
{
	enum TE {    // Состояние сервера подключения
		EMPTY,   // Пустой элемент списка подключений
		ACCEPT,  // Подключен (accept), но не обслуживается
		CONTACT  // Передан обслуживающему серверу
	} type;      // Тип элемента списка подключений
	enum ST {    // Состояние обслуживающего сервера
		WORK,    // Идет обмен данными с клиентом
		ABORT,   // Обслуживающийсервер завершился ненормально
		TIMEOUT, // Обслуживающий сервер завершился по времени
		FINISH   // Обслуживающий сервер завершился нормально
	} sthread;   // Состояние обслуживающего сервера (потока)
	SOCKET      s;              // Сокет для обмена данными с клиентом
	SOCKADDR_IN prms;           // Параметры сокета
	int         lprms;          // Длина prms
	HANDLE hthread;             // handle потока (или процесса)
	HANDLE htimer;              // handle таймера
	char msg[50];               // Сообщение
	char srvname[15];           // Наименование обслуживающего сервера
	bool TimerOff;
	HANDLE hAcceptServer;
	Contact(TE t = EMPTY, const char* namesrv = "") // Конструктор
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
typedef list<Contact> ListContact; // Список подключений
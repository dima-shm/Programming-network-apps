#pragma once
#include "Global.h"

using std::iterator;

DWORD WINAPI GarbageCleaner(LPVOID pPrm)
{
	cout << "GarbageCleaner запущен" << endl;

	DWORD rc = 0;

	while ((*((TalkersCmd*)pPrm) != EXIT) && (&sInfo.Work > 0))
	{
		EnterCriticalSection(&csListContact); // Вход в критическую секцию

		for (list<Contact>::iterator contact = listContacts.begin(); contact != listContacts.end();)
		{
			if (contact->sthread == Contact::FINISH || 
				contact->sthread == Contact::TIMEOUT || 
				contact->sthread == Contact::ABORT || 
				contact->type == Contact::EMPTY)
			{
				cout << "IP-адрес удаленного клиента: " << inet_ntoa(contact->prms.sin_addr);
				cout << " с кодом " << contact->sthread << endl;

				if (contact->type == Contact::EMPTY)
					InterlockedIncrement(&sInfo.Fail);
				else
				{
					if (contact->sthread == Contact::FINISH)
						InterlockedIncrement(&sInfo.Finished);

					if (contact->sthread == Contact::TIMEOUT)
						InterlockedIncrement(&sInfo.Fail);

					if (contact->sthread == Contact::ABORT)
						InterlockedIncrement(&sInfo.Fail);

					CloseHandle(contact->hthread);
					CloseHandle(contact->htimer);
				}
				closesocket(contact->s);
				contact = listContacts.erase(contact);
				InterlockedDecrement(&sInfo.Accept);
				InterlockedDecrement(&sInfo.Work);
			}
			else
				contact++;
		}
		LeaveCriticalSection(&csListContact); // Выход из критической секции
		Sleep(1000);
	}

	cout << "GarbageCleaner остановлен" << endl;

	ExitThread(rc);
}
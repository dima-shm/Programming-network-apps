#pragma once
#include "Global.h"

using std::iterator;

DWORD WINAPI GarbageCleaner(LPVOID pPrm)
{
	cout << "GarbageCleaner запущен" << endl;

	DWORD rc = 0;

	try
	{
		while (*((TalkersCmd*)pPrm) != Exit && &Work>0)
		{
			EnterCriticalSection(&scListContact);

			for (list<Contact>::iterator first = listContacts.begin(); first != listContacts.end();)
			{
				if (first->sthread == Contact::FINISH || first->sthread == Contact::TIMEOUT || first->sthread == Contact::ABORT || first->type == Contact::EMPTY)
				{
					printf("IP -адрес удаленного клиента:-%s", inet_ntoa(first->prms.sin_addr));
					cout << " с кодом " << first->sthread << endl;

					if (first->type == Contact::EMPTY)
						InterlockedIncrement(&Fail);
					else
					{
						if (first->sthread == Contact::FINISH)
							InterlockedIncrement(&Finished);

						if (first->sthread == Contact::TIMEOUT)
							InterlockedIncrement(&Fail);

						if (first->sthread == Contact::ABORT)
							InterlockedIncrement(&Fail);

						CloseHandle(first->hthread);
						CloseHandle(first->htimer);
					}
					closesocket(first->s);
					first = listContacts.erase(first);
					InterlockedDecrement(&Work);
				}
				else
					first++;
			}
			LeaveCriticalSection(&scListContact);
			Sleep(1000);
		}
	}
	catch (string errorMsgText)
	{
		cout << errorMsgText << endl;
	}

	cout << "GarbageCleaner остановлен" << endl;

	ExitThread(rc);
}
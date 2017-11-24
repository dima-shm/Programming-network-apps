#pragma once
#include "Global.h"

DWORD WINAPI DispathServer(LPVOID pPrm)
{
	cout << "DispathServer запущен" << endl;

	DWORD rc = 0;

	while (*((TalkersCmd*)pPrm) != EXIT)
	{
		try
		{
			if (WaitForSingleObject(Event, 300) == WAIT_OBJECT_0)
			{
				if (&sInfo.Work > 0)
				{
					Contact *client = NULL;
					char buf[50];

					EnterCriticalSection(&csListContact);

					for (ListContact::iterator contact = listContacts.begin(); contact != listContacts.end(); contact++)
					{
						if (contact->type == Contact::ACCEPT)
						{
							client = &(*contact);

							bool Check = false;
							while (Check == false)
							{
								if (recv(client->s, buf, sizeof(buf), NULL) == SOCKET_ERROR)
									if (WSAGetLastError() == WSAEWOULDBLOCK)
										Sleep(100);
									else
										throw SetErrorMsgText("recv: ", WSAGetLastError());
								else
									Check = true;
							}


							if (strcmp(buf, "Rand") == 0 ||
								strcmp(buf, "Echo") == 0 ||
								strcmp(buf, "Time") == 0)
							{
								client->type = Contact::CONTACT;
								strcpy_s(client->srvname, buf);
								client->htimer = CreateWaitableTimer(NULL, false, NULL);
								_int64 time = -10000;
								SetWaitableTimer(client->htimer, (LARGE_INTEGER*)&time, 0, ASWTimer, client, false);

								client->hthread = tableService(buf, client);
								strcpy_s(client->msg, buf);

								Sleep(100);
								if (send(client->s, client->msg, sizeof(client->msg), NULL) == SOCKET_ERROR)
									throw SetErrorMsgText("send: ", WSAGetLastError());
							}
							else
							{
								if (send(client->s, "Error", sizeof("Error"), NULL) == SOCKET_ERROR)
									throw SetErrorMsgText("send: ", WSAGetLastError());
								closesocket(client->s);
								client->sthread = Contact::ABORT;
								CancelWaitableTimer(client->htimer);
								InterlockedIncrement(&sInfo.Fail);
							}
						}
					}
					LeaveCriticalSection(&csListContact);
				}
				SleepEx(0, true);
			}
			SleepEx(0, true);
		}
		catch (string errorMessage)
		{
			cout << errorMessage << endl;
		}
	}

	cout << "DispathServer остановлен" << endl;

	ExitThread(rc);
}
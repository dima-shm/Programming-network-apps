#pragma once
#include "Global.h"

DWORD WINAPI DispathServer(LPVOID pPrm)
{
	cout << "DispathServer запущен" << endl;

	DWORD rc = 0;

	try
	{
		while (*((TalkersCmd*)pPrm) != EXIT)
		{
			try
			{
				if (WaitForSingleObject(Event, 300) == WAIT_OBJECT_0)
				{
					if (&sInfo.Work > 0)
					{
						Contact *client = NULL;
						int libuf = 1;
						char CallBuf[50] = "",
							SendError[50] = "ErrorInquiry";

						EnterCriticalSection(&csListContact);

						for (ListContact::iterator p = listContacts.begin(); p != listContacts.end(); p++)
						{
							if (p->type == Contact::ACCEPT)
							{
								client = &(*p);

								bool Check = false;
								while (Check == false)
								{
									if ((libuf = recv(client->s, CallBuf, sizeof(CallBuf), NULL)) == SOCKET_ERROR)
									{
										switch (WSAGetLastError())
										{
										case WSAEWOULDBLOCK: Sleep(100); break;
										default: throw SetErrorMsgText("recv: ", WSAGetLastError());
										}
									}
									else
										Check = true;
								}

								if (strcmp(CallBuf, "Rand") == 0 || strcmp(CallBuf, "Echo") == 0 || strcmp(CallBuf, "Time") == 0)
									Check == true;
								else
									Check == false;

								if (Check == true)
								{
									client->type = Contact::CONTACT;
									strcpy_s(client->srvname, CallBuf);
									client->htimer = CreateWaitableTimer(NULL, false, NULL);
									_int64 time = -600000000;
									SetWaitableTimer(client->htimer, (LARGE_INTEGER*)&time, 0, ASWTimer, client, false);

									if ((libuf = send(client->s, CallBuf, sizeof(CallBuf), NULL)) == SOCKET_ERROR)
										throw SetErrorMsgText("send: ", WSAGetLastError());

									client->hthread = tableService(CallBuf, client);
								}

								else
								{
									if ((libuf = send(client->s, SendError, sizeof(SendError) + 1, NULL)) == SOCKET_ERROR)
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
			catch (string errorMsgText)
			{
				cout << errorMsgText << endl;
			}
		}
	}
	catch (string errorMessage)
	{
		cout << errorMessage << endl;
	}

	cout << "DispathServer остановлен" << endl;

	ExitThread(rc);
}
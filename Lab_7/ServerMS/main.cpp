#include <iostream>
#include <Windows.h>
#include <string>
#include <conio.h>

using namespace std;

int main()
{
	setlocale(0, "");

	HANDLE sH;
	DWORD rb;
	char buf[26];

	try
	{
		if ((sH = CreateMailslot(L"\\\\.\\mailslot\\Box",
			300, MAILSLOT_WAIT_FOREVER, NULL)) == INVALID_HANDLE_VALUE)
			throw "CreateMailslot exception";
		while (true)
		{
			if (!ReadFile(sH, buf, sizeof(buf), &rb, NULL))
				throw "ReadFile exception";

			cout << buf << endl;
		}
		CloseHandle(sH);
	}
	catch (string errorMessage)
	{
		cout << errorMessage << endl;
	}
	_getch();
}
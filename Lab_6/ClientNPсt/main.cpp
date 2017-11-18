#define _CRT_SECURE_NO_WARNINGS
#include "func_ErrorMsgText.h"

void main()
{
	setlocale(0, "");

	char buf[24] = "CHello from Client";
	DWORD lBuf;
	int numMsg;

	try
	{
		CallNamedPipe(L"\\\\.\\pipe\\Tube", buf, sizeof(buf), buf, sizeof(buf), &lBuf, 2000);
		
		cout << buf << endl;
	}
	catch (string errorMessage)
	{
		cout << errorMessage << endl;
	}
	_getch();
}
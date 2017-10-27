#pragma once
#include <windows.h>

HANDLE consoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);// дескриптор консоли

void setConsoleClrBlue()
{
	SetConsoleTextAttribute(consoleOutput, 0x0B);
}

void setConsoleClrWhite()
{
	SetConsoleTextAttribute(consoleOutput, 0x0F);
}

void setConsoleClrGreen()
{
	SetConsoleTextAttribute(consoleOutput, 0x0A);
}

void setConsoleClrRed()
{
	SetConsoleTextAttribute(consoleOutput, 0x0C);
}
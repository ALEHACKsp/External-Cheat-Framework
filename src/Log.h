#ifndef _Log_H_
#define _Log_H_

#include <Windows.h>
#include <stdio.h>
#include <utility>
#include "CustomString.h"

class CCheat;

class CLog
{
	// Members
public:

private:
	bool _Enabled;

	// Methods
public:
	CLog(bool);

	template<typename... Args>
	void PrintPlain(CCustomString String, Args... arguments) const
	{
		printf_s(String.String(), std::forward<Args>(arguments)...);
	}

	template<typename... Args>
	void Print(CCustomString String, Args... arguments) const
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
		printf_s("[+]");

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		printf_s(" ");

		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
		String += "\n";
		printf_s(String.String(), std::forward<Args>(arguments)...);

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	}

	template<typename... Args>
	void PrintWarning(CCustomString String, Args... arguments) const
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY | BACKGROUND_RED);
		printf_s("[!]");

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		printf_s(" ");

		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
		String += "\n";
		printf_s(String.String(), std::forward<Args>(arguments)...);

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	}

	template<typename... Args>
	void PrintError(CCustomString String, Args... arguments) const
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, BACKGROUND_RED);
		printf_s("[-]");

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		printf_s(" ");

		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		String += "\n";
		printf_s(String.String(), std::forward<Args>(arguments)...);

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
	}

private:

};

#endif // _Log_H_
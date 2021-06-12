#ifndef _Log_Hpp_
#define _Log_Hpp_

#include <Windows.h>
#include <stdio.h>
#include <utility>
#include "CustomString.hpp"

class CECF;

class CLog
{
	// Members
public:

private:
	bool _bEnabled;

	// Methods
public:
	CLog(bool);

	template<typename... Args>
	void PrintPlain(CString String, Args... arguments) const
	{
		if (!_bEnabled)
			return;

		String += "\n";
		printf_s(String.String(), std::forward<Args>(arguments)...);
	}

	template<typename... Args>
	void Print(CString String, Args... arguments) const
	{
		if (!_bEnabled)
			return;


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
	void PrintWarning(CString String, Args... arguments) const
	{
		if (!_bEnabled)
			return;


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
	void PrintError(CString String, Args... arguments) const
	{
		if (!_bEnabled)
			return;


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

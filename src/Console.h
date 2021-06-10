#ifndef _Console_H_
#define _Console_H_

#include <Windows.h>

class CCheat;

class CConsole
{
	// Members
public:

private:
	CCheat* _pParent;
	size_t _dwBufferSize;
	bool _bFirstDrawCall;

	// Methods
public:
	CConsole();

	void StartDraw();
	void EndDraw();
    DWORD GetNumCharsInConsoleBuffer();
    DWORD ReadConsoleBuffer(char* buffer, DWORD bufsize);

private:

};

#endif // _Console_H_
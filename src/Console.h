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
	char* _Buffer;
	bool _bFirstDrawCall;
	COORD _DrawPosition;
	int _nDrawLines;

	// Methods
public:
	CConsole(CCheat*);
	~CConsole();

	void SetCursor(int column, int line);

	void StartDraw();
	void EndDraw();
    DWORD GetNumCharsInConsoleBuffer();
    DWORD ReadConsoleBuffer(char* buffer, DWORD bufsize);

private:

};

#endif // _Console_H_
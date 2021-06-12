#ifndef _Console_Hpp_
#define _Console_Hpp_

#include <Windows.h>

class CECF;

class CConsole
{
	// Members
public:

private:
	CECF* _pParent;
	size_t _dwBufferSize;
	char* _Buffer;
	bool _bFirstDrawCall;
	COORD _DrawPosition;
	int _nDrawLines;

	// Methods
public:
	CConsole(CECF*);
	~CConsole();

	void SetCursor(int column, int line);

	void StartDraw();
	void EndDraw();
    DWORD GetNumCharsInConsoleBuffer();
    DWORD ReadConsoleBuffer(char* buffer, DWORD bufsize);

private:

};

#endif // _Console_Hpp_
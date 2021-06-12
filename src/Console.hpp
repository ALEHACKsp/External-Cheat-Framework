#ifndef _Console_Hpp_
#define _Console_Hpp_

#include <Windows.h>

class CECF;

/// <summary> The ECF Debugger Console class. </summary>
class CConsole
{
	// Members
public:

private:
	/// <summary> A Pointer to this Objects' Parent CCheat class. </summary>
	CECF* _pParent;

	/// <summary> Used to store the size of the debug area buffer in the console whenever GetNumCharsInConsoleBuffer() is called. </summary>
	size_t _dwBufferSize;

	/// <summary> unused. </summary>
	char* _Buffer;

	/// <summary> Set to whether the debugger console has made it's first draw call. </summary>
	bool _bFirstDrawCall;

	/// <summary> Set automatically to the starting line for the debugger console print(er) when the first draw call is made. </summary>
	COORD _DrawPosition;

	/// <summary> Set automatically to the amount of lines the debugging console will take up to assist with flushing the buffer. </summary>
	int _nDrawLines;

	// Methods
public:
	/// <summary> .... </summary>
	/// <param name="bar"> .... </param>
	CConsole(CECF*);

	/// <summary> .... </summary>
	~CConsole();

	/// <summary> .... </summary>
	/// <param name="bar"> .... </param>
	void SetCursor(int Column, int Line);

	/// <summary> .... </summary>
	void StartDraw();

	/// <summary> .... </summary>
	void EndDraw();

	/// <summary> .... </summary>
	/// <returns> .... </returns>
    DWORD GetNumCharsInConsoleBuffer();

	/// <summary> .... </summary>
	/// <param name="Buffer"> .... </param>
	/// <param name="BufferSize"> .... </param>
	/// <returns> .... </returns>
    DWORD ReadConsoleBuffer(char* Buffer, DWORD BufferSize);

private:

};

#endif // _Console_Hpp_

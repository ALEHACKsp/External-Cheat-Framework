#include "Console.h"
#include "Cheat.h"
#include "Log.h"
#include "CustomString.h"

CConsole::CConsole(CCheat* pParent)
    : _pParent{ pParent }, _dwBufferSize{ 0 }, _Buffer{ const_cast<char*>("") }, _bFirstDrawCall{ true }
{
}

CConsole::~CConsole()
{
    delete[] _Buffer;
}

void CConsole::SetCursor(int column, int line)
{
    COORD coord;

    coord.X = column;
    coord.Y = line;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void CConsole::StartDraw()
{
    if (_bFirstDrawCall)
    {
        CONSOLE_SCREEN_BUFFER_INFO BufferInfo;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufferInfo);
        _DrawPosition = BufferInfo.dwCursorPosition;
        _DrawPosition.Y += 3;

        /*_dwBufferSize = GetNumCharsInConsoleBuffer();
        _Buffer = new char[_dwBufferSize + 1];
        memset(_Buffer, 0, _dwBufferSize);

        if (!ReadConsoleBuffer(_Buffer, _dwBufferSize))
            return;*/

        _bFirstDrawCall = false;
    }
    else
    {
        SetCursor(_DrawPosition.X, _DrawPosition.Y - 2);
        _pParent->Log()->PrintPlain("[--------------------------------------------------------]\n");
    }

    for (int i = 0; i < _nDrawLines; i++)
    {
        printf("                                                                                                   \n");
    }

    if (_bFirstDrawCall)
    {
        SetCursor(_DrawPosition.X, _DrawPosition.Y - 2);
        _pParent->Log()->PrintPlain("[--------------------------------------------------------]\n");
    }

    SetCursor(_DrawPosition.X, _DrawPosition.Y);
}

void CConsole::EndDraw()
{
    CONSOLE_SCREEN_BUFFER_INFO BufferInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufferInfo);

    _nDrawLines = -_DrawPosition.Y + BufferInfo.dwCursorPosition.Y;

    _pParent->Log()->PrintPlain("\n[--------------------------------------------------------]\n");
}

DWORD CConsole::GetNumCharsInConsoleBuffer()
{
    CONSOLE_SCREEN_BUFFER_INFO BufferInfo = { 0 };

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufferInfo) != FALSE)
    {
        return static_cast<DWORD>((BufferInfo.dwSize.X * (BufferInfo.dwCursorPosition.Y + 1)) - (BufferInfo.dwSize.X - (BufferInfo.dwCursorPosition.X)));
    }
    else
    {
        return 0;
    }
}

DWORD CConsole::ReadConsoleBuffer(char* Buffer, DWORD BufferSize)
{
    DWORD NumberOfCharactersRead = 0;
    COORD FirstCharacterToRead = { 0 };

    if (ReadConsoleOutputCharacterA(GetStdHandle(STD_OUTPUT_HANDLE), Buffer, BufferSize, FirstCharacterToRead, &NumberOfCharactersRead) != FALSE)
    {
        Buffer[BufferSize - 1] = '\0';
    }
    else
    {
        Buffer[0] = '\0';
    }

    return NumberOfCharactersRead;
}
#include "Console.h"
#include "Cheat.h"

CConsole::CConsole()
    : _dwBufferSize{ 0 }, _bFirstDrawCall{ true }
{
}

void CConsole::StartDraw()
{
    if (_bFirstDrawCall)
    {
        _dwBufferSize = GetNumCharsInConsoleBuffer();
        _bFirstDrawCall = false;
    }
    
    if (_dwBufferSize > 0)
    {
        char* Buffer = new char[_dwBufferSize + 1];

        memset(Buffer, 0, _dwBufferSize);

        if (!ReadConsoleBuffer(Buffer, _dwBufferSize))
            return;

        delete [] Buffer;
    }

    
}

void CConsole::EndDraw()
{
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
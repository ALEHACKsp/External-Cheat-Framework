#ifndef _ProcessEntry_Hpp_
#define _ProcessEntry_Hpp_

#include <Windows.h>
#include <TlHelp32.h>

class CProcess;

class CProcessEntry
{
    friend CProcess;

    // ProcessEntry Members
public:

private:
    PROCESSENTRY32 _Process;

    // ProcessEntry Methods
public:
    CProcessEntry();
    ~CProcessEntry();

    PROCESSENTRY32* Process();

private:

};

#endif // _ProcessEntry_Hpp_
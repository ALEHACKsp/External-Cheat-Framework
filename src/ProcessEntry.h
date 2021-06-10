#ifndef _ProcessEntry_H_
#define _ProcessEntry_H_

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

#endif // _ProcessEntry_H_
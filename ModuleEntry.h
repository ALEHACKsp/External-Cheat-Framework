#ifndef _ModuleEntry_H_
#define _ModuleEntry_H_

#include <Windows.h>
#include <TlHelp32.h>

class CProcess;

class CModuleEntry
{
    friend CProcess;

    // Members
public:

private:
    MODULEENTRY32 _Module;

    // Methods
public:
    CModuleEntry();
    ~CModuleEntry();

    MODULEENTRY32* Module();

private:

};

#endif // _ModuleEntry_H_
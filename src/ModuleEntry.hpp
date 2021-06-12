#ifndef _ModuleEntry_Hpp_
#define _ModuleEntry_Hpp_

#include <Windows.h>
#include <TlHelp32.h>

class CProcess;

/// <summary> Wrapper Class for a MODULEENTRY32 Object </summary>
class CModuleEntry
{
    friend CProcess;

    // Members
public:

private:
    /// <summary> Module Entry Object </summary>
    MODULEENTRY32 _Module;

    // Methods
public:
    CModuleEntry();             // Constructor
    ~CModuleEntry();            // Destructor

    /// <returns> Module Entry Object </returns>
    MODULEENTRY32* Module();

private:

};

#endif // _ModuleEntry_Hpp_
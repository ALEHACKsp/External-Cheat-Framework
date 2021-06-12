#ifndef _ModuleEntry_Hpp_
#define _ModuleEntry_Hpp_

#include <Windows.h>
#include <TlHelp32.h>

class CProcess;

/// <summary> Wrapper Class for a MODULEENTRY32 Object. </summary>
class CModuleEntry
{
    friend CProcess;

    // Members
public:

private:
    /// <summary> MODULEENTRY32 Object. </summary>
    MODULEENTRY32 _Module;

    // Methods
public:
    /// <summary> The no-args CModuleEntry constructor. </summary>
    CModuleEntry();

    /// <summary> The CModuleEntry destructor. </summary>
    ~CModuleEntry();

    /// <returns> A pointer to the MODULEENTRY32 Object. </returns>
    MODULEENTRY32* Module();

private:

};

#endif // _ModuleEntry_Hpp_
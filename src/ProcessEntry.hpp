#ifndef _ProcessEntry_Hpp_
#define _ProcessEntry_Hpp_

#include <Windows.h>
#include <TlHelp32.h>

class CProcess;

/// <summary> Wrapper Class for a MODULEENTRY32 Object. </summary>
class CProcessEntry
{
    friend CProcess;

    // ProcessEntry Members
public:

private:
    /// <summary> PROCESSENTRY32 Object. </summary>
    PROCESSENTRY32 _Process;

    // ProcessEntry Methods
public:
    /// <summary> The no-args CProcessEntry constructor. </summary>
    CProcessEntry();

    /// <summary> The CProcessEntry destructor. </summary>
    ~CProcessEntry();

    /// <returns> A pointer to the PROCESSENTRY32 Object. </returns>
    PROCESSENTRY32* Process();

private:

};

#endif // _ProcessEntry_Hpp_

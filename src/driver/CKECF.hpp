#ifndef _CKECF_Hpp_
#define _CKECF_Hpp_

#include <ntifs.h>

class CUtility;
class CMemory;

class CKECF
{
	// Members
public:

private:
	CUtility* _pUtility;
	CMemory* _pMemory;

	// Methods
public:

private:
	CKECF();
	~CKECF();

	CUtility* Utility() const;
	CMemory* Memory() const;
	NTSTATUS RegisterCallbacks(const wchar_t* Module);
	NTSTATUS DriverCallback(void* Context, void* Reason, void* KeyData);
};

#endif // !_CKECF_Hpp_
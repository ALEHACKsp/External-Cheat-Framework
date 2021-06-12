#ifndef _Memory_Hpp_
#define _Memory_Hpp_

#include <ntifs.h>

typedef struct LDR_DATA_TABLE_ENTRY;

class CMemory
{
	// Members
public:

private:


	// Methods
public:
	CMemory();
	~CMemory();

	DWORD64 FindPattern(LDR_DATA_TABLE_ENTRY* Module, const char* Signature, const char* Mask) const;

	void WriteLocalMemory(PEPROCESS LocalProcess, void* Data, void* DataDestination, size_t Size);

private:

};

#endif // !_Memory_Hpp_
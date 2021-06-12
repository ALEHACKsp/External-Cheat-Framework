#include "Memory.hpp"
#include "Windows.hpp"

CMemory::CMemory()
{
}

CMemory::~CMemory()
{
}

DWORD64 CMemory::FindPattern(LDR_DATA_TABLE_ENTRY* Module, const char* Signature, const char* Mask) const
{
	for (DWORD64 i = 0; i < Module->SizeOfImage; i++)
	{
		if ([](BYTE const* Data, BYTE const* Signature, char const* Mask)
		{
			for (; *Mask; ++Mask, ++Data, ++Signature)
			{
				if (*Mask == 'x' && *Data != *Signature) return false;
			}

			return (*Mask) == 0;
		} (reinterpret_cast<BYTE*>(reinterpret_cast<DWORD64>(Module->DllBase) + i), reinterpret_cast<BYTE*>(const_cast<char*>(Signature)), Mask))

		return reinterpret_cast<DWORD64>(Module->DllBase) + i;
	}
}

void CMemory::WriteLocalMemory(PEPROCESS LocalProcess, void* Data, void* DataDestination, size_t Size)
{
	if (!Data)
		return;

	if (!LocalProcess)
		return;

	KAPC_STATE APC;

	if (LocalProcess != IoGetCurrentProcess())
		KeStackAttachProcess(LocalProcess, &APC);

	memcpy(DataDestination, Data, Size);

	if (LocalProcess != IoGetCurrentProcess())
		KeUnstackDetachProcess(&APC);
}
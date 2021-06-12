#include "Global.hpp"
#include "Windows.hpp"
#include "Raid.hpp"
#include "Shared.hpp"
#include "Memory.hpp"
#include "Utility.hpp"
#include <string>
#include <memory>

NTSTATUS DriverMain()
{
	if (!*reinterpret_cast<void**>(std::uintptr_t(PsLoadedModuleList) + 0x30)) // NTOSKrnl Base Address
		return STATUS_UNSUCCESSFUL;

	const wchar_t* CallbackDrivers[5] = { L"ndis.sys", L"ntfs.sys", L"tcpip.sys", L"fltmgr.sys", L"dxgkrnl.sys" };

	for (INT i = 0; i < 5; ++i)
		if (pGlobal-> RegisterCallbacks(CallbackDrivers[i]) != STATUS_UNSUCCESSFUL)
			return STATUS_SUCCESS;

	return STATUS_UNSUCCESSFUL;
}

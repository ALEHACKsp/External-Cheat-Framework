#include "Global.hpp"
#include "ECFrameworkDriver.hpp"
#include "Windows.hpp"
#include "Raid.hpp"
#include "Shared.hpp"
#include "Memory.hpp"
#include "Utility.hpp"
#include <string>
#include <memory>

NTSTATUS DriverMain()
{
	CECFrameworkDriver* pECFramework = new CECFrameworkDriver;
	pGlobal = new CGlobal(pECFramework);

	if (!*reinterpret_cast<void**>(DWORD64(PsLoadedModuleList) + 0x30)) // NTOSKrnl Base Address
		return STATUS_UNSUCCESSFUL;

	const wchar_t* CallbackDrivers[5] = { L"ndis.sys", L"ntfs.sys", L"tcpip.sys", L"fltmgr.sys", L"dxgkrnl.sys" };

	for (size_t i = 0; i < 5; ++i)
		if (pECFramework->RegisterCallbacks(CallbackDrivers[i]) != STATUS_UNSUCCESSFUL)
			return STATUS_SUCCESS;

	return STATUS_UNSUCCESSFUL;
}

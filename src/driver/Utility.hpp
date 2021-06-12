#ifndef _Utility_Hpp_
#define _Utility_Hpp_

#include <ntifs.h>
#include <tuple>
#include <random>
#include <cstdint>
#include <memory>
#include <cstdarg>

using ProcessReference = std::unique_ptr<std::remove_pointer_t<PEPROCESS>, decltype(&ObfDereferenceObject)>;
using DriverReference = std::unique_ptr<std::remove_pointer_t<PDRIVER_OBJECT>, decltype(&ObfDereferenceObject)>;

struct LDR_DATA_TABLE_ENTRY;

class CUtility
{
	// Members
public:

private:


	// Methods
public:
	LDR_DATA_TABLE_ENTRY* SystemModule(const wchar_t* ModuleName);
	void* SystemRoutine(const wchar_t* RoutineName);
	void* SystemExport(const wchar_t* ModuleName, const char* ExportName);
	auto IsSpecificSection(IMAGE_SECTION_HEADER Section, const char* Target) -> bool;
	NTSTATUS ForceDeleteFile(UNICODE_STRING* pUsDriverPath);
	BOOLEAN DoesFileExist(UNICODE_STRING Path);
	auto CharToUnicode(const char* String)->UNICODE_STRING;
	void* TrampolineHook(void* BaseAddress, const char* TargetAddress);
	ProcessReference GetProcessReference(std::uintptr_t PID);
	DriverReference GetDriverReference(const wchar_t* DriverName);

private:

};

#endif // !_Utility_Hpp_
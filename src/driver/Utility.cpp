#include "Utility.hpp"
#include "Memory.hpp"
#include "Windows.hpp"

using ProcessReference = std::unique_ptr<std::remove_pointer_t<PEPROCESS>, decltype(&ObfDereferenceObject)>;
using DriverReference = std::unique_ptr<std::remove_pointer_t<PDRIVER_OBJECT>, decltype(&ObfDereferenceObject)>;

LDR_DATA_TABLE_ENTRY* CUtility::SystemModule(const wchar_t* ModuleName)
{
	LDR_DATA_TABLE_ENTRY* ModuleEntry{ nullptr };
	UNICODE_STRING UnicodeModuleName;

	RtlInitUnicodeString(&UnicodeModuleName, ModuleName);

	for (auto Entry = PsLoadedModuleList; Entry != PsLoadedModuleList->Blink; Entry = Entry->Flink)
	{
		LDR_DATA_TABLE_ENTRY* data_table = CONTAINING_RECORD(Entry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

		if (RtlEqualUnicodeString(&data_table->BaseDllName, &UnicodeModuleName, true))
		{
			ModuleEntry = data_table;

			break;
		}
	}

	return ModuleEntry;
}

void* CUtility::SystemRoutine(const wchar_t* RoutineName)
{
	UNICODE_STRING UnicodeRoutineName{ };

	RtlInitUnicodeString(&UnicodeRoutineName, RoutineName);

	return MmGetSystemRoutineAddress(&UnicodeRoutineName);
}

void* CUtility::SystemExport(const wchar_t* ModuleName, const char* ExportName)
{
	LDR_DATA_TABLE_ENTRY* Module = SystemModule(ModuleName);

	if (!Module)
		return nullptr;

	return RtlFindExportedRoutineByName(Module->DllBase, ExportName);
}

auto CUtility::IsSpecificSection(IMAGE_SECTION_HEADER Section, const char* Target) -> bool
{
	if (!_stricmp(reinterpret_cast<const char*>(Section.Name), Target))
		return true;

	return false;
}

NTSTATUS CUtility::ForceDeleteFile(UNICODE_STRING* pUsDriverPath)
{
	IO_STATUS_BLOCK IoStatusBlock;
	HANDLE FileHandle;
	PFILE_OBJECT FileObject;
	OBJECT_ATTRIBUTES ObjectAttributes;

	InitializeObjectAttributes(&ObjectAttributes, pUsDriverPath, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, 0, 0);

	if (!NT_SUCCESS(IoCreateFileEx(&FileHandle, SYNCHRONIZE | DELETE, &ObjectAttributes, &IoStatusBlock, nullptr, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_DELETE,
		FILE_OPEN, FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT, nullptr, 0, CreateFileTypeNone, nullptr, IO_NO_PARAMETER_CHECKING, nullptr)))
		return static_cast<NTSTATUS>(false);

	if (!NT_SUCCESS(ObReferenceObjectByHandleWithTag(FileHandle, SYNCHRONIZE | DELETE, *IoFileObjectType, KernelMode, POOL_TAG_USE, reinterpret_cast<PVOID*>(&FileObject), nullptr)))
	{
		ObCloseHandle(FileHandle, KernelMode);

		return static_cast<NTSTATUS>(false);
	}

	PSECTION_OBJECT_POINTERS SectionObjectPointer = FileObject->SectionObjectPointer; SectionObjectPointer->ImageSectionObject = nullptr;
	BOOLEAN ImageSectionFlushed = MmFlushImageSection(SectionObjectPointer, MmFlushForDelete);

	ObfDereferenceObject(FileObject);
	ObCloseHandle(FileHandle, KernelMode);

	if (ImageSectionFlushed)
		if (NT_SUCCESS(ZwDeleteFile(&ObjectAttributes)))
			return static_cast<NTSTATUS>(true);

	return static_cast<NTSTATUS>(false);
}

BOOLEAN CUtility::DoesFileExist(UNICODE_STRING Path)
{
	HANDLE hFile;
	IO_STATUS_BLOCK ioStatusBlock;
	OBJECT_ATTRIBUTES objAttr;

	InitializeObjectAttributes(&objAttr, &Path, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

	if (STATUS_SUCCESS(ZwOpenFile(&hFile, GENERIC_READ, &objAttr, &ioStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL)) && hFile != NULL) 
	{
		ZwClose(hFile);

		return TRUE;
	}

	return FALSE;
}

auto CUtility::CharToUnicode(const char* String) -> UNICODE_STRING
{
	UNICODE_STRING UnicodeString;
	ANSI_STRING AnsiString;

	RtlInitAnsiString(&AnsiString, String);
	RtlAnsiStringToUnicodeString(&UnicodeString, &AnsiString, TRUE);

	return UnicodeString;
}

void* CUtility::TrampolineHook(void* BaseAddress, const char* TargetAddress)
{
	PIMAGE_NT_HEADERS BaseImageHeader = RtlImageNtHeader(BaseAddress);

	if (!BaseImageHeader)
		return nullptr;

	PIMAGE_SECTION_HEADER BaseImageSectionHeader = reinterpret_cast<PIMAGE_SECTION_HEADER>(BaseImageHeader + 1);

	for (auto Section = 0; Section < BaseImageHeader->FileHeader.NumberOfSections; Section++)
	{
		IMAGE_SECTION_HEADER CurrentHeader = BaseImageSectionHeader[Section];

		if (CurrentHeader.VirtualAddress == 0 || CurrentHeader.Misc.VirtualSize == 0)
			continue;

		if (!(IsSpecificSection(CurrentHeader, TargetAddress)))
			continue;

		char* SectionAddress = reinterpret_cast<char*>(BaseAddress) + CurrentHeader.VirtualAddress;

		for (char* Address = SectionAddress; Address < (SectionAddress + CurrentHeader.SizeOfRawData) - 1; ++Address)
		{
			if (!Address)
				continue;

			if (*reinterpret_cast<std::uint16_t*>(Address) == 0xe1ff)
				return Address;
		}
	}

	return nullptr;
}

ProcessReference CUtility::GetProcessReference(std::uintptr_t PID)
{
	PEPROCESS process{ };

	if (!NT_SUCCESS(PsLookupProcessByProcessId(reinterpret_cast<HANDLE>(PID), &process)))
		return ProcessReference(nullptr, nullptr);

	return ProcessReference(process, &ObfDereferenceObject);
}

DriverReference CUtility::GetDriverReference(const wchar_t* DriverName)
{
	UNICODE_STRING UnicodeDriverName;
	RtlInitUnicodeString(&UnicodeDriverName, DriverName);

	PDRIVER_OBJECT DriverObject{ nullptr };
	ObReferenceObjectByName(&UnicodeDriverName, OBJ_CASE_INSENSITIVE, nullptr, 0, *IoDriverObjectType, KernelMode, nullptr, reinterpret_cast<void**>(&DriverObject));

	return DriverReference(DriverObject, &ObfDereferenceObject);
}
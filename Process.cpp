#include "Cheat.h"
#include "Process.h"
#include "CustomString.h"
#include "ProcessEntry.h"
#include "ModuleEntry.h"
#include "Log.h"

CProcess::CProcess(CCheat* pParent)
	: _pParent{ pParent }, _hTargetProcess{ static_cast<HANDLE>(0) }, _dwTargetProcessID{ 0 }
{
}

CProcess::~CProcess()
{
	CloseHandle(_hTargetProcess);
	_dwTargetProcessID = 0;
	delete _ExeName;
}

bool CProcess::Attach(CCustomString ProcessName)
{
	HANDLE hProcessSnap{ NULL };
	HANDLE hProcess{ NULL };
	CProcessEntry ProcessEntry{ };
	
	if ((hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL)) == INVALID_HANDLE_VALUE)
	{
		_pParent->Log()->PrintError("CProcess::Attach(): CreateToolhelp32Snapshot Failed");

		return false;
	}

	if (!Process32First(hProcessSnap, &ProcessEntry._Process))
	{
		_pParent->Log()->PrintError("CProcess::Attach(): Process32FirstW Failed");
		CloseHandle(hProcessSnap);

		return false;
	}

	do
	{
		if ((hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessEntry.Process()->th32ProcessID)) != NULL)
		{
			if (!strcmp(ProcessEntry.Process()->szExeFile, ProcessName.String()))
			{
				_dwTargetProcessID = ProcessEntry.Process()->th32ProcessID;
				_hTargetProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, _dwTargetProcessID);

				if (_hTargetProcess == NULL)
				{
					_pParent->Log()->PrintError("CProcess::Attach(): OpenProcess Failed");

					return false;
				}

				_ExeName = new CCustomString(ProcessEntry.Process()->szExeFile);
				CloseHandle(hProcess);

				return true;
			}
		}
	} while (Process32Next(hProcessSnap, &ProcessEntry._Process));

	_pParent->Log()->PrintError("CProcess::Attach(): %s Not Found!", ProcessName.String());

	return false;
}

CModuleEntry CProcess::GetModuleInformation(CCustomString ModuleName) const
{
	HANDLE hModuleSnap{ NULL };
	CModuleEntry ModuleEntry{ };

	if ((hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, _dwTargetProcessID)) == INVALID_HANDLE_VALUE)
	{
		_pParent->Log()->PrintError("CProcess::GetModuleInformation(): CreateToolhelp32Snapshot Failed");

		return CModuleEntry();
	}

	do
	{
		if (!strcmp(ModuleEntry.Module()->szModule, ModuleName.String()))
		{
			CloseHandle(hModuleSnap);

			return ModuleEntry;
		}
	} while (Module32Next(hModuleSnap, &ModuleEntry._Module));

	CloseHandle(hModuleSnap);

	_pParent->Log()->PrintError("CProcess::GetModuleInformation(): %s Not Found!", ModuleName.String());

	return CModuleEntry();
}

DWORD64 CProcess::FindSignature(DWORD64 BaseAddress, DWORD64 Length, BYTE* Signature, BYTE* Mask) const
{
	MEMORY_BASIC_INFORMATION MemoryRegionInfo = { 0 };
	DWORD64 RegionOffset{ 0 };

	do
	{
		VirtualQueryEx(_hTargetProcess, reinterpret_cast<LPCVOID>(BaseAddress + RegionOffset), &MemoryRegionInfo, sizeof(MEMORY_BASIC_INFORMATION));

		if (MemoryRegionInfo.State != MEM_FREE)
		{
			BYTE* RegionStoreBuffer = new BYTE[MemoryRegionInfo.RegionSize];

			ReadProcessMemory(_hTargetProcess, MemoryRegionInfo.BaseAddress, RegionStoreBuffer, MemoryRegionInfo.RegionSize, NULL);

			for (size_t i = 0; i < MemoryRegionInfo.RegionSize; i++)
			{
				if (DataCompare(RegionStoreBuffer + i, Signature, Mask))
				{
					delete[] RegionStoreBuffer;

					return reinterpret_cast<DWORD64>(MemoryRegionInfo.BaseAddress) + i;
				}
			}

			delete[] RegionStoreBuffer;
		}

		RegionOffset += MemoryRegionInfo.RegionSize;
	} while (RegionOffset < Length);

	return 0;
}

bool CProcess::DataCompare(BYTE* Data, BYTE* Signature, BYTE* Mask) const
{
	for (; *Mask; Mask++, Signature++, Data++)
		if (*Mask == 'x' && *Data != *Signature)
			return false;

	return true;
}

template <class T>
T CProcess::Read(DWORD64 dwAddress)
{
	T Result;

	ReadProcessMemory(_hTargetProcess, (LPVOID)dwAddress, &Result, sizeof(T), NULL);

	return Result;
}

template <class T>
void CProcess::Write(DWORD64 dwAddress, T Value)
{
	WriteProcessMemory(_hTargetProcess, (LPVOID)dwAddress, &Value, sizeof(T), NULL);
}

HANDLE CProcess::hTargetProcess() const
{
	return _hTargetProcess;
}

DWORD CProcess::dwTargetProcessID() const
{
	return _dwTargetProcessID;
}

CCustomString CProcess::ExeName() const
{
	return *_ExeName;
}
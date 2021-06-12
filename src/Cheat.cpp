#include "Globals.h"
#include "Cheat.h"
#include "Log.h"
#include "Process.h"
#include "Offsets.h"
#include "Console.h"
#include "ModuleEntry.h"
#include <ctime>
#include <string>

CCheat::CCheat()
	: _pLog{ new CLog(true) }, _pProcess{ new CProcess(this) }, _pOffsets{ new COffsets(this) }, 
	_pConsole{ new CConsole(this) }, _bRunning{ true }, _dwBaseModule{ 0 }, _dwBaseSize{ 0 },
	DeltaTimeStart{ 0 }, DeltaTimeEnd{ 0 }
{
	pGlobals->_gpCheat = this;
}

CCheat::~CCheat()
{
	delete _pLog;
	delete _pProcess;
	delete _pOffsets;
}

CLog* CCheat::Log() const
{
	return _pLog;
}

CProcess* CCheat::Process() const
{
	return _pProcess;
}

COffsets* CCheat::Offsets() const
{
	return _pOffsets;
}

CConsole* CCheat::Console() const
{
	return _pConsole;
}

bool CCheat::Running() const
{
	return _bRunning;
}

int CCheat::DeltaTime() const
{
	return DeltaTimeEnd - DeltaTimeStart;
}

void CCheat::StartDeltaTime()
{
	DeltaTimeStart = clock();
}

void CCheat::EndDeltaTime()
{
	DeltaTimeEnd = clock();
}

DWORD CCheat::BaseModule() const
{
	return _dwBaseModule;
}

DWORD CCheat::BaseSize() const
{
	return _dwBaseSize;
}


bool CCheat::GetGameModule(CString GameName)
{
	CModuleEntry ModuleEntry = _pProcess->GetModuleInformation(GameName);

	if (ModuleEntry.Module()->modBaseSize == 0)
		return false;

	_dwBaseModule = reinterpret_cast<DWORD>(ModuleEntry.Module()->modBaseAddr);
	_dwBaseSize = ModuleEntry.Module()->modBaseSize;

	return true;
}

void CCheat::GetGameData()
{
	// Game Specific

}

void CCheat::Draw()
{
	// Game Specific

}
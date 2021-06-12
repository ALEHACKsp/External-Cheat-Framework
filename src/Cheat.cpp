#include "Globals.hpp"
#include "Cheat.hpp"
#include "Log.hpp"
#include "Process.hpp"
#include "Offsets.hpp"
#include "Console.hpp"
#include "ModuleEntry.hpp"
#include <ctime>
#include <string>

CECF::CECF()
	: _pLog{ new CLog(true) }, _pProcess{ new CProcess(this) }, _pOffsets{ new COffsets(this) }, 
	_pConsole{ new CConsole(this) }, _bRunning{ true }, _dwBaseModule{ 0 }, _dwBaseSize{ 0 },
	_iDeltaTimeStart{ 0 }, _iDeltaTimeEnd{ 0 }
{
	pGlobals->_gpCheat = this;
}

CECF::~CECF()
{
	delete _pLog;
	delete _pProcess;
	delete _pOffsets;
}

CLog* CECF::Log() const
{
	return _pLog;
}

CProcess* CECF::Process() const
{
	return _pProcess;
}

COffsets* CECF::Offsets() const
{
	return _pOffsets;
}

CConsole* CECF::Console() const
{
	return _pConsole;
}

bool CECF::Running() const
{
	return _bRunning;
}

int CECF::DeltaTime() const
{
	return _iDeltaTimeEnd - _iDeltaTimeStart;
}

void CECF::StartDeltaTime()
{
	_iDeltaTimeStart = clock();
}

void CECF::EndDeltaTime()
{
	_iDeltaTimeEnd = clock();
}

DWORD CECF::BaseModule() const
{
	return _dwBaseModule;
}

DWORD CECF::BaseSize() const
{
	return _dwBaseSize;
}


bool CECF::GetGameModule(CString ModuleName)
{
	CModuleEntry ModuleEntry = _pProcess->GetModuleInformation(ModuleName);

	if (ModuleEntry.Module()->modBaseSize == 0)
		return false;

	_dwBaseModule = reinterpret_cast<DWORD>(ModuleEntry.Module()->modBaseAddr);
	_dwBaseSize = ModuleEntry.Module()->modBaseSize;

	return true;
}

void CECF::GetSetGameData()
{
	// Game Specific

}

void CECF::Draw()
{
	// Game Specific

}
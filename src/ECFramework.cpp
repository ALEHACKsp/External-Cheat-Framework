#include "Globals.hpp"
#include "ECFramework.hpp"
#include "Log.hpp"
#include "Process.hpp"
#include "Offsets.hpp"
#include "Console.hpp"
#include "ModuleEntry.hpp"
#include <ctime>
#include <string>

CECFramework::CECFramework()
	: _pLog{ new CLog(true) }, _pProcess{ new CProcess(this) }, _pOffsets{ new COffsets(this) }, 
	_pConsole{ new CConsole(this) }, _bRunning{ true }, _dwBaseModule{ 0 }, _dwBaseSize{ 0 },
	_iDeltaTimeStart{ 0 }, _iDeltaTimeEnd{ 0 }
{
	pGlobals->_gpCheat = this;
}

CECFramework::~CECFramework()
{
	delete _pLog;
	delete _pProcess;
	delete _pOffsets;
}

CLog* CECFramework::Log() const
{
	return _pLog;
}

CProcess* CECFramework::Process() const
{
	return _pProcess;
}

COffsets* CECFramework::Offsets() const
{
	return _pOffsets;
}

CConsole* CECFramework::Console() const
{
	return _pConsole;
}

bool CECFramework::Running() const
{
	return _bRunning;
}

int CECFramework::DeltaTime() const
{
	return _iDeltaTimeEnd - _iDeltaTimeStart;
}

void CECFramework::StartDeltaTime()
{
	_iDeltaTimeStart = clock();
}

void CECFramework::EndDeltaTime()
{
	_iDeltaTimeEnd = clock();
}

DWORD CECFramework::BaseModule() const
{
	return _dwBaseModule;
}

DWORD CECFramework::BaseSize() const
{
	return _dwBaseSize;
}


bool CECFramework::GetGameModule(CString ModuleName)
{
	CModuleEntry ModuleEntry = _pProcess->GetModuleInformation(ModuleName);

	if (ModuleEntry.Module()->modBaseSize == 0)
		return false;

	_dwBaseModule = reinterpret_cast<DWORD>(ModuleEntry.Module()->modBaseAddr);
	_dwBaseSize = ModuleEntry.Module()->modBaseSize;

	return true;
}

void CECFramework::GetSetGameData()
{
	// Game Specific

}

void CECFramework::Draw()
{
	// Game Specific

}
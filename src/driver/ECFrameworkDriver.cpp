#include "ECFrameworkDriver.hpp"
#include "Global.hpp"
#include "Log.h"
#include "Memory.hpp"
#include "Shared.hpp"
#include "Socket.h"
#include "Utility.hpp"
#include "Windows.hpp"

CECFrameworkDriver::CECFrameworkDriver()
	: _pUtility{ new CUtility }, _pMemory{ new CMemory }, _pLog{ new CLog }, _pSocket{ new CSocket }
{
	pGlobal = new CGlobal(this);
}

CECFrameworkDriver::~CECFrameworkDriver()
{
	delete _pUtility;
	delete _pMemory;
	delete _pLog;
	delete _pSocket;
	pGlobal->_pECFrameworkDriver = nullptr;
}

CUtility* CECFrameworkDriver::Utility() const
{
	return _pUtility;
}

CMemory* CECFrameworkDriver::Memory() const
{
	return _pMemory;
}

CLog* CECFrameworkDriver::Log() const
{
	return _pLog;
}

CSocket* CECFrameworkDriver::Socket() const
{
	return _pSocket;
}

bool CECFrameworkDriver::bShutdown() const
{
	return _bShutdown;
}

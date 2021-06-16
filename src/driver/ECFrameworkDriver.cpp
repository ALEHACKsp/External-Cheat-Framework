#include "ECFrameworkDriver.hpp"
#include "Log.h"
#include "Memory.hpp"
#include "Shared.hpp"
#include "Utility.hpp"
#include "Windows.hpp"

CECFrameworkDriver::CECFrameworkDriver()
	: _pUtility{ new CUtility }, _pMemory{ new CMemory }, _pLog{ new CLog }, _pSocket{ new CSocket }
{
}

CECFrameworkDriver::~CECFrameworkDriver()
{
	delete _pUtility;
	delete _pMemory;
	delete _pLog;
	delete _pSocket;
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

#include "ECFrameworkDriver.hpp"
#include "Global.hpp"
#include "Utility.hpp"
#include "Memory.hpp"
#include "Windows.hpp"
#include "Shared.hpp"

CECFrameworkDriver::CECFrameworkDriver()
	: _pUtility{ new CUtility }, _pMemory{ new CMemory }
{
}

CECFrameworkDriver::~CECFrameworkDriver()
{
}

CUtility* CECFrameworkDriver::Utility() const
{
	return _pUtility;
}

CMemory* CECFrameworkDriver::Memory() const
{
	return _pMemory;
}
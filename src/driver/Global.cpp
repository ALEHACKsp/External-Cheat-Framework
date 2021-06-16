#include "Global.hpp"

CGlobal* pGlobal;

CGlobal::CGlobal(CECFrameworkDriver* pECFrameworkDriver)
	: _pECFrameworkDriver{ pECFrameworkDriver }
{
}

CECFrameworkDriver* CGlobal::ECFrameworkDriver() const
{
	return _pECFrameworkDriver;
}

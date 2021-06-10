#include "ModuleEntry.h"

CModuleEntry::CModuleEntry()
	: _Module{ MODULEENTRY32{sizeof(_Module),0,0,0,0,0,0,0,0,0} }
{
}

CModuleEntry::~CModuleEntry()
{
}

MODULEENTRY32* CModuleEntry::Module()
{
	return &_Module;
}
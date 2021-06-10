#include "ProcessEntry.h"

CProcessEntry::CProcessEntry()
	: _Process{ PROCESSENTRY32{sizeof(_Process),0,0,0,0,0,0,0,0,0} }
{
}

CProcessEntry::~CProcessEntry()
{
}

PROCESSENTRY32* CProcessEntry::Process()
{
	return &_Process;
}
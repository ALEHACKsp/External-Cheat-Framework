#pragma once

class CCheat;

class Globals
{
	friend CCheat;

private:
	CCheat* _gpCheat;

public:
	CCheat* pCheat() { return _gpCheat; }

}; extern Globals* pGlobals;
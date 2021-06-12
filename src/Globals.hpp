#ifndef _Globals_Hpp_
#define _Globals_Hpp_

class CECF;

class Globals
{
	friend CECF;

private:
	CECF* _gpCheat;

public:
	CECF* pCheat() { return _gpCheat; }

}; extern Globals* pGlobals;

#endif // !_Globals_Hpp_

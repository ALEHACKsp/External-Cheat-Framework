#ifndef _Globals_Hpp_
#define _Globals_Hpp_

class CECFramework;

class Globals
{
	friend CECFramework;

private:
	CECFramework* _gpCheat;

public:
	CECFramework* pCheat() { return _gpCheat; }

}; extern Globals* pGlobals;

#endif // !_Globals_Hpp_

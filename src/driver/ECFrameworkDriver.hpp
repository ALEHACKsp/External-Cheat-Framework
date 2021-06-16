#ifndef _CKECF_Hpp_
#define _CKECF_Hpp_

class CUtility;
class CMemory;

class CECFrameworkDriver
{
	// Members
public:

private:
	CUtility* _pUtility;
	CMemory* _pMemory;

	// Methods
public:
	CECFrameworkDriver();
	~CECFrameworkDriver();

	CUtility* Utility() const;
	CMemory* Memory() const;

private:

};

#endif // !_CKECF_Hpp_
#ifndef _Global_Hpp_
#define _Global_Hpp_

class CECFrameworkDriver;

class CGlobal
{
	friend CECFrameworkDriver;

	// Members
public:

private:
	CECFrameworkDriver* _pECFrameworkDriver;

	// Methods
public:
	CGlobal(CECFrameworkDriver* pCKECF);

	CECFrameworkDriver* ECFrameworkDriver() const;

private:

}; extern CGlobal* pGlobal;

#endif // !_Global_Hpp_
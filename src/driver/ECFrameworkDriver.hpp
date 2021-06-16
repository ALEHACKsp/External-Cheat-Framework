#ifndef _CKECF_Hpp_
#define _CKECF_Hpp_

class CUtility;
class CMemory;
class CLog;
class CSocket;

class CECFrameworkDriver
{
	// Members
public:

private:
	CUtility* _pUtility;
	CMemory* _pMemory;
	CLog* _pLog;
	CSocket* _pSocket;
	bool _bShutdown;

	// Methods
public:
	CECFrameworkDriver();
	~CECFrameworkDriver();

	CUtility* Utility() const;
	CMemory* Memory() const;
	CLog* Log() const;
	CSocket* Socket() const;
	bool bShutdown() const;

private:

};

#endif // !_CKECF_Hpp_

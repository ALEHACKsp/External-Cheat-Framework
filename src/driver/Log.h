#ifndef _Log_Hpp_
#define _Log_Hpp_

#include <ntifs.h>
#include <utility>

class CLog
{
	// Members
public:

private:

	// Methods
public:
	CLog();
	~CLog();

	template<typename... Args>
	void Print(const char* String, Args... arguments) const
	{
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, String, arguments...);
	}

private:

};

#endif // _Log_H_

#ifndef _Cheat_H_
#define _Cheat_H_

#include <Windows.h>
#include <map>
#include <vector>

class CString;
class CLog;
class CProcess;
class COffsets;
class CConsole;

class CCheat
{
	// Members
public:

private:
	CLog* _pLog;
	CProcess* _pProcess;
	COffsets* _pOffsets;
	CConsole* _pConsole;
	bool _bRunning;
	DWORD _dwBaseModule;
	DWORD _dwBaseSize;
	int DeltaTimeStart;
	int DeltaTimeEnd;

	// Methods
public:
	CCheat();
	~CCheat();

	CLog* Log() const;
	CProcess* Process() const;
	COffsets* Offsets() const;
	CConsole* Console() const;
	bool Running() const;
	int DeltaTime() const;
	void StartDeltaTime();
	void EndDeltaTime();
	DWORD BaseModule() const;
	DWORD BaseSize() const;
	bool GetGameModule(CString);
	void GetGameData();
	void Draw();
	
private:

};

#endif // !_Cheat_H_
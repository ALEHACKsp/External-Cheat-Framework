#ifndef _Cheat_H_
#define _Cheat_H_

#include <Windows.h>
#include <map>
#include <vector>

class CLog;
class CProcess;
class CCustomString;
class COffsets;
struct Team;

class CCheat
{
	// Members
public:

private:
	CLog* _pLog;
	CProcess* _pProcess;
	COffsets* _pOffsets;
	bool _bRunning;
	DWORD _dwBaseModule;
	DWORD _dwBaseSize;

	// Methods
public:
	CCheat();
	~CCheat();

	CLog* Log() const;
	CProcess* Process() const;
	COffsets* Offsets() const;
	bool Running() const;
	DWORD BaseModule() const;
	DWORD BaseSize() const;
	bool GetGameModule(CCustomString);
	void GetGameData();
	void Draw();
	
private:

};

#endif // _Cheat_H_
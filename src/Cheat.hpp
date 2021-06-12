#ifndef _Cheat_Hpp_
#define _Cheat_Hpp_

#include <Windows.h>
#include <map>
#include <vector>

class CString;
class CLog;
class CProcess;
class COffsets;
class CConsole;

/// <summary> ECF Parent Class </summary>
class CECF
{
	// Members
public:

private:
	/// <summary> Pointer to an Information Logger object, stored on the heap. </summary>
	CLog* _pLog;

	/// <summary> Pointer to a Process Utility object, stored on the heap. </summary>
	CProcess* _pProcess;

	/// <summary> Pointer to a Offset Manager object, stored on the heap.  </summary>
	COffsets* _pOffsets;

	/// <summary> Pointer to  </summary>
	CConsole* _pConsole;

	/// <summary> .... </summary>
	bool _bRunning;

	/// <summary> .... </summary>
	DWORD _dwBaseModule;

	/// <summary> .... </summary>
	DWORD _dwBaseSize;

	/// <summary> .... </summary>
	int DeltaTimeStart;

	/// <summary> .... </summary>
	int DeltaTimeEnd;

	/// <summary> .... </summary>

	// Methods
public:
	CECF();
	~CECF();

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

#endif // !_Cheat_Hpp_
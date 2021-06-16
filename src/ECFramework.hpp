#ifndef _CECF_Hpp_
#define _CECF_Hpp_

#include <Windows.h>
#include <map>
#include <vector>

class CString;
class CLog;
class CProcess;
class COffsets;
class CConsole;

/// <summary> The ECF Parent Class. </summary>
class CECFramework
{
	// Members
public:

private:
	/// <summary> Pointer to the Information Logger object, stored on the heap. </summary>
	CLog* _pLog;

	/// <summary> Pointer to the Process Utility object, stored on the heap. </summary>
	CProcess* _pProcess;

	/// <summary> Pointer to the Offset Manager object, stored on the heap.  </summary>
	COffsets* _pOffsets;

	/// <summary> Pointer to the Debugger Console object, stored on the heap. </summary>
	CConsole* _pConsole;

	/// <summary> Boolean Switch for whether the framework should continue running. Needs to be implemented in your own way. </summary>
	bool _bRunning;

	// TO DO; add support for multiple game modules.

	/// <summary> DWORD for storing the base address of your game's main module, this can be your game's EXE, or in the case of a game like CS:GO, one of it's game modules (e.g. client.dll). </summary>
	DWORD _dwBaseModule;

	/// <summary> DWORD for storing the size of your game's main module. </summary>
	DWORD _dwBaseSize;

	/// <summary> Initial Timestamp used to calculate Delta Time </summary>
	int _iDeltaTimeStart;

	/// <summary> Latter Timestamp used to calculate Delta Time </summary>
	int _iDeltaTimeEnd;

	// Methods
public:
	/// <summary> The no-args CECFramework constructor. </summary>
	CECFramework();

	/// <summary> The CECFramework destructor. </summary>
	~CECFramework();

	/// <returns> A Pointer to the Information Logger object. </returns>
	CLog* Log() const;

	/// <returns> A Pointer to the Process Utility object. </returns>
	CProcess* Process() const;

	/// <returns> A Pointer to the Offset Manager object. </returns>
	COffsets* Offsets() const;

	/// <returns> A Pointer to the Debugger Console object. </returns>
	CConsole* Console() const;

	/// <returns> Whether the framework should be running. </returns>
	bool Running() const;

	/// <returns> The current Delta Time, should be called after EndDeltaTime(). </returns>
	int DeltaTime() const;

	/// <summary> Sets the initial Delta Time Timestamp, should be called at the start of your function loop. </summary>
	void StartDeltaTime();

	/// <summary> Sets the latter Delta Time Timestamp, should be called at the end of your function loop, BEFORE DeltaTime(). </summary>
	void EndDeltaTime();

	/// <returns> The Address of your game's assigned main module. </returns>
	DWORD BaseModule() const;

	/// <returns> The size of your game's assigned main module. </returns>
	DWORD BaseSize() const;

	/// <summary> Uses Process->GetModuleInformation to set this objects' Game Module Information (BaseAddress & ModuleSize). </summary>
	/// <param name="ModuleName"> The FILE name of your game's EXE. </param>
	/// <returns> True or false based on whether the specified module was successfully found. </returns>
	bool GetGameModule(CString ModuleName);

	/// <summary> Contains code relating to read/write calls to the game process. </summary>
	void GetSetGameData();

	/// <summary> Contains code relating to drawing any UI or ESP elements. </summary>
	void Draw();
	
private:

};

#endif // !_CECF_Hpp_
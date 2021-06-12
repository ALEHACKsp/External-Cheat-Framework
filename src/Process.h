#ifndef _Process_H_
#define _Process_H_

#include <Windows.h>
#include <TlHelp32.h>

class CString;
class CCheat;
class CModuleEntry;
class CDriver;

class CProcess
{
	// Class Members
public:

private:	
	CCheat* _pParent;
	HANDLE	_hTargetProcess;
	DWORD	_dwTargetProcessID;
	CString* _ExeName;

#ifdef KERNEL_MODE_RW
	CDriver* _pDriver;
#endif // KERNEL_MODE_RW


	// Class Methods
public:
	CProcess(CCheat*);												// Constructor, Requires Pointer to Parent Object
	~CProcess();													// Destructor

	bool Attach(CString);											// Attach 
	CModuleEntry GetModuleInformation(CString) const;				// Get Base Address of Module of Given Name
	DWORD64 FindSignature(DWORD64, DWORD64, BYTE*, BYTE*) const;	// Find Memory Signature within Specified Region
	bool DataCompare(BYTE*, BYTE*, BYTE*) const;					// Utility Function for FindSignature()
	HANDLE hTargetProcess() const;									// Get handle to target process
	DWORD dwTargetProcessID() const;								// Get PID of target process
	CString ExeName() const;

#ifdef KERNEL_MODE_RW

	// TO DO; add kernel read/writes

#else

	template <class T> T Read(DWORD64 dwAddress) // Reads a Memory Value at the Given DWORD64 Address
	{ T Result;	ReadProcessMemory(_hTargetProcess, static_cast<LPVOID>(dwAddress), &Result, sizeof(T), NULL); return Result; }

	template <class T> void Write(DWORD64 dwAddress, T Value) // Writes a Memory Value at the Given DWORD64 Address
	{ WriteProcessMemory(_hTargetProcess, static_cast<LPVOID>(dwAddress), &Value, sizeof(T), NULL); }

	template <class T> T Read(DWORD dwAddress) // Reads a Memory Value at the Given DWORD Address
	{ T Result;	ReadProcessMemory(_hTargetProcess, static_cast<LPVOID>(dwAddress), &Result, sizeof(T), NULL); return Result; }

	template <class T> void Write(DWORD dwAddress, T Value) // Writes a Memory Value at the Given DWORD Address
	{ WriteProcessMemory(_hTargetProcess, static_cast<LPVOID>(dwAddress), &Value, sizeof(T), NULL); }

	template <class T> T Read(WORD dwAddress) // Reads a Memory Value at the Given WORD Address
	{ T Result;	ReadProcessMemory(_hTargetProcess, static_cast<LPVOID>(dwAddress), &Result, sizeof(T), NULL); return Result; }

	template <class T> void Write(WORD dwAddress, T Value) // Writes a Memory Value at the Given WORD Address
	{ WriteProcessMemory(_hTargetProcess, static_cast<LPVOID>(dwAddress), &Value, sizeof(T), NULL); }

#endif // KERNEL_MODE_RW

private:

};

#endif // !_Process_H_
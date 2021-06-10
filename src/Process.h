#ifndef _Process_H_
#define _Process_H_

#include <Windows.h>
#include <TlHelp32.h>

class CCheat;
class CCustomString;
class CModuleEntry;

class CProcess
{
	// Class Members
public:

private:	
	CCheat* _pParent;
	HANDLE	_hTargetProcess;
	DWORD	_dwTargetProcessID;
	CCustomString* _ExeName;

	// Class Methods
public:
	CProcess(CCheat*);												// Constructor, Requires Pointer to Parent Object
	~CProcess();													// Destructor

	bool Attach(CCustomString);										// Attach 
	CModuleEntry GetModuleInformation(CCustomString) const;			// Get Base Address of Module of Given Name
	DWORD64 FindSignature(DWORD64, DWORD64, BYTE*, BYTE*) const;	// Find Memory Signature within Specified Region
	bool DataCompare(BYTE*, BYTE*, BYTE*) const;					// Utility Function for FindSignature()
	template<class T> T Read(DWORD64 dwAddress);					// Reads a Memory Value at the Given Address
	template<class T> void Write(DWORD64 dwAddress, T Value);		// Writes a Memory Value at the Given Address
	HANDLE hTargetProcess() const;									// Get handle to target process
	DWORD dwTargetProcessID() const;								// Get PID of target process
	CCustomString ExeName() const;

private:

};

#endif // _Process_H_
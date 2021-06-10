#include "Cheat.h"
#include "CustomString.h"
#include "Process.h"
#include "Log.h"


int main()
{
	CCheat* Cheat = new CCheat;

	{
		if (!Cheat->Process()->Attach("ac_client.exe"))
			return 0;

		if (!Cheat->GetGameModule("ac_client.exe"))
			return 0;

		Cheat->Log()->Print("Name: %s", Cheat->Process()->ExeName().String());
		Cheat->Log()->Print("Handle: %d", Cheat->Process()->hTargetProcess());
		Cheat->Log()->Print("Base Module Address: 0x%X", Cheat->BaseModule());
		Cheat->Log()->Print("Base Module Size: %d Bytes", Cheat->BaseSize());

		WORD BaseAddress = 0x400000;
		WORD OffsetLocalPlayer = 0x10F4F4;
		WORD m_XPos = 0x0038;
		WORD m_YPos = 0x003C;
		WORD m_ZPos = 0x0040;
		WORD m_isPosMoving = 0x0070;
		WORD m_Speed = 0x0080;
		WORD m_Health = 0x00F8;
		WORD m_Vest = 0x00FC;
		WORD m_AmmoMags = 0x0128;
		WORD m_Ammo = 0x0150;
		WORD m_SecAmmo = 0x013C;
		WORD m_Flashbangs = 0x0158;
		WORD m_WeaponTimer = 0x0178;
		WORD m_ShotsFired = 0x01A0;

		DWORD LocalPlayerPtr = Cheat->Process()->Read<DWORD>(BaseAddress + OffsetLocalPlayer);
		Cheat->Log()->Print("LocalPlayerPtr: 0x%X", LocalPlayerPtr);
	
		while (Cheat->Running())
		{
			

			Sleep(5);
		}
	}

	delete Cheat;

	return 1;
}
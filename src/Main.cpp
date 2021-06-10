#include "Cheat.h"
#include "CustomString.h"
#include "Process.h"
#include "Log.h"
#include "Console.h"

#define REFRESH_DELAY 17 // ~60hz Frametime

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

		DWORD BaseAddress = 0x400000;
		DWORD LocalPlayer = 0x10F4F4;
		WORD m_XPos = 0x0038;
		WORD m_YPos = 0x003C;
		WORD m_ZPos = 0x0040;
		WORD m_isPosMoving = 0x0070;
		WORD m_Health = 0x00F8;
		WORD m_Vest = 0x00FC;
		WORD m_AmmoMags = 0x0128;
		WORD m_Ammo = 0x0150;

		DWORD LocalPlayerPtr = Cheat->Process()->Read<DWORD>(BaseAddress + LocalPlayer);
		Cheat->Log()->Print("LocalPlayerPtr: 0x%X", LocalPlayerPtr);

		while (Cheat->Running())
		{
			{
				float Position[3] = 
				{ 
					Cheat->Process()->Read<float>(LocalPlayerPtr + m_XPos),
					Cheat->Process()->Read<float>(LocalPlayerPtr + m_YPos),
					Cheat->Process()->Read<float>(LocalPlayerPtr + m_ZPos)
				};

				bool bIsMoving =
				{
					Cheat->Process()->Read<bool>(LocalPlayerPtr + m_isPosMoving)
				};

				int Health =
				{
					Cheat->Process()->Read<int>(LocalPlayerPtr + m_Health)
				};

				int Armour =
				{
					Cheat->Process()->Read<int>(LocalPlayerPtr + m_Vest)
				};

				int AmmoMags =
				{
					Cheat->Process()->Read<int>(LocalPlayerPtr + m_AmmoMags)
				};

				int Ammo =
				{
					Cheat->Process()->Read<int>(LocalPlayerPtr + m_Ammo)
				};

				Cheat->Console()->StartDraw();

				// Dynamically Print using Log, example:
				Cheat->Log()->PrintPlain("LocalPlayer Position: %.3f, %.3f, %.3f\n", Position[0], Position[1], Position[2]);
				Cheat->Log()->PrintPlain("LocalPlayer IsMoving: %s\n", bIsMoving ? "True" : "False");
				Cheat->Log()->PrintPlain("LocalPlayer Health: %d\n", Health);
				Cheat->Log()->PrintPlain("LocalPlayer Armour: %d\n", Armour);
				Cheat->Log()->PrintPlain("CurrentWeapon Ammo Magazines: %d\n", AmmoMags);
				Cheat->Log()->PrintPlain("CurrentWeapon Ammo: %d\n", Ammo);

				Cheat->Console()->EndDraw();
			}

			Sleep(REFRESH_DELAY);
		}
	}

	delete Cheat;

	return 1;
}
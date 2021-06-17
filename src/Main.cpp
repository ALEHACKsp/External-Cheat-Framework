#include "ECFramework.hpp"
#include "String.hpp"
#include "Process.hpp"
#include "Log.hpp"
#include "Console.hpp"
#include "Offsets.hpp"

// Global Switches, defined here for convenience...

#define REFRESH_DELAY float(17.7)																				// ~60hz Frametime.

int main()
{
	CECFramework* Cheat = new CECFramework;																		// Allocate new storage on the heap for a CECFramework object, and then store a pointer that points to that storage here.

	{
		if (!Cheat->Process()->Attach("ac_client.exe"))
			return 0;

		if (!Cheat->GetGameModule("ac_client.exe"))
			return 0;

		// START: ASSAULT CUBE EXAMPLE
		Cheat->Log()->Print("Name: %s", Cheat->Process()->ExeName().String());
		Cheat->Log()->Print("Handle: %d", Cheat->Process()->hTargetProcess());
		Cheat->Log()->Print("Base Module Address: 0x%X", Cheat->BaseModule());
		Cheat->Log()->Print("Base Module Size: %d Bytes", Cheat->BaseSize());

		DWORD LocalPlayerPtr = Cheat->Process()->Read<DWORD>(NOffsets::BaseAddress + NOffsets::LocalPlayer);
		Cheat->Log()->Print("LocalPlayerPtr: 0x%X", LocalPlayerPtr);
		// END: ASSAULT CUBE EXAMPLE

		while (Cheat->Running())
		{
			{
				Cheat->StartDeltaTime();

				// START: ASSAULT CUBE EXAMPLE
				float Position[3] = { 0 };

				for (int i = 0; i < 3; i++)																		// Here we perform 3 read calls, 4 bytes apart, to collect x,y,z info to form a set of 3D coordinates.
					Position[i] = Cheat->Process()->Read<float>(LocalPlayerPtr + NOffsets::m_XPos + (static_cast<int>(i) * 0x4));

				bool bIsMoving = { Cheat->Process()->Read<bool>(LocalPlayerPtr + NOffsets::m_isPosMoving) };	// Here we read a single byte of data at the address resulted by the equation
				int Health = { Cheat->Process()->Read<int>(LocalPlayerPtr + NOffsets::m_Health) };				// LocalPlayerPtr + m_isPosMoving.
				int Armour = { Cheat->Process()->Read<int>(LocalPlayerPtr + NOffsets::m_Vest) };
				int AmmoMags = { Cheat->Process()->Read<int>(LocalPlayerPtr + NOffsets::m_AmmoMags) };
				int Ammo = { Cheat->Process()->Read<int>(LocalPlayerPtr + NOffsets::m_Ammo) };
				// END: ASSAULT CUBE EXAMPLE


				Cheat->Console()->StartDraw();																	// Clear debug buffer for new printing


				// START: ASSAULT CUBE EXAMPLE
				Cheat->Log()->PrintPlain("LocalPlayer Position: %.3f, %.3f, %.3f\n", Position[0], Position[1], Position[2]);
				Cheat->Log()->PrintPlain("LocalPlayer IsMoving: %s\n", bIsMoving ? "False" : "True");
				Cheat->Log()->PrintPlain("LocalPlayer Health: %d\n", Health);
				Cheat->Log()->PrintPlain("LocalPlayer Armour: %d\n", Armour);
				Cheat->Log()->PrintPlain("CurrentWeapon Ammo Magazines: %d\n", AmmoMags);
				Cheat->Log()->PrintPlain("CurrentWeapon Ammo: %d\n", Ammo);
				// END: ASSAULT CUBE EXAMPLE


				Cheat->Console()->EndDraw();																	// Finish debug draw

				Cheat->EndDeltaTime();
			}

			Sleep((static_cast<int>(REFRESH_DELAY - Cheat->DeltaTime())											// Cap the refresh rate of the console.. We minus the time it took for memory reading/writing and console
				& 0x80000000) ? 0 : REFRESH_DELAY - Cheat->DeltaTime());										// printing to keep it consistent. We check if the high bit is set to see if it is a negative value.

		}
	}

	delete Cheat;																								// Free the previously allocated storage for the CECFramework object(line 11), and set the pointer to nullptr.

	return 1;
}
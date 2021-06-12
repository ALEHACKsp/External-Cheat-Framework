#include "Cheat.hpp"
#include "String.hpp"
#include "Process.hpp"
#include "Log.hpp"
#include "Console.hpp"

// Global Switches, defined here for convenience... 
// You can define KERNEL_MODE_RW in the preprocessor settings in the properties windows to toggle Kernel Driver based read/writes

	#define REFRESH_DELAY 17.7	// ~60hz Frametime.


int main()
{
	CECF* Cheat = new CECF(); // Allocate new storage on the heap for a CECF object, and then store a pointer that points to that storage here.

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
		// END: ASSAULT CUBE EXAMPLE

		while (Cheat->Running())
		{
			{
				Cheat->StartDeltaTime();

				// START: ASSAULT CUBE EXAMPLE
				float Position[3] = { 0 };

				for (size_t i = 0; i < 3; i++)																		// Here we perform 3 read calls that each read 4 bytes of data at the address
					Position[i] = Cheat->Process()->Read<float>(LocalPlayerPtr + m_XPos + (i * sizeof(float)));		// resulted by the equation LocalPlayerPtr + m_XPos + (i * sizeof(float)).

				bool bIsMoving = { Cheat->Process()->Read<bool>(LocalPlayerPtr + m_isPosMoving) };					// Here we read a single byte of data at the address resulted by the equation
				int Health = { Cheat->Process()->Read<int>(LocalPlayerPtr + m_Health) };							// LocalPlayerPtr + m_isPosMoving.
				int Armour = { Cheat->Process()->Read<int>(LocalPlayerPtr + m_Vest) };
				int AmmoMags = { Cheat->Process()->Read<int>(LocalPlayerPtr + m_AmmoMags) };
				int Ammo = { Cheat->Process()->Read<int>(LocalPlayerPtr + m_Ammo) };
				// END: ASSAULT CUBE EXAMPLE


				Cheat->Console()->StartDraw(); // Clear debug buffer for new printing


				// START: ASSAULT CUBE EXAMPLE
				Cheat->Log()->PrintPlain("LocalPlayer Position: %.3f, %.3f, %.3f\n", Position[0], Position[1], Position[2]);
				Cheat->Log()->PrintPlain("LocalPlayer IsMoving: %s\n", bIsMoving ? "False" : "True");
				Cheat->Log()->PrintPlain("LocalPlayer Health: %d\n", Health);
				Cheat->Log()->PrintPlain("LocalPlayer Armour: %d\n", Armour);
				Cheat->Log()->PrintPlain("CurrentWeapon Ammo Magazines: %d\n", AmmoMags);
				Cheat->Log()->PrintPlain("CurrentWeapon Ammo: %d\n", Ammo);
				// END: ASSAULT CUBE EXAMPLE


				Cheat->Console()->EndDraw(); // Finish debug draw

				Cheat->EndDeltaTime();
			}

			Sleep((static_cast<int>(REFRESH_DELAY - Cheat->DeltaTime())  // Cap the refresh rate of the console.. We minus the time it took for memory reading/writing and console
				& 0x80000000) ? 0 : REFRESH_DELAY - Cheat->DeltaTime()); // printing to keep it consistent. We check if the high bit is set to see if it is a negative value.

		}
	}

	delete Cheat; // Free the previously allocated storage for the CECF object(line 11), and set the pointer to nullptr.

	return 1;
}
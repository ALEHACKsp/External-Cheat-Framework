#include "Cheat.h"
#include "CustomString.h"
#include "Log.h"
#include "Process.h"
#include "Globals.h"

int main()
{
	CCheat* Cheat = new CCheat;

	{
		if (!Cheat->Process()->Attach("notepad.exe"))
			return 0;

		if (!Cheat->GetGameModule("notepad.exe"))
			return 0;

		Cheat->Log()->Print("Name: %s", Cheat->Process()->ExeName().String());
		Cheat->Log()->Print("Handle: %d", Cheat->Process()->hTargetProcess());
		Cheat->Log()->Print("Base Module Address: 0x%X", Cheat->BaseModule());
		Cheat->Log()->Print("Base Module Size: %d Bytes", Cheat->BaseSize());
	
		while (Cheat->Running())
		{
			//Cheat->GetGameData();
			//Cheat->Draw();

			

			Sleep(5);
		}
	}

	delete pGlobals;
	delete Cheat;

	return 1;
}
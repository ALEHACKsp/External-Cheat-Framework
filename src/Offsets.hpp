#ifndef _Offsets_Hpp_
#define _Offsets_Hpp_

#include <Windows.h>

namespace NOffsets
{
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
}

#endif // _Offsets_H_

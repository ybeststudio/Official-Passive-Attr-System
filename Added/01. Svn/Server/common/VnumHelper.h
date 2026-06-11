#ifndef __INC_COMMON_VNUM_HELPER_H__
#define __INC_COMMON_VNUM_HELPER_H__

#include "service.h"
#include "length.h"

class CItemVnumHelper
{
public:
#if defined(__PET_SYSTEM__)
	static const bool IsPhoenix(DWORD vnum) { return 53001 == vnum; }
#endif
	static const bool IsRamadanMoonRing(DWORD vnum) { return 71135 == vnum; }
	static const bool IsHalloweenCandy(DWORD vnum) { return 71136 == vnum; }
	static const bool IsHappinessRing(DWORD vnum) { return 71143 == vnum; }
	static const bool IsLovePendant(DWORD vnum) { return 71145 == vnum; }

	/// Magic Ring
	static const bool IsMagicRing(DWORD vnum) { return 71148 == vnum || 71149 == vnum; }

	/// Easter Candy
	static const bool IsEasterCandy(DWORD vnum) { return 71188 == vnum; }

	/// Chocolate Pendant
	static const bool IsChocolatePendant(DWORD vnum) { return 71199 == vnum; }

	/// Nazar Pendant
	static const bool IsNazarPendant(DWORD vnum) { return 71202 == vnum; }

	/// Guardian Pendant
	static const bool IsGuardianPendant(DWORD vnum) { return 72054 == vnum; }

	/// Gem Pendant
	static const bool IsGemCandy(DWORD vnum) { return 76030 == vnum || 76047 == vnum; }

#if defined(__PASSIVE_ATTR__)
	static	const bool	IsPassive(DWORD vnum) { return 30272 <= vnum && vnum <= 30276; }
#endif

	// Unique Items
	static const bool IsUniqueItem(DWORD vnum)
	{
		switch (vnum)
		{
			case 71135: // IsRamadanMoonRing
			case 71136: // IsHalloweenCandy
			case 71143: // IsHappinessRing
			case 71145: // IsLovePendant

			case 71148: // IsMagicRing
			case 71149: // IsMagicRing

			case 71188: // IsEasterCandy
			case 71199: // IsChocolatePendant
			case 71202: // IsNazarPendant
			case 72054: // IsGuardianPendant

			case 76030: // IsGemCandy
			case 76047: // IsGemCandy
				return true;
		}
		return false;
	}

	static const bool IsDragonSoul(DWORD vnum)
	{
		return (vnum >= 110000 && vnum <= 165400);
	}
};

class CMobVnumHelper
{
public:
#if defined(__PET_SYSTEM__)
	static const bool IsPhoenix(DWORD vnum) { return 34001 == vnum; }
	static const bool IsIcePhoenix(DWORD vnum) { return 34003 == vnum; }
	static const bool IsReindeerYoung(DWORD vnum) { return 34002 == vnum; }

	static const bool IsPetUsingPetSystem(DWORD vnum) { return (IsPhoenix(vnum) || IsReindeerYoung(vnum)) || IsIcePhoenix(vnum); }
#endif

	static const bool IsRamadanBlackHorse(DWORD vnum) { return 20119 == vnum || 20219 == vnum || 22022 == vnum; }
};

class CVnumHelper
{
};

#endif // __INC_COMMON_VNUM_HELPER_H__

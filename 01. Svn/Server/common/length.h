// Find this line:
ON_IDLE_MAX_NUM

// Add after it:
#if defined(__PASSIVE_ATTR__)
// Official playerm2g2 window layout (26.0.6 client ABI). See playerm2g2.script.py.
enum EWindows
{
	RESERVED_WINDOW = 0,
	INVENTORY = 1,
	WEAR_EQUIPMENT_1 = 2,
	WEAR_EQUIPMENT_2 = 3,
	WEAR_COSTUME = 4,
	WEAR_DRAGONSOUL = 5,
	WEAR_PASSIVE_ATTR = 6,
	WEAR_UNIQUE = 7,
	SAFEBOX = 8,
	MALL = 9,
	DRAGON_SOUL_INVENTORY = 10,
	BELT_INVENTORY = 11,
	BELT_INVENTORY_2 = 12,
	GUILDBANK = 13,
	MAIL = 14,
	NPC_STORAGE = 15,
	PREMIUM_PRIVATE_SHOP = 16,
	ACCEREFINE = 17,
	GROUND = 18,
	PET_FEED = 19,
	CHANGELOOK = 20,
	AURA_REFINE = 21,
	CUBE_WINDOW = 22,

	WINDOW_TYPE_MAX = 23,
};

#define EQUIPMENT WEAR_EQUIPMENT_1
#else
// Legacy fork window layout (YEDEKLER/common/length.h).
enum EWindows
{
	RESERVED_WINDOW,
	INVENTORY,
	EQUIPMENT,
	SAFEBOX,
	MALL,
	DRAGON_SOUL_INVENTORY,
	BELT_INVENTORY,
	GUILDBANK, // __GUILDRENEWAL_SYSTEM__
	MAIL, // __MAILBOX__
	NPC_STORAGE, // __ATTR_6TH_7TH__
	PREMIUM_PRIVATE_SHOP, // __PREMIUM_PRIVATE_SHOP__
	ACCEREFINE, // __ACCE_COSTUME_SYSTEM__
	GROUND,
#if defined(__GROWTH_PET_SYSTEM__)
	PET_FEED, // __GROWTH_PET_SYSTEM__
#endif
	CHANGELOOK, // __CHANGE_LOOK_SYSTEM__
	AURA_REFINE, // __AURA_COSTUME_SYSTEM__
	CUBE_WINDOW, // __CUBE_RENEWAL__

	WINDOW_TYPE_MAX,
};
#endif

// Find this line:
SE_POINT_AREA_ELECT_ATTACK,

// Add after it:
#if defined(__PASSIVE_ATTR__)
	SE_PASSIVE_EFFECT,
#endif

// Find this line:
return cell < EQUIPMENT_MAX_NUM;

// Add after it:
#if defined(__PASSIVE_ATTR__)
			case WEAR_PASSIVE_ATTR:
				return cell < WEAR_PASSIVE_ATTR_SLOT_MAX;
#endif

// Find this line:
|| IsDragonSoulEquipPosition()

// Add after it:
#if defined(__PASSIVE_ATTR__)
			|| (window_type == WEAR_PASSIVE_ATTR && cell < WEAR_PASSIVE_ATTR_SLOT_MAX)
#endif

// Find this line:
} TItemPos;

// Add after it:
#if defined(__PASSIVE_ATTR__)
// Official wire/DB (WEAR_PASSIVE_ATTR) <-> server runtime (EQUIPMENT wear cells).
inline void NormalizePlayerItemPos(TItemPos& rPos)
{
	if (rPos.window_type != WEAR_PASSIVE_ATTR)
		return;

	if (rPos.cell == WEAR_PASSIVE_ATTR_UP)
	{
		rPos.window_type = EQUIPMENT;
		rPos.cell = WEAR_PASSIVE;
	}
}

inline void EncodePlayerItemWirePos(BYTE& bWindow, WORD& wCell)
{
	if (bWindow == EQUIPMENT && wCell == WEAR_PASSIVE)
	{
		bWindow = WEAR_PASSIVE_ATTR;
		wCell = WEAR_PASSIVE_ATTR_UP;
	}
}

inline void EncodePlayerItemWirePos(TItemPos& rPos)
{
	EncodePlayerItemWirePos(rPos.window_type, rPos.cell);
}
#endif

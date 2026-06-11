// Find this line:
const DWORD c_Equipment_Slot_Glove = c_Equipment_Slot_Start + CItemData::WEAR_GLOVE;

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
const DWORD c_Equipment_Slot_Passive = c_Equipment_Slot_Start + CItemData::WEAR_PASSIVE;

enum EPassiveAttrWearCell
{
	WEAR_PASSIVE_ATTR_UP = 0,
	WEAR_PASSIVE_ATTR_DOWN = 1,
	WEAR_PASSIVE_ATTR_SLOT_MAX = 2,
};

enum EOfficialWearSlotType
{
	SLOT_TYPE_WEAR_EQUIPMENT_1 = 15,
	SLOT_TYPE_WEAR_EQUIPMENT_2 = 16,
	SLOT_TYPE_WEAR_COSTUME = 17,
	SLOT_TYPE_WEAR_DRAGONSOUL = 18,
	SLOT_TYPE_WEAR_PASSIVE_ATTR = 19,
	SLOT_TYPE_WEAR_UNIQUE = 20,
};
#endif

// Find this line:
SLOT_TYPE_MAX,

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
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
// Legacy fork window layout (matches server when __PASSIVE_ATTR__ is off).
enum EWindows
{
	RESERVED_WINDOW = 0,
	INVENTORY = 1,
	EQUIPMENT = 2,
	SAFEBOX = 3,
	MALL = 4,
	DRAGON_SOUL_INVENTORY = 5,
	BELT_INVENTORY = 6,
	GUILDBANK = 7,
	MAIL = 8,
	NPC_STORAGE = 9,
	PREMIUM_PRIVATE_SHOP = 10,
	ACCEREFINE = 11,
	GROUND = 12,
	PET_FEED = 13,
	CHANGELOOK = 14,
	AURA_REFINE = 15,
	CUBE_WINDOW = 16,

	WINDOW_TYPE_MAX = 17,
};
#endif

// Find this line:
return cell < ATTR67_ADD_SLOT_MAX;

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
			case WEAR_PASSIVE_ATTR:
				return cell < WEAR_PASSIVE_ATTR_SLOT_MAX;
#endif

// Find this line:
BYTE WindowTypeToSlotType(BYTE bWndType);

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
void NormalizePlayerItemPos(TItemPos& rPos);
bool IsPassiveAttrWearPos(const TItemPos& rPos);
#endif

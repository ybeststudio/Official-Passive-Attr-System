// Find this line:
ITEM_GACHA, // 37 Gacha

// Add after it:
#if defined(__PASSIVE_ATTR__)
	ITEM_PASSIVE, // 38 Passive
#endif

// Find this line:
MATERIAL_DS_CHANGE_ATTR,

// Add after it:
#if defined(__PASSIVE_ATTR__)
	MATERIAL_PASSIVE_WEAPON,
	MATERIAL_PASSIVE_ARMOR,
	MATERIAL_PASSIVE_ACCE,
	MATERIAL_PASSIVE_ELEMENT,
#endif

// Find this line:
SPECIAL_LUCKY_BOX_GACHA,

// Add after it:
#if defined(__PASSIVE_ATTR__)
enum EPassiveSubTypes
{
	PASSIVE_JOB,
};
#endif

// Find this line:
WEAR_COSTUME_AURA,

// Add after it:
#if defined(__PASSIVE_ATTR__)
	WEAR_PASSIVE, // WEAR_PASSIVE_ATTR_UP (single physical slot; deck 2 is virtual UI)
#endif

// Find this line:
WEAR_MAX_NUM = 32

// Add after it:
#if defined(__PASSIVE_ATTR__)
enum EPassiveAttrWearCell
{
	WEAR_PASSIVE_ATTR_UP = 0,
	WEAR_PASSIVE_ATTR_DOWN = 1,
	WEAR_PASSIVE_ATTR_SLOT_MAX = 2,
};
#endif

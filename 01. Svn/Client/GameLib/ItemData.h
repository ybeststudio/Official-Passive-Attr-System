// Find this line:
ITEM_TYPE_GACHA,

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
		ITEM_TYPE_PASSIVE,
#endif

// Find this line:
MATERIAL_DS_CHANGE_ATTR,

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
		MATERIAL_PASSIVE_WEAPON,
		MATERIAL_PASSIVE_ARMOR,
		MATERIAL_PASSIVE_ACCE,
		MATERIAL_PASSIVE_ELEMENT,
#endif

// Find this line:
SPECIAL_LUCKY_BOX_GACHA,

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
	enum EPassiveSubTypes
	{
		PASSIVE_JOB,
	};
#endif

// Find this line:
WEAR_COSTUME_AURA,

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
		WEAR_PASSIVE,
#endif

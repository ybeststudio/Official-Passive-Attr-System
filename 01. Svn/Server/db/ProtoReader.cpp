// Find this line:
"ITEM_GACHA",

// Add after it:
#if defined(__PASSIVE_ATTR__)
		"ITEM_PASSIVE",
#endif

// Find this line:
"MATERIAL_DS_CHANGE_ATTR",

// Add after it:
#if defined(__PASSIVE_ATTR__)
		"MATERIAL_PASSIVE_WEAPON",
		"MATERIAL_PASSIVE_ARMOR",
		"MATERIAL_PASSIVE_ACCE",
		"MATERIAL_PASSIVE_ELEMENT",
#endif

// Find this line:
"SPECIAL_LUCKY_BOX_GACHA",

// Add after it:
#if defined(__PASSIVE_ATTR__)
	static std::string arSub38[]
	{
		"PASSIVE_JOB",
	};
#endif

// Find this line:
arSub37,	// ITEM_GACHA

// Add after it:
#if defined(__PASSIVE_ATTR__)
		arSub38,	// ITEM_PASSIVE
#endif

// Find this line:
sizeof(arSub37) / sizeof(arSub37[0]),	// ITEM_GACHA

// Add after it:
#if defined(__PASSIVE_ATTR__)
		sizeof(arSub38) / sizeof(arSub38[0]),	// ITEM_PASSIVE
#endif

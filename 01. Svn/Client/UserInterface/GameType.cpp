// Find this line:
RESERVED_WINDOW, // SLOT_TYPE_GOLDEN_LAND_FRUIT,

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
bool IsPassiveAttrWearPos(const TItemPos& rPos)
{
	return rPos.window_type == WEAR_PASSIVE_ATTR && rPos.cell < WEAR_PASSIVE_ATTR_SLOT_MAX;
}

void NormalizePlayerItemPos(TItemPos& rPos)
{
	if (rPos.window_type != WEAR_PASSIVE_ATTR)
		return;

	if (rPos.cell == WEAR_PASSIVE_ATTR_UP)
	{
		rPos.window_type = EQUIPMENT;
		rPos.cell = static_cast<WORD>(c_Equipment_Slot_Passive);
	}
	// WEAR_PASSIVE_ATTR_DOWN: virtual deck tab only (Dragon Soul style), not a second wear cell.
}

#endif

// Find this line:
BYTE SlotTypeToInvenType(BYTE bSlotType)

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
	if (bSlotType == SLOT_TYPE_WEAR_PASSIVE_ATTR)
		return WEAR_PASSIVE_ATTR;
#endif

// Find this line:
return c_aSlotTypeToInvenType[bSlotType];

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
BYTE c_aWndTypeToSlotType[WINDOW_TYPE_MAX] =
{
	SLOT_TYPE_NONE,                  // RESERVED_WINDOW
	SLOT_TYPE_INVENTORY,             // INVENTORY
	SLOT_TYPE_EQUIPMENT,             // WEAR_EQUIPMENT_1
	SLOT_TYPE_EQUIPMENT,             // WEAR_EQUIPMENT_2
	SLOT_TYPE_EQUIPMENT,             // WEAR_COSTUME
	SLOT_TYPE_EQUIPMENT,             // WEAR_DRAGONSOUL
	SLOT_TYPE_WEAR_PASSIVE_ATTR,     // WEAR_PASSIVE_ATTR
	SLOT_TYPE_EQUIPMENT,             // WEAR_UNIQUE
	SLOT_TYPE_SAFEBOX,               // SAFEBOX
	SLOT_TYPE_MALL,                  // MALL
	SLOT_TYPE_DRAGON_SOUL_INVENTORY, // DRAGON_SOUL_INVENTORY
	SLOT_TYPE_BELT_INVENTORY,        // BELT_INVENTORY
	SLOT_TYPE_BELT_INVENTORY,        // BELT_INVENTORY_2
	SLOT_TYPE_GUILDBANK,             // GUILDBANK
	SLOT_TYPE_NONE,                  // MAIL
	SLOT_TYPE_NONE,                  // NPC_STORAGE
	SLOT_TYPE_PREMIUM_PRIVATE_SHOP,  // PREMIUM_PRIVATE_SHOP
	SLOT_TYPE_ACCE,                  // ACCEREFINE
	SLOT_TYPE_NONE,                  // GROUND
	SLOT_TYPE_PET_FEED_WINDOW,       // PET_FEED
	SLOT_TYPE_CHANGE_LOOK,           // CHANGELOOK
	SLOT_TYPE_AURA,                  // AURA_REFINE
	SLOT_TYPE_NONE,                  // CUBE_WINDOW
};
#else
BYTE c_aWndTypeToSlotType[WINDOW_TYPE_MAX] =
{
	SLOT_TYPE_NONE,                    // RESERVED_WINDOW
	SLOT_TYPE_INVENTORY,               // INVENTORY
	SLOT_TYPE_EQUIPMENT,               // EQUIPMENT
	SLOT_TYPE_SAFEBOX,                 // SAFEBOX
	SLOT_TYPE_MALL,                    // MALL
	SLOT_TYPE_DRAGON_SOUL_INVENTORY,   // DRAGON_SOUL_INVENTORY
	SLOT_TYPE_BELT_INVENTORY,          // BELT_INVENTORY
	SLOT_TYPE_GUILDBANK,               // GUILDBANK
	SLOT_TYPE_NONE,                    // MAIL
	SLOT_TYPE_NONE,                    // NPC_STORAGE
	SLOT_TYPE_PREMIUM_PRIVATE_SHOP,    // PREMIUM_PRIVATE_SHOP
	SLOT_TYPE_ACCE,                    // ACCEREFINE
	SLOT_TYPE_NONE,                    // GROUND
	SLOT_TYPE_PET_FEED_WINDOW,         // PET_FEED
	SLOT_TYPE_CHANGE_LOOK,             // CHANGELOOK
	SLOT_TYPE_AURA,                    // AURA_REFINE
	SLOT_TYPE_NONE,                    // CUBE_WINDOW
};
#endif

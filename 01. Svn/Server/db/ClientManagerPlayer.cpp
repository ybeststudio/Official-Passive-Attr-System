// Find this line:
extern int g_log;

// Add after it:
#if defined(__PASSIVE_ATTR__)
// Official EWindows player item load/delete filter (26.0.6 playerm2g2 ABI).
#	define PLAYER_ITEM_WINDOWS_SQL \
		"'INVENTORY', 'EQUIPMENT', 'WEAR_EQUIPMENT_2', 'WEAR_COSTUME', 'WEAR_DRAGONSOUL', " \
		"'WEAR_PASSIVE_ATTR', 'WEAR_UNIQUE', 'DRAGON_SOUL_INVENTORY', 'BELT_INVENTORY'"
#else
#	define PLAYER_ITEM_WINDOWS_SQL \
		"'INVENTORY', 'EQUIPMENT', 'DRAGON_SOUL_INVENTORY', 'BELT_INVENTORY'"
#endif

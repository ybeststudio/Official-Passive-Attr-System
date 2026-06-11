// Find this line:
NEW_AFFECT_BATTLE_ROYALE_MOVE_SPEED = 591,

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
		NEW_AFFECT_PASSIVE_JOB_DECK = 593,
#endif

// Find this line:
EFFECT_TEMP,

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
		EFFECT_PASSIVE,
#endif

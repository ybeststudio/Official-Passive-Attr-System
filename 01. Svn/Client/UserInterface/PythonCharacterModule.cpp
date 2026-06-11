// Find this line:
PyModule_AddIntConstant(poModule, "AFFECT_BATTLE_ROYALE_MOVE_SPEED", CInstanceBase::NEW_AFFECT_BATTLE_ROYALE_MOVE_SPEED);

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
	PyModule_AddIntConstant(poModule, "AFFECT_PASSIVE_JOB_DECK", CInstanceBase::NEW_AFFECT_PASSIVE_JOB_DECK);
#endif

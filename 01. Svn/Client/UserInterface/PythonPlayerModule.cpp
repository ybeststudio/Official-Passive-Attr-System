// Find this line:
PyModule_AddIntConstant(poModule, "BELT_INVENTORY_2", BELT_INVENTORY_2);

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
	PyModule_AddIntConstant(poModule, "WEAR_PASSIVE_ATTR", WEAR_PASSIVE_ATTR);
	PyModule_AddIntConstant(poModule, "WEAR_PASSIVE_ATTR_UP", WEAR_PASSIVE_ATTR_UP);
	PyModule_AddIntConstant(poModule, "WEAR_PASSIVE_ATTR_DOWN", WEAR_PASSIVE_ATTR_DOWN);
#endif

// Find this line:
PyModule_AddIntConstant(poModule, "SLOT_TYPE_GOLDEN_LAND_FRUIT", SLOT_TYPE_GOLDEN_LAND_FRUIT);

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_WEAR_PASSIVE_ATTR", SLOT_TYPE_WEAR_PASSIVE_ATTR);
#endif

// Find this line:
PyModule_AddIntConstant(poModule, "EQUIPMENT_GLOVE", c_Equipment_Slot_Glove);

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
	PyModule_AddIntConstant(poModule, "EQUIPMENT_PASSIVE", c_Equipment_Slot_Passive);
#endif

// Find this line:
PyModule_AddIntConstant(poModule, "ITEM_TYPE_GACHA", CItemData::ITEM_TYPE_GACHA);

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_PASSIVE", CItemData::ITEM_TYPE_PASSIVE);
#endif

// Find this line:
PyModule_AddIntConstant(poModule, "MATERIAL_DS_CHANGE_ATTR", CItemData::MATERIAL_DS_CHANGE_ATTR);

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
	PyModule_AddIntConstant(poModule, "MATERIAL_PASSIVE_WEAPON", CItemData::MATERIAL_PASSIVE_WEAPON);
	PyModule_AddIntConstant(poModule, "MATERIAL_PASSIVE_ARMOR", CItemData::MATERIAL_PASSIVE_ARMOR);
	PyModule_AddIntConstant(poModule, "MATERIAL_PASSIVE_ACCE", CItemData::MATERIAL_PASSIVE_ACCE);
	PyModule_AddIntConstant(poModule, "MATERIAL_PASSIVE_ELEMENT", CItemData::MATERIAL_PASSIVE_ELEMENT);
#endif

// Find this line:
PyModule_AddIntConstant(poModule, "SPECIAL_LUCKY_BOX_GACHA", CItemData::SPECIAL_LUCKY_BOX_GACHA);

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
	PyModule_AddIntConstant(poModule, "PASSIVE_JOB", CItemData::PASSIVE_JOB);
#endif

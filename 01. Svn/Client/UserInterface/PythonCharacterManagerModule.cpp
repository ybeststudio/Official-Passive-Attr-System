// Find this line:
PyModule_AddIntConstant(poModule, "EFFECT_REFINE_ELEMENT_WEAPON_MAX", CInstanceBase::EFFECT_REFINE_ELEMENT_WEAPON_MAX);

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
	PyModule_AddIntConstant(poModule, "EFFECT_PASSIVE",		CInstanceBase::EFFECT_PASSIVE);
#endif

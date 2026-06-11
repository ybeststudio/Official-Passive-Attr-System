// Find this line:
PyModule_AddIntConstant(poModule, "ENABLE_TITLE_SYSTEM", 0);

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
	PyModule_AddIntConstant(poModule, "ENABLE_PASSIVE_ATTR", 1);
	#if defined(ENABLE_PASSIVE_ATTR_TOOLTIP)
	PyModule_AddIntConstant(poModule, "ENABLE_PASSIVE_ATTR_TOOLTIP", 1);
	#else
	PyModule_AddIntConstant(poModule, "ENABLE_PASSIVE_ATTR_TOOLTIP", 0);
	#endif
#else
	PyModule_AddIntConstant(poModule, "ENABLE_PASSIVE_ATTR", 0);
#endif

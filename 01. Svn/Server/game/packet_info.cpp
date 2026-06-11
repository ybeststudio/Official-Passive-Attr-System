// Find this line:
Set(HEADER_CG_SCRIPT_SELECT_ITEM, sizeof(TPacketCGScriptSelectItem), "ScriptSelectItem", true);

// Add after it:
#if defined(__PASSIVE_ATTR__)
	Set(HEADER_CG_PASSIVE_ATTR, sizeof(TPacketCGPassiveAttr), "PassiveAttr", false);
#endif

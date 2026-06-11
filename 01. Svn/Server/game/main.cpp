// Add to includes:
#if defined(__PASSIVE_ATTR__)
#	include "passive_attr.h"
#endif

// Find this line:
CTreasureHuntManager TreasureHuntManager;

// Add after it:
#if defined(__PASSIVE_ATTR__)
	CPassiveAttrManager passive_attr_manager;
#endif

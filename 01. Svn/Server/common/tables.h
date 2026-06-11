// Find this line:
BYTE bMaxLevelBySet[ATTRIBUTE_SET_MAX_NUM];

// Add after it:
#if defined(__PASSIVE_ATTR__)
struct TPassiveAttrTable
{
	TPassiveAttrTable() :
		wApplyIndex(0),
		dwProb(0),
		dwCooldownSec(0)
	{
		szApply[0] = 0;
		memset(&lValues, 0, sizeof(lValues));
	}

	char szApply[APPLY_NAME_MAX_LEN + 1];
	POINT_TYPE wApplyIndex;
	DWORD dwProb;
	DWORD dwCooldownSec;
	POINT_VALUE lValues[ITEM_ATTRIBUTE_MAX_LEVEL];
};
#endif

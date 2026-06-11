// Find this line:
static const bool IsGemCandy(DWORD vnum) { return 76030 == vnum || 76047 == vnum; }

// Add after it:
#if defined(__PASSIVE_ATTR__)
	static	const bool	IsPassive(DWORD vnum) { return 30272 <= vnum && vnum <= 30276; }
#endif

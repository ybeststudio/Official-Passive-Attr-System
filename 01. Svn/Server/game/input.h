// Find this line:
void MountUpGrade(LPCHARACTER ch, const char* c_pData);

// Add after it:
#if defined(__PASSIVE_ATTR__)
	void PassiveAttr(LPCHARACTER ch, const char* c_pData);
#endif

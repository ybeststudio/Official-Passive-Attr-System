// Find this line:
bool RecvMountUpGradeChat();

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
public:
	bool SendPassiveAttrPacket(BYTE bSubHeader, BYTE bPage, const WORD* awMaterialPos, WORD wJobItemPos);
	bool RecvPassiveAttr();
#endif

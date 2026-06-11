// Add the following packet definitions in the related packet section:
#if defined(__PASSIVE_ATTR__)
	HEADER_CG_PASSIVE_ATTR = 184,
#endif

// Add the following packet definitions in the related packet section:
#if defined(__PASSIVE_ATTR__)
	HEADER_GC_PASSIVE_ATTR = 188,
#endif

// Add the following packet definitions in the related packet section:
#if defined(__PASSIVE_ATTR__)
typedef struct SPacketCGPassiveAttr
{
	BYTE bHeader;
	BYTE bSubHeader;
	BYTE bPage;
	WORD wMaterialPos[4];
	WORD wJobItemPos;
} TPacketCGPassiveAttr;

typedef struct SPacketGCPassiveAttr
{
	BYTE bHeader;
	BYTE bSubHeader;
	BYTE bPage;
	BYTE bResult;
	BYTE bActive;
	DWORD dwRemainSec;
	BYTE bHasRelic;
} TPacketGCPassiveAttr;
#endif

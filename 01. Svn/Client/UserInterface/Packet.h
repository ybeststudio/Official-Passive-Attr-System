// Add the following packet definitions in the related packet section:
#if defined(ENABLE_PASSIVE_ATTR)
	HEADER_CG_PASSIVE_ATTR = 184,
#endif

// Add the following packet definitions in the related packet section:
#if defined(ENABLE_PASSIVE_ATTR)
	HEADER_GC_PASSIVE_ATTR = 188,
#endif

// Add the following packet definitions in the related packet section:
#if defined(ENABLE_PASSIVE_ATTR)
	SE_PASSIVE_EFFECT,
#endif

// Add the following packet definitions in the related packet section:
#if defined(ENABLE_PASSIVE_ATTR)
typedef struct SPacketCGPassiveAttr
{
	uint8_t bHeader;
	uint8_t bSubHeader;
	uint8_t bPage;
	uint16_t wMaterialPos[4];
	uint16_t wJobItemPos;
} TPacketCGPassiveAttr;

typedef struct SPacketGCPassiveAttr
{
	uint8_t bHeader;
	uint8_t bSubHeader;
	uint8_t bPage;
	uint8_t bResult;
	uint8_t bActive;
	uint32_t dwRemainSec;
	uint8_t bHasRelic;
} TPacketGCPassiveAttr;
#endif

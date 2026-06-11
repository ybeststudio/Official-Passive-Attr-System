// Add to includes:
#if defined(__PASSIVE_ATTR__)
#	include "passive_attr.h"

void CInputMain::PassiveAttr(LPCHARACTER ch, const char* c_pData)
{
	if (!ch)
		return;

	const TPacketCGPassiveAttr* p = reinterpret_cast<const TPacketCGPassiveAttr*>(c_pData);
	if (!p)
		return;

	CPassiveAttrManager::instance().RecvCGPacket(ch, p);
}
#endif

// In `int CInputMain::Analyze(LPDESC d, BYTE bHeader, const char* c_pData)`, extend the switch statement with:
#if defined(__PASSIVE_ATTR__)
		case HEADER_CG_PASSIVE_ATTR:
			PassiveAttr(ch, c_pData);
			break;
#endif

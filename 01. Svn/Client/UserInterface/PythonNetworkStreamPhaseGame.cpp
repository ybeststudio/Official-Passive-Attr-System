// Add to includes:
#if defined(ENABLE_PASSIVE_ATTR)
	#include "PythonPassiveAttr.h"
#endif

// In `void CPythonNetworkStream::GamePhase()`, extend the switch statement with:
#if defined(ENABLE_PASSIVE_ATTR)
			case HEADER_GC_PASSIVE_ATTR:
				ret = RecvPassiveAttr();
				break;
#endif

// Add the following `CPythonNetworkStream::SendPassiveAttrPacket` function anywhere in this file:
#if defined(ENABLE_PASSIVE_ATTR)
bool CPythonNetworkStream::SendPassiveAttrPacket(BYTE bSubHeader, BYTE bPage, const WORD* awMaterialPos, WORD wJobItemPos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGPassiveAttr pack = {};
	pack.bHeader = HEADER_CG_PASSIVE_ATTR;
	pack.bSubHeader = bSubHeader;
	pack.bPage = bPage;
	pack.wJobItemPos = wJobItemPos;

	if (awMaterialPos)
	{
		for (int i = 0; i < 4; ++i)
			pack.wMaterialPos[i] = awMaterialPos[i];
	}

	if (!Send(sizeof(pack), &pack))
	{
		Tracen("CPythonNetworkStream::SendPassiveAttrPacket Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::RecvPassiveAttr()
{
	TPacketGCPassiveAttr pack = {};
	if (!Recv(sizeof(pack), &pack))
	{
		Tracen("RecvPassiveAttr Error");
		return false;
	}

	if (!CPythonPassiveAttr::Instance().GetHandler())
	{
		if (m_apoPhaseWnd[PHASE_WINDOW_GAME])
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RegisterPassiveAttrHandler", Py_BuildValue("()"));
	}

	CPythonPassiveAttr::Instance().Process(pack.bSubHeader, pack.bPage, pack.bResult, pack.bActive, pack.dwRemainSec, pack.bHasRelic);
	return true;
}
#endif

#pragma once

#include "../../common/service.h"

#if defined(__PASSIVE_ATTR__)

#	include "../../common/tables.h"
#	include "packet.h"

class CPassiveAttrManager : public singleton<CPassiveAttrManager>
{
public:
	enum EPassiveAttrMaterialSlot : BYTE
	{
		PASSIVE_ATTR_MATERIAL_WEAPON = 0,
		PASSIVE_ATTR_MATERIAL_ELEMENT = 1,
		PASSIVE_ATTR_MATERIAL_ARMOR = 2,
		PASSIVE_ATTR_MATERIAL_ACCE = 3,
		PASSIVE_ATTR_MATERIAL_MAX = 4,
	};

	enum EPassiveAttrPage : BYTE
	{
		PASSIVE_ATTR_PAGE_UP = 1,
		PASSIVE_ATTR_PAGE_DOWN = 2,
		PASSIVE_ATTR_PAGE_MAX = 2,
	};

	enum EPassiveAttrCGSubHeader : BYTE
	{
		SUBHEADER_CG_PASSIVE_ATTR_OPEN = 0,
		SUBHEADER_CG_PASSIVE_ATTR_CLOSE = 1,
		SUBHEADER_CG_PASSIVE_ATTR_CHANGE_PAGE = 2,
		SUBHEADER_CG_PASSIVE_ATTR_ADD = 3,
		SUBHEADER_CG_PASSIVE_ATTR_CHARGE = 4,
		SUBHEADER_CG_PASSIVE_ATTR_ACTIVATE_DEACTIVATE = 5,
		SUBHEADER_CG_PASSIVE_ATTR_USE_ITEM_JOB = 6,
	};

	enum EPassiveAttrGCSubHeader : BYTE
	{
		SUBHEADER_GC_PASSIVE_ATTR_OPEN = 0,
		SUBHEADER_GC_PASSIVE_ATTR_CHANGE_WINDOW = 1,
		SUBHEADER_GC_PASSIVE_ATTR_CHARGE = 2,
		SUBHEADER_GC_PASSIVE_ATTR_ADD = 3,
		SUBHEADER_GC_PASSIVE_ATTR_ACTIVATE_DEACTIVATE = 4,
		SUBHEADER_GC_PASSIVE_ATTR_USE_ITEM_JOB = 5,
	};

	enum EPassiveAttrGCResult : BYTE
	{
		PASSIVE_ATTR_GC_OK = 0,
		PASSIVE_ATTR_GC_FAIL = 1,
		PASSIVE_ATTR_GC_FAIL_MAX_ATTR = 2,
		PASSIVE_ATTR_GC_FAIL_NOT_ENOUGH_JOB_ITEM = 3,
		PASSIVE_ATTR_GC_FAIL_FULL_TIME = 4,
		PASSIVE_ATTR_GC_FAIL_NOT_ENOUGH_SUB_ITEM = 5,
		PASSIVE_ATTR_GC_FAIL_COOLTIME = 6,
		PASSIVE_ATTR_GC_FAIL_CONTROL = 7,
	};

	void InitializeTable(const TPassiveAttrTable* pTable, WORD wSize);

	void RecvCGPacket(LPCHARACTER ch, const TPacketCGPassiveAttr* p);

	void OnAttackStone(LPCHARACTER ch, LPCHARACTER pkStone);
	void OnKillBoss(LPCHARACTER ch);
	void OnMount(LPCHARACTER ch);
	void OnDismount(LPCHARACTER ch);
	void OnAttackFromMount(LPCHARACTER ch, LPCHARACTER pkVictim);
	void OnAttack(LPCHARACTER ch, LPCHARACTER pkVictim);
	void OnDeath(LPCHARACTER ch, LPCHARACTER pkKiller);
	void OnCharacterItemLoad(LPCHARACTER ch);
	void OnPassiveRelicEquipped(LPCHARACTER ch, LPITEM pkRelic);

private:
	bool CanUse(LPCHARACTER ch, bool bCheckCooldown = true) const;
	void TouchCooldown(LPCHARACTER ch) const;
	void SetFirstAttackPending(LPCHARACTER ch, bool bPending) const;
	bool IsFirstAttackPending(LPCHARACTER ch) const;
	void TryPlayFirstAttackEffect(LPCHARACTER ch) const;

	LPITEM GetEquippedRelic(LPCHARACTER ch) const;
	bool IsRelicActive(LPITEM pkRelic) const;

	void SendGC(LPCHARACTER ch, BYTE bSubHeader, BYTE bPage, BYTE bResult, bool bActive, DWORD dwRemainSec) const;

	void HandleOpen(LPCHARACTER ch);
	void HandleClose(LPCHARACTER ch);
	void HandleChangePage(LPCHARACTER ch, BYTE bPage);
	void HandleAdd(LPCHARACTER ch, BYTE bPage, const WORD awMaterialPos[PASSIVE_ATTR_MATERIAL_MAX]);
	void HandleCharge(LPCHARACTER ch, BYTE bPage, const WORD awMaterialPos[PASSIVE_ATTR_MATERIAL_MAX]);
	void HandleActivateDeactivate(LPCHARACTER ch, BYTE bPage);
	void HandleUseItemJob(LPCHARACTER ch, BYTE bPage, WORD wJobItemPos);

	void SetRelicActive(LPCHARACTER ch, LPITEM pkRelic, bool bActive);
	void RefreshRelicState(LPCHARACTER ch, LPITEM pkRelic) const;

	int GetBonusCount(LPITEM pkRelic) const;
	bool AddRandomBonus(LPITEM pkRelic);
	bool TryProcBonus(LPCHARACTER ch, LPITEM pkRelic, WORD wApply, int iCooldownSec) const;

	void SaveDeckState(LPCHARACTER ch, LPITEM pkRelic, BYTE bPage);
	void LoadDeckState(LPCHARACTER ch, LPITEM pkRelic, BYTE bPage);
	void SwitchDeck(LPCHARACTER ch, BYTE bPage);
	void EnsureRelicDeckPage(LPCHARACTER ch, LPITEM pkRelic, BYTE bPage);

	BYTE GetSelectedPage(LPCHARACTER ch) const;
	void SetSelectedPage(LPCHARACTER ch, BYTE bPage) const;
	void InitializeDeckState(LPCHARACTER ch, LPITEM pkRelic);

	std::vector<TPassiveAttrTable> m_vecTable;
};

#endif

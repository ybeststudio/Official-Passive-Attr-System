#include "stdafx.h"

#if defined(__PASSIVE_ATTR__)

#include "passive_attr.h"
#include "utils.h"
#include "char.h"
#include "desc.h"
#include "item.h"
#include "item_manager.h"
#include "affect.h"
#include "log.h"
#include "constants.h"
#include "../../common/VnumHelper.h"

namespace
{
	bool IsPassiveAttrApply(WORD wApply)
	{
		switch (wApply)
		{
			case APPLY_HIT_BUFF_SUNGMA_STR:
			case APPLY_HIT_BUFF_SUNGMA_MOVE:
			case APPLY_HIT_BUFF_SUNGMA_HP:
			case APPLY_HIT_BUFF_SUNGMA_IMMUNE:
			case APPLY_HIT_AUTO_HP_RECOVERY:
			case APPLY_HIT_AUTO_SP_RECOVERY:
			case APPLY_HIT_STONE_DEF_GRADE_BONUS:
			case APPLY_HIT_STONE_ATTBONUS_STONE:
			case APPLY_KILL_BOSS_ITEM_BONUS:
			case APPLY_MOUNT_MELEE_MAGIC_ATTBONUS_PER:
			case APPLY_DISMOUNT_MOVE_SPEED_BONUS_PER:
			case APPLY_MOUNT_NO_KNOCKBACK:
			case APPLY_USE_SKILL_COOLTIME_DECREASE_ALL:
			case APPLY_AUTO_PICKUP:
			case APPLY_NO_DEATH_AND_HP_RECOVERY30:
			case APPLY_MOB_HIT_MOB_AGGRESSIVE:
				return true;
			default:
				return false;
		}
	}

	enum EPassiveRelicSocket
	{
		PASSIVE_RELIC_SOCKET_REMAIN = ITEM_SOCKET_REMAIN_SEC,
		PASSIVE_RELIC_SOCKET_ACTIVE = 1,
	};

	const char* PASSIVE_ATTR_COOLDOWN_FLAG = "passive_attr.cooldown";
	const char* PASSIVE_ATTR_FIRST_ATTACK_FLAG = "passive_attr.first_attack";
	const char* PASSIVE_ATTR_DECK_SELECTED_FLAG = "passive_attr.deck.selected";
	const char* PASSIVE_ATTR_DECK_INIT_FLAG = "passive_attr.deck.init";
	const int PASSIVE_ATTR_ACTION_COOLDOWN_SEC = 1;
	const char* PASSIVE_ATTR_DECK_ATTR_TYPE_FLAG_FMT = "passive_attr.deck%d.attr.type%d";
	const char* PASSIVE_ATTR_DECK_ATTR_VALUE_FLAG_FMT = "passive_attr.deck%d.attr.value%d";
	const char* PASSIVE_ATTR_DECK_HAS_RELIC_FLAG_FMT = "passive_attr.deck%d.has_relic";
	const char* PASSIVE_ATTR_DECK_ACTIVE_FLAG_FMT = "passive_attr.deck%d.active";
	const char* PASSIVE_ATTR_DECK_REMAIN_FLAG_FMT = "passive_attr.deck%d.remain";

	const BYTE PASSIVE_ATTR_MAX_BONUS_COUNT = 4;
	const int PASSIVE_ATTR_ADD_SUCCESS_RATE = 70;
	const int PASSIVE_ATTR_DIRECT_UNEQUIP_SUCCESS_CHANCE = 20;
	const int PASSIVE_ATTR_FALLBACK_DURATION = 60 * 60 * 12;

	const DWORD PASSIVE_ATTR_MATERIAL_VNUM[CPassiveAttrManager::PASSIVE_ATTR_MATERIAL_MAX] =
	{
		30255,
		30258,
		30256,
		30257,
	};

	const BYTE PASSIVE_ATTR_MATERIAL_SUBTYPE[CPassiveAttrManager::PASSIVE_ATTR_MATERIAL_MAX] =
	{
		MATERIAL_PASSIVE_WEAPON,
		MATERIAL_PASSIVE_ELEMENT,
		MATERIAL_PASSIVE_ARMOR,
		MATERIAL_PASSIVE_ACCE,
	};

	BYTE PageToDeckIndex(BYTE bPage)
	{
		// deck_button1 (UP) -> PASSIVE_ATTR_PAGE_UP -> deck 0
		// deck_button2 (DOWN) -> PASSIVE_ATTR_PAGE_DOWN -> deck 1
		return (bPage == CPassiveAttrManager::PASSIVE_ATTR_PAGE_UP) ? 0 : 1;
	}

	BYTE DeckIndexToPage(BYTE bDeck)
	{
		return (bDeck == 0) ? CPassiveAttrManager::PASSIVE_ATTR_PAGE_UP : CPassiveAttrManager::PASSIVE_ATTR_PAGE_DOWN;
	}

	std::string BuildDeckHasRelicFlag(BYTE bDeck)
	{
		char szFlag[64];
		snprintf(szFlag, sizeof(szFlag), PASSIVE_ATTR_DECK_HAS_RELIC_FLAG_FMT, bDeck);
		return szFlag;
	}

	std::string BuildDeckActiveFlag(BYTE bDeck)
	{
		char szFlag[64];
		snprintf(szFlag, sizeof(szFlag), PASSIVE_ATTR_DECK_ACTIVE_FLAG_FMT, bDeck);
		return szFlag;
	}

	std::string BuildDeckRemainFlag(BYTE bDeck)
	{
		char szFlag[64];
		snprintf(szFlag, sizeof(szFlag), PASSIVE_ATTR_DECK_REMAIN_FLAG_FMT, bDeck);
		return szFlag;
	}

	bool HasDeckRelic(LPCHARACTER ch, BYTE bPage)
	{
		if (!ch)
			return false;

		return ch->GetQuestFlag(BuildDeckHasRelicFlag(PageToDeckIndex(bPage)).c_str()) != 0;
	}

	void SetDeckHasRelic(LPCHARACTER ch, BYTE bPage, bool bHas)
	{
		if (!ch)
			return;

		ch->SetQuestFlag(BuildDeckHasRelicFlag(PageToDeckIndex(bPage)).c_str(), bHas ? 1 : 0);
	}

	std::string BuildDeckAttrTypeFlag(BYTE bDeck, int iAttrIndex)
	{
		char szFlag[64];
		snprintf(szFlag, sizeof(szFlag), PASSIVE_ATTR_DECK_ATTR_TYPE_FLAG_FMT, bDeck, iAttrIndex);
		return szFlag;
	}

	std::string BuildDeckAttrValueFlag(BYTE bDeck, int iAttrIndex)
	{
		char szFlag[64];
		snprintf(szFlag, sizeof(szFlag), PASSIVE_ATTR_DECK_ATTR_VALUE_FLAG_FMT, bDeck, iAttrIndex);
		return szFlag;
	}

	int GetRelicDurationSeconds(LPITEM pkRelic)
	{
		if (!pkRelic)
			return 0;

		const int iDuration = pkRelic->GetDuration();
		if (iDuration > 0)
			return iDuration;

		const int iRemain = pkRelic->GetSocket(PASSIVE_RELIC_SOCKET_REMAIN);
		if (iRemain > 0)
			return iRemain;

		return PASSIVE_ATTR_FALLBACK_DURATION;
	}
}

void CPassiveAttrManager::InitializeTable(const TPassiveAttrTable* pTable, WORD wSize)
{
	m_vecTable.clear();
	if (!pTable || wSize == 0)
	{
		sys_err("PASSIVE_ATTR: empty table (import passive_attr.sql and rebuild db)");
		return;
	}

	m_vecTable.reserve(wSize);
	for (WORD i = 0; i < wSize; ++i)
	{
		TPassiveAttrTable row = pTable[i];
		if (!row.szApply[0])
			continue;

		const long lExpectedApply = FN_get_apply_type(row.szApply);
		if (lExpectedApply <= 0)
		{
			sys_err("PASSIVE_ATTR: unknown apply name '%s'", row.szApply);
			continue;
		}

		if (row.wApplyIndex != static_cast<POINT_TYPE>(lExpectedApply))
		{
			sys_err("PASSIVE_ATTR: apply index mismatch for %s (got %u expected %ld) - corrected",
				row.szApply, row.wApplyIndex, lExpectedApply);
			row.wApplyIndex = static_cast<POINT_TYPE>(lExpectedApply);
		}

		if (!IsPassiveAttrApply(row.wApplyIndex))
		{
			sys_err("PASSIVE_ATTR: %s (%u) is not an official passive relic bonus - skipped",
				row.szApply, row.wApplyIndex);
			continue;
		}

		m_vecTable.push_back(row);
		sys_log(0, "PASSIVE_ATTR: %s -> apply %u", row.szApply, row.wApplyIndex);
	}

	if (m_vecTable.empty())
		sys_err("PASSIVE_ATTR: no valid rows loaded");

	sys_log(0, "PASSIVE_ATTR: loaded %u rows", static_cast<WORD>(m_vecTable.size()));
}

void CPassiveAttrManager::SendGC(LPCHARACTER ch, BYTE bSubHeader, BYTE bPage, BYTE bResult, bool bActive, DWORD dwRemainSec) const
{
	if (!ch || !ch->GetDesc())
		return;

	TPacketGCPassiveAttr pack = {};
	pack.bHeader = HEADER_GC_PASSIVE_ATTR;
	pack.bSubHeader = bSubHeader;
	pack.bPage = bPage;
	pack.bResult = bResult;
	pack.bActive = bActive ? 1 : 0;
	pack.dwRemainSec = dwRemainSec;
	pack.bHasRelic = (ch && HasDeckRelic(ch, bPage)) ? 1 : 0;
	ch->GetDesc()->Packet(&pack, sizeof(pack));
}

bool CPassiveAttrManager::CanUse(LPCHARACTER ch, bool bCheckCooldown) const
{
	if (!ch || !ch->IsPC())
		return false;

	if (ch->IsDead() || ch->IsStun())
		return false;

	if (ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen())
		return false;

	if (bCheckCooldown && ch->GetQuestFlag(PASSIVE_ATTR_COOLDOWN_FLAG) > get_global_time())
		return false;

	return true;
}

void CPassiveAttrManager::TouchCooldown(LPCHARACTER ch) const
{
	if (ch)
		ch->SetQuestFlag(PASSIVE_ATTR_COOLDOWN_FLAG, get_global_time() + PASSIVE_ATTR_ACTION_COOLDOWN_SEC);
}

void CPassiveAttrManager::SetFirstAttackPending(LPCHARACTER ch, bool bPending) const
{
	if (ch)
		ch->SetQuestFlag(PASSIVE_ATTR_FIRST_ATTACK_FLAG, bPending ? 1 : 0);
}

bool CPassiveAttrManager::IsFirstAttackPending(LPCHARACTER ch) const
{
	return ch && ch->GetQuestFlag(PASSIVE_ATTR_FIRST_ATTACK_FLAG) != 0;
}

void CPassiveAttrManager::TryPlayFirstAttackEffect(LPCHARACTER ch) const
{
	if (!ch || !IsFirstAttackPending(ch))
		return;

	SetFirstAttackPending(ch, false);
	ch->EffectPacket(SE_PASSIVE_EFFECT);
	ch->SpecificEffectPacket("d:/ymir work/effect/etc/buff/buff_passive_01.mse");
}

LPITEM CPassiveAttrManager::GetEquippedRelic(LPCHARACTER ch) const
{
	if (!ch)
		return nullptr;

	return ch->GetWear(WEAR_PASSIVE);
}

bool CPassiveAttrManager::IsRelicActive(LPITEM pkRelic) const
{
	return pkRelic && pkRelic->GetSocket(PASSIVE_RELIC_SOCKET_ACTIVE) != 0;
}

BYTE CPassiveAttrManager::GetSelectedPage(LPCHARACTER ch) const
{
	if (!ch)
		return PASSIVE_ATTR_PAGE_UP;

	return DeckIndexToPage(static_cast<BYTE>(ch->GetQuestFlag(PASSIVE_ATTR_DECK_SELECTED_FLAG)));
}

void CPassiveAttrManager::SetSelectedPage(LPCHARACTER ch, BYTE bPage) const
{
	if (ch)
		ch->SetQuestFlag(PASSIVE_ATTR_DECK_SELECTED_FLAG, PageToDeckIndex(bPage));
}

void CPassiveAttrManager::InitializeDeckState(LPCHARACTER ch, LPITEM pkRelic)
{
	if (!ch || !pkRelic)
		return;

	if (ch->GetQuestFlag(PASSIVE_ATTR_DECK_INIT_FLAG) != 0)
		return;

	SaveDeckState(ch, pkRelic, PASSIVE_ATTR_PAGE_UP);
	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
	{
		ch->SetQuestFlag(BuildDeckAttrTypeFlag(1, i), 0);
		ch->SetQuestFlag(BuildDeckAttrValueFlag(1, i), 0);
	}
	SetSelectedPage(ch, PASSIVE_ATTR_PAGE_UP);
	SetDeckHasRelic(ch, PASSIVE_ATTR_PAGE_UP, true);
	ch->SetQuestFlag(PASSIVE_ATTR_DECK_INIT_FLAG, 1);
}

void CPassiveAttrManager::SaveDeckState(LPCHARACTER ch, LPITEM pkRelic, BYTE bPage)
{
	if (!ch || !pkRelic)
		return;

	const BYTE bDeck = PageToDeckIndex(bPage);
	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
	{
		const TPlayerItemAttribute& rAttr = pkRelic->GetAttribute(i);
		ch->SetQuestFlag(BuildDeckAttrTypeFlag(bDeck, i), rAttr.wType);
		ch->SetQuestFlag(BuildDeckAttrValueFlag(bDeck, i), rAttr.lValue);
	}
	ch->SetQuestFlag(BuildDeckActiveFlag(bDeck).c_str(), IsRelicActive(pkRelic) ? 1 : 0);
	ch->SetQuestFlag(BuildDeckRemainFlag(bDeck).c_str(), static_cast<int>(pkRelic->GetSocket(PASSIVE_RELIC_SOCKET_REMAIN)));
}

void CPassiveAttrManager::LoadDeckState(LPCHARACTER ch, LPITEM pkRelic, BYTE bPage)
{
	if (!ch || !pkRelic)
		return;

	const BYTE bDeck = PageToDeckIndex(bPage);
	const bool bHasRelic = ch->GetQuestFlag(BuildDeckHasRelicFlag(bDeck).c_str()) != 0;

	while (pkRelic->GetAttributeCount() > 0)
	{
		if (!pkRelic->RemoveAttributeAt(pkRelic->GetAttributeCount() - 1))
			break;
	}

	if (!bHasRelic)
	{
		pkRelic->SetSocket(PASSIVE_RELIC_SOCKET_ACTIVE, 0);
		pkRelic->SetSocket(PASSIVE_RELIC_SOCKET_REMAIN, 0);
		return;
	}

	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
	{
		const int iType = ch->GetQuestFlag(BuildDeckAttrTypeFlag(bDeck, i));
		const int iValue = ch->GetQuestFlag(BuildDeckAttrValueFlag(bDeck, i));
		if (iType <= 0 || iValue == 0)
			continue;

		pkRelic->AddAttribute(static_cast<WORD>(iType), static_cast<short>(iValue));
	}

	const bool bActive = ch->GetQuestFlag(BuildDeckActiveFlag(bDeck).c_str()) != 0;
	const int iRemain = ch->GetQuestFlag(BuildDeckRemainFlag(bDeck).c_str());
	pkRelic->SetSocket(PASSIVE_RELIC_SOCKET_REMAIN, iRemain > 0 ? static_cast<long>(iRemain) : 0);
	pkRelic->SetSocket(PASSIVE_RELIC_SOCKET_ACTIVE, bActive ? 1 : 0);
}

void CPassiveAttrManager::SwitchDeck(LPCHARACTER ch, BYTE bPage)
{
	if (!ch)
		return;

	if (bPage != PASSIVE_ATTR_PAGE_UP && bPage != PASSIVE_ATTR_PAGE_DOWN)
		return;

	const BYTE bCurrentPage = GetSelectedPage(ch);
	LPITEM pkRelic = ch->GetWear(WEAR_PASSIVE);

	if (bCurrentPage == bPage)
	{
		const bool bHasRelic = pkRelic && HasDeckRelic(ch, bPage);
		SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_CHANGE_WINDOW, bPage, PASSIVE_ATTR_GC_OK,
			bHasRelic && IsRelicActive(pkRelic),
			bHasRelic ? pkRelic->GetSocket(PASSIVE_RELIC_SOCKET_REMAIN) : 0);
		return;
	}

	if (pkRelic)
	{
		const bool bWasActive = IsRelicActive(pkRelic);
		if (bWasActive && pkRelic->IsEquipped())
			pkRelic->ModifyPoints(false);

		SaveDeckState(ch, pkRelic, bCurrentPage);
		LoadDeckState(ch, pkRelic, bPage);

		if (IsRelicActive(pkRelic) && pkRelic->IsEquipped())
			pkRelic->ModifyPoints(true);
	}

	SetSelectedPage(ch, bPage);

	ch->RemoveAffect(AFFECT_PASSIVE_JOB_DECK);
	if (pkRelic && HasDeckRelic(ch, bPage) && IsRelicActive(pkRelic))
		ch->AddAffect(AFFECT_PASSIVE_JOB_DECK, POINT_NONE, bPage, AFF_NONE, INFINITE_AFFECT_DURATION, 0, true);

	const bool bHasRelic = pkRelic && HasDeckRelic(ch, bPage);
	SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_CHANGE_WINDOW, bPage, PASSIVE_ATTR_GC_OK,
		bHasRelic && IsRelicActive(pkRelic),
		bHasRelic ? pkRelic->GetSocket(PASSIVE_RELIC_SOCKET_REMAIN) : 0);

	if (pkRelic)
	{
		pkRelic->UpdatePacket();
		ch->UpdatePacket();
	}
}

void CPassiveAttrManager::EnsureRelicDeckPage(LPCHARACTER ch, LPITEM pkRelic, BYTE bPage)
{
	if (!ch || !pkRelic)
		return;

	if (bPage != PASSIVE_ATTR_PAGE_UP && bPage != PASSIVE_ATTR_PAGE_DOWN)
		return;

	InitializeDeckState(ch, pkRelic);

	if (GetSelectedPage(ch) != bPage)
		SwitchDeck(ch, bPage);
}

void CPassiveAttrManager::RefreshRelicState(LPCHARACTER ch, LPITEM pkRelic) const
{
	if (!ch || !pkRelic)
		return;

	if (pkRelic->IsEquipped())
	{
		ch->ComputeBattlePoints();
		ch->UpdatePacket();
	}
	pkRelic->UpdatePacket();
}

void CPassiveAttrManager::SetRelicActive(LPCHARACTER ch, LPITEM pkRelic, bool bActive)
{
	if (!ch || !pkRelic)
		return;

	const bool bWasActive = IsRelicActive(pkRelic);
	if (bWasActive == bActive)
		return;

	if (pkRelic->IsEquipped() && bWasActive)
		pkRelic->ModifyPoints(false);

	pkRelic->SetSocket(PASSIVE_RELIC_SOCKET_ACTIVE, bActive ? 1 : 0);

	if (pkRelic->IsEquipped() && bActive)
	{
		pkRelic->ModifyPoints(true);
		SetFirstAttackPending(ch, true);
		ch->AddAffect(AFFECT_PASSIVE_JOB_DECK, POINT_NONE, GetSelectedPage(ch), AFF_NONE, INFINITE_AFFECT_DURATION, 0, true);
	}
	else if (!bActive)
	{
		SetFirstAttackPending(ch, false);
		ch->RemoveAffect(AFFECT_PASSIVE_JOB_DECK);
	}

	RefreshRelicState(ch, pkRelic);
}

int CPassiveAttrManager::GetBonusCount(LPITEM pkRelic) const
{
	return pkRelic ? pkRelic->GetAttributeCount() : 0;
}

bool CPassiveAttrManager::AddRandomBonus(LPITEM pkRelic)
{
	if (!pkRelic || m_vecTable.empty())
		return false;

	std::vector<size_t> vecAvailable;
	for (size_t i = 0; i < m_vecTable.size(); ++i)
	{
		const TPassiveAttrTable& rRow = m_vecTable[i];
		if (!pkRelic->HasAttr(rRow.wApplyIndex))
			vecAvailable.push_back(i);
	}

	if (vecAvailable.empty())
		return false;

	const TPassiveAttrTable& rSelected = m_vecTable[vecAvailable[number(0, vecAvailable.size() - 1)]];
	std::vector<short> vecValues;
	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_LEVEL; ++i)
	{
		if (rSelected.lValues[i] != 0)
			vecValues.push_back(static_cast<short>(rSelected.lValues[i]));
	}
	if (vecValues.empty())
		return false;

	const short sValue = vecValues[number(0, vecValues.size() - 1)];
	pkRelic->AddAttribute(rSelected.wApplyIndex, sValue);
	return true;
}

void CPassiveAttrManager::RecvCGPacket(LPCHARACTER ch, const TPacketCGPassiveAttr* p)
{
	if (!ch || !p)
		return;

	switch (p->bSubHeader)
	{
		case SUBHEADER_CG_PASSIVE_ATTR_OPEN:
			HandleOpen(ch);
			break;
		case SUBHEADER_CG_PASSIVE_ATTR_CLOSE:
			HandleClose(ch);
			break;
		case SUBHEADER_CG_PASSIVE_ATTR_CHANGE_PAGE:
			HandleChangePage(ch, p->bPage);
			break;
		case SUBHEADER_CG_PASSIVE_ATTR_ADD:
			HandleAdd(ch, p->bPage, p->wMaterialPos);
			break;
		case SUBHEADER_CG_PASSIVE_ATTR_CHARGE:
			HandleCharge(ch, p->bPage, p->wMaterialPos);
			break;
		case SUBHEADER_CG_PASSIVE_ATTR_ACTIVATE_DEACTIVATE:
			HandleActivateDeactivate(ch, p->bPage);
			break;
		case SUBHEADER_CG_PASSIVE_ATTR_USE_ITEM_JOB:
			HandleUseItemJob(ch, p->bPage, p->wJobItemPos);
			break;
		default:
			sys_err("CPassiveAttrManager::RecvCGPacket unknown subheader %u (%s)", p->bSubHeader, ch->GetName());
			break;
	}
}

void CPassiveAttrManager::HandleOpen(LPCHARACTER ch)
{
	if (!CanUse(ch, false))
	{
		SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_OPEN, GetSelectedPage(ch), PASSIVE_ATTR_GC_FAIL_COOLTIME, false, 0);
		return;
	}

	LPITEM pkRelic = GetEquippedRelic(ch);
	if (!pkRelic)
	{
		SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_OPEN, GetSelectedPage(ch), PASSIVE_ATTR_GC_OK, false, 0);
		return;
	}

	InitializeDeckState(ch, pkRelic);

	const BYTE bPage = GetSelectedPage(ch);
	LoadDeckState(ch, pkRelic, bPage);
	RefreshRelicState(ch, pkRelic);

	SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_OPEN, bPage, PASSIVE_ATTR_GC_OK,
		HasDeckRelic(ch, bPage) && IsRelicActive(pkRelic), pkRelic->GetSocket(PASSIVE_RELIC_SOCKET_REMAIN));
}

void CPassiveAttrManager::HandleClose(LPCHARACTER ch)
{
	if (!ch)
		return;

	LPITEM pkRelic = GetEquippedRelic(ch);
	if (pkRelic)
		SaveDeckState(ch, pkRelic, GetSelectedPage(ch));
}

void CPassiveAttrManager::HandleChangePage(LPCHARACTER ch, BYTE bPage)
{
	if (!CanUse(ch, false))
		return;

	if (bPage != PASSIVE_ATTR_PAGE_UP && bPage != PASSIVE_ATTR_PAGE_DOWN)
		return;

	LPITEM pkRelic = GetEquippedRelic(ch);
	if (pkRelic)
	{
		SwitchDeck(ch, bPage);
		return;
	}

	const BYTE bCurrentPage = GetSelectedPage(ch);
	if (bCurrentPage == bPage)
	{
		SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_CHANGE_WINDOW, bPage, PASSIVE_ATTR_GC_OK, false, 0);
		return;
	}

	SetSelectedPage(ch, bPage);
	SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_CHANGE_WINDOW, bPage, PASSIVE_ATTR_GC_OK, false, 0);
}

void CPassiveAttrManager::HandleAdd(LPCHARACTER ch, BYTE bPage, const WORD awMaterialPos[PASSIVE_ATTR_MATERIAL_MAX])
{
	if (!CanUse(ch))
	{
		SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_ADD, bPage, PASSIVE_ATTR_GC_FAIL_COOLTIME, false, 0);
		return;
	}

	LPITEM pkRelic = GetEquippedRelic(ch);
	if (!pkRelic)
		return;

	if (bPage != PASSIVE_ATTR_PAGE_UP && bPage != PASSIVE_ATTR_PAGE_DOWN)
		return;

	EnsureRelicDeckPage(ch, pkRelic, bPage);

	if (!HasDeckRelic(ch, bPage))
	{
		SetDeckHasRelic(ch, bPage, true);
		SaveDeckState(ch, pkRelic, bPage);
	}

	if (IsRelicActive(pkRelic))
	{
		SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_ADD, bPage, PASSIVE_ATTR_GC_FAIL_CONTROL, true, 0);
		return;
	}

	if (GetBonusCount(pkRelic) >= PASSIVE_ATTR_MAX_BONUS_COUNT)
	{
		SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_ADD, bPage, PASSIVE_ATTR_GC_FAIL_MAX_ATTR, false, 0);
		return;
	}

	LPITEM apItems[PASSIVE_ATTR_MATERIAL_MAX] = {};
	for (int i = 0; i < PASSIVE_ATTR_MATERIAL_MAX; ++i)
	{
		const WORD wPos = awMaterialPos[i];
		if (wPos >= INVENTORY_MAX_NUM)
		{
			SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_ADD, bPage, PASSIVE_ATTR_GC_FAIL_NOT_ENOUGH_SUB_ITEM, false, 0);
			return;
		}

		LPITEM pkItem = ch->GetInventoryItem(wPos);
		if (!pkItem || pkItem->GetType() != ITEM_MATERIAL || pkItem->GetSubType() != PASSIVE_ATTR_MATERIAL_SUBTYPE[i]
			|| pkItem->GetVnum() != PASSIVE_ATTR_MATERIAL_VNUM[i] || pkItem->GetCount() <= 0)
		{
			SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_ADD, bPage, PASSIVE_ATTR_GC_FAIL_NOT_ENOUGH_SUB_ITEM, false, 0);
			return;
		}
		apItems[i] = pkItem;
	}

	for (int i = 0; i < PASSIVE_ATTR_MATERIAL_MAX; ++i)
	{
		if (apItems[i]->GetCount() > 1)
			apItems[i]->SetCount(apItems[i]->GetCount() - 1);
		else
			apItems[i]->SetCount(0);
	}

	if (number(1, 100) > PASSIVE_ATTR_ADD_SUCCESS_RATE)
	{
		SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_ADD, bPage, PASSIVE_ATTR_GC_FAIL, false, 0);
		return;
	}

	if (!AddRandomBonus(pkRelic))
	{
		SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_ADD, bPage, PASSIVE_ATTR_GC_FAIL_MAX_ATTR, false, 0);
		return;
	}

	SaveDeckState(ch, pkRelic, bPage);
	RefreshRelicState(ch, pkRelic);
	TouchCooldown(ch);
	SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_ADD, bPage, PASSIVE_ATTR_GC_OK, false, pkRelic->GetSocket(PASSIVE_RELIC_SOCKET_REMAIN));
}

void CPassiveAttrManager::HandleCharge(LPCHARACTER ch, BYTE bPage, const WORD awMaterialPos[PASSIVE_ATTR_MATERIAL_MAX])
{
	if (!CanUse(ch))
	{
		SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_CHARGE, bPage, PASSIVE_ATTR_GC_FAIL_COOLTIME, false, 0);
		return;
	}

	LPITEM pkRelic = GetEquippedRelic(ch);
	if (!pkRelic)
		return;

	if (bPage != PASSIVE_ATTR_PAGE_UP && bPage != PASSIVE_ATTR_PAGE_DOWN)
		return;

	EnsureRelicDeckPage(ch, pkRelic, bPage);

	if (!HasDeckRelic(ch, bPage))
	{
		SetDeckHasRelic(ch, bPage, true);
		SaveDeckState(ch, pkRelic, bPage);
	}

	if (IsRelicActive(pkRelic))
	{
		SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_CHARGE, bPage, PASSIVE_ATTR_GC_FAIL_CONTROL, true, 0);
		return;
	}

	LPITEM apItems[PASSIVE_ATTR_MATERIAL_MAX] = {};
	for (int i = 0; i < PASSIVE_ATTR_MATERIAL_MAX; ++i)
	{
		const WORD wPos = awMaterialPos[i];
		LPITEM pkItem = (wPos < INVENTORY_MAX_NUM) ? ch->GetInventoryItem(wPos) : nullptr;
		if (!pkItem || pkItem->GetType() != ITEM_MATERIAL || pkItem->GetSubType() != PASSIVE_ATTR_MATERIAL_SUBTYPE[i]
			|| pkItem->GetVnum() != PASSIVE_ATTR_MATERIAL_VNUM[i] || pkItem->GetCount() <= 0)
		{
			SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_CHARGE, bPage, PASSIVE_ATTR_GC_FAIL_NOT_ENOUGH_SUB_ITEM, false, 0);
			return;
		}
		apItems[i] = pkItem;
	}

	const int iDuration = GetRelicDurationSeconds(pkRelic);
	if (iDuration <= 0)
		return;

	if (pkRelic->GetSocket(PASSIVE_RELIC_SOCKET_REMAIN) >= iDuration)
	{
		SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_CHARGE, bPage, PASSIVE_ATTR_GC_FAIL_FULL_TIME, false, iDuration);
		return;
	}

	if (pkRelic->IsEquipped() && -1 != pkRelic->GetProto()->cLimitTimerBasedOnWearIndex)
		pkRelic->StopTimerBasedOnWearExpireEvent();

	pkRelic->SetSocket(PASSIVE_RELIC_SOCKET_REMAIN, iDuration);

	if (pkRelic->IsEquipped() && -1 != pkRelic->GetProto()->cLimitTimerBasedOnWearIndex)
		pkRelic->StartTimerBasedOnWearExpireEvent();

	for (int i = 0; i < PASSIVE_ATTR_MATERIAL_MAX; ++i)
	{
		if (apItems[i]->GetCount() > 1)
			apItems[i]->SetCount(apItems[i]->GetCount() - 1);
		else
			apItems[i]->SetCount(0);
	}

	RefreshRelicState(ch, pkRelic);
	SaveDeckState(ch, pkRelic, bPage);
	TouchCooldown(ch);
	SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_CHARGE, bPage, PASSIVE_ATTR_GC_OK, false, iDuration);
}

void CPassiveAttrManager::HandleActivateDeactivate(LPCHARACTER ch, BYTE bPage)
{
	if (!CanUse(ch))
	{
		SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_ACTIVATE_DEACTIVATE, bPage, PASSIVE_ATTR_GC_FAIL_COOLTIME, false, 0);
		return;
	}

	LPITEM pkRelic = GetEquippedRelic(ch);
	if (!pkRelic)
		return;

	if (bPage != PASSIVE_ATTR_PAGE_UP && bPage != PASSIVE_ATTR_PAGE_DOWN)
		return;

	EnsureRelicDeckPage(ch, pkRelic, bPage);

	if (!HasDeckRelic(ch, bPage))
	{
		SetDeckHasRelic(ch, bPage, true);
		SaveDeckState(ch, pkRelic, bPage);
	}

	if (GetBonusCount(pkRelic) <= 0)
	{
		SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_ACTIVATE_DEACTIVATE, bPage, PASSIVE_ATTR_GC_FAIL, false, 0);
		return;
	}

	const int iDuration = GetRelicDurationSeconds(pkRelic);
	if (iDuration > 0 && pkRelic->GetSocket(PASSIVE_RELIC_SOCKET_REMAIN) <= 0)
	{
		SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_ACTIVATE_DEACTIVATE, bPage, PASSIVE_ATTR_GC_FAIL_FULL_TIME, false, 0);
		return;
	}

	const bool bNext = !IsRelicActive(pkRelic);
	SetRelicActive(ch, pkRelic, bNext);
	SaveDeckState(ch, pkRelic, bPage);
	TouchCooldown(ch);
	SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_ACTIVATE_DEACTIVATE, bPage, PASSIVE_ATTR_GC_OK,
		bNext, pkRelic->GetSocket(PASSIVE_RELIC_SOCKET_REMAIN));
}

void CPassiveAttrManager::HandleUseItemJob(LPCHARACTER ch, BYTE bPage, WORD wJobItemPos)
{
	if (!ch)
		return;

	LPITEM pkRelic = GetEquippedRelic(ch);
	if (!pkRelic)
		return;

	if (bPage != PASSIVE_ATTR_PAGE_UP && bPage != PASSIVE_ATTR_PAGE_DOWN)
		return;

	EnsureRelicDeckPage(ch, pkRelic, bPage);

	if (IsRelicActive(pkRelic))
	{
		SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_USE_ITEM_JOB, bPage, PASSIVE_ATTR_GC_FAIL_CONTROL, true, 0);
		return;
	}

	if (wJobItemPos == 0xFFFF)
	{
		if (!HasDeckRelic(ch, bPage))
			return;

		if (number(1, 100) > PASSIVE_ATTR_DIRECT_UNEQUIP_SUCCESS_CHANCE)
		{
			SaveDeckState(ch, pkRelic, bPage);
			SetDeckHasRelic(ch, bPage, false);
			pkRelic->RemoveFromCharacter();
			pkRelic->SetCount(0);
			SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_USE_ITEM_JOB, bPage, PASSIVE_ATTR_GC_FAIL, false, 0);
			return;
		}

		const int iEmptyPos = ch->GetEmptyInventory(pkRelic->GetSize());
		if (iEmptyPos < 0)
			return;

		SaveDeckState(ch, pkRelic, bPage);
		SetDeckHasRelic(ch, bPage, false);
		pkRelic->RemoveFromCharacter();
		pkRelic->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));
		SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_USE_ITEM_JOB, bPage, PASSIVE_ATTR_GC_OK, false, 0);
		return;
	}

	if (wJobItemPos >= INVENTORY_MAX_NUM)
		return;

	LPITEM pkJob = ch->GetInventoryItem(wJobItemPos);
	if (!pkJob || pkJob->GetType() != ITEM_PASSIVE || pkJob->GetSubType() != PASSIVE_JOB
		|| !CItemVnumHelper::IsPassive(pkJob->GetVnum()))
		return;

	if (pkRelic->GetVnum() != pkJob->GetVnum())
		return;

	ch->EquipItem(pkJob);
	RefreshRelicState(ch, pkRelic);
	SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_USE_ITEM_JOB, bPage, PASSIVE_ATTR_GC_OK, false, 0);
}

bool CPassiveAttrManager::TryProcBonus(LPCHARACTER ch, LPITEM pkRelic, WORD wApply, int iCooldownSec) const
{
	if (!ch || !pkRelic || !IsRelicActive(pkRelic))
		return false;

	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
	{
		const TPlayerItemAttribute& rAttr = pkRelic->GetAttribute(i);
		if (rAttr.wType != wApply || rAttr.lValue == 0)
			continue;

		if (iCooldownSec > 0)
		{
			char szFlag[64];
			snprintf(szFlag, sizeof(szFlag), "passive_attr.proc.%u.%d", wApply, i);
			if (ch->GetQuestFlag(szFlag) > get_global_time())
				return false;
			ch->SetQuestFlag(szFlag, get_global_time() + iCooldownSec);
		}

		ch->ApplyPoint(wApply, rAttr.lValue);
		return true;
	}
	return false;
}

void CPassiveAttrManager::OnAttackStone(LPCHARACTER ch, LPCHARACTER pkStone)
{
	LPITEM pkRelic = GetEquippedRelic(ch);
	if (!pkRelic || !pkStone)
		return;

	for (size_t i = 0; i < m_vecTable.size(); ++i)
	{
		const TPassiveAttrTable& rRow = m_vecTable[i];
		if (rRow.wApplyIndex == APPLY_HIT_STONE_DEF_GRADE_BONUS || rRow.wApplyIndex == APPLY_HIT_STONE_ATTBONUS_STONE)
			TryProcBonus(ch, pkRelic, rRow.wApplyIndex, rRow.dwCooldownSec);
	}
}

void CPassiveAttrManager::OnKillBoss(LPCHARACTER ch)
{
	LPITEM pkRelic = GetEquippedRelic(ch);
	if (!pkRelic)
		return;
	TryProcBonus(ch, pkRelic, APPLY_KILL_BOSS_ITEM_BONUS, 0);
}

void CPassiveAttrManager::OnMount(LPCHARACTER ch)
{
	LPITEM pkRelic = GetEquippedRelic(ch);
	if (!pkRelic)
		return;
	TryProcBonus(ch, pkRelic, APPLY_MOUNT_MELEE_MAGIC_ATTBONUS_PER, 120);
}

void CPassiveAttrManager::OnDismount(LPCHARACTER ch)
{
	LPITEM pkRelic = GetEquippedRelic(ch);
	if (!pkRelic)
		return;
	TryProcBonus(ch, pkRelic, APPLY_DISMOUNT_MOVE_SPEED_BONUS_PER, 120);
}

void CPassiveAttrManager::OnAttackFromMount(LPCHARACTER ch, LPCHARACTER pkVictim)
{
	LPITEM pkRelic = GetEquippedRelic(ch);
	if (!pkRelic)
		return;
	TryProcBonus(ch, pkRelic, APPLY_MOUNT_NO_KNOCKBACK, 120);
}

void CPassiveAttrManager::OnAttack(LPCHARACTER ch, LPCHARACTER pkVictim)
{
	LPITEM pkRelic = GetEquippedRelic(ch);
	if (!pkRelic || !IsRelicActive(pkRelic))
		return;

	TryPlayFirstAttackEffect(ch);

	for (size_t i = 0; i < m_vecTable.size(); ++i)
	{
		const TPassiveAttrTable& rRow = m_vecTable[i];
		switch (rRow.wApplyIndex)
		{
			case APPLY_HIT_BUFF_SUNGMA_STR:
			case APPLY_HIT_BUFF_SUNGMA_HP:
			case APPLY_HIT_BUFF_SUNGMA_MOVE:
			case APPLY_HIT_BUFF_SUNGMA_IMMUNE:
			case APPLY_HIT_AUTO_HP_RECOVERY:
			case APPLY_HIT_AUTO_SP_RECOVERY:
			case APPLY_USE_SKILL_COOLTIME_DECREASE_ALL:
			case APPLY_AUTO_PICKUP:
			case APPLY_MOB_HIT_MOB_AGGRESSIVE:
				TryProcBonus(ch, pkRelic, rRow.wApplyIndex, rRow.dwCooldownSec);
				break;
			default:
				break;
		}
	}
}

void CPassiveAttrManager::OnDeath(LPCHARACTER ch, LPCHARACTER pkKiller)
{
	LPITEM pkRelic = GetEquippedRelic(ch);
	if (!pkRelic || !pkKiller)
		return;

	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
	{
		const TPlayerItemAttribute& rAttr = pkRelic->GetAttribute(i);
		if (rAttr.wType != APPLY_NO_DEATH_AND_HP_RECOVERY30)
			continue;

		if (number(1, 100) > rAttr.lValue)
			continue;

		const TPassiveAttrTable* pRow = nullptr;
		for (size_t j = 0; j < m_vecTable.size(); ++j)
		{
			if (m_vecTable[j].wApplyIndex == APPLY_NO_DEATH_AND_HP_RECOVERY30)
			{
				pRow = &m_vecTable[j];
				break;
			}
		}
		if (pRow && pRow->dwCooldownSec > 0)
		{
			if (ch->GetQuestFlag("passive_attr.proc.nodeath") > get_global_time())
				break;
			ch->SetQuestFlag("passive_attr.proc.nodeath", get_global_time() + static_cast<int>(pRow->dwCooldownSec));
		}

		ch->SetHP(ch->GetMaxHP() * 30 / 100);
		ch->SetPosition(POS_STANDING);
		ch->ReviveInvisible(3);
		break;
	}
}

void CPassiveAttrManager::OnCharacterItemLoad(LPCHARACTER ch)
{
	if (!ch)
		return;

	LPITEM pkRelic = ch->GetWear(WEAR_PASSIVE);
	if (!pkRelic)
		return;

	InitializeDeckState(ch, pkRelic);

	if (ch->GetQuestFlag(PASSIVE_ATTR_DECK_INIT_FLAG) != 0
		&& ch->GetQuestFlag(BuildDeckHasRelicFlag(0).c_str()) == 0
		&& ch->GetQuestFlag(BuildDeckHasRelicFlag(1).c_str()) == 0)
	{
		SetDeckHasRelic(ch, PASSIVE_ATTR_PAGE_UP, true);
	}

	const BYTE bPage = GetSelectedPage(ch);

	if (pkRelic->IsEquipped() && IsRelicActive(pkRelic))
		pkRelic->ModifyPoints(false);

	LoadDeckState(ch, pkRelic, bPage);

	if (!HasDeckRelic(ch, bPage) || !IsRelicActive(pkRelic))
		return;

	if (pkRelic->IsEquipped())
		pkRelic->ModifyPoints(true);

	SetFirstAttackPending(ch, true);

	if (!ch->FindAffect(AFFECT_PASSIVE_JOB_DECK))
		ch->AddAffect(AFFECT_PASSIVE_JOB_DECK, POINT_NONE, bPage, AFF_NONE, INFINITE_AFFECT_DURATION, 0, true);
}

void CPassiveAttrManager::OnPassiveRelicEquipped(LPCHARACTER ch, LPITEM pkRelic)
{
	if (!ch || !pkRelic)
		return;

	InitializeDeckState(ch, pkRelic);

	const BYTE bPage = GetSelectedPage(ch);
	SetDeckHasRelic(ch, bPage, true);
	SaveDeckState(ch, pkRelic, bPage);
	RefreshRelicState(ch, pkRelic);

	SendGC(ch, SUBHEADER_GC_PASSIVE_ATTR_USE_ITEM_JOB, bPage, PASSIVE_ATTR_GC_OK, false,
		pkRelic->GetSocket(PASSIVE_RELIC_SOCKET_REMAIN));
}

#endif

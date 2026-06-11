// Before
	if (!GetVnum())
		return;

	packet->header = HEADER_GC_ITEM_USE;
	packet->ch_vid = ch->GetVID();
	packet->victim_vid = victim->GetVID();
	packet->Cell = TItemPos(GetWindow(), m_wCell);
	packet->vnum = GetVnum();
}

void CItem::RemoveFlag(long bit)
{

// After
	if (!GetVnum())
		return;

	packet->header = HEADER_GC_ITEM_USE;
	packet->ch_vid = ch->GetVID();
	packet->victim_vid = victim->GetVID();
	packet->Cell = TItemPos(GetWindow(), m_wCell);
#if defined(__PASSIVE_ATTR__)
	EncodePlayerItemWirePos(packet->Cell);
#endif
	packet->vnum = GetVnum();
}

void CItem::RemoveFlag(long bit)
{

// Before
	if (!m_pOwner || !m_pOwner->GetDesc())
		return;

	TPacketGCItemUpdate pack;

	pack.bHeader = HEADER_GC_ITEM_UPDATE;
	pack.Cell = TItemPos(GetWindow(), m_wCell);
	pack.dwCount = m_dwCount;
#if defined(__SOUL_BIND_SYSTEM__)
	pack.lSealDate = m_lSealDate;
#endif
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		pack.alSockets[i] = m_alSockets[i];
	thecore_memcpy(pack.aAttr, GetAttributes(), sizeof(pack.aAttr));
#if defined(__CHANGE_LOOK_SYSTEM__)
	pack.dwTransmutationVnum = GetTransmutationVnum();
#endif
#if defined(__REFINE_ELEMENT_SYSTEM__)
	thecore_memcpy(&pack.RefineElement, GetRefineElement(), sizeof(pack.RefineElement));
#endif
#if defined(__ITEM_APPLY_RANDOM__)
	thecore_memcpy(pack.aApplyRandom, GetRandomApplies(), sizeof(pack.aApplyRandom));
#endif
#if defined(__SET_ITEM__)
	pack.bSetValue = GetItemSetValue();
#endif

	sys_log(2, "UpdatePacket %s -> %s", GetName(), m_pOwner->GetName());
	m_pOwner->GetDesc()->Packet(&pack, sizeof(pack));
}

DWORD CItem::GetCount()
{

// After
	if (!m_pOwner || !m_pOwner->GetDesc())
		return;

	TPacketGCItemUpdate pack;

	pack.bHeader = HEADER_GC_ITEM_UPDATE;
	pack.Cell = TItemPos(GetWindow(), m_wCell);
#if defined(__PASSIVE_ATTR__)
	EncodePlayerItemWirePos(pack.Cell);
#endif
	pack.dwCount = m_dwCount;
#if defined(__SOUL_BIND_SYSTEM__)
	pack.lSealDate = m_lSealDate;
#endif
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		pack.alSockets[i] = m_alSockets[i];
	thecore_memcpy(pack.aAttr, GetAttributes(), sizeof(pack.aAttr));
#if defined(__CHANGE_LOOK_SYSTEM__)
	pack.dwTransmutationVnum = GetTransmutationVnum();
#endif
#if defined(__REFINE_ELEMENT_SYSTEM__)
	thecore_memcpy(&pack.RefineElement, GetRefineElement(), sizeof(pack.RefineElement));
#endif
#if defined(__ITEM_APPLY_RANDOM__)
	thecore_memcpy(pack.aApplyRandom, GetRandomApplies(), sizeof(pack.aApplyRandom));
#endif
#if defined(__SET_ITEM__)
	pack.bSetValue = GetItemSetValue();
#endif

	sys_log(2, "UpdatePacket %s -> %s", GetName(), m_pOwner->GetName());
	m_pOwner->GetDesc()->Packet(&pack, sizeof(pack));
}

DWORD CItem::GetCount()
{

// Before
	if ((0 == GetWearFlag() || ITEM_TOTEM == GetType()) && ITEM_COSTUME != GetType()
#if defined(__DRAGON_SOUL_SYSTEM__)
		&& ITEM_DS != GetType() && ITEM_SPECIAL_DS != GetType()
#endif
		&& ITEM_RING != GetType() && ITEM_BELT != GetType()
		)
		return -1;

	/* Dragon Soul Stone slots cannot be processed as WEAR (up to 32 WEARs are possible, but if you add Dragon Soul Stones, it will exceed 32.)
	* From a specific location in the inventory ((INVENTORY_MAX_NUM + WEAR_MAX_NUM) to (INVENTORY_MAX_NUM + WEAR_MAX_NUM + DRAGON_SOUL_DECK_MAX_NUM * DS_SLOT_MAX - 1))
	* Determined by Dragon Soul Stone slot.
	* When returning, the reason for subtracting INVENTORY_MAX_NUM is,
	* Because WearCell originally returns INVENTORY_MAX_NUM.
	*/
#if defined(__DRAGON_SOUL_SYSTEM__)
	if (GetType() == ITEM_DS || GetType() == ITEM_SPECIAL_DS)
	{
		if (iCandidateCell < 0)
		{
			return WEAR_MAX_NUM + GetSubType();
		}

		for (int i = 0; i < DRAGON_SOUL_DECK_MAX_NUM; i++)
		{
			if (WEAR_MAX_NUM + i * DS_SLOT_MAX + GetSubType() == iCandidateCell)
			{
				return iCandidateCell;
			}
		}
		return -1;
	}

// After
	if ((0 == GetWearFlag() || ITEM_TOTEM == GetType()) && ITEM_COSTUME != GetType()
#if defined(__DRAGON_SOUL_SYSTEM__)
		&& ITEM_DS != GetType() && ITEM_SPECIAL_DS != GetType()
#endif
		&& ITEM_RING != GetType() && ITEM_BELT != GetType()
#if defined(__PASSIVE_ATTR__)
		&& ITEM_PASSIVE != GetType()
#endif
		)
		return -1;

	/* Dragon Soul Stone slots cannot be processed as WEAR (up to 32 WEARs are possible, but if you add Dragon Soul Stones, it will exceed 32.)
	* From a specific location in the inventory ((INVENTORY_MAX_NUM + WEAR_MAX_NUM) to (INVENTORY_MAX_NUM + WEAR_MAX_NUM + DRAGON_SOUL_DECK_MAX_NUM * DS_SLOT_MAX - 1))
	* Determined by Dragon Soul Stone slot.
	* When returning, the reason for subtracting INVENTORY_MAX_NUM is,
	* Because WearCell originally returns INVENTORY_MAX_NUM.
	*/
#if defined(__DRAGON_SOUL_SYSTEM__)
	if (GetType() == ITEM_DS || GetType() == ITEM_SPECIAL_DS)
	{
		if (iCandidateCell < 0)
		{
			return WEAR_MAX_NUM + GetSubType();
		}

		for (int i = 0; i < DRAGON_SOUL_DECK_MAX_NUM; i++)
		{
			if (WEAR_MAX_NUM + i * DS_SLOT_MAX + GetSubType() == iCandidateCell)
			{
				return iCandidateCell;
			}
		}
		return -1;
	}

// Before
	else if (GetWearFlag() & WEARABLE_GLOVE)
		return WEAR_GLOVE;
#endif

	return -1;
}

//#define __CHECK_MODIFY_POINTS_PERF__

void CItem::ModifyPoints(bool bAdd)
{

// After
	else if (GetWearFlag() & WEARABLE_GLOVE)
		return WEAR_GLOVE;
#endif
#if defined(__PASSIVE_ATTR__)
	else if (GetType() == ITEM_PASSIVE)
		return WEAR_PASSIVE;
#endif

	return -1;
}

//#define __CHECK_MODIFY_POINTS_PERF__

void CItem::ModifyPoints(bool bAdd)
{

// Before
	if (m_pProto->bType == ITEM_COSTUME && m_pProto->bSubType == COSTUME_AURA)
		m_pOwner->ModifyAuraPoints(this, bAdd);
#endif
#if defined(__CHECK_MODIFY_POINTS_PERF__)
	auto _aura_e1 = std::chrono::steady_clock::now();
	auto _aura_d1 = std::chrono::duration_cast<std::chrono::nanoseconds>(_aura_e1 - _aura_s1).count();
	m_pOwner->ChatPacket(CHAT_TYPE_INFO, "ModifyAuraPoints %lld nanoseconds.", static_cast<long long>(_aura_d1));
#endif


	//if (true == CItemVnumHelper::IsRamadanMoonRing(GetVnum()) || true == CItemVnumHelper::IsHalloweenCandy(GetVnum())
		//|| true == CItemVnumHelper::IsHappinessRing(GetVnum()) || true == CItemVnumHelper::IsLovePendant(GetVnum()))
	if (CItemVnumHelper::IsUniqueItem(GetVnum()))
	{
		// Do not anything.
	}

// After
	if (m_pProto->bType == ITEM_COSTUME && m_pProto->bSubType == COSTUME_AURA)
		m_pOwner->ModifyAuraPoints(this, bAdd);
#endif
#if defined(__CHECK_MODIFY_POINTS_PERF__)
	auto _aura_e1 = std::chrono::steady_clock::now();
	auto _aura_d1 = std::chrono::duration_cast<std::chrono::nanoseconds>(_aura_e1 - _aura_s1).count();
	m_pOwner->ChatPacket(CHAT_TYPE_INFO, "ModifyAuraPoints %lld nanoseconds.", static_cast<long long>(_aura_d1));
#endif

#if defined(__PASSIVE_ATTR__)
	if (CItemVnumHelper::IsPassive(GetVnum()) && GetSubType() == PASSIVE_JOB && GetSocket(1) == 0)
		return;
#endif

	//if (true == CItemVnumHelper::IsRamadanMoonRing(GetVnum()) || true == CItemVnumHelper::IsHalloweenCandy(GetVnum())
		//|| true == CItemVnumHelper::IsHappinessRing(GetVnum()) || true == CItemVnumHelper::IsLovePendant(GetVnum()))
	if (CItemVnumHelper::IsUniqueItem(GetVnum()))
	{
		// Do not anything.
	}

// In `bool CItem::IsEquipable()`, extend the switch statement with:
#if defined(__PASSIVE_ATTR__)
		case ITEM_PASSIVE:
#endif

// Before
	if (IsRideItem())
		ClearMountAttributeAndAffect();


#if defined(__DRAGON_SOUL_SYSTEM__)
	if (IsDragonSoul())
	{
		DSManager::instance().DeactivateDragonSoul(this);
#if defined(__DS_SET__)
		m_pOwner->DragonSoul_SetBonus();
#endif
	}

// After
	if (IsRideItem())
		ClearMountAttributeAndAffect();

#if defined(__PASSIVE_ATTR__)
	if (CItemVnumHelper::IsPassive(GetVnum()) && GetSubType() == PASSIVE_JOB && m_pOwner)
		m_pOwner->RemoveAffect(AFFECT_PASSIVE_JOB_DECK);
#endif

#if defined(__DRAGON_SOUL_SYSTEM__)
	if (IsDragonSoul())
	{
		DSManager::instance().DeactivateDragonSoul(this);
#if defined(__DS_SET__)
		m_pOwner->DragonSoul_SetBonus();
#endif
	}

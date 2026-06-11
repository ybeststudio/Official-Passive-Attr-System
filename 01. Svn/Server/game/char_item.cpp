// Add to includes:
#if defined(__PASSIVE_ATTR__)
#	include "passive_attr.h"
#endif

// In `LPITEM CHARACTER::GetItem(TItemPos Cell)`, extend the switch statement with:
#if defined(__PASSIVE_ATTR__)
		case WEAR_PASSIVE_ATTR:
		{
			if (wCell == WEAR_PASSIVE_ATTR_UP)
				return m_pointsInstant.pEquipmentItems[WEAR_PASSIVE];
			return nullptr;
		}
		break;
#endif

// In `void CHARACTER::SetItem(TItemPos Cell, LPITEM pItem , bool isHighLight)`, extend the switch statement with:
#if defined(__PASSIVE_ATTR__)
		case WEAR_PASSIVE_ATTR:
#endif

// Before
		case WEAR_PASSIVE_ATTR:
#endif
		{
			if (wCell >= EQUIPMENT_MAX_NUM)
			{
				sys_err("CHARACTER::SetItem: Invalid Equipment item! Window %d Cell %d", bWindowType, wCell);
				return;
			}

			LPITEM pOldItem = m_pointsInstant.pEquipmentItems[wCell];
			if (pOldItem)
			{
				if (wCell < EQUIPMENT_MAX_NUM)
				{
					for (BYTE bSize = 0; bSize < pOldItem->GetSize(); ++bSize)
					{
						BYTE bSlot = wCell + bSize;
						if (bSlot >= EQUIPMENT_MAX_NUM)
							continue;

						if (m_pointsInstant.pEquipmentItems[bSlot] && m_pointsInstant.pEquipmentItems[bSlot] != pOldItem)
							continue;

						m_pointsInstant.bEquipmentItemGrid[bSlot] = 0;
					}
				}
				else
					m_pointsInstant.bEquipmentItemGrid[wCell] = 0;
			}

			if (pItem)
			{
				if (wCell < EQUIPMENT_MAX_NUM)
				{
					for (BYTE bSize = 0; bSize < pItem->GetSize(); ++bSize)
					{
						BYTE bSlot = wCell + bSize;
						if (bSlot >= EQUIPMENT_MAX_NUM)
							continue;

						m_pointsInstant.bEquipmentItemGrid[bSlot] = wCell + 1;
					}
				}
				else
					m_pointsInstant.bEquipmentItemGrid[wCell] = wCell + 1;
			}

			m_pointsInstant.pEquipmentItems[wCell] = pItem;
		}

// After
		case WEAR_PASSIVE_ATTR:
#endif
		{
#if defined(__PASSIVE_ATTR__)
			if (bWindowType == WEAR_PASSIVE_ATTR)
			{
				if (wCell == WEAR_PASSIVE_ATTR_UP)
					wCell = WEAR_PASSIVE;
				else
				{
					sys_err("CHARACTER::SetItem: Invalid WEAR_PASSIVE_ATTR cell %d", wCell);
					return;
				}
			}
#endif
			if (wCell >= EQUIPMENT_MAX_NUM)
			{
				sys_err("CHARACTER::SetItem: Invalid Equipment item! Window %d Cell %d", bWindowType, wCell);
				return;
			}

			LPITEM pOldItem = m_pointsInstant.pEquipmentItems[wCell];
			if (pOldItem)
			{
				if (wCell < EQUIPMENT_MAX_NUM)
				{
					for (BYTE bSize = 0; bSize < pOldItem->GetSize(); ++bSize)
					{
						BYTE bSlot = wCell + bSize;
						if (bSlot >= EQUIPMENT_MAX_NUM)
							continue;

						if (m_pointsInstant.pEquipmentItems[bSlot] && m_pointsInstant.pEquipmentItems[bSlot] != pOldItem)
							continue;

						m_pointsInstant.bEquipmentItemGrid[bSlot] = 0;
					}
				}
				else
					m_pointsInstant.bEquipmentItemGrid[wCell] = 0;
			}

			if (pItem)
			{
				if (wCell < EQUIPMENT_MAX_NUM)
				{
					for (BYTE bSize = 0; bSize < pItem->GetSize(); ++bSize)
					{
						BYTE bSlot = wCell + bSize;
						if (bSlot >= EQUIPMENT_MAX_NUM)
							continue;

						m_pointsInstant.bEquipmentItemGrid[bSlot] = wCell + 1;
					}
				}
				else
					m_pointsInstant.bEquipmentItemGrid[wCell] = wCell + 1;
			}

			m_pointsInstant.pEquipmentItems[wCell] = pItem;
		}

// Before
		if (pItem)
		{
			TPacketGCItemSet pack;
			pack.bHeader = HEADER_GC_ITEM_SET;
			pack.Cell = Cell;
			pack.dwVnum = pItem->GetVnum();
			pack.dwCount = pItem->GetCount();
			pack.dwFlags = pItem->GetFlag();
			pack.dwAntiFlags = pItem->GetAntiFlag();
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
			if (isHighLight)
				pack.bHighLight = true;
			else
				pack.bHighLight = (Cell.window_type == DRAGON_SOUL_INVENTORY);
#else
			pack.bHighLight = (Cell.window_type == DRAGON_SOUL_INVENTORY);
#endif
#if defined(__SOUL_BIND_SYSTEM__)
			pack.lSealDate = pItem->GetSealDate();
#endif
			thecore_memcpy(pack.alSockets, pItem->GetSockets(), sizeof(pack.alSockets));
			thecore_memcpy(pack.aAttr, pItem->GetAttributes(), sizeof(pack.aAttr));
#if defined(__CHANGE_LOOK_SYSTEM__)
			pack.dwTransmutationVnum = pItem->GetTransmutationVnum();
#endif
#if defined(__REFINE_ELEMENT_SYSTEM__)
			thecore_memcpy(&pack.RefineElement, pItem->GetRefineElement(), sizeof(pack.RefineElement));
#endif
#if defined(__ITEM_APPLY_RANDOM__)
			thecore_memcpy(pack.aApplyRandom, pItem->GetRandomApplies(), sizeof(pack.aApplyRandom));
#endif
#if defined(__SET_ITEM__)
			pack.bSetValue = pItem->GetItemSetValue();
#endif

			GetDesc()->Packet(&pack, sizeof(TPacketGCItemSet));
		}

// After
		if (pItem)
		{
			TPacketGCItemSet pack;
			pack.bHeader = HEADER_GC_ITEM_SET;
			pack.Cell = Cell;
#if defined(__PASSIVE_ATTR__)
			EncodePlayerItemWirePos(pack.Cell);
#endif
			pack.dwVnum = pItem->GetVnum();
			pack.dwCount = pItem->GetCount();
			pack.dwFlags = pItem->GetFlag();
			pack.dwAntiFlags = pItem->GetAntiFlag();
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
			if (isHighLight)
				pack.bHighLight = true;
			else
				pack.bHighLight = (Cell.window_type == DRAGON_SOUL_INVENTORY);
#else
			pack.bHighLight = (Cell.window_type == DRAGON_SOUL_INVENTORY);
#endif
#if defined(__SOUL_BIND_SYSTEM__)
			pack.lSealDate = pItem->GetSealDate();
#endif
			thecore_memcpy(pack.alSockets, pItem->GetSockets(), sizeof(pack.alSockets));
			thecore_memcpy(pack.aAttr, pItem->GetAttributes(), sizeof(pack.aAttr));
#if defined(__CHANGE_LOOK_SYSTEM__)
			pack.dwTransmutationVnum = pItem->GetTransmutationVnum();
#endif
#if defined(__REFINE_ELEMENT_SYSTEM__)
			thecore_memcpy(&pack.RefineElement, pItem->GetRefineElement(), sizeof(pack.RefineElement));
#endif
#if defined(__ITEM_APPLY_RANDOM__)
			thecore_memcpy(pack.aApplyRandom, pItem->GetRandomApplies(), sizeof(pack.aApplyRandom));
#endif
#if defined(__SET_ITEM__)
			pack.bSetValue = pItem->GetItemSetValue();
#endif

			GetDesc()->Packet(&pack, sizeof(TPacketGCItemSet));
		}

// Before
		else
		{
			TPacketGCItemSetEmpty pack;
			pack.bHeader = HEADER_GC_ITEM_DEL;
			pack.Cell = Cell;
			pack.dwVnum = 0;
			pack.dwCount = 0;
			memset(pack.alSockets, 0, sizeof(pack.alSockets));
			memset(pack.aAttr, 0, sizeof(pack.aAttr));
#if defined(__CHANGE_LOOK_SYSTEM__)
			pack.dwTransmutationVnum = 0;
#endif
#if defined(__REFINE_ELEMENT_SYSTEM__)
			memset(&pack.RefineElement, 0, sizeof(pack.RefineElement));
#endif
#if defined(__ITEM_APPLY_RANDOM__)
			memset(pack.aApplyRandom, 0, sizeof(pack.aApplyRandom));
#endif
#if defined(__SET_ITEM__)
			pack.bSetValue = 0;
#endif

			GetDesc()->Packet(&pack, sizeof(TPacketGCItemSetEmpty));
		}

// After
		else
		{
			TPacketGCItemSetEmpty pack;
			pack.bHeader = HEADER_GC_ITEM_DEL;
			pack.Cell = Cell;
#if defined(__PASSIVE_ATTR__)
			EncodePlayerItemWirePos(pack.Cell);
#endif
			pack.dwVnum = 0;
			pack.dwCount = 0;
			memset(pack.alSockets, 0, sizeof(pack.alSockets));
			memset(pack.aAttr, 0, sizeof(pack.aAttr));
#if defined(__CHANGE_LOOK_SYSTEM__)
			pack.dwTransmutationVnum = 0;
#endif
#if defined(__REFINE_ELEMENT_SYSTEM__)
			memset(&pack.RefineElement, 0, sizeof(pack.RefineElement));
#endif
#if defined(__ITEM_APPLY_RANDOM__)
			memset(pack.aApplyRandom, 0, sizeof(pack.aApplyRandom));
#endif
#if defined(__SET_ITEM__)
			pack.bSetValue = 0;
#endif

			GetDesc()->Packet(&pack, sizeof(TPacketGCItemSetEmpty));
		}

// In `void CHARACTER::SetItem(TItemPos Cell, LPITEM pItem , bool isHighLight)`, extend the switch statement with:
#if defined(__PASSIVE_ATTR__)
			case WEAR_PASSIVE_ATTR:
				pItem->SetWindow(EQUIPMENT);
				break;
#endif

// In `bool CHARACTER::UseItemEx(LPITEM item, TItemPos DestCell)`, extend the switch statement with:
#if defined(__PASSIVE_ATTR__)
		case ITEM_PASSIVE:
#endif

// In `bool CHARACTER::MoveItem(TItemPos Cell, TItemPos DestCell, WORD count)`, find this block:
{
	LPITEM item = nullptr;

#if defined(__PASSIVE_ATTR__)
	NormalizePlayerItemPos(Cell);
	NormalizePlayerItemPos(DestCell);
#endif

	if (Cell.IsSameItemPosition(DestCell))
		return false;

	if (!IsValidItemPosition(Cell))
		return false;

// Add after it:
#if defined(__PASSIVE_ATTR__)
	NormalizePlayerItemPos(Cell);
	NormalizePlayerItemPos(DestCell);
#endif

// Before
	if (true == item->IsEquipped())
	{
		if (-1 != item->GetProto()->cLimitRealTimeFirstUseIndex)
		{
			if (0 == item->GetSocket(1))
			{
				long duration = (0 != item->GetSocket(0)) ? item->GetSocket(0) : item->GetProto()->aLimits[(BYTE)item->GetProto()->cLimitRealTimeFirstUseIndex].lValue;

				if (0 == duration)
					duration = 60 * 60 * 24 * 7;

				item->SetSocket(0, time(0) + duration);
				item->StartRealTimeExpireEvent();
			}

			item->SetSocket(1, item->GetSocket(1) + 1);
		}

		if (item->GetVnum() == UNIQUE_ITEM_HIDE_ALIGNMENT_TITLE)
			ShowAlignment(false);

		if (item->IsRing() || item->IsCostume()
#if defined(__MOUNT_COSTUME_SYSTEM__)
			&& !item->IsCostumeMount()
#endif
			)
			ChatPacket(CHAT_TYPE_COMMAND, "OpenCostumeWindow");

		const DWORD& dwVnum = item->GetVnum();

		// (71135)
		if (true == CItemVnumHelper::IsRamadanMoonRing(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_RAMADAN_RING);
		}
		// (71136)
		else if (true == CItemVnumHelper::IsHalloweenCandy(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_HALLOWEEN_CANDY);
		}
		// (71143)
		else if (true == CItemVnumHelper::IsHappinessRing(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_HAPPINESS_RING);
		}
		// (71145)
		else if (true == CItemVnumHelper::IsLovePendant(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_LOVE_PENDANT);
		}
		// ITEM_UNIQUE, SpecialItemGroup, (item->GetSIGVnum() != NULL)
		else if ((ITEM_UNIQUE == item->GetType() || ITEM_RING == item->GetType()) && 0 != item->GetSIGVnum())
		{
			const CSpecialItemGroup* pGroup = ITEM_MANAGER::instance().GetSpecialItemGroup(item->GetSIGVnum());
			if (NULL != pGroup)
			{
				const CSpecialAttrGroup* pAttrGroup = ITEM_MANAGER::instance().GetSpecialAttrGroup(pGroup->GetAttrVnum(item->GetVnum()));
				if (NULL != pAttrGroup)
				{
					const std::string& std = pAttrGroup->m_stEffectFileName;
					SpecificEffectPacket(std.c_str());
				}
			}
		}

#if defined(__ACCE_COSTUME_SYSTEM__)
		if ((item->GetType() == ITEM_COSTUME) && (item->GetSubType() == COSTUME_ACCE
#if defined(__AURA_COSTUME_SYSTEM__)
			|| item->GetSubType() == COSTUME_AURA
#endif
			))
		{
			if (item->GetSocket(ITEM_SOCKET_ACCE_DRAIN_VALUE) > 18)
				this->EffectPacket(SE_ACCE_BACK);
			this->EffectPacket(SE_ACCE_EQUIP);
		}
#endif

		if ((ITEM_UNIQUE == item->GetType() && UNIQUE_SPECIAL_RIDE == item->GetSubType() && IS_SET(item->GetFlag(), ITEM_FLAG_QUEST_USE))
#if defined(__MOUNT_COSTUME_SYSTEM__)
			|| (ITEM_COSTUME == item->GetType() && COSTUME_MOUNT == item->GetSubType())
#endif
			)
		{
			quest::CQuestManager::instance().UseItem(GetPlayerID(), item, false);
		}

	}

// After
	if (true == item->IsEquipped())
	{
		if (-1 != item->GetProto()->cLimitRealTimeFirstUseIndex)
		{
			if (0 == item->GetSocket(1))
			{
				long duration = (0 != item->GetSocket(0)) ? item->GetSocket(0) : item->GetProto()->aLimits[(BYTE)item->GetProto()->cLimitRealTimeFirstUseIndex].lValue;

				if (0 == duration)
					duration = 60 * 60 * 24 * 7;

				item->SetSocket(0, time(0) + duration);
				item->StartRealTimeExpireEvent();
			}

			item->SetSocket(1, item->GetSocket(1) + 1);
		}

		if (item->GetVnum() == UNIQUE_ITEM_HIDE_ALIGNMENT_TITLE)
			ShowAlignment(false);

		if (item->IsRing() || item->IsCostume()
#if defined(__MOUNT_COSTUME_SYSTEM__)
			&& !item->IsCostumeMount()
#endif
			)
			ChatPacket(CHAT_TYPE_COMMAND, "OpenCostumeWindow");

		const DWORD& dwVnum = item->GetVnum();

		// (71135)
		if (true == CItemVnumHelper::IsRamadanMoonRing(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_RAMADAN_RING);
		}
		// (71136)
		else if (true == CItemVnumHelper::IsHalloweenCandy(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_HALLOWEEN_CANDY);
		}
		// (71143)
		else if (true == CItemVnumHelper::IsHappinessRing(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_HAPPINESS_RING);
		}
		// (71145)
		else if (true == CItemVnumHelper::IsLovePendant(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_LOVE_PENDANT);
		}
		// ITEM_UNIQUE, SpecialItemGroup, (item->GetSIGVnum() != NULL)
		else if ((ITEM_UNIQUE == item->GetType() || ITEM_RING == item->GetType()) && 0 != item->GetSIGVnum())
		{
			const CSpecialItemGroup* pGroup = ITEM_MANAGER::instance().GetSpecialItemGroup(item->GetSIGVnum());
			if (NULL != pGroup)
			{
				const CSpecialAttrGroup* pAttrGroup = ITEM_MANAGER::instance().GetSpecialAttrGroup(pGroup->GetAttrVnum(item->GetVnum()));
				if (NULL != pAttrGroup)
				{
					const std::string& std = pAttrGroup->m_stEffectFileName;
					SpecificEffectPacket(std.c_str());
				}
			}
		}

#if defined(__ACCE_COSTUME_SYSTEM__)
		if ((item->GetType() == ITEM_COSTUME) && (item->GetSubType() == COSTUME_ACCE
#if defined(__AURA_COSTUME_SYSTEM__)
			|| item->GetSubType() == COSTUME_AURA
#endif
			))
		{
			if (item->GetSocket(ITEM_SOCKET_ACCE_DRAIN_VALUE) > 18)
				this->EffectPacket(SE_ACCE_BACK);
			this->EffectPacket(SE_ACCE_EQUIP);
		}
#endif

		if ((ITEM_UNIQUE == item->GetType() && UNIQUE_SPECIAL_RIDE == item->GetSubType() && IS_SET(item->GetFlag(), ITEM_FLAG_QUEST_USE))
#if defined(__MOUNT_COSTUME_SYSTEM__)
			|| (ITEM_COSTUME == item->GetType() && COSTUME_MOUNT == item->GetSubType())
#endif
			)
		{
			quest::CQuestManager::instance().UseItem(GetPlayerID(), item, false);
		}

#if defined(__PASSIVE_ATTR__)
		if (iWearCell == WEAR_PASSIVE && item->GetType() == ITEM_PASSIVE && item->GetSubType() == PASSIVE_JOB)
			CPassiveAttrManager::instance().OnPassiveRelicEquipped(this, item);
#endif
	}

// In `bool CHARACTER::IsValidItemPosition(TItemPos Pos)`, extend the switch statement with:
#if defined(__PASSIVE_ATTR__)
		case WEAR_PASSIVE_ATTR:
			return cell < WEAR_PASSIVE_ATTR_SLOT_MAX;
#endif

// Add to includes:
#if defined(__PASSIVE_ATTR__)
#	include "passive_attr.h"
#endif

// In `void CInputDB::Boot(const char* data)`, find this block:
data += size * sizeof(TItemAttrTable);

// Add after it:
#if defined(__PASSIVE_ATTR__)
	/*
	* PASSIVE ATTR
	*/
	if (decode_2bytes(data) != sizeof(TPassiveAttrTable))
	{
		sys_err("passive attr table size error");
		thecore_shutdown();
		return;
	}
	data += 2;

	size = decode_2bytes(data);
	data += 2;
	sys_log(0, "BOOT: PASSIVE_ATTR: %d", size);

	if (size)
	{
		TPassiveAttrTable* p = (TPassiveAttrTable*)data;
		CPassiveAttrManager::instance().InitializeTable(p, size);
	}
	data += size * sizeof(TPassiveAttrTable);
#endif

// In `void CInputDB::ItemLoad(LPDESC d, const char* c_pData)`, extend the related check like this:
#if defined(__PASSIVE_ATTR__)
			|| (p->bWindow == WEAR_PASSIVE_ATTR && ch->GetWear(WEAR_PASSIVE))
#endif

// In `void CInputDB::ItemLoad(LPDESC d, const char* c_pData)`, extend the switch statement with:
#if defined(__PASSIVE_ATTR__)
				case WEAR_EQUIPMENT_2:
				case WEAR_COSTUME:
				case WEAR_DRAGONSOUL:
				case WEAR_UNIQUE:
#endif

// In `void CInputDB::ItemLoad(LPDESC d, const char* c_pData)`, extend the switch statement with:
#if defined(__PASSIVE_ATTR__)
				case WEAR_PASSIVE_ATTR:
				{
					if (p->wPos >= WEAR_PASSIVE_ATTR_SLOT_MAX)
					{
						v.emplace_back(item);
						break;
					}

					if (item->CheckItemUseLevel(ch->GetLevel()) == true)
					{
						if (item->EquipTo(ch, WEAR_PASSIVE) == false)
							v.emplace_back(item);
					}
					else
						v.emplace_back(item);
				}
				break;
#endif

// Before
	if (ch->GetWear(WEAR_COSTUME_WEAPON) && !ch->GetWear(WEAR_WEAPON))
		ch->SetPart(PART_WEAPON, 0);
#endif

	ch->CheckMaximumPoints();
	ch->PointsPacket();
	ch->SetItemLoaded();

}

void CInputDB::AffectLoad(LPDESC d, const char* c_pData)
{

// After
	if (ch->GetWear(WEAR_COSTUME_WEAPON) && !ch->GetWear(WEAR_WEAPON))
		ch->SetPart(PART_WEAPON, 0);
#endif

	ch->CheckMaximumPoints();
	ch->PointsPacket();
	ch->SetItemLoaded();

#if defined(__PASSIVE_ATTR__)
	CPassiveAttrManager::instance().OnCharacterItemLoad(ch);
#endif
}

void CInputDB::AffectLoad(LPDESC d, const char* c_pData)
{

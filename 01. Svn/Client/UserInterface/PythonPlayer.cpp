// Before
	if (!Cell.IsValidCell())
		return NULL;

	TItemPos cell = Cell;

	switch (cell.window_type)
	{
		case INVENTORY:
			return &m_playerStatus.aInventoryItem[cell.cell];

		case EQUIPMENT:
			return &m_playerStatus.aEquipmentItem[cell.cell];

#if defined(ENABLE_DRAGON_SOUL_SYSTEM)
		case DRAGON_SOUL_INVENTORY:
			return &m_playerStatus.aDragonSoulInventoryItem[cell.cell];
#endif

		case BELT_INVENTORY:
			return &m_playerStatus.aBeltInventoryItem[cell.cell];

#if defined(ENABLE_ATTR_6TH_7TH)
		case NPC_STORAGE:
			return &m_playerStatus.aAttr67AddItem;
#endif

		default:
			return NULL;
	}

// After
	if (!Cell.IsValidCell())
		return NULL;

	TItemPos cell = Cell;
#if defined(ENABLE_PASSIVE_ATTR)
	NormalizePlayerItemPos(cell);
#endif

	switch (cell.window_type)
	{
		case INVENTORY:
			return &m_playerStatus.aInventoryItem[cell.cell];

		case EQUIPMENT:
			return &m_playerStatus.aEquipmentItem[cell.cell];

#if defined(ENABLE_DRAGON_SOUL_SYSTEM)
		case DRAGON_SOUL_INVENTORY:
			return &m_playerStatus.aDragonSoulInventoryItem[cell.cell];
#endif

		case BELT_INVENTORY:
			return &m_playerStatus.aBeltInventoryItem[cell.cell];

#if defined(ENABLE_ATTR_6TH_7TH)
		case NPC_STORAGE:
			return &m_playerStatus.aAttr67AddItem;
#endif

		default:
			return NULL;
	}

// Before
	if (!Cell.IsValidCell())
		return;

	TItemPos cell = Cell;

	if (c_rkItemInst.dwVnum != 0)
	{
		CItemData* pItemData;
		if (!CItemManager::Instance().GetItemDataPointer(c_rkItemInst.dwVnum, &pItemData))
		{
			TraceError("CPythonPlayer::SetItemData(window_type : %d, dwSlotIndex=%d, itemIndex=%d) - Failed to item data\n", cell.window_type, cell.cell, c_rkItemInst.dwVnum);
			return;
		}
	}

// After
	if (!Cell.IsValidCell())
		return;

	TItemPos cell = Cell;
#if defined(ENABLE_PASSIVE_ATTR)
	NormalizePlayerItemPos(cell);
#endif

	if (c_rkItemInst.dwVnum != 0)
	{
		CItemData* pItemData;
		if (!CItemManager::Instance().GetItemDataPointer(c_rkItemInst.dwVnum, &pItemData))
		{
			TraceError("CPythonPlayer::SetItemData(window_type : %d, dwSlotIndex=%d, itemIndex=%d) - Failed to item data\n", cell.window_type, cell.cell, c_rkItemInst.dwVnum);
			return;
		}
	}

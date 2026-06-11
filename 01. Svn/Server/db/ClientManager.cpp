// Find this line:
sizeof(WORD) + sizeof(WORD) + sizeof(TItemAttrTable) * m_vec_itemRareTable.size() +

// Add after it:
#if defined(__PASSIVE_ATTR__)
		sizeof(WORD) + sizeof(WORD) + sizeof(TPassiveAttrTable) * m_vec_passiveAttrTable.size() +
#endif

// Find this line:
peer->Encode(&m_vec_itemRareTable[0], sizeof(TItemAttrTable) * m_vec_itemRareTable.size());

// Add after it:
#if defined(__PASSIVE_ATTR__)
	peer->EncodeWORD(sizeof(TPassiveAttrTable));
	peer->EncodeWORD(static_cast<WORD>(m_vec_passiveAttrTable.size()));
	if (!m_vec_passiveAttrTable.empty())
		peer->Encode(&m_vec_passiveAttrTable[0], sizeof(TPassiveAttrTable) * m_vec_passiveAttrTable.size());
#endif

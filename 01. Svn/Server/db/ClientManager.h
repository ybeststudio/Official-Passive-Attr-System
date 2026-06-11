// Find this line:
bool InitializeItemRareTable();

// Add after it:
#if defined(__PASSIVE_ATTR__)
	bool InitializePassiveAttrTable();
#endif

// Find this line:
std::vector<TItemAttrTable> m_vec_itemAttrTable;

// Add after it:
#if defined(__PASSIVE_ATTR__)
	std::vector<TPassiveAttrTable> m_vec_passiveAttrTable;
#endif

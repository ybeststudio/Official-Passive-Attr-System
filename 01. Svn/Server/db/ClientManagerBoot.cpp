// In `bool CClientManager::InitializeTables()`, find this block:
if (!InitializeItemRareTable())
	{
		sys_err("InitializeItemRareTable FAILED");
		return false;
	}

// Add after it:
#if defined(__PASSIVE_ATTR__)
	if (!InitializePassiveAttrTable())
	{
		sys_err("InitializePassiveAttrTable FAILED");
		return false;
	}
#endif

// Add the following `CClientManager::InitializePassiveAttrTable` function anywhere in this file:
#if defined(__PASSIVE_ATTR__)
bool CClientManager::InitializePassiveAttrTable()
{
	char query[4096];
	snprintf(query, sizeof(query),
		"SELECT `apply`, `apply`+0, `prob`, `cooldown_sec`, "
		"`lv1`, `lv2`, `lv3`, `lv4`, `lv5`, "
#if defined(__ATTR_6TH_7TH__)
		"`lv6`, `lv7`, `lv8`, `lv9`, `lv10` "
#else
		"`lv5` "
#endif
		"FROM passive_attr%s ORDER BY `apply`",
		GetTablePostfix()
	);

	std::unique_ptr<SQLMsg> pkMsg(CDBManager::instance().DirectQuery(query));
	SQLResult* pRes = pkMsg->Get();

	if (!pRes->uiNumRows)
	{
		sys_err("no result from passive_attr");
		return false;
	}

	if (!m_vec_passiveAttrTable.empty())
	{
		sys_log(0, "RELOAD: passive_attr");
		m_vec_passiveAttrTable.clear();
	}

	m_vec_passiveAttrTable.reserve(pRes->uiNumRows);

	MYSQL_ROW data;
	while ((data = mysql_fetch_row(pRes->pSQLResult)))
	{
		TPassiveAttrTable t;
		memset(&t, 0, sizeof(TPassiveAttrTable));

		int col = 0;
		strlcpy(t.szApply, data[col++], sizeof(t.szApply));
		++col; // skip MySQL enum ordinal (`apply`+0); not equal to game APPLY_* index

		std::string applyKey = std::string("APPLY_") + t.szApply;
		const int iApplyIndex = get_Item_ApplyType_Value(applyKey);
		if (iApplyIndex < 0)
		{
			sys_err("passive_attr: unknown apply %s", t.szApply);
			continue;
		}
		t.wApplyIndex = static_cast<POINT_TYPE>(iApplyIndex);
		sys_log(0, "PASSIVE_ATTR: %s -> apply index %d", t.szApply, iApplyIndex);

		str_to_number(t.dwProb, data[col++]);
		str_to_number(t.dwCooldownSec, data[col++]);
		str_to_number(t.lValues[0], data[col++]);
		str_to_number(t.lValues[1], data[col++]);
		str_to_number(t.lValues[2], data[col++]);
		str_to_number(t.lValues[3], data[col++]);
		str_to_number(t.lValues[4], data[col++]);
#if defined(__ATTR_6TH_7TH__)
		str_to_number(t.lValues[5], data[col++]);
		str_to_number(t.lValues[6], data[col++]);
		str_to_number(t.lValues[7], data[col++]);
		str_to_number(t.lValues[8], data[col++]);
		str_to_number(t.lValues[9], data[col++]);
#endif

		m_vec_passiveAttrTable.push_back(t);
	}

	sys_log(0, "PASSIVE_ATTR: total %d", m_vec_passiveAttrTable.size());
	return true;
}
#endif

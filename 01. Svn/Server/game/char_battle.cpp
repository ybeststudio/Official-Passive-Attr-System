// Add to includes:
#if defined(__PASSIVE_ATTR__)
#	include "passive_attr.h"
#endif

// Before
	if (!pkKiller && m_dwKillerPID)
		pkKiller = CHARACTER_MANAGER::instance().FindByPID(m_dwKillerPID);


	m_dwKillerPID = 0; // DO NOT DELETE THIS LINE UNLESS YOU ARE 1000000% SURE

#if defined(__SKILL_COOLTIME_UPDATE__)
	if (IsPC())
		ResetSkillCoolTimes();
#endif

	// Remove bad affects on death
	if (IsPC())
		RemoveBadAffect();

	ForgetMyAttacker(false);

	bool isAgreedPVP = false;
	bool isUnderGuildWar = false;
	bool isDuel = false;
	bool isForked = false;

	if (pkKiller && pkKiller->IsPC())
	{
		if (pkKiller->m_pkChrTarget == this)
			pkKiller->SetTarget(NULL);

		if (!IsPC() && pkKiller->GetDungeon())
			pkKiller->GetDungeon()->IncKillCount(pkKiller, this);

		isAgreedPVP = CPVPManager::instance().Dead(this, pkKiller->GetPlayerID());
		isDuel = CArenaManager::instance().OnDead(pkKiller, this);

		if (IsPC())
		{
			CGuild* g1 = GetGuild();
			CGuild* g2 = pkKiller->GetGuild();

			if (g1 && g2)
				if (g1->UnderWar(g2->GetID()))
					isUnderGuildWar = true;

			pkKiller->SetQuestNPCID(GetVID());
			quest::CQuestManager::instance().Kill(pkKiller->GetPlayerID(), quest::QUEST_NO_NPC);
			CGuildManager::instance().Kill(pkKiller, this);

#if defined(ENABLE_MONSTER_CARD)
			if (pkKiller->GetMonsterCardSystem() != nullptr && IsMonstercardTarget())
				pkKiller->GetMonsterCardSystem()->OnKillMonsterCardMob(GetRaceNum());
#endif
		}
	}

// After
	if (!pkKiller && m_dwKillerPID)
		pkKiller = CHARACTER_MANAGER::instance().FindByPID(m_dwKillerPID);

#if defined(__PASSIVE_ATTR__)
	if (pkKiller && pkKiller->IsPC() && !IsPC() && GetMobRank() >= MOB_RANK_BOSS && !IsStone())
		CPassiveAttrManager::instance().OnKillBoss(pkKiller);
#endif

	m_dwKillerPID = 0; // DO NOT DELETE THIS LINE UNLESS YOU ARE 1000000% SURE

#if defined(__SKILL_COOLTIME_UPDATE__)
	if (IsPC())
		ResetSkillCoolTimes();
#endif

	// Remove bad affects on death
	if (IsPC())
		RemoveBadAffect();

	ForgetMyAttacker(false);

	bool isAgreedPVP = false;
	bool isUnderGuildWar = false;
	bool isDuel = false;
	bool isForked = false;

	if (pkKiller && pkKiller->IsPC())
	{
		if (pkKiller->m_pkChrTarget == this)
			pkKiller->SetTarget(NULL);

		if (!IsPC() && pkKiller->GetDungeon())
			pkKiller->GetDungeon()->IncKillCount(pkKiller, this);

		isAgreedPVP = CPVPManager::instance().Dead(this, pkKiller->GetPlayerID());
		isDuel = CArenaManager::instance().OnDead(pkKiller, this);

		if (IsPC())
		{
			CGuild* g1 = GetGuild();
			CGuild* g2 = pkKiller->GetGuild();

			if (g1 && g2)
				if (g1->UnderWar(g2->GetID()))
					isUnderGuildWar = true;

			pkKiller->SetQuestNPCID(GetVID());
			quest::CQuestManager::instance().Kill(pkKiller->GetPlayerID(), quest::QUEST_NO_NPC);
			CGuildManager::instance().Kill(pkKiller, this);

#if defined(ENABLE_MONSTER_CARD)
			if (pkKiller->GetMonsterCardSystem() != nullptr && IsMonstercardTarget())
				pkKiller->GetMonsterCardSystem()->OnKillMonsterCardMob(GetRaceNum());
#endif
		}
	}

// Before
	if (!cannot_dead)
	{
		dam = (GetHP() - dam <= 0) ? GetHP() : dam;


#if defined(__QUEST_EVENT_DAMAGE__)
		if (pAttacker && pAttacker->IsPC())
		{
			pAttacker->SetQuestNPCID(GetVID());
			quest::CQuestManager::instance().Damage(pAttacker->GetPlayerID(), GetRaceNum());
		}
#endif

		PointChange(POINT_HP, -dam, false);

#if defined(__GUILD_DRAGONLAIR_SYSTEM__)
		if (pAttacker && CGuildDragonLairManager::Instance().IsRedDragonLair(pAttacker->GetMapIndex()))
		{
			if (GetHP() == 0 && CGuildDragonLairManager::Instance().IsStone(GetRaceNum()))
			{
				PointChange(POINT_HP, 1, false);
			}
		}
#endif
	}

// After
	if (!cannot_dead)
	{
		dam = (GetHP() - dam <= 0) ? GetHP() : dam;

#if defined(__PASSIVE_ATTR__)
		if (IsPC() && pAttacker && dam > 0 && GetHP() <= dam)
		{
			CPassiveAttrManager::instance().OnDeath(this, pAttacker);
			if (GetHP() > 0)
				return false;
		}
#endif

#if defined(__QUEST_EVENT_DAMAGE__)
		if (pAttacker && pAttacker->IsPC())
		{
			pAttacker->SetQuestNPCID(GetVID());
			quest::CQuestManager::instance().Damage(pAttacker->GetPlayerID(), GetRaceNum());
		}
#endif

		PointChange(POINT_HP, -dam, false);

#if defined(__GUILD_DRAGONLAIR_SYSTEM__)
		if (pAttacker && CGuildDragonLairManager::Instance().IsRedDragonLair(pAttacker->GetMapIndex()))
		{
			if (GetHP() == 0 && CGuildDragonLairManager::Instance().IsStone(GetRaceNum()))
			{
				PointChange(POINT_HP, 1, false);
			}
		}
#endif
	}

// Find this line:
pAttacker->SetAccumulatedDamage(dam);

// Add after it:
#if defined(__PASSIVE_ATTR__)
	if (pAttacker && pAttacker->IsPC() && dam > 0)
	{
		if (IsStone())
			CPassiveAttrManager::instance().OnAttackStone(pAttacker, this);
		CPassiveAttrManager::instance().OnAttack(pAttacker, this);
		if (pAttacker->GetMountVnum())
			CPassiveAttrManager::instance().OnAttackFromMount(pAttacker, this);
	}
#endif

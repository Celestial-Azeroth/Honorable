#include "HonorGuard.h"

#pragma region Config

bool HonorGuard::modEnabled;

bool HonorGuard::honorOnGuardKill = true;

float HonorGuard::honorGainRate = 1.0F;
float HonorGuard::honorEliteMultiplier = 2.0F;

bool HonorGuard::splitHonorParty = false;

void HonorGuard::LoadConfig()
{
	HonorGuard::modEnabled = sConfigMgr->GetOption<bool>("Honorable.Enable", true);
}

#pragma endregion

#pragma region Guard

void HonorGuard::HandleKilledGuard(Player* player, Creature* creature)
{
	if (!HonorGuard::modEnabled || !HonorGuard::honorOnGuardKill)
		return;

	// Is player okay to gain Honor
	if (!player->IsAlive() || player->InArena() || player->HasAura(SPELL_AURA_PLAYER_INACTIVE))
		return;

	// Is creature okay to give Honor
	if (!creature && creature->HasAuraType(SPELL_AURA_NO_PVP_CREDIT))
		return;

	// Ensure Creature is a guard
	if (!creature->ToCreature()->IsGuard())
		return;

	float honorToGain = CalculateHonorGain(player, creature);

	if (honorToGain <= 0)
		return;

	honorToGain = honorToGain * HonorGuard::honorGainRate;

	if (creature->isElite())
		honorToGain = honorToGain * HonorGuard::honorEliteMultiplier;

	if (HonorGuard::splitHonorParty && player->GetGroup())
	{
		int groupSize = GetPlayerGroupSize(player);
		honorToGain /= groupSize;

		player->GetGroup()->DoForAllMembers([&](Player* groupMember)
			{
				RewardPlayer(groupMember, creature, honorToGain);
			});
	}
	else
	{
		RewardPlayer(player, creature, honorToGain);
	}
}

void HonorGuard::RewardPlayer(Player* player, Creature* creature, float honorToGain)
{
	player->UpdateHonorFields();

	// Give and progress the necessary achievements
	HandleHonorAchievements(player, creature);

	// Apply honor mulitplier from aura (if applicable)
	AddPct(honorToGain, player->GetMaxPositiveAuraModifier(SPELL_AURA_MOD_HONOR_GAIN_PCT));

	int32 honor = int32(honorToGain);

	// Add honor points to player
	player->ModifyHonorPoints(honor);
	player->ApplyModUInt32Value(PLAYER_FIELD_TODAY_CONTRIBUTION, honor, true);
}

void HonorGuard::HandleHonorAchievements(Player* player, Creature* creature)
{
	player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL);
	player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS, creature->getClass());
	player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HK_RACE, creature->getRace());
	player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA, player->GetAreaId());
	player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL, 1, 0, creature);
}

float HonorGuard::CalculateHonorGain(Player* player, Creature* creature)
{
	const uint8 playerLevel = player->GetLevel();
	const uint8 guardLevel = creature->GetLevel();

	const uint8 grayLevel = Acore::XP::GetGrayLevel(playerLevel);

	if (guardLevel <= grayLevel)
		return 0;

	float honorFactor = static_cast<float>(guardLevel - grayLevel) / (playerLevel - grayLevel);
	float honorGain = Acore::Honor::hk_honor_at_level_f(playerLevel) * honorFactor;

	return honorGain;
}

int HonorGuard::GetPlayerGroupSize(Player* player)
{
	int groupSize = 1;

	const Group* group = player->GetGroup();
	if (group)
	{
		Group::MemberSlotList const& groupMembers = group->GetMemberSlots();
		groupSize = groupMembers.size();
	}

	return groupSize;
}

#pragma endregion
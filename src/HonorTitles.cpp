#include "HonorTitles.h"

bool HonorTitles::modEnabled;
bool HonorTitles::pvpTitlesEnabled = true;

void HonorTitles::LoadConfig()
{
	HonorTitles::modEnabled = sConfigMgr->GetOption<bool>("Honorable.Enable", true);
	HonorTitles::pvpTitlesEnabled = sConfigMgr->GetOption<bool>("Honorable.PvPTitles", true);
}

void HonorTitles::AwardEarnedTitles(Player* player)
{
	if (!HonorTitles::modEnabled || !HonorTitles::pvpTitlesEnabled)
		return;

	uint8 faction = uint8(player->GetTeamId(true));
	uint32 honorKills = player->GetUInt32Value(PLAYER_FIELD_LIFETIME_HONORABLE_KILLS);

	for (size_t i = 0; i < requiredKills.size(); ++i)
	{
		if (honorKills < requiredKills[i])
			break;

		uint32 title = static_cast<uint32>(TitleData[i][faction]);
		if (!player->HasTitle(title))
		{
			player->SetTitle(sCharTitlesStore.LookupEntry(title));
		}
	}
}
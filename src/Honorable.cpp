#include "ScriptMgr.h"
#include "Player.h"
#include "Pet.h"
#include "Config.h"
#include "Chat.h"
#include "Unit.h"
#include "World.h"

#include "HonorGuard.h"
#include "HonorTitles.h"

bool modEnabled = true;
bool modAnnouncer = true;

class HonorableConfig : public WorldScript
{
public:
	HonorableConfig() : WorldScript("HonorableConfig_World") {}

	void OnBeforeConfigLoad(bool reload) override
	{
		if (!reload)
		{
			SetConfigSettings();
			HonorGuard::LoadConfig();
		}
	}

	void SetConfigSettings()
	{
		modEnabled = sConfigMgr->GetOption<bool>("Honorable.Enable", true);
		modAnnouncer = sConfigMgr->GetOption<bool>("Honorable.AnnounceOnLogin", true);
	}
};


class HonorableAnnouncer : public PlayerScript
{
public:
	HonorableAnnouncer() : PlayerScript("Honorable_Player") {}

	void OnPlayerLogin(Player* player) override
	{
		if (!modEnabled || !modAnnouncer)
			return;

		ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00Honorable|r module.");

		HonorTitles::AwardEarnedTitles(player);
	}
};

class HonorableKill : public PlayerScript
{
public:
	HonorableKill() : PlayerScript("Honorable_Player") {}

	void OnPlayerCreatureKill(Player* player, Creature* creature) override
	{
		HonorGuard::HandleKilledGuard(player, creature);
		HonorTitles::AwardEarnedTitles(player);
	}

	void OnPlayerCreatureKilledByPet(Player* player, Creature* creature) override
	{
		HonorGuard::HandleKilledGuard(player, creature);
		HonorTitles::AwardEarnedTitles(player);
	}

	void OnPlayerPVPKill(Player* attacker, Player* killed) override
	{
		if (attacker->GetGUID() == killed->GetGUID())
			return;

		HonorTitles::AwardEarnedTitles(attacker);
	}

};

void AddHonorableScripts()
{
	new HonorableConfig();
	new HonorableAnnouncer();
	new HonorableKill();
}
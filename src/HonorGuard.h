#include "ScriptMgr.h"
#include "Player.h"
#include "Pet.h"
#include "Config.h"
#include "Formulas.h"
#include "Chat.h"
#include "Group.h"
#include "Unit.h"
#include "World.h"

class HonorGuard
{
	static bool modEnabled;

	static bool honorOnGuardKill;

	static float honorGainRate;
	static float honorEliteMultiplier;

	static bool splitHonorParty;

public:
	static void LoadConfig();

	static void HandleKilledGuard(Player* player, Creature* creature);

	static void RewardPlayer(Player* player, Creature* creature, float honorToGain);
	static void HandleHonorAchievements(Player* player, Creature* creature);

	static float CalculateHonorGain(Player* player, Creature* creature);
	static int GetPlayerGroupSize(Player* player);
};
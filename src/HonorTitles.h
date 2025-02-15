#include "ScriptMgr.h"
#include "Config.h"
#include "Player.h"
#include "Chat.h"

#include <array>
#include <iterator>

// Title IDs: https://wowpedia.fandom.com/wiki/TitleId
enum class Titles
{
	// Alliance
	Private = 1,
	Corporal,
	Sergeant_A,
	MasterSergeant,
	SergeantMajor,
	Knight,
	KnightLieutenant,
	KnightCaptain,
	KnightChampion,
	LieutenantCommander,
	Commander,
	Marshal,
	FieldMarshal,
	GrandMarshal,

	// Horde
	Scout,
	Grunt,
	Sergeant_H,
	SeniorSergeant,
	FirstSergeant,
	StoneGuard,
	BloodGuard,
	Legionnaire,
	Centurion,
	Champion,
	LieutenantGeneral,
	General,
	Warlord,
	HighWarlord
};

class HonorTitles
{
	static bool modEnabled;
	static bool pvpTitlesEnabled;

	static constexpr std::array<uint32, 14> requiredKills = 
	{
		 25, 100, 250, 500, 1000, 2000, 4000, 6000, 8000, 10000, 12500, 15000, 20000, 25000
	};

	static constexpr std::array<std::array<Titles, 2>, 14> TitleData = 
	{ 
		{
			{ Titles::Private, Titles::Scout },
			{ Titles::Corporal, Titles::Grunt },
			{ Titles::Sergeant_A, Titles::Sergeant_H },
			{ Titles::MasterSergeant, Titles::SeniorSergeant },
			{ Titles::SergeantMajor, Titles::FirstSergeant },
			{ Titles::Knight, Titles::StoneGuard },
			{ Titles::KnightLieutenant, Titles::BloodGuard },
			{ Titles::KnightCaptain, Titles::Legionnaire },
			{ Titles::KnightChampion, Titles::Centurion },
			{ Titles::LieutenantCommander, Titles::Champion },
			{ Titles::Commander, Titles::LieutenantGeneral },
			{ Titles::Marshal, Titles::General },
			{ Titles::FieldMarshal, Titles::Warlord },
			{ Titles::GrandMarshal, Titles::HighWarlord }
		} 
	};

public:
	static void LoadConfig();

	static void AwardEarnedTitles(Player* player);
};
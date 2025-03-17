#include <map>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <shlobj.h>
#include <filesystem>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static const char* const VERSION = "1.1.0";
static const std::string GIFT_PREFERENCE_DETECTED_LOCALIZATION_KEY = "mods/ThePerfectGift/gift_preference_detected";
static const std::string GIFT_PREFERENCE_UNLOCKED_LOCALIZATION_KEY = "mods/ThePerfectGift/gift_preference_unlocked";
static const std::string ITEM_PLACEHOLDER_TEXT = "<ITEM>";
static const std::string NPC_PLACEHOLDER_TEXT = "<NPC>";
static const std::string ADELINE = "adeline";
static const std::string BALOR = "balor";
//static const std::string CALDARUS = "caldarus";
static const std::string CELINE = "celine";
static const std::string DARCY = "darcy";
static const std::string DELL = "dell";
static const std::string DOZY = "dozy";
static const std::string EILAND = "eiland";
static const std::string ELSIE = "elsie";
static const std::string ERROL = "errol";
static const std::string HAYDEN = "hayden";
static const std::string HEMLOCK = "hemlock";
static const std::string HENRIETTA = "henrietta";
static const std::string HOLT = "holt";
static const std::string JOSEPHINE = "josephine";
static const std::string JUNIPER = "juniper";
static const std::string LANDEN = "landen";
static const std::string LOUIS = "louis";
static const std::string LUC = "luc";
static const std::string MAPLE = "maple";
static const std::string MARCH = "march";
static const std::string MERRI = "merri";
static const std::string NORA = "nora";
static const std::string OLRIC = "olric";
//static const std::string PRIESTESS = "priestess";
static const std::string REINA = "reina";
static const std::string RYIS = "ryis";
//static const std::string STILLWELL = "stillwell";
//static const std::string TALIFERRO = "taliferro";
static const std::string TERITHIA = "terithia";
static const std::string VALEN = "valen";
static const std::string VERA = "vera";
//static const std::string WHEEDLE = "wheedle";
//static const std::string ZOREL = "zorel";
static const std::multimap<std::string, std::vector<std::string>> GIFT_DIALOG_MAP = {
	// Adeline
	{ "Conversations/Bank/Adeline/Banked Lines/inn_work/inn_work/init", { ADELINE, "wildberry_scone" }},
	{ "Conversations/Bank/Adeline/Banked Lines/loves_paperwork/loves_paperwork/init", { ADELINE, "paper" }},
	{ "Conversations/Bank/Adeline/Banked Lines/wine_cellar/wine_cellar/1", { ADELINE, "red_wine" }},
	{ "Conversations/Bank/Adeline/Banked Lines/wine_cellar/wine_cellar/init", { ADELINE, "coffee", "white_wine" }},
	{ "Conversations/Bank/Adeline/Market Lines/market_darcy_1/market_darcy_1/init", { ADELINE, "coffee" }},
	{ "Conversations/Bank/Adeline/Market Lines/market_darcy_2/market_darcy_2/init", { ADELINE, "coffee" }},
	{ "Conversations/Bank/Adeline/Market Lines/market_darcy_4/market_darcy_4/init", { ADELINE, "spicy_cheddar_biscuit" }},
	{ "Cutscenes/Story Events/adeline_quest_board/adeline_quest_board/15", { ADELINE, "tulip" }},
	{ "Cutscenes/Story Events/adeline_quest_board/adeline_quest_board/17", { ADELINE, "tulip" }},
	// Balor
	{ "Conversations/Bank/Balor/Banked Lines/bath_smelled_nice/bath_smelled_nice/1", { BALOR, "jasmine" }},
	{ "Conversations/Bank/Balor/Museum Lines/alda_gem_bracelet/alda_gem_bracelet/init", { BALOR, "alda_gem_bracelet" }},
	{ "Conversations/Bank/Balor/Museum Lines/family_crest_pendant/family_crest_pendant/init", { BALOR, "family_crest_pendant" }},
	{ "Conversations/Bank/Balor/Museum Lines/perfect_emerald/perfect_emerald/init", { BALOR, "perfect_emerald" }},
	{ "Conversations/Bank/Balor/Museum Lines/perfect_ruby/perfect_ruby/init", { BALOR, "perfect_ruby" }},
	{ "Conversations/Bank/Balor/Museum Lines/perfect_sapphire/perfect_sapphire/init", { BALOR, "perfect_sapphire" }},
	{ "Conversations/Bank/Balor/Museum Lines/rusted_treasure_chest/rusted_treasure_chest/init", { BALOR, "rusted_treasure_chest" }},
	{ "Conversations/Bank/Balor/Museum Lines/sapphire_betta/sapphire_betta/init", { BALOR, "sapphire_betta" }},
	{ "Conversations/Tutorial Dialogue/misc_quest_lines/gossip_for_elsie_juniper/7", { BALOR, "ruby" }},
	// Caldarus
	// Celine
	{ "Conversations/Bank/Celine/Banked Lines/daisies/daisies/init", { CELINE, "daisy" }},
	{ "Conversations/Bank/Celine/Market Lines/market_darcy_3/market_darcy_3/init", { CELINE, "rose_tea" }},
	{ "Conversations/Group Conversations/Celine_Reina/foraging/foraging/1", { CELINE, "frost_lily" }},	
	// Darcy
	{ "Conversations/General Dialogue/birthday_anticipation/darcy_birthday_anticipation/1", { DARCY, "apple" }},
	// Dell
	{ "Conversations/Bank/Dell/Museum Lines/alda_bronze_sword/alda_bronze_sword/init", { DELL, "alda_bronze_sword" }},
	{ "Conversations/Bank/Dell/Museum Lines/aldarian_sword/aldarian_sword/init", { DELL, "aldarian_sword" }},
	{ "Conversations/Bank/Dell/Museum Lines/caldosian_sword/caldosian_sword/init", { DELL, "caldosian_sword" }},
	{ "Conversations/Bank/Dell/Museum Lines/lightning_dragonfly/lightning_dragonfly/init", { DELL, "lightning_dragonfly" }},
	{ "Conversations/General Dialogue/birthday_anticipation/dell_birthday_anticipation_family/init", { DELL, "chocolate" }},
	{ "Conversations/Tutorial Dialogue/misc_quest_lines/gossip_for_elsie_dell/12", { DELL, "chocolate" }},
	// Dozy
	// Eiland
	{ "Conversations/Bank/Eiland/Market Lines/market_darcy_1/market_darcy_1/init", { EILAND, "roasted_rice_tea" }},
	// Elsie
	{ "Conversations/Bank/Elsie/Market Lines/market_darcy_1/market_darcy_1/init", { ELSIE, "jasmine_tea" }},
	{ "Conversations/Bank/Elsie/Market Lines/market_darcy_2/market_darcy_2/init", { ELSIE, "jasmine_tea" }},
	{ "Conversations/Bank/Elsie/Market Lines/market_darcy_3/market_darcy_3/init", { ELSIE, "jasmine_tea" }},
	{ "Conversations/Bank/Elsie/Museum Lines/crystal_rose/crystal_rose/init", { ELSIE, "crystal_rose" }},
	{ "Conversations/Bank/Elsie/Museum Lines/feather_pendant/feather_pendant/init", { ELSIE, "alda_feather_pendant" }},
	// Errol
	{ "Conversations/Bank/Errol/Market Lines/market_darcy_1/market_darcy_1/init", { ERROL, "latte" }},
	{ "Conversations/Bank/Errol/Market Lines/market_darcy_3/market_darcy_3/init", { ERROL, "latte" }},
	{ "Conversations/Bank/Errol/Market Lines/market_darcy_4/market_darcy_4/init", { ERROL, "mocha" }},
	// Hayden
	{ "Conversations/Bank/Hayden/Banked Lines/hot_toddy/hot_toddy/init", { HAYDEN, "cup_of_tea" }},
	{ "Conversations/Bank/Hayden/Market Lines/market_darcy_1/market_darcy_1/init", { HAYDEN, "coffee", "cup_of_tea"}},
	{ "Conversations/Bank/Hayden/Market Lines/market_darcy_2/market_darcy_2/init", { HAYDEN, "coffee" }},
	{ "Conversations/Bank/Hayden/Museum Lines/store_horse/stone_horse/init", { HAYDEN, "stone_horse" }},
	{ "Conversations/Festival Lines/Hayden/shooting_star/shooting_star_romantic_follow_up_A/1", { HAYDEN, "apple_pie" }},
	// Hemlock
	{ "Conversations/Bank/Hemlock/Market Lines/market_darcy_1/market_darcy_1/init", { HEMLOCK, "coffee", "beer"}},
	{ "Conversations/Bank/Hemlock/Market Lines/market_darcy_2/market_darcy_2/init", { HEMLOCK, "coffee" }},
	{ "Conversations/Bank/Hemlock/Market Lines/market_darcy_4/market_darcy_4/init", { HEMLOCK, "coffee" }},
	{ "Conversations/Bank/Josephine/Museum Lines/caldosian_drinking_horn/caldosian_drinking_horn/init", { HEMLOCK, "caldosian_drinking_horn" }},
	// Henrietta
	{ "Conversations/General Dialogue/season_transition/early_summer_henrietta/init", { HENRIETTA, "corn" }},
	{ "Conversations/Threads/Hayden/henrietta_tales/hayden_henrietta_tales_3/2", { HENRIETTA, "corn" }},
	{ "Conversations/fetch_quests_follow_ups/request_for_ultimate_small_animal_feed_follow_up_henrietta/init", { HENRIETTA, "ultimate_small_animal_feed" }},
	// Holt
	{ "Conversations/Bank/Holt/Museum Lines/narrows_moss/narrows_moss/init", { HOLT, "narrows_moss" }},
	{ "Conversations/General Dialogue/birthday_anticipation/holt_birthday_anticipation_kids/1", { HOLT, "hardwood" }},
	// Josephine
	{ "Conversations/Bank/Josephine/Banked Lines/quiche/quiche/init", { JOSEPHINE, "quiche" }},
	{ "Conversations/Bank/Josephine/Market Lines/market_darcy_1/market_darcy_1/init", { JOSEPHINE, "cup_of_tea", "green_tea", "jasmine_tea", "lavender_tea", "roasted_rice_tea", "rose_tea", "tea"}},
	{ "Conversations/Bank/Josephine/Market Lines/market_darcy_2/market_darcy_2/init", { JOSEPHINE, "cup_of_tea", "green_tea", "jasmine_tea", "lavender_tea", "roasted_rice_tea", "rose_tea", "tea" }},
	{ "Conversations/Bank/Josephine/Market Lines/market_darcy_4/market_darcy_4/init", { JOSEPHINE, "cup_of_tea", "green_tea", "jasmine_tea", "lavender_tea", "roasted_rice_tea", "rose_tea", "tea" }},
	{ "Conversations/Bank/Hemlock/Market Lines/market_darcy_2/market_darcy_2/init", { JOSEPHINE, "cup_of_tea", "green_tea", "jasmine_tea", "lavender_tea", "roasted_rice_tea", "rose_tea", "tea" }}, // Indicates Josephine likes tea, but without the highlighting hint.
	// Juniper
	{ "Conversations/Bank/Juniper/Banked Lines/breakfast/breakfast/1", { JUNIPER, "latte" }},
	{ "Conversations/Bank/Juniper/Banked Lines/foraging/foraging_3/1", { JUNIPER, "nettle" }},
	{ "Conversations/Bank/Juniper/Banked Lines/foraging/foraging_3/init", { JUNIPER, "nettle" }},
	{ "Conversations/Bank/Juniper/Banked Lines/middlemist_red/middlemist_red/init", { JUNIPER, "middlemist" }},
	{ "Conversations/Bank/Juniper/Banked Lines/new_potion/new_potion/init", { JUNIPER, "newt" }},
	{ "Conversations/Bank/Juniper/Banked Lines/orb_viewing/orb_viewing_2/1", { JUNIPER, "water_chestnut_fritters" }},
	{ "Conversations/Bank/Juniper/Market Lines/market_darcy_1/market_darcy_1/init", { JUNIPER, "latte" }},
	{ "Conversations/Bank/Juniper/Museum Lines/ancient_royal_scepter/ancient_royal_scepter/init", { JUNIPER, "ancient_royal_scepter" }},
	{ "Conversations/Bank/Valen/Market Lines/market_darcy_4/market_darcy_4/init", { JUNIPER, "latte" }},
	{ "Conversations/Tutorial Dialogue/misc_quest_lines/gossip_for_elsie_balor/2", { JUNIPER, "newt" }},
	{ "Conversations/Tutorial Dialogue/misc_quest_lines/gossip_for_elsie_balor/4", { JUNIPER, "newt" }},
	// Landen
	{ "Conversations/Bank/Landen/Banked Lines/inn_special/inn_special/init", { LANDEN, "vegetable_pot_pie" }},
	{ "Cutscenes/Heart Events/Ryis/ryis_six_hearts/ryis_six_hearts/17", { LANDEN, "coconut_cream_pie" }},
	// Luc
	{ "Conversations/Bank/Luc/Market Lines/market_darcy_1/market_darcy_1/init", { LUC, "hot_cocoa" }},
	{ "Conversations/Bank/Luc/Museum Lines/amber_trapped_insect/amber_trapped_insect/init", { LUC, "amber_trapped_insect" }},
	{ "Conversations/Bank/Luc/Museum Lines/cave_shrimp/cave_shrimp/init", { LUC, "cave_shrimp" }},
	{ "Conversations/Bank/Luc/Museum Lines/copper_nugget_beetle/copper_nugget_beetle/init", { LUC, "copper_beetle" }},
	{ "Conversations/Bank/Luc/Museum Lines/fairy_bee/fairy_bee/init", { LUC, "fairy_bee" }},
	{ "Conversations/Bank/Luc/Museum Lines/puddle_spider/puddle_spider/init", { LUC, "puddle_spider" }},
	{ "Conversations/Bank/Luc/Museum Lines/sea_scarab/sea_scarab/init", { LUC, "sea_scarab" }},
	{ "Conversations/Bank/Luc/Museum Lines/snowball_beetle/snowball_beetle/init", { LUC, "snowball_beetle" }},
	{ "Conversations/Bank/Luc/Museum Lines/strobe_firefly/strobe_firefly/init", { LUC, "strobe_firefly" }},
	{ "Conversations/Bank/Reina/Banked Lines/luc_and_maple_cheese/luc_and_maple_cheese/init", { LUC, "cheese" }},
	// Louis
	// Maple
	{ "Conversations/Bank/Maple/Market Lines/market_darcy_2/market_darcy_2/init", { MAPLE, "hot_cocoa" }},
	{ "Conversations/Bank/Maple/Market Lines/market_darcy_4/market_darcy_4/init", { MAPLE, "hot_cocoa" }},
	{ "Conversations/Bank/Maple/Museum Lines/stone_shell/stone_shell/init", { MAPLE, "stone_shell" }},
	{ "Conversations/Bank/Reina/Banked Lines/luc_and_maple_cheese/luc_and_maple_cheese/init", { MAPLE, "cheese" }},
	{ "Conversations/General Dialogue/birthday_anticipation/maple_self_birthday_anticipation/init", { MAPLE, "lost_crown_of_aldaria" }},
	// March
	{ "Conversations/Bank/March/Banked Lines/beer/beer/init", { MARCH, "beer" }},
	{ "Conversations/Bank/March/Banked Lines/chocolate/chocolate/init", { MARCH, "chocolate" }},
	{ "Conversations/Bank/March/Banked Lines/cold_beer_long_day/cold_beer_long_day/init", { MARCH, "beer" }},
	{ "Conversations/Bank/March/Market Lines/market_darcy_1/market_darcy_1/init", { MARCH, "hot_cocoa" }},
	{ "Conversations/Bank/March/Market Lines/market_darcy_3/market_darcy_3/init", { MARCH, "hot_cocoa" }},
	{ "Conversations/Bank/March/Museum Lines/meteorite/meteorite/init", { MARCH, "meteorite" }},
	{ "Conversations/Bank/March/Museum Lines/perfect_copper_ore/perfect_copper_ore/init", { MARCH, "perfect_copper_ore" }},
	{ "Conversations/Bank/March/Museum Lines/perfect_iron_ore/perfect_iron_ore/init", { MARCH, "perfect_iron_ore" }},
	{ "Conversations/Bank/March/Museum Lines/perfect_silver_ore/perfect_silver_ore/init", { MARCH, "perfect_silver_ore" }},
	{ "Conversations/Festival Lines/March/animal_festival/animal_festival_0/init", { MARCH, "hot_cocoa" } },
	// Merri
	// Nora
	{ "Conversations/Bank/Nora/Market Lines/market_darcy_4/market_darcy_4/init", { NORA, "latte" }},
	{ "Conversations/Bank/Nora/Museum Lines/ancient_gold_coin/ancient_gold_coin/init", { NORA, "ancient_gold_coin" }},
	{ "Conversations/General Dialogue/birthday_anticipation/nora_birthday_anticipation_kids/1", { NORA, "baked_potato" }},
	// Olric
	// Priestess
	// Reina
	{ "Conversations/Bank/Reina/Banked Lines/garlic/garlic/init", { REINA, "garlic" }},
	{ "Conversations/Bank/Reina/Banked Lines/garlic/garlic_2/init", { REINA, "garlic" }},
	{ "Conversations/Bank/Reina/Market Lines/market_darcy_1/market_darcy_1/init", { REINA, "coffee", "iced_coffee"}},
	{ "Conversations/Bank/Reina/Market Lines/market_darcy_4/market_darcy_4/init", { REINA, "coffee" }},
	{ "Cutscenes/Heart Events/Reina/reina_six_hearts/reina_six_hearts/37", { REINA, "grilled_cheese" }},
	{ "Cutscenes/Heart Events/Reina/reina_two_hearts/reina_two_hearts/1", { REINA, "wildberry_pie" }},
	// Ryis
	{ "Conversations/Bank/Ryis/Banked Lines/bath_smells_like_lavender/bath_smells_like_lavender/init", { RYIS, "lilac" }},
	{ "Conversations/Bank/Ryis/Market Lines/market_darcy_1/market_darcy_1/init", { RYIS, "iced_coffee" }},
	{ "Conversations/Bank/Ryis/Market Lines/market_darcy_2/market_darcy_2/init", { RYIS, "iced_coffee" }},
	{ "Conversations/Bank/Ryis/Market Lines/market_darcy_3/market_darcy_3/init", { RYIS, "iced_coffee" }},
	{ "Conversations/Bank/Landen/Banked Lines/veggie_sub/veggie_sub/init", { RYIS, "veggie_sub_sandwich" }},
	{ "Conversations/Bank/Ryis/Banked Lines/shopping_for_landen/shopping_for_landen_2/init", { RYIS, "bread" } },
	// Stillwell
	// Taliferro
	// Terithia
	{ "Conversations/Bank/Valen/Banked Lines/terithia_fish_jerky/terithia_fish_jerky/init", { TERITHIA, "canned_sardines" }},
	{ "Conversations/General Dialogue/birthday_anticipation/terithia_birthday_anticipation/1", { TERITHIA, "dried_squid" }},
	// Valen
	{ "Conversations/Bank/Valen/Banked Lines/rainy_stock_up/rainy_stock_up/init", { VALEN, "honey" }},
	{ "Conversations/Bank/Valen/Banked Lines/winter_honey/winter_honey/init", { VALEN, "honey" }},
	{ "Conversations/Bank/Valen/Market Lines/market_darcy_1/market_darcy_1/init", { VALEN, "coffee" }},
	{ "Conversations/Bank/Valen/Market Lines/market_darcy_2/market_darcy_2/init", { VALEN, "coffee" }},
	{ "Conversations/Bank/Valen/Market Lines/market_darcy_3/market_darcy_3/init", { VALEN, "coffee" }},
	{ "Conversations/Festival Lines/Valen/shooting_star/shooting_star_romantic_follow_up_A/1", { VALEN, "beet_soup" }},
	{ "Conversations/General Dialogue/birthday_anticipation/valen_birthday_anticipation/1", { VALEN, "vegetable_soup" }},
	{ "Conversations/Group Conversations/Adeline_Valen/salmon_benefits/salmon_benefits/init", { VALEN, "pan_fried_salmon" }},
	{ "Conversations/Group Conversations/Celine_Valen/deep_woods_nettle/deep_woods_nettle/1", { VALEN, "nettle" }},
	// Vera
	// Wheedle
	// Zorel
	// MOD: Talkative Celine
	{ "conversations/talkative_celine/frost lily/init", { CELINE, "frost_lily" }},
	{ "conversations/talkative_celine/plum blossom/init", { ADELINE, "plum_blossom" }},
	{ "conversations/talkative_celine/plum blossom/init", { CELINE, "plum_blossom" }},
	{ "conversations/talkative_celine/jasmine/init", { CELINE, "jasmine" }},
	{ "conversations/talkative_celine/jasmine/init", { JOSEPHINE, "jasmine" }},
	{ "conversations/talkative_celine/poinsettia/init", { CELINE, "poinsettia" }},
	{ "conversations/talkative_celine/poinsettia/init", { JUNIPER, "poinsettia" }},
	{ "conversations/talkative_celine/crocus/init", { CELINE, "crocus" }},
	{ "conversations/talkative_celine/snapdragon/init", { CELINE, "snapdragon" }},
	{ "conversations/talkative_celine/middlemist/init", { CELINE, "middlemist" }},
	{ "conversations/talkative_celine/snowdrop anemone/1", { CELINE, "snowdrop_anemone" }},
	{ "conversations/talkative_celine/lilac/init", { CELINE, "lilac" }},
	{ "conversations/talkative_celine/daffodil/init", { CELINE, "daffodil" }},
	{ "conversations/talkative_celine/daffodil/1", { REINA, "daffodil" }},
	{ "conversations/talkative_celine/dandelion/init", { CELINE, "dandelion" }},
	{ "conversations/talkative_celine/dandelion/init", { VALEN, "dandelion" }},
	{ "conversations/talkative_celine/tulip/init", { CELINE, "tulip" }},
	{ "conversations/talkative_celine/spring galette/init", { CELINE, "spring_galette" }},
	{ "conversations/talkative_celine/rose tea/init", { CELINE, "rose_tea" }},
	{ "conversations/talkative_celine/spring salad/init", { CELINE, "spring_salad" }},
	{ "conversations/talkative_celine/hydrangea/init", { CELINE, "hydrangea" }},
	{ "conversations/talkative_celine/rose/init", { CELINE, "rose" }},
	{ "conversations/talkative_celine/catmint/init", { CELINE, "catmint" }},
	{ "conversations/talkative_celine/cosmos/init", { CELINE, "cosmos" }},
	{ "conversations/talkative_celine/cosmos/1", { ELSIE, "cosmos" }},
	{ "conversations/talkative_celine/marigold/init", { CELINE, "marigold" }},
	{ "conversations/talkative_celine/daisy/init", { CELINE, "daisy" }},
	{ "conversations/talkative_celine/daisy/init", { MAPLE, "daisy" }},
	{ "conversations/talkative_celine/sunflower/init", { CELINE, "sunflower" }},
	{ "conversations/talkative_celine/sunflower/init", { NORA, "toasted_sunflower_seeds" }},
	{ "conversations/talkative_celine/iris/init", { CELINE, "iris" }},
	{ "conversations/talkative_celine/iris/init", { ELSIE, "iris" }},
	{ "conversations/talkative_celine/celosia/init", { CELINE, "celosia" }},
	{ "conversations/talkative_celine/celosia/init", { LOUIS, "celosia" }},
	{ "conversations/talkative_celine/chrysanthemum/init", { CELINE, "chrysanthemum" }},
	{ "conversations/talkative_celine/heather/init", { CELINE, "heather" }},
	{ "conversations/talkative_celine/viola/init", { CELINE, "viola" }},
	{ "conversations/talkative_celine/crystal rose/init", { CELINE, "crystal_rose" }},
};

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_items = true;
static bool localize_items = true;
static std::map<std::string, std::vector<std::string>> gifts_to_unlock = {};
static std::map<std::string, INT64> item_name_to_id_map = {};
static std::map<std::string, std::string> internal_item_name_to_localized_item_name_map = {};
static RValue live_item = RValue();
static bool greet_the_townsfolk_quest_complete_or_in_progress = false;
static bool greet_the_vendors_quest_complete_or_in_progress = false;
static bool unlocking_gift_preference = false;
static std::string gift_preference_npc_name = "";
static std::string gift_preference_internal_item_name = "";
static CInstance* obj_ari = nullptr;

bool GameIsPaused()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);
	RValue paused = global_instance->at("__pause_status");
	return paused.m_i64 > 0;
}

bool QuestCompletedOrInProgress(std::string quest_name)
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	RValue quest_log = global_instance->at("__quest_log");

	// Check the active quests.
	RValue active = quest_log.at("active");
	RValue active_inner = active.at("inner");
	RValue active_entry_exists = g_ModuleInterface->CallBuiltin("struct_exists", { active_inner, quest_name });
	if (active_entry_exists.m_Kind == VALUE_BOOL && active_entry_exists.m_Real == 1)
		return true;

	// Check the completed quests.
	RValue completed = quest_log.at("completed");
	RValue completed_inner = completed.at("inner");
	RValue quest_entry_exists = g_ModuleInterface->CallBuiltin("struct_exists", { completed_inner, quest_name });
	if (quest_entry_exists.m_Kind == VALUE_BOOL && quest_entry_exists.m_Real == 1)
		return true;
	
	return false;
}

void DisplayNotification(CInstance* Self, CInstance* Other, std::string localization_key)
{
	CScript* gml_script_create_notification = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_create_notification",
		(PVOID*)&gml_script_create_notification
	);

	RValue result;
	RValue notification = localization_key;
	RValue* notification_ptr = &notification;
	gml_script_create_notification->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &notification_ptr }
	);

	gift_preference_npc_name = "";
	gift_preference_internal_item_name = "";
}

void UnlockGifts(CInstance* npc, std::string npc_name)
{
	RValue me_exists = g_ModuleInterface->CallBuiltin("struct_exists", { npc, "me" });
	if (me_exists.m_Kind == VALUE_BOOL && me_exists.m_Real == 1)
	{
		unlocking_gift_preference = true;
		for (int i = 0; i < gifts_to_unlock[npc_name].size(); i++)
		{
			g_ModuleInterface->CallBuiltin("struct_set", { live_item, "item_id", item_name_to_id_map[gifts_to_unlock[npc_name][i]] });

			CScript* gml_script_receive_gift = nullptr;
			g_ModuleInterface->GetNamedRoutinePointer(
				"gml_Script_receive_gift@gml_Object_par_NPC_Create_0",
				(PVOID*)&gml_script_receive_gift
			);

			RValue result;
			RValue* live_item_ptr = &live_item;

			gml_script_receive_gift->m_Functions->m_ScriptFunction(
				npc,
				obj_ari,
				result,
				1,
				{ &live_item_ptr }
			);

			// Display the notification.
			gift_preference_npc_name = npc_name;
			gift_preference_internal_item_name = gifts_to_unlock[npc_name][i];
			DisplayNotification(npc, npc, GIFT_PREFERENCE_UNLOCKED_LOCALIZATION_KEY);
		}
		unlocking_gift_preference = false;
		gifts_to_unlock[npc_name].clear();
	}
}

RValue GetLocalizedString(CInstance* Self, CInstance* Other, std::string localization_key)
{
	CScript* gml_script_get_localizer = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_get@Localizer@Localizer",
		(PVOID*)&gml_script_get_localizer
	);

	RValue result;
	RValue input = localization_key;
	RValue* input_ptr = &input;
	gml_script_get_localizer->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &input_ptr }
	);

	return result;
}

void ObjectCallback(
	IN FWCodeEvent& CodeEvent
)
{
	auto& [self, other, code, argc, argv] = CodeEvent.Arguments();

	if (!self)
		return;

	if (!self->m_Object)
		return;

	if (strstr(self->m_Object->m_Name, "obj_ari"))
	{
		obj_ari = self;
	}

	if (gifts_to_unlock.size() > 0 && !unlocking_gift_preference && live_item.m_Kind != VALUE_UNDEFINED && !GameIsPaused())
	{
		if (gifts_to_unlock.contains(ADELINE) && strstr(self->m_Object->m_Name, "obj_adeline") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, ADELINE);
		if (gifts_to_unlock.contains(BALOR) && strstr(self->m_Object->m_Name, "obj_balor") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, BALOR);
		//if (gifts_to_unlock.contains(CALDARUS) && strstr(self->m_Object->m_Name, "obj_caldarus"))
		//	UnlockGifts(self, CALDARUS);
		if (gifts_to_unlock.contains(CELINE) && strstr(self->m_Object->m_Name, "obj_celine") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, CELINE);
		if (gifts_to_unlock.contains(DARCY) && strstr(self->m_Object->m_Name, "obj_darcy") && greet_the_vendors_quest_complete_or_in_progress)
			UnlockGifts(self, DARCY);
		if (gifts_to_unlock.contains(DELL) && strstr(self->m_Object->m_Name, "obj_dell") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, DELL);
		if (gifts_to_unlock.contains(DOZY) && strstr(self->m_Object->m_Name, "obj_dozy") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, DOZY);
		if (gifts_to_unlock.contains(EILAND) && strstr(self->m_Object->m_Name, "obj_eiland") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, EILAND);
		if (gifts_to_unlock.contains(ELSIE) && strstr(self->m_Object->m_Name, "obj_elsie") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, ELSIE);
		if (gifts_to_unlock.contains(ERROL) && strstr(self->m_Object->m_Name, "obj_errol") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, ERROL);
		if (gifts_to_unlock.contains(HAYDEN) && strstr(self->m_Object->m_Name, "obj_hayden") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, HAYDEN);
		if (gifts_to_unlock.contains(HEMLOCK) && strstr(self->m_Object->m_Name, "obj_hemlock") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, HEMLOCK);
		if (gifts_to_unlock.contains(HENRIETTA) && strstr(self->m_Object->m_Name, "obj_henrietta") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, HENRIETTA);
		if (gifts_to_unlock.contains(HOLT) && strstr(self->m_Object->m_Name, "obj_holt") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, HOLT);
		if (gifts_to_unlock.contains(JOSEPHINE) && strstr(self->m_Object->m_Name, "obj_josephine") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, JOSEPHINE);
		if (gifts_to_unlock.contains(JUNIPER) && strstr(self->m_Object->m_Name, "obj_juniper") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, JUNIPER);
		if (gifts_to_unlock.contains(LANDEN) && strstr(self->m_Object->m_Name, "obj_landen") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, LANDEN);
		if (gifts_to_unlock.contains(LOUIS) && strstr(self->m_Object->m_Name, "obj_louis") && greet_the_vendors_quest_complete_or_in_progress)
			UnlockGifts(self, LOUIS);
		if (gifts_to_unlock.contains(LUC) && strstr(self->m_Object->m_Name, "obj_luc") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, LUC);
		if (gifts_to_unlock.contains(MAPLE) && strstr(self->m_Object->m_Name, "obj_maple") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, MAPLE);
		if (gifts_to_unlock.contains(MARCH) && strstr(self->m_Object->m_Name, "obj_march") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, MARCH);
		if (gifts_to_unlock.contains(MERRI) && strstr(self->m_Object->m_Name, "obj_merri") && greet_the_vendors_quest_complete_or_in_progress)
			UnlockGifts(self, MERRI);
		if (gifts_to_unlock.contains(NORA) && strstr(self->m_Object->m_Name, "obj_nora") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, NORA);
		if (gifts_to_unlock.contains(OLRIC) && strstr(self->m_Object->m_Name, "obj_olric") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, OLRIC);
		//if (gifts_to_unlock.contains(PRIESTESS) && strstr(self->m_Object->m_Name, "obj_priestess"))
		//	UnlockGifts(self, PRIESTESS);
		if (gifts_to_unlock.contains(REINA) && strstr(self->m_Object->m_Name, "obj_reina") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, REINA);
		if (gifts_to_unlock.contains(RYIS) && strstr(self->m_Object->m_Name, "obj_ryis") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, RYIS);
		//if (gifts_to_unlock.contains(STILLWELL) && strstr(self->m_Object->m_Name, "obj_stillwell"))
		//	UnlockGifts(self, STILLWELL);
		//if (gifts_to_unlock.contains(TALIFERRO) && strstr(self->m_Object->m_Name, "obj_taliferro"))
		//	UnlockGifts(self, TALIFERRO);
		if (gifts_to_unlock.contains(TERITHIA) && strstr(self->m_Object->m_Name, "obj_terithia") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, TERITHIA);
		if (gifts_to_unlock.contains(VALEN) && strstr(self->m_Object->m_Name, "obj_valen") && greet_the_townsfolk_quest_complete_or_in_progress)
			UnlockGifts(self, VALEN);
		if (gifts_to_unlock.contains(VERA) && strstr(self->m_Object->m_Name, "obj_vera") && greet_the_vendors_quest_complete_or_in_progress)
			UnlockGifts(self, VERA);
		//if (gifts_to_unlock.contains(WHEEDLE) && strstr(self->m_Object->m_Name, "obj_wheedle"))
		//	UnlockGifts(self, WHEEDLE);
		//if (gifts_to_unlock.contains(ZOREL) && strstr(self->m_Object->m_Name, "obj_zorel"))
		//	UnlockGifts(self, ZOREL);
	}
}

RValue& GmlScriptWeatherManagerOnRoomEndCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	live_item = RValue();
	greet_the_townsfolk_quest_complete_or_in_progress = QuestCompletedOrInProgress("greet_the_townsfolk");
	greet_the_vendors_quest_complete_or_in_progress = QuestCompletedOrInProgress("greet_the_vendors");

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_on_room_end@WeatherManager@Weather"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptTextboxTranslateCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	// Load localized item names.
	if (localize_items)
	{
		localize_items = false;

		// Load items.
		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		RValue __item_data = global_instance->at("__item_data");

		size_t array_length;
		g_ModuleInterface->GetArraySize(__item_data, array_length);

		for (size_t i = 0; i < array_length; i++)
		{
			RValue* array_element;
			g_ModuleInterface->GetArrayEntry(__item_data, i, array_element);

			RValue name_key = array_element->at("name_key");
			if (name_key.m_Kind != VALUE_NULL && name_key.m_Kind != VALUE_UNDEFINED && name_key.m_Kind != VALUE_UNSET)
			{
				RValue recipe_key = array_element->at("recipe_key");
				RValue localized_name = GetLocalizedString(Self, Other, name_key.AsString().data());
				internal_item_name_to_localized_item_name_map[recipe_key.AsString().data()] = localized_name.AsString().data();
			}
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_get@Localizer@Localizer"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (ArgumentCount == 1 && Arguments[0]->m_Kind == VALUE_STRING)
	{
		std::string localization_key = Arguments[0]->AsString().data();
		if (localization_key.compare(GIFT_PREFERENCE_DETECTED_LOCALIZATION_KEY) == 0 || localization_key.compare(GIFT_PREFERENCE_UNLOCKED_LOCALIZATION_KEY) == 0)
		{
			std::string result_str = Result.AsString().data();

			// Replace the <ITEM> placeholder text.
			size_t item_placeholder_index = result_str.find(ITEM_PLACEHOLDER_TEXT);
			if (item_placeholder_index != std::string::npos) {
				result_str.replace(item_placeholder_index, ITEM_PLACEHOLDER_TEXT.length(), internal_item_name_to_localized_item_name_map[gift_preference_internal_item_name]);
			}

			// Replace the <NPC> placeholder text.
			size_t npc_placeholder_index = result_str.find(NPC_PLACEHOLDER_TEXT);
			if (npc_placeholder_index != std::string::npos) {
				std::string npc_name = gift_preference_npc_name;
				npc_name[0] = std::toupper(npc_name[0]);
				result_str.replace(npc_placeholder_index, NPC_PLACEHOLDER_TEXT.length(), npc_name);
			}

			Result = result_str;
		}
	}
	
	return Result;
}

RValue& GmlScriptAriGiveItemCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	live_item = Arguments[0]->m_Object;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_give_item@Ari@Ari"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptNpcAddHeartPointsCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (unlocking_gift_preference)
		return Result;
	else
	{
		const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_add_heart_points@Npc@Npc"));
		original(
			Self,
			Other,
			Result,
			ArgumentCount,
			Arguments
		);

		return Result;
	}
}

RValue& GmlScriptBarkOnDrawCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (unlocking_gift_preference)
		return Result;
	else
	{
		const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_emit@BarkEmitter@BarkEmitter"));
		original(
			Self,
			Other,
			Result,
			ArgumentCount,
			Arguments
		);

		return Result;
	}
}

RValue& GmlScriptPlayConversationCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (unlocking_gift_preference)
		return Result;
	else
	{
		const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_play_conversation"));
		original(
			Self,
			Other,
			Result,
			ArgumentCount,
			Arguments
		);

		return Result;
	}
}

RValue& GmlScriptSetupMainScreenCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	gifts_to_unlock = {};
	obj_ari = nullptr;
	live_item = RValue();
	gift_preference_npc_name = "";
	gift_preference_internal_item_name = "";
	unlocking_gift_preference = false;
	greet_the_townsfolk_quest_complete_or_in_progress = false;
	greet_the_vendors_quest_complete_or_in_progress = false;

	if (load_items)
	{
		for (int64_t i = 0; i < 2000; i++)
		{
			CScript* gml_script_try_item_id_to_string = nullptr;
			g_ModuleInterface->GetNamedRoutinePointer(
				"gml_Script_try_item_id_to_string",
				(PVOID*)&gml_script_try_item_id_to_string
			);

			RValue* item_id = new RValue(i);
			RValue item_name;
			gml_script_try_item_id_to_string->m_Functions->m_ScriptFunction(
				Self,
				Other,
				item_name,
				1,
				{ &item_id }
			);
			delete item_id;

			if (item_name.m_Kind != VALUE_NULL && item_name.m_Kind != VALUE_UNSET && item_name.m_Kind != VALUE_UNDEFINED)
			{
				const char* item_name_str = item_name.AsString().data();
				if (item_name_to_id_map.count(item_name_str) <= 0)
				{
					item_name_to_id_map[item_name_str] = i;
				}
			}
		}
		if (item_name_to_id_map.size() > 0)
		{
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[ThePerfectGift %s] - Loaded data for %d items!", VERSION, item_name_to_id_map.size());
		}
		else {
			g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to load data for items!", VERSION);
		}

		load_items = false;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_setup_main_screen@TitleMenu@TitleMenu"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptTranslateCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	std::string dialog_string = Arguments[0]->AsString().data();
	if (GIFT_DIALOG_MAP.count(dialog_string) > 0)
	{
		auto range = GIFT_DIALOG_MAP.equal_range(dialog_string);
		for (auto it = range.first; it != range.second; ++it)
		{
			std::vector<std::string> gifts = it->second; // { REINA, "coffee", "iced_coffee"}}
			std::string npc_name = gifts[0];

			if (gifts_to_unlock.count(npc_name) <= 0)
				gifts_to_unlock[npc_name] = {};

			for (int i = 1; i < gifts.size(); i++)
			{
				gifts_to_unlock[npc_name].push_back(gifts[i]);

				// Display the notification.
				gift_preference_npc_name = npc_name;
				gift_preference_internal_item_name = gifts[i];
				DisplayNotification(Self, Other, GIFT_PREFERENCE_DETECTED_LOCALIZATION_KEY);
			}
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_say@TextboxMenu@TextboxMenu"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

void CreateHookGmlScriptAriGiveItem(AurieStatus& status)
{
	CScript* gml_script_ari_give_item = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_give_item@Ari@Ari",
		(PVOID*)&gml_script_ari_give_item
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to get script (gml_Script_give_item@Ari@Ari)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_give_item@Ari@Ari",
		gml_script_ari_give_item->m_Functions->m_ScriptFunction,
		GmlScriptAriGiveItemCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to hook script (gml_Script_give_item@Ari@Ari)!", VERSION);
	}
}

void CreateHookGmlScriptTextboxTranslate(AurieStatus& status)
{
	CScript* gml_script_textbox_translate = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_get@Localizer@Localizer",
		(PVOID*)&gml_script_textbox_translate
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to get script (gml_Script_get@Localizer@Localizer)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_get@Localizer@Localizer",
		gml_script_textbox_translate->m_Functions->m_ScriptFunction,
		GmlScriptTextboxTranslateCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to hook script (gml_Script_get@Localizer@Localizer)!", VERSION);
	}
}

void CreateHookGmlScriptWeatherManagerOnRoomEnd(AurieStatus& status)
{
	CScript* gml_script_weather_manager_on_room_end = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_on_room_end@WeatherManager@Weather",
		(PVOID*)&gml_script_weather_manager_on_room_end
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to get script (gml_Script_on_room_end@WeatherManager@Weather)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_on_room_end@WeatherManager@Weather",
		gml_script_weather_manager_on_room_end->m_Functions->m_ScriptFunction,
		GmlScriptWeatherManagerOnRoomEndCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to hook script (gml_Script_on_room_end@WeatherManager@Weather)!", VERSION);
	}
}

void CreateHookGmlScriptNpcAddHeartPoints(AurieStatus& status)
{
	CScript* gml_script_npc_add_heart_points = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_add_heart_points@Npc@Npc",
		(PVOID*)&gml_script_npc_add_heart_points
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to get script (gml_Script_add_heart_points@Npc@Npc)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_add_heart_points@Npc@Npc",
		gml_script_npc_add_heart_points->m_Functions->m_ScriptFunction,
		GmlScriptNpcAddHeartPointsCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to hook script (gml_Script_add_heart_points@Npc@Npc)!", VERSION);
	}
}

void CreateHookGmlScriptBarkOnDraw(AurieStatus& status)
{
	CScript* gml_script_bark_on_draw = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_emit@BarkEmitter@BarkEmitter",
		(PVOID*)&gml_script_bark_on_draw
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to get script (gml_Script_emit@BarkEmitter@BarkEmitter)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_emit@BarkEmitter@BarkEmitter",
		gml_script_bark_on_draw->m_Functions->m_ScriptFunction,
		GmlScriptBarkOnDrawCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to hook script (gml_Script_emit@BarkEmitter@BarkEmitter)!", VERSION);
	}
}

void CreateHookGmlScriptPlayConversation(AurieStatus& status)
{
	CScript* gml_script_play_conversation = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_play_conversation",
		(PVOID*)&gml_script_play_conversation
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to get script (gml_Script_play_conversation)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_play_conversation",
		gml_script_play_conversation->m_Functions->m_ScriptFunction,
		GmlScriptPlayConversationCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to hook script (gml_Script_play_conversation)!", VERSION);
	}
}

void CreateHookGmlScriptSetupMainScreen(AurieStatus& status)
{
	CScript* gml_script_setup_main_screen = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_setup_main_screen@TitleMenu@TitleMenu",
		(PVOID*)&gml_script_setup_main_screen
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to get script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_setup_main_screen@TitleMenu@TitleMenu",
		gml_script_setup_main_screen->m_Functions->m_ScriptFunction,
		GmlScriptSetupMainScreenCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to hook script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
	}
}

void CreateHookGmlScriptTranslate(AurieStatus& status)
{
	CScript* gml_script_translate = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_say@TextboxMenu@TextboxMenu",
		(PVOID*)&gml_script_translate
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to get script (gml_Script_say@TextboxMenu@TextboxMenu)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_say@TextboxMenu@TextboxMenu",
		gml_script_translate->m_Functions->m_ScriptFunction,
		GmlScriptTranslateCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to hook script (gml_Script_say@TextboxMenu@TextboxMenu)!", VERSION);
	}
}

void CreateHookEventObject(AurieStatus& status)
{
	status = g_ModuleInterface->CreateCallback(
		g_ArSelfModule,
		EVENT_OBJECT_CALL,
		ObjectCallback,
		0
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to hook EVENT_OBJECT_CALL!", VERSION);
	}
}

EXPORTED AurieStatus ModuleInitialize(IN AurieModule* Module, IN const fs::path& ModulePath) {
	UNREFERENCED_PARAMETER(ModulePath);

	AurieStatus status = AURIE_SUCCESS;
	
	status = ObGetInterface(
		"YYTK_Main", 
		(AurieInterfaceBase*&)(g_ModuleInterface)
	);

	if (!AurieSuccess(status))
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[ThePerfectGift %s] - Plugin starting...", VERSION);

	CreateHookEventObject(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptTranslate(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptPlayConversation(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptBarkOnDraw(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptNpcAddHeartPoints(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptAriGiveItem(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptTextboxTranslate(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptWeatherManagerOnRoomEnd(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[ThePerfectGift %s] - Plugin started!", VERSION);
	return AURIE_SUCCESS;
}
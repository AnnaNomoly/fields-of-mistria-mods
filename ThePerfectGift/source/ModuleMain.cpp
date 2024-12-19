#include <map>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <shlobj.h>
#include <filesystem>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static const char* const VERSION = "1.0.0";
static const std::string ADELINE = "adeline";
static const std::string BALOR = "balor";
static const std::string CELINE = "celine";
static const std::string DARCY = "darcy";
static const std::string DELL = "dell";
static const std::string ERROL = "errol";
static const std::string HAYDEN = "hayden";
static const std::string HEMLOCK = "hemlock";
static const std::string HENRIETTA = "henrietta";
static const std::string HOLT = "holt";
static const std::string JOSEPHINE = "josephine";
static const std::string JUNIPER = "juniper";
static const std::string LANDEN = "landen";
static const std::string LUC = "luc";
static const std::string MAPLE = "maple";
static const std::string MARCH = "march";
static const std::string NORA = "nora";
static const std::string REINA = "reina";
static const std::string RYIS = "ryis";
static const std::string VALEN = "valen";
static const std::string TERITHIA = "terithia";
static const std::map<std::string, std::vector<std::string>> GIFT_DIALOG_MAP = {
	// Adeline
	{ "Conversations/Bank/Adeline/Banked Lines/inn_work/inn_work/init", { ADELINE, "wildberry_scone" }},
	{ "Conversations/Bank/Adeline/Banked Lines/loves_paperwork/loves_paperwork/init", { ADELINE, "paper" }},
	{ "Conversations/Bank/Adeline/Banked Lines/wine_cellar/wine_cellar/1", { ADELINE, "red_wine" }},
	{ "Conversations/Bank/Adeline/Banked Lines/wine_cellar/wine_cellar/init", { ADELINE, "coffee" }},
	{ "Conversations/Bank/Adeline/Market Lines/market_darcy_1/market_darcy_1/init", { ADELINE, "coffee" }},
	{ "Conversations/Bank/Adeline/Market Lines/market_darcy_1/market_darcy_2/init", { ADELINE, "coffee" }},
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
	{ "Conversations/Bank/Josephine/Museum Lines/caldosian_drinking_horn/caldosian_drinking_horn/1", { HEMLOCK, "caldosian_drinking_horn" }},
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
	// Nora
	{ "Conversations/Bank/Nora/Market Lines/market_darcy_4/market_darcy_4/init", { NORA, "latte" }},
	{ "Conversations/Bank/Nora/Museum Lines/ancient_gold_coin/ancient_gold_coin/init", { NORA, "ancient_gold_coin" }},
	{ "Conversations/General Dialogue/birthday_anticipation/nora_birthday_anticipation_kids/1", { NORA, "baked_potato" }},
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
	// Terithia
	{ "Conversations/Bank/Valen/Banked Lines/terithia_fish_jerky/terithia_fish_jerky/init", { TERITHIA, "canned_sardines" }},
	{ "Conversations/General Dialogue/birthday_anticipation/terithia_birthday_anticipation/1", { TERITHIA, "dried_squid" }},
};

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_items = true;
static boolean unlock_gift = false; // Used to indicate when a gift preference has been mentioned in dialog
static std::string gift_npc = "";
static std::vector<std::string> gift_items = {};
static std::map<std::string, int> item_name_to_id_map = {};

void UnlockGifts(CInstance* self)
{
	RValue me_exists = g_ModuleInterface->CallBuiltin(
		"struct_exists", {
			self, "me"
		}
	);

	if (me_exists.m_Kind == VALUE_BOOL && me_exists.m_Real == 1)
	{
		RValue me = self->at("me");
		RValue gifts_given = me.at("gifts_given");
		RValue inner = gifts_given.at("inner");

		for (int i = 0; i < gift_items.size(); i++)
		{
			RValue gift_preference = g_ModuleInterface->CallBuiltin(
				"struct_get", {
					inner, std::to_string(item_name_to_id_map[gift_items[i]])
				}
			);

			if (gift_preference.m_Kind == VALUE_UNDEFINED)
			{
				RValue zero = 0.0;
				g_ModuleInterface->CallBuiltin(
					"struct_set", {
						inner, std::to_string(item_name_to_id_map[gift_items[i]]), zero
					}
				);
				g_ModuleInterface->Print(CM_LIGHTGREEN, "[ThePerfectGift %s] - Learned gift preference for %s: %s", VERSION, gift_npc.c_str(), gift_items[i].c_str());
			}
		}

		unlock_gift = false;
		gift_npc = "";
		gift_items = {};
	}
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

	if (unlock_gift)
	{
		if (gift_npc == ADELINE && strstr(self->m_Object->m_Name, "obj_adeline"))
			UnlockGifts(self);
		if (gift_npc == BALOR && strstr(self->m_Object->m_Name, "obj_balor"))
			UnlockGifts(self);
		if (gift_npc == CELINE && strstr(self->m_Object->m_Name, "obj_celine"))
			UnlockGifts(self);
		if (gift_npc == DARCY && strstr(self->m_Object->m_Name, "obj_darcy"))
			UnlockGifts(self);
		if (gift_npc == DELL && strstr(self->m_Object->m_Name, "obj_dell"))
			UnlockGifts(self);
		if (gift_npc == ERROL && strstr(self->m_Object->m_Name, "obj_errol"))
			UnlockGifts(self);
		if (gift_npc == HAYDEN && strstr(self->m_Object->m_Name, "obj_hayden"))
			UnlockGifts(self);
		if (gift_npc == HEMLOCK && strstr(self->m_Object->m_Name, "obj_hemlock"))
			UnlockGifts(self);
		if (gift_npc == HENRIETTA && strstr(self->m_Object->m_Name, "obj_henrietta"))
			UnlockGifts(self);
		if (gift_npc == HOLT && strstr(self->m_Object->m_Name, "obj_holt"))
			UnlockGifts(self);
		if (gift_npc == JOSEPHINE && strstr(self->m_Object->m_Name, "obj_josephine"))
			UnlockGifts(self);
		if (gift_npc == JUNIPER && strstr(self->m_Object->m_Name, "obj_juniper"))
			UnlockGifts(self);
		if (gift_npc == LANDEN && strstr(self->m_Object->m_Name, "obj_landen"))
			UnlockGifts(self);
		if (gift_npc == LUC && strstr(self->m_Object->m_Name, "obj_luc"))
			UnlockGifts(self);
		if (gift_npc == MAPLE && strstr(self->m_Object->m_Name, "obj_maple"))
			UnlockGifts(self);
		if (gift_npc == MARCH && strstr(self->m_Object->m_Name, "obj_march"))
			UnlockGifts(self);
		if (gift_npc == NORA && strstr(self->m_Object->m_Name, "obj_nora"))
			UnlockGifts(self);
		if (gift_npc == REINA && strstr(self->m_Object->m_Name, "obj_reina"))
			UnlockGifts(self);
		if (gift_npc == RYIS && strstr(self->m_Object->m_Name, "obj_ryis"))
			UnlockGifts(self);
		if (gift_npc == TERITHIA && strstr(self->m_Object->m_Name, "obj_terithia"))
			UnlockGifts(self);
		if (gift_npc == VALEN && strstr(self->m_Object->m_Name, "obj_valen"))
			UnlockGifts(self);
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
		unlock_gift = true;
		std::vector<std::string> values = GIFT_DIALOG_MAP.at(dialog_string);
		gift_npc = values[0];
		for (int i = 1; i < values.size(); i++)
		{
			gift_items.push_back(values[i]);
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

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[ThePerfectGift %s] - Plugin started!", VERSION);
	return AURIE_SUCCESS;
}
#include <random>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <YYToolkit/YYTK_Shared.hpp> // YYTK v4
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

static const char* const MOD_NAME = "DeepDungeon";
static const char* const VERSION = "0.0.1";
static const char* const GML_SCRIPT_GET_LOCALIZER = "gml_Script_get@Localizer@Localizer";
static const char* const GML_SCRIPT_CREATE_NOTIFICATION = "gml_Script_create_notification";
static const char* const GML_SCRIPT_PLAY_CONVERSATION = "gml_Script_play_conversation";
static const char* const GML_SCRIPT_CANCEL_STATUS_EFFECT = "gml_Script_cancel@StatusEffectManager@StatusEffectManager";
static const char* const GML_SCRIPT_REGISTER_STATUS_EFFECT = "gml_Script_register@StatusEffectManager@StatusEffectManager";
static const char* const GML_SCRIPT_GET_MAX_HEALTH = "gml_Script_get_max_health@Ari@Ari";
static const char* const GML_SCRIPT_GET_HEALTH = "gml_Script_get_health@Ari@Ari";
static const char* const GML_SCRIPT_SET_HEALTH = "gml_Script_set_health@Ari@Ari";
static const char* const GML_SCRIPT_MODIFY_HEALTH = "gml_Script_modify_health@Ari@Ari";
static const char* const GML_SCRIPT_MODIFY_STAMINA = "gml_Script_modify_stamina@Ari@Ari";
static const char* const GML_SCRIPT_CAN_CAST_SPELL = "gml_Script_can_cast_spell";
static const char* const GML_SCRIPT_GET_MOVE_SPEED = "gml_Script_get_move_speed@Ari@Ari";
static const char* const GML_SCRIPT_DAMAGE = "gml_Script_damage@gml_Object_obj_damage_receiver_Create_0";
static const char* const GML_SCRIPT_ARI_SHOULD_DIE = "gml_Script_should_die@gml_Object_obj_ari_Create_0";
static const char* const GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE = "gml_Script_deserialize@StatusEffectManager@StatusEffectManager";
static const char* const GML_SCRIPT_USE_ITEM = "gml_Script_use_item";
static const char* const GML_SCRIPT_HELD_ITEM = "gml_Script_held_item@Ari@Ari";
static const char* const GML_SCRIPT_GET_MINUTES = "gml_Script_get_minutes";
static const char* const GML_SCRIPT_PLAY_TEXT = "gml_Script_play_text@TextboxMenu@TextboxMenu";
static const char* const GML_SCRIPT_GET_WEATHER = "gml_Script_get_weather@WeatherManager@Weather";
static const char* const GML_SCRIPT_TRY_LOCATION_ID_TO_STRING = "gml_Script_try_location_id_to_string";
static const char* const GML_SCRIPT_ON_DUNGEON_ROOM_START = "gml_Script_on_room_start@DungeonRunner@DungeonRunner";
static const char* const GML_SCRIPT_GO_TO_ROOM = "gml_Script_goto_gm_room";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const char* const GML_SCRIPT_ON_DRAW_GUI = "gml_Script_on_draw_gui@Display@Display";
static const std::string ITEM_PENALTY_NOTIFICATION_KEY = "Notifications/Mods/Deep Dungeon/item_penalty";
static const std::string FLOOR_ENCHANTMENT_CONVERSATION_KEY = "Conversations/Mods/Deep Dungeon/floor_enchantments";
static const std::string DREAD_BEAST_WARNING_CONVERSATION_KEY = "Conversations/Mods/Deep Dungeon/dread_beast_warning";
static const std::string FLOOR_ENCHANTMENT_PLACEHOLDER_TEXT_KEY = "Conversations/Mods/Deep Dungeon/placeholders/floor_enchantments/init";
static const std::string DREAD_BEAST_WARNING_PLACEHOLDER_TEXT_KEY = "Conversations/Mods/Deep Dungeon/placeholders/dread_beast_warning/init";
static const std::string DREAD_BEAST_WARNING_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Special/dread";
static const std::string GLOOM_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Negative/gloom";
static const std::string HP_PENALTY_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Negative/hp_penalty";
static const std::string EXHAUSTION_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Negative/exhaustion";
static const std::string AMNESIA_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Negative/amnesia";
static const std::string ITEM_PENALTY_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Negative/item_penalty";
static const std::string BLIND_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Negative/blind";
static const std::string DAMAGE_DOWN_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Negative/damage_down";
static const std::string GRAVITY_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Negative/gravity";
static const std::string FEY_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Positive/fey";
static const std::string RESTORATION_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Positive/restoration";
static const std::string SECOND_WIND_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Positive/second_wind";
static const std::string HASTE_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Positive/haste";
//static const std::string HP_STAMINA_BOOST_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Positive/hp_stamina_boost";
static const int TWO_MINUTES_IN_SECONDS = 120;

static enum class DungeonBiomes {
	UPPER,
	TIDE_CAVERNS,
	DEEP_EARTH,
	LAVA_CAVES,
	RUINS
};

static enum class FloorEnchantments {
	// Negative effects
	HP_PENALTY, // Group 1
	EXHAUSTION, // Group 1
	AMNESIA, // Group 2
	ITEM_PENALTY, // Group 2
	BLIND, // Group 2
	DAMAGE_DOWN, // Group 2
	GRAVITY, // Group 1
	GLOOM, // Group 3

	// Positive effects
	RESTORATION, // Group 1
	SECOND_WIND, // Group 1
	HASTE, // Group 1
	//HP_STAMINA_BOOST, // Group 1
	FEY, // Group 3
};

static enum class FloorShrines {
	// Negative effects
	DREAD,
	PERIL,
	CONFUSION,

	// Positive effects
	LEECH,
	INNER_FIRE,
	GRIT
};

static enum class Sigils {
	RAGE,
	STRENGTH,
	FORTIFICATION,
	PROTECTION,
	SERENITY,
	SAFETY,
	SILENCE,
	FORTUNE,
	REDEMPTION,
	ALTERATION,
	CONCEALMENT
};

static const std::vector<FloorEnchantments> GROUP_ONE_FLOOR_ENCHANTMENTS = {
	FloorEnchantments::HP_PENALTY,
	FloorEnchantments::EXHAUSTION,
	FloorEnchantments::GRAVITY,
	FloorEnchantments::RESTORATION,
	FloorEnchantments::SECOND_WIND,
	FloorEnchantments::HASTE,
	//FloorEnchantments::HP_STAMINA_BOOST
};

static const std::vector<FloorEnchantments> GROUP_TWO_FLOOR_ENCHANTMENTS = {
	FloorEnchantments::AMNESIA,
	FloorEnchantments::ITEM_PENALTY,
	FloorEnchantments::BLIND,
	FloorEnchantments::DAMAGE_DOWN,
};

static const std::vector<FloorEnchantments> GROUP_THREE_FLOOR_ENCHANTMENTS = {
	FloorEnchantments::GLOOM,
	FloorEnchantments::FEY
};

static const std::vector<FloorShrines> NEGATIVE_FLOOR_SHRINES = {
	FloorShrines::DREAD,
	FloorShrines::PERIL,
	FloorShrines::CONFUSION
};

static const std::vector<FloorShrines> POSITIVE_FLOOR_SHRINES = {
	FloorShrines::LEECH,
	FloorShrines::INNER_FIRE,
	FloorShrines::GRIT
};

static const std::vector<Sigils> SIGILS = {
	Sigils::RAGE,
	Sigils::STRENGTH,
	Sigils::FORTIFICATION,
	Sigils::PROTECTION,
	Sigils::SERENITY,
	Sigils::SAFETY,
	Sigils::SILENCE,
	Sigils::FORTUNE,
	Sigils::REDEMPTION,
	Sigils::ALTERATION,
	Sigils::CONCEALMENT
};

static YYTKInterface* g_ModuleInterface = nullptr;
static CInstance* global_instance = nullptr;
static bool load_on_start = true;
static bool localize_mod_text = false;
static bool game_is_active = false;
static bool fairy_buff_applied = false;
static bool is_restoration_tracked_interval = false;
static bool is_second_wind_tracked_interval = false;
static int current_time_in_seconds = -1;
static int time_of_last_restoration_tick = -1;
static int time_of_last_second_wind_tick = -1;
static int held_item_id = -1;
static std::string ari_current_location = "";
static std::string ari_current_gm_room = "";
static std::unordered_set<int> consumable_items = {};
static std::map<Sigils, int> sigil_to_item_id_map = {};
static std::map<Sigils, bool> sigil_is_being_used_map = {};
static std::map<std::string, int> perk_name_to_id_map = {};
static std::map<int, int> spell_id_to_default_cost_map = {};
static std::vector<FloorEnchantments> active_floor_enchantments = {};
static std::map<FloorEnchantments, std::string> floor_enchantments_to_localized_string_map = {};
static std::map<std::string, std::vector<CInstance*>> script_name_to_reference_map; // Vector<CInstance*> holds references to Self and Other for each script. 

void ResetStaticFields(bool returned_to_title_screen)
{
	if (returned_to_title_screen)
	{
		game_is_active = false;
		is_restoration_tracked_interval = false;
		is_second_wind_tracked_interval = false;
		current_time_in_seconds = -1;
		time_of_last_restoration_tick = -1;
		time_of_last_second_wind_tick = -1;
		held_item_id = -1;
		ari_current_location = "";
		ari_current_gm_room = "";
		script_name_to_reference_map = {};
	}

	fairy_buff_applied = false;
	sigil_is_being_used_map = {};
	active_floor_enchantments = {};
}

bool GameIsPaused()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);
	RValue paused = global_instance->GetMember("__pause_status");
	return paused.m_i64 > 0;
}

bool StructVariableExists(RValue the_struct, const char* variable_name)
{
	RValue struct_exists = g_ModuleInterface->CallBuiltin(
		"struct_exists",
		{ the_struct, variable_name }
	);

	return struct_exists.ToBoolean();
}

RValue StructVariableSet(RValue the_struct, const char* variable_name, RValue value)
{
	return g_ModuleInterface->CallBuiltin(
		"struct_set",
		{ the_struct, variable_name, value }
	);
}

void LoadPerks()
{
	size_t array_length;
	RValue perk_names = global_instance->GetMember("__perk__");
	g_ModuleInterface->GetArraySize(perk_names, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* perk_name;
		g_ModuleInterface->GetArrayEntry(perk_names, i, perk_name);

		perk_name_to_id_map[perk_name->ToString()] = i;
	}
}

void RemoveAriInvulnerabilityHits()
{
	RValue ari = *global_instance->GetRefMember("__ari");
	*ari.GetRefMember("invulnerable_hits") = 0;
}

void DisableAllPerks()
{
	std::unordered_set<int> perks_to_disable = {};

	std::vector<std::string> struct_field_names = {};
	auto GetStructFieldNames = [&](IN const char* MemberName, IN OUT RValue* Value) {
		struct_field_names.push_back(MemberName);
		return false;
	};
	
	RValue dragon_shrine_data = global_instance->GetMember("__dragon_shrine_data");
	RValue inner = dragon_shrine_data.GetMember("inner");
	
	// Combat Perks
	RValue combat = inner.GetMember("combat");
	g_ModuleInterface->EnumInstanceMembers(combat, GetStructFieldNames);
	for (std::string field_name : struct_field_names)
	{
		if (field_name.contains("tier"))
		{
			size_t array_length;
			RValue tier = combat.GetMember(field_name);
			g_ModuleInterface->GetArraySize(tier, array_length);

			for (size_t i = 0; i < array_length; i++)
			{
				RValue* entry;
				g_ModuleInterface->GetArrayEntry(tier, i, entry);

				perks_to_disable.insert(entry->GetMember("perk").ToInt64());
			}
		}
	}

	// Mining Perks
	struct_field_names = {};
	RValue mining = inner.GetMember("mining");
	g_ModuleInterface->EnumInstanceMembers(mining, GetStructFieldNames);
	for (std::string field_name : struct_field_names)
	{
		if (field_name.contains("tier"))
		{
			size_t array_length;
			RValue tier = mining.GetMember(field_name);
			g_ModuleInterface->GetArraySize(tier, array_length);

			for (size_t i = 0; i < array_length; i++)
			{
				RValue* entry;
				g_ModuleInterface->GetArrayEntry(tier, i, entry);

				perks_to_disable.insert(entry->GetMember("perk").ToInt64());
			}
		}
	}

	// Cooking Perks
	perks_to_disable.insert(perk_name_to_id_map["snacktime"]);

	RValue __ari = *global_instance->GetRefMember("__ari");
	RValue __ari_perks_active = *__ari.GetRefMember("perks_active");
	
	for(int perk : perks_to_disable)
		__ari_perks_active[perk] = false;
}

void LoadConsumableItems()
{
	size_t array_length;
	RValue item_data = global_instance->GetMember("__item_data");
	g_ModuleInterface->GetArraySize(item_data, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* item;
		g_ModuleInterface->GetArrayEntry(item_data, i, item);

		RValue name_key = item->GetMember("name_key");
		if (name_key.m_Kind != VALUE_NULL && name_key.m_Kind != VALUE_UNDEFINED && name_key.m_Kind != VALUE_UNSET)
		{
			if (name_key.ToString().contains("cooked_dishes"))
				consumable_items.insert(item->GetMember("item_id").ToInt64());
			else
			{
				RValue edible = item->GetMember("edible");
				if (edible.m_Kind == VALUE_BOOL && edible.m_Real == 1.0)
					consumable_items.insert(item->GetMember("item_id").ToInt64());
			}
		}
	}
}

void LoadSpells()
{
	size_t array_length = 0;
	RValue spells = global_instance->GetMember("__spells");
	g_ModuleInterface->GetArraySize(spells, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(spells, i, array_element);

		spell_id_to_default_cost_map[i] = array_element->GetMember("cost").ToInt64();
	}
}

void ModifySpellCosts(bool reset_cost) {
	size_t array_length = 0;
	RValue spells = global_instance->GetMember("__spells");
	g_ModuleInterface->GetArraySize(spells, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(spells, i, array_element);

		int cost = reset_cost ? spell_id_to_default_cost_map[i] : spell_id_to_default_cost_map[i] - static_cast<int>(spell_id_to_default_cost_map[i] * 0.5);
		*array_element->GetRefMember("cost") = cost;
	}
}

RValue LocalizeString(CInstance* Self, CInstance* Other, std::string localization_key)
{
	CScript* gml_script_get_localizer = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_LOCALIZER,
		(PVOID*)&gml_script_get_localizer
	);

	RValue result;
	RValue input = RValue(localization_key);
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

void CreateNotification(std::string notification_localization_str, CInstance* Self, CInstance* Other)
{
	CScript* gml_script_create_notification = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CREATE_NOTIFICATION,
		(PVOID*)&gml_script_create_notification
	);

	RValue result;
	RValue notification = RValue(notification_localization_str);
	RValue* notification_ptr = &notification;
	gml_script_create_notification->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &notification_ptr }
	);
}

void PlayConversation(std::string conversation_localization_str, CInstance* Self, CInstance* Other)
{
	CScript* gml_script_play_conversation = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_PLAY_CONVERSATION,
		(PVOID*)&gml_script_play_conversation
	);

	RValue zero = 0;
	RValue conversation = RValue(conversation_localization_str);
	RValue undefined;

	RValue* zero_ptr = &zero;
	RValue* conversation_ptr = &conversation;
	RValue* undefined_ptr = &undefined;

	RValue result;
	RValue* arguments[4] = { zero_ptr, conversation_ptr, undefined_ptr, undefined_ptr };

	gml_script_play_conversation->m_Functions->m_ScriptFunction(
		Self,
		Self,
		result,
		4,
		arguments
	);
}

std::vector<FloorEnchantments> RandomFloorEnchantments(bool is_first_floor, DungeonBiomes dungeon_biome)
{
	static thread_local std::mt19937 random_generator(std::random_device{}());
	std::uniform_int_distribution<size_t> zero_to_ninety_nine_distribution(0, 99);

	std::vector<FloorEnchantments> random_floor_enchantments = {};

	if (is_first_floor)
	{
		const std::vector<FloorEnchantments> FIRST_FLOOR_POSSIBLE_ENCHANTMENTS = {
			FloorEnchantments::RESTORATION, FloorEnchantments::SECOND_WIND, FloorEnchantments::HASTE/*, FloorEnchantments::HP_STAMINA_BOOST*/
		};

		std::uniform_int_distribution<size_t> first_floor_distribution(0, FIRST_FLOOR_POSSIBLE_ENCHANTMENTS.size() - 1);
		return { FIRST_FLOOR_POSSIBLE_ENCHANTMENTS[first_floor_distribution(random_generator)]};
	}
	
	if (dungeon_biome == DungeonBiomes::UPPER)
	{
		// 50% chance for Group 1
		int group_one_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_one_chance < 50)
		{
			std::uniform_int_distribution<size_t> group_one_distribution(0, GROUP_ONE_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.push_back(GROUP_ONE_FLOOR_ENCHANTMENTS[group_one_distribution(random_generator)]);
		}

		// 25% chance for Group 2
		int group_two_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_two_chance < 25)
		{
			std::uniform_int_distribution<size_t> group_two_distribution(0, GROUP_TWO_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.push_back(GROUP_TWO_FLOOR_ENCHANTMENTS[group_two_distribution(random_generator)]);
		}
	}

	if (dungeon_biome == DungeonBiomes::TIDE_CAVERNS)
	{
		// 65% chance for Group 1
		int group_one_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_one_chance < 65)
		{
			std::uniform_int_distribution<size_t> group_one_distribution(0, GROUP_ONE_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.push_back(GROUP_ONE_FLOOR_ENCHANTMENTS[group_one_distribution(random_generator)]);
		}

		// 40% chance for Group 2
		int group_two_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_two_chance < 40)
		{
			std::uniform_int_distribution<size_t> group_two_distribution(0, GROUP_TWO_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.push_back(GROUP_TWO_FLOOR_ENCHANTMENTS[group_two_distribution(random_generator)]);
		}

		// 25% chance for Group 3
		int group_three_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_three_chance < 25)
		{
			int gloom_chance = zero_to_ninety_nine_distribution(random_generator);
			if(gloom_chance < 60) // 60% chance for Gloom
				random_floor_enchantments.push_back(FloorEnchantments::GLOOM);
			else // 40% chance for Fey
				random_floor_enchantments.push_back(FloorEnchantments::FEY);
		}
	}

	if (dungeon_biome == DungeonBiomes::DEEP_EARTH)
	{
		// 45% chance for Group 1
		int group_one_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_one_chance < 45)
		{
			std::uniform_int_distribution<size_t> group_one_distribution(0, GROUP_ONE_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.push_back(GROUP_ONE_FLOOR_ENCHANTMENTS[group_one_distribution(random_generator)]);
		}

		// 65% chance for Group 2
		int group_two_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_two_chance < 65)
		{
			std::uniform_int_distribution<size_t> group_two_distribution(0, GROUP_TWO_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.push_back(GROUP_TWO_FLOOR_ENCHANTMENTS[group_two_distribution(random_generator)]);
		}

		// 30% chance for Group 3
		int group_three_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_three_chance < 30)
		{
			int gloom_chance = zero_to_ninety_nine_distribution(random_generator);
			if (gloom_chance < 70) // 70% chance for Gloom
				random_floor_enchantments.push_back(FloorEnchantments::GLOOM);
			else // 30% chance for Fey
				random_floor_enchantments.push_back(FloorEnchantments::FEY);
		}
	}

	if (dungeon_biome == DungeonBiomes::LAVA_CAVES)
	{
		// 60% chance for Group 1
		int group_one_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_one_chance < 60)
		{
			std::uniform_int_distribution<size_t> group_one_distribution(0, GROUP_ONE_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.push_back(GROUP_ONE_FLOOR_ENCHANTMENTS[group_one_distribution(random_generator)]);
		}

		// 75% chance for Group 2
		int group_two_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_two_chance < 75)
		{
			std::uniform_int_distribution<size_t> group_two_distribution(0, GROUP_TWO_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.push_back(GROUP_TWO_FLOOR_ENCHANTMENTS[group_two_distribution(random_generator)]);
		}

		// 35% chance for Group 3
		int group_three_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_three_chance < 35)
		{
			int gloom_chance = zero_to_ninety_nine_distribution(random_generator);
			if (gloom_chance < 80) // 80% chance for Gloom
				random_floor_enchantments.push_back(FloorEnchantments::GLOOM);
			else // 20% chance for Fey
				random_floor_enchantments.push_back(FloorEnchantments::FEY);
		}
	}

	if (dungeon_biome == DungeonBiomes::RUINS)
	{
		// 65% chance for Group 1
		int group_one_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_one_chance < 65)
		{
			std::uniform_int_distribution<size_t> group_one_distribution(0, GROUP_ONE_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.push_back(GROUP_ONE_FLOOR_ENCHANTMENTS[group_one_distribution(random_generator)]);
		}

		// 75% chance for Group 2
		int group_two_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_two_chance < 75)
		{
			std::uniform_int_distribution<size_t> group_two_distribution(0, GROUP_TWO_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.push_back(GROUP_TWO_FLOOR_ENCHANTMENTS[group_two_distribution(random_generator)]);
		}

		// 40% chance for Group 3
		int group_three_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_three_chance < 40)
		{
			int gloom_chance = zero_to_ninety_nine_distribution(random_generator);
			if (gloom_chance < 90) // 90% chance for Gloom
				random_floor_enchantments.push_back(FloorEnchantments::GLOOM);
			else // 10% chance for Fey
				random_floor_enchantments.push_back(FloorEnchantments::FEY);
		}
	}

	random_floor_enchantments.push_back(FloorEnchantments::FEY);
	random_floor_enchantments.push_back(FloorEnchantments::GLOOM);
	return random_floor_enchantments;
}

void CancelStatusEffect(CInstance* Self, CInstance* Other, RValue status_effect_id)
{
	CScript* gml_script_cancel_status_effect = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CANCEL_STATUS_EFFECT,
		(PVOID*)&gml_script_cancel_status_effect
	);

	RValue result;
	RValue* status_effect_id_ptr = &status_effect_id;

	gml_script_cancel_status_effect->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &status_effect_id_ptr }
	);
}

void CancelAllStatusEffects()
{
	std::vector<CInstance*> refs = script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE];

	// TODO: Don't hardcode this range
	for (int i = 0; i <= 14; i++)
		CancelStatusEffect(refs[0], refs[1], i);
}

void RegisterStatusEffect(CInstance* Self, CInstance* Other, RValue status_effect_id, RValue amount, RValue start, RValue finish)
{
	CScript* gml_script_register_status_effect = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_REGISTER_STATUS_EFFECT,
		(PVOID*)&gml_script_register_status_effect
	);

	RValue result;
	RValue* status_effect_id_ptr = &status_effect_id;
	RValue* amount_ptr = &amount;
	RValue* start_ptr = &start;
	RValue* finish_ptr = &finish;
	RValue* argument_array[4] = { status_effect_id_ptr, amount_ptr, start_ptr, finish_ptr };

	gml_script_register_status_effect->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		4,
		argument_array
	);
}

RValue GetMaxHealth(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_get_max_health = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_MAX_HEALTH,
		(PVOID*)&gml_script_get_max_health
	);

	RValue result;
	gml_script_get_max_health->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		0,
		nullptr
	);

	return result;
}

RValue GetCurrentHealth(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_get_current_health = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_HEALTH,
		(PVOID*)&gml_script_get_current_health
	);

	RValue result;
	gml_script_get_current_health->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		0,
		nullptr
	);

	return result;
}

void SetHealth(CInstance* Self, CInstance* Other, int value)
{
	CScript* gml_script_set_health = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SET_HEALTH,
		(PVOID*)&gml_script_set_health
	);

	RValue result;
	RValue health_modifier = value;
	RValue* health_modifier_ptr = &health_modifier;

	gml_script_set_health->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &health_modifier_ptr }
	);
}

void ModifyHealth(CInstance* Self, CInstance* Other, int value)
{
	CScript* gml_script_modify_health = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_MODIFY_HEALTH,
		(PVOID*)&gml_script_modify_health
	);

	RValue result;
	RValue health_modifier = value;
	RValue* health_modifier_ptr = &health_modifier;

	gml_script_modify_health->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &health_modifier_ptr }
	);
}

void ModifyStamina(CInstance* Self, CInstance* Other, int value)
{
	CScript* gml_script_modify_stamina = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_MODIFY_STAMINA,
		(PVOID*)&gml_script_modify_stamina
	);

	RValue result;
	RValue health_modifier = value;
	RValue* health_modifier_ptr = &health_modifier;

	gml_script_modify_stamina->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &health_modifier_ptr }
	);
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
		// Restoration
		if (is_restoration_tracked_interval)
		{
			ModifyHealth(global_instance->GetRefMember("__ari")->ToInstance(), self, 1);
			is_restoration_tracked_interval = false;
		}

		// Second Wind
		if (is_second_wind_tracked_interval)
		{
			ModifyStamina(global_instance->GetRefMember("__ari")->ToInstance(), self, 1);
			is_second_wind_tracked_interval = false;
		}

		// HP Penalty
		auto hp_penalty = std::find(active_floor_enchantments.begin(), active_floor_enchantments.end(), FloorEnchantments::HP_PENALTY);
		if (hp_penalty != active_floor_enchantments.end())
		{
			RValue max_health = GetMaxHealth(global_instance->GetRefMember("__ari")->ToInstance(), self);
			RValue current_health = GetCurrentHealth(global_instance->GetRefMember("__ari")->ToInstance(), self);

			int penalty = std::trunc(max_health.ToDouble() * 0.25);
			int adjusted_max_health = max_health.ToInt64() - penalty;
			if (current_health.ToInt64() > adjusted_max_health)
				SetHealth(global_instance->GetRefMember("__ari")->ToInstance(), self, adjusted_max_health);
		}

		// Fey
		auto fey = std::find(active_floor_enchantments.begin(), active_floor_enchantments.end(), FloorEnchantments::FEY);
		if (fey != active_floor_enchantments.end())
		{
			std::vector<CInstance*> refs = script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE];

			if (!fairy_buff_applied)
			{
				ModifySpellCosts(false);
				RegisterStatusEffect(refs[0], refs[1], 2, RValue(), 1, 2147483647.0); // TODO: Don't hardcode the status ID (2)
				fairy_buff_applied = true;
			}
		}
	}

	if (strstr(self->m_Object->m_Name, "obj_monster"))
	{
		// Gloom
		auto gloom = std::find(active_floor_enchantments.begin(), active_floor_enchantments.end(), FloorEnchantments::GLOOM);
		if (gloom != active_floor_enchantments.end())
		{
			RValue monster = self->ToRValue();
			if (!StructVariableExists(monster, "__deep_dungeon__gloom_applied") && StructVariableExists(monster, "hit_points"))
			{
				double hit_points = monster.GetMember("hit_points").ToDouble();
				if (std::isfinite(hit_points))
				{
					*monster.GetRefMember("hit_points") = hit_points * 2;
					StructVariableSet(monster, "__deep_dungeon__gloom_applied", true);
				}
			}
		}
	}

	/*
	std::string name = self->m_Object->m_Name;
	if (name == "obj_monster_clod")
	{
		RValue monster = self->ToRValue();
		RValue monster_id = monster.GetMember("monster_id");
		if (monster_id.ToInt64() == 17)
		{
			RValue wait_to_change_attack_pattern_exists = g_ModuleInterface->CallBuiltin("struct_exists", { monster, "__deep_dungeon__wait_to_change_attack_pattern" });
			if (!wait_to_change_attack_pattern_exists.ToBoolean())
				StructVariableSet(monster, "__deep_dungeon__wait_to_change_attack_pattern", false);
			RValue wait_to_change_attack_pattern = monster.GetMember("__deep_dungeon__wait_to_change_attack_pattern");

			RValue custom_attack_pattern_exists = g_ModuleInterface->CallBuiltin("struct_exists", { monster, "__deep_dungeon__custom_attack_pattern" });
			if (!custom_attack_pattern_exists.ToBoolean())
			{
				StructVariableSet(monster, "__deep_dungeon__custom_attack_pattern", 0);
				if (StructVariableExists(monster, "config"))
				{
					RValue config = *monster.GetRefMember("config");
					*config.GetRefMember("attack_sequence") = 20.0;
					*config.GetRefMember("attack_legion") = 10.0;
					*config.GetRefMember("projectile_speed") = 3.5;
				}
			}

			if (StructVariableExists(monster, "aggro"))
			{
				RValue aggro = monster.GetMember("aggro"); // BOOL
				int temp = 5;
			}

			if (StructVariableExists(monster, "fsm"))
			{
				RValue state_id = monster.GetMember("fsm").GetMember("state").GetMember("state_id");
				if (state_id.ToInt64() == 4)
					*monster.GetRefMember("__deep_dungeon__wait_to_change_attack_pattern") = false;

				if (state_id.ToInt64() == 5 && !wait_to_change_attack_pattern.ToBoolean()) // Tired
				{
					*monster.GetRefMember("__deep_dungeon__wait_to_change_attack_pattern") = true;

					int custom_attack_pattern = monster.GetMember("__deep_dungeon__custom_attack_pattern").ToInt64() + 1;
					if (custom_attack_pattern > 2)
						custom_attack_pattern = 0;

					if (custom_attack_pattern == 0)
					{
						if (StructVariableExists(monster, "config"))
						{
							// Shoots a wall of 10 pellets repeatedly 5 times
							RValue config = *monster.GetRefMember("config");
							*config.GetRefMember("attack_sequence") = 5.0;
							*config.GetRefMember("attack_legion") = 10.0;
							*config.GetRefMember("attack_sequence_turn") = -1.0;
							*config.GetRefMember("attack_sequence_image_speed") = -1.0;
							*config.GetRefMember("projectile_speed") = 3.0;
							*config.GetRefMember("split_distance") = -1.0;
							*config.GetRefMember("split_depth") = -1.0;
							*config.GetRefMember("split_angle") = -1.0;
						}
					}
					if (custom_attack_pattern == 1)
					{
						if (StructVariableExists(monster, "config"))
						{
							// Rotates 18-degrees at a time while shooting 5 pellets in a small cone
							RValue config = *monster.GetRefMember("config");
							*config.GetRefMember("attack_sequence") = 20.0;
							*config.GetRefMember("attack_legion") = 5.0;
							*config.GetRefMember("attack_sequence_turn") = 18.0;
							*config.GetRefMember("attack_sequence_image_speed") = 2.0;
							*config.GetRefMember("projectile_speed") = 3.0;
							*config.GetRefMember("split_distance") = -1.0;
							*config.GetRefMember("split_depth") = -1.0;
							*config.GetRefMember("split_angle") = -1.0;
						}
					}
					if (custom_attack_pattern == 2)
					{
						if (StructVariableExists(monster, "config"))
						{
							// Shoots a single pellet that then splits into many that repeatedly split
							RValue config = *monster.GetRefMember("config");
							*config.GetRefMember("attack_sequence") = 5.0;
							*config.GetRefMember("attack_legion") = 1.0;
							*config.GetRefMember("attack_sequence_turn") = -1.0;
							*config.GetRefMember("attack_sequence_image_speed") = -1.0;
							*config.GetRefMember("projectile_speed") = 3.0;
							*config.GetRefMember("split_distance") = 20.0;
							*config.GetRefMember("split_depth") = 5.0;
							*config.GetRefMember("split_angle") = 20.0;
						}
					}

					*monster.GetRefMember("__deep_dungeon__custom_attack_pattern") = custom_attack_pattern;
				}
			}
		}
	}
	*/
}

RValue& GmlScriptModifyStaminaCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	// Exhaustion
	auto exhaustion = std::find(active_floor_enchantments.begin(), active_floor_enchantments.end(), FloorEnchantments::EXHAUSTION);
	if (exhaustion != active_floor_enchantments.end())
	{
		if (Arguments[0]->ToDouble() < 0)
		{
			double modified_stamina_cost = Arguments[0]->ToDouble() * 2;
			*Arguments[0] = modified_stamina_cost;
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_MODIFY_STAMINA));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptCanCastSpellCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_CAN_CAST_SPELL));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	// Amnesia
	auto amnesia = std::find(active_floor_enchantments.begin(), active_floor_enchantments.end(), FloorEnchantments::AMNESIA);
	if (amnesia != active_floor_enchantments.end())
	{
		Result = 0.0;
	}

	return Result;
}

RValue& GmlScriptGetMoveSpeedCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_MOVE_SPEED));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	// Gravity
	auto gravity = std::find(active_floor_enchantments.begin(), active_floor_enchantments.end(), FloorEnchantments::GRAVITY);
	if (gravity != active_floor_enchantments.end())
	{
		Result = 1.0;
	}

	// Haste
	auto haste = std::find(active_floor_enchantments.begin(), active_floor_enchantments.end(), FloorEnchantments::HASTE);
	if (haste != active_floor_enchantments.end())
	{
		Result = 3.0;
	}
	
	return Result; // 2.0 is default run speed
}

RValue& GmlScriptDamageCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	static thread_local std::mt19937 random_generator(std::random_device{}());
	std::uniform_int_distribution<size_t> zero_to_one_distribution(0, 1);

	// Blind
	auto blind = std::find(active_floor_enchantments.begin(), active_floor_enchantments.end(), FloorEnchantments::BLIND);
	if (blind != active_floor_enchantments.end())
	{
		RValue target = Arguments[0]->GetMember("target");
		if (target.ToInt64() != 1) // Everything not Ari
		{
			bool miss = zero_to_one_distribution(random_generator);
			if (miss)
			{
				*Arguments[0]->GetRefMember("damage") = 0.0;
				*Arguments[0]->GetRefMember("critical") = false;
				*Arguments[0]->GetRefMember("knockback") = false;
				//*Arguments[0]->GetRefMember("frozen") = true; // Frozen debuff
				//*Arguments[0]->GetRefMember("venomous") = true; // Poison debuff
				//*Arguments[0]->GetRefMember("electrocute_kind") = 0; // Paralysis debuff (doesn't seem to affect monsters)
				//*Arguments[0]->GetRefMember("can_pick_grid_objects") = true; // Pick node objects
				//*Arguments[0]->GetRefMember("can_chop_grid_objects") = true; // Chop node objects
			}
		}
	}

	// Damage Down
	auto damage_down = std::find(active_floor_enchantments.begin(), active_floor_enchantments.end(), FloorEnchantments::DAMAGE_DOWN);
	if (damage_down != active_floor_enchantments.end())
	{
		// TODO: Test if Fire Breath gets penalized multiple times by this.

		RValue target = Arguments[0]->GetMember("target");
		if (target.ToInt64() != 1) // Everything not Ari
		{
			double damage = Arguments[0]->GetMember("damage").ToDouble();
			int penalty = std::trunc(damage * 0.30); // 30% reduced damage
			*Arguments[0]->GetRefMember("damage") = damage - penalty;
		}
	}

	// Gloom
	auto gloom = std::find(active_floor_enchantments.begin(), active_floor_enchantments.end(), FloorEnchantments::GLOOM);
	if (gloom != active_floor_enchantments.end())
	{
		if (!StructVariableExists(*Arguments[0], "__deep_dungeon__gloom_applied")) // Prevents monster attacks that "persist" from repeatedly getting Gloom applied
		{
			RValue target = Arguments[0]->GetMember("target");
			if (target.ToInt64() == 1) // Ari
			{
				double damage = std::trunc(Arguments[0]->GetMember("damage").ToDouble() * 1.5); // 50% increased damage
				*Arguments[0]->GetRefMember("damage") = damage;
			}
			else
			{
				double damage = Arguments[0]->GetMember("damage").ToDouble();
				int penalty = std::trunc(damage * 0.50); // 50% reduced damage
				*Arguments[0]->GetRefMember("damage") = damage - penalty;
			}

			StructVariableSet(*Arguments[0], "__deep_dungeon__gloom_applied", true);
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_DAMAGE));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptAriShouldDieCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_ARI_SHOULD_DIE));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	//auto fey = std::find(active_floor_enchantments.begin(), active_floor_enchantments.end(), FloorEnchantments::FEY);
	//if (fey != active_floor_enchantments.end() && Result.ToBoolean() && fairy_buff_is_active)
	//{
	//	fairy_buff_is_active = false;
	//	canceling_fairy_buff = true;
	//	Result = false;
	//}

	return Result;
}

RValue& GmlScriptStatusEffectManagerCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	// TODO: Empty the map when returning to the title screen
	if (!script_name_to_reference_map.contains(GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE))
		script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE] = { Self, Other };
	
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptUseItemCallback(
	IN CInstance* Self, // Changes depending on the invocation context. For world interactables like a fountain, Self->m_Object->m_Name == "obj_world_fountain". For Ari using an item, Self->m_Object == NULL.
	IN CInstance* Other, // Changes depending on the invocation context. For world interactables like a fountain, Other->m_Object->m_Name == "Game". For Ari using an item, Other->m_Object->m_Name == "obj_ari".
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	auto item_penalty = std::find(active_floor_enchantments.begin(), active_floor_enchantments.end(), FloorEnchantments::ITEM_PENALTY);
	if (item_penalty != active_floor_enchantments.end())
	{
		if (Self->m_Object == NULL && strstr(Other->m_Object->m_Name, "obj_ari"))
		{
			if (held_item_id != sigil_to_item_id_map[Sigils::SERENITY] && consumable_items.contains(held_item_id))
			{
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - You are unable to use items due to the Item Penalty floor enchantment!", MOD_NAME, VERSION);
				CreateNotification(ITEM_PENALTY_NOTIFICATION_KEY, Self, Other);
				return Result;
			}
			else
				sigil_is_being_used_map[Sigils::SERENITY] = true;
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_USE_ITEM));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptHeldItemCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_HELD_ITEM));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (Result.m_Kind != VALUE_UNDEFINED)
	{
		int item_id = Result.GetMember("item_id").ToInt64();
		if (held_item_id != item_id)
			held_item_id = item_id;
	}

	return Result;
}

RValue& GmlScriptGetMinutesCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (game_is_active)
	{
		current_time_in_seconds = Arguments[0]->ToInt64();

		// Restoration
		auto restoration = std::find(active_floor_enchantments.begin(), active_floor_enchantments.end(), FloorEnchantments::RESTORATION);
		if (restoration != active_floor_enchantments.end())
		{
			if (!is_restoration_tracked_interval && (current_time_in_seconds - time_of_last_restoration_tick) >= TWO_MINUTES_IN_SECONDS)
			{
				is_restoration_tracked_interval = true;
				time_of_last_restoration_tick = current_time_in_seconds;
			}
		}

		// Second Wind
		auto second_wind = std::find(active_floor_enchantments.begin(), active_floor_enchantments.end(), FloorEnchantments::SECOND_WIND);
		if (second_wind != active_floor_enchantments.end())
		{
			if (!is_second_wind_tracked_interval && (current_time_in_seconds - time_of_last_second_wind_tick) >= TWO_MINUTES_IN_SECONDS)
			{
				is_second_wind_tracked_interval = true;
				time_of_last_second_wind_tick = current_time_in_seconds;
			}
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_MINUTES));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptGetLocalizerCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (localize_mod_text)
	{
		localize_mod_text = false;

		floor_enchantments_to_localized_string_map[FloorEnchantments::GLOOM] = LocalizeString(Self, Other, GLOOM_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::HP_PENALTY] = LocalizeString(Self, Other, HP_PENALTY_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::EXHAUSTION] = LocalizeString(Self, Other, EXHAUSTION_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::AMNESIA] = LocalizeString(Self, Other, AMNESIA_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::ITEM_PENALTY] = LocalizeString(Self, Other, ITEM_PENALTY_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::BLIND] = LocalizeString(Self, Other, BLIND_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::DAMAGE_DOWN] = LocalizeString(Self, Other, DAMAGE_DOWN_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::GRAVITY] = LocalizeString(Self, Other, GRAVITY_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::FEY] = LocalizeString(Self, Other, FEY_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::RESTORATION] = LocalizeString(Self, Other, RESTORATION_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::SECOND_WIND] = LocalizeString(Self, Other, SECOND_WIND_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::HASTE] = LocalizeString(Self, Other, HASTE_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();		
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_LOCALIZER));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active)
	{
		std::string debug = Arguments[0]->ToString();
		if (Arguments[0]->ToString() == FLOOR_ENCHANTMENT_PLACEHOLDER_TEXT_KEY)
		{
			std::string custom_text = "";
			for (int i = 0; i < active_floor_enchantments.size(); i++)
			{
				custom_text += floor_enchantments_to_localized_string_map[active_floor_enchantments[i]];
				if (i < active_floor_enchantments.size() - 1)
					custom_text += "\n";
			}
			Result = RValue(custom_text);
			return Result;
		}
		if (Arguments[0]->ToString() == DREAD_BEAST_WARNING_PLACEHOLDER_TEXT_KEY)
		{
			// TODO
		}
	}

	return Result;
}

RValue& GmlScriptGetWeatherCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	game_is_active = true;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_WEATHER));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptTryLocationIdToStringCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_TRY_LOCATION_ID_TO_STRING));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active && Result.m_Kind == VALUE_STRING)
		ari_current_location = Result.ToString();

	return Result;
}

RValue& GmlScriptOnDungeonRoomStartCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	// TODO: Run logic to actually undo all active floor enchantments.
	// TOOD: Remove all buffs.
	active_floor_enchantments = {};
	DisableAllPerks();
	ModifySpellCosts(true);
	CancelAllStatusEffects();
	RemoveAriInvulnerabilityHits();
	fairy_buff_applied = false;

	if (ari_current_gm_room != "rm_mines_entry" && ari_current_gm_room.find("seal") == std::string::npos && ari_current_gm_room.find("ritual") == std::string::npos && ari_current_gm_room.find("treasure") == std::string::npos && ari_current_gm_room.find("milestone") == std::string::npos)
	{
		if (ari_current_gm_room == "rm_mines_upper_floor1")
		{
			active_floor_enchantments = RandomFloorEnchantments(true, DungeonBiomes::UPPER);

			// DEBUG SPAWN OBJ -------------------------------------------------------------------------------------------------------
			//RValue layer_exists = g_ModuleInterface->CallBuiltin("layer_exists", { "Impl_Offering" });
			//if (!layer_exists.ToBoolean())
			//{
			//	g_ModuleInterface->CallBuiltin("layer_create", { 400, "Impl_Offering" });
			//	RValue obj_dungeon_ritual_altar_index = g_ModuleInterface->CallBuiltin("asset_get_index", { "obj_dungeon_shrine" });
			//	double x = 224.0;
			//	double y = 160.0;
			//	std::string layer_name = "Impl_Offering";
			//	RValue shrine_struct;
			//	RValue item_id = 1427;
			//	RValue shrine_type = 0.0;
			//	RValue allowed_in_fire_spell = false;
			//	RValue offset_with_cardinal = false;
			//	RValue interactable_mode = 1;
			//	RValue can_use = true;
			//	g_ModuleInterface->GetRunnerInterface().StructCreate(&shrine_struct);
			//	g_ModuleInterface->GetRunnerInterface().StructAddRValue(&shrine_struct, "item_id", &item_id);
			//	g_ModuleInterface->GetRunnerInterface().StructAddRValue(&shrine_struct, "shrine_type", &shrine_type);
			//	g_ModuleInterface->GetRunnerInterface().StructAddRValue(&shrine_struct, "allowed_in_fire_spell", &allowed_in_fire_spell);
			//	g_ModuleInterface->GetRunnerInterface().StructAddRValue(&shrine_struct, "offset_with_cardinal", &offset_with_cardinal);
			//	g_ModuleInterface->GetRunnerInterface().StructAddRValue(&shrine_struct, "interactable_mode", &interactable_mode);
			//	g_ModuleInterface->GetRunnerInterface().StructAddRValue(&shrine_struct, "can_use", &can_use);
			//	
			//	try {
			//		g_ModuleInterface->CallBuiltin("instance_create_layer", { x, y, RValue(layer_name), obj_dungeon_ritual_altar_index, shrine_struct });
			//	}
			//	catch (...) {}
			//}
			// -----------------------------------------------------------------------------------------------------------------------
		}
		else if (ari_current_gm_room.find("rm_mines_upper") != std::string::npos)
		{
			active_floor_enchantments = RandomFloorEnchantments(false, DungeonBiomes::UPPER);
		}
		//else if (ari_current_gm_room.find("rm_mines_tide") != std::string::npos || ari_current_gm_room.find("rm_mines_deep") != std::string::npos || ari_current_gm_room.find("rm_mines_lava") != std::string::npos || ari_current_gm_room.find("rm_mines_ruins") != std::string::npos)
		//{
		//	std::uniform_int_distribution<int> distribution(1, 100);
		//	int random = distribution(generator);
		//	if (random <= ritual_chamber_additional_spawn_chance)
		//		teleport_ari = true;
		//}

		if (!active_floor_enchantments.empty())
			PlayConversation(FLOOR_ENCHANTMENT_CONVERSATION_KEY, Self, Other);

		for (FloorEnchantments floor_enchantment : active_floor_enchantments)
		{
			if (floor_enchantment == FloorEnchantments::RESTORATION)
				time_of_last_restoration_tick = current_time_in_seconds;
			if (floor_enchantment == FloorEnchantments::SECOND_WIND)
				time_of_last_second_wind_tick = current_time_in_seconds;
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_ON_DUNGEON_ROOM_START));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptGoToRoomCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GO_TO_ROOM));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	RValue gm_room = Result.GetMember("gm_room"); //StructVariableGet(Result, "gm_room"); 
	RValue room_name = g_ModuleInterface->CallBuiltin("room_get_name", { gm_room });
	ari_current_gm_room = room_name.ToString();

	if (ari_current_location == "dungeon" && (!ari_current_gm_room.contains("rm_mines") || ari_current_gm_room == "rm_mines_entry"))
	{
		// TODO: Run logic to actually undo all active floor enchantments.
		// TOOD: Remove all buffs.
		active_floor_enchantments = {};
		CancelAllStatusEffects();
		ModifySpellCosts(true);
		fairy_buff_applied = false;
	}

	return Result;
}

RValue& GmlScriptSetupMainScreenCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (load_on_start)
	{
		load_on_start = false;
		localize_mod_text = true;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		LoadPerks();
		LoadSpells();
		LoadConsumableItems();
		// TODO: Load other stuff
	}
	else
		ResetStaticFields(true);

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_SETUP_MAIN_SCREEN));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptOnDrawGuiCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_on_draw_gui@Display@Display"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active && !GameIsPaused())
	{
		// Gloom
		auto gloom = std::find(active_floor_enchantments.begin(), active_floor_enchantments.end(), FloorEnchantments::GLOOM);
		if (gloom != active_floor_enchantments.end())
		{
			RValue window_width = g_ModuleInterface->CallBuiltin("window_get_width", {}); // TODO: Don't do this in this function
			RValue window_height = g_ModuleInterface->CallBuiltin("window_get_height", {}); // TODO: Don't do this in this function

			// Draw semi-transparent overlay
			static double transparency = 0.60; // temp for testing

			g_ModuleInterface->CallBuiltin(
				"draw_set_alpha",
				{ transparency }
			);

			g_ModuleInterface->CallBuiltin(
				"draw_set_color", {
					8388736 // c_purple
				}
			);

			g_ModuleInterface->CallBuiltin(
				"draw_rectangle",
				{ 0, 0, window_width, window_height, false }
			);
		}
	}

	return Result;
}

void CreateObjectCallback(AurieStatus& status)
{
	status = g_ModuleInterface->CreateCallback(
		g_ArSelfModule,
		EVENT_OBJECT_CALL,
		ObjectCallback,
		0
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook (EVENT_OBJECT_CALL)!", MOD_NAME, VERSION);
	}
}

void CreateHookGmlScriptModifyStamina(AurieStatus& status)
{
	CScript* gml_script_modify_stamina = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_MODIFY_STAMINA,
		(PVOID*)&gml_script_modify_stamina
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_MODIFY_STAMINA);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_MODIFY_STAMINA,
		gml_script_modify_stamina->m_Functions->m_ScriptFunction,
		GmlScriptModifyStaminaCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_MODIFY_STAMINA);
	}
}

void CreateHookGmlScriptCanCastSpell(AurieStatus& status)
{
	CScript* gml_script_can_cast_spell = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CAN_CAST_SPELL,
		(PVOID*)&gml_script_can_cast_spell
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CAN_CAST_SPELL);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_CAN_CAST_SPELL,
		gml_script_can_cast_spell->m_Functions->m_ScriptFunction,
		GmlScriptCanCastSpellCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CAN_CAST_SPELL);
	}
}

void CreateHookGmlScriptGetMoveSpeed(AurieStatus& status)
{
	CScript* gml_script_get_move_speed = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_MOVE_SPEED,
		(PVOID*)&gml_script_get_move_speed
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_MOVE_SPEED);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_MOVE_SPEED,
		gml_script_get_move_speed->m_Functions->m_ScriptFunction,
		GmlScriptGetMoveSpeedCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_MOVE_SPEED);
	}
}

void CreateHookGmlScriptDamage(AurieStatus& status)
{
	CScript* gml_script_damage = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_DAMAGE,
		(PVOID*)&gml_script_damage
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_DAMAGE);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_DAMAGE,
		gml_script_damage->m_Functions->m_ScriptFunction,
		GmlScriptDamageCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_DAMAGE);
	}
}

void CreateHookGmlScriptAriShouldDie(AurieStatus& status)
{
	CScript* gml_script_ari_should_die = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_ARI_SHOULD_DIE,
		(PVOID*)&gml_script_ari_should_die
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ARI_SHOULD_DIE);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_ARI_SHOULD_DIE,
		gml_script_ari_should_die->m_Functions->m_ScriptFunction,
		GmlScriptAriShouldDieCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ARI_SHOULD_DIE);
	}
}

void CreateHookGmlScriptStatusEffectManager(AurieStatus& status)
{
	CScript* gml_script_status_effect_manager = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE,
		(PVOID*)&gml_script_status_effect_manager
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE,
		gml_script_status_effect_manager->m_Functions->m_ScriptFunction,
		GmlScriptStatusEffectManagerCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE);
	}
}

void CreateHookGmlScriptHeldItem(AurieStatus& status)
{
	CScript* gml_script_held_item = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_HELD_ITEM,
		(PVOID*)&gml_script_held_item
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_HELD_ITEM);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_HELD_ITEM,
		gml_script_held_item->m_Functions->m_ScriptFunction,
		GmlScriptHeldItemCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_HELD_ITEM);
	}
}

void CreateHookGmlScriptUseItem(AurieStatus& status)
{
	CScript* gml_script_use_item = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_USE_ITEM,
		(PVOID*)&gml_script_use_item
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_USE_ITEM);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_USE_ITEM,
		gml_script_use_item->m_Functions->m_ScriptFunction,
		GmlScriptUseItemCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_USE_ITEM);
	}
}

void CreateHookGmlScriptGetMinutes(AurieStatus& status)
{
	CScript* gml_script_get_minutes = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_MINUTES,
		(PVOID*)&gml_script_get_minutes
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_MINUTES);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_MINUTES,
		gml_script_get_minutes->m_Functions->m_ScriptFunction,
		GmlScriptGetMinutesCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_MINUTES);
	}
}

void CreateHookGmlScriptGetLocalizer(AurieStatus& status)
{
	CScript* gml_script_get_localizer = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_LOCALIZER,
		(PVOID*)&gml_script_get_localizer
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_LOCALIZER);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_LOCALIZER,
		gml_script_get_localizer->m_Functions->m_ScriptFunction,
		GmlScriptGetLocalizerCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_LOCALIZER);
	}
}

void CreateHookGmlScriptGetWeather(AurieStatus& status)
{
	CScript* gml_script_get_weather = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_WEATHER,
		(PVOID*)&gml_script_get_weather
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_WEATHER);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_WEATHER,
		gml_script_get_weather->m_Functions->m_ScriptFunction,
		GmlScriptGetWeatherCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_WEATHER);
	}
}

void CreateHookGmlScriptTryLocationIdToString(AurieStatus& status)
{
	CScript* gml_script_try_location_id_to_string = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_TRY_LOCATION_ID_TO_STRING,
		(PVOID*)&gml_script_try_location_id_to_string
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_TRY_LOCATION_ID_TO_STRING);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_TRY_LOCATION_ID_TO_STRING,
		gml_script_try_location_id_to_string->m_Functions->m_ScriptFunction,
		GmlScriptTryLocationIdToStringCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_TRY_LOCATION_ID_TO_STRING);
	}
}

void CreateHookGmlScriptOnDungeonRoomStart(AurieStatus& status)
{
	CScript* gml_script_on_dungeon_room_start = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_ON_DUNGEON_ROOM_START,
		(PVOID*)&gml_script_on_dungeon_room_start
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ON_DUNGEON_ROOM_START);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_ON_DUNGEON_ROOM_START,
		gml_script_on_dungeon_room_start->m_Functions->m_ScriptFunction,
		GmlScriptOnDungeonRoomStartCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ON_DUNGEON_ROOM_START);
	}
}

void CreateHookGmlScriptGoToRoom(AurieStatus& status)
{
	CScript* gml_script_go_to_room = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GO_TO_ROOM,
		(PVOID*)&gml_script_go_to_room
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GO_TO_ROOM);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GO_TO_ROOM,
		gml_script_go_to_room->m_Functions->m_ScriptFunction,
		GmlScriptGoToRoomCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GO_TO_ROOM);
	}
}

void CreateHookGmlScriptSetupMainScreen(AurieStatus& status)
{
	CScript* gml_script_setup_main_screen = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SETUP_MAIN_SCREEN,
		(PVOID*)&gml_script_setup_main_screen
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_SETUP_MAIN_SCREEN);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_SETUP_MAIN_SCREEN,
		gml_script_setup_main_screen->m_Functions->m_ScriptFunction,
		GmlScriptSetupMainScreenCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_SETUP_MAIN_SCREEN);
	}
}

void CreateHookGmlScriptOnDrawGui(AurieStatus& status)
{
	CScript* gml_script_on_draw_gui = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_ON_DRAW_GUI,
		(PVOID*)&gml_script_on_draw_gui
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ON_DRAW_GUI);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_ON_DRAW_GUI,
		gml_script_on_draw_gui->m_Functions->m_ScriptFunction,
		GmlScriptOnDrawGuiCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ON_DRAW_GUI);
	}
}

EXPORTED AurieStatus ModuleInitialize(
	IN AurieModule* Module,
	IN const fs::path& ModulePath
)
{
	UNREFERENCED_PARAMETER(ModulePath);

	AurieStatus status = AURIE_SUCCESS;

	Sigils x = Sigils::RAGE;

	status = ObGetInterface(
		"YYTK_Main",
		(AurieInterfaceBase*&)(g_ModuleInterface)
	);

	if (!AurieSuccess(status))
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[%s %s] - Plugin starting...", MOD_NAME, VERSION);

	CreateObjectCallback(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptModifyStamina(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptCanCastSpell(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptGetMoveSpeed(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptDamage(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptAriShouldDie(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptStatusEffectManager(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptHeldItem(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptUseItem(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptGetMinutes(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptGetLocalizer(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptGetWeather(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptTryLocationIdToString(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptOnDungeonRoomStart(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptGoToRoom(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptOnDrawGui(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Plugin started!", MOD_NAME, VERSION);
	return AURIE_SUCCESS;
}
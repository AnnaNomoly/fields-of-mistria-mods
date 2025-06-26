#include <map>
#include <random>
#include <fstream>
#include <nlohmann/json.hpp>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

static const char* const MOD_NAME = "StatueOfBoons";
static const char* const VERSION = "1.0.0";
static const char* const MANA_COST_KEY = "mana_cost"; // Used in mod config file
static const char* const ESSENCE_COST_KEY = "essence_cost"; // Used in mod config file
static const char* const MANA_IS_REQUIRED_KEY = "mana_is_required"; // Used in mod config file
static const char* const ESSENCE_IS_REQUIRED_KEY = "essence_is_required"; // Used in mod config file
static const char* const ACTIVE_BOON_KEY = "active_boon"; // Used in mod save file
static const char* const PREVIOUS_BOON_KEY = "previous_boon"; // Used in mod save file
static const char* const DRAGON_FAIRY_LOCATION_KEY = "dragon_fairy_location"; // Used in mod save file
static const char* const DRAGON_FAIRY_CAUGHT_KEY = "dragon_fairy_caught"; // Used in mod save file
static const char* const GML_SCRIPT_TRY_STRING_TO_ITEM_ID = "gml_Script_try_string_to_item_id";
static const char* const GML_SCRIPT_TRY_OBJECT_ID_TO_STRING = "gml_Script_try_object_id_to_string";
static const char* const GML_SCRIPT_CREATE_NOTIFICATION = "gml_Script_create_notification";
static const char* const GML_SCRIPT_SPAWN_BUG = "gml_Script_spawn_bug";
static const char* const GML_SCRIPT_GET_MANA = "gml_Script_get_mana@Ari@Ari";
static const char* const GML_SCRIPT_GET_ESSENCE = "gml_Script_get_essence@Ari@Ari";
static const char* const GML_SCRIPT_MODIFY_MANA = "gml_Script_modify_mana@Ari@Ari";
static const char* const GML_SCRIPT_MODIFY_ESSENCE = "gml_Script_modify_essence@Ari@Ari";
static const char* const GML_SCRIPT_GET_FISHING_CELEBRATION_DATA = "gml_Script_get_celebration_data_essence_exp@anon@14736@Fish@Fish";
static const char* const GML_SCRIPT_GET_DIVING_CELEBRATION_DATA = "gml_Script_get_celebration_data@anon@15509@DiveSpot@Fish";
static const char* const GML_SCRIPT_GIVE_ARI_ITEM = "gml_Script_give_item@Ari@Ari";
static const char* const GML_SCRIPT_GET_MOVE_SPEED = "gml_Script_get_move_speed@Ari@Ari";
static const char* const GML_SCRIPT_INTERACT = "gml_Script_interact";
static const char* const GML_SCRIPT_SHOW_ROOM_TITLE = "gml_Script_show_room_title";
static const char* const GML_SCRIPT_CREATE_BUG = "gml_Script_setup@gml_Object_obj_bug_Create_0"; // TODO: This script isn't needed -- gml_Script_goto_gm_room
static const char* const GML_SCRIPT_ADD_HEART_POINTS = "gml_Script_add_heart_points@Npc@Npc";
static const char* const GML_SCRIPT_MODIFY_STAMINA = "gml_Script_modify_stamina@Ari@Ari";
static const char* const GML_SCRIPT_TRY_LOCATION_ID_TO_STRING = "gml_Script_try_location_id_to_string";
static const char* const GML_SCRIPT_END_DAY = "gml_Script_end_day";
static const char* const GML_SCRIPT_PLAY_TEXT = "gml_Script_play_text@TextboxMenu@TextboxMenu";
static const char* const GML_SCRIPT_PLAY_CONVERSATION = "gml_Script_play_conversation";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const char* const GML_SCRIPT_SAVE_GAME = "gml_Script_save_game";
static const char* const GML_SCRIPT_LOAD_GAME = "gml_Script_load_game";
static const std::string NONE = "none";
static const std::string LOCATION_FARM = "farm";
static const std::string OBJECT_CATEGORY_BUSH = "bush";
static const std::string OBJECT_CATEGORY_CROP = "crop";
static const std::string OBJECT_CATEGORY_ROCK = "rock";
static const std::string OBJECT_CATEGORY_STUMP = "stump";
static const std::string OBJECT_CATEGORY_TREE = "tree";
static const std::string CUSTOM_ITEM_NAME = "dragon_fairy";
static const std::string CUSTOM_OBJECT_NAME = "statue_of_boons";
static const std::string BOON_OF_SPEED = "boon_of_speed";
static const std::string BOON_OF_FORAGE = "boon_of_forage";
static const std::string BOON_OF_FISHING = "boon_of_fishing";
static const std::string BOON_OF_BUTTERFLY = "boon_of_butterfly";
static const std::string BOON_OF_FRIENDSHIP = "boon_of_friendship";
static const std::string BOON_OF_STAMINA = "boon_of_stamina";
static const std::string WESTERN_RUINS = "western_ruins";
static const std::string EASTERN_ROAD = "eastern_road";
static const std::string NARROWS = "narrows";
static const std::string HAYDENS_FARM = "haydens_farm";
static const std::string BEACH = "beach";
static const std::string STATUE_OF_BOONS_CONVERSATION_KEY = "Conversations/Mods/Statue of Boons/statue_of_boons";
static const std::string STATUE_OF_BOONS_PLACEHOLDER_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/placeholder";
static const std::string STATUE_OF_BOONS_INSUFFICIENT_MANA_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/insufficient_mana";
static const std::string STATUE_OF_BOONS_INSUFFICIENT_ESSENCE_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/insufficient_essence";
static const std::string STATUE_OF_BOONS_BOON_OF_SPEED_GRANTED_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_speed/granted"; // "Boon of the Wind"
static const std::string STATUE_OF_BOONS_BOON_OF_FORAGE_GRANTED_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_forage/granted"; // "Boon of the Land"
static const std::string STATUE_OF_BOONS_BOON_OF_FISHING_GRANTED_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_fishing/granted"; // "Boon of the Sea"
static const std::string STATUE_OF_BOONS_BOON_OF_BUTTERFLY_GRANTED_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_butterfly/granted"; // "Boon of the Dragons"
static const std::string STATUE_OF_BOONS_BOON_OF_FRIENDSHIP_GRANTED_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_friendship/granted"; // "Boon of the Moon"
static const std::string STATUE_OF_BOONS_BOON_OF_STAMINA_GRANTED_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_stamina/granted"; // "Boon of the Sun"
static const std::string STATUE_OF_BOONS_BOON_OF_SPEED_ACTIVE_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_speed/active";
static const std::string STATUE_OF_BOONS_BOON_OF_FORAGE_ACTIVE_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_forage/active";
static const std::string STATUE_OF_BOONS_BOON_OF_FISHING_ACTIVE_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_fishing/active";
static const std::string STATUE_OF_BOONS_BOON_OF_BUTTERFLY_ACTIVE_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_butterfly/active";
static const std::string STATUE_OF_BOONS_BOON_OF_FRIENDSHIP_ACTIVE_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_friendship/active";
static const std::string STATUE_OF_BOONS_BOON_OF_STAMINA_ACTIVE_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_stamina/active";
static const std::string BOON_OF_BUTTERFLY_DETECTED_DIALOGUE_KEY = "Notifications/Mods/Statue of Boons/boon_of_butterfly/detected";
static const std::vector<std::string> LIST_OF_BOONS = { BOON_OF_SPEED, BOON_OF_FORAGE, BOON_OF_FISHING, BOON_OF_BUTTERFLY, BOON_OF_FRIENDSHIP, BOON_OF_STAMINA };
static const std::vector<std::string> LIST_OF_LOCATIONS = { WESTERN_RUINS, EASTERN_ROAD, NARROWS, HAYDENS_FARM, BEACH };
static const std::map<std::string, std::vector<std::vector<int>>> CUSTOM_BUG_SPAWN_LOCATIONS = {
	{WESTERN_RUINS, {{145,128},{177,98},{177,166}}}, // working
	{EASTERN_ROAD, {{63,69},{131,69},{31,127},{190,95},{114,144},{39,186},{111,241},{32,259}}}, // working
	{NARROWS, {{138,52},{41,108},{130,117},{176,145},{114,189},{195,217}}}, // working
	{HAYDENS_FARM, {{73,45},{49,83},{70,105},{164,54},{185,115},{121,104}}}, // working
	{BEACH, {{303,77},{162,45},{175,75},{267,74},{259,37}}} // working
};
static const int DEFAULT_MANA_COST = 1;
static const int DEFAULT_ESSENCE_COST = 5;
static const bool DEFAULT_MANA_IS_REQUIRED = true;
static const bool DEFAULT_ESSENCE_IS_REQUIRED = false;

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_on_start = true;
static bool game_is_active = false;
static bool custom_object_used = false;
static int mana_cost = DEFAULT_MANA_COST;
static int essence_cost = DEFAULT_ESSENCE_COST;
static bool mana_is_required = DEFAULT_MANA_IS_REQUIRED;
static bool essence_is_required = DEFAULT_ESSENCE_IS_REQUIRED;
static int ari_current_mana = -1;
static int ari_current_essence = -1;
static bool reduce_ari_mana = false;
static bool reduce_ari_essence = false;
static std::string ari_current_location = NONE;
static RValue custom_conversation_value;
static RValue* custom_conversation_value_ptr = nullptr;
static RValue custom_dialogue_value;
static RValue* custom_dialogue_value_ptr = nullptr;
static bool boon_of_speed = false;
static bool boon_of_forage = false;
static bool boon_of_fishing = false;
static bool boon_of_butterfly = false;
static bool boon_of_friendship = false;
static bool boon_of_stamina = false;
static bool modify_items_added = false;
static bool dragon_fairy_caught = false; // Used to track if the bug has been caught
static bool spawning_dragon_fairy = false; // Used to track when the bug is being spawned during script call stack
static std::string dragon_fairy_location = NONE; // Used to track the randomly selected location
static std::string previous_boon = NONE;
static int dragon_fairy_item_id = -1;
static std::map<std::string, int> object_category_to_id_map = {};
static std::vector<int> forage_boon_objects = {};
static std::map<int, std::string> object_id_to_name_map = {};
static std::random_device rd;
static std::mt19937 gen(rd());
static std::string save_prefix = "";
static std::string mod_folder = "";

// DEBUG------------------------------------------------------
static int interact_count = 0;
static int bug_spawn_count = 0;
static std::vector<std::string> struct_field_names = {};
bool EnumFunction(
	IN const char* MemberName,
	IN OUT RValue* Value
)
{
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "Member Name: %s", MemberName);
	return false;
}
bool GetStructFieldNames(
	IN const char* MemberName,
	IN OUT RValue* Value
)
{
	struct_field_names.push_back(MemberName);
	return false;
}
//------------------------------------------------------------

int RValueAsInt(RValue value)
{
	if (value.m_Kind == VALUE_REAL)
		return static_cast<int>(value.m_Real);
	if (value.m_Kind == VALUE_INT64)
		return static_cast<int>(value.m_i64);
	if (value.m_Kind == VALUE_INT32)
		return static_cast<int>(value.m_i32);
}

bool RValueAsBool(RValue value)
{
	if (value.m_Kind == VALUE_REAL && value.m_Real == 1)
		return true;
	if (value.m_Kind == VALUE_BOOL && value.m_Real == 1)
		return true;
	return false;
}

bool StructVariableExists(RValue the_struct, const char* variable_name)
{
	RValue struct_exists = g_ModuleInterface->CallBuiltin(
		"struct_exists",
		{ the_struct, variable_name }
	);

	return RValueAsBool(struct_exists);
}

RValue StructVariableGet(RValue the_struct, const char* variable_name)
{
	return g_ModuleInterface->CallBuiltin(
		"struct_get",
		{ the_struct, variable_name }
	);
}

RValue StructVariableSet(RValue the_struct, const char* variable_name, RValue value)
{
	return g_ModuleInterface->CallBuiltin(
		"struct_set",
		{ the_struct, variable_name, value }
	);
}

RValue TryStringToItemId(CInstance* Self, CInstance* Other, std::string item_name_str)
{
	CScript* gml_script_try_string_to_item_id = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_TRY_STRING_TO_ITEM_ID,
		(PVOID*)&gml_script_try_string_to_item_id
	);

	RValue item_id;
	RValue item_name = item_name_str;
	RValue* item_name_ptr = &item_name;

	gml_script_try_string_to_item_id->m_Functions->m_ScriptFunction(
		Self,
		Other,
		item_id,
		1,
		{ &item_name_ptr }
	);

	return item_id;
}

void LoadObjectCategories()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	size_t array_length = 0;
	RValue object_categories = global_instance->at("__object_category__");
	g_ModuleInterface->GetArraySize(object_categories, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* object_category;
		g_ModuleInterface->GetArrayEntry(object_categories, i, object_category);

		std::string object_category_str = object_category->AsString().data();
		object_category_to_id_map[object_category_str] = i;
	}
}

void LoadObjectIds(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_try_object_id_to_string = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_TRY_OBJECT_ID_TO_STRING,
		(PVOID*)&gml_script_try_object_id_to_string
	);

	for (int i = 0; i < 5000; i++)
	{
		RValue result;
		RValue object_id = i;
		RValue* object_id_ptr = &object_id;
		gml_script_try_object_id_to_string->m_Functions->m_ScriptFunction(
			Self,
			Other,
			result,
			1,
			{ &object_id_ptr }
		);

		if (result.m_Kind == VALUE_STRING)
			object_id_to_name_map[i] = result.AsString().data();
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Loaded %d objects!", MOD_NAME, VERSION, static_cast<int>(object_id_to_name_map.size()));
}

void LoadObjectItemData()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	size_t array_length = 0;
	RValue node_prototypes = global_instance->at("__node_prototypes");
	g_ModuleInterface->GetArraySize(node_prototypes, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* node_prototype;
		g_ModuleInterface->GetArrayEntry(node_prototypes, i, node_prototype);

		bool object_is_eligible = false;
		int object_id = RValueAsInt(StructVariableGet(*node_prototype, "object_id"));
		int category_id = RValueAsInt(StructVariableGet(*node_prototype, "category_id"));

		if (category_id == object_category_to_id_map[OBJECT_CATEGORY_BUSH] || category_id == object_category_to_id_map[OBJECT_CATEGORY_CROP])		
			object_is_eligible = true;
		if (category_id == object_category_to_id_map[OBJECT_CATEGORY_ROCK] || category_id == object_category_to_id_map[OBJECT_CATEGORY_STUMP] || category_id == object_category_to_id_map[OBJECT_CATEGORY_TREE])
			object_is_eligible = true;

		if (object_is_eligible)
		{
			auto it = std::find(forage_boon_objects.begin(), forage_boon_objects.end(), object_id);
			if (it == forage_boon_objects.end())
				forage_boon_objects.push_back(object_id);
		}
	}
}

void CreateNotification(std::string notification_localization_str, CInstance* Self, CInstance* Other)
{
	CScript* gml_script_create_notification = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CREATE_NOTIFICATION,
		(PVOID*)&gml_script_create_notification
	);

	RValue result;
	RValue notification = notification_localization_str;
	RValue* notification_ptr = &notification;
	gml_script_create_notification->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &notification_ptr }
	);
}

RValue SpawnBug(CInstance* Self, CInstance* Other, int x_coord, int y_coord)
{
	CScript* gml_script_spawn_bug = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SPAWN_BUG,
		(PVOID*)&gml_script_spawn_bug
	);

	RValue result;
	RValue x = x_coord;
	RValue y = y_coord;
	RValue undefined;
	RValue* x_ptr = &x;
	RValue* y_ptr = &y;
	RValue* undefined_ptr = &undefined;
	RValue* argument_array[3] = { x_ptr, y_ptr, undefined_ptr };

	spawning_dragon_fairy = true;
	gml_script_spawn_bug->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		3,
		argument_array
	);

	return result;
}

RValue GetCurrentMana(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_get_mana = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_MANA,
		(PVOID*)&gml_script_get_mana
	);

	RValue current_mana;
	gml_script_get_mana->m_Functions->m_ScriptFunction(
		Self,
		Other,
		current_mana,
		0,
		nullptr
	);

	return current_mana;
}

RValue GetCurrentEssence(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_get_essence = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_ESSENCE,
		(PVOID*)&gml_script_get_essence
	);

	RValue current_essence;
	gml_script_get_essence->m_Functions->m_ScriptFunction(
		Self,
		Other,
		current_essence,
		0,
		nullptr
	);

	return current_essence;
}

void ModifyMana(CInstance* Self, CInstance* Other, int value)
{
	CScript* gml_script_modify_mana = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_MODIFY_MANA,
		(PVOID*)&gml_script_modify_mana
	);

	RValue result;
	RValue mana_modifier = value;
	RValue* mana_modifier_ptr = &mana_modifier;

	gml_script_modify_mana->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &mana_modifier_ptr }
	);
}

void ModifyEssence(CInstance* Self, CInstance* Other, int value)
{
	CScript* gml_script_modify_essence = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_MODIFY_ESSENCE,
		(PVOID*)&gml_script_modify_essence
	);

	RValue result;
	RValue essence_modifier = value;
	RValue* essence_modifier_ptr = &essence_modifier;

	gml_script_modify_essence->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &essence_modifier_ptr }
	);
}

bool AnyBoonIsActive()
{
	if (boon_of_speed || boon_of_forage || boon_of_fishing || boon_of_butterfly || boon_of_friendship || boon_of_stamina)
		return true;
	return false;
}

std::string GetActiveBoonString()
{
	if (boon_of_speed)
		return BOON_OF_SPEED;
	if (boon_of_forage)
		return BOON_OF_FORAGE;
	if (boon_of_fishing)
		return BOON_OF_FISHING;
	if (boon_of_butterfly)
		return BOON_OF_BUTTERFLY;
	if (boon_of_friendship)
		return BOON_OF_FRIENDSHIP;
	if (boon_of_stamina)
		return BOON_OF_STAMINA;
	return NONE;
}

void LoadActiveBoonString(std::string input)
{
	if (input == BOON_OF_SPEED)
		boon_of_speed = true;
	if (input == BOON_OF_FORAGE)
		boon_of_forage = true;
	if (input == BOON_OF_FISHING)
		boon_of_fishing = true;
	if (input == BOON_OF_BUTTERFLY)
		boon_of_butterfly = true;
	if (input == BOON_OF_FRIENDSHIP)
		boon_of_friendship = true;
	if (input == BOON_OF_STAMINA)
		boon_of_stamina = true;
}

void PrintError(std::exception_ptr eptr)
{
	try {
		if (eptr) {
			std::rethrow_exception(eptr);
		}
	}
	catch (const std::exception& e) {
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Error: %s", MOD_NAME, VERSION, e.what());
	}
}

void WriteModSaveFile()
{
	// Write the custom mod data file.
	if (save_prefix.length() != 0 && mod_folder.length() != 0)
	{
		json mod_save_data = {};
		mod_save_data[ACTIVE_BOON_KEY] = GetActiveBoonString();
		mod_save_data[PREVIOUS_BOON_KEY] = previous_boon;
		mod_save_data[DRAGON_FAIRY_LOCATION_KEY] = dragon_fairy_location;
		mod_save_data[DRAGON_FAIRY_CAUGHT_KEY] = dragon_fairy_caught;

		std::exception_ptr eptr;
		try
		{
			std::ofstream out_stream(mod_folder + "\\" + save_prefix + ".json");
			out_stream << std::setw(4) << mod_save_data << std::endl;
			out_stream.close();
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Successfully saved the mod file!", MOD_NAME, VERSION);
		}
		catch (...)
		{
			g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - An error occurred writing the mod file.", MOD_NAME, VERSION);

			eptr = std::current_exception();
			PrintError(eptr);
		}
	}
}

void ReadModSaveFile()
{
	std::exception_ptr eptr;
	try
	{
		std::ifstream in_stream(mod_folder + "\\" + save_prefix + ".json");
		if (in_stream.good())
		{
			json mod_save_data = json::parse(in_stream);
			LoadActiveBoonString(mod_save_data[ACTIVE_BOON_KEY]);
			previous_boon = mod_save_data[PREVIOUS_BOON_KEY];
			dragon_fairy_location = mod_save_data[DRAGON_FAIRY_LOCATION_KEY];
			dragon_fairy_caught = mod_save_data[DRAGON_FAIRY_CAUGHT_KEY];
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Successfully loaded the mod file!", MOD_NAME, VERSION);
		}
	}
	catch (...)
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - An error occurred reading the mod file.", MOD_NAME, VERSION);

		eptr = std::current_exception();
		PrintError(eptr);
	}
}

void LogDefaultConfigValues()
{
	mana_cost = DEFAULT_MANA_COST;
	essence_cost = DEFAULT_ESSENCE_COST;
	mana_is_required = DEFAULT_MANA_IS_REQUIRED;
	essence_is_required = DEFAULT_ESSENCE_IS_REQUIRED;

	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, MANA_COST_KEY, DEFAULT_MANA_COST);
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, ESSENCE_COST_KEY, DEFAULT_ESSENCE_COST);
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, MANA_IS_REQUIRED_KEY, DEFAULT_MANA_IS_REQUIRED ? "true" : "false");
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, ESSENCE_IS_REQUIRED_KEY, DEFAULT_ESSENCE_IS_REQUIRED ? "true" : "false");
}

void CreateOrLoadModConfigFile()
{
	// Load config file.
	std::exception_ptr eptr;
	try
	{
		// Try to find the mod_data directory.
		std::string current_dir = std::filesystem::current_path().string();
		std::string mod_data_folder = current_dir + "\\mod_data";
		if (!std::filesystem::exists(mod_data_folder))
		{
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"mod_data\" directory was not found. Creating directory: %s", MOD_NAME, VERSION, mod_data_folder.c_str());
			std::filesystem::create_directory(mod_data_folder);
		}

		// Try to find the mod_data/StatueOfBoons directory.
		std::string statue_of_boons_folder = mod_data_folder + "\\StatueOfBoons";
		if (!std::filesystem::exists(statue_of_boons_folder))
		{
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"StatueOfBoons\" directory was not found. Creating directory: %s", MOD_NAME, VERSION, statue_of_boons_folder.c_str());
			std::filesystem::create_directory(statue_of_boons_folder);
		}

		mod_folder = statue_of_boons_folder;

		// Try to find the mod_data/StatueOfBoons/StatueOfBoons.json config file.
		std::string config_file = statue_of_boons_folder + "\\" + "StatueOfBoons.json";
		std::ifstream in_stream(config_file);
		if (in_stream.good())
		{
			try
			{
				json json_object = json::parse(in_stream);

				// Check if the json_object is empty.
				if (json_object.empty())
				{
					g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - No values found in mod configuration file: %s!", MOD_NAME, VERSION, config_file.c_str());
					g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Add your desired values to the configuration file, otherwise defaults will be used.", MOD_NAME, VERSION);
					LogDefaultConfigValues();
				}
				else
				{
					// Try loading the mana_cost value.
					if (json_object.contains(MANA_COST_KEY))
					{
						mana_cost = json_object[MANA_COST_KEY];
						if (mana_cost < 0 || mana_cost > 12)
						{
							g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Invalid \"%s\" value (%d) in mod configuration file: %s", MOD_NAME, VERSION, MANA_COST_KEY, mana_cost, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - This value MUST be a valid integer between 0 and 12 (inclusive)!", MOD_NAME, VERSION);
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, MANA_COST_KEY, DEFAULT_MANA_COST);
							mana_cost = DEFAULT_MANA_COST;
						}
						else
						{
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %d!", MOD_NAME, VERSION, MANA_COST_KEY, mana_cost);
						}
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, MANA_COST_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, MANA_COST_KEY, DEFAULT_MANA_COST);
						mana_cost = DEFAULT_MANA_COST;
					}

					// Try loading the essence_cost value.
					if (json_object.contains(ESSENCE_COST_KEY))
					{
						essence_cost = json_object[ESSENCE_COST_KEY];
						if (essence_cost < 0 || essence_cost > 100)
						{
							g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Invalid \"%s\" value (%d) in mod configuration file: %s", MOD_NAME, VERSION, ESSENCE_COST_KEY, essence_cost, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - This value MUST be a valid integer between 0 and 100 (inclusive)!", MOD_NAME, VERSION);
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, ESSENCE_COST_KEY, DEFAULT_ESSENCE_COST);
							essence_cost = DEFAULT_ESSENCE_COST;
						}
						else
						{
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %d!", MOD_NAME, VERSION, ESSENCE_COST_KEY, essence_cost);
						}
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, ESSENCE_COST_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, ESSENCE_COST_KEY, DEFAULT_ESSENCE_COST);
						essence_cost = DEFAULT_ESSENCE_COST;
					}

					// Try loading the mana_is_required value.
					if (json_object.contains(MANA_IS_REQUIRED_KEY))
					{
						mana_is_required = json_object[MANA_IS_REQUIRED_KEY];
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %s!", MOD_NAME, VERSION, MANA_IS_REQUIRED_KEY, mana_is_required ? "true" : "false");
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, MANA_IS_REQUIRED_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, MANA_IS_REQUIRED_KEY, DEFAULT_MANA_IS_REQUIRED ? "true" : "false");
						mana_is_required = DEFAULT_MANA_IS_REQUIRED;
					}

					// Try loading the essence_is_required value.
					if (json_object.contains(ESSENCE_IS_REQUIRED_KEY))
					{
						essence_is_required = json_object[ESSENCE_IS_REQUIRED_KEY];
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %s!", MOD_NAME, VERSION, ESSENCE_IS_REQUIRED_KEY, mana_is_required ? "true" : "false");
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, ESSENCE_IS_REQUIRED_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, ESSENCE_IS_REQUIRED_KEY, DEFAULT_ESSENCE_IS_REQUIRED ? "true" : "false");
						essence_is_required = DEFAULT_ESSENCE_IS_REQUIRED;
					}
				}
			}
			catch (...)
			{
				eptr = std::current_exception();
				PrintError(eptr);

				g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to parse JSON from configuration file: %s", MOD_NAME, VERSION, config_file.c_str());
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Make sure the file is valid JSON!", MOD_NAME, VERSION);
				LogDefaultConfigValues();
			}

			in_stream.close();
		}
		else
		{
			in_stream.close();

			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"StatueOfBoons.json\" file was not found. Creating file: %s", MOD_NAME, VERSION, config_file.c_str());
			json default_json = {
				{MANA_COST_KEY, DEFAULT_MANA_COST},
				{ESSENCE_COST_KEY, DEFAULT_ESSENCE_COST},
				{MANA_IS_REQUIRED_KEY, DEFAULT_MANA_IS_REQUIRED},
				{ESSENCE_IS_REQUIRED_KEY, DEFAULT_ESSENCE_IS_REQUIRED}
			};

			std::ofstream out_stream(config_file);
			out_stream << std::setw(4) << default_json << std::endl;
			out_stream.close();

			LogDefaultConfigValues();
		}
	}
	catch (...)
	{
		eptr = std::current_exception();
		PrintError(eptr);

		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - An error occurred loading the mod configuration file.", MOD_NAME, VERSION);
		LogDefaultConfigValues();
	}
}

void ResetStaticFields(bool returned_to_title_screen)
{
	if (returned_to_title_screen)
	{
		game_is_active = false;
		ari_current_mana = -1;
		ari_current_essence = -1;
		ari_current_location = NONE;
		custom_conversation_value = RValue();
		custom_conversation_value_ptr = nullptr;
		custom_dialogue_value = RValue();
		custom_dialogue_value_ptr = nullptr;
		previous_boon = NONE;
		save_prefix = "";
	}

	custom_object_used = false;
	reduce_ari_mana = false;
	reduce_ari_essence = false;
	boon_of_speed = false;
	boon_of_forage = false;
	boon_of_fishing = false;
	boon_of_butterfly = false;
	boon_of_friendship = false;
	boon_of_stamina = false;
	modify_items_added = false;
	dragon_fairy_caught = false;
	spawning_dragon_fairy = false;
	dragon_fairy_location = NONE;
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
		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		if (reduce_ari_mana)
		{
			reduce_ari_mana = false;
			ModifyMana(global_instance->at("__ari").m_Object, self, static_cast<double>(-1 * mana_cost));
		}
			
		if (reduce_ari_essence)
		{
			reduce_ari_essence = false;
			ModifyEssence(global_instance->at("__ari").m_Object, self, static_cast<double>(-1 * essence_cost));
		}

		ari_current_mana = RValueAsInt(GetCurrentMana(global_instance->at("__ari").m_Object, self));
		ari_current_essence = RValueAsInt(GetCurrentEssence(global_instance->at("__ari").m_Object, self));
	}
}

RValue& GmlScriptGetFishingCelebrationDataCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if(boon_of_fishing)
		modify_items_added = true;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_FISHING_CELEBRATION_DATA));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptGetDivingCelebrationDataCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (boon_of_fishing)
		modify_items_added = true;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_DIVING_CELEBRATION_DATA));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptGiveItemCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (boon_of_butterfly && !dragon_fairy_caught && ari_current_location == dragon_fairy_location)
	{
		RValue item = Arguments[0]->m_Object;
		int item_id = RValueAsInt(item.at("item_id"));

		if(item_id == dragon_fairy_item_id)
			dragon_fairy_caught = true;
	}

	if (modify_items_added)
	{
		modify_items_added = false;
		if (ari_current_location != LOCATION_FARM)
		{
			Arguments[1]->m_Real = Arguments[1]->m_Real * 2;
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Doubled the item obtained.", MOD_NAME, VERSION);
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GIVE_ARI_ITEM));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

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
	if (!game_is_active)
		game_is_active = true;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_MOVE_SPEED));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (boon_of_speed)
		Result.m_Real += 0.5;
		
	return Result; // 2.0 is default run speed, max run speed boost from gear is 20% => 2.4
}

RValue& GmlScriptInteractCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	RValue object = Arguments[0]->m_Object;
	int object_id = RValueAsInt(object.at("object_id"));
	std::string object_name = object_id_to_name_map[object_id];

	// Statue of Boons
	if (object_name.find(CUSTOM_OBJECT_NAME) != std::string::npos)
		custom_object_used = true;
	else 
	{
		// Objects affected by Statue of Boons
		auto it = std::find(forage_boon_objects.begin(), forage_boon_objects.end(), object_id);
		if (it != forage_boon_objects.end())
		{
			if (boon_of_forage)
				modify_items_added = true;
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_INTERACT));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);
	return Result;
}

RValue& GmlScriptShowRoomTitleCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_SHOW_ROOM_TITLE));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (boon_of_butterfly && !dragon_fairy_caught)
	{
		if (ari_current_location == dragon_fairy_location)
		{
			std::vector<std::vector<int>> possible_spawn_points = CUSTOM_BUG_SPAWN_LOCATIONS.at(dragon_fairy_location);
			std::uniform_int_distribution<> choose_random_spawn_point(0, static_cast<int>(possible_spawn_points.size() - 1));
			std::vector<int> random_spawn_point = possible_spawn_points[choose_random_spawn_point(gen)];
			SpawnBug(Self, Other, random_spawn_point[0], random_spawn_point[1]);
			CreateNotification(BOON_OF_BUTTERFLY_DETECTED_DIALOGUE_KEY, Self, Other);
		}
	}

	return Result;
}

RValue& GmlScriptCreateBugCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (spawning_dragon_fairy)
	{
		spawning_dragon_fairy = false;
		int fairy_bee_item_id = RValueAsInt(TryStringToItemId(Self, Other, "dragon_fairy"));
		Arguments[0]->m_i64 = fairy_bee_item_id;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_CREATE_BUG));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;  
}

RValue& GmlScriptAddHeartPointsCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_ADD_HEART_POINTS));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (boon_of_friendship)
		Arguments[0]->m_Real = ceil(1.5 * Arguments[0]->m_Real);

	return Result;
}

RValue& GmlScriptModifyStaminaCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_MODIFY_STAMINA));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (boon_of_stamina && Arguments[0]->m_Real < 0)
		Arguments[0]->m_Real = 0;

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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_try_location_id_to_string"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active && Result.m_Kind == VALUE_STRING)
		ari_current_location = Result.AsString().data();

	return Result;
}

RValue& GmlScriptEndDayCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_END_DAY));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	previous_boon = GetActiveBoonString();
	ResetStaticFields(false);
	WriteModSaveFile(); // TODO: Should this be called here? Save Game might get called automatically after this (CHECKING...)
	
	return Result;
}

RValue& GmlScriptPlayTextCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	std::string conversation_name = Arguments[0]->AsString().data();
	if (conversation_name.find(STATUE_OF_BOONS_PLACEHOLDER_DIALOGUE_KEY) != std::string::npos)
	{
		// Check if a boon is already active.
		if (AnyBoonIsActive())
		{
			if (boon_of_speed)
				custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_SPEED_ACTIVE_DIALOGUE_KEY;
			if (boon_of_forage)
				custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_FORAGE_ACTIVE_DIALOGUE_KEY;
			if (boon_of_fishing)
				custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_FISHING_ACTIVE_DIALOGUE_KEY;
			if (boon_of_butterfly)
				custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_BUTTERFLY_ACTIVE_DIALOGUE_KEY;
			if (boon_of_friendship)
				custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_FRIENDSHIP_ACTIVE_DIALOGUE_KEY;
			if (boon_of_stamina)
				custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_STAMINA_ACTIVE_DIALOGUE_KEY;
		}
		else
		{
			// Check if mana or essence is required and that Ari has enough to activate the statue.
			bool statue_activation_requirements_met = true;
			if (mana_is_required && ari_current_mana < mana_cost)
			{
				statue_activation_requirements_met = false;
				custom_dialogue_value = STATUE_OF_BOONS_INSUFFICIENT_MANA_DIALOGUE_KEY;
			}
			if (essence_is_required && ari_current_mana < mana_cost)
			{
				statue_activation_requirements_met = false;
				custom_dialogue_value = STATUE_OF_BOONS_INSUFFICIENT_ESSENCE_DIALOGUE_KEY;
			}

			if (statue_activation_requirements_met)
			{
				if (mana_is_required)
					reduce_ari_mana = true;
				if (essence_is_required)
					reduce_ari_essence = true;

				//std::uniform_int_distribution<> choose_random_boon(0, static_cast<int>(LIST_OF_BOONS.size() - 1));
				//std::string random_boon = LIST_OF_BOONS[choose_random_boon(gen)];
				std::string random_boon = BOON_OF_BUTTERFLY; // DEBUG

				if (random_boon == BOON_OF_SPEED)
				{
					boon_of_speed = true;
					custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_SPEED_GRANTED_DIALOGUE_KEY;
				}
				if (random_boon == BOON_OF_FORAGE)
				{
					boon_of_forage = true;
					custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_FORAGE_GRANTED_DIALOGUE_KEY;
				}
				if (random_boon == BOON_OF_FISHING)
				{
					boon_of_fishing = true;
					custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_FISHING_GRANTED_DIALOGUE_KEY;
				}
				if (random_boon == BOON_OF_BUTTERFLY)
				{
					boon_of_butterfly = true;
					dragon_fairy_caught = false;
					spawning_dragon_fairy = false;
					custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_BUTTERFLY_GRANTED_DIALOGUE_KEY;

					std::uniform_int_distribution<> choose_random_location(0, static_cast<int>(LIST_OF_LOCATIONS.size() - 1));
					dragon_fairy_location = LIST_OF_LOCATIONS[choose_random_location(gen)];
				}
				if (random_boon == BOON_OF_FRIENDSHIP)
				{
					boon_of_friendship = true;
					custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_FRIENDSHIP_GRANTED_DIALOGUE_KEY;
				}
				if (random_boon == BOON_OF_STAMINA)
				{
					boon_of_stamina = true;
					custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_STAMINA_GRANTED_DIALOGUE_KEY;
				}
			}
		}

		custom_dialogue_value_ptr = &custom_dialogue_value;
		Arguments[0] = custom_dialogue_value_ptr;
	}
	
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_PLAY_TEXT));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptPlayConversationCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (custom_object_used)
	{
		custom_object_used = false;
		custom_conversation_value = STATUE_OF_BOONS_CONVERSATION_KEY;
		custom_conversation_value_ptr = &custom_conversation_value;
		Arguments[1] = custom_conversation_value_ptr;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_PLAY_CONVERSATION));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

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
		dragon_fairy_item_id = RValueAsInt(TryStringToItemId(Self, Other, CUSTOM_ITEM_NAME));
		LoadObjectCategories();
		LoadObjectIds(Self, Other);
		LoadObjectItemData();
		CreateOrLoadModConfigFile();
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

RValue& GmlScriptSaveGameCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	// No save prefix has been detected. This should only happen when a new game is started.
	if (save_prefix.size() == 0)
	{
		// Get the save file name.
		std::string save_file = Arguments[0]->AsString().data();
		std::size_t save_file_name_delimiter_index = save_file.find_last_of("/");
		std::string save_name = save_file.substr(save_file_name_delimiter_index + 1);

		// Check it's a valid value.
		if (save_name.find("undefined") == std::string::npos)
		{
			// Get the save prefix.
			std::size_t first_hyphen_index = save_name.find_first_of("-") + 1;
			std::size_t second_hyphen_index = save_name.find_last_of("-");
			save_prefix = save_name.substr(first_hyphen_index, (second_hyphen_index - first_hyphen_index));
		}
	}

	// Write the custom mod data file.
	WriteModSaveFile();

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_SAVE_GAME));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptLoadGameCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	// Get the save file name.
	std::string save_file = std::string(Arguments[0]->m_Object->at("save_path").AsString().data());
	std::size_t save_file_name_delimiter_index = save_file.find_last_of("/");
	std::string save_name = save_file.substr(save_file_name_delimiter_index + 1);

	// Get the save prefix.
	std::size_t first_hyphen_index = save_name.find_first_of("-") + 1;
	std::size_t second_hyphen_index = save_name.find_last_of("-");
	save_prefix = save_name.substr(first_hyphen_index, (second_hyphen_index - first_hyphen_index));

	// Read from the custom mod data file.
	ReadModSaveFile();

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_LOAD_GAME));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

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

void CreateHookGmlScriptGetFishingCelebrationData(AurieStatus& status)
{
	CScript* gml_script_get_fishing_celebration_data = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_FISHING_CELEBRATION_DATA,
		(PVOID*)&gml_script_get_fishing_celebration_data
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_FISHING_CELEBRATION_DATA);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_FISHING_CELEBRATION_DATA,
		gml_script_get_fishing_celebration_data->m_Functions->m_ScriptFunction,
		GmlScriptGetFishingCelebrationDataCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_FISHING_CELEBRATION_DATA);
	}
}

void CreateHookGmlScriptGetDivingCelebrationData(AurieStatus& status)
{
	CScript* gml_script_get_diving_celebration_data = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_DIVING_CELEBRATION_DATA,
		(PVOID*)&gml_script_get_diving_celebration_data
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_DIVING_CELEBRATION_DATA);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_DIVING_CELEBRATION_DATA,
		gml_script_get_diving_celebration_data->m_Functions->m_ScriptFunction,
		GmlScriptGetDivingCelebrationDataCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_DIVING_CELEBRATION_DATA);
	}
}

void CreateHookGmlScriptGiveItem(AurieStatus& status)
{
	CScript* gml_script_give_item = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GIVE_ARI_ITEM,
		(PVOID*)&gml_script_give_item
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GIVE_ARI_ITEM);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GIVE_ARI_ITEM,
		gml_script_give_item->m_Functions->m_ScriptFunction,
		GmlScriptGiveItemCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GIVE_ARI_ITEM);
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

void CreateHookGmlScriptInteract(AurieStatus& status)
{
	CScript* gml_script_interact = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_INTERACT,
		(PVOID*)&gml_script_interact
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_INTERACT);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_INTERACT,
		gml_script_interact->m_Functions->m_ScriptFunction,
		GmlScriptInteractCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_INTERACT);
	}
}

void CreateHookGmlScriptShowRoomTitle(AurieStatus& status)
{
	CScript* gml_script_create_bug = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SHOW_ROOM_TITLE,
		(PVOID*)&gml_script_create_bug
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_SHOW_ROOM_TITLE);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_SHOW_ROOM_TITLE,
		gml_script_create_bug->m_Functions->m_ScriptFunction,
		GmlScriptShowRoomTitleCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_SHOW_ROOM_TITLE);
	}
}

void CreateHookGmlScriptCreateBug(AurieStatus& status)
{
	CScript* gml_script_create_bug = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CREATE_BUG,
		(PVOID*)&gml_script_create_bug
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CREATE_BUG);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_CREATE_BUG,
		gml_script_create_bug->m_Functions->m_ScriptFunction,
		GmlScriptCreateBugCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CREATE_BUG);
	}
}

void CreateHookGmlScriptAddHeartPoints(AurieStatus& status)
{
	CScript* gml_script_add_heart_points = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_ADD_HEART_POINTS,
		(PVOID*)&gml_script_add_heart_points
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ADD_HEART_POINTS);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_ADD_HEART_POINTS,
		gml_script_add_heart_points->m_Functions->m_ScriptFunction,
		GmlScriptAddHeartPointsCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ADD_HEART_POINTS);
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

void CreateHookGmlScriptEndDay(AurieStatus& status)
{
	CScript* gml_script_end_day = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_END_DAY,
		(PVOID*)&gml_script_end_day
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_END_DAY);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_END_DAY,
		gml_script_end_day->m_Functions->m_ScriptFunction,
		GmlScriptEndDayCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_END_DAY);
	}
}

void CreateHookGmlScriptPlayText(AurieStatus& status)
{
	CScript* gml_script_play_text = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_PLAY_TEXT,
		(PVOID*)&gml_script_play_text
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_PLAY_TEXT);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_PLAY_TEXT,
		gml_script_play_text->m_Functions->m_ScriptFunction,
		GmlScriptPlayTextCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_PLAY_TEXT);
	}
}

void CreateHookGmlScriptPlayConversation(AurieStatus& status)
{
	CScript* gml_script_play_conversation = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_PLAY_CONVERSATION,
		(PVOID*)&gml_script_play_conversation
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_PLAY_CONVERSATION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_PLAY_CONVERSATION,
		gml_script_play_conversation->m_Functions->m_ScriptFunction,
		GmlScriptPlayConversationCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_PLAY_CONVERSATION);
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

void CreateHookGmlScriptSaveGame(AurieStatus& status)
{
	CScript* gml_script_save_game = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SAVE_GAME,
		(PVOID*)&gml_script_save_game
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_SAVE_GAME);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_SAVE_GAME,
		gml_script_save_game->m_Functions->m_ScriptFunction,
		GmlScriptSaveGameCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_SAVE_GAME);
	}
}

void CreateHookGmlScriptLoadGame(AurieStatus& status)
{
	CScript* gml_script_load_game = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_LOAD_GAME,
		(PVOID*)&gml_script_load_game
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_LOAD_GAME);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_LOAD_GAME,
		gml_script_load_game->m_Functions->m_ScriptFunction,
		GmlScriptLoadGameCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_LOAD_GAME);
	}
}

EXPORTED AurieStatus ModuleInitialize(
	IN AurieModule* Module,
	IN const fs::path& ModulePath
)
{
	UNREFERENCED_PARAMETER(ModulePath);

	AurieStatus status = AURIE_SUCCESS;
	
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

	CreateHookGmlScriptGetFishingCelebrationData(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptGetDivingCelebrationData(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptGiveItem(status);
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

	CreateHookGmlScriptInteract(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptShowRoomTitle(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptCreateBug(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptAddHeartPoints(status);
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

	CreateHookGmlScriptTryLocationIdToString(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptEndDay(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptPlayText(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptPlayConversation(status);
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

	CreateHookGmlScriptSaveGame(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptLoadGame(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Plugin started!", MOD_NAME, VERSION);
	return AURIE_SUCCESS;
}
#include <fstream>
#include <algorithm>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <YYToolkit/YYTK_Shared.hpp> // YYTK v4
using namespace Aurie; 
using namespace YYTK;
using json = nlohmann::json;

static const char* const MOD_NAME = "MagicalGirlAri";
static const char* const VERSION = "1.0.0";
static const char* const EASTER_EGG_SET_PIECE_REQUIREMENT_KEY = "easter_egg_set_piece_requirement";
static const char* const BONUS_DAILY_MANA_SET_PIECE_REQUIREMENT_KEY = "bonus_daily_mana_set_piece_requirement";
static const char* const VANQUISH_ENEMY_SET_PIECE_REQUIREMENT_KEY = "vanquish_enemy_set_piece_requirement";
static const char* const HEALTH_RECOVERY_SET_PIECE_REQUIREMENT_KEY = "health_recovery_set_piece_requirement";
static const char* const GML_SCRIPT_MODIFY_HEALTH = "gml_Script_modify_health@Ari@Ari";
static const char* const GML_SCRIPT_HELD_ITEM = "gml_Script_held_item@Ari@Ari";
static const char* const GML_SCRIPT_DAMAGE = "gml_Script_damage@gml_Object_obj_damage_receiver_Create_0";
static const char* const GML_SCRIPT_PLAY_CONVERSATION = "gml_Script_play_conversation";
static const char* const GML_SCRIPT_NPC_RECEIVE_GIFT = "gml_Script_receive_gift@gml_Object_par_NPC_Create_0";
static const char* const GML_SCRIPT_GET_MANA = "gml_Script_get_mana@Ari@Ari";
static const char* const GML_SCRIPT_MODIFY_MANA = "gml_Script_modify_mana@Ari@Ari";
static const char* const GML_SCRIPT_GET_WEATHER = "gml_Script_get_weather@WeatherManager@Weather";
static const char* const GML_SCRIPT_ARI_ON_NEW_DAY = "gml_Script_on_new_day@Ari@Ari";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const std::string JUNIPER_EASTER_EGG_CONVERSATION_KEY = "Conversations/Mods/Magical Girl Ari/juniper_contract";
static const std::string SAILOR_MISTRIA_DRESS_ITEM_NAME = "sailor_mistria"; // Core set piece
static const std::string SAILOR_MISTRIA_HEELS_ITEM_NAME = "sailor_mistria_heels"; // Core set piece (interchangeable with boots)
static const std::string SAILOR_MISTRIA_BOOTS_ITEM_NAME = "sailor_mistria_boots"; // Core set piece (interchangeable with heels)
static const std::string PRINCESS_TWINTAILS_HAIR_NAME = "princess_twintails"; // Additional set piece
static const std::string SERENE_BUN_HAIR_NAME = "serene_bun"; // Additional set piece
static const std::string RINI_BUN_HAIR_NAME = "rini_bun"; // Additional set piece
static const std::string PRINCESS_BRIOCHE_HAIR_NAME = "princess_brioche"; // Additional set piece
static const std::string SAILOR_MISTRIA_PINS_ACCESSORY_NAME = "sailor_mistria_pins"; // Additional set piece
static const std::string SAILOR_MISTRIA_PINS_B_ACCESSORY_NAME = "sailor_mistria_pins_b"; // Additional set piece
static const std::string SAILOR_MISTRIA_PINS_C_ACCESSORY_NAME = "sailor_mistria_pins_c"; // Additional set piece
static const std::string HEART_SWORD_NAME = "sword_verdigris";

static const std::unordered_set<std::string> MAGICAL_GIRL_ARI_CORE_SET_PIECE_NAMES = { SAILOR_MISTRIA_DRESS_ITEM_NAME, SAILOR_MISTRIA_HEELS_ITEM_NAME, SAILOR_MISTRIA_BOOTS_ITEM_NAME };
static const std::unordered_set<std::string> MAGICAL_GIRL_ARI_ADDITIONAL_SET_PIECE_NAMES = { PRINCESS_TWINTAILS_HAIR_NAME, SERENE_BUN_HAIR_NAME, RINI_BUN_HAIR_NAME, SAILOR_MISTRIA_PINS_ACCESSORY_NAME, SAILOR_MISTRIA_PINS_B_ACCESSORY_NAME, SAILOR_MISTRIA_PINS_C_ACCESSORY_NAME };
static const std::string MANA_POTION_ITEM_NAME = "mana_potion";
static const int DEFAULT_EASTER_EGG_SET_PIECE_REQUIREMENT = 3;
static const int DEFAULT_BONUS_DAILY_MANA_SET_PIECE_REQUIREMENT = 3;
static const int DEFAULT_VANQUISH_ENEMY_SET_PIECE_REQUIREMENT = 3;
static const int DEFAULT_HEALTH_RECOVERY_SET_PIECE_REQUIREMENT = 3;

static YYTKInterface* g_ModuleInterface = nullptr;
static CInstance* global_instance = nullptr;
static bool load_on_start = true;
static bool game_is_active = false;
static bool is_new_day = false;
static bool modify_juniper_conversation = false;
static bool heart_sword_equipped = false;
static int ari_health_recovery = 0;
static int core_set_pieces_equipped = 0;
static int additional_set_pieces_equipped = 0;
static int original_heart_sword_damage = 10;
static int easter_egg_set_piece_requirement = DEFAULT_EASTER_EGG_SET_PIECE_REQUIREMENT;
static int bonus_daily_mana_set_piece_requirement = DEFAULT_BONUS_DAILY_MANA_SET_PIECE_REQUIREMENT;
static int vanquish_enemy_set_piece_requirement = DEFAULT_VANQUISH_ENEMY_SET_PIECE_REQUIREMENT;
static int health_recovery_set_piece_requirement = DEFAULT_HEALTH_RECOVERY_SET_PIECE_REQUIREMENT;
static int mana_potion_item_id = -1;
static int heart_sword_item_id = -1;
static std::map<int, int> spell_id_to_default_cost_map = {};
static std::map<std::string, int> monster_name_to_id_map = {};

void ResetStaticFields(bool returned_to_title_screen)
{
	if (returned_to_title_screen)
	{
		game_is_active = false;
		is_new_day = false;
		modify_juniper_conversation = false;
		heart_sword_equipped = false;
		ari_health_recovery = 0;
		core_set_pieces_equipped = 0;
		additional_set_pieces_equipped = 0;
	}
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

json CreateConfigJson(bool use_defaults)
{
	json config_json = {
		{ EASTER_EGG_SET_PIECE_REQUIREMENT_KEY, use_defaults ? DEFAULT_EASTER_EGG_SET_PIECE_REQUIREMENT : easter_egg_set_piece_requirement },
		{ BONUS_DAILY_MANA_SET_PIECE_REQUIREMENT_KEY, use_defaults ? DEFAULT_BONUS_DAILY_MANA_SET_PIECE_REQUIREMENT : bonus_daily_mana_set_piece_requirement },
		{ VANQUISH_ENEMY_SET_PIECE_REQUIREMENT_KEY, use_defaults ? DEFAULT_VANQUISH_ENEMY_SET_PIECE_REQUIREMENT : vanquish_enemy_set_piece_requirement },
		{ HEALTH_RECOVERY_SET_PIECE_REQUIREMENT_KEY, use_defaults ? DEFAULT_HEALTH_RECOVERY_SET_PIECE_REQUIREMENT : health_recovery_set_piece_requirement }
	};
	return config_json;
}

void LogDefaultConfigValues()
{
	easter_egg_set_piece_requirement = DEFAULT_EASTER_EGG_SET_PIECE_REQUIREMENT;
	bonus_daily_mana_set_piece_requirement = DEFAULT_BONUS_DAILY_MANA_SET_PIECE_REQUIREMENT;
	vanquish_enemy_set_piece_requirement = DEFAULT_VANQUISH_ENEMY_SET_PIECE_REQUIREMENT;
	health_recovery_set_piece_requirement = DEFAULT_HEALTH_RECOVERY_SET_PIECE_REQUIREMENT;

	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, EASTER_EGG_SET_PIECE_REQUIREMENT_KEY, DEFAULT_EASTER_EGG_SET_PIECE_REQUIREMENT);
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, BONUS_DAILY_MANA_SET_PIECE_REQUIREMENT_KEY, DEFAULT_BONUS_DAILY_MANA_SET_PIECE_REQUIREMENT);
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, VANQUISH_ENEMY_SET_PIECE_REQUIREMENT_KEY, DEFAULT_VANQUISH_ENEMY_SET_PIECE_REQUIREMENT);
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, HEALTH_RECOVERY_SET_PIECE_REQUIREMENT_KEY, DEFAULT_HEALTH_RECOVERY_SET_PIECE_REQUIREMENT);
}


void CreateOrLoadConfigFile()
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

		// Try to find the mod_data/MagicalGirlAri directory.
		std::string magical_girl_ari_folder = mod_data_folder + "\\MagicalGirlAri";
		if (!std::filesystem::exists(magical_girl_ari_folder))
		{
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"MagicalGirlAri\" directory was not found. Creating directory: %s", MOD_NAME, VERSION, magical_girl_ari_folder.c_str());
			std::filesystem::create_directory(magical_girl_ari_folder);
		}

		// Try to find the mod_data/MagicalGirlAri/MagicalGirlAri.json config file.
		bool update_config_file = false;
		std::string config_file = magical_girl_ari_folder + "\\" + "MagicalGirlAri.json";
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
					// Try loading the easter_egg_set_piece_requirement value.
					if (json_object.contains(EASTER_EGG_SET_PIECE_REQUIREMENT_KEY))
					{
						easter_egg_set_piece_requirement = json_object[EASTER_EGG_SET_PIECE_REQUIREMENT_KEY];
						if (easter_egg_set_piece_requirement < 2 || easter_egg_set_piece_requirement > 3)
						{
							g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Invalid \"%s\" value (%d) in mod configuration file: %s", MOD_NAME, VERSION, EASTER_EGG_SET_PIECE_REQUIREMENT_KEY, easter_egg_set_piece_requirement, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Make sure the value is a valid integer between 2 and 3 (inclusive)!", MOD_NAME, VERSION);
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, EASTER_EGG_SET_PIECE_REQUIREMENT_KEY, DEFAULT_EASTER_EGG_SET_PIECE_REQUIREMENT);
							easter_egg_set_piece_requirement = DEFAULT_EASTER_EGG_SET_PIECE_REQUIREMENT;
						}
						else
						{
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %d!", MOD_NAME, VERSION, EASTER_EGG_SET_PIECE_REQUIREMENT_KEY, easter_egg_set_piece_requirement);
						}
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, EASTER_EGG_SET_PIECE_REQUIREMENT_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, EASTER_EGG_SET_PIECE_REQUIREMENT_KEY, DEFAULT_EASTER_EGG_SET_PIECE_REQUIREMENT);
					}

					// Try loading the bonus_daily_mana_set_piece_requirement value.
					if (json_object.contains(BONUS_DAILY_MANA_SET_PIECE_REQUIREMENT_KEY))
					{
						bonus_daily_mana_set_piece_requirement = json_object[BONUS_DAILY_MANA_SET_PIECE_REQUIREMENT_KEY];
						if (bonus_daily_mana_set_piece_requirement < 2 || bonus_daily_mana_set_piece_requirement > 3)
						{
							g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Invalid \"%s\" value (%d) in mod configuration file: %s", MOD_NAME, VERSION, BONUS_DAILY_MANA_SET_PIECE_REQUIREMENT_KEY, bonus_daily_mana_set_piece_requirement, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Make sure the value is a valid integer between 2 and 3 (inclusive)!", MOD_NAME, VERSION);
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, BONUS_DAILY_MANA_SET_PIECE_REQUIREMENT_KEY, DEFAULT_BONUS_DAILY_MANA_SET_PIECE_REQUIREMENT);
							bonus_daily_mana_set_piece_requirement = DEFAULT_BONUS_DAILY_MANA_SET_PIECE_REQUIREMENT;
						}
						else
						{
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %d!", MOD_NAME, VERSION, BONUS_DAILY_MANA_SET_PIECE_REQUIREMENT_KEY, bonus_daily_mana_set_piece_requirement);
						}
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, BONUS_DAILY_MANA_SET_PIECE_REQUIREMENT_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, BONUS_DAILY_MANA_SET_PIECE_REQUIREMENT_KEY, DEFAULT_BONUS_DAILY_MANA_SET_PIECE_REQUIREMENT);
					}

					// Try loading the vanquish_enemy_set_piece_requirement value.
					if (json_object.contains(VANQUISH_ENEMY_SET_PIECE_REQUIREMENT_KEY))
					{
						vanquish_enemy_set_piece_requirement = json_object[VANQUISH_ENEMY_SET_PIECE_REQUIREMENT_KEY];
						if (vanquish_enemy_set_piece_requirement < 0 || vanquish_enemy_set_piece_requirement > 3)
						{
							g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Invalid \"%s\" value (%d) in mod configuration file: %s", MOD_NAME, VERSION, VANQUISH_ENEMY_SET_PIECE_REQUIREMENT_KEY, vanquish_enemy_set_piece_requirement, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Make sure the value is a valid integer between 0 and 3 (inclusive)!", MOD_NAME, VERSION);
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, VANQUISH_ENEMY_SET_PIECE_REQUIREMENT_KEY, DEFAULT_VANQUISH_ENEMY_SET_PIECE_REQUIREMENT);
							vanquish_enemy_set_piece_requirement = DEFAULT_VANQUISH_ENEMY_SET_PIECE_REQUIREMENT;
						}
						else
						{
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %d!", MOD_NAME, VERSION, VANQUISH_ENEMY_SET_PIECE_REQUIREMENT_KEY, vanquish_enemy_set_piece_requirement);
						}
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, VANQUISH_ENEMY_SET_PIECE_REQUIREMENT_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, VANQUISH_ENEMY_SET_PIECE_REQUIREMENT_KEY, DEFAULT_VANQUISH_ENEMY_SET_PIECE_REQUIREMENT);
					}

					// Try loading the health_recovery_set_piece_requirement value.
					if (json_object.contains(HEALTH_RECOVERY_SET_PIECE_REQUIREMENT_KEY))
					{
						health_recovery_set_piece_requirement = json_object[HEALTH_RECOVERY_SET_PIECE_REQUIREMENT_KEY];
						if (health_recovery_set_piece_requirement < 0 || health_recovery_set_piece_requirement > 3)
						{
							g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Invalid \"%s\" value (%d) in mod configuration file: %s", MOD_NAME, VERSION, HEALTH_RECOVERY_SET_PIECE_REQUIREMENT_KEY, health_recovery_set_piece_requirement, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Make sure the value is a valid integer between 0 and 3 (inclusive)!", MOD_NAME, VERSION);
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, HEALTH_RECOVERY_SET_PIECE_REQUIREMENT_KEY, DEFAULT_HEALTH_RECOVERY_SET_PIECE_REQUIREMENT);
							health_recovery_set_piece_requirement = DEFAULT_HEALTH_RECOVERY_SET_PIECE_REQUIREMENT;
						}
						else
						{
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %d!", MOD_NAME, VERSION, HEALTH_RECOVERY_SET_PIECE_REQUIREMENT_KEY, health_recovery_set_piece_requirement);
						}
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, HEALTH_RECOVERY_SET_PIECE_REQUIREMENT_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, HEALTH_RECOVERY_SET_PIECE_REQUIREMENT_KEY, DEFAULT_HEALTH_RECOVERY_SET_PIECE_REQUIREMENT);
					}
				}

				update_config_file = true;
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

			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"MagicalGirlAri.json\" file was not found. Creating file: %s", MOD_NAME, VERSION, config_file.c_str());

			json default_config_json = CreateConfigJson(true);
			std::ofstream out_stream(config_file);
			out_stream << std::setw(4) << default_config_json << std::endl;
			out_stream.close();

			LogDefaultConfigValues();
		}

		if (update_config_file)
		{
			json config_json = CreateConfigJson(false);
			std::ofstream out_stream(config_file);
			out_stream << std::setw(4) << config_json << std::endl;
			out_stream.close();
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


void LoadItems()
{
	size_t array_length = 0;
	RValue item_id_array = global_instance->GetMember("__item_id__");
	g_ModuleInterface->GetArraySize(item_id_array, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(item_id_array, i, array_element);

		std::string item_name = array_element->ToString();
		if (item_name == MANA_POTION_ITEM_NAME)
			mana_potion_item_id = i;

		if (item_name == HEART_SWORD_NAME)
			heart_sword_item_id = i;
	}
}

void LoadOriginalHeartSwordDamage()
{
	RValue item_data = global_instance->GetMember("__item_data");
	
	RValue* heart_sword;
	g_ModuleInterface->GetArrayEntry(item_data, heart_sword_item_id, heart_sword);

	original_heart_sword_damage = heart_sword->GetMember("damage").ToInt64();
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

void LoadMonsters()
{
	size_t array_length;
	RValue monster_names = global_instance->GetMember("__monster_id__");
	g_ModuleInterface->GetArraySize(monster_names, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* monster_name;
		g_ModuleInterface->GetArrayEntry(monster_names, i, monster_name);

		monster_name_to_id_map[monster_name->ToString()] = i;
	}
}

void ModifySpellCosts(int mana_cost_reduction) {
	size_t array_length = 0;
	RValue spells = global_instance->GetMember("__spells");
	g_ModuleInterface->GetArraySize(spells, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(spells, i, array_element);

		int cost = (std::max)(0, spell_id_to_default_cost_map[i] - mana_cost_reduction);
		*array_element->GetRefMember("cost") = cost;
	}
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

RValue GetMana(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_get_mana = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_MANA,
		(PVOID*)&gml_script_get_mana
	);

	RValue result;
	gml_script_get_mana->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		0,
		nullptr
	);

	return result;
}

void ScaleHeartSword(int current_mana)
{
	RValue item_data = *global_instance->GetRefMember("__item_data");

	RValue* heart_sword;
	g_ModuleInterface->GetArrayEntry(item_data, heart_sword_item_id, heart_sword);

	double damage = original_heart_sword_damage + current_mana;
	*heart_sword->GetRefMember("damage") = damage;
}

std::tuple<int, int> CountMagicalGirlAriCosmeticsEquipped()
{
	RValue __ari = *global_instance->GetRefMember("__ari");
	RValue preset_index_selected = *__ari.GetRefMember("preset_index_selected");
	RValue presets = *__ari.GetRefMember("presets");
	RValue buffer = *presets.GetRefMember("__buffer");

	RValue* selelected_buffer_entry; // Ari's current cosmetics
	g_ModuleInterface->GetArrayEntry(buffer, preset_index_selected.m_Real, selelected_buffer_entry);

	RValue assets = *selelected_buffer_entry->GetRefMember("assets");
	RValue inner_buffer = *assets.GetRefMember("__buffer");
	size_t inner_buffer_size;
	g_ModuleInterface->GetArraySize(inner_buffer, inner_buffer_size);

	int core_set_piece_cosmetics_equipped = 0;
	int additional_set_piece_cosmetics_equipped = 0;

	for (int i = 0; i < inner_buffer_size; i++)
	{
		RValue* equipped_cosmetic;
		g_ModuleInterface->GetArrayEntry(inner_buffer, i, equipped_cosmetic);
		RValue equipped_cosmetic_name = *equipped_cosmetic->GetRefMember("name");

		if (MAGICAL_GIRL_ARI_CORE_SET_PIECE_NAMES.contains(equipped_cosmetic_name.ToString()))
			core_set_piece_cosmetics_equipped++;

		if (MAGICAL_GIRL_ARI_ADDITIONAL_SET_PIECE_NAMES.contains(equipped_cosmetic_name.ToString()))
			additional_set_piece_cosmetics_equipped++;
	}

	std::tuple<int, int> tuple = { core_set_piece_cosmetics_equipped, additional_set_piece_cosmetics_equipped };
	return tuple;
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

	if (!game_is_active)
		return;

	if (strstr(self->m_Object->m_Name, "obj_ari"))
	{
		int mana = GetMana(global_instance->GetRefMember("__ari")->ToInstance(), self).ToInt64();
		ScaleHeartSword(mana);

		// Track equipped gear.
		std::tuple<int, int> tuple = CountMagicalGirlAriCosmeticsEquipped();
		core_set_pieces_equipped = std::get<0>(tuple);
		additional_set_pieces_equipped = std::get<1>(tuple);
		
		// Adjust spell costs.
		ModifySpellCosts(core_set_pieces_equipped > 2 ? 2 : core_set_pieces_equipped);
		
		// Process health recovery.
		if (ari_health_recovery > 0)
		{
			ModifyHealth(global_instance->GetRefMember("__ari")->ToInstance(), self, ari_health_recovery);
			ari_health_recovery = 0;
		}
	}

	if (strstr(self->m_Object->m_Name, "obj_monster"))
	{
		RValue monster = self->ToRValue();
		if (StructVariableExists(monster, "monster_id"))
		{
			RValue monster_id = *monster.GetRefMember("monster_id");

			bool is_valid_monster_object = false;
			for (const auto& entry : monster_name_to_id_map) {
				if (entry.second == monster_id.ToInt64()) {
					is_valid_monster_object = true;
					break;
				}
			}

			if (!is_valid_monster_object)
				return;

			if (StructVariableExists(monster, "config") && StructVariableExists(monster, "hit_points"))
			{
				RValue config = *monster.GetRefMember("config");
				RValue hit_points = monster.GetMember("hit_points");

				// Vanquish Enemy
				if (heart_sword_equipped && (core_set_pieces_equipped + additional_set_pieces_equipped) >= vanquish_enemy_set_piece_requirement)
				{
					if (!StructVariableExists(monster, "__magical_girl_ari__vanquish_hit_points"))
						StructVariableSet(monster, "__magical_girl_ari__vanquish_hit_points", hit_points);

					RValue original_hit_points = monster.GetMember("__magical_girl_ari__vanquish_hit_points");
					if (hit_points.ToDouble() != original_hit_points.ToDouble())
						*monster.GetRefMember("hit_points") = 0;
				}

				// Health Recovery
				if (heart_sword_equipped && (core_set_pieces_equipped + additional_set_pieces_equipped) >= health_recovery_set_piece_requirement)
				{
					if (!StructVariableExists(monster, "__magical_girl_ari__processed_monster_death"))
					{
						if (hit_points.ToInt64() <= 0)
						{
							g_ModuleInterface->CallBuiltin("struct_set", { monster, "__magical_girl_ari__processed_monster_death", true });
							ari_health_recovery += 10;
						}
					}
				}
			}
		}
	}
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
		if (item_id == heart_sword_item_id)
			heart_sword_equipped = true;
		else
			heart_sword_equipped = false;
	}
	else
		heart_sword_equipped = false;

	return Result;
}

RValue& GmlScriptDamageCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	// Vanquish Enemy
	if (heart_sword_equipped && (core_set_pieces_equipped + additional_set_pieces_equipped) >= vanquish_enemy_set_piece_requirement)
	{
		RValue target = Arguments[0]->GetMember("target");
		if (target.ToInt64() != 1) // Everything not Ari
		{
			double damage = Arguments[0]->GetMember("damage").ToDouble();
			if (damage != 0) // Not a miss
			{
				*Arguments[0]->GetRefMember("critical") = true;
				*Arguments[0]->GetRefMember("damage") = 9999.0;
			}
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

RValue& GmlScriptPlayConversationCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (game_is_active)
	{
		std::string conversation_name = Arguments[1]->ToString();
		if (conversation_name.contains("gift_lines") && conversation_name.contains("Conversations/Bank/Juniper"))
		{
			if (modify_juniper_conversation)
			{
				modify_juniper_conversation = false;
				*Arguments[1] = RValue(JUNIPER_EASTER_EGG_CONVERSATION_KEY);
			}
		}
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

RValue& GmlScriptNpcReceiveGiftCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (Self != NULL && Self->m_Object != NULL)
	{
		std::string npc_name = Self->m_Object->m_Name;
		if (npc_name == "obj_juniper")
		{
			if (ArgumentCount == 0 || ArgumentCount > 1)
				g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Expected 1 argument but got %d for %s!", MOD_NAME, VERSION, ArgumentCount, GML_SCRIPT_NPC_RECEIVE_GIFT);

			if (ArgumentCount == 1 && Arguments[0]->m_Kind == VALUE_OBJECT)
			{
				RValue item_id = Arguments[0]->GetMember("item_id");
				if (item_id.ToInt64() == mana_potion_item_id)
				{
					if ((core_set_pieces_equipped + additional_set_pieces_equipped) >= easter_egg_set_piece_requirement)
						modify_juniper_conversation = true;
				}
			}
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_NPC_RECEIVE_GIFT));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptModifyManaCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (is_new_day)
	{
		if ((core_set_pieces_equipped + additional_set_pieces_equipped) >= bonus_daily_mana_set_piece_requirement)
		{
			double mana_modifier = Arguments[0]->m_Real;
			Arguments[0]->m_Real = mana_modifier + 1;
		}

		is_new_day = false;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_MODIFY_MANA));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

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

RValue& GmlScriptAriOnNewDayCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	is_new_day = true;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_ARI_ON_NEW_DAY));
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
		g_ModuleInterface->GetGlobalInstance(&global_instance);
		CreateOrLoadConfigFile();
		LoadSpells();
		LoadMonsters();
		LoadItems();
		LoadOriginalHeartSwordDamage();
		load_on_start = false;
	}

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

void CreateHookGmlScriptNpcReceiveGift(AurieStatus& status)
{
	CScript* gml_script_npc_receive_gift = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_NPC_RECEIVE_GIFT,
		(PVOID*)&gml_script_npc_receive_gift
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_NPC_RECEIVE_GIFT);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_NPC_RECEIVE_GIFT,
		gml_script_npc_receive_gift->m_Functions->m_ScriptFunction,
		GmlScriptNpcReceiveGiftCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_NPC_RECEIVE_GIFT);
	}
}

void CreateHookGmlScriptModifyMana(AurieStatus& status)
{
	CScript* gml_script_modify_mana = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_MODIFY_MANA,
		(PVOID*)&gml_script_modify_mana
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_MODIFY_MANA);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_MODIFY_MANA,
		gml_script_modify_mana->m_Functions->m_ScriptFunction,
		GmlScriptModifyManaCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_MODIFY_MANA);
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

void CreateHookGmlScriptAriOnNewDay(AurieStatus& status)
{
	CScript* gml_script_ari_on_new_day = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_ARI_ON_NEW_DAY,
		(PVOID*)&gml_script_ari_on_new_day
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ARI_ON_NEW_DAY);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_ARI_ON_NEW_DAY,
		gml_script_ari_on_new_day->m_Functions->m_ScriptFunction,
		GmlScriptAriOnNewDayCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ARI_ON_NEW_DAY);
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

	CreateHookGmlScriptHeldItem(status);
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

	CreateHookGmlScriptPlayConversation(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptNpcReceiveGift(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptModifyMana(status);
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

	CreateHookGmlScriptAriOnNewDay(status);
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

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Plugin started!", MOD_NAME, VERSION);
	return AURIE_SUCCESS;
}
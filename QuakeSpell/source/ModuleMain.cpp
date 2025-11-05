#include <random>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <YYToolkit/YYTK_Shared.hpp> // YYTK v4
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

static const char* const FULL_RESTORE_VARIANT_NAME = "Full Restore";
static const char* const GROWTH_SPELL_VARIANT_NAME = "Growth Spell";
static const char* const SUMMON_RAIN_VARIANT_NAME = "Summon Rain";
static const char* const FIRE_BREATH_VARIANT_NAME = "Fire Breath";

// Mod metadata
static const char* const VERSION = "1.2.0";
static const char* const MOD_NAME = "QuakeSpell";
static const char* const MOD_VARIANT = FULL_RESTORE_VARIANT_NAME; // CHANGE PER VARIANT BUILD!

static const char* const SPELL_COST_KEY = "spell_cost";
static const char* const DAMAGE_ARI_KEY = "damage_ari";
static const char* const IGNORE_MIMICS_KEY = "ignore_mimics";
static const char* const SOUND_EFFECTS_KEY = "sound_effects";
static const char* const SCREEN_FLASH_KEY = "screen_flash";
static const char* const AFTERBURN_KEY = "afterburn";
static const char* const ONE_SHOT_MONSTERS_KEY = "one_shot_monsters";
static const char* const MINIMUM_MAGNITUDE_KEY = "minimum_magnitude";
static const char* const MAXIMUM_MAGNITUDE_KEY = "maximum_magnitude";
static const int DEFAULT_SPELL_COST_VALUE = 2;
static const bool DEFAULT_DAMAGE_ARI_VALUE = true;
static const bool DEFAULT_IGNORE_MIMICS_VALUE = false;
static const bool DEFAULT_SOUND_EFFECTS_VALUE = true;
static const bool DEFAULT_SCREEN_FLASH_VALUE = true;
static const bool DEFAULT_AFTERBURN_VALUE = true;
static const bool DEFAULT_ONE_SHOT_MONSTERS_VALUE = true;
static const int DEFAULT_MINIMUM_MAGNITUDE_VALUE = 5;
static const int DEFAULT_MAXIMUM_MAGNITUDE_VALUE = 9;
static const char* const FULL_RESTORE_SPELL = "full_restore";
static const char* const GROWTH_SPELL = "growth";
static const char* const SUMMON_RAIN_SPELL = "summon_rain";
static const char* const FIRE_BREATH_SPELL = "fire_breath";
static std::map<std::string, std::string> MOD_VARIANT_SPELL_NAME_MAP = {
	{ FULL_RESTORE_VARIANT_NAME, FULL_RESTORE_SPELL },
	{ GROWTH_SPELL_VARIANT_NAME, GROWTH_SPELL },
	{ SUMMON_RAIN_VARIANT_NAME, SUMMON_RAIN_SPELL },
	{ FIRE_BREATH_VARIANT_NAME, FIRE_BREATH_SPELL },
};
static const char* const GML_SCRIPT_SET_PINNED_SPELL = "gml_Script_set_pinned_spell@Ari@Ari";
static const char* const GML_SCRIPT_CAN_CAST_SPELL = "gml_Script_can_cast_spell";
static const char* const GML_SCRIPT_CAST_SPELL = "gml_Script_cast_spell";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const char* const GML_SCRIPT_TRY_END_OF_DAY_STATUS_TO_STRING = "gml_Script_try_end_of_day_status_to_string";
static const char* const GML_SCRIPT_SHOW_ROOM_TITLE = "gml_Script_show_room_title";
static const char* const GML_SCRIPT_GET_MANA = "gml_Script_get_mana@Ari@Ari";
static const char* const GML_SCRIPT_MODIFY_MANA = "gml_Script_modify_mana@Ari@Ari";
static const char* const GML_SCRIPT_ON_DRAW_GUI = "gml_Script_on_draw_gui@Display@Display";
static const char* const GML_SCRIPT_TRY_LOCATION_ID_TO_STRING = "gml_Script_try_location_id_to_string";
static const char* const GML_SCRIPT_FADE_OUT = "gml_Script_fade_out@ScreenFaderMenu@ScreenFaderMenu";
static const char* const GML_SCRIPT_SAVE_GAME = "gml_Script_save_game";

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_on_start = true;
static int spell_cost = DEFAULT_SPELL_COST_VALUE;
static bool damage_ari_option = DEFAULT_DAMAGE_ARI_VALUE;
static bool ignore_mimics_option = DEFAULT_IGNORE_MIMICS_VALUE;
static bool sound_effects_option = DEFAULT_SOUND_EFFECTS_VALUE;
static bool screen_flash_option = DEFAULT_SCREEN_FLASH_VALUE;
static bool afterburn_option = DEFAULT_AFTERBURN_VALUE;
static bool one_shot_monsters = DEFAULT_ONE_SHOT_MONSTERS_VALUE;
static int minimum_magnitude = DEFAULT_MINIMUM_MAGNITUDE_VALUE;
static int maximum_magnitude = DEFAULT_MAXIMUM_MAGNITUDE_VALUE;

static double ari_current_mana = -1.0;
static int pinned_spell = -1;
static int quake_magnitude = 0;
static bool screen_flash = false;
static bool screen_flash_initialized = false;
static uint64_t screen_flash_start_time = 0;
static bool rumble_sound = false;
static bool rumble_sound_initialized = false;
static uint64_t rumble_sound_start_time = 0;
static bool quake_spell_active = false;
static bool afterburn = false;
static bool room_loaded = false;
static bool modify_mana = false;
static bool modify_health = false;
static bool in_dungeon = false;
static std::mt19937 generator(std::random_device{}());
static std::map<std::string, int64_t> spell_name_to_id_map = {};
static std::map<std::string, int64_t> status_effect_name_to_id_map = {};

void PrintError(std::exception_ptr eptr)
{
	try {
		if (eptr) {
			std::rethrow_exception(eptr);
		}
	}
	catch (const std::exception& e) {
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Error: %s", MOD_NAME, MOD_VARIANT, VERSION, e.what());
	}
}

json CreateConfigJson(bool use_defaults)
{
	json config_json = {
		{ SPELL_COST_KEY, use_defaults ? DEFAULT_SPELL_COST_VALUE : spell_cost },
		{ DAMAGE_ARI_KEY, use_defaults ? DEFAULT_DAMAGE_ARI_VALUE : damage_ari_option },
		{ IGNORE_MIMICS_KEY, use_defaults ? DEFAULT_IGNORE_MIMICS_VALUE : ignore_mimics_option },
		{ SOUND_EFFECTS_KEY, use_defaults ? DEFAULT_SOUND_EFFECTS_VALUE : sound_effects_option },
		{ SCREEN_FLASH_KEY, use_defaults ? DEFAULT_SCREEN_FLASH_VALUE : screen_flash_option },
		{ AFTERBURN_KEY, use_defaults ? DEFAULT_AFTERBURN_VALUE : afterburn_option },
		{ ONE_SHOT_MONSTERS_KEY, use_defaults ? DEFAULT_ONE_SHOT_MONSTERS_VALUE : one_shot_monsters },
		{ MINIMUM_MAGNITUDE_KEY, use_defaults ? DEFAULT_MINIMUM_MAGNITUDE_VALUE : minimum_magnitude },
		{ MAXIMUM_MAGNITUDE_KEY, use_defaults ? DEFAULT_MAXIMUM_MAGNITUDE_VALUE : maximum_magnitude }
	};
	return config_json;
}

void LogDefaultConfigValues()
{
	spell_cost = DEFAULT_SPELL_COST_VALUE;
	damage_ari_option = DEFAULT_DAMAGE_ARI_VALUE;
	ignore_mimics_option = DEFAULT_IGNORE_MIMICS_VALUE;
	sound_effects_option = DEFAULT_SOUND_EFFECTS_VALUE;
	screen_flash_option = DEFAULT_SCREEN_FLASH_VALUE;
	afterburn_option = DEFAULT_AFTERBURN_VALUE;
	one_shot_monsters = DEFAULT_ONE_SHOT_MONSTERS_VALUE;
	minimum_magnitude = DEFAULT_MINIMUM_MAGNITUDE_VALUE;
	maximum_magnitude = DEFAULT_MAXIMUM_MAGNITUDE_VALUE;

	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, MOD_VARIANT, VERSION, SPELL_COST_KEY, DEFAULT_SPELL_COST_VALUE);
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, MOD_VARIANT, VERSION, DAMAGE_ARI_KEY, DEFAULT_DAMAGE_ARI_VALUE ? "true" : "false");
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, MOD_VARIANT, VERSION, IGNORE_MIMICS_KEY, DEFAULT_IGNORE_MIMICS_VALUE ? "true" : "false");
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, MOD_VARIANT, VERSION, SOUND_EFFECTS_KEY, DEFAULT_SOUND_EFFECTS_VALUE ? "true" : "false");
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, MOD_VARIANT, VERSION, SCREEN_FLASH_KEY, DEFAULT_SCREEN_FLASH_VALUE ? "true" : "false");
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, MOD_VARIANT, VERSION, AFTERBURN_KEY, DEFAULT_AFTERBURN_VALUE ? "true" : "false");
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, MOD_VARIANT, VERSION, ONE_SHOT_MONSTERS_KEY, DEFAULT_ONE_SHOT_MONSTERS_VALUE ? "true" : "false");
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, MOD_VARIANT, VERSION, MINIMUM_MAGNITUDE_KEY, DEFAULT_MINIMUM_MAGNITUDE_VALUE);
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, MOD_VARIANT, VERSION, MAXIMUM_MAGNITUDE_KEY, DEFAULT_MAXIMUM_MAGNITUDE_VALUE);
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
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - The \"mod_data\" directory was not found. Creating directory: %s", MOD_NAME, MOD_VARIANT, VERSION, mod_data_folder.c_str());
			std::filesystem::create_directory(mod_data_folder);
		}

		// Try to find the mod_data/QuakeSpell directory.
		std::string quake_spell_folder = mod_data_folder + "\\QuakeSpell";
		if (!std::filesystem::exists(quake_spell_folder))
		{
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "%s (%s) %s] - The \"QuakeSpell\" directory was not found. Creating directory: %s", MOD_NAME, MOD_VARIANT, VERSION, quake_spell_folder.c_str());
			std::filesystem::create_directory(quake_spell_folder);
		}

		// Try to find the mod_data/QuakeSpell/QuakeSpell.json config file.
		bool update_config_file = false;
		std::string config_file = quake_spell_folder + "\\" + "QuakeSpell.json";
		std::ifstream in_stream(config_file);
		if (in_stream.good())
		{
			try
			{
				json json_object = json::parse(in_stream);

				// Check if the json_object is empty.
				if (json_object.empty())
				{
					g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - No values found in mod configuration file: %s!", MOD_NAME, MOD_VARIANT, VERSION, config_file.c_str());
					g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Add your desired values to the configuration file, otherwise defaults will be used.", MOD_NAME, MOD_VARIANT, VERSION);
					LogDefaultConfigValues();
				}
				else
				{
					// Try loading the spell_cost value.
					if (json_object.contains(SPELL_COST_KEY))
					{
						spell_cost = json_object[SPELL_COST_KEY];
						if (spell_cost < 0 || spell_cost > 12)
						{
							g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Invalid \"%s\" value (%d) in mod configuration file: %s", MOD_NAME, MOD_VARIANT, VERSION, SPELL_COST_KEY, spell_cost, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - This value MUST be a valid integer between 0 and 12 (inclusive)!", MOD_NAME, MOD_VARIANT, VERSION);
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, MOD_VARIANT, VERSION, SPELL_COST_KEY, DEFAULT_SPELL_COST_VALUE);
							spell_cost = DEFAULT_SPELL_COST_VALUE;
						}
						else
						{
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using CUSTOM \"%s\" value: %d!", MOD_NAME, MOD_VARIANT, VERSION, SPELL_COST_KEY, spell_cost);
						}
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, MOD_VARIANT, VERSION, SPELL_COST_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, MOD_VARIANT, VERSION, SPELL_COST_KEY, DEFAULT_SPELL_COST_VALUE);
						spell_cost = DEFAULT_SPELL_COST_VALUE;
					}

					// Try loading the damage_ari value.
					if (json_object.contains(DAMAGE_ARI_KEY))
					{
						damage_ari_option = json_object[DAMAGE_ARI_KEY];
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using CUSTOM \"%s\" value: %s!", MOD_NAME, MOD_VARIANT, VERSION, DAMAGE_ARI_KEY, damage_ari_option ? "true" : "false");
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, MOD_VARIANT, VERSION, DAMAGE_ARI_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, MOD_VARIANT, VERSION, DAMAGE_ARI_KEY, DEFAULT_DAMAGE_ARI_VALUE ? "true" : "false");
						damage_ari_option = DEFAULT_DAMAGE_ARI_VALUE;
					}

					// Try loading the ignore_mimics value.
					if (json_object.contains(IGNORE_MIMICS_KEY))
					{
						ignore_mimics_option = json_object[IGNORE_MIMICS_KEY];
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using CUSTOM \"%s\" value: %s!", MOD_NAME, MOD_VARIANT, VERSION, IGNORE_MIMICS_KEY, ignore_mimics_option ? "true" : "false");
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, MOD_VARIANT, VERSION, IGNORE_MIMICS_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, MOD_VARIANT, VERSION, IGNORE_MIMICS_KEY, DEFAULT_IGNORE_MIMICS_VALUE ? "true" : "false");
						ignore_mimics_option = DEFAULT_IGNORE_MIMICS_VALUE;
					}

					// Try loading the sound_effects value.
					if (json_object.contains(SOUND_EFFECTS_KEY))
					{
						sound_effects_option = json_object[SOUND_EFFECTS_KEY];
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using CUSTOM \"%s\" value: %s!", MOD_NAME, MOD_VARIANT, VERSION, SOUND_EFFECTS_KEY, sound_effects_option ? "true" : "false");
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, MOD_VARIANT, VERSION, SOUND_EFFECTS_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, MOD_VARIANT, VERSION, SOUND_EFFECTS_KEY, DEFAULT_SOUND_EFFECTS_VALUE ? "true" : "false");
						sound_effects_option = DEFAULT_SOUND_EFFECTS_VALUE;
					}

					// Try loading the screen_flash value.
					if (json_object.contains(SCREEN_FLASH_KEY))
					{
						screen_flash_option = json_object[SCREEN_FLASH_KEY];
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using CUSTOM \"%s\" value: %s!", MOD_NAME, MOD_VARIANT, VERSION, SCREEN_FLASH_KEY, screen_flash_option ? "true" : "false");
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, MOD_VARIANT, VERSION, SCREEN_FLASH_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, MOD_VARIANT, VERSION, SCREEN_FLASH_KEY, DEFAULT_SCREEN_FLASH_VALUE ? "true" : "false");
						screen_flash_option = DEFAULT_SCREEN_FLASH_VALUE;
					}

					// Try loading the afterburn value.
					if (json_object.contains(AFTERBURN_KEY))
					{
						afterburn_option = json_object[AFTERBURN_KEY];
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using CUSTOM \"%s\" value: %s!", MOD_NAME, MOD_VARIANT, VERSION, AFTERBURN_KEY, afterburn_option ? "true" : "false");
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, MOD_VARIANT, VERSION, AFTERBURN_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, MOD_VARIANT, VERSION, AFTERBURN_KEY, DEFAULT_AFTERBURN_VALUE ? "true" : "false");
						afterburn_option = DEFAULT_AFTERBURN_VALUE;
					}

					// Try loading the one_shot_monsters value.
					if (json_object.contains(ONE_SHOT_MONSTERS_KEY))
					{
						one_shot_monsters = json_object[ONE_SHOT_MONSTERS_KEY];
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using CUSTOM \"%s\" value: %s!", MOD_NAME, MOD_VARIANT, VERSION, ONE_SHOT_MONSTERS_KEY, one_shot_monsters ? "true" : "false");
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, MOD_VARIANT, VERSION, ONE_SHOT_MONSTERS_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, MOD_VARIANT, VERSION, ONE_SHOT_MONSTERS_KEY, DEFAULT_ONE_SHOT_MONSTERS_VALUE ? "true" : "false");
						one_shot_monsters = DEFAULT_ONE_SHOT_MONSTERS_VALUE;
					}

					// Try loading the minimum_magnitude value.
					if (json_object.contains(MINIMUM_MAGNITUDE_KEY))
					{
						minimum_magnitude = json_object[MINIMUM_MAGNITUDE_KEY];
						if (minimum_magnitude < 1 || minimum_magnitude > 9)
						{
							g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Invalid \"%s\" value (%d) in mod configuration file: %s", MOD_NAME, MOD_VARIANT, VERSION, MINIMUM_MAGNITUDE_KEY, minimum_magnitude, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - This value MUST be a valid integer between 1 and 9 (inclusive)!", MOD_NAME, MOD_VARIANT, VERSION);
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, MOD_VARIANT, VERSION, MINIMUM_MAGNITUDE_KEY, DEFAULT_MINIMUM_MAGNITUDE_VALUE);
							minimum_magnitude = DEFAULT_MINIMUM_MAGNITUDE_VALUE;
						}
						else
						{
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using CUSTOM \"%s\" value: %d!", MOD_NAME, MOD_VARIANT, VERSION, MINIMUM_MAGNITUDE_KEY, minimum_magnitude);
						}
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, MOD_VARIANT, VERSION, MINIMUM_MAGNITUDE_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, MOD_VARIANT, VERSION, MINIMUM_MAGNITUDE_KEY, DEFAULT_MINIMUM_MAGNITUDE_VALUE);
						minimum_magnitude = DEFAULT_MINIMUM_MAGNITUDE_VALUE;
					}

					// Try loading the maximum_magnitude value.
					if (json_object.contains(MAXIMUM_MAGNITUDE_KEY))
					{
						maximum_magnitude = json_object[MAXIMUM_MAGNITUDE_KEY];
						if (maximum_magnitude < 1 || maximum_magnitude > 9)
						{
							g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Invalid \"%s\" value (%d) in mod configuration file: %s", MOD_NAME, MOD_VARIANT, VERSION, MAXIMUM_MAGNITUDE_KEY, maximum_magnitude, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - This value MUST be a valid integer between 1 and 9 (inclusive)!", MOD_NAME, MOD_VARIANT, VERSION);
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, MOD_VARIANT, VERSION, MAXIMUM_MAGNITUDE_KEY, DEFAULT_MAXIMUM_MAGNITUDE_VALUE);
							maximum_magnitude = DEFAULT_MAXIMUM_MAGNITUDE_VALUE;
						}
						else
						{
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using CUSTOM \"%s\" value: %d!", MOD_NAME, MOD_VARIANT, VERSION, MAXIMUM_MAGNITUDE_KEY, maximum_magnitude);
						}
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, MOD_VARIANT, VERSION, MAXIMUM_MAGNITUDE_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, MOD_VARIANT, VERSION, MAXIMUM_MAGNITUDE_KEY, DEFAULT_MAXIMUM_MAGNITUDE_VALUE);
						maximum_magnitude = DEFAULT_MAXIMUM_MAGNITUDE_VALUE;
					}

					// Verify minimum_magnitude is less than maximum_magnitude.
					if (minimum_magnitude > maximum_magnitude)
					{
						g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - The \"%s\" value (%d) MUST be less than or equal to the \"%s\" value (%d)!", MOD_NAME, MOD_VARIANT, VERSION, MINIMUM_MAGNITUDE_KEY, minimum_magnitude, MAXIMUM_MAGNITUDE_KEY, maximum_magnitude);
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, MOD_VARIANT, VERSION, MINIMUM_MAGNITUDE_KEY, DEFAULT_MINIMUM_MAGNITUDE_VALUE);
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, MOD_VARIANT, VERSION, MAXIMUM_MAGNITUDE_KEY, DEFAULT_MAXIMUM_MAGNITUDE_VALUE);
						minimum_magnitude = DEFAULT_MINIMUM_MAGNITUDE_VALUE;
						maximum_magnitude = DEFAULT_MAXIMUM_MAGNITUDE_VALUE;
					}
				}

				update_config_file = true;
			}
			catch (...)
			{
				eptr = std::current_exception();
				PrintError(eptr);

				g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to parse JSON from configuration file: %s", MOD_NAME, MOD_VARIANT, VERSION, config_file.c_str());
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - Make sure the file is valid JSON!", MOD_NAME, MOD_VARIANT, VERSION);
				LogDefaultConfigValues();
			}

			in_stream.close();
		}
		else
		{
			in_stream.close();

			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s (%s) %s] - The \"QuakeSpell.json\" file was not found. Creating file: %s", MOD_NAME, MOD_VARIANT, VERSION, config_file.c_str());

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

		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - An error occurred loading the mod configuration file.", MOD_NAME, MOD_VARIANT, VERSION);
		LogDefaultConfigValues();
	}
}

void AfterburnToggle(bool enable)
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	RValue __ari = *global_instance->GetRefMember("__ari");

	if (enable)
		*__ari.GetRefMember("fire_breath_time") = 2147483647.0;
	else
		*__ari.GetRefMember("fire_breath_time") = 0;

	RValue status_effects = *__ari.GetRefMember("status_effects");
	RValue effects = *status_effects.GetRefMember("effects");
	RValue inner = *effects.GetRefMember("inner");
	RValue flame_breath = *inner.GetRefMember(std::to_string(status_effect_name_to_id_map["flame_breath"]));

	if (enable)
		*flame_breath.GetRefMember("finish") = 2147483647.0;
	else
		*flame_breath.GetRefMember("finish") = 0;
}

void ResetStaticFields(bool returnedToTitleScreen)
{
	if (returnedToTitleScreen)
	{
		ari_current_mana = -1.0;
		pinned_spell = -1;
	}

	if(afterburn)
		AfterburnToggle(false);

	quake_magnitude = 0;
	afterburn = false;
	screen_flash = false;
	screen_flash_initialized = false;
	screen_flash_start_time = 0;
	rumble_sound = false;
	rumble_sound_initialized = false;
	rumble_sound_start_time = 0;
	quake_spell_active = false;
	room_loaded = false;
	modify_mana = false;
	modify_health = false;
	in_dungeon = false;
}

void draw_rectangle(int color, float x1, float y1, float x2, float y2, bool outline)
{
	g_ModuleInterface->CallBuiltin(
		"draw_set_color", {
		 color
		}
	);

	g_ModuleInterface->CallBuiltin(
		"draw_rectangle", {
			x1, y1, x2, y2, outline
		}
	);
}

uint64_t get_current_time_ms() {
	return duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

void LoadStatusEffects()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	size_t array_length = 0;
	RValue status_effects = global_instance->GetMember("__status_effect_id__");
	g_ModuleInterface->GetArraySize(status_effects, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(status_effects, i, array_element);

		status_effect_name_to_id_map[array_element->ToString()] = i;
	}
}

void LoadSpells()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	size_t array_length = 0;
	RValue spells = global_instance->GetMember("__spell__");
	g_ModuleInterface->GetArraySize(spells, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(spells, i, array_element);

		spell_name_to_id_map[array_element->ToString()] = i;
	}
}

void ModifyHealth(CInstance* Self, CInstance* Other, int value)
{
	CScript* gml_script_modify_health = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_modify_health@Ari@Ari",
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

void ModifyMonsterHealth(CInstance* Self)
{
	RValue processed_monster_damage = g_ModuleInterface->CallBuiltin("struct_exists", { Self, "__quake_spell__processed_monster_damage" });
	if (processed_monster_damage.m_Kind == VALUE_BOOL && processed_monster_damage.m_Real == 0)
	{
		RValue hit_points_exists = g_ModuleInterface->CallBuiltin("struct_exists", { Self, "hit_points" });
		if (hit_points_exists.m_Kind == VALUE_BOOL && hit_points_exists.m_Real == 1)
		{
			RValue hit_points = *Self->GetRefMember("hit_points");
			if (hit_points.m_Kind != VALUE_UNSET && hit_points.m_Kind != VALUE_UNDEFINED)
			{
				if (one_shot_monsters)
					*Self->GetRefMember("hit_points") = 0;
				else
				{
					int quake_damage = quake_magnitude * 10;
					int original_monster_hp = Self->GetMember("hit_points").m_Real;
					int modified_moster_hp = original_monster_hp - quake_damage;
					if (modified_moster_hp < 0)
						modified_moster_hp = 0;
					*Self->GetRefMember("hit_points") = modified_moster_hp; // TODO: Check if this needs to be cast to a double
				}

				g_ModuleInterface->CallBuiltin("struct_set", { Self, "__quake_spell__processed_monster_damage", true });

				// Debug print
				g_ModuleInterface->Print(CM_LIGHTPURPLE, "(DEBUG) Quake spell damaged monster: %s", Self->m_Object->m_Name); // TODO: Remove this debug print out
			}
		}
	}
}

void PlaySoundEffect(const char* sound_name, int priority)
{
	const auto sound_index = g_ModuleInterface->CallBuiltin(
		"asset_get_index",
		{ sound_name }
	);

	g_ModuleInterface->CallBuiltin(
		"audio_play_sound",
		{ sound_index, priority, false }
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

	if (quake_spell_active && strstr(self->m_Object->m_Name, "obj_monster"))
	{
		if(strstr(self->m_Object->m_Name, "obj_monster_mimic"))
		{
			if (!ignore_mimics_option)
			{
				ModifyMonsterHealth(self);
			}
		}
		else
		{
			ModifyMonsterHealth(self);
		}
	}

	if (modify_health && strstr(self->m_Object->m_Name, "obj_ari"))
	{
		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		ModifyHealth(global_instance->GetRefMember("__ari")->ToInstance(), self, quake_magnitude * 10 * -1);
		modify_health = false;

		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Ari took %d damage from the quake!", MOD_NAME, MOD_VARIANT, VERSION, quake_magnitude * 10);
	}
}

RValue& GmlScriptSetPinnedSpellCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if(Arguments[0]->m_Kind == VALUE_INT64)
		pinned_spell = Arguments[0]->m_i64;
	if (Arguments[0]->m_Kind == VALUE_REAL)
		pinned_spell = Arguments[0]->m_Real;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_SET_PINNED_SPELL));
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

	if (pinned_spell == spell_name_to_id_map[MOD_VARIANT_SPELL_NAME_MAP[MOD_VARIANT]])
	{
		if (in_dungeon && !quake_spell_active && ari_current_mana >= spell_cost)
		{
			Result.m_Real = 1.0;
		}
		else {
			Result.m_Real = 0.0;
		}
	}

	return Result;
}

RValue& GmlScriptCastSpellCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (pinned_spell == spell_name_to_id_map[MOD_VARIANT_SPELL_NAME_MAP[MOD_VARIANT]])
	{
		// Reduce the HP of all breakables in the room.
		CRoom* current_room = nullptr;
		if (AurieSuccess(g_ModuleInterface->GetCurrentRoomData(current_room)))
		{
			for (CInstance* inst = current_room->GetMembers().m_ActiveInstances.m_First; inst != nullptr; inst = inst->GetMembers().m_Flink)
			{
				auto map = inst->ToRValue().ToRefMap();
				if (!map.contains("node"))
					continue;

				RValue* nodeValue = map["node"];
				if (!nodeValue || nodeValue->GetKindName() != "struct")
					continue;

				auto nodeRefMap = nodeValue->ToRefMap();
				if (!nodeRefMap.contains("prototype") || !nodeRefMap.contains("hitpoints"))
					continue;

				RValue* protoVal = nodeRefMap["prototype"];
				if (!protoVal || protoVal->GetKindName() != "struct")
					continue;

				auto protoMap = protoVal->ToRefMap();
				if (!protoMap.contains("ladder_candidate") || !protoMap["ladder_candidate"]->ToBoolean())
					continue;

				*nodeValue->GetRefMember("hitpoints") = 0;
			}
		}

		std::uniform_int_distribution<int> distribution(minimum_magnitude, maximum_magnitude);
		quake_magnitude = distribution(generator);

		if(damage_ari_option)
			modify_health = true;
		if(screen_flash_option)
			screen_flash = true;
		if(sound_effects_option)
			rumble_sound = true;

		modify_mana = true;
		quake_spell_active = true;
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Quake spell cast! Magnitude: %d", MOD_NAME, MOD_VARIANT, VERSION, quake_magnitude);

		if (afterburn_option)
		{
			afterburn = true;
			Arguments[0]->m_i64 = spell_name_to_id_map[FIRE_BREATH_SPELL];
		}
		else
			return Result;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_CAST_SPELL));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (afterburn && pinned_spell == spell_name_to_id_map[MOD_VARIANT_SPELL_NAME_MAP[MOD_VARIANT]])
		AfterburnToggle(true);

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
		CreateOrLoadConfigFile();
		LoadSpells();
		LoadStatusEffects();
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

RValue& GmlScriptTryEndOfDayStatusToStringCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	ResetStaticFields(false);

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_TRY_END_OF_DAY_STATUS_TO_STRING));
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
	room_loaded = true;
	
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_SHOW_ROOM_TITLE));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}


RValue& GmlScriptGetManaCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_MANA));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (room_loaded && Result.m_Kind != VALUE_UNSET)
	{
		ari_current_mana = Result.m_Real;
	}

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
	if (modify_mana)
	{
		Arguments[0]->m_Real = (spell_cost * -1.0);
		modify_mana = false;
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

RValue& GmlScriptOnDrawGuiCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_ON_DRAW_GUI));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (screen_flash)
	{
		if (!screen_flash_initialized)
		{
			screen_flash_start_time = get_current_time_ms();
			screen_flash_initialized = true;
			PlaySoundEffect("snd_AriLowHealthWarning", 100);
		}

		if (get_current_time_ms() < screen_flash_start_time + 50)
		{
			RValue window_width = g_ModuleInterface->CallBuiltin("window_get_width", {});
			RValue window_height = g_ModuleInterface->CallBuiltin("window_get_height", {}); 
			draw_rectangle(255, 0, 0, static_cast<int>(window_width.m_Real), static_cast<int>(window_height.m_Real), false);
		}
		else
		{
			screen_flash = false;
			screen_flash_initialized = false;
			screen_flash_start_time = 0;
		}
	}

	if (rumble_sound)
	{
		if (!rumble_sound_initialized)
		{
			rumble_sound_start_time = get_current_time_ms();
			rumble_sound_initialized = true;
		}

		uint64_t current_time = get_current_time_ms();
		if (current_time < rumble_sound_start_time + 1000)
		{
			if(current_time % 10 == 0)
				PlaySoundEffect("snd_EarthbreakerRockBreak1", 50);
			else if (current_time % 7 == 0)
				PlaySoundEffect("snd_EarthbreakerRockBreak2", 50);
			else if (current_time % 5 == 0)
				PlaySoundEffect("snd_EarthbreakerRockBreak3", 50);
		}
		else
		{
			rumble_sound = false;
			rumble_sound_initialized = false;
			rumble_sound_start_time = 0;
		}
	}

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

	std::string location = Result.ToString();
	if (location == "dungeon")
		in_dungeon = true;
	else
		in_dungeon = false;

	return Result;
}

RValue& GmlScriptFadeOutCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	ResetStaticFields(false);

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_FADE_OUT));
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
	if (afterburn)
	{
		afterburn = false;
		AfterburnToggle(false);
	}

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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to hook (EVENT_OBJECT_CALL)!", MOD_NAME, MOD_VARIANT, VERSION);
	}
}

void CreateHookGmlScriptSetPinnedSpell(AurieStatus& status)
{
	CScript* gml_script_set_pinned_spell = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SET_PINNED_SPELL,
		(PVOID*)&gml_script_set_pinned_spell
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to get script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_SET_PINNED_SPELL);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_SET_PINNED_SPELL,
		gml_script_set_pinned_spell->m_Functions->m_ScriptFunction,
		GmlScriptSetPinnedSpellCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to hook script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_SET_PINNED_SPELL);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to get script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_CAN_CAST_SPELL);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to hook script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_CAN_CAST_SPELL);
	}
}

void CreateHookGmlScriptCastSpell(AurieStatus& status)
{
	CScript* gml_script_cast_spell = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CAST_SPELL,
		(PVOID*)&gml_script_cast_spell
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to get script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_CAST_SPELL);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_CAST_SPELL,
		gml_script_cast_spell->m_Functions->m_ScriptFunction,
		GmlScriptCastSpellCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to hook script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_CAST_SPELL);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to get script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_SETUP_MAIN_SCREEN);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to hook script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_SETUP_MAIN_SCREEN);
	}
}

void CreateHookGmlScriptEndOfDayStatusToString(AurieStatus& status)
{
	CScript* gml_script_try_end_of_day_status_to_string = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_TRY_END_OF_DAY_STATUS_TO_STRING,
		(PVOID*)&gml_script_try_end_of_day_status_to_string
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to get script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_TRY_END_OF_DAY_STATUS_TO_STRING);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_TRY_END_OF_DAY_STATUS_TO_STRING,
		gml_script_try_end_of_day_status_to_string->m_Functions->m_ScriptFunction,
		GmlScriptTryEndOfDayStatusToStringCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to hook script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_TRY_END_OF_DAY_STATUS_TO_STRING);
	}
}

void CreateHookGmlScriptShowRoomTitle(AurieStatus& status)
{
	CScript* gml_script_show_room_title = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SHOW_ROOM_TITLE,
		(PVOID*)&gml_script_show_room_title
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to get script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_SHOW_ROOM_TITLE);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_SHOW_ROOM_TITLE,
		gml_script_show_room_title->m_Functions->m_ScriptFunction,
		GmlScriptShowRoomTitleCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to hook script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_SHOW_ROOM_TITLE);
	}
}

void CreateHookGmlScriptGetMana(AurieStatus& status)
{
	CScript* gml_script_get_mana = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_MANA,
		(PVOID*)&gml_script_get_mana
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to get script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_GET_MANA);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_MANA,
		gml_script_get_mana->m_Functions->m_ScriptFunction,
		GmlScriptGetManaCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to hook script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_GET_MANA);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to get script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_MODIFY_MANA);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to hook script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_MODIFY_MANA);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to get script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_ON_DRAW_GUI);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to hook script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_ON_DRAW_GUI);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to get script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_TRY_LOCATION_ID_TO_STRING);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to hook script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_TRY_LOCATION_ID_TO_STRING);
	}
}

void CreateHookGmlScriptFadeOut(AurieStatus& status)
{
	CScript* gml_script_fade_out = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_FADE_OUT,
		(PVOID*)&gml_script_fade_out
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to get script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_FADE_OUT);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_FADE_OUT,
		gml_script_fade_out->m_Functions->m_ScriptFunction,
		GmlScriptFadeOutCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to hook script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_FADE_OUT);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to hook script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_SAVE_GAME);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Failed to hook script (%s)!", MOD_NAME, MOD_VARIANT, VERSION, GML_SCRIPT_SAVE_GAME);
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

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[%s (%s) %s] - Plugin starting...", MOD_NAME, MOD_VARIANT, VERSION);

	CreateObjectCallback(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Exiting due to failure on start!", MOD_NAME, MOD_VARIANT, VERSION);
		return status;
	}

	CreateHookGmlScriptSetPinnedSpell(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Exiting due to failure on start!", MOD_NAME, MOD_VARIANT, VERSION);
		return status;
	}

	CreateHookGmlScriptCanCastSpell(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Exiting due to failure on start!", MOD_NAME, MOD_VARIANT, VERSION);
		return status;
	}

	CreateHookGmlScriptCastSpell(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Exiting due to failure on start!", MOD_NAME, MOD_VARIANT, VERSION);
		return status;
	}

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Exiting due to failure on start!", MOD_NAME, MOD_VARIANT, VERSION);
		return status;
	}

	CreateHookGmlScriptEndOfDayStatusToString(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Exiting due to failure on start!", MOD_NAME, MOD_VARIANT, VERSION);
		return status;
	}

	CreateHookGmlScriptShowRoomTitle(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Exiting due to failure on start!", MOD_NAME, MOD_VARIANT, VERSION);
		return status;
	}

	CreateHookGmlScriptGetMana(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Exiting due to failure on start!", MOD_NAME, MOD_VARIANT, VERSION);
		return status;
	}

	CreateHookGmlScriptModifyMana(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Exiting due to failure on start!", MOD_NAME, MOD_VARIANT, VERSION);
		return status;
	}

	CreateHookGmlScriptOnDrawGui(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Exiting due to failure on start!", MOD_NAME, MOD_VARIANT, VERSION);
		return status;
	}

	CreateHookGmlScriptTryLocationIdToString(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Exiting due to failure on start!", MOD_NAME, MOD_VARIANT, VERSION);
		return status;
	}

	CreateHookGmlScriptFadeOut(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Exiting due to failure on start!", MOD_NAME, MOD_VARIANT, VERSION);
		return status;
	}

	CreateHookGmlScriptSaveGame(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s (%s) %s] - Exiting due to failure on start!", MOD_NAME, MOD_VARIANT, VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s (%s) %s] - Plugin started!", MOD_NAME, MOD_VARIANT, VERSION);
	return AURIE_SUCCESS;
}
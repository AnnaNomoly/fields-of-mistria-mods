#include <fstream>
#include <iostream>
#include <filesystem>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <YYToolkit/YYTK_Shared.hpp> // YYTK v4
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

static const char* const MOD_NAME = "UtilitySword";
static const char* const VERSION = "1.0.0";
static const char* const SWORD_CAN_PICK_KEY = "sword_can_pick";
static const char* const SWORD_CAN_CHOP_KEY = "sword_can_chop";
static const char* const ENABLED_ON_FARM_KEY = "enabled_on_farm";
static const char* const ENABLED_IN_FARM_HOUSE_KEY = "enabled_in_farm_house";
static const char* const ENABLED_IN_MISTRIA_KEY = "enabled_in_mistria";
static const char* const ENABLED_IN_MINES_KEY = "enabled_in_mines";
static const char* const GML_SCRIPT_GO_TO_ROOM = "gml_Script_goto_gm_room";
static const char* const GML_SCRIPT_HELD_ITEM = "gml_Script_held_item@Ari@Ari";
static const char* const GML_SCRIPT_DAMAGE = "gml_Script_damage@gml_Object_obj_damage_receiver_Create_0";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const bool DEFAULT_SWORD_CAN_PICK = true;
static const bool DEFAULT_SWORD_CAN_CHOP = true;
static const bool DEFAULT_ENABLED_ON_FARM = false;
static const bool DEFAULT_ENABLED_IN_FARM_HOUSE = false;
static const bool DEFAULT_ENABLED_IN_MISTRIA = true;
static const bool DEFAULT_ENABLED_IN_MINES = true;

static YYTKInterface* g_ModuleInterface = nullptr;
CInstance* global_instance = nullptr;
static bool load_on_start = true;
static bool sword_can_pick = DEFAULT_SWORD_CAN_PICK;
static bool sword_can_chop = DEFAULT_SWORD_CAN_CHOP;
static bool enabled_on_farm = DEFAULT_ENABLED_ON_FARM;
static bool enabled_in_farm_house = DEFAULT_ENABLED_IN_FARM_HOUSE;
static bool enabled_in_mistria = DEFAULT_ENABLED_IN_MISTRIA;
static bool enabled_in_mines = DEFAULT_ENABLED_IN_MINES;
static int held_item_id = -1;
static std::string ari_current_gm_room = "";
static std::unordered_set<int> swords = {};

bool GameIsPaused()
{
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

void ResetStaticFields()
{
	held_item_id = -1;
	ari_current_gm_room = "";
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
		{ SWORD_CAN_PICK_KEY, use_defaults ? DEFAULT_SWORD_CAN_PICK : sword_can_pick },
		{ SWORD_CAN_CHOP_KEY, use_defaults ? DEFAULT_SWORD_CAN_CHOP : sword_can_chop },
		{ ENABLED_ON_FARM_KEY, use_defaults ? DEFAULT_ENABLED_ON_FARM : enabled_on_farm},
		{ ENABLED_IN_FARM_HOUSE_KEY, use_defaults ? DEFAULT_ENABLED_IN_FARM_HOUSE : enabled_in_farm_house },
		{ ENABLED_IN_MISTRIA_KEY, use_defaults ? DEFAULT_ENABLED_IN_MISTRIA : enabled_in_mistria },
		{ ENABLED_IN_MINES_KEY, use_defaults ? DEFAULT_ENABLED_IN_MINES : enabled_in_mines }
	};
	return config_json;
}

void LogDefaultConfigValues()
{
	sword_can_pick = DEFAULT_SWORD_CAN_PICK;
	sword_can_chop = DEFAULT_SWORD_CAN_CHOP;
	enabled_on_farm = DEFAULT_ENABLED_ON_FARM;
	enabled_in_farm_house = DEFAULT_ENABLED_IN_FARM_HOUSE;
	enabled_in_mistria = DEFAULT_ENABLED_IN_MISTRIA;
	enabled_in_mines = DEFAULT_ENABLED_IN_MINES;

	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, SWORD_CAN_PICK_KEY, DEFAULT_SWORD_CAN_PICK ? "true" : "false");
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, SWORD_CAN_CHOP_KEY, DEFAULT_SWORD_CAN_CHOP ? "true" : "false");
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, ENABLED_ON_FARM_KEY, DEFAULT_ENABLED_ON_FARM ? "true" : "false");
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, ENABLED_IN_FARM_HOUSE_KEY, DEFAULT_ENABLED_IN_FARM_HOUSE ? "true" : "false");
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, ENABLED_IN_MISTRIA_KEY, DEFAULT_ENABLED_IN_MISTRIA ? "true" : "false");
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, ENABLED_IN_MINES_KEY, DEFAULT_ENABLED_IN_MINES ? "true" : "false");
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

		// Try to find the mod_data/UtilitySword directory.
		std::string utility_sword_folder = mod_data_folder + "\\UtilitySword";
		if (!std::filesystem::exists(utility_sword_folder))
		{
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"UtilitySword\" directory was not found. Creating directory: %s", MOD_NAME, VERSION, utility_sword_folder.c_str());
			std::filesystem::create_directory(utility_sword_folder);
		}

		// Try to find the mod_data/UtilitySword/UtilitySword.json config file.
		bool update_config_file = false;
		std::string config_file = utility_sword_folder + "\\" + "UtilitySword.json";
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
					// Try loading the sword_can_pick value.
					if (json_object.contains(SWORD_CAN_PICK_KEY))
					{
						sword_can_pick = json_object[SWORD_CAN_PICK_KEY];
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %s!", MOD_NAME, VERSION, SWORD_CAN_PICK_KEY, sword_can_pick ? "true" : "false");
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, SWORD_CAN_PICK_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, SWORD_CAN_PICK_KEY, DEFAULT_SWORD_CAN_PICK ? "true" : "false");
					}

					// Try loading the sword_can_chop value.
					if (json_object.contains(SWORD_CAN_CHOP_KEY))
					{
						sword_can_chop = json_object[SWORD_CAN_CHOP_KEY];
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %s!", MOD_NAME, VERSION, SWORD_CAN_CHOP_KEY, sword_can_chop ? "true" : "false");
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, SWORD_CAN_CHOP_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, SWORD_CAN_CHOP_KEY, DEFAULT_SWORD_CAN_CHOP ? "true" : "false");
					}

					// Try loading the enabled_on_farm value.
					if (json_object.contains(ENABLED_ON_FARM_KEY))
					{
						enabled_on_farm = json_object[ENABLED_ON_FARM_KEY];
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %s!", MOD_NAME, VERSION, ENABLED_ON_FARM_KEY, enabled_on_farm ? "true" : "false");
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, ENABLED_ON_FARM_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, ENABLED_ON_FARM_KEY, DEFAULT_ENABLED_ON_FARM ? "true" : "false");
					}

					// Try loading the enabled_in_farm_house value.
					if (json_object.contains(ENABLED_IN_FARM_HOUSE_KEY))
					{
						enabled_in_farm_house = json_object[ENABLED_IN_FARM_HOUSE_KEY];
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %s!", MOD_NAME, VERSION, ENABLED_IN_FARM_HOUSE_KEY, enabled_in_farm_house ? "true" : "false");
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, ENABLED_IN_FARM_HOUSE_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, ENABLED_IN_FARM_HOUSE_KEY, DEFAULT_ENABLED_IN_FARM_HOUSE ? "true" : "false");
					}

					// Try loading the enabled_in_mistria value.
					if (json_object.contains(ENABLED_IN_MISTRIA_KEY))
					{
						enabled_in_mistria = json_object[ENABLED_IN_MISTRIA_KEY];
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %s!", MOD_NAME, VERSION, ENABLED_IN_MISTRIA_KEY, enabled_in_mistria ? "true" : "false");
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, ENABLED_IN_MISTRIA_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, ENABLED_IN_MISTRIA_KEY, DEFAULT_ENABLED_IN_MISTRIA ? "true" : "false");
					}

					// Try loading the enabled_in_mines value.
					if (json_object.contains(ENABLED_IN_MINES_KEY))
					{
						enabled_in_mines = json_object[ENABLED_IN_MINES_KEY];
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %s!", MOD_NAME, VERSION, ENABLED_IN_MINES_KEY, enabled_in_mines ? "true" : "false");
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, ENABLED_IN_MINES_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, ENABLED_IN_MINES_KEY, DEFAULT_ENABLED_IN_MINES ? "true" : "false");
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

			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"UtilitySword.json\" file was not found. Creating file: %s", MOD_NAME, VERSION, config_file.c_str());

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

bool AriIsOnFarm()
{
	return ari_current_gm_room == "rm_farm";
}

bool AriIsInFarmHouse()
{
	return ari_current_gm_room.contains("rm_farmhouse");
}

bool AriIsInMistria()
{
	return ari_current_gm_room == "rm_narrows" ||
		ari_current_gm_room == "rm_beach" ||
		ari_current_gm_room == "rm_deep_woods" ||
		ari_current_gm_room == "rm_western_ruins" ||
		ari_current_gm_room == "rm_summit" ||
		ari_current_gm_room == "rm_eastern_road" ||
		ari_current_gm_room == "rm_haydens_farm" ||
		ari_current_gm_room == "rm_town";
}

bool AriIsInMines()
{
	return ari_current_gm_room.contains("rm_mines") && ari_current_gm_room != "rm_mines_entry";
}

void LoadItems()
{
	size_t array_length;
	RValue item_data = global_instance->GetMember("__item_data");
	g_ModuleInterface->GetArraySize(item_data, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* item;
		g_ModuleInterface->GetArrayEntry(item_data, i, item);

		if (StructVariableExists(*item, "item_id") && StructVariableExists(*item, "tags"))
		{
			int item_id = item->GetMember("item_id").ToInt64();
			RValue tags = item->GetMember("tags");
			RValue buffer = tags.GetMember("__buffer");

			size_t array_length = 0;
			g_ModuleInterface->GetArraySize(buffer, array_length);
			for (size_t i = 0; i < array_length; i++)
			{
				RValue* array_element;
				g_ModuleInterface->GetArrayEntry(buffer, i, array_element);

				if (array_element->ToString() == "weapon")
					swords.insert(item_id);
			}
		}
	}
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

	RValue gm_room = Result.GetMember("gm_room");
	RValue room_name = g_ModuleInterface->CallBuiltin("room_get_name", { gm_room });
	ari_current_gm_room = room_name.ToString();

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

RValue& GmlScriptDamageCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (!GameIsPaused() && swords.contains(held_item_id))
	{
		RValue target = Arguments[0]->GetMember("target");
		if (target.ToInt64() != 1) // Everything not Ari
		{
			bool activate = false;
			if (AriIsOnFarm() && enabled_on_farm)
				activate = true;
			else if (AriIsInFarmHouse() && enabled_in_farm_house)
				activate = true;
			else if (AriIsInMistria() && enabled_in_mistria)
				activate = true;
			else if (AriIsInMines() && enabled_in_mines)
				activate = true;

			if (activate)
			{
				*Arguments[0]->GetRefMember("can_pick_grid_objects") = sword_can_pick; // Pick node objects
				*Arguments[0]->GetRefMember("can_chop_grid_objects") = sword_can_chop; // Chop node objects
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
		LoadItems();
		load_on_start = false;
	}
	else
		ResetStaticFields();

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

EXPORTED AurieStatus ModuleInitialize(IN AurieModule* Module, IN const fs::path& ModulePath) {
	UNREFERENCED_PARAMETER(ModulePath);

	AurieStatus status = AURIE_SUCCESS;

	status = ObGetInterface(
		"YYTK_Main",
		(AurieInterfaceBase*&)(g_ModuleInterface)
	);

	if (!AurieSuccess(status))
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[%s %s] - Plugin starting...", MOD_NAME, VERSION);

	CreateHookGmlScriptGoToRoom(status);
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

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Plugin started!", MOD_NAME, VERSION);
	return AURIE_SUCCESS;
}
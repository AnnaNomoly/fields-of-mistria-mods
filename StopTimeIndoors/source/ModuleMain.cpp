#include <map>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <YYToolkit/YYTK_Shared.hpp> // YYTK v4
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

static const char* const MOD_NAME = "StopTimeIndoors";
static const char* const VERSION = "1.0.3";
static const char* const STOP_TIME_IN_DUNGEON_KEY = "stop_time_in_dungeon";
static const char* const STOP_TIME_ON_FARM_KEY = "stop_time_on_farm";
static const char* const GML_SCRIPT_GET_WEATHER = "gml_Script_get_weather@WeatherManager@Weather";
static const char* const GML_SCRIPT_IS_DUNGEON_ROOM = "gml_Script_is_dungeon_room";
static const char* const GML_SCRIPT_TRY_LOCATION_ID_TO_STRING = "gml_Script_try_location_id_to_string";
static const char* const GML_SCRIPT_UPDATE_CLOCK = "gml_Script_update@Clock@Clock";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";

static const bool DEFAULT_STOP_TIME_IN_DUNGEON = false;
static const bool DEFAULT_STOP_TIME_ON_FARM = false;

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_on_start = true;
static bool stop_time_in_dungeon = DEFAULT_STOP_TIME_IN_DUNGEON;
static bool stop_time_on_farm = DEFAULT_STOP_TIME_ON_FARM;
static bool game_is_active = false;
static bool ari_is_in_dungeon = false;
static int ari_current_location_id = -1;
static std::map<int, bool> location_id_to_outdoor_map = {};
static std::map<std::string, int> location_name_to_id_map = {};

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
		{ STOP_TIME_IN_DUNGEON_KEY, use_defaults ? DEFAULT_STOP_TIME_IN_DUNGEON : stop_time_in_dungeon },
		{ STOP_TIME_ON_FARM_KEY, use_defaults ? DEFAULT_STOP_TIME_ON_FARM : stop_time_on_farm }
	};
	return config_json;
}

void LogDefaultConfigValues()
{
	stop_time_in_dungeon = DEFAULT_STOP_TIME_IN_DUNGEON;

	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, STOP_TIME_IN_DUNGEON_KEY, DEFAULT_STOP_TIME_IN_DUNGEON ? "true" : "false");
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

		// Try to find the mod_data/StopTimeIndoors directory.
		std::string stop_time_indoors_folder = mod_data_folder + "\\StopTimeIndoors";
		if (!std::filesystem::exists(stop_time_indoors_folder))
		{
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"StopTimeIndoors\" directory was not found. Creating directory: %s", MOD_NAME, VERSION, stop_time_indoors_folder.c_str());
			std::filesystem::create_directory(stop_time_indoors_folder);
		}

		// Try to find the mod_data/StopTimeIndoors/StopTimeIndoors.json config file.
		bool update_config_file = false;
		std::string config_file = stop_time_indoors_folder + "\\" + "StopTimeIndoors.json";
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
					// Try loading the stop_time_in_dungeon value.
					if (json_object.contains(STOP_TIME_IN_DUNGEON_KEY))
					{
						stop_time_in_dungeon = json_object[STOP_TIME_IN_DUNGEON_KEY];
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %s!", MOD_NAME, VERSION, STOP_TIME_IN_DUNGEON_KEY, stop_time_in_dungeon ? "true" : "false");
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, STOP_TIME_IN_DUNGEON_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, STOP_TIME_IN_DUNGEON_KEY, DEFAULT_STOP_TIME_IN_DUNGEON ? "true" : "false");
					}

					// Try loading the stop_time_on_farm value.
					if (json_object.contains(STOP_TIME_ON_FARM_KEY))
					{
						stop_time_on_farm = json_object[STOP_TIME_ON_FARM_KEY];
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %s!", MOD_NAME, VERSION, STOP_TIME_ON_FARM_KEY, stop_time_on_farm ? "true" : "false");
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, STOP_TIME_ON_FARM_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, STOP_TIME_ON_FARM_KEY, DEFAULT_STOP_TIME_ON_FARM ? "true" : "false");
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

			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"StopTimeIndoors.json\" file was not found. Creating file: %s", MOD_NAME, VERSION, config_file.c_str());

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

bool AriIsIndoors()
{
	if (!game_is_active || ari_current_location_id == -1)
		return false;

	// NOTE: The value for location_id_to_outdoor_map["farm"] is modified in the LoadLocations() function based on the config value: stop_time_on_farm
	if (location_id_to_outdoor_map[ari_current_location_id])
		return false;

	if (ari_is_in_dungeon && !stop_time_in_dungeon)
		return false;

	return true;
}

bool RValueToBool(RValue value)
{
	if (value.m_Kind == VALUE_BOOL && value.m_Real == 1)
		return true;
	return false;
}

void ResetStaticFields()
{
	game_is_active = false;
	ari_is_in_dungeon = false;
	ari_current_location_id = -1;
}

void LoadLocationData()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	// Location IDs
	RValue location_ids = global_instance->GetMember("__location_id__");
	RValue location_ids_size = g_ModuleInterface->CallBuiltin("array_length", { location_ids });
	for (int i = 0; i < location_ids_size.m_Real; i++)
	{
		RValue location = g_ModuleInterface->CallBuiltin("array_get", { location_ids, i });
		location_name_to_id_map[location.ToString()] = i;
	}

	// Default location data
	RValue locations = global_instance->GetMember("__locations");
	RValue locations_size = g_ModuleInterface->CallBuiltin("array_length", { locations });

	for (int i = 0; i < locations_size.m_Real; i++)
	{
		RValue location = g_ModuleInterface->CallBuiltin("array_get", { locations, i });
		RValue outdoor = g_ModuleInterface->CallBuiltin("struct_get", { location, "outdoor" });
		location_id_to_outdoor_map[i] = RValueToBool(outdoor);
	}

	if (stop_time_on_farm)
		location_id_to_outdoor_map[location_name_to_id_map["farm"]] = false;
}

RValue& GmlScriptGetWeatherCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (!game_is_active)
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

RValue& GmlScriptIsDungeonRoomCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_IS_DUNGEON_ROOM));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active && Result.m_Kind == VALUE_BOOL)
	{
		if (Result.m_Real == 0)
			ari_is_in_dungeon = false;
		else
			ari_is_in_dungeon = true;
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
	if (game_is_active)
	{
		if (Arguments[0]->m_Kind == VALUE_INT64)
			ari_current_location_id = Arguments[0]->m_i64;
		if (Arguments[0]->m_Kind == VALUE_REAL)
			ari_current_location_id = Arguments[0]->m_Real;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_TRY_LOCATION_ID_TO_STRING));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptClockUpdateCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_UPDATE_CLOCK));

	CInstance* global_instance = nullptr;
	if (!AurieSuccess(g_ModuleInterface->GetGlobalInstance(&global_instance)))
	{
		return original(
			Self,
			Other,
			Result,
			ArgumentCount,
			Arguments
		);
	}

	RValue time = global_instance->GetMember("__clock").GetMember("time");
	int64_t old_time_value = time.ToInt64();
	
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);
	
	RValue new_time = global_instance->GetMember("__clock").GetMember("time");;
	int64_t new_time_value = new_time.ToInt64();
	int64_t difference = new_time_value - old_time_value;

	if (difference != 0 && AriIsIndoors())
		*global_instance->GetRefMember("__clock")->GetRefMember("time") = old_time_value;

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
	ResetStaticFields();

	if (load_on_start)
	{
		CreateOrLoadConfigFile();
		LoadLocationData();
		load_on_start = false;
	}

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

void CreateHookGmlScriptIsDungeonRoom(AurieStatus& status)
{
	CScript* gml_script_is_dungeon_room = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_IS_DUNGEON_ROOM,
		(PVOID*)&gml_script_is_dungeon_room
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_IS_DUNGEON_ROOM);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_IS_DUNGEON_ROOM,
		gml_script_is_dungeon_room->m_Functions->m_ScriptFunction,
		GmlScriptIsDungeonRoomCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_IS_DUNGEON_ROOM);
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

void CreateHookGmlScriptClockUpdate(AurieStatus& status)
{
	CScript* gml_script_clock_update = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_UPDATE_CLOCK,
		(PVOID*)&gml_script_clock_update
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_UPDATE_CLOCK);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_UPDATE_CLOCK,
		gml_script_clock_update->m_Functions->m_ScriptFunction,
		GmlScriptClockUpdateCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_UPDATE_CLOCK);
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

	CreateHookGmlScriptGetWeather(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptIsDungeonRoom(status);
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

	CreateHookGmlScriptClockUpdate(status);
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
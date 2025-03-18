#include <map>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static const char* const VERSION = "0.0.1";

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_locations = true;
static bool game_is_active = false;
static bool ari_is_in_dungeon = false;
static int ari_current_location_id = -1;
static std::map<int, bool> location_id_to_outdoor_map = {};

bool AriIsIndoors()
{
	if (!game_is_active || ari_current_location_id == -1)
		return false;

	if (ari_is_in_dungeon || location_id_to_outdoor_map[ari_current_location_id])
		return false;

	return true;
}

bool RValueToBool(RValue value)
{
	if (value.m_Kind == VALUE_BOOL && value.m_Real == 1)
		return true;
	return false;
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

	if (!strstr(self->m_Object->m_Name, "obj_ari"))
		return;
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

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_get_weather@WeatherManager@Weather"));
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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_is_dungeon_room"));
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

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_try_location_id_to_string"));
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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_update@Clock@Clock"));

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

	RValue& time = global_instance->at("__clock").at("time");
	double old_time_value = time.AsReal();
	
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);
	
	double new_time_value = time.AsReal();
	double difference = new_time_value - old_time_value;

	if (static_cast<int64_t>(difference) != 0)
		if(AriIsIndoors())
			time = old_time_value;

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
	game_is_active = false;
	ari_is_in_dungeon = false;
	ari_current_location_id = -1;

	if (load_locations)
	{
		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		RValue locations = global_instance->at("__locations");
		RValue locations_size = g_ModuleInterface->CallBuiltin("array_length", { locations });

		for (int i = 0; i < locations_size.m_Real; i++)
		{
			RValue locations_element = g_ModuleInterface->CallBuiltin("array_get", { locations, i });
			RValue outdoor = g_ModuleInterface->CallBuiltin("struct_get", { locations_element, "outdoor" });
			location_id_to_outdoor_map[i] = RValueToBool(outdoor);
		}

		load_locations = false;
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

void CreateHookGmlScriptGetWeather(AurieStatus& status)
{
	CScript* gml_script_get_weather = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_get_weather@WeatherManager@Weather",
		(PVOID*)&gml_script_get_weather
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[StopTimeIndoors %s] - Failed to get script (gml_Script_get_weather@WeatherManager@Weather)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_get_weather@WeatherManager@Weather",
		gml_script_get_weather->m_Functions->m_ScriptFunction,
		GmlScriptGetWeatherCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[StopTimeIndoors %s] - Failed to hook script (gml_Script_get_weather@WeatherManager@Weather)!", VERSION);
	}
}

void CreateHookGmlScriptIsDungeonRoom(AurieStatus& status)
{
	CScript* gml_script_is_dungeon_room = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_is_dungeon_room",
		(PVOID*)&gml_script_is_dungeon_room
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[StopTimeIndoors %s] - Failed to get script (gml_Script_is_dungeon_room)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_is_dungeon_room",
		gml_script_is_dungeon_room->m_Functions->m_ScriptFunction,
		GmlScriptIsDungeonRoomCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[StopTimeIndoors %s] - Failed to hook script (gml_Script_is_dungeon_room)!", VERSION);
	}
}

void CreateHookGmlScriptTryLocationIdToString(AurieStatus& status)
{
	CScript* gml_script_try_location_id_to_string = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_try_location_id_to_string",
		(PVOID*)&gml_script_try_location_id_to_string
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[StopTimeIndoors %s] - Failed to get script (gml_Script_try_location_id_to_string)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_try_location_id_to_string",
		gml_script_try_location_id_to_string->m_Functions->m_ScriptFunction,
		GmlScriptTryLocationIdToStringCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[StopTimeIndoors %s] - Failed to hook script (gml_Script_try_location_id_to_string)!", VERSION);
	}
}

void CreateHookGmlScriptClockUpdate(AurieStatus& status)
{
	CScript* gml_script_clock_update = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_update@Clock@Clock",
		(PVOID*)&gml_script_clock_update
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[StopTimeIndoors %s] - Failed to get script (gml_Script_update@Clock@Clock)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_update@Clock@Clock",
		gml_script_clock_update->m_Functions->m_ScriptFunction,
		GmlScriptClockUpdateCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[StopTimeIndoors %s] - Failed to hook script (gml_Script_update@Clock@Clock)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[StopTimeIndoors %s] - Failed to get script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[StopTimeIndoors %s] - Failed to hook script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
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

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[StopTimeIndoors %s] - Plugin starting...", VERSION);
	
	g_ModuleInterface->CreateCallback(
		g_ArSelfModule,
		EVENT_OBJECT_CALL,
		ObjectCallback,
		0
	);

	CreateHookGmlScriptGetWeather(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[StopTimeIndoors %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptIsDungeonRoom(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[StopTimeIndoors %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptTryLocationIdToString(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[StopTimeIndoors %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptClockUpdate(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[StopTimeIndoors %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[StopTimeIndoors %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[StopTimeIndoors %s] - Plugin started!", VERSION);
	return AURIE_SUCCESS;
}
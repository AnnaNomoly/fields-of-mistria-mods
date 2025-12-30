#include <map>
#include <fstream>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <YYToolkit/YYTK_Shared.hpp> // YYTK v4
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

static const char* const MOD_NAME = "DynamicNpcPortraits";
static const char* const VERSION = "1.0.0";
static const char* const GML_SCRIPT_IS_DUNGEON_ROOM = "gml_Script_is_dungeon_room";
static const char* const GML_SCRIPT_GO_TO_ROOM = "gml_Script_goto_gm_room";
static const char* const GML_SCRIPT_TRY_LOCATION_ID_TO_STRING = "gml_Script_try_location_id_to_string";
static const char* const GML_SCRIPT_GET_MINUTES = "gml_Script_get_minutes";
static const char* const GML_SCRIPT_CALENDAR_DAY = "gml_Script_day@Calendar@Calendar";
static const char* const GML_SCRIPT_CALENDAR_SEASON = "gml_Script_season@Calendar@Calendar";
static const char* const GML_SCRIPT_GET_WEATHER = "gml_Script_get_weather@WeatherManager@Weather";
static const char* const GML_SCRIPT_VERTIGO_DRAW_WITH_COLOR = "gml_Script_vertigo_draw_with_color";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";

static YYTKInterface* g_ModuleInterface = nullptr;
static CInstance* global_instance = nullptr;
static bool load_on_start = true;
static bool game_is_active = false;
static bool ari_is_in_dungeon = false;
static int current_time_in_seconds = -1;
static int current_day = -1;
static int current_season = -1;
static int current_weather = -1;
static int current_location = -1;
static std::string ari_current_gm_room = "";
static std::map<int, bool> location_id_to_outdoor_map = {};
static std::map<std::string, int> weather_name_to_id_map = {}; // __weather__
static std::map<std::string, int> location_name_to_id_map = {}; // __location_id__

void ResetStaticFields(bool title_screen)
{
	game_is_active = false;
	ari_is_in_dungeon = false;
	current_time_in_seconds = -1;
	current_day = -1;
	current_season = -1;
	current_weather = -1;
	current_location = -1;
	ari_current_gm_room = "";
}

bool GameIsPaused()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);
	RValue paused = global_instance->GetMember("__pause_status");
	return paused.ToInt64() > 0;
}

bool IsNumeric(RValue value)
{
	return value.m_Kind == VALUE_INT32 || value.m_Kind == VALUE_INT64 || value.m_Kind == VALUE_REAL;
}

bool IsObject(RValue value)
{
	return value.m_Kind == VALUE_OBJECT;
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

void LoadWeather()
{
	size_t array_length = 0;
	RValue weather = global_instance->GetMember("__weather__");
	g_ModuleInterface->GetArraySize(weather, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(weather, i, array_element);
		weather_name_to_id_map[array_element->ToString()] = i;
	}
}

// TODO
void LoadLocations()
{
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
		location_id_to_outdoor_map[i] = outdoor.ToBoolean();
	}
}

bool AriIsIndoors()
{
	if (!game_is_active || current_location == -1)
		return false;

	if (location_id_to_outdoor_map[current_location])
		return false;

	if (ari_is_in_dungeon)
		return false;

	return true;
}

RValue GetDynamicNpcPortrait(std::string sprite_name)
{
	if (sprite_name == "spr_portrait_balor_spring_angry_blush")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_angry_blush" });
		//else
		//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
	if (sprite_name == "spr_portrait_balor_spring_blush")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_blush" });
		//else
			//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
	if (sprite_name == "spr_portrait_balor_spring_concerned")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_concerned" });
		//else
			//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
	if (sprite_name == "spr_portrait_balor_spring_embarrassed")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_embarrassed" });
		//else
			//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
	if (sprite_name == "spr_portrait_balor_spring_happy")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_happy" });
		//else
			//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
	if (sprite_name == "spr_portrait_balor_spring_mad")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_mad" });
		//else
			//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
	if (sprite_name == "spr_portrait_balor_spring_neutral")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_neutral" });
		//else
			//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
	if (sprite_name == "spr_portrait_balor_spring_sad")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_sad" });
		//else
			//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
	if (sprite_name == "spr_portrait_balor_spring_sigh")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_sigh" });
		//else
			//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
	if (sprite_name == "spr_portrait_balor_spring_sly")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_sly" });
		//else
			//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
	if (sprite_name == "spr_portrait_balor_spring_think")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_think" });
		//else
			//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
	if (sprite_name == "spr_portrait_balor_spring_ugh")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_ugh" });
		//else
			//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
	if (sprite_name == "spr_portrait_balor_spring_wink")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_wink" });
		//else
			//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
	if (sprite_name == "spr_portrait_balor_spring_sincere_special")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_sincere_special" });
		//else
			//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
	if (sprite_name == "spr_portrait_balor_spring_hope_special")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_hope_special" });
		//else
			//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
	if (sprite_name == "spr_portrait_balor_spring_blush_special")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_blush_special" });
		//else
			//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
	if (sprite_name == "spr_portrait_balor_spring_sad_special")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_sad_special" });
		//else
			//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
	if (sprite_name == "spr_portrait_balor_spring_hurt")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_hurt" });
		//else
			//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
	if (sprite_name == "spr_portrait_balor_spring_happy_blush")
	{
		if (AriIsIndoors())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_portrait_balor_spring_indoor_happy_blush" });
		//else
			//	return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_journal_magic_restore_spell_icon_main" });
	}
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
		ari_is_in_dungeon = Result.ToBoolean();

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

	RValue gm_room = Result.GetMember("gm_room");
	RValue room_name = g_ModuleInterface->CallBuiltin("room_get_name", { gm_room });
	ari_current_gm_room = room_name.ToString();
	
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
		current_location = Arguments[0]->ToInt64();

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

RValue& GmlScriptGetMinutesCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_MINUTES));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active && !GameIsPaused())
	{
		RValue time = global_instance->GetMember("__clock").GetMember("time");
		current_time_in_seconds = time.ToInt64();
	}

	return Result;
}

RValue& GmlScriptCalendarDayCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_CALENDAR_DAY));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active)
		current_day = Result.ToInt64() + 1; // Result is a VALUE_REAL that is the 0-indexed calendar day

	return Result;
}

RValue& GmlScriptCalendarSeasonCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_CALENDAR_SEASON));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active)
		current_season = Result.ToInt64() + 1; // Result is a VALUE_REAL that is the 0-indexed calendar season

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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_WEATHER));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (IsNumeric(Result))
		current_weather = Result.ToInt64();

	game_is_active = true;
	return Result;
}

RValue& GmlScriptVertigoDrawWithColorCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (game_is_active)
	{
		RValue type = g_ModuleInterface->CallBuiltin("asset_get_type", { *Arguments[0] });
		if (type.ToInt64() == 1) // asset_sprite
		{
			RValue name = g_ModuleInterface->CallBuiltin("sprite_get_name", { *Arguments[0] });
			std::string name_str = name.ToString();

			RValue dynamic_sprite = GetDynamicNpcPortrait(name_str);
			if (dynamic_sprite.m_Kind == VALUE_REF)
				*Arguments[0] = dynamic_sprite;
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_VERTIGO_DRAW_WITH_COLOR));
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
		g_ModuleInterface->GetGlobalInstance(&global_instance);
		
		LoadWeather();
		LoadLocations();
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

void CreateHookGmlScriptCalendarDay(AurieStatus& status)
{
	CScript* gml_script_calendar_day = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CALENDAR_DAY,
		(PVOID*)&gml_script_calendar_day
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CALENDAR_DAY);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_CALENDAR_DAY,
		gml_script_calendar_day->m_Functions->m_ScriptFunction,
		GmlScriptCalendarDayCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CALENDAR_DAY);
	}
}

void CreateHookGmlScriptCalendarSeason(AurieStatus& status)
{
	CScript* gml_script_calendar_season = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CALENDAR_SEASON,
		(PVOID*)&gml_script_calendar_season
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CALENDAR_SEASON);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_CALENDAR_SEASON,
		gml_script_calendar_season->m_Functions->m_ScriptFunction,
		GmlScriptCalendarSeasonCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CALENDAR_SEASON);
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

void CreateHookGmlScriptVertigoDrawWithColor(AurieStatus& status)
{
	CScript* gml_script_vertigo_draw_with_color = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_VERTIGO_DRAW_WITH_COLOR,
		(PVOID*)&gml_script_vertigo_draw_with_color
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_VERTIGO_DRAW_WITH_COLOR);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_VERTIGO_DRAW_WITH_COLOR,
		gml_script_vertigo_draw_with_color->m_Functions->m_ScriptFunction,
		GmlScriptVertigoDrawWithColorCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_VERTIGO_DRAW_WITH_COLOR);
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

	CreateHookGmlScriptIsDungeonRoom(status);
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

	CreateHookGmlScriptTryLocationIdToString(status);
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

	CreateHookGmlScriptCalendarDay(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptCalendarSeason(status);
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

	CreateHookGmlScriptVertigoDrawWithColor(status);
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
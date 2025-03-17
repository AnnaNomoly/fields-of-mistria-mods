#include <map>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static YYTKInterface* g_ModuleInterface = nullptr;

static const char* const VERSION = "1.0.1";
static const int EIGHT_PM_IN_SECONDS = 72000;

static bool notification_sent = false;
static bool is_friday = false;

void DisplayFridayNightNotification(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_create_notification = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_create_notification",
		(PVOID*)&gml_script_create_notification
	);

	RValue result;
	RValue notification = "misc_local/FridayNightReminder";
	RValue* notification_ptr = &notification;
	gml_script_create_notification->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &notification_ptr }
	);
}

RValue& GmlScriptGetMinutesCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (is_friday && !notification_sent)
	{
		bool send_notification = false;
		if (ArgumentCount == 1 && Arguments[0]->m_Kind == VALUE_INT64 && Arguments[0]->m_i64 >= EIGHT_PM_IN_SECONDS)
			send_notification = true;
		if (ArgumentCount == 1 && Arguments[0]->m_Kind == VALUE_REAL && Arguments[0]->m_Real >= EIGHT_PM_IN_SECONDS)
			send_notification = true;

		if (send_notification)
		{
			DisplayFridayNightNotification(Self, Other);
			notification_sent = true;
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_get_minutes"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_day@Calendar@Calendar"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (Result.m_Kind == VALUE_REAL)
	{
		int day_of_month = Result.m_Real + 1; // Calendar offset
		if (day_of_month == 5 || day_of_month == 12 || day_of_month == 19 || day_of_month == 26) // Fridays
			is_friday = true;
		else
			is_friday = false;
	}	

	return Result;
}

RValue& GmlScriptNewDayCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	notification_sent = false;
	is_friday = false;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_new_day"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptOnRoomStartCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_on_room_start@WeatherManager@Weather"));
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
	notification_sent = false;
	is_friday = false;

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

void CreateHookGmlScriptGetMinutes(AurieStatus& status)
{
	CScript* gml_script_get_minutes = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_get_minutes",
		(PVOID*)&gml_script_get_minutes
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[FridayNightReminder %s] - Failed to get script (gml_Script_get_minutes)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_get_minutes",
		gml_script_get_minutes->m_Functions->m_ScriptFunction,
		GmlScriptGetMinutesCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[FridayNightReminder %s] - Failed to hook script (gml_Script_get_minutes)!", VERSION);
	}
}

void CreateHookGmlScriptCalendarDay(AurieStatus& status)
{
	CScript* gml_script_calendar_day = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_day@Calendar@Calendar",
		(PVOID*)&gml_script_calendar_day
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[FridayNightReminder %s] - Failed to get script (gml_Script_day@Calendar@Calendar)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_day@Calendar@Calendar",
		gml_script_calendar_day->m_Functions->m_ScriptFunction,
		GmlScriptCalendarDayCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[FridayNightReminder %s] - Failed to hook script (gml_Script_day@Calendar@Calendar)!", VERSION);
	}
}

void CreateHookGmlScriptNewDay(AurieStatus& status)
{
	CScript* gml_script_new_day = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_new_day",
		(PVOID*)&gml_script_new_day
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[FridayNightReminder %s] - Failed to get script (gml_Script_new_day)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_new_day",
		gml_script_new_day->m_Functions->m_ScriptFunction,
		GmlScriptNewDayCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[FridayNightReminder %s] - Failed to hook script (gml_Script_new_day)!", VERSION);
	}
}

void CreateHookGmlScriptOnRoomStart(AurieStatus& status)
{
	CScript* gml_script_new_day = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_on_room_start@WeatherManager@Weather",
		(PVOID*)&gml_script_new_day
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[FridayNightReminder %s] - Failed to get script (gml_Script_on_room_start@WeatherManager@Weather)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_on_room_start@WeatherManager@Weather",
		gml_script_new_day->m_Functions->m_ScriptFunction,
		GmlScriptOnRoomStartCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[FridayNightReminder %s] - Failed to hook script (gml_Script_on_room_start@WeatherManager@Weather)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[FridayNightReminder %s] - Failed to get script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[FridayNightReminder %s] - Failed to hook script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
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

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[FridayNightReminder %s] - Plugin starting...", VERSION);
	
	CreateHookGmlScriptGetMinutes(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[FridayNightReminder %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptCalendarDay(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[FridayNightReminder %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptNewDay(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[FridayNightReminder %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptOnRoomStart(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[FridayNightReminder %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[FridayNightReminder %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[FridayNightReminder %s] - Plugin started!", VERSION);
	return AURIE_SUCCESS;
}
#include <map>
#include <YYToolkit/YYTK_Shared.hpp> // YYTK v4
using namespace Aurie;
using namespace YYTK;

static const char* const MOD_NAME = "InfiniteHealth";
static const char* const VERSION = "1.0.2";
static const char* const GML_SCRIPT_IN_DARK_MINES = "gml_Script_in_dark_mines";
static const char* const GML_SCRIPT_MODIFY_HEALTH = "gml_Script_PostProcessor";
static const char* const GML_SCRIPT_POST_PROCESSOR_DRAW = "gml_Script_setup_post_process_database"; //"gml_Script_post_process_by_location";

// gml_Script_is_dungeon_room

static YYTKInterface* g_ModuleInterface = nullptr;
static bool darkness = false;
static std::map<std::string, RValue> post_processor_map = {};
static std::vector<std::string> struct_field_names = {};

bool GetStructFieldNames(
	IN const char* MemberName,
	IN OUT RValue* Value
)
{
	struct_field_names.push_back(MemberName);
	return false;
}

bool EnumFunction(
	IN const char* MemberName,
	IN OUT RValue* Value
)
{
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "Member Name: %s", MemberName);
	return false;
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

RValue& GmlScriptInGameMusicSelectorCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_POST_PROCESSOR_DRAW));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	struct_field_names = {};
	g_ModuleInterface->EnumInstanceMembers(Result, GetStructFieldNames);
	for (std::string field : struct_field_names)
	{
		if (StructVariableExists(Result, field.c_str()))
		{
			if (!post_processor_map.contains(field))
				post_processor_map[field] = Result.GetMember(field);
		}
	}


	//g_ModuleInterface->Print(CM_WHITE, "=============== Result ===============");
	//if (Result.m_Kind == VALUE_OBJECT)
	//{
	//	g_ModuleInterface->Print(CM_AQUA, "OBJECT");
	//	g_ModuleInterface->EnumInstanceMembers(Result, EnumFunction);
	//	g_ModuleInterface->Print(CM_WHITE, "------------------------------");
	//}
	//else if (Result.m_Kind == VALUE_ARRAY)
	//{
	//	RValue array_length = g_ModuleInterface->CallBuiltin("array_length", { Result });
	//	g_ModuleInterface->Print(CM_AQUA, "ARRAY (length == %d)", static_cast<int>(array_length.m_Real));
	//	for (int i = 0; i < array_length.m_Real; i++)
	//	{
	//		RValue array_element = g_ModuleInterface->CallBuiltin("array_get", { Result[i], i });
	//		if (array_element.m_Kind == VALUE_OBJECT)
	//		{
	//			g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: OBJECT", i);
	//			g_ModuleInterface->EnumInstanceMembers(array_element, EnumFunction);
	//			g_ModuleInterface->Print(CM_WHITE, "------------------------------");
	//		}
	//		else if (array_element.m_Kind == VALUE_REAL)
	//			g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: REAL: %f", i, array_element.m_Real);
	//		else if (array_element.m_Kind == VALUE_INT64)
	//			g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: INT64: %d", i, array_element.m_i64);
	//		else if (array_element.m_Kind == VALUE_INT32)
	//			g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: INT32: %d", i, array_element.m_i32);
	//		else if (array_element.m_Kind == VALUE_BOOL)
	//			g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: BOOL: %s", i, array_element.m_Real == 0 ? "false" : "true");
	//		else if (array_element.m_Kind == VALUE_STRING)
	//			g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: STRING: %s", i, array_element.ToString().c_str());
	//		else if (array_element.m_Kind == VALUE_REF)
	//			g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: REFERENCE", i);
	//		else if (array_element.m_Kind == VALUE_NULL)
	//			g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: NULL", i);
	//		else if (array_element.m_Kind == VALUE_UNDEFINED)
	//			g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: UNDEFINED", i);
	//		else if (array_element.m_Kind == VALUE_UNSET)
	//			g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: UNSET", i);
	//		else if (array_element.m_Kind == VALUE_ARRAY)
	//			g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: ARRAY", i);
	//		else
	//			g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: OTHER", i);
	//	}
	//}
	//else if (Result.m_Kind == VALUE_INT32)
	//	g_ModuleInterface->Print(CM_AQUA, "INT32 == %d", Result.m_i32);
	//else if (Result.m_Kind == VALUE_INT64)
	//	g_ModuleInterface->Print(CM_AQUA, "INT64 == %d", Result.m_i64);
	//else if (Result.m_Kind == VALUE_REAL)
	//	g_ModuleInterface->Print(CM_AQUA, "REAL == %f", Result.m_Real);
	//else if (Result.m_Kind == VALUE_BOOL)
	//	g_ModuleInterface->Print(CM_AQUA, "BOOL == %s", Result.m_Real == 0 ? "false" : "true");
	//else if (Result.m_Kind == VALUE_STRING)
	//	g_ModuleInterface->Print(CM_AQUA, "STRING == %s", Result.ToString().c_str());
	//else
	//	g_ModuleInterface->Print(CM_AQUA, "OTHER");

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
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	if (StructVariableExists(global_instance, "post_process"))
	{
		RValue post_process = *global_instance->GetRefMember("post_process");
		g_ModuleInterface->EnumInstanceMembers(post_process, EnumFunction);
		int temp = 5;
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

RValue& GmlScriptIsDungeonRoomCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	//g_ModuleInterface->Print(CM_WHITE, "ENTER: %s", GML_SCRIPT_MODIFY_HEALTH);
	//g_ModuleInterface->Print(CM_WHITE, "=============== %s ===============", "Self");
	//g_ModuleInterface->EnumInstanceMembers(Self, EnumFunction);
	//g_ModuleInterface->Print(CM_WHITE, "=============== %s ===============", "Other");
	//g_ModuleInterface->EnumInstanceMembers(Other, EnumFunction);

	//for (int i = 0; i < ArgumentCount; i++)
	//{
	//	g_ModuleInterface->Print(CM_WHITE, "=============== Argument[%d] ===============", i);
	//	if (Arguments[i]->m_Kind == VALUE_OBJECT)
	//	{
	//		g_ModuleInterface->Print(CM_AQUA, "OBJECT:");
	//		struct_field_names = {};
	//		g_ModuleInterface->EnumInstanceMembers(Arguments[i]->m_Object, GetStructFieldNames);
	//		for (int j = 0; j < struct_field_names.size(); j++)
	//		{
	//			std::string field_name = struct_field_names[j];
	//			RValue field = *Arguments[i]->GetRefMember(field_name);
	//			if (field.m_Kind == VALUE_OBJECT)
	//			{
	//				g_ModuleInterface->Print(CM_AQUA, "%s: OBJECT", field_name.c_str());
	//				g_ModuleInterface->EnumInstanceMembers(field, EnumFunction);
	//				g_ModuleInterface->Print(CM_WHITE, "------------------------------");
	//			}
	//			else if (field.m_Kind == VALUE_ARRAY)
	//			{
	//				RValue array_length = g_ModuleInterface->CallBuiltin("array_length", { field });
	//				g_ModuleInterface->Print(CM_AQUA, "%s: ARRAY (length == %d)", field_name.c_str(), static_cast<int>(array_length.m_Real));
	//				for (int k = 0; k < array_length.m_Real; k++)
	//				{
	//					//INT64 == 956
	//					RValue array_element = g_ModuleInterface->CallBuiltin("array_get", { field, k });
	//					int temp = 5;
	//				}
	//			}
	//			else if (field.m_Kind == VALUE_INT32)
	//				g_ModuleInterface->Print(CM_AQUA, "%s: INT32 == %d", field_name.c_str(), field.m_i32);
	//			else if (field.m_Kind == VALUE_INT64)
	//				g_ModuleInterface->Print(CM_AQUA, "%s: INT64 == %d", field_name.c_str(), field.m_i64);
	//			else if (field.m_Kind == VALUE_REAL)
	//				g_ModuleInterface->Print(CM_AQUA, "%s: REAL == %f", field_name.c_str(), field.m_Real);
	//			else if (field.m_Kind == VALUE_BOOL)
	//				g_ModuleInterface->Print(CM_AQUA, "%s: BOOL == %s", field_name.c_str(), field.m_Real == 0 ? "false" : "true");
	//			else if (field.m_Kind == VALUE_STRING)
	//				g_ModuleInterface->Print(CM_AQUA, "%s: STRING == %s", field_name.c_str(), field.ToString().c_str());
	//			else if (field.m_Kind == VALUE_REF)
	//				g_ModuleInterface->Print(CM_AQUA, "%s: REFERENCE", field_name.c_str());
	//			else if (field.m_Kind == VALUE_NULL)
	//				g_ModuleInterface->Print(CM_AQUA, "%s: NULL", field_name.c_str());
	//			else if (field.m_Kind == VALUE_UNDEFINED)
	//				g_ModuleInterface->Print(CM_AQUA, "%s: UNDEFINED", field_name.c_str());
	//			else if (field.m_Kind == VALUE_UNSET)
	//				g_ModuleInterface->Print(CM_AQUA, "%s: UNSET", field_name.c_str());
	//			else
	//				g_ModuleInterface->Print(CM_AQUA, "%s: OTHER", field_name.c_str());
	//		}
	//	}
	//	else if (Arguments[i]->m_Kind == VALUE_ARRAY)
	//	{
	//		RValue array_length = g_ModuleInterface->CallBuiltin("array_length", { *Arguments[i] });
	//		g_ModuleInterface->Print(CM_AQUA, "ARRAY (length == %d)", static_cast<int>(array_length.m_Real));
	//		for (int j = 0; j < array_length.m_Real; j++)
	//		{
	//			RValue array_element = g_ModuleInterface->CallBuiltin("array_get", { *Arguments[i], j });
	//			if (array_element.m_Kind == VALUE_OBJECT)
	//			{
	//				g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: OBJECT", j);
	//				g_ModuleInterface->EnumInstanceMembers(array_element, EnumFunction);
	//				g_ModuleInterface->Print(CM_WHITE, "------------------------------");
	//			}
	//			else if (array_element.m_Kind == VALUE_REAL)
	//				g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: REAL: %f", j, array_element.m_Real);
	//			else if (array_element.m_Kind == VALUE_INT64)
	//				g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: INT64: %d", j, array_element.m_i64);
	//			else if (array_element.m_Kind == VALUE_INT32)
	//				g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: INT32: %d", j, array_element.m_i32);
	//			else if (array_element.m_Kind == VALUE_BOOL)
	//				g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: BOOL: %s", j, array_element.m_Real == 0 ? "false" : "true");
	//			else if (array_element.m_Kind == VALUE_STRING)
	//				g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: STRING: %s", j, array_element.ToString().c_str());
	//			else if (array_element.m_Kind == VALUE_REF)
	//				g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: REFERENCE", j);
	//			else if (array_element.m_Kind == VALUE_NULL)
	//				g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: NULL", j);
	//			else if (array_element.m_Kind == VALUE_UNDEFINED)
	//				g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: UNDEFINED", j);
	//			else if (array_element.m_Kind == VALUE_ARRAY)
	//				g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: ARRAY", j);
	//			else if (array_element.m_Kind == VALUE_UNSET)
	//				g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: UNSET", j);
	//			else
	//				g_ModuleInterface->Print(CM_AQUA, "ARRAY[%d]: OTHER", j);
	//		}
	//	}
	//	else if (Arguments[i]->m_Kind == VALUE_REAL)
	//		g_ModuleInterface->Print(CM_AQUA, "REAL: %f", Arguments[i]->m_Real);
	//	else if (Arguments[i]->m_Kind == VALUE_INT64)
	//		g_ModuleInterface->Print(CM_AQUA, "INT64: %d", Arguments[i]->m_i64);
	//	else if (Arguments[i]->m_Kind == VALUE_INT32)
	//		g_ModuleInterface->Print(CM_AQUA, "INT32: %d", Arguments[i]->m_i32);
	//	else if (Arguments[i]->m_Kind == VALUE_BOOL)
	//		g_ModuleInterface->Print(CM_AQUA, "BOOL: %s", Arguments[i]->m_Real == 0 ? "false" : "true");
	//	else if (Arguments[i]->m_Kind == VALUE_STRING)
	//		g_ModuleInterface->Print(CM_AQUA, "STRING: %s", Arguments[i]->ToString().c_str());
	//	else if (Arguments[i]->m_Kind == VALUE_REF)
	//		g_ModuleInterface->Print(CM_AQUA, "REFERENCE");
	//	else if (Arguments[i]->m_Kind == VALUE_NULL)
	//		g_ModuleInterface->Print(CM_AQUA, "NULL");
	//	else if (Arguments[i]->m_Kind == VALUE_UNDEFINED)
	//		g_ModuleInterface->Print(CM_AQUA, "UNDEFINED");
	//	else if (Arguments[i]->m_Kind == VALUE_UNSET)
	//		g_ModuleInterface->Print(CM_AQUA, "UNSET");
	//	else
	//		g_ModuleInterface->Print(CM_AQUA, "OTHER");
	//}


	*Arguments[0] = post_processor_map["dark_mines"];
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_MODIFY_HEALTH));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	darkness = true;
	return Result;
}

RValue& GmlScriptModifyHealthCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_IN_DARK_MINES));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	Result = true;
	return Result;
}

void CreateHookGmlScriptInGameMusicSelector(AurieStatus& status)
{
	CScript* gml_script_modify_health = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_POST_PROCESSOR_DRAW,
		(PVOID*)&gml_script_modify_health
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_POST_PROCESSOR_DRAW);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_POST_PROCESSOR_DRAW,
		gml_script_modify_health->m_Functions->m_ScriptFunction,
		GmlScriptInGameMusicSelectorCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_POST_PROCESSOR_DRAW);
	}
}

void CreateHookGmlScriptShowRoomTitle(AurieStatus& status)
{
	CScript* gml_Script_try_location_id_to_string = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_try_location_id_to_string",
		(PVOID*)&gml_Script_try_location_id_to_string
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[CharmSpell] - Failed to get script (gml_Script_try_location_id_to_string)!");
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_try_location_id_to_string",
		gml_Script_try_location_id_to_string->m_Functions->m_ScriptFunction,
		GmlScriptShowRoomTitleCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[CharmSpell] - Failed to hook script (gml_Script_try_location_id_to_string)!");
	}
}

void CreateHookGmlScriptIsDungeonRoom(AurieStatus& status)
{
	CScript* gml_script_modify_health = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_MODIFY_HEALTH,
		(PVOID*)&gml_script_modify_health
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_MODIFY_HEALTH);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_MODIFY_HEALTH,
		gml_script_modify_health->m_Functions->m_ScriptFunction,
		GmlScriptIsDungeonRoomCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_MODIFY_HEALTH);
	}
}

void CreateHookGmlScriptModifyHealth(AurieStatus& status)
{
	CScript* gml_script_modify_health = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_IN_DARK_MINES,
		(PVOID*)&gml_script_modify_health
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_IN_DARK_MINES);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_IN_DARK_MINES,
		gml_script_modify_health->m_Functions->m_ScriptFunction,
		GmlScriptModifyHealthCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_IN_DARK_MINES);
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

	CreateHookGmlScriptInGameMusicSelector(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[CharmSpell] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptShowRoomTitle(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[CharmSpell] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptIsDungeonRoom(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptModifyHealth(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Plugin started!", MOD_NAME, VERSION);
	return AURIE_SUCCESS;
}
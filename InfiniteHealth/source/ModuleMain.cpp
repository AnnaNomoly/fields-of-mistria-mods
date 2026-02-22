#include <YYToolkit/YYTK_Shared.hpp> // YYTK v4
using namespace Aurie;
using namespace YYTK;

static const char* const MOD_NAME = "InfiniteHealth";
static const char* const VERSION = "1.0.2";
static const char* const GML_SCRIPT_FACE_DIR = "gml_Script_face_dir@gml_Object_obj_ari_Create_0";
static const char* const GML_SCRIPT_SET_CARDINAL = "gml_Script_set_cardinal@gml_Object_obj_ari_Create_0";

static YYTKInterface* g_ModuleInterface = nullptr;

RValue& GmlScriptFaceDirCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_FACE_DIR));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptSetCardinalCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_SET_CARDINAL));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

void CreateHookGmlScriptFaceDir(AurieStatus& status)
{
	CScript* gml_script_modify_health = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_FACE_DIR,
		(PVOID*)&gml_script_modify_health
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_FACE_DIR);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_FACE_DIR,
		gml_script_modify_health->m_Functions->m_ScriptFunction,
		GmlScriptFaceDirCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_FACE_DIR);
	}
}

void CreateHookGmlScriptSetCardinal(AurieStatus& status)
{
	CScript* gml_script_modify_health = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SET_CARDINAL,
		(PVOID*)&gml_script_modify_health
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_SET_CARDINAL);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_SET_CARDINAL,
		gml_script_modify_health->m_Functions->m_ScriptFunction,
		GmlScriptSetCardinalCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_SET_CARDINAL);
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

	CreateHookGmlScriptFaceDir(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptSetCardinal(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Plugin started!", MOD_NAME, VERSION);
	return AURIE_SUCCESS;
}
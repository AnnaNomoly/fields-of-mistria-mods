#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static YYTKInterface* g_ModuleInterface = nullptr;

static const char* const VERSION = "1.0.0";

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

	if (GetAsyncKeyState(VK_NEXT) & 1)
	{
		std::string location = Result.AsString().data();
		if (location == "dungeon")
		{
			for (int i = 0; i < 100; i+=20)
			{
				for (int j = 0; j < 100; j+=10)
				{
					try
					{
						CScript* gml_Script_spawn_ladder = nullptr;
						g_ModuleInterface->GetNamedRoutinePointer(
							"gml_Script_spawn_ladder@DungeonRunner@DungeonRunner",
							(PVOID*)&gml_Script_spawn_ladder
						);

						RValue x = static_cast<double>(i);
						RValue y = static_cast<double>(j);
						RValue* x_ptr = &x;
						RValue* y_ptr = &y;
						RValue* rvalue_array[2] = { x_ptr, y_ptr };
						RValue retval;
						gml_Script_spawn_ladder->m_Functions->m_ScriptFunction(
							Self,
							Other,
							retval,
							2,
							rvalue_array
						);

						g_ModuleInterface->Print(CM_LIGHTGREEN, "[ChutesAndLadders %s] - Spawned ladder at (%d, %d)!", VERSION, i, j);
						return Result;
					}
					catch (...) {}
				}
			}
			g_ModuleInterface->Print(CM_LIGHTRED, "[ChutesAndLadders %s] - Found no suitable position for a ladder!", VERSION);
		}
	}
	
	return Result;
}

RValue& GmlScriptErrorCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	std::string error = Arguments[0]->AsString().data();
	if (error == "Failed to spawn a ladder in the dungeon!")
	{
		return Result;
	}
	else
	{
		const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_error"));
		original(
			Self,
			Other,
			Result,
			ArgumentCount,
			Arguments
		);

		return Result;
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[ChutesAndLadders %s] - Failed to get script (gml_Script_try_location_id_to_string)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[ChutesAndLadders %s] - Failed to hook script (gml_Script_try_location_id_to_string)!", VERSION);
	}
}

void CreateHookGmlScriptError(AurieStatus& status)
{
	CScript* gml_script_error = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_error",
		(PVOID*)&gml_script_error
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ChutesAndLadders %s] - Failed to get script (gml_Script_error)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_error",
		gml_script_error->m_Functions->m_ScriptFunction,
		GmlScriptErrorCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ChutesAndLadders %s] - Failed to hook script (gml_Script_error)!", VERSION);
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

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[ChutesAndLadders %s] - Plugin starting...", VERSION);
	
	CreateHookGmlScriptTryLocationIdToString(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ChutesAndLadders %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptError(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ChutesAndLadders %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[ChutesAndLadders %s] - Plugin started!", VERSION);
	return AURIE_SUCCESS;
}
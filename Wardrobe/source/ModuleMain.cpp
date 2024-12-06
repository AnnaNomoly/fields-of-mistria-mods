#include <map>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static YYTKInterface* g_ModuleInterface = nullptr;

static const char* const VERSION = "1.0.0";

static bool load_items = true;
static std::vector<std::string> cosmetic_names;

bool EnumFunction(
	IN const char* MemberName,
	IN OUT RValue* Value
)
{
	auto it = std::find(cosmetic_names.begin(), cosmetic_names.end(), MemberName);
	if (it == cosmetic_names.end())
	{
		cosmetic_names.push_back(MemberName);
	}
	return false;
}

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

	if (GetAsyncKeyState(VK_F9) & 1)
	{
		RValue user_input_string = g_ModuleInterface->CallBuiltin(
			"get_string",
			{
				"Input the desired cosmetic's internal name.\r\n"
				"(Ex.: dress_maid)\r\n"
				"More Info: https://github.com/AnnaNomoly/YYToolkit/tree/stable/Wardrobe",
				"dress_maid"
			}
		);

		auto it = std::find(cosmetic_names.begin(), cosmetic_names.end(), user_input_string.AsString().data());
		if (it != cosmetic_names.end())
		{
			CInstance* global_instance = nullptr;
			g_ModuleInterface->GetGlobalInstance(&global_instance);

			RValue __ari = global_instance->at("__ari").m_Object;
			RValue cosmetic_unlocks = __ari.at("cosmetic_unlocks");
			RValue inner = cosmetic_unlocks.at("inner");
			RValue target_cosmetic = inner.at(user_input_string.AsString().data());

			RValue cosmetic_unlocked = g_ModuleInterface->CallBuiltin(
				"struct_exists", {
					inner, user_input_string.AsString().data()
				}
			);
			
			if(cosmetic_unlocked.m_Kind == VALUE_BOOL && cosmetic_unlocked.m_Real == 0)
			{
				RValue zero = 0.0;
				g_ModuleInterface->CallBuiltin(
					"struct_set", {
						inner, user_input_string.AsString().data(), zero
					}
				);
				g_ModuleInterface->Print(CM_LIGHTGREEN, "[Wardrobe %s] - Unlocked recipe: %s", VERSION, user_input_string.AsString().data());
			}
			else {
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[Wardrobe %s] - Ignoring already known recipe: %s", VERSION, user_input_string.AsString().data());
			}
		}
		else {
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[Wardrobe %s] - Ignoring invalid cosmetic: %s", VERSION, user_input_string.AsString().data());

		}
	}

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
	if (load_items)
	{
		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);
		
		RValue __pad = global_instance->at("__pad");
		RValue player_assets = __pad.at("player_assets");
		RValue inner = player_assets.at("inner");

		g_ModuleInterface->EnumInstanceMembers(inner, EnumFunction);
			
		if (cosmetic_names.size() > 0)
		{
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[Wardrobe %s] - Loaded data for %d cosmetics!", VERSION, cosmetic_names.size());
		}
		else {
			g_ModuleInterface->Print(CM_LIGHTRED, "[Wardrobe %s] - Failed to load data for recipes!", VERSION);
		}

		load_items = false;
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

void CreateHookGmlScriptTryLocationIdToString(AurieStatus& status)
{
	CScript* gml_script_try_location_id_to_string = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_try_location_id_to_string",
		(PVOID*)&gml_script_try_location_id_to_string
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[Wardrobe %s] - Failed to get script (gml_Script_try_location_id_to_string)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[Wardrobe %s] - Failed to hook script (gml_Script_try_location_id_to_string)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[Wardrobe %s] - Failed to get script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[Wardrobe %s] - Failed to hook script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
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

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[Wardrobe %s] - Plugin starting...", VERSION);
	
	CreateHookGmlScriptTryLocationIdToString(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[Wardrobe %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[Wardrobe %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[Wardrobe %s] - Plugin started!", VERSION);
	return AURIE_SUCCESS;
}
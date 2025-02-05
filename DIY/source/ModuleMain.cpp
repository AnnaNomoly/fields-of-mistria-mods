#include <map>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static YYTKInterface* g_ModuleInterface = nullptr;

static const char* const VERSION = "1.0.0";

static bool load_items = true;
static std::map<std::string, std::vector<int>> item_name_to_id_map = {};

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

	if (GetAsyncKeyState(VK_F4) & 1)
	{
		RValue user_input_string = g_ModuleInterface->CallBuiltin(
			"get_string",
			{
				"Input the desired furniture's internal name.\r\n"
				"(Ex.: summer_bed)\r\n"
				"More Info: https://github.com/AnnaNomoly/YYToolkit/tree/stable/Cookbook",
				"summer_bed"
			}
		);

		if (item_name_to_id_map.count(user_input_string.AsString().data()) > 0)
		{
			CInstance* global_instance = nullptr;
			g_ModuleInterface->GetGlobalInstance(&global_instance);

			RValue __ari = global_instance->at("__ari").m_Object;
			RValue recipe_unlocks = __ari.at("recipe_unlocks");

			bool new_furniture_unlocked = false;
			std::vector furniture_ids = item_name_to_id_map[user_input_string.AsString().data()];

			for (int id : furniture_ids)
			{
				if (recipe_unlocks[id].m_Real == 0.0)
				{
					recipe_unlocks[id] = 1.0; // This value is ultimately what unlocks the furniture.
					new_furniture_unlocked = true;
				}
			}

			if (new_furniture_unlocked)
			{
				// This script actually just displays the "recipe unlocked!" window.
				CScript* gml_script_unlock_recipe = nullptr;
				g_ModuleInterface->GetNamedRoutinePointer(
					"gml_Script_unlock_recipe@Ari@Ari",
					(PVOID*)&gml_script_unlock_recipe
				);

				RValue* in = new RValue(static_cast<double>(furniture_ids[0])); // Can just use the first furniture ID associated with the internal name
				RValue out;
				gml_script_unlock_recipe->m_Functions->m_ScriptFunction(
					Self,
					Other,
					out,
					1,
					{ &in }
				);

				delete in;
				g_ModuleInterface->Print(CM_LIGHTGREEN, "[DIY %s] - Unlocked recipe: %s", VERSION, user_input_string.AsString().data());
			}
			else {
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[DIY %s] - Ignoring already known recipe: %s", VERSION, user_input_string.AsString().data());
			}
		}
		else {
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[DIY %s] - Ignoring invalid recipe: %s", VERSION, user_input_string.AsString().data());
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
		
		RValue __item_data = global_instance->at("__item_data");

		size_t array_length;
		g_ModuleInterface->GetArraySize(__item_data, array_length);

		for (size_t i = 0; i < array_length; i++)
		{
			RValue* array_element;
			g_ModuleInterface->GetArrayEntry(__item_data, i, array_element);

			RValue name_key = array_element->at("name_key");
			if (name_key.m_Kind != VALUE_NULL && name_key.m_Kind != VALUE_UNDEFINED && name_key.m_Kind != VALUE_UNSET)
			{
				if (strstr(name_key.AsString().data(), "furniture"))
				{
					RValue recipe = array_element->at("recipe");
					if (recipe.m_Kind != VALUE_NULL && recipe.m_Kind != VALUE_UNDEFINED && recipe.m_Kind != VALUE_UNSET)
					{
						RValue item_id = recipe.at("item_id");
						RValue recipe_key = array_element->at("recipe_key");

						if (item_name_to_id_map.count(recipe_key.AsString().data()) <= 0)
							item_name_to_id_map[recipe_key.AsString().data()] = {};

						item_name_to_id_map[recipe_key.AsString().data()].push_back(item_id.m_i64);
					}
				}
			}
		}

		if (item_name_to_id_map.size() > 0)
		{
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[DIY %s] - Loaded data for %d recipes!", VERSION, item_name_to_id_map.size());
		}
		else {
			g_ModuleInterface->Print(CM_LIGHTRED, "[DIY %s] - Failed to load data for recipes!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[DIY %s] - Failed to get script (gml_Script_try_location_id_to_string)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[DIY %s] - Failed to hook script (gml_Script_try_location_id_to_string)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[DIY %s] - Failed to get script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[DIY %s] - Failed to hook script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
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

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[DIY %s] - Plugin starting...", VERSION);
	
	CreateHookGmlScriptTryLocationIdToString(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DIY %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DIY %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[DIY %s] - Plugin started!", VERSION);
	return AURIE_SUCCESS;
}
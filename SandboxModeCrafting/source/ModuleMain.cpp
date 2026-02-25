#include <YYToolkit/YYTK_Shared.hpp> // YYTK v4
using namespace Aurie;
using namespace YYTK;

static const char* const MOD_NAME = "SandboxModeCrafting";
static const char* const VERSION = "1.0.0";
static const char* const GML_SCRIPT_PAY_COMPONENT_COSTS = "gml_Script_pay_component_costs";
static const char* const GML_SCRIPT_MAXIMUM_CRAFTS = "gml_Script_maximum_crafts@CraftingMenu@CraftingMenu";
static const char* const GML_SCRIPT_CHECK_ITEM_CRAFTABLE = "gml_Script_check_item_craftable@CraftingMenu@CraftingMenu";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";

static YYTKInterface* g_ModuleInterface = nullptr;
static CInstance* global_instance = nullptr;
static bool modify_item_costs = true;

bool StructVariableExists(RValue the_struct, const char* variable_name)
{
	RValue struct_exists = g_ModuleInterface->CallBuiltin(
		"struct_exists",
		{ the_struct, variable_name }
	);

	return struct_exists.ToBoolean();
}

RValue StructVariableGet(RValue the_struct, std::string variable_name)
{
	return g_ModuleInterface->CallBuiltin(
		"struct_get",
		{ the_struct, RValue(variable_name) }
	);
}

RValue StructVariableSet(RValue the_struct, const char* variable_name, RValue value)
{
	return g_ModuleInterface->CallBuiltin(
		"struct_set",
		{ the_struct, variable_name, value }
	);
}

void ModifyItemCosts()
{
	size_t array_length;
	RValue item_data = *global_instance->GetRefMember("__item_data");
	g_ModuleInterface->GetArraySize(item_data, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue item = g_ModuleInterface->CallBuiltin("array_get", { item_data, i });
		if (item.m_Kind == VALUE_OBJECT && StructVariableExists(item, "recipe"))
		{
			RValue recipe = StructVariableGet(item, "recipe");
			if (recipe.m_Kind == VALUE_OBJECT && StructVariableExists(recipe, "components"))
			{
				RValue components = StructVariableGet(recipe, "components");
				if (components.m_Kind == VALUE_OBJECT && StructVariableExists(components, "__buffer"))
				{
					RValue buffer = StructVariableGet(components, "__buffer");
					if (buffer.m_Kind == VALUE_ARRAY)
					{
						size_t buffer_array_length;
						g_ModuleInterface->GetArraySize(buffer, buffer_array_length);

						for (size_t j = 0; j < buffer_array_length; j++)
						{
							RValue buffer_element = g_ModuleInterface->CallBuiltin("array_get", { buffer, j });
							if(buffer_element.m_Kind == VALUE_OBJECT && StructVariableExists(buffer_element, "duration"))
								StructVariableSet(buffer_element, "duration", 0);
						}
					}
				}
			}
		}
	}
}

RValue& GmlScriptPayComponentCostsCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	return Result;
}

RValue& GmlScriptMaximumCraftsCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	Result = 999;
	return Result;
}

RValue& GmlScriptCheckItemCraftableCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	Result = true;
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
	if (modify_item_costs)
	{
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		ModifyItemCosts();
		modify_item_costs = false;
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

void CreateHookGmlScriptPayComponentCosts(AurieStatus& status)
{
	CScript* gml_script_pay_component_costs = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_PAY_COMPONENT_COSTS,
		(PVOID*)&gml_script_pay_component_costs
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_PAY_COMPONENT_COSTS);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_PAY_COMPONENT_COSTS,
		gml_script_pay_component_costs->m_Functions->m_ScriptFunction,
		GmlScriptPayComponentCostsCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_PAY_COMPONENT_COSTS);
	}
}

void CreateHookGmlScriptMaximumCrafts(AurieStatus& status)
{
	CScript* gml_script_maximum_crafts = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_MAXIMUM_CRAFTS,
		(PVOID*)&gml_script_maximum_crafts
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_MAXIMUM_CRAFTS);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_MAXIMUM_CRAFTS,
		gml_script_maximum_crafts->m_Functions->m_ScriptFunction,
		GmlScriptMaximumCraftsCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_MAXIMUM_CRAFTS);
	}
}

void CreateHookGmlScriptCheckItemCraftable(AurieStatus& status)
{
	CScript* gml_script_check_item_craftable = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CHECK_ITEM_CRAFTABLE,
		(PVOID*)&gml_script_check_item_craftable
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CHECK_ITEM_CRAFTABLE);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_CHECK_ITEM_CRAFTABLE,
		gml_script_check_item_craftable->m_Functions->m_ScriptFunction,
		GmlScriptCheckItemCraftableCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CHECK_ITEM_CRAFTABLE);
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

	CreateHookGmlScriptPayComponentCosts(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptMaximumCrafts(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptCheckItemCraftable(status);
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
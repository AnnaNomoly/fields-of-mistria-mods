#include <map>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static YYTKInterface* g_ModuleInterface = nullptr;

static const char* const MOD_NAME = "DigUpAnything";
static const char* const VERSION = "1.1.0";
static const std::string VALID_ITEM_LOCALIZATION_KEY = "mods/DigUpAnything/valid_item";
static const std::string DISABLED_ITEM_LOCALIZATION_KEY = "mods/DigUpAnything/disabled_item";
static const std::string UNRECOGNIZED_ITEM_LOCALIZATION_KEY = "mods/DigUpAnything/unrecognized_item";
static const std::string ITEM_NOT_ACQUIRED_LOCALIZATION_KEY = "mods/DigUpAnything/item_not_acquired";
static const std::string UNACQUIRED_ITEM_ALREADY_SPAWNED_LOCALIZATION_KEY = "mods/DigUpAnything/unacquired_item_already_spawned";
static const std::string DISABLED_ITEMS[] = { "animal_cosmetic", "cosmetic", "crafting_scroll", "purse", "recipe_scroll", "unidentified_artifact" };

static RValue __YYTK;
static int item_id = 0;
static bool load_on_start = true;
static bool override_next_dig_spot = false;
static bool duplicate_item = false;
static std::map<std::string, int> item_name_to_id_map = {};
static std::map<int, std::string> item_id_to_name_map = {};
static std::vector<std::string> unacquired_items_spawned = {};

void ResetStaticFields()
{
	item_id = 0;
	override_next_dig_spot = false;
	duplicate_item = false;
	std::vector<std::string> unacquired_items_spawned = {};
}

bool RValueAsBool(RValue value)
{
	if (value.m_Kind == VALUE_BOOL && value.m_Real == 1)
		return true;
	return false;
}

bool GlobalVariableExists(const char* variable_name)
{
	RValue global_variable_exists = g_ModuleInterface->CallBuiltin(
		"variable_global_exists",
		{ variable_name }
	);

	return RValueAsBool(global_variable_exists);
}

RValue GlobalVariableGet(const char* variable_name)
{
	return g_ModuleInterface->CallBuiltin(
		"variable_global_get",
		{ variable_name }
	);
}

RValue GlobalVariableSet(const char* variable_name, RValue value)
{
	return g_ModuleInterface->CallBuiltin(
		"variable_global_set",
		{ variable_name, value }
	);
}

bool StructVariableExists(RValue the_struct, const char* variable_name)
{
	RValue struct_exists = g_ModuleInterface->CallBuiltin(
		"struct_exists",
		{ the_struct, variable_name }
	);

	return RValueAsBool(struct_exists);
}

RValue StructVariableGet(RValue the_struct, const char* variable_name)
{
	return g_ModuleInterface->CallBuiltin(
		"struct_get",
		{ the_struct, variable_name }
	);
}

RValue StructVariableSet(RValue the_struct, const char* variable_name, RValue value)
{
	return g_ModuleInterface->CallBuiltin(
		"struct_set",
		{ the_struct, variable_name, value }
	);
}

void CreateOrGetGlobalYYTKVariable()
{
	if (!GlobalVariableExists("__YYTK"))
	{
		g_ModuleInterface->GetRunnerInterface().StructCreate(&__YYTK);
		GlobalVariableSet("__YYTK", __YYTK);
	}
	else
		__YYTK = GlobalVariableGet("__YYTK");
}

void CreateModInfoInGlobalYYTKVariable()
{
	if (!StructVariableExists(__YYTK, MOD_NAME))
	{
		RValue DigUpAnything;
		RValue version = VERSION;
		g_ModuleInterface->GetRunnerInterface().StructCreate(&DigUpAnything);
		g_ModuleInterface->GetRunnerInterface().StructAddRValue(&DigUpAnything, "version", &version);
		g_ModuleInterface->GetRunnerInterface().StructAddRValue(&__YYTK, MOD_NAME, &DigUpAnything);
	}
}

void DisplayNotification(CInstance* Self, CInstance* Other, std::string localization_key)
{
	CScript* gml_script_create_notification = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_create_notification",
		(PVOID*)&gml_script_create_notification
	);

	RValue result;
	RValue notification = localization_key;
	RValue* notification_ptr = &notification;
	gml_script_create_notification->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &notification_ptr }
	);
}

bool ItemHasBeenAcquired(int item_id)
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	RValue __ari = global_instance->at("__ari");
	RValue items_acquired = __ari.at("items_acquired");
	RValue item_acquired = g_ModuleInterface->CallBuiltin("array_get", { items_acquired, item_id });
	return RValueAsBool(item_acquired);
}

void UpdateUnacquiredItemsSpawnedList(int item_id)
{
	bool item_has_been_acquired = ItemHasBeenAcquired(item_id);
	if(item_has_been_acquired)
		unacquired_items_spawned.erase(std::remove(unacquired_items_spawned.begin(), unacquired_items_spawned.end(), item_id_to_name_map[item_id]), unacquired_items_spawned.end());
}

bool LiveItemHasId(RValue live_item, int item_id)
{
	int value = -1;
	RValue live_item_id = live_item.at("item_id");
	if (live_item_id.m_Kind == VALUE_REAL)
		value = live_item_id.m_Real;
	if (live_item_id.m_Kind == VALUE_INT64)
		value = live_item_id.m_i64;
	if (live_item_id.m_Kind == VALUE_INT32)
		value = live_item_id.m_i32;
	return value == item_id;
}

RValue& GmlScriptGiveItemCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (duplicate_item && (LiveItemHasId(Arguments[0]->m_Object, item_id) || LiveItemHasId(Arguments[0]->m_Object, item_name_to_id_map["unidentified_artifact"])))
	{
		duplicate_item = false;
		if (ItemHasBeenAcquired(item_id))
		{
			RValue user_input = g_ModuleInterface->CallBuiltin(
				"get_integer",
				{
					"Dig Up Anything\r\n"
					"--------------------\r\n"
					"How many of the item would you like?\r\n"
					"Input a number between 1 and 999.\r\n",
					1
				}
			);
			
			double value = user_input.m_Real;
			if (value < 1)
				value = 1;
			if (value > 999)
				value = 999;

			Arguments[1]->m_Real = value;
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[DigUpAnything %s] - Spawning %d of the requested item.", VERSION, static_cast<int>(value));
		}
		else
		{
			unacquired_items_spawned.push_back(item_id_to_name_map[item_id]);
			DisplayNotification(Self, Other, ITEM_NOT_ACQUIRED_LOCALIZATION_KEY);
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[DigUpAnything %s] - The requested item hasn't been acquired before. Only 1 may be spawned.", VERSION);
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_give_item@Ari@Ari"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptChooseRandomArtifactCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_choose_random_artifact@Archaeology@Archaeology"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (override_next_dig_spot)
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DigUpAnything %s] - Modified the artifact spot!", VERSION);
		Result.m_i64 = item_id;
		override_next_dig_spot = false;
		duplicate_item = true;
	}

	return Result;
}

RValue& GmlScriptHeldItemCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (GetAsyncKeyState(VK_F11) & 1)
	{
		RValue user_input_string = g_ModuleInterface->CallBuiltin(
			"get_string",
			{
				"Dig Up Anything\r\n"
				"--------------------\r\n"
				"Input the desired item's internal name.\r\n"
				"Example: rosehip_jam\r\n"
				"More Info: https://github.com/AnnaNomoly/YYToolkit/tree/stable/DigUpAnything",
				"rosehip_jam"
			}
		);

		if (item_name_to_id_map.count(user_input_string.AsString().data()) > 0)
		{
			UpdateUnacquiredItemsSpawnedList(item_name_to_id_map[user_input_string.AsString().data()]);
			auto unacquired_item_already_spawned = std::find(std::begin(unacquired_items_spawned), std::end(unacquired_items_spawned), user_input_string.AsString().data());
			auto disabled_item = std::find(std::begin(DISABLED_ITEMS), std::end(DISABLED_ITEMS), user_input_string.AsString().data());
			
			if (unacquired_item_already_spawned != std::end(unacquired_items_spawned))
			{
				DisplayNotification(Self, Other, UNACQUIRED_ITEM_ALREADY_SPAWNED_LOCALIZATION_KEY);
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[DigUpAnything %s] - Ignoring duplicate unacquired item: %s", VERSION, user_input_string.AsString().data());
			}
			else if (disabled_item != std::end(DISABLED_ITEMS))
			{
				DisplayNotification(Self, Other, DISABLED_ITEM_LOCALIZATION_KEY);
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[DigUpAnything %s] - Ignoring disabled item: %s", VERSION, user_input_string.AsString().data());
			}
			else
			{
				// Indicate to other mods the dig site is being modified by DigUpAnything.
				if (StructVariableExists(__YYTK, "SecretSanta"))
				{
					RValue SecretSanta = StructVariableGet(__YYTK, "SecretSanta");
					RValue ignore_next_dig_spot = true;
					StructVariableSet(SecretSanta, "ignore_next_dig_spot", ignore_next_dig_spot);
				}
				if (StructVariableExists(__YYTK, "Curator"))
				{
					RValue Curator = StructVariableGet(__YYTK, "Curator");
					RValue ignore_next_dig_spot = true;
					StructVariableSet(Curator, "ignore_next_dig_spot", ignore_next_dig_spot);
				}
				
				item_id = item_name_to_id_map[user_input_string.AsString().data()];
				override_next_dig_spot = true;
				DisplayNotification(Self, Other, VALID_ITEM_LOCALIZATION_KEY);
				g_ModuleInterface->Print(CM_LIGHTGREEN, "[DigUpAnything %s] - Next artifact spot will be: %s", VERSION, user_input_string.AsString().data());
			}
		}
		else
		{
			DisplayNotification(Self, Other, UNRECOGNIZED_ITEM_LOCALIZATION_KEY);
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[DigUpAnything %s] - Ignoring unrecognized item: %s", VERSION, user_input_string.AsString().data());
		}

	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_held_item@Ari@Ari"));
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
		CreateOrGetGlobalYYTKVariable();
		CreateModInfoInGlobalYYTKVariable();

		for (int64_t i = 0; i < 3000; i++)
		{
			CScript* gml_script_try_item_id_to_string = nullptr;
			g_ModuleInterface->GetNamedRoutinePointer(
				"gml_Script_try_item_id_to_string",
				(PVOID*)&gml_script_try_item_id_to_string
			);

			RValue* item_id = new RValue(i);
			RValue item_name;
			gml_script_try_item_id_to_string->m_Functions->m_ScriptFunction(
				Self,
				Other,
				item_name,
				1,
				{ &item_id }
			);
			delete item_id;

			if (item_name.m_Kind != VALUE_NULL && item_name.m_Kind != VALUE_UNSET && item_name.m_Kind != VALUE_UNDEFINED)
			{
				const char* item_name_str = item_name.AsString().data();
				if (item_name_to_id_map.count(item_name_str) <= 0)
				{
					item_name_to_id_map[item_name_str] = i;
					item_id_to_name_map[i] = item_name_str;
				}
			}
		}
		if (item_name_to_id_map.size() > 0)
		{
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[DigUpAnything %s] - Loaded data for %d items!", VERSION, item_name_to_id_map.size());
		}
		else {
			g_ModuleInterface->Print(CM_LIGHTRED, "[DigUpAnything %s] - Failed to load data for items!", VERSION);
		}

		load_on_start = false;
	}
	else
	{
		ResetStaticFields();
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

void CreateHookGmlScriptGiveItem(AurieStatus& status)
{
	CScript* gml_script_give_item = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_give_item@Ari@Ari",
		(PVOID*)&gml_script_give_item
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DigUpAnything %s] - Failed to get script (gml_Script_give_item@Ari@Ari)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_give_item@Ari@Ari",
		gml_script_give_item->m_Functions->m_ScriptFunction,
		GmlScriptGiveItemCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DigUpAnything %s] - Failed to hook script (gml_Script_give_item@Ari@Ari)!", VERSION);
	}
}

void CreateHookGmlScriptChooseRandomArtifact(AurieStatus& status)
{
	CScript* gml_script_choose_random_artifact = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_choose_random_artifact@Archaeology@Archaeology",
		(PVOID*)&gml_script_choose_random_artifact
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DigUpAnything %s] - Failed to get script (gml_Script_choose_random_artifact@Archaeology@Archaeology)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_choose_random_artifact@Archaeology@Archaeology",
		gml_script_choose_random_artifact->m_Functions->m_ScriptFunction,
		GmlScriptChooseRandomArtifactCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DigUpAnything %s] - Failed to hook script (gml_Script_choose_random_artifact@Archaeology@Archaeology)!", VERSION);
	}
}

void CreateHookGmlScriptHeldItem(AurieStatus& status)
{
	CScript* gml_script_held_item = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_held_item@Ari@Ari",
		(PVOID*)&gml_script_held_item
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DigUpAnything %s] - Failed to get script (gml_Script_held_item@Ari@Ari)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_held_item@Ari@Ari",
		gml_script_held_item->m_Functions->m_ScriptFunction,
		GmlScriptHeldItemCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DigUpAnything %s] - Failed to hook script (gml_Script_held_item@Ari@Ari)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[DigUpAnything %s] - Failed to get script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[DigUpAnything %s] - Failed to hook script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
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

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[DigUpAnything %s] - Plugin starting...", VERSION);

	CreateHookGmlScriptGiveItem(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DigUpAnything %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptChooseRandomArtifact(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DigUpAnything %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptHeldItem(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DigUpAnything %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DigUpAnything %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[DigUpAnything %s] - Plugin started!", VERSION);
	return AURIE_SUCCESS;
}
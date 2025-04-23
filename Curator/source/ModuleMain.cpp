#include <map>
#include <random>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static const char* const VERSION = "1.0.0";
static const char* const MOD_NAME = "Curator";
static const char* const YYTK_KEY = "__YYTK";
static const char* const VERSION_KEY = "version";
static const char* const IGNORE_NEXT_DIG_SPOT_KEY = "ignore_next_dig_spot";
static const char* const UNPEATABLE = "unpeatable";
static const std::vector<std::string> DISABLED_ITEMS = { "perfect_mistril_ore", "perfect_pink_diamond" };
static const bool PERK_MOD_VARIANT = true; // Indicates if the mod uses the custom perk.

static YYTKInterface* g_ModuleInterface = nullptr;
static std::map<std::string, std::vector<int>> archaeology_set_to_items = {};
static std::map<int, std::string> item_id_to_archaeology_set = {};
static std::vector<std::string> struct_field_names = {};
static std::vector<int> disabled_item_ids = {};
static bool load_on_start = true;
static RValue __YYTK;
static std::mt19937 generator(std::random_device{}());
static std::map<std::string, bool> active_perk_map = {}; // Tracks which mod specific perks are active.
static std::map<std::string, int64_t> perk_name_to_id_map = {};

std::string RValueKindAsString(RValue value)
{
	if (value.m_Kind == VALUE_REAL)
		return "VALUE_REAL";
	if (value.m_Kind == VALUE_STRING)
		return "VALUE_STRING";
	if (value.m_Kind == VALUE_ARRAY)
		return "VALUE_ARRAY";
	if (value.m_Kind == VALUE_PTR)
		return "VALUE_PTR";
	if (value.m_Kind == VALUE_VEC3)
		return "VALUE_VEC3";
	if (value.m_Kind == VALUE_UNDEFINED)
		return "VALUE_UNDEFINED";
	if (value.m_Kind == VALUE_OBJECT)
		return "VALUE_OBJECT";
	if (value.m_Kind == VALUE_INT32)
		return "VALUE_INT32";
	if (value.m_Kind == VALUE_VEC4)
		return "VALUE_VEC4";
	if (value.m_Kind == VALUE_VEC44)
		return "VALUE_VEC44";
	if (value.m_Kind == VALUE_INT64)
		return "VALUE_INT64";
	if (value.m_Kind == VALUE_ACCESSOR)
		return "VALUE_ACCESSOR";
	if (value.m_Kind == VALUE_NULL)
		return "VALUE_NULL";
	if (value.m_Kind == VALUE_BOOL)
		return "VALUE_BOOL";
	if (value.m_Kind == VALUE_ITERATOR)
		return "VALUE_ITERATOR";
	if (value.m_Kind == VALUE_REF)
		return "VALUE_REF";
	if (value.m_Kind == VALUE_UNSET)
		return "VALUE_UNSET";
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
		RValue mod;
		RValue version = VERSION;
		RValue ignore_next_dig_spot = false;
		g_ModuleInterface->GetRunnerInterface().StructCreate(&mod);
		g_ModuleInterface->GetRunnerInterface().StructAddRValue(&mod, VERSION_KEY, &version);
		g_ModuleInterface->GetRunnerInterface().StructAddRValue(&mod, IGNORE_NEXT_DIG_SPOT_KEY, &ignore_next_dig_spot);
		g_ModuleInterface->GetRunnerInterface().StructAddRValue(&__YYTK, MOD_NAME, &mod);
	}
}

void ResetStaticFields()
{
	active_perk_map = {};
}

bool GetStructFieldNames(
	IN const char* MemberName,
	IN OUT RValue* Value
)
{
	struct_field_names.push_back(MemberName);
	return false;
}

bool IgnoreNextDigSpot()
{
	if (GlobalVariableExists(YYTK_KEY))
	{
		RValue __YYTK = GlobalVariableGet(YYTK_KEY);
		if (StructVariableExists(__YYTK, MOD_NAME))
		{
			RValue Curator = StructVariableGet(__YYTK, MOD_NAME);
			if (StructVariableExists(Curator, IGNORE_NEXT_DIG_SPOT_KEY))
			{
				RValue ignore_next_dig_spot = StructVariableGet(Curator, IGNORE_NEXT_DIG_SPOT_KEY);
				return RValueAsBool(ignore_next_dig_spot);
			}
		}
	}

	return false;
}

void ParseDisabledItemIds(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_try_string_to_item_id = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_try_string_to_item_id",
		(PVOID*)&gml_script_try_string_to_item_id
	);

	for (int i = 0; i < DISABLED_ITEMS.size(); i++)
	{
		RValue item_id;
		RValue item_name = DISABLED_ITEMS[i];
		RValue* item_name_ptr = &item_name;
		
		gml_script_try_string_to_item_id->m_Functions->m_ScriptFunction(
			Self,
			Other,
			item_id,
			1,
			{ &item_name_ptr }
		);

		if (item_id.m_Kind == VALUE_INT32)
			disabled_item_ids.push_back(item_id.m_i32);
		if (item_id.m_Kind == VALUE_INT64)
			disabled_item_ids.push_back(item_id.m_i64);
		if (item_id.m_Kind == VALUE_REAL)
			disabled_item_ids.push_back(item_id.m_Real);
	}
}

void ParsePerks()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	size_t array_length;
	RValue perks = global_instance->at("__perk__");
	g_ModuleInterface->GetArraySize(perks, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(perks, i, array_element);

		perk_name_to_id_map[array_element->AsString().data()] = i;
	}
}

void ParseMuseumData()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	// Find out the index for the archaeology museum wing.
	int archaeology_index = 0;
	RValue __museum_wing__ = global_instance->at("__museum_wing__");
	RValue __museum_wing__length = g_ModuleInterface->CallBuiltin("array_length", { __museum_wing__ });
	for (int i = 0; i < __museum_wing__length.m_Real; i++)
	{
		RValue array_entry = g_ModuleInterface->CallBuiltin("array_get", { __museum_wing__, i });
		if (strstr(array_entry.AsString().data(), "archaeology"))
		{
			archaeology_index = i;
			break;
		}
	}

	// Get the archaeology sets.
	RValue __museum_data = global_instance->at("__museum_data");
	RValue data = __museum_data.at("data");
	RValue archaeology_wing = g_ModuleInterface->CallBuiltin("array_get", { data, archaeology_index });
	RValue archaeology_sets = archaeology_wing.at("sets");
	RValue archaeology_sets_inner = archaeology_sets.at("inner");

	struct_field_names = {};
	g_ModuleInterface->EnumInstanceMembers(archaeology_sets_inner, GetStructFieldNames);
	for (int i = 0; i < struct_field_names.size(); i++)
	{
		archaeology_set_to_items[struct_field_names[i]] = {};
		RValue archaeology_set = archaeology_sets_inner.at(struct_field_names[i]);
		RValue items = archaeology_set.at("items");
		RValue items_length = g_ModuleInterface->CallBuiltin("array_length", { items });
		for (int j = 0; j < items_length.m_Real; j++)
		{
			RValue item = g_ModuleInterface->CallBuiltin("array_get", { items, j });
			if (item.m_Kind == VALUE_REAL)
			{
				archaeology_set_to_items[struct_field_names[i]].push_back(item.m_Real);
				item_id_to_archaeology_set[item.m_Real] = struct_field_names[i];
			}
			if (item.m_Kind == VALUE_INT64)
			{
				archaeology_set_to_items[struct_field_names[i]].push_back(item.m_i64);
				item_id_to_archaeology_set[item.m_i64] = struct_field_names[i];
			}
		}
	}
}

RValue GetMuseumProgress()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);
	return global_instance->at("__museum_progress_data");
}

bool PerkActive(CInstance* Self, CInstance* Other, std::string perk_name)
{
	int64_t perk_id = perk_name_to_id_map[perk_name];

	CScript* gml_script_perk_active = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_perk_active@Ari@Ari",
		(PVOID*)&gml_script_perk_active
	);

	RValue result;
	RValue input = perk_id;
	RValue* input_ptr = &input;
	gml_script_perk_active->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &input_ptr }
	);

	if (result.m_Kind == VALUE_BOOL && result.m_Real == 1.0)
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

	if (strstr(self->m_Object->m_Name, "obj_ari"))
	{
		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		// Update active perks.
		if (PerkActive(global_instance->at("__ari").m_Object, self, UNPEATABLE))
			active_perk_map[UNPEATABLE] = true;
		else
			active_perk_map[UNPEATABLE] = false;
	}
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

	bool modify_dig_spot = false;
	if (!IgnoreNextDigSpot())
	{
		if (PERK_MOD_VARIANT)
		{
			if (active_perk_map[UNPEATABLE])
			{
				modify_dig_spot = true;
			}
		}
		else
		{
			modify_dig_spot = true;
		}
	}


	if (modify_dig_spot)
	{
		if (Result.m_Kind == VALUE_INT64)
		{
			// Check if the artifact is a museum item.
			if (item_id_to_archaeology_set.count(Result.m_i64) > 0)
			{
				// Check if the artifact has already been donated.
				RValue museum_progress = GetMuseumProgress();
				RValue item_donated = g_ModuleInterface->CallBuiltin("array_get", { museum_progress, Result.m_i64 });
				if (RValueAsBool(item_donated))
				{
					// Make a list of candidate replacements from the set.
					std::string archaeology_set = item_id_to_archaeology_set[Result.m_i64];
					std::vector<int> archaeology_set_items = archaeology_set_to_items[archaeology_set];
					std::vector<int> candidate_replacement_items = {};
					for (int i = 0; i < archaeology_set_items.size(); i++)
					{
						RValue set_item_donated = g_ModuleInterface->CallBuiltin("array_get", { museum_progress, archaeology_set_items[i] });
						if (!RValueAsBool(set_item_donated) && std::find(disabled_item_ids.begin(), disabled_item_ids.end(), archaeology_set_items[i]) == disabled_item_ids.end())
						{
							candidate_replacement_items.push_back(archaeology_set_items[i]);
						}
					}

					if (candidate_replacement_items.size() > 0)
					{
						// Pick a random artifact from the candidates.
						std::uniform_int_distribution<int> distribution(0, candidate_replacement_items.size() - 1);
						int random = distribution(generator);
						Result.m_i64 = candidate_replacement_items[random];

						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Changed the donated artifact to a missing one from the matching set: %s!", MOD_NAME, VERSION, archaeology_set.c_str());
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - All artifacts have been already donated for the matching set: %s!", MOD_NAME, VERSION, archaeology_set.c_str());
					}
				}
			}
		}
		else
		{
			g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Expected gml_Script_choose_random_artifact Result to be type INT64, but it was %s!", MOD_NAME, VERSION, RValueKindAsString(Result).c_str());
		}
	}
	else
	{
		RValue Curator = StructVariableGet(__YYTK, MOD_NAME);
		RValue ignore_next_dig_spot = false;
		StructVariableSet(Curator, IGNORE_NEXT_DIG_SPOT_KEY, ignore_next_dig_spot);
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
	if (load_on_start)
	{
		CreateOrGetGlobalYYTKVariable();
		CreateModInfoInGlobalYYTKVariable();
		ParseMuseumData();
		ParseDisabledItemIds(Self, Other);
		ParsePerks();
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

void CreateObjectCallback(AurieStatus& status)
{
	status = g_ModuleInterface->CreateCallback(
		g_ArSelfModule,
		EVENT_OBJECT_CALL,
		ObjectCallback,
		0
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook (EVENT_OBJECT_CALL)!", MOD_NAME, VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (gml_Script_choose_random_artifact@Archaeology@Archaeology)!", MOD_NAME, VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (gml_Script_choose_random_artifact@Archaeology@Archaeology)!", MOD_NAME, VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", MOD_NAME, VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", MOD_NAME, VERSION);
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

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[%s %s] - Plugin starting...", MOD_NAME, VERSION);

	CreateObjectCallback(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptChooseRandomArtifact(status);
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
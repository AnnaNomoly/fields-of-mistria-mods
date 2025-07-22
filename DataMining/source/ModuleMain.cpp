#include <map>
#include <iostream>
#include <fstream>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static const char* const MOD_NAME = "DataMining";
static const char* const VERSION = "1.0.0";
static const char* GML_SCRIPT_GET_LOCALIZER = "gml_Script_get@Localizer@Localizer";
static const char* GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";

static YYTKInterface* g_ModuleInterface = nullptr;
static bool run_once = true;
static bool localize_items = false;
std::string data_mining_folder = "";
static std::vector<std::string> cosmetic_names;
static std::map<std::string, int> item_name_to_id_map = {};
static std::map<std::string, std::string> item_name_to_localized_name_map = {};
static std::map<std::string, std::string> cooking_recipe_name_to_localized_name_map = {};
static std::map<std::string, std::string> furniture_recipe_name_to_localized_name_map = {}; // All furniture
static std::map<std::string, std::string> craftable_furniture_recipe_name_to_localized_name_map = {}; // Craftable furniture (has a recipe)
static std::map<std::string, std::string> non_craftable_furniture_recipe_name_to_localized_name_map = {}; // Non-craftable furniture (has no recipe)
static std::map<std::string, std::string> cosmetic_name_to_localized_name_map = {};

bool GetAllCosmeticNames(
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

int RValueAsInt(RValue value)
{
	if (value.m_Kind == VALUE_REAL)
		return static_cast<int>(value.m_Real);
	if (value.m_Kind == VALUE_INT64)
		return static_cast<int>(value.m_i64);
	if (value.m_Kind == VALUE_INT32)
		return static_cast<int>(value.m_i32);
}

bool RValueAsBool(RValue value)
{
	if (value.m_Kind == VALUE_REAL && value.m_Real == 1)
		return true;
	if (value.m_Kind == VALUE_BOOL && value.m_Real == 1)
		return true;
	return false;
}

bool StructVariableExists(RValue the_struct, const char* variable_name)
{
	RValue struct_exists = g_ModuleInterface->CallBuiltin(
		"struct_exists",
		{ the_struct, variable_name }
	);

	return RValueAsBool(struct_exists);
}

RValue GetLocalizedString(CInstance* Self, CInstance* Other, std::string localization_key)
{
	CScript* gml_script_get_localizer = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_LOCALIZER,
		(PVOID*)&gml_script_get_localizer
	);

	RValue result;
	RValue input = localization_key;
	RValue* input_ptr = &input;
	gml_script_get_localizer->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &input_ptr }
	);

	return result;
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

	if (!strstr(self->m_Object->m_Name, "obj_ari"))
		return;
}

RValue& GmlScriptGetLocalizerCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (localize_items)
	{
		localize_items = false;

		// All items.
		std::string items_file_name = data_mining_folder + "\\" + "items.md";
		std::remove(items_file_name.c_str());
		std::ofstream items_outfile(items_file_name, std::ios::out);
		if (items_outfile.is_open())
		{
			for (auto& pair : item_name_to_localized_name_map)
			{
				RValue localized_name = GetLocalizedString(Self, Other, pair.second);
				std::string localized_name_str = localized_name.AsString().data();
				pair.second = localized_name_str;

				// Item ID, Internal Item Name, Localized Item Name
				items_outfile << "| " + std::to_string(item_name_to_id_map[pair.first]) + " | " + pair.first + " | " + pair.second + " |\n";
			}
		}
		items_outfile.close();

		// Cooking recipes.
		std::string cooking_file_name = data_mining_folder + "\\" + "cooking_recipes.md";
		std::remove(cooking_file_name.c_str());
		std::ofstream cooking_outfile(cooking_file_name, std::ios::out);
		if (cooking_outfile.is_open())
		{
			for (auto& pair : cooking_recipe_name_to_localized_name_map) {
				RValue localized_name = GetLocalizedString(Self, Other, pair.second);
				std::string localized_name_str = localized_name.AsString().data();
				pair.second = localized_name_str;

				// Internal Recipe Name, Localized Recipe Name
				cooking_outfile << "| " + pair.first + " | " + pair.second + " |\n";
			}
		}
		cooking_outfile.close();

		// All Furniture
		std::string furniture_file_name = data_mining_folder + "\\" + "all_furniture.md";
		std::remove(furniture_file_name.c_str());
		std::ofstream furniture_outfile(furniture_file_name, std::ios::out);
		if (furniture_outfile.is_open())
		{
			for (auto& pair : furniture_recipe_name_to_localized_name_map) {
				RValue localized_name = GetLocalizedString(Self, Other, pair.second);
				std::string localized_name_str = localized_name.AsString().data();
				pair.second = localized_name_str;

				// Internal Recipe Name, Localized Recipe Name
				furniture_outfile << "| " + pair.first + " | " + pair.second + " |\n";
			}
		}
		furniture_outfile.close();

		// Craftable Furniture
		std::string craftable_furniture_file_name = data_mining_folder + "\\" + "craftable_furniture.md";
		std::remove(craftable_furniture_file_name.c_str());
		std::ofstream craftable_furniture_outfile(craftable_furniture_file_name, std::ios::out);
		if (craftable_furniture_outfile.is_open())
		{
			for (auto& pair : craftable_furniture_recipe_name_to_localized_name_map) {
				RValue localized_name = GetLocalizedString(Self, Other, pair.second);
				std::string localized_name_str = localized_name.AsString().data();
				pair.second = localized_name_str;

				// Internal Recipe Name, Localized Recipe Name
				craftable_furniture_outfile << "| " + pair.first + " | " + pair.second + " |\n";
			}
		}
		craftable_furniture_outfile.close();

		// Non-Craftable Furniture
		std::string non_craftable_furniture_file_name = data_mining_folder + "\\" + "non_craftable_furniture.md";
		std::remove(non_craftable_furniture_file_name.c_str());
		std::ofstream non_craftable_furniture_outfile(non_craftable_furniture_file_name, std::ios::out);
		if (non_craftable_furniture_outfile.is_open())
		{
			for (auto& pair : non_craftable_furniture_recipe_name_to_localized_name_map) {
				RValue localized_name = GetLocalizedString(Self, Other, pair.second);
				std::string localized_name_str = localized_name.AsString().data();
				pair.second = localized_name_str;

				// Internal Recipe Name, Localized Recipe Name
				non_craftable_furniture_outfile << "| " + pair.first + " | " + pair.second + " |\n";
			}
		}
		non_craftable_furniture_outfile.close();

		// Cosmetics
		std::string cosmetics_file_name = data_mining_folder + "\\" + "cosmetics.md";
		std::remove(cosmetics_file_name.c_str());
		std::ofstream cosmetics_outfile(cosmetics_file_name, std::ios::out);
		if (cosmetics_outfile.is_open())
		{
			for (auto& pair : cosmetic_name_to_localized_name_map) {
				RValue localized_name = GetLocalizedString(Self, Other, pair.second);
				std::string localized_name_str = localized_name.AsString().data();
				pair.second = localized_name_str;

				// Internal Recipe Name, Localized Recipe Name
				cosmetics_outfile << "| " + pair.first + " | " + pair.second + " |\n";
			}
		}
		cosmetics_outfile.close();
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_LOCALIZER));
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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_SETUP_MAIN_SCREEN));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (run_once)
	{
		// Try to find the mod_data directory.
		std::string current_dir = std::filesystem::current_path().string();
		std::string mod_data_folder = current_dir + "\\mod_data";
		if (!std::filesystem::exists(mod_data_folder))
		{
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"mod_data\" directory was not found. Creating directory: %s", MOD_NAME, VERSION, mod_data_folder.c_str());
			std::filesystem::create_directory(mod_data_folder);
		}

		// Try to find the mod_data/DataMining directory.
		data_mining_folder = mod_data_folder + "\\DataMining";
		if (!std::filesystem::exists(data_mining_folder))
		{
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"DataMining\" directory was not found. Creating directory: %s", MOD_NAME, VERSION, data_mining_folder.c_str());
			std::filesystem::create_directory(data_mining_folder);
		}

		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		RValue __item_data = global_instance->at("__item_data");
		size_t array_length;
		g_ModuleInterface->GetArraySize(__item_data, array_length);

		// Load all items.
		for (size_t i = 0; i < array_length; i++)
		{
			RValue* array_element;
			g_ModuleInterface->GetArrayEntry(__item_data, i, array_element);

			RValue name_key = array_element->at("name_key"); // The item's localization key
			if (name_key.m_Kind != VALUE_NULL && name_key.m_Kind != VALUE_UNDEFINED && name_key.m_Kind != VALUE_UNSET)
			{
				RValue item_id = array_element->at("item_id");
				RValue recipe_key = array_element->at("recipe_key"); // The internal item name
				item_name_to_id_map[recipe_key.AsString().data()] = RValueAsInt(item_id);
				item_name_to_localized_name_map[recipe_key.AsString().data()] = name_key.AsString().data();

				// Cooking recipes
				if (strstr(name_key.AsString().data(), "cooked_dishes"))
				{
					cooking_recipe_name_to_localized_name_map[recipe_key.AsString().data()] = name_key.AsString().data();
				}

				// Furniture recipes.
				if (strstr(name_key.AsString().data(), "furniture"))
				{
					furniture_recipe_name_to_localized_name_map[recipe_key.AsString().data()] = name_key.AsString().data();

					if (StructVariableExists(*array_element, "recipe"))
					{
						RValue recipe = array_element->at("recipe");
						if (recipe.m_Kind != VALUE_NULL && recipe.m_Kind != VALUE_UNDEFINED && recipe.m_Kind != VALUE_UNSET)
						{
							if (StructVariableExists(recipe, "item_id"))
							{
								craftable_furniture_recipe_name_to_localized_name_map[recipe_key.AsString().data()] = name_key.AsString().data();
							}
						}
						else
						{
							non_craftable_furniture_recipe_name_to_localized_name_map[recipe_key.AsString().data()] = name_key.AsString().data();
						}
					}
					else
					{
						non_craftable_furniture_recipe_name_to_localized_name_map[recipe_key.AsString().data()] = name_key.AsString().data();
					}
				}
			}
		}

		// Load all cosmetics.
		RValue __pad = global_instance->at("__pad");
		RValue player_assets = __pad.at("player_assets");
		RValue inner = player_assets.at("inner");
		g_ModuleInterface->EnumInstanceMembers(inner, GetAllCosmeticNames);
		for (std::string cosmetic_name : cosmetic_names)
		{
			RValue cosmetic = inner.at(cosmetic_name);
			RValue cosmetic_localization_key = cosmetic.at("name");
			cosmetic_name_to_localized_name_map[cosmetic_name] = cosmetic_localization_key.AsString().data();
		}

		run_once = false;
		localize_items = true;
	}

	return Result;
}

void CreateHookGmlScriptGetLocalizer(AurieStatus& status)
{
	CScript* gml_script_get_localizer = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_LOCALIZER,
		(PVOID*)&gml_script_get_localizer
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_LOCALIZER);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_LOCALIZER,
		gml_script_get_localizer->m_Functions->m_ScriptFunction,
		GmlScriptGetLocalizerCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_LOCALIZER);
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

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[Zoom Control] - Hello from PluginEntry!");

	CreateHookGmlScriptGetLocalizer(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[Zoom Control] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[Zoom Control] - Exiting due to failure on start!");
		return status;
	}

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[Zoom Control] - Failed to register callback!");
	}

	return AURIE_SUCCESS;
}
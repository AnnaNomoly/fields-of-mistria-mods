#include <map>
#include <fstream>
#include <nlohmann/json.hpp>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

static const char* const MOD_NAME = "BugRadar";
static const char* const VERSION = "1.0.0";
static const char* const BUG_LIST_KEY = "bug_list";
static const std::string BUG_NAME_PLACEHOLDER_TEXT = "<BUG>";
static const char* const BUG_DETECTED_NOTIFICATION_KEY = "Notifications/Mods/Bug Radar/bug_detected";
static const char* const GML_SCRIPT_CREATE_NOTIFICATION = "gml_Script_create_notification";
static const char* const GML_SCRIPT_GET_LOCALIZER = "gml_Script_get@Localizer@Localizer";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const std::vector<std::string> DEFAULT_BUG_LIST = {"Fairy Bee", "Flower Crown Beetle", "Snowball Beetle", "Speedy Snail", "Strobe Firefly"};

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_on_start = true;
static bool localize_items = false;
static bool processing_bug = false;
static std::string bug_name = "";
static std::vector<std::string> bug_list = DEFAULT_BUG_LIST;
static std::map<std::string, int> item_name_to_id_map = {};
static std::map<int, std::string> item_id_to_name_map = {};
static std::map<std::string, std::string> item_name_to_localized_name_map = {};
static std::map<std::string, std::string> lowercase_localized_name_to_item_name_map = {};

// DEBUG
std::vector<std::string> bugs_detected = {};

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

void PrintError(std::exception_ptr eptr)
{
	try {
		if (eptr) {
			std::rethrow_exception(eptr);
		}
	}
	catch (const std::exception& e) {
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Error: %s", MOD_NAME, VERSION, e.what());
	}
}

void LogDefaultConfigValues()
{
	bug_list = DEFAULT_BUG_LIST;
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, BUG_LIST_KEY, "[\"Fairy Bee\", \"Flower Crown Beetle\", \"Snowball Beetle\", \"Speedy Snail\", \"Strobe Firefly\"]");
}

void CreateOrLoadConfigFile()
{
	// Load config file.
	std::exception_ptr eptr;
	try
	{
		// Try to find the mod_data directory.
		std::string current_dir = std::filesystem::current_path().string();
		std::string mod_data_folder = current_dir + "\\mod_data";
		if (!std::filesystem::exists(mod_data_folder))
		{
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"mod_data\" directory was not found. Creating directory: %s", MOD_NAME, VERSION, mod_data_folder.c_str());
			std::filesystem::create_directory(mod_data_folder);
		}

		// Try to find the mod_data/BugRadar directory.
		std::string bug_radar_folder = mod_data_folder + "\\BugRadar";
		if (!std::filesystem::exists(bug_radar_folder))
		{
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"BugRadar\" directory was not found. Creating directory: %s", MOD_NAME, VERSION, bug_radar_folder.c_str());
			std::filesystem::create_directory(bug_radar_folder);
		}

		// Try to find the mod_data/BugRadar/BugRadar.json config file.
		std::string config_file = bug_radar_folder + "\\" + "BugRadar.json";
		std::ifstream in_stream(config_file);
		if (in_stream.good())
		{
			try
			{
				json json_object = json::parse(in_stream);

				// Check if the json_object is empty.
				if (json_object.empty())
				{
					g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - No values found in mod configuration file: %s!", MOD_NAME, VERSION, config_file.c_str());
					g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Add your desired values to the configuration file, otherwise defaults will be used.", MOD_NAME, VERSION);
					LogDefaultConfigValues();
				}
				else
				{
					// Try loading the activation_button value.
					if (json_object.contains(BUG_LIST_KEY))
					{
						bug_list = json_object[BUG_LIST_KEY];
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value!", MOD_NAME, VERSION, BUG_LIST_KEY);
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, BUG_LIST_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, BUG_LIST_KEY, "[\"Fairy Bee\", \"Flower Crown Beetle\", \"Snowball Beetle\", \"Speedy Snail\", \"Strobe Firefly\"]");
					}
				}
			}
			catch (...)
			{
				eptr = std::current_exception();
				PrintError(eptr);

				g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to parse JSON from configuration file: %s", MOD_NAME, VERSION, config_file.c_str());
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Make sure the file is valid JSON!", MOD_NAME, VERSION);
				LogDefaultConfigValues();
			}

			in_stream.close();
		}
		else
		{
			in_stream.close();

			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"BugRadar.json\" file was not found. Creating file: %s", MOD_NAME, VERSION, config_file.c_str());
			json default_json = {
				{BUG_LIST_KEY, DEFAULT_BUG_LIST}
			};

			std::ofstream out_stream(config_file);
			out_stream << std::setw(4) << default_json << std::endl;
			out_stream.close();

			LogDefaultConfigValues();
		}
	}
	catch (...)
	{
		eptr = std::current_exception();
		PrintError(eptr);

		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - An error occurred loading the mod configuration file.", MOD_NAME, VERSION);
		LogDefaultConfigValues();
	}
}

void CreateNotification(std::string notification_localization_str, CInstance* Self, CInstance* Other)
{
	CScript* gml_script_create_notification = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CREATE_NOTIFICATION,
		(PVOID*)&gml_script_create_notification
	);

	RValue result;
	RValue notification = notification_localization_str;
	RValue* notification_ptr = &notification;
	gml_script_create_notification->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &notification_ptr }
	);
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

void LoadAllItemData()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	size_t array_length;
	RValue item_data = global_instance->at("__item_data");
	g_ModuleInterface->GetArraySize(item_data, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* item;
		g_ModuleInterface->GetArrayEntry(item_data, i, item);

		RValue name_key = item->at("name_key"); // The item's localization key
		if (name_key.m_Kind != VALUE_NULL && name_key.m_Kind != VALUE_UNDEFINED && name_key.m_Kind != VALUE_UNSET)
		{
			RValue item_id = item->at("item_id");
			RValue recipe_key = item->at("recipe_key"); // The internal item name
			item_name_to_id_map[recipe_key.AsString().data()] = RValueAsInt(item_id);
			item_id_to_name_map[RValueAsInt(item_id)] = recipe_key.AsString().data();
			item_name_to_localized_name_map[recipe_key.AsString().data()] = name_key.AsString().data();
		}
	}
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

	if (!strstr(self->m_Object->m_Name, "obj_bug"))
		return;

	if (!processing_bug && !StructVariableExists(self, "__bug_radar__processed_bug"))
	{
		bug_name = "";
		processing_bug = true;

		if (StructVariableExists(self, "item_id"))
		{
			RValue item_id = StructVariableGet(self, "item_id");
			if (item_id.m_Kind != VALUE_UNSET && item_id.m_Kind != VALUE_UNDEFINED && item_id.m_Kind != VALUE_NULL)
			{
				std::string item_name = item_id_to_name_map[RValueAsInt(item_id)]; // internal item name
				bugs_detected.push_back(item_name); // DEBUG

				auto it = std::find(bug_list.begin(), bug_list.end(), item_name);
				if (it != bug_list.end())
				{
					bug_name = item_name_to_localized_name_map[item_name];
					CreateNotification(BUG_DETECTED_NOTIFICATION_KEY, self, self);
				}
				else
					processing_bug = false;

				StructVariableSet(self, "__bug_radar__processed_bug", true);
			}
			else
				processing_bug = false;
		}
		else
			processing_bug = false;
	}
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

		for (auto& pair : item_name_to_localized_name_map)
		{
			RValue localized_name = GetLocalizedString(Self, Other, pair.second);
			std::string localized_name_str = localized_name.AsString().data();
			pair.second = localized_name_str;

			std::string lowercase_localized_name_str = localized_name.AsString().data();
			std::transform(lowercase_localized_name_str.begin(), lowercase_localized_name_str.end(), lowercase_localized_name_str.begin(), [](unsigned char c) { return std::tolower(c); });
			lowercase_localized_name_to_item_name_map[lowercase_localized_name_str] = pair.first;
		}

		// Convert the bug_list strings to lowercase.
		for (size_t i = 0; i < bug_list.size(); i++)
			std::transform(bug_list[i].begin(), bug_list[i].end(), bug_list[i].begin(), [](unsigned char c) { return std::tolower(c); });

		// Change the bug_list names to internal names. Convert localized names to internal names. Filter out invalid names.
		std::vector<std::string> bug_list_internal_names = {};
		for (size_t i = 0; i < bug_list.size(); i++)
		{
			if (lowercase_localized_name_to_item_name_map.count(bug_list[i]) > 0)
				bug_list_internal_names.push_back(lowercase_localized_name_to_item_name_map[bug_list[i]]);
			else if (item_name_to_id_map.count(bug_list[i]) > 0)
				bug_list_internal_names.push_back(bug_list[i]);
		}
		bug_list = bug_list_internal_names;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_LOCALIZER));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (ArgumentCount == 1 && Arguments[0]->m_Kind == VALUE_STRING)
	{
		std::string localization_key = Arguments[0]->AsString().data();
		if (localization_key.compare(BUG_DETECTED_NOTIFICATION_KEY) == 0)
		{
			std::string result_str = Result.AsString().data();

			// Replace the <BUG> placeholder text.
			size_t bug_placeholder_index = result_str.find(BUG_NAME_PLACEHOLDER_TEXT);
			if (bug_placeholder_index != std::string::npos)
				result_str.replace(bug_placeholder_index, BUG_NAME_PLACEHOLDER_TEXT.length(), bug_name);

			Result = result_str;
			processing_bug = false;
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
	if (load_on_start)
	{
		CreateOrLoadConfigFile();
		LoadAllItemData();

		load_on_start = false;
		localize_items = true;
	}
	else
	{
		processing_bug = false;
		bug_name = "";
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

	CreateObjectCallback(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptGetLocalizer(status);
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
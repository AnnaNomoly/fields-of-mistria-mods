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
static const char* const GML_SCRIPT_GET_WEATHER = "gml_Script_get_weather@WeatherManager@Weather";
static const char* const GML_SCRIPT_TRY_LOCATION_ID_TO_STRING = "gml_Script_try_location_id_to_string";
static const char* const GML_SCRIPT_SPAWN_BUG = "gml_Script_spawn_bug";
static const char* const GML_SCRIPT_CREATE_BUG = "gml_Script_setup@gml_Object_obj_bug_Create_0";
static const char* const GML_SCRIPT_GET_LOCALIZER = "gml_Script_get@Localizer@Localizer";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const char* const GML_SCRIPT_INVENTORY_SLOT_POP = "gml_Script_drain@InventorySlot@Inventory";
static const std::vector<std::string> DEFAULT_BUG_LIST = {"Fairy Bee", "Flower Crown Beetle", "Snowball Beetle", "Speedy Snail", "Strobe Firefly"};

static const std::map<std::string, std::vector<std::vector<std::pair<int, int>>>> ROOM_BUG_SPAWN_BOUNDING_BOXES_MAP = {
	{ "western_ruins", {
		{   // BBox 1
			{1360 / 8, 648 / 8},
			{1472 / 8, 648 / 8},
			{1472 / 8, 832 / 8},
			{1360 / 8, 832 / 8}
		},
		{	// BBox 2
			{1240 / 8, 792 / 8},
			{1592 / 8, 792 / 8},
			{1592 / 8, 848 / 8},
			{1240 / 8, 848 / 8}
		},
		{	// BBox 3
			{1128 / 8, 912 / 8},
			{1312 / 8, 912 / 8},
			{1312 / 8, 1112 / 8},
			{1128 / 8, 1112 / 8}
		},
		{	// BBox 4
			{1368 / 8, 936 / 8},
			{1464 / 8, 936 / 8},
			{1464 / 8, 1112 / 8},
			{1368 / 8, 1112 / 8}
		},
		{	// BBox 5
			{1520 / 8, 912 / 8},
			{1688 / 8, 912 / 8},
			{1688 / 8, 1096 / 8},
			{1520 / 8, 1096 / 8}
		},
		{	// BBox 6
			{1288 / 8, 1192 / 8},
			{1512 / 8, 1192 / 8},
			{1512 / 8, 1248 / 8},
			{1288 / 8, 1248 / 8}
		},
		{	// BBox 7
			{1160 / 8, 1224 / 8},
			{1304 / 8, 1224 / 8},
			{1304 / 8, 1256 / 8},
			{1160 / 8, 1256 / 8}
		},
		{	// BBox 8
			{1160 / 8, 1336 / 8},
			{1224 / 8, 1336 / 8},
			{1224 / 8, 1384 / 8},
			{1160 / 8, 1384 / 8}
		},
		{	// BBox 9
			{1240 / 8, 1224 / 8},
			{1304 / 8, 1224 / 8},
			{1304 / 8, 1312 / 8},
			{1240 / 8, 1312 / 8}
		},
		{	// BBox 10
			{1232 / 8, 1352 / 8},
			{1304 / 8, 1352 / 8},
			{1304 / 8, 1384 / 8},
			{1232 / 8, 1384 / 8}
		},
		{	// BBox 11
			{1544 / 8, 1184 / 8},
			{1632 / 8, 1184 / 8},
			{1632 / 8, 1384 / 8},
			{1544 / 8, 1384 / 8}
		},
	}},
	//{ "western_ruins", {
	//	{ {0, 0}, {0, 0}, {0, 0}, {0, 0} },
	//	{ {0, 0}, {0, 0}, {0, 0}, {0, 0} },
	//	{ {0, 0}, {0, 0}, {0, 0}, {0, 0} },
	//	{ {0, 0}, {0, 0}, {0, 0}, {0, 0} }
	//}},

};

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_on_start = true;
static bool game_is_active = false;
static bool localize_items = false;
static bool processing_bug = false;
static std::string bug_name = "";
static std::string ari_current_location = "";
static std::vector<std::string> bug_list = DEFAULT_BUG_LIST;
static std::map<std::string, int> item_name_to_id_map = {};
static std::map<int, std::string> item_id_to_name_map = {};
static std::map<std::string, std::string> item_name_to_localized_name_map = {};
static std::map<std::string, std::string> lowercase_localized_name_to_item_name_map = {};
static std::map<std::string, std::vector<std::pair<int, int>>> ROOM_BUG_SPAWN_BOUNDING_BOX_CENTER_MAP = {};

// DEBUG --------------------------------------------------------------------
static std::vector<std::string> bugs_detected = {};
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
//---------------------------------------------------------------------------


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

void CalculateBoundingBoxCenters()
{
	for (const auto& room : ROOM_BUG_SPAWN_BOUNDING_BOXES_MAP) {
		for (const auto& bounding_box : room.second) {
			int total_x = 0;
			int total_y = 0;

			for (const auto& point : bounding_box) {
				total_x += point.first;
				total_y += point.second;
			}

			int center_x = total_x / bounding_box.size();
			int center_y = total_y / bounding_box.size();
			std::pair<int, int> center = { center_x, center_y };
			ROOM_BUG_SPAWN_BOUNDING_BOX_CENTER_MAP[room.first].push_back(center);
		}
	}
}

double CalculateDistance(int x1, int y1, int x2, int y2) {
	return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

std::pair<int, int> GenerateRandomPointInClosestBoundingBox(int X, int Y, const std::string& room_name) {
	double min_distance = DBL_MAX;
	size_t closest_index = 0;

	for (size_t i = 0; i < ROOM_BUG_SPAWN_BOUNDING_BOX_CENTER_MAP[room_name].size(); ++i) {
		const auto& center = ROOM_BUG_SPAWN_BOUNDING_BOX_CENTER_MAP[room_name][i];
		double distance = CalculateDistance(X, Y, center.first, center.second);

		if (distance < min_distance) {
			min_distance = distance;
			closest_index = i;
		}
	}

	const auto& bounding_box = ROOM_BUG_SPAWN_BOUNDING_BOXES_MAP.at(room_name)[closest_index];
	int min_x = bounding_box[0].first;
	int max_x = bounding_box[0].first;
	int min_y = bounding_box[0].second;
	int max_y = bounding_box[0].second;

	for (const auto& point : bounding_box) {
		min_x = min(min_x, point.first);
		max_x = max(max_x, point.first);
		min_y = min(min_y, point.second);
		max_y = max(max_y, point.second);
	}

	srand(static_cast<unsigned int>(time(0)));
	int random_x = min_x + rand() % (max_x - min_x + 1);
	int random_y = min_y + rand() % (max_y - min_y + 1);

	return { random_x, random_y };
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

	if (/*!processing_bug && */!StructVariableExists(self, "__bug_radar__processed_bug"))
	{
		bug_name = "";
		//processing_bug = true;

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

RValue& GmlScriptGetWeatherCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	game_is_active = true;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_WEATHER));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptTryLocationIdToStringCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_TRY_LOCATION_ID_TO_STRING));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active)
		if (Result.m_Kind == VALUE_STRING)
			ari_current_location = Result.AsString().data();

	return Result;
}

RValue& GmlScriptSpawnBugCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	int x_coord = RValueAsInt(*Arguments[0]);
	int y_coord = RValueAsInt(*Arguments[1]);
	g_ModuleInterface->Print(CM_WHITE, "[%s %s] - A bug is being spawned at (%d, %d)...", MOD_NAME, VERSION, x_coord, y_coord);

	if (ROOM_BUG_SPAWN_BOUNDING_BOXES_MAP.contains(ari_current_location))
	{
		std::pair<int, int> point = GenerateRandomPointInClosestBoundingBox(x_coord, y_coord, ari_current_location);
		*Arguments[0] = point.first;
		*Arguments[1] = point.second;
		g_ModuleInterface->Print(CM_WHITE, "[%s %s] - Modified bug spawn to (%d, %d)...", MOD_NAME, VERSION, point.first, point.second);
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_SPAWN_BUG));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptCreateBugCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	int item_id = RValueAsInt(*Arguments[0]);
	std::string item_name = item_id_to_name_map[RValueAsInt(item_id)];
	auto it = std::find(bug_list.begin(), bug_list.end(), item_name);
	if (it != bug_list.end())
	{
		bug_name = item_name_to_localized_name_map[item_name];
		g_ModuleInterface->Print(CM_WHITE, "[%s %s] - A (%s) bug is being spawned...", MOD_NAME, VERSION, bug_name.c_str());
		CreateNotification(BUG_DETECTED_NOTIFICATION_KEY, Self, Other);
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_CREATE_BUG));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
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
		CalculateBoundingBoxCenters();

		load_on_start = false;
		localize_items = true;
	}
	else
	{
		game_is_active = false;
		processing_bug = false;
		ari_current_location = "";
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

// TODO: Remove this.
RValue& GmlScriptMenuPlaySoundCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	g_ModuleInterface->Print(CM_WHITE, "ENTER: %s. ArgCount: %d", GML_SCRIPT_INVENTORY_SLOT_POP, ArgumentCount);
	for (int i = 0; i < ArgumentCount; i++)
	{
		if (Arguments[i]->m_Kind == VALUE_INT32)
		{
			g_ModuleInterface->Print(CM_WHITE, "========== Argument[%d]: INT32 ==========", i);
			g_ModuleInterface->Print(CM_WHITE, "Value: %d", Arguments[i]->m_i32);
		}
		else if (Arguments[i]->m_Kind == VALUE_INT64)
		{
			g_ModuleInterface->Print(CM_WHITE, "========== Argument[%d]: INT64 ==========", i);
			g_ModuleInterface->Print(CM_WHITE, "Value: %d", Arguments[i]->m_i64);
		}
		else if (Arguments[i]->m_Kind == VALUE_REAL)
		{
			g_ModuleInterface->Print(CM_WHITE, "========== Argument[%d]: REAL ==========", i);
			g_ModuleInterface->Print(CM_WHITE, "Value: %f", Arguments[i]->m_Real);
		}
		else if (Arguments[i]->m_Kind == VALUE_BOOL)
		{
			g_ModuleInterface->Print(CM_WHITE, "========== Argument[%d]: BOOL ==========", i);
			g_ModuleInterface->Print(CM_WHITE, "Value: %s", Arguments[i]->m_Real == 1 ? "true" : "false");
		}
		else if (Arguments[i]->m_Kind == VALUE_ARRAY)
		{
			g_ModuleInterface->Print(CM_WHITE, "========== Argument[%d]: ARRAY ==========", i);
			size_t size;
			g_ModuleInterface->GetArraySize(*Arguments[i], size);
			g_ModuleInterface->Print(CM_WHITE, "Size: %d", static_cast<int>(size));
		}
		else if (Arguments[i]->m_Kind == VALUE_REF)
		{
			g_ModuleInterface->Print(CM_WHITE, "========== Argument[%d]: REFEFERENCE ==========", i);
			g_ModuleInterface->Print(CM_WHITE, "Value: ???");
		}
		else if (Arguments[i]->m_Kind == VALUE_PTR)
		{
			g_ModuleInterface->Print(CM_WHITE, "========== Argument[%d]: POUNTER ==========", i);
			g_ModuleInterface->Print(CM_WHITE, "Value: ???");
		}
		else if (Arguments[i]->m_Kind == VALUE_STRING)
		{
			g_ModuleInterface->Print(CM_WHITE, "========== Argument[%d]: STRING ==========", i);
			g_ModuleInterface->Print(CM_WHITE, "Value: %s", Arguments[i]->AsString().data());
		}
		else if (Arguments[i]->m_Kind == VALUE_OBJECT)
		{
			g_ModuleInterface->Print(CM_WHITE, "========== Argument[%d]: OBJECT ==========", i);

			struct_field_names = {};
			g_ModuleInterface->EnumInstanceMembers(Arguments[i]->m_Object, GetStructFieldNames);
			for (int j = 0; j < struct_field_names.size(); j++)
			{
				std::string field_name = struct_field_names[j];
				RValue field = Arguments[i]->m_Object->at(field_name);
				if (field.m_Kind == VALUE_OBJECT)
				{
					g_ModuleInterface->Print(CM_AQUA, "%s: OBJECT", field_name.c_str());
					g_ModuleInterface->EnumInstanceMembers(field, EnumFunction);
					g_ModuleInterface->Print(CM_WHITE, "------------------------------");
				}
				else if (field.m_Kind == VALUE_ARRAY)
				{
					RValue array_length = g_ModuleInterface->CallBuiltin("array_length", { field });
					g_ModuleInterface->Print(CM_AQUA, "%s: ARRAY (length == %d)", field_name.c_str(), static_cast<int>(array_length.m_Real));
					for (int k = 0; k < array_length.m_Real; k++)
					{
						RValue array_element = g_ModuleInterface->CallBuiltin("array_get", { field, k });
						int temp = 5;
					}
				}
				else if (field.m_Kind == VALUE_INT32)
					g_ModuleInterface->Print(CM_AQUA, "%s: INT32 == %d", field_name.c_str(), field.m_i32);
				else if (field.m_Kind == VALUE_INT64)
					g_ModuleInterface->Print(CM_AQUA, "%s: INT64 == %d", field_name.c_str(), field.m_i64);
				else if (field.m_Kind == VALUE_REAL)
					g_ModuleInterface->Print(CM_AQUA, "%s: REAL == %f", field_name.c_str(), field.m_Real);
				else if (field.m_Kind == VALUE_BOOL)
					g_ModuleInterface->Print(CM_AQUA, "%s: BOOL == %s", field_name.c_str(), field.m_Real == 0 ? "false" : "true");
				else if (field.m_Kind == VALUE_STRING)
					g_ModuleInterface->Print(CM_AQUA, "%s: STRING == %s", field_name.c_str(), field.AsString().data());
				else
					g_ModuleInterface->Print(CM_AQUA, "%s: OTHER", field_name.c_str());
			}
		}
		else if (Arguments[i]->m_Kind == VALUE_UNDEFINED)
		{
			g_ModuleInterface->Print(CM_WHITE, "========== Argument[%d]: UNDEFINED ==========", i);
			g_ModuleInterface->Print(CM_WHITE, "Value: UNDEFINED");
		}
		else if (Arguments[i]->m_Kind == VALUE_UNSET)
		{
			g_ModuleInterface->Print(CM_WHITE, "========== Argument[%d]: UNSET ==========", i);
			g_ModuleInterface->Print(CM_WHITE, "Value: UNSET");
		}
		else if (Arguments[i]->m_Kind == VALUE_NULL)
		{
			g_ModuleInterface->Print(CM_WHITE, "========== Argument[%d]: NULL ==========", i);
			g_ModuleInterface->Print(CM_WHITE, "Value: NULL");
		}
		else
		{
			g_ModuleInterface->Print(CM_WHITE, "========== Argument[%d]: OTHER ==========", i);
			g_ModuleInterface->Print(CM_WHITE, "Value: ???");
		}
	}

	RValue trash_item_sound = g_ModuleInterface->CallBuiltin(
		"asset_get_index",
		{ "snd_UITrashItem" }
	);

	RValue sound_is_playing = g_ModuleInterface->CallBuiltin(
		"audio_is_playing",
		{ trash_item_sound }
	);

	if (RValueAsBool(sound_is_playing))
	{
		g_ModuleInterface->Print(CM_WHITE, "!");
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_INVENTORY_SLOT_POP));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (Result.m_Kind == VALUE_INT32)
	{
		g_ModuleInterface->Print(CM_WHITE, "========== Result: INT32 ==========");
		g_ModuleInterface->Print(CM_WHITE, "Value: %d", Result.m_i32);
	}
	else if (Result.m_Kind == VALUE_INT64)
	{
		g_ModuleInterface->Print(CM_WHITE, "========== Result: INT64 ==========");
		g_ModuleInterface->Print(CM_WHITE, "Value: %d", Result.m_i64);
	}
	else if (Result.m_Kind == VALUE_REAL)
	{
		g_ModuleInterface->Print(CM_WHITE, "========== Result: REAL ==========");
		g_ModuleInterface->Print(CM_WHITE, "Value: %f", Result.m_Real);
	}
	else if (Result.m_Kind == VALUE_BOOL)
	{
		g_ModuleInterface->Print(CM_WHITE, "========== Result: BOOL ==========");
		g_ModuleInterface->Print(CM_WHITE, "Value: %s", Result.m_Real == 1 ? "true" : "false");
	}
	else if (Result.m_Kind == VALUE_ARRAY)
	{
		g_ModuleInterface->Print(CM_WHITE, "========== Result: ARRAY ==========");
		size_t size;
		g_ModuleInterface->GetArraySize(Result, size);
		g_ModuleInterface->Print(CM_WHITE, "Size: %d", static_cast<int>(size));
	}
	else if (Result.m_Kind == VALUE_REF)
	{
		g_ModuleInterface->Print(CM_WHITE, "========== Result: REFEFERENCE ==========");
		g_ModuleInterface->Print(CM_WHITE, "Value: ???");
	}
	else if (Result.m_Kind == VALUE_PTR)
	{
		g_ModuleInterface->Print(CM_WHITE, "========== Result: POUNTER ==========");
		g_ModuleInterface->Print(CM_WHITE, "Value: ???");
	}
	else if (Result.m_Kind == VALUE_STRING)
	{
		g_ModuleInterface->Print(CM_WHITE, "========== Result: STRING ==========");
		g_ModuleInterface->Print(CM_WHITE, "Value: %s", Result.AsString().data());
	}
	else if (Result.m_Kind == VALUE_OBJECT)
	{
		g_ModuleInterface->Print(CM_WHITE, "========== Result: OBJECT ==========");

		struct_field_names = {};
		g_ModuleInterface->EnumInstanceMembers(Result.m_Object, GetStructFieldNames);
		for (int j = 0; j < struct_field_names.size(); j++)
		{
			std::string field_name = struct_field_names[j];
			RValue field = Result.m_Object->at(field_name);
			if (field.m_Kind == VALUE_OBJECT)
			{
				g_ModuleInterface->Print(CM_AQUA, "%s: OBJECT", field_name.c_str());
				g_ModuleInterface->EnumInstanceMembers(field, EnumFunction);
				g_ModuleInterface->Print(CM_WHITE, "------------------------------");
			}
			else if (field.m_Kind == VALUE_ARRAY)
			{
				RValue array_length = g_ModuleInterface->CallBuiltin("array_length", { field });
				g_ModuleInterface->Print(CM_AQUA, "%s: ARRAY (length == %d)", field_name.c_str(), static_cast<int>(array_length.m_Real));
				for (int k = 0; k < array_length.m_Real; k++)
				{
					RValue array_element = g_ModuleInterface->CallBuiltin("array_get", { field, k });
					int temp = 5;
				}
			}
			else if (field.m_Kind == VALUE_INT32)
				g_ModuleInterface->Print(CM_AQUA, "%s: INT32 == %d", field_name.c_str(), field.m_i32);
			else if (field.m_Kind == VALUE_INT64)
				g_ModuleInterface->Print(CM_AQUA, "%s: INT64 == %d", field_name.c_str(), field.m_i64);
			else if (field.m_Kind == VALUE_REAL)
				g_ModuleInterface->Print(CM_AQUA, "%s: REAL == %f", field_name.c_str(), field.m_Real);
			else if (field.m_Kind == VALUE_BOOL)
				g_ModuleInterface->Print(CM_AQUA, "%s: BOOL == %s", field_name.c_str(), field.m_Real == 0 ? "false" : "true");
			else if (field.m_Kind == VALUE_STRING)
				g_ModuleInterface->Print(CM_AQUA, "%s: STRING == %s", field_name.c_str(), field.AsString().data());
			else
				g_ModuleInterface->Print(CM_AQUA, "%s: OTHER", field_name.c_str());
		}
	}
	else if (Result.m_Kind == VALUE_UNDEFINED)
	{
		g_ModuleInterface->Print(CM_WHITE, "========== Result: UNDEFINED ==========");
		g_ModuleInterface->Print(CM_WHITE, "Value: UNDEFINED");
	}
	else if (Result.m_Kind == VALUE_UNSET)
	{
		g_ModuleInterface->Print(CM_WHITE, "========== Result: UNSET ==========");
		g_ModuleInterface->Print(CM_WHITE, "Value: UNSET");
	}
	else if (Result.m_Kind == VALUE_NULL)
	{
		g_ModuleInterface->Print(CM_WHITE, "========== Result: NULL ==========");
		g_ModuleInterface->Print(CM_WHITE, "Value: NULL");
	}
	else
	{
		g_ModuleInterface->Print(CM_WHITE, "========== Result: OTHER ==========");
		g_ModuleInterface->Print(CM_WHITE, "Value: ???");
	}

	trash_item_sound = g_ModuleInterface->CallBuiltin(
		"asset_get_index", 
		{ "snd_UITrashItem" }
	);

	sound_is_playing = g_ModuleInterface->CallBuiltin(
		"audio_is_playing",
		{ trash_item_sound }
	);

	if (RValueAsBool(sound_is_playing))
	{
		g_ModuleInterface->Print(CM_WHITE, "!");
	}

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

void CreateHookGmlScriptGetWeather(AurieStatus& status)
{
	CScript* gml_script_get_weather = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_WEATHER,
		(PVOID*)&gml_script_get_weather
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_WEATHER);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_WEATHER,
		gml_script_get_weather->m_Functions->m_ScriptFunction,
		GmlScriptGetWeatherCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_WEATHER);
	}
}

void CreateHookGmlScriptTryLocationIdToString(AurieStatus& status)
{
	CScript* gml_script_try_location_id_to_string = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_TRY_LOCATION_ID_TO_STRING,
		(PVOID*)&gml_script_try_location_id_to_string
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_TRY_LOCATION_ID_TO_STRING);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_TRY_LOCATION_ID_TO_STRING,
		gml_script_try_location_id_to_string->m_Functions->m_ScriptFunction,
		GmlScriptTryLocationIdToStringCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_TRY_LOCATION_ID_TO_STRING);
	}
}

void CreateHookGmlScriptSpawnBug(AurieStatus& status)
{
	CScript* gml_script_create_bug = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SPAWN_BUG,
		(PVOID*)&gml_script_create_bug
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_SPAWN_BUG);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_SPAWN_BUG,
		gml_script_create_bug->m_Functions->m_ScriptFunction,
		GmlScriptSpawnBugCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_SPAWN_BUG);
	}
}

void CreateHookGmlScriptCreateBug(AurieStatus& status)
{
	CScript* gml_script_create_bug = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CREATE_BUG,
		(PVOID*)&gml_script_create_bug
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CREATE_BUG);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_CREATE_BUG,
		gml_script_create_bug->m_Functions->m_ScriptFunction,
		GmlScriptCreateBugCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CREATE_BUG);
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

void CreateHookGmlScriptMenuPlaySound(AurieStatus& status)
{
	CScript* gml_script_setup_main_screen = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_INVENTORY_SLOT_POP,
		(PVOID*)&gml_script_setup_main_screen
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_INVENTORY_SLOT_POP);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_INVENTORY_SLOT_POP,
		gml_script_setup_main_screen->m_Functions->m_ScriptFunction,
		GmlScriptMenuPlaySoundCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_INVENTORY_SLOT_POP);
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

	//CreateObjectCallback(status);
	//if (!AurieSuccess(status))
	//{
	//	g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
	//	return status;
	//}

	CreateHookGmlScriptGetWeather(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptTryLocationIdToString(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptSpawnBug(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptCreateBug(status);
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

	CreateHookGmlScriptMenuPlaySound(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Plugin started!", MOD_NAME, VERSION);
	return AURIE_SUCCESS;
}
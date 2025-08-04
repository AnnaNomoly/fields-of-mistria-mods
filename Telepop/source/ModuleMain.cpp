#include <map>
#include <fstream>
#include <nlohmann/json.hpp>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

static const char* const MOD_NAME = "Telepop";
static const char* const VERSION = "2.0.0";
static const char* const GML_SCRIPT_CREATE_NOTIFICATION = "gml_Script_create_notification";
static const char* const GML_SCRIPT_TELEPORT_ARI_TO_ROOM = "gml_Script_ari_teleport_to_room";
static const char* const GML_SCRIPT_SAVE_GAME = "gml_Script_save_game";
static const char* const GML_SCRIPT_LOAD_GAME = "gml_Script_load_game";
static const char* const GML_SCRIPT_GET_WEATHER = "gml_Script_get_weather@WeatherManager@Weather";
static const char* const GML_SCRIPT_TRY_LOCATION_ID_TO_STRING = "gml_Script_try_location_id_to_string";
static const char* const GML_SCRIPT_GET_LOCALIZER = "gml_Script_get@Localizer@Localizer";
static const char* const GML_SCRIPT_USE_ITEM = "gml_Script_use_item";
static const char* const GML_SCRIPT_HELD_ITEM = "gml_Script_held_item@Ari@Ari";
static const char* const TELEPOP_MYSTERY_LOCATION_KEY = "telepop_mystery_location";
static const char* const TELEPOP_MYSTERY_X_COORDINATE_KEY = "telepop_mystery_x_coordinate";
static const char* const TELEPOP_MYSTERY_Y_COORDINATE_KEY = "telepop_mystery_y_coordinate";
static const char* const TELEPOP_RECIPE_SUGAR_INGREDIENT_AMOUNT_KEY = "telepop_recipe_sugar_ingredient_amount";
static const char* const TELEPOP_RECIPE_COOKING_TIME_IN_MINUTES_KEY = "telepop_recipe_cooking_time_in_minutes";
static const char* const TELEPOP_SHIPPING_BIN_PRICE_KEY = "telepop_shipping_bin_price";
static const char* const TELEPOP_STORE_PRICE_KEY = "telepop_store_price";
static const std::string ECHO_MINT_LOCATION_REMINDER_NOTIFICATION_KEY = "Notifications/Mods/Telepop/echo_mint/location_reminder";
static const std::string ECHO_MINT_LOCATION_SAVED_NOTIFICATION_KEY = "Notifications/Mods/Telepop/echo_mint/location_saved";
static const std::string ECHO_MINT_LOCATION_INVALID_NOTIFICATION_KEY = "Notifications/Mods/Telepop/echo_mint/location_invalid";
static const std::string TELEPOP_MYSTERY_NO_LOCATION_SET_NOTIFICATION_KEY = "Notifications/Mods/Telepop/telepop_mystery/no_location_set";
static const std::string PLACEHOLDER_TEXT = "<PLACEHOLDER>";
static const std::string DUNGEON = "dungeon";
static const std::string WATER_SEAL = "water_seal";
static const std::string EARTH_SEAL = "earth_seal";
static const std::string FIRE_SEAL = "fire_seal";
static const std::string RUINS_SEAL = "ruins_seal";
static const std::string MINES_ENTRY_LOCATION_NAME = "mines_entry";
static const std::string BEACH_LOCATION_NAME = "beach";
static const std::string TOWN_LOCATION_NAME = "town";
static const std::string FARM_LOCATION_NAME = "farm";
static const std::string EASTERN_ROAD_LOCATION_NAME = "eastern_road";
static const std::string WESTERN_RUINS_LOCATION_NAME = "western_ruins";
static const std::string TELEPOP_PURPLE_ITEM_NAME = "telepop_purple"; // Mines Entrance
static const std::string TELEPOP_BLUE_ITEM_NAME = "telepop_blue"; // Beach
static const std::string TELEPOP_ORANGE_ITEM_NAME = "telepop_orange"; // Town
static const std::string TELEPOP_PINK_ITEM_NAME = "telepop_pink"; // Farm
static const std::string TELEPOP_GREEN_ITEM_NAME = "telepop_green"; // Eastern Road
static const std::string TELEPOP_YELLOW_ITEM_NAME = "telepop_yellow"; // Western Ruins
static const std::string TELEPOP_MYSTERY_ITEM_NAME = "telepop_mystery"; // Player Chosen
static const std::string ECHO_MINT_ITEM_NAME = "echo_mint"; // Sets player chosen point
static const std::vector<std::string> CUSTOM_ITEM_NAMES = {
	TELEPOP_PURPLE_ITEM_NAME,
	TELEPOP_BLUE_ITEM_NAME,
	TELEPOP_ORANGE_ITEM_NAME,
	TELEPOP_PINK_ITEM_NAME,
	TELEPOP_GREEN_ITEM_NAME,
	TELEPOP_YELLOW_ITEM_NAME,
	TELEPOP_MYSTERY_ITEM_NAME,
	ECHO_MINT_ITEM_NAME,
};
static const int UNSET_INT = -1;
static const int DEFAULT_TELEPOP_RECIPE_SUGAR_INGREDIENT_AMOUNT = 1;
static const int DEFAULT_TELEPOP_RECIPE_COOKING_TIME_IN_MINUTES = 10;
static const int DEFAULT_TELEPOP_SHIPPING_BIN_PRICE = 115;
static const int DEFAULT_TELEPOP_STORE_PRICE = 115;

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_on_start = true;
static bool mod_healthy = true;
static bool game_is_active = false;
static bool notify_on_file_load = true;
static bool teleport_ari = false;
static bool telepop_mystery = false;
static bool room_loaded = false;
static std::map<int, std::string> location_id_to_name_map = {};
static std::map<std::string, int> location_name_to_id_map = {};
static std::map<std::string, int> item_name_to_id_map = {};
static std::string ari_current_location = "";
static std::string save_prefix = "";
static std::string mod_folder = "";
static std::string telepop_mystery_location = "";
static int ari_x = UNSET_INT;
static int ari_y = UNSET_INT;
static int held_item_id = UNSET_INT;
static int telepop_destination_id = UNSET_INT;
static int telepop_mystery_x_coordinate = UNSET_INT;
static int telepop_mystery_y_coordinate = UNSET_INT;
static int telepop_recipe_sugar_ingredient_amount = DEFAULT_TELEPOP_RECIPE_SUGAR_INGREDIENT_AMOUNT;
static int telepop_recipe_cooking_time_in_minutes = DEFAULT_TELEPOP_RECIPE_COOKING_TIME_IN_MINUTES;
static int telepop_shipping_bin_price = DEFAULT_TELEPOP_SHIPPING_BIN_PRICE;
static int telepop_store_price = DEFAULT_TELEPOP_STORE_PRICE;

void ResetStaticFields(bool returned_to_title_screen)
{
	if (returned_to_title_screen)
	{
		game_is_active = false;
		notify_on_file_load = true;
		teleport_ari = false;
		telepop_mystery = false;
		room_loaded = false;
		ari_current_location = "";
		save_prefix = "";
		telepop_mystery_location = "";
		ari_x = UNSET_INT;
		ari_y = UNSET_INT;
		held_item_id = UNSET_INT;
		telepop_destination_id = UNSET_INT;
		telepop_mystery_x_coordinate = UNSET_INT;
		telepop_mystery_y_coordinate = UNSET_INT;
	}
}

bool GameIsPaused()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);
	RValue paused = global_instance->at("__pause_status");
	return paused.m_i64 > 0;
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

RValue StructVariableGet(RValue the_struct, std::string variable_name)
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - Error: %s", VERSION, e.what());
	}
}

void ModifyItem(int item_id)
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	RValue __item_data = global_instance->at("__item_data");
	RValue item = g_ModuleInterface->CallBuiltin("array_get", { __item_data, item_id });

	// Modify the item's recipe.
	RValue recipe = StructVariableGet(item, "recipe");
	RValue components = StructVariableGet(recipe, "components");
	RValue buffer = StructVariableGet(components, "__buffer");

	RValue ingredient = g_ModuleInterface->CallBuiltin("array_get", { buffer, 0 });
	StructVariableSet(ingredient, "count", telepop_recipe_sugar_ingredient_amount);

	RValue cooking_time = g_ModuleInterface->CallBuiltin("array_get", { buffer, 1 });
	StructVariableSet(cooking_time, "duration", telepop_recipe_cooking_time_in_minutes);

	// Modify the item's value.
	RValue value = StructVariableGet(item, "value");
	StructVariableSet(value, "bin", telepop_shipping_bin_price);
	StructVariableSet(value, "store", telepop_store_price);
}

void ModifyCustomItems()
{
	for (std::string item_name : CUSTOM_ITEM_NAMES)
	{
		int item_id = item_name_to_id_map[item_name];
		ModifyItem(item_id);
	}
}

bool LoadItemIds(CInstance* global_instance)
{
	RValue item_data = global_instance->at("__item_data");
	size_t array_length;
	g_ModuleInterface->GetArraySize(item_data, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(item_data, i, array_element);

		RValue name_key = array_element->at("name_key"); // The item's localization key
		if (name_key.m_Kind != VALUE_NULL && name_key.m_Kind != VALUE_UNDEFINED && name_key.m_Kind != VALUE_UNSET)
		{
			RValue item_id = array_element->at("item_id");
			RValue recipe_key = array_element->at("recipe_key"); // The internal item name
			item_name_to_id_map[recipe_key.AsString().data()] = RValueAsInt(item_id);
		}
	}

	bool success = true;

	for (std::string item_name : CUSTOM_ITEM_NAMES)
	{
		if (item_name_to_id_map.count(item_name) == 0)
		{
			success = false;
			g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to load item data for: %s", MOD_NAME, VERSION, item_name);
		}
	}

	if (item_name_to_id_map.size() == 0)
	{
		success = false;
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to load ANY item data!", MOD_NAME, VERSION);
	}

	return success;
}

bool LoadLocationIds(CInstance* global_instance)
{
	static const std::vector<std::string> LOCATION_NAMES = {
		DUNGEON,
		WATER_SEAL,
		EARTH_SEAL,
		FIRE_SEAL,
		RUINS_SEAL,
		MINES_ENTRY_LOCATION_NAME,
		BEACH_LOCATION_NAME,
		TOWN_LOCATION_NAME,
		FARM_LOCATION_NAME,
		EASTERN_ROAD_LOCATION_NAME,
		WESTERN_RUINS_LOCATION_NAME
	};

	size_t array_length;
	RValue location_ids = global_instance->at("__location_id__");
	g_ModuleInterface->GetArraySize(location_ids, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(location_ids, i, array_element);

		location_id_to_name_map[i] = array_element->AsString().data();
		location_name_to_id_map[array_element->AsString().data()] = i;
	}

	bool success = true;

	for (std::string location_name : LOCATION_NAMES)
	{
		if (location_name_to_id_map.count(location_name) == 0)
		{
			success = false;
			g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to load location data for: %s", MOD_NAME, VERSION, location_name);
		}
	}

	if (location_name_to_id_map.size() == 0)
	{
		success = false;
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to load ANY location data!", MOD_NAME, VERSION);
	}
	
	return success;
}

json CreateModSaveJson()
{
	json save_json = {
		{ TELEPOP_MYSTERY_LOCATION_KEY, telepop_mystery_location },
		{ TELEPOP_MYSTERY_X_COORDINATE_KEY, telepop_mystery_x_coordinate },
		{ TELEPOP_MYSTERY_Y_COORDINATE_KEY, telepop_mystery_y_coordinate },
	};
	return save_json;
}

void WriteModSaveFile()
{
	if (save_prefix.length() != 0 && mod_folder.length() != 0)
	{
		json mod_save_data = CreateModSaveJson();

		std::exception_ptr eptr;
		try
		{
			std::ofstream out_stream(mod_folder + "\\" + save_prefix + ".json");
			out_stream << std::setw(4) << mod_save_data << std::endl;
			out_stream.close();
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Successfully saved the mod file!", MOD_NAME, VERSION);
		}
		catch (...)
		{
			g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - An error occurred writing the mod file.", MOD_NAME, VERSION);

			eptr = std::current_exception();
			PrintError(eptr);
		}
	}
	else
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Unable to write mod save file!", MOD_NAME, VERSION);
	}
}

void ReadModSaveFile()
{
	if (save_prefix.length() != 0 && mod_folder.length() != 0)
	{
		std::exception_ptr eptr;
		try
		{
			std::ifstream in_stream(mod_folder + "\\" + save_prefix + ".json");
			if (in_stream.good())
			{
				json mod_save_data = json::parse(in_stream);
				telepop_mystery_location = mod_save_data[TELEPOP_MYSTERY_LOCATION_KEY];
				telepop_mystery_x_coordinate = mod_save_data[TELEPOP_MYSTERY_X_COORDINATE_KEY];
				telepop_mystery_y_coordinate = mod_save_data[TELEPOP_MYSTERY_Y_COORDINATE_KEY];
				g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Successfully loaded the mod file!", MOD_NAME, VERSION);
			}
		}
		catch (...)
		{
			g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - An error occurred reading the mod file.", MOD_NAME, VERSION);

			eptr = std::current_exception();
			PrintError(eptr);
		}
	}
	else
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Unable to read mod save file!", MOD_NAME, VERSION);
	}
}

json CreateModConfigJson(bool use_defaults)
{
	json config_json = {
		{ TELEPOP_RECIPE_SUGAR_INGREDIENT_AMOUNT_KEY, use_defaults ? DEFAULT_TELEPOP_RECIPE_SUGAR_INGREDIENT_AMOUNT : telepop_recipe_sugar_ingredient_amount },
		{ TELEPOP_RECIPE_COOKING_TIME_IN_MINUTES_KEY, use_defaults ? DEFAULT_TELEPOP_RECIPE_COOKING_TIME_IN_MINUTES : telepop_recipe_cooking_time_in_minutes },
		{ TELEPOP_SHIPPING_BIN_PRICE_KEY, use_defaults ? DEFAULT_TELEPOP_SHIPPING_BIN_PRICE : telepop_shipping_bin_price },
		{ TELEPOP_STORE_PRICE_KEY, use_defaults ? DEFAULT_TELEPOP_STORE_PRICE : telepop_store_price }
	};
	return config_json;
}

void LogDefaultModConfigValues()
{
	telepop_recipe_sugar_ingredient_amount = DEFAULT_TELEPOP_RECIPE_SUGAR_INGREDIENT_AMOUNT;
	telepop_recipe_cooking_time_in_minutes = DEFAULT_TELEPOP_RECIPE_COOKING_TIME_IN_MINUTES;
	telepop_shipping_bin_price = DEFAULT_TELEPOP_SHIPPING_BIN_PRICE;
	telepop_store_price = DEFAULT_TELEPOP_STORE_PRICE;

	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, TELEPOP_RECIPE_SUGAR_INGREDIENT_AMOUNT_KEY, DEFAULT_TELEPOP_RECIPE_SUGAR_INGREDIENT_AMOUNT);
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, TELEPOP_RECIPE_COOKING_TIME_IN_MINUTES_KEY, DEFAULT_TELEPOP_RECIPE_COOKING_TIME_IN_MINUTES);
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, TELEPOP_SHIPPING_BIN_PRICE_KEY, DEFAULT_TELEPOP_SHIPPING_BIN_PRICE);
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, TELEPOP_STORE_PRICE_KEY, DEFAULT_TELEPOP_STORE_PRICE);
}

bool CreateOrLoadModConfigFile()
{
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

			// Verify the directory now exists.
			if (!std::filesystem::exists(mod_data_folder))
			{
				g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to create the \"mod_data\" directory: ", MOD_NAME, VERSION, mod_data_folder.c_str());
				return false;
			}
		}

		// Try to find the mod_data/Telepop directory.
		std::string telepop_folder = mod_data_folder + "\\Telepop";
		if (!std::filesystem::exists(telepop_folder))
		{
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[Telepop %s] - The \"Telepop\" directory was not found. Creating directory: %s", VERSION, telepop_folder.c_str());
			std::filesystem::create_directory(telepop_folder);

			// Verify the directory now exists.
			if (!std::filesystem::exists(telepop_folder))
			{
				g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to create the \"Telepop\" directory: ", MOD_NAME, VERSION, telepop_folder.c_str());
				return false;
			}
		}

		mod_folder = telepop_folder;

		// Try to find the mod_data/Telepop/Telepop.json config file.
		bool update_config_file = false;
		std::string config_file = telepop_folder + "\\" + "Telepop.json";
		std::ifstream in_stream(config_file);
		if (in_stream.good())
		{
			try
			{
				json json_object = json::parse(in_stream);

				// Check if the json_object is empty.
				if (json_object.empty())
				{
					g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - No values found in mod configuration file: %s!", VERSION, config_file.c_str());
					g_ModuleInterface->Print(CM_LIGHTYELLOW, "[Telepop %s] - Add your desired values to the configuration file, otherwise defaults will be used.", VERSION);
					LogDefaultModConfigValues();
				}
				else
				{
					// Try loading the telepop_recipe_sugar_ingredient_amount value.
					if (json_object.contains(TELEPOP_RECIPE_SUGAR_INGREDIENT_AMOUNT_KEY))
					{
						telepop_recipe_sugar_ingredient_amount = json_object[TELEPOP_RECIPE_SUGAR_INGREDIENT_AMOUNT_KEY];
						if (telepop_recipe_sugar_ingredient_amount < 0 || telepop_recipe_sugar_ingredient_amount > 6)
						{
							g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - Invalid \"%s\" value (%d) in mod configuration file: %s", VERSION, TELEPOP_RECIPE_SUGAR_INGREDIENT_AMOUNT_KEY, telepop_recipe_sugar_ingredient_amount, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[Telepop %s] - Make sure the value is a valid integer between 0 and 6 (inclusive)!", VERSION);
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Using DEFAULT \"%s\" value: %d!", VERSION, TELEPOP_RECIPE_SUGAR_INGREDIENT_AMOUNT_KEY, DEFAULT_TELEPOP_RECIPE_SUGAR_INGREDIENT_AMOUNT);
							telepop_recipe_sugar_ingredient_amount = DEFAULT_TELEPOP_RECIPE_SUGAR_INGREDIENT_AMOUNT;
						}
						else
						{
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Using CUSTOM \"%s\" value: %d!", VERSION, TELEPOP_RECIPE_SUGAR_INGREDIENT_AMOUNT_KEY, telepop_recipe_sugar_ingredient_amount);
						}
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[Telepop %s] - Missing \"%s\" value in mod configuration file: %s!", VERSION, TELEPOP_RECIPE_SUGAR_INGREDIENT_AMOUNT_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Using DEFAULT \"%s\" value: %d!", VERSION, TELEPOP_RECIPE_SUGAR_INGREDIENT_AMOUNT_KEY, DEFAULT_TELEPOP_RECIPE_SUGAR_INGREDIENT_AMOUNT);
					}

					// Try loading the telepop_recipe_cooking_time_in_minutes value.
					if (json_object.contains(TELEPOP_RECIPE_COOKING_TIME_IN_MINUTES_KEY))
					{
						telepop_recipe_cooking_time_in_minutes = json_object[TELEPOP_RECIPE_COOKING_TIME_IN_MINUTES_KEY];
						if (telepop_recipe_cooking_time_in_minutes < 0 || telepop_recipe_cooking_time_in_minutes > 60 || telepop_recipe_cooking_time_in_minutes % 10 != 0)
						{
							g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - Invalid \"%s\" value (%d) in mod configuration file: %s", VERSION, TELEPOP_RECIPE_COOKING_TIME_IN_MINUTES_KEY, telepop_recipe_cooking_time_in_minutes, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[Telepop %s] - Make sure the value is a valid integer between 0 and 60 (inclusive)!", VERSION);
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[Telepop %s] - Make sure the value is a multiple of 10 (or 0)!", VERSION);
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Using DEFAULT \"%s\" value: %d!", VERSION, TELEPOP_RECIPE_COOKING_TIME_IN_MINUTES_KEY, DEFAULT_TELEPOP_RECIPE_COOKING_TIME_IN_MINUTES);
							telepop_recipe_cooking_time_in_minutes = DEFAULT_TELEPOP_RECIPE_COOKING_TIME_IN_MINUTES;
						}
						else
						{
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Using CUSTOM \"%s\" value: %d!", VERSION, TELEPOP_RECIPE_COOKING_TIME_IN_MINUTES_KEY, telepop_recipe_cooking_time_in_minutes);
						}
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[Telepop %s] - Missing \"%s\" value in mod configuration file: %s!", VERSION, TELEPOP_RECIPE_COOKING_TIME_IN_MINUTES_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Using DEFAULT \"%s\" value: %d!", VERSION, TELEPOP_RECIPE_COOKING_TIME_IN_MINUTES_KEY, DEFAULT_TELEPOP_RECIPE_COOKING_TIME_IN_MINUTES);
					}

					// Try loading the telepop_shipping_bin_price value.
					if (json_object.contains(TELEPOP_SHIPPING_BIN_PRICE_KEY))
					{
						telepop_shipping_bin_price = json_object[TELEPOP_SHIPPING_BIN_PRICE_KEY];
						if (telepop_shipping_bin_price < 1 || telepop_shipping_bin_price > 10000)
						{
							g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - Invalid \"%s\" value (%d) in mod configuration file: %s", VERSION, TELEPOP_SHIPPING_BIN_PRICE_KEY, telepop_shipping_bin_price, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[Telepop %s] - Make sure the value is a valid integer between 1 and 10,000 (inclusive)!", VERSION);
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Using DEFAULT \"%s\" value: %d!", VERSION, TELEPOP_SHIPPING_BIN_PRICE_KEY, DEFAULT_TELEPOP_SHIPPING_BIN_PRICE);
							telepop_shipping_bin_price = DEFAULT_TELEPOP_SHIPPING_BIN_PRICE;
						}
						else
						{
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Using CUSTOM \"%s\" value: %d!", VERSION, TELEPOP_SHIPPING_BIN_PRICE_KEY, telepop_shipping_bin_price);
						}
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[Telepop %s] - Missing \"%s\" value in mod configuration file: %s!", VERSION, TELEPOP_SHIPPING_BIN_PRICE_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Using DEFAULT \"%s\" value: %d!", VERSION, TELEPOP_SHIPPING_BIN_PRICE_KEY, DEFAULT_TELEPOP_SHIPPING_BIN_PRICE);
					}

					// Try loading the telepop_store_price value.
					if (json_object.contains(TELEPOP_STORE_PRICE_KEY))
					{
						telepop_store_price = json_object[TELEPOP_STORE_PRICE_KEY];
						if (telepop_store_price < 1 || telepop_store_price > 10000)
						{
							g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - Invalid \"%s\" value (%d) in mod configuration file: %s", VERSION, TELEPOP_STORE_PRICE_KEY, telepop_store_price, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[Telepop %s] - Make sure the value is a valid integer between 1 and 10,000 (inclusive)!", VERSION);
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Using DEFAULT \"%s\" value: %d!", VERSION, TELEPOP_STORE_PRICE_KEY, DEFAULT_TELEPOP_STORE_PRICE);
							telepop_store_price = DEFAULT_TELEPOP_STORE_PRICE;
						}
						else
						{
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Using CUSTOM \"%s\" value: %d!", VERSION, TELEPOP_STORE_PRICE_KEY, telepop_store_price);
						}
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[Telepop %s] - Missing \"%s\" value in mod configuration file: %s!", VERSION, TELEPOP_STORE_PRICE_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Using DEFAULT \"%s\" value: %d!", VERSION, TELEPOP_STORE_PRICE_KEY, DEFAULT_TELEPOP_STORE_PRICE);
					}
				}

				update_config_file = true;
			}
			catch (...)
			{
				eptr = std::current_exception();
				PrintError(eptr);

				g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - Failed to parse JSON from configuration file: %s", VERSION, config_file.c_str());
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[Telepop %s] - Make sure the file is valid JSON!", VERSION);
				LogDefaultModConfigValues();
			}

			in_stream.close();
		}
		else
		{
			in_stream.close();

			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[Telepop %s] - The \"Telepop.json\" file was not found. Creating file: %s", VERSION, config_file.c_str());

			json default_config_json = CreateModConfigJson(true);
			std::ofstream out_stream(config_file);
			out_stream << std::setw(4) << default_config_json << std::endl;
			out_stream.close();

			// Verify the file now exists.
			if (!std::filesystem::exists(config_file))
			{
				g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to create the \"Telepop.json\" file: ", MOD_NAME, VERSION, config_file.c_str());
				return false;
			}

			LogDefaultModConfigValues();
		}

		if (update_config_file)
		{
			json config_json = CreateModConfigJson(false);
			std::ofstream out_stream(config_file);
			out_stream << std::setw(4) << config_json << std::endl;
			out_stream.close();
		}
	}
	catch (...)
	{
		eptr = std::current_exception();
		PrintError(eptr);

		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - An error occurred loading the mod configuration file.", MOD_NAME, VERSION);
		return false;
	}

	return true;
}

bool TelepopMysteryUsable()
{
	if (telepop_mystery_location.size() != 0 && telepop_mystery_x_coordinate != UNSET_INT && telepop_mystery_y_coordinate != UNSET_INT)
		return true;
	return false;
}

bool EchoMintUsable()
{
	return ari_current_location != DUNGEON && 
		ari_current_location != WATER_SEAL && 
		ari_current_location != EARTH_SEAL &&
		ari_current_location != FIRE_SEAL &&
		ari_current_location != RUINS_SEAL;
}

void CreateNotification(CInstance* Self, CInstance* Other, std::string notification_localization_str)
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

void TeleportAriToRoom(CInstance* Self, CInstance* Other, int location_id, int x_coordinate, int y_coordinate)
{
	CScript* gml_script_ari_teleport_to_room = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_TELEPORT_ARI_TO_ROOM,
		(PVOID*)&gml_script_ari_teleport_to_room
	);

	RValue retval;
	RValue location = location_id;
	RValue x = x_coordinate;
	RValue y = y_coordinate;
	RValue* location_ptr = &location;
	RValue* x_ptr = &x;
	RValue* y_ptr = &y;
	RValue* argument_array[3] = { location_ptr, x_ptr, y_ptr };
	gml_script_ari_teleport_to_room->m_Functions->m_ScriptFunction(
		Self,
		Other,
		retval,
		3,
		argument_array
	);

	if (telepop_mystery)
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Ari teleported to her Echo (%s)!", VERSION, telepop_mystery_location.c_str());
	else if (telepop_destination_id == location_name_to_id_map[MINES_ENTRY_LOCATION_NAME])
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Ari teleported to the Mines!", VERSION);
	else if (telepop_destination_id == location_name_to_id_map[BEACH_LOCATION_NAME])
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Ari teleported to the Beach!", VERSION);
	else if (telepop_destination_id == location_name_to_id_map[TOWN_LOCATION_NAME])
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Ari teleported to the Town!", VERSION);
	else if (telepop_destination_id == location_name_to_id_map[FARM_LOCATION_NAME])
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Ari teleported to the Farm!", VERSION);
	else if (telepop_destination_id == location_name_to_id_map[EASTERN_ROAD_LOCATION_NAME])
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Ari teleported to the Eastern Road!", VERSION);
	else if (telepop_destination_id == location_name_to_id_map[WESTERN_RUINS_LOCATION_NAME])
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Ari teleported to the Western Ruins!", VERSION);
}

void ObjectCallback(
	IN FWCodeEvent& CodeEvent
)
{
	static const std::map<std::string, std::pair<int, int>> LOCATION_TO_TELEPORT_POINT_MAP = {
		{ MINES_ENTRY_LOCATION_NAME, {216, 198} },
		{ BEACH_LOCATION_NAME, {1722, 505} },
		{ TOWN_LOCATION_NAME, {1097, 1323} },
		{ FARM_LOCATION_NAME, {809, 306} },
		{ EASTERN_ROAD_LOCATION_NAME, {1128, 936} },
		{ WESTERN_RUINS_LOCATION_NAME, {1416, 808} }
	};

	auto& [self, other, code, argc, argv] = CodeEvent.Arguments();

	if (!self)
		return;

	if (!self->m_Object)
		return;

	if (!strstr(self->m_Object->m_Name, "obj_ari"))
		return;

	if (mod_healthy)
	{
		if (game_is_active && notify_on_file_load)
		{
			if (TelepopMysteryUsable())
				CreateNotification(self, other, ECHO_MINT_LOCATION_REMINDER_NOTIFICATION_KEY);
			notify_on_file_load = false;
		}

		RValue x;
		g_ModuleInterface->GetBuiltin("x", self, NULL_INDEX, x);
		ari_x = x.m_Real;

		RValue y;
		g_ModuleInterface->GetBuiltin("y", self, NULL_INDEX, y);
		ari_y = y.m_Real;

		if (teleport_ari)
		{
			TeleportAriToRoom(
				self,
				other,
				telepop_mystery ? location_name_to_id_map[telepop_mystery_location] : telepop_destination_id,
				telepop_mystery ? telepop_mystery_x_coordinate : LOCATION_TO_TELEPORT_POINT_MAP.at(location_id_to_name_map[telepop_destination_id]).first,
				telepop_mystery ? telepop_mystery_y_coordinate : LOCATION_TO_TELEPORT_POINT_MAP.at(location_id_to_name_map[telepop_destination_id]).second
			);

			teleport_ari = false;
			telepop_mystery = false;
		}
	}
}

RValue& GmlScriptSaveGameCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	// No save prefix has been detected. This should only happen when a new game is started.
	if (mod_healthy && save_prefix.size() == 0)
	{
		// Get the save file name.
		std::string save_file = Arguments[0]->AsString().data();
		std::size_t save_file_name_delimiter_index = save_file.find_last_of("/");
		std::string save_name = save_file.substr(save_file_name_delimiter_index + 1);

		// Check it's a valid value.
		if (save_name.find("undefined") == std::string::npos)
		{
			// Get the save prefix.
			std::size_t first_hyphen_index = save_name.find_first_of("-") + 1;
			std::size_t second_hyphen_index = save_name.find_last_of("-");
			save_prefix = save_name.substr(first_hyphen_index, (second_hyphen_index - first_hyphen_index));
		}
	}

	if(mod_healthy && TelepopMysteryUsable())
		WriteModSaveFile();

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_SAVE_GAME));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptLoadGameCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_healthy)
	{
		// Get the save file name.
		std::string save_file = std::string(Arguments[0]->m_Object->at("save_path").AsString().data());
		std::size_t save_file_name_delimiter_index = save_file.find_last_of("/");
		std::string save_name = save_file.substr(save_file_name_delimiter_index + 1);

		// Get the save prefix.
		std::size_t first_hyphen_index = save_name.find_first_of("-") + 1;
		std::size_t second_hyphen_index = save_name.find_last_of("-");
		save_prefix = save_name.substr(first_hyphen_index, (second_hyphen_index - first_hyphen_index));

		// Read from the custom mod data file.
		ReadModSaveFile();
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_LOAD_GAME));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptGetWeatherCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_healthy)
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

	if (mod_healthy && game_is_active && Result.m_Kind == VALUE_STRING)
		ari_current_location = Result.AsString().data();

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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_LOCALIZER));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (mod_healthy && ArgumentCount == 1 && Arguments[0]->m_Kind == VALUE_STRING)
	{
		std::string localization_key = Arguments[0]->AsString().data();
		if (localization_key.compare(ECHO_MINT_LOCATION_REMINDER_NOTIFICATION_KEY) == 0)
		{
			std::string result_str = Result.AsString().data();

			// Replace the placeholder text.
			size_t placeholder_index = result_str.find(PLACEHOLDER_TEXT);
			if (placeholder_index != std::string::npos) {
				result_str.replace(placeholder_index, PLACEHOLDER_TEXT.length(), telepop_mystery_location);
			}

			Result = result_str;
		}
	}

	return Result;
}

RValue& GmlScriptUseItemCallback(
	IN CInstance* Self, // The AriFsm?
	IN CInstance* Other, // obj_ari
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_healthy)
	{
		if (held_item_id == item_name_to_id_map[ECHO_MINT_ITEM_NAME])
		{
			if (!EchoMintUsable())
			{
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - You are unable to use an Echo Mint at your location: %s!", MOD_NAME, VERSION, ari_current_location.c_str());
				CreateNotification(Self, Other, ECHO_MINT_LOCATION_INVALID_NOTIFICATION_KEY);
				return Result;
			}
		}
		else if (held_item_id == item_name_to_id_map[TELEPOP_MYSTERY_ITEM_NAME])
		{
			if (!TelepopMysteryUsable())
			{
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - You are unable to use a Telepop (Mystery)! You must use an Echo Mint to record a location first.", MOD_NAME, VERSION);
				CreateNotification(Self, Other, TELEPOP_MYSTERY_NO_LOCATION_SET_NOTIFICATION_KEY);
				return Result;
			}
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_USE_ITEM));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_HELD_ITEM));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (mod_healthy)
	{
		if (Result.m_Kind != VALUE_UNDEFINED)
		{
			if (held_item_id != RValueAsInt(Result.at("item_id")) && !teleport_ari)
			{
				held_item_id = RValueAsInt(Result.at("item_id"));
			}
		}
	}

	return Result;
}

RValue& GmlScriptModifyHealthCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_healthy && !GameIsPaused())
	{
		// Telepop (Purple)
		if (held_item_id == item_name_to_id_map[TELEPOP_PURPLE_ITEM_NAME])
		{
			teleport_ari = true;
			telepop_destination_id = location_name_to_id_map[MINES_ENTRY_LOCATION_NAME];
		}

		// Telepop (Blue)
		if (held_item_id == item_name_to_id_map[TELEPOP_BLUE_ITEM_NAME])
		{
			teleport_ari = true;
			telepop_destination_id = location_name_to_id_map[BEACH_LOCATION_NAME];
		}

		// Telepop (Orange)
		if (held_item_id == item_name_to_id_map[TELEPOP_ORANGE_ITEM_NAME])
		{
			teleport_ari = true;
			telepop_destination_id = location_name_to_id_map[TOWN_LOCATION_NAME];
		}

		// Telepop (Pink)
		if (held_item_id == item_name_to_id_map[TELEPOP_PINK_ITEM_NAME])
		{
			teleport_ari = true;
			telepop_destination_id = location_name_to_id_map[FARM_LOCATION_NAME];
		}

		// Telepop (Green)
		if (held_item_id == item_name_to_id_map[TELEPOP_GREEN_ITEM_NAME])
		{
			teleport_ari = true;
			telepop_destination_id = location_name_to_id_map[EASTERN_ROAD_LOCATION_NAME];
		}

		// Telepop (Yellow)
		if (held_item_id == item_name_to_id_map[TELEPOP_YELLOW_ITEM_NAME])
		{
			teleport_ari = true;
			telepop_destination_id = location_name_to_id_map[WESTERN_RUINS_LOCATION_NAME];
		}

		// Telepop (Mystery)
		if (held_item_id == item_name_to_id_map[TELEPOP_MYSTERY_ITEM_NAME])
		{
			teleport_ari = true;
			telepop_mystery = true;
		}

		// Echo Mint
		if (held_item_id == item_name_to_id_map[ECHO_MINT_ITEM_NAME])
		{
			telepop_mystery_location = ari_current_location;
			telepop_mystery_x_coordinate = ari_x;
			telepop_mystery_y_coordinate = ari_y;

			CreateNotification(Self, Other, ECHO_MINT_LOCATION_SAVED_NOTIFICATION_KEY);
		}

		held_item_id = UNSET_INT;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_modify_health@Ari@Ari"));
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
	ResetStaticFields(true);

	if (load_on_start)
	{
		load_on_start = false;

		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		mod_healthy &= LoadItemIds(global_instance);
		mod_healthy &= LoadLocationIds(global_instance);
		mod_healthy &= CreateOrLoadModConfigFile();
		ModifyCustomItems();

		if(!mod_healthy)
			g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - The mod loaded in an unhealthy state and will NOT function!", MOD_NAME, VERSION);
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

void CreateHookGmlScriptSaveGame(AurieStatus& status)
{
	CScript* gml_script_save_game = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SAVE_GAME,
		(PVOID*)&gml_script_save_game
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_SAVE_GAME);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_SAVE_GAME,
		gml_script_save_game->m_Functions->m_ScriptFunction,
		GmlScriptSaveGameCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_SAVE_GAME);
	}
}

void CreateHookGmlScriptLoadGame(AurieStatus& status)
{
	CScript* gml_script_load_game = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_LOAD_GAME,
		(PVOID*)&gml_script_load_game
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_LOAD_GAME);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_LOAD_GAME,
		gml_script_load_game->m_Functions->m_ScriptFunction,
		GmlScriptLoadGameCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_LOAD_GAME);
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

void CreateHookGmlScriptGetLocalizer(AurieStatus& status)
{
	CScript* gml_script_textbox_translate = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_LOCALIZER,
		(PVOID*)&gml_script_textbox_translate
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_LOCALIZER);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_LOCALIZER,
		gml_script_textbox_translate->m_Functions->m_ScriptFunction,
		GmlScriptGetLocalizerCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_LOCALIZER);
	}
}

void CreateHookGmlScriptUseItem(AurieStatus& status)
{
	CScript* gml_script_use_item = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_USE_ITEM,
		(PVOID*)&gml_script_use_item
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - Failed to get script (%s)!", VERSION, GML_SCRIPT_USE_ITEM);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_USE_ITEM,
		gml_script_use_item->m_Functions->m_ScriptFunction,
		GmlScriptUseItemCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - Failed to hook script (%s)!", VERSION, GML_SCRIPT_USE_ITEM);
	}
}

void CreateHookGmlScriptHeldItem(AurieStatus& status)
{
	CScript* gml_script_held_item = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_HELD_ITEM,
		(PVOID*)&gml_script_held_item
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - Failed to get script (%s)!", VERSION, GML_SCRIPT_HELD_ITEM);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_HELD_ITEM,
		gml_script_held_item->m_Functions->m_ScriptFunction,
		GmlScriptHeldItemCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - Failed to hook script (%s)!", VERSION, GML_SCRIPT_HELD_ITEM);
	}
}

void CreateHookGmlScriptModifyHealth(AurieStatus& status)
{
	CScript* gml_script_modify_stamina = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_modify_health@Ari@Ari",
		(PVOID*)&gml_script_modify_stamina
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - Failed to get script (gml_Script_modify_health@Ari@Ari)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_modify_health@Ari@Ari",
		gml_script_modify_stamina->m_Functions->m_ScriptFunction,
		GmlScriptModifyHealthCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - Failed to hook script (gml_Script_modify_health@Ari@Ari)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - Failed to get script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - Failed to hook script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
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

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[Telepop %s] - Plugin starting...", VERSION);
	
	g_ModuleInterface->CreateCallback(
		g_ArSelfModule,
		EVENT_OBJECT_CALL,
		ObjectCallback,
		0
	);

	CreateHookGmlScriptSaveGame(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptLoadGame(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

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

	CreateHookGmlScriptGetLocalizer(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptUseItem(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptHeldItem(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptModifyHealth(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[Telepop %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[Telepop %s] - Plugin started!", VERSION);
	return AURIE_SUCCESS;
}
#include <map>
#include <fstream>
#include <nlohmann/json.hpp>
#include <YYToolkit/YYTK_Shared.hpp>
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

static const char* const MOD_NAME = "SummoningCircle";
static const char* const VERSION = "1.0.3";
static const char* const GML_SCRIPT_TRY_OBJECT_ID_TO_STRING = "gml_Script_try_object_id_to_string";
static const char* const GML_SCRIPT_CREATE_NOTIFICATION = "gml_Script_create_notification";
static const char* const GML_SCRIPT_PLAY_CONVERSATION = "gml_Script_play_conversation";
static const char* const GML_SCRIPT_CLOSE_TEXTBOX = "gml_Script_begin_close@TextboxMenu@TextboxMenu";
static const char* const GML_SCRIPT_TELEPORT_ARI_TO_ROOM = "gml_Script_ari_teleport_to_room"; // Used to teleport Ari.
static const char* const GML_SCRIPT_INTERACT = "gml_Script_interact"; // Used to track when the furniture is used.
static const char* const GML_SCRIPT_GET_LOCALIZER = "gml_Script_get@Localizer@Localizer";
static const char* const GML_SCRIPT_PLAY_TEXT = "gml_Script_play_text@TextboxMenu@TextboxMenu"; // Used to track conversation/dialogue choices.
static const char* const GML_SCRIPT_WRITE_FURNITURE_TO_LOCATION = "gml_Script_write_furniture_to_location"; // Used to track when the furniture is placed.
static const char* const GML_SCRIPT_ERASE_OBJECT_RENDERER = "gml_Script_erase_object_renderer"; // Used to track when the furniture is removed.
static const char* const GML_SCRIPT_ON_ROOM_START = "gml_Script_on_room_start@WeatherManager@Weather";
static const char* const GML_SCRIPT_GET_WEATHER = "gml_Script_get_weather@WeatherManager@Weather";
static const char* const GML_SCRIPT_TRY_LOCATION_ID_TO_STRING = "gml_Script_try_location_id_to_string";
static const char* const GML_SCRIPT_LOAD_GAME = "gml_Script_load_game";
static const char* const GML_SCRIPT_SAVE_GAME = "gml_Script_save_game";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const char* const SUMMONING_CIRCLE_POSITIONS_KEY = "summoning_circle_positions";
static const char* const SUMMONING_CIRCLE_CONFIRMATION_REQUIRED_KEY = "summoning_circle_confirmation_required";
static const char* const SUMMONING_CIRCLE_SHIPPING_BIN_PRICE_KEY = "summoning_circle_shipping_bin_price";
static const char* const SUMMONING_CIRCLE_STORE_PRICE_KEY = "summoning_circle_store_price";
static const std::string SUMMONING_CIRCLE_TELEPORT_INTERACT_KEY = "misc_local/Mods/Summoning Circle/teleport_interact";
static const std::string SUMMONING_CIRCLE_TWO_REQUIRED_NOTIFICATION_KEY = "Notifications/Mods/Summoning Circle/two_required";
static const std::string SUMMONING_CIRCLE_TWO_ALREADY_PRESENT_NOTIFICATION_KEY = "Notifications/Mods/Summoning Circle/two_already_present";
static const std::string SUMMONING_CIRCLE_ACTIVATION_REQUIRED_CONVERSATION_KEY = "Conversations/Mods/Summoning Circle/activation_confirmation";
static const std::string SUMMONING_CIRCLE_ACTIVATION_ACCEPTED_CONVERSATION_KEY = "Conversations/Mods/Summoning Circle/activation_confirmation/1";
static const std::string SUMMONING_CIRCLE_ACTIVATION_REJECTED_CONVERSATION_KEY = "Conversations/Mods/Summoning Circle/activation_confirmation/2";
static const std::string FARM_LOCATION_NAME = "farm";
static const std::string SUMMONING_CIRCLE_ITEM_NAME = "summoning_circle";
static const int DEFAULT_SUMMONING_CIRCLE_SHIPPING_BIN_PRICE = 5000;
static const int DEFAULT_SUMMONING_CIRCLE_STORE_PRICE = 5000;
static const bool DEFAULT_SUMMONING_CIRCLE_CONFIRMATION_REQUIRED = true;

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_on_start = true;
static bool mod_is_healthy = true;
static bool game_is_active = false;
static bool once_per_save_load = true;
static bool teleport_ari = false;
static bool play_conversation = false;
static double ari_x = 0;
static double ari_y = 0;
static std::pair<int, int> teleport_ari_to = {};
static std::string ari_current_location = "";
static std::string save_prefix = "";
static std::string mod_folder = "";
static std::map<std::string, int> item_name_to_id_map = {};
static std::map<int, std::string> object_id_to_name_map = {};
static std::map<std::string, int> object_name_to_id_map = {};
static std::map<std::string, int> location_name_to_id_map = {};
static std::vector<std::pair<int, int>> summoning_circle_positions = {};
static RValue custom_interact_key;
static RValue* custom_interact_key_ptr = nullptr;
static int summoning_circle_shipping_bin_price = DEFAULT_SUMMONING_CIRCLE_SHIPPING_BIN_PRICE;
static int summoning_circle_store_price = DEFAULT_SUMMONING_CIRCLE_STORE_PRICE;
static bool summoning_circle_confirmation_required = DEFAULT_SUMMONING_CIRCLE_CONFIRMATION_REQUIRED;

void ResetStaticFields(bool return_to_title_screen)
{
	if (return_to_title_screen)
	{
		game_is_active = false;
		once_per_save_load = true;
		teleport_ari = false;
		play_conversation = false;
		ari_x = 0;
		ari_y = 0;
		teleport_ari_to = {};
		ari_current_location = "";
		save_prefix = "";
		summoning_circle_positions = {};
		custom_interact_key = "";
		custom_interact_key_ptr = nullptr;
	}
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

json CreateModSaveJson()
{
	json save_json = {
		{ SUMMONING_CIRCLE_POSITIONS_KEY, summoning_circle_positions }
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
				summoning_circle_positions = mod_save_data[SUMMONING_CIRCLE_POSITIONS_KEY];
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
		{ SUMMONING_CIRCLE_SHIPPING_BIN_PRICE_KEY, use_defaults ? DEFAULT_SUMMONING_CIRCLE_SHIPPING_BIN_PRICE : summoning_circle_shipping_bin_price },
		{ SUMMONING_CIRCLE_STORE_PRICE_KEY, use_defaults ? DEFAULT_SUMMONING_CIRCLE_STORE_PRICE : summoning_circle_store_price },
		{ SUMMONING_CIRCLE_CONFIRMATION_REQUIRED_KEY, use_defaults ? DEFAULT_SUMMONING_CIRCLE_CONFIRMATION_REQUIRED : summoning_circle_confirmation_required }
	};
	return config_json;
}

void LogDefaultModConfigValues()
{
	summoning_circle_shipping_bin_price = DEFAULT_SUMMONING_CIRCLE_SHIPPING_BIN_PRICE;
	summoning_circle_store_price = DEFAULT_SUMMONING_CIRCLE_STORE_PRICE;
	summoning_circle_confirmation_required = DEFAULT_SUMMONING_CIRCLE_CONFIRMATION_REQUIRED;

	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, SUMMONING_CIRCLE_SHIPPING_BIN_PRICE_KEY, DEFAULT_SUMMONING_CIRCLE_SHIPPING_BIN_PRICE);
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, SUMMONING_CIRCLE_STORE_PRICE_KEY, DEFAULT_SUMMONING_CIRCLE_STORE_PRICE);
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, SUMMONING_CIRCLE_CONFIRMATION_REQUIRED_KEY, DEFAULT_SUMMONING_CIRCLE_CONFIRMATION_REQUIRED);
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

		// Try to find the mod_data/SummoningCircle directory.
		std::string summoning_circle_folder = mod_data_folder + "\\SummoningCircle";
		if (!std::filesystem::exists(summoning_circle_folder))
		{
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"SummoningCircle\" directory was not found. Creating directory: %s", MOD_NAME, VERSION, summoning_circle_folder.c_str());
			std::filesystem::create_directory(summoning_circle_folder);

			// Verify the directory now exists.
			if (!std::filesystem::exists(summoning_circle_folder))
			{
				g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to create the \"SummoningCircle\" directory: ", MOD_NAME, VERSION, summoning_circle_folder.c_str());
				return false;
			}
		}

		mod_folder = summoning_circle_folder;

		// Try to find the mod_data/SummoningCircle/SummoningCircle.json config file.
		bool update_config_file = false;
		std::string config_file = summoning_circle_folder + "\\" + "SummoningCircle.json";
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
					LogDefaultModConfigValues();
				}
				else
				{
					// Try loading the summoning_circle_shipping_bin_price value.
					if (json_object.contains(SUMMONING_CIRCLE_SHIPPING_BIN_PRICE_KEY))
					{
						summoning_circle_shipping_bin_price = json_object[SUMMONING_CIRCLE_SHIPPING_BIN_PRICE_KEY];
						if (summoning_circle_shipping_bin_price < 1 || summoning_circle_shipping_bin_price > 10000)
						{
							g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Invalid \"%s\" value (%d) in mod configuration file: %s", MOD_NAME, VERSION, SUMMONING_CIRCLE_SHIPPING_BIN_PRICE_KEY, summoning_circle_shipping_bin_price, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Make sure the value is a valid integer between 1 and 10,000 (inclusive)!", MOD_NAME, VERSION);
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, SUMMONING_CIRCLE_SHIPPING_BIN_PRICE_KEY, DEFAULT_SUMMONING_CIRCLE_SHIPPING_BIN_PRICE);
							summoning_circle_shipping_bin_price = DEFAULT_SUMMONING_CIRCLE_SHIPPING_BIN_PRICE;
						}
						else
						{
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %d!", MOD_NAME, VERSION, SUMMONING_CIRCLE_SHIPPING_BIN_PRICE_KEY, summoning_circle_shipping_bin_price);
						}
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, SUMMONING_CIRCLE_SHIPPING_BIN_PRICE_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, SUMMONING_CIRCLE_SHIPPING_BIN_PRICE_KEY, DEFAULT_SUMMONING_CIRCLE_SHIPPING_BIN_PRICE);
					}

					// Try loading the summoning_circle_store_price value.
					if (json_object.contains(SUMMONING_CIRCLE_STORE_PRICE_KEY))
					{
						summoning_circle_store_price = json_object[SUMMONING_CIRCLE_STORE_PRICE_KEY];
						if (summoning_circle_store_price < 1 || summoning_circle_store_price > 10000)
						{
							g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Invalid \"%s\" value (%d) in mod configuration file: %s", MOD_NAME, VERSION, SUMMONING_CIRCLE_STORE_PRICE_KEY, summoning_circle_store_price, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Make sure the value is a valid integer between 1 and 10,000 (inclusive)!", MOD_NAME, VERSION);
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, SUMMONING_CIRCLE_STORE_PRICE_KEY, DEFAULT_SUMMONING_CIRCLE_STORE_PRICE);
							summoning_circle_store_price = DEFAULT_SUMMONING_CIRCLE_STORE_PRICE;
						}
						else
						{
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %d!", MOD_NAME, VERSION, SUMMONING_CIRCLE_STORE_PRICE_KEY, summoning_circle_store_price);
						}
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, SUMMONING_CIRCLE_STORE_PRICE_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %d!", MOD_NAME, VERSION, SUMMONING_CIRCLE_STORE_PRICE_KEY, DEFAULT_SUMMONING_CIRCLE_STORE_PRICE);
					}

					// Try loading the summoning_circle_confirmation_required value.
					if (json_object.contains(SUMMONING_CIRCLE_CONFIRMATION_REQUIRED_KEY))
					{
						summoning_circle_confirmation_required = json_object[SUMMONING_CIRCLE_CONFIRMATION_REQUIRED_KEY];
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %s!", MOD_NAME, VERSION, SUMMONING_CIRCLE_CONFIRMATION_REQUIRED_KEY, summoning_circle_confirmation_required ? "true" : "false");
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, SUMMONING_CIRCLE_CONFIRMATION_REQUIRED_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, SUMMONING_CIRCLE_CONFIRMATION_REQUIRED_KEY, DEFAULT_SUMMONING_CIRCLE_CONFIRMATION_REQUIRED ? "true" : "false");
					}
				}

				update_config_file = true;
			}
			catch (...)
			{
				eptr = std::current_exception();
				PrintError(eptr);

				g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to parse JSON from configuration file: %s", MOD_NAME, VERSION, config_file.c_str());
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Make sure the file is valid JSON!", MOD_NAME, VERSION);
				LogDefaultModConfigValues();
			}

			in_stream.close();
		}
		else
		{
			in_stream.close();

			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"SummoningCircle.json\" file was not found. Creating file: %s", MOD_NAME, VERSION, config_file.c_str());

			json default_config_json = CreateModConfigJson(true);
			std::ofstream out_stream(config_file);
			out_stream << std::setw(4) << default_config_json << std::endl;
			out_stream.close();

			// Verify the file now exists.
			if (!std::filesystem::exists(config_file))
			{
				g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to create the \"SummoningCircle.json\" file: ", MOD_NAME, VERSION, config_file.c_str());
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

bool LoadItemIds(CInstance* global_instance)
{
	RValue item_data = *global_instance->GetRefMember("__item_data");
	size_t array_length;
	g_ModuleInterface->GetArraySize(item_data, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(item_data, i, array_element);

		RValue name_key = *array_element->GetRefMember("name_key"); // The item's localization key
		if (name_key.m_Kind != VALUE_NULL && name_key.m_Kind != VALUE_UNDEFINED && name_key.m_Kind != VALUE_UNSET)
		{
			RValue item_id = *array_element->GetRefMember("item_id");
			RValue recipe_key = *array_element->GetRefMember("recipe_key"); // The internal item name
			item_name_to_id_map[recipe_key.ToString()] = item_id.ToInt64();
		}
	}

	if (!item_name_to_id_map.contains(SUMMONING_CIRCLE_ITEM_NAME))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to load item data for: %s", MOD_NAME, VERSION, SUMMONING_CIRCLE_ITEM_NAME.c_str());
		return false;
	}
	
	if (item_name_to_id_map.size() == 0)
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to load ANY item data!", MOD_NAME, VERSION);
		return false;
	}

	return true;
}

bool LoadObjectIds(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_try_object_id_to_string = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_TRY_OBJECT_ID_TO_STRING,
		(PVOID*)&gml_script_try_object_id_to_string
	);

	for (int i = 0; i < 5000; i++)
	{
		RValue result;
		RValue object_id = i;
		RValue* object_id_ptr = &object_id;
		gml_script_try_object_id_to_string->m_Functions->m_ScriptFunction(
			Self,
			Other,
			result,
			1,
			{ &object_id_ptr }
		);

		if (result.m_Kind == VALUE_STRING)
		{
			object_id_to_name_map[i] = result.ToString();
			object_name_to_id_map[result.ToString()] = i;
		}
			
	}

	if (!object_name_to_id_map.contains(SUMMONING_CIRCLE_ITEM_NAME) || !object_id_to_name_map.contains(object_name_to_id_map[SUMMONING_CIRCLE_ITEM_NAME]))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to load object data for: %s", MOD_NAME, VERSION, SUMMONING_CIRCLE_ITEM_NAME.c_str());
		return false;
	}

	if (object_id_to_name_map.size() == 0 || object_name_to_id_map.size() == 0)
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to load ANY object data!", MOD_NAME, VERSION);
		return false;
	}

	return true;
}

bool LoadLocationIds(CInstance* global_instance)
{
	size_t array_length;
	RValue location_ids = global_instance->GetMember("__location_id__");
	g_ModuleInterface->GetArraySize(location_ids, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(location_ids, i, array_element);

		location_name_to_id_map[array_element->ToString()] = i;
	}

	if (!location_name_to_id_map.contains(FARM_LOCATION_NAME))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to location data for: %s", MOD_NAME, VERSION, FARM_LOCATION_NAME.c_str());
		return false;
	}

	if (location_name_to_id_map.size() == 0)
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to load ANY location data!", MOD_NAME, VERSION);
		return false;
	}

	return true;
}

double CalculateDistance(int x1, int y1, int x2, int y2) {
	return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

void PruneMissingSummoningCircles()
{
	CRoom* current_room = nullptr;
	if (!AurieSuccess(g_ModuleInterface->GetCurrentRoomData(current_room)))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get data for the current room!", MOD_NAME, VERSION);
		return;
	}

	std::vector<std::pair<int, int>> existing_positions;
	for (CInstance* inst = current_room->GetMembers().m_ActiveInstances.m_First; inst != nullptr; inst = inst->GetMembers().m_Flink)
	{
		auto map = inst->ToRValue().ToRefMap();
		if (!map.contains("node")) continue;

		RValue* nodeValue = map["node"];
		if (!nodeValue || nodeValue->GetKindName() != "struct") continue;

		auto nodeRefMap = nodeValue->ToRefMap();
		if (!nodeRefMap.contains("prototype")) continue;

		RValue* protoVal = nodeRefMap["prototype"];
		if (!protoVal || protoVal->GetKindName() != "struct") continue;

		auto protoMap = protoVal->ToRefMap();
		if (!protoMap.contains("object_id")) continue;

		int object_id = protoMap["object_id"]->ToInt64();
		if (object_id == object_name_to_id_map[SUMMONING_CIRCLE_ITEM_NAME])
		{
			int x = nodeRefMap["top_left_x"]->ToInt64();
			int y = nodeRefMap["top_left_y"]->ToInt64();
			existing_positions.emplace_back(x, y);
		}
	}

	std::erase_if(summoning_circle_positions, [&](const std::pair<int, int>& p) {
		return std::find(existing_positions.begin(), existing_positions.end(), p) == existing_positions.end();
	});
}

void ModifyItem(int item_id)
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	RValue __item_data = *global_instance->GetRefMember("__item_data");
	RValue item = g_ModuleInterface->CallBuiltin("array_get", { __item_data, item_id });

	// Modify the item's value.
	RValue value = *item.GetRefMember("value");
	*value.GetRefMember("bin") = summoning_circle_shipping_bin_price;
	*value.GetRefMember("store") = summoning_circle_store_price;
	//RValue bin = *value.GetRefMember("bin");
	//RValue store = *value.GetRefMember("store");
	//
	//bin = summoning_circle_shipping_bin_price;
	//store = summoning_circle_store_price;
}

void CreateNotification(std::string notification_localization_str, CInstance* Self, CInstance* Other)
{
	CScript* gml_script_create_notification = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CREATE_NOTIFICATION,
		(PVOID*)&gml_script_create_notification
	);

	RValue result;
	RValue notification = RValue(notification_localization_str);
	RValue* notification_ptr = &notification;
	gml_script_create_notification->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &notification_ptr }
	);
}

void PlayConversation(std::string conversation_localization_str, CInstance* Self, CInstance* Other)
{
	CScript* gml_script_play_conversation = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_PLAY_CONVERSATION,
		(PVOID*)&gml_script_play_conversation
	);

	RValue zero = 2;
	RValue notification = RValue(conversation_localization_str);
	RValue undefined;

	RValue* zero_ptr = &zero;
	RValue* notification_ptr = &notification;
	RValue* undefined_ptr = &undefined;

	RValue result;
	RValue* arguments[4] = { zero_ptr, notification_ptr, undefined_ptr, undefined_ptr };

	gml_script_play_conversation->m_Functions->m_ScriptFunction(
		Self,
		Self,
		result,
		4,
		arguments
	);
}

void CloseTextbox(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_close_textbox = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CLOSE_TEXTBOX,
		(PVOID*)&gml_script_close_textbox
	);

	RValue result;
	gml_script_close_textbox->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		0,
		nullptr
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

	if (mod_is_healthy && game_is_active)
	{
		RValue x;
		g_ModuleInterface->GetBuiltin("x", self, NULL_INDEX, x);
		ari_x = x.m_Real;

		RValue y;
		g_ModuleInterface->GetBuiltin("y", self, NULL_INDEX, y);
		ari_y = y.m_Real;

		if (teleport_ari)
		{
			teleport_ari = false;
			int x = (teleport_ari_to.first * 8) + 12; // See notes. The 12 is from: (write_size_x * 8 / 2)
			int y = (teleport_ari_to.second * 8) + 12;

			teleport_ari_to = {};
			TeleportAriToRoom(self, other, location_name_to_id_map[FARM_LOCATION_NAME], x, y);
		}

		if (play_conversation)
		{
			play_conversation = false;
			PlayConversation(SUMMONING_CIRCLE_ACTIVATION_REQUIRED_CONVERSATION_KEY, self, other);
		}
	}
}

RValue& GmlScriptInteractCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_is_healthy && game_is_active)
	{
		RValue object = Arguments[0]->ToInstance();
		int object_id = object.GetMember("object_id").ToInt64();
		std::string object_name = object_id_to_name_map[object_id];

		if (object_name == SUMMONING_CIRCLE_ITEM_NAME)
		{
			if (summoning_circle_positions.size() < 2)
			{
				CreateNotification(SUMMONING_CIRCLE_TWO_REQUIRED_NOTIFICATION_KEY, Self, Other);
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] Two summoning circles are required to use it!", MOD_NAME, VERSION);
			}
			else
			{
				int x = object.GetMember("top_left_x").ToInt64();
				int y = object.GetMember("top_left_y").ToInt64();

				for (auto it : summoning_circle_positions)
				{
					if (it.first != x || it.second != y)
					{
						teleport_ari_to = it;
						break;
					}
				}

				if (summoning_circle_confirmation_required)
				{
					play_conversation = true;
				}
				else
				{
					teleport_ari = true;
					g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] Teleporting Ari.", MOD_NAME, VERSION);
				}
			}

			return Result;
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_INTERACT));
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
	if (mod_is_healthy && game_is_active)
	{
		if (ari_current_location == FARM_LOCATION_NAME && Arguments[0]->ToString() == "misc_local/input_interact")
		{
			for (auto it : summoning_circle_positions)
			{
				if (CalculateDistance(ari_x, ari_y, it.first * 8 + 12, it.second * 8 + 12) <= 44)
				{
					custom_interact_key = RValue(SUMMONING_CIRCLE_TELEPORT_INTERACT_KEY);
					custom_interact_key_ptr = &custom_interact_key;
					Arguments[0] = custom_interact_key_ptr;
				}
			}
		}
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

RValue& GmlScriptPlayTextCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_is_healthy && game_is_active)
	{
		std::string conversation_name = Arguments[0]->ToString();
		if (conversation_name == SUMMONING_CIRCLE_ACTIVATION_ACCEPTED_CONVERSATION_KEY)
		{
			teleport_ari = true;
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] Teleporting Ari.", MOD_NAME, VERSION);

			CloseTextbox(Self, Other);
			return Result;
		}
		if (conversation_name == SUMMONING_CIRCLE_ACTIVATION_REJECTED_CONVERSATION_KEY)
		{
			CloseTextbox(Self, Other);
			return Result;
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_PLAY_TEXT));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptWriteFurnitureToLocationCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_is_healthy && game_is_active && 
		Arguments[3]->GetRefMember("object_id")->ToInt64() == object_name_to_id_map[SUMMONING_CIRCLE_ITEM_NAME] &&
		summoning_circle_positions.size() == 2)
	{
		CreateNotification(SUMMONING_CIRCLE_TWO_ALREADY_PRESENT_NOTIFICATION_KEY, Self, Other);
		g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] You already have two summoning circles on your farm!");
		return Result;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_WRITE_FURNITURE_TO_LOCATION));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (mod_is_healthy && game_is_active)
	{
		if (Result.m_Kind != VALUE_UNDEFINED && Result.m_Kind != VALUE_UNSET && Result.m_Kind != VALUE_NULL)
		{
			RValue object_id = Result.GetMember("object_id");
			if (object_id.ToInt64() == object_name_to_id_map[SUMMONING_CIRCLE_ITEM_NAME])
			{
				RValue top_left_x = Result.GetMember("top_left_x");
				RValue top_left_y = Result.GetMember("top_left_y");
				//RValue write_size_x = Result.GetMember("write_size_x");
				//RValue write_size_y = Result.GetMember("write_size_y");

				std::pair<int64_t, int64_t> position = { top_left_x.ToInt64(), top_left_y.ToInt64() };
				summoning_circle_positions.push_back(position);
			}
		}
	}

	return Result;
}

RValue& GmlScriptEraseObjectRendererCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_ERASE_OBJECT_RENDERER));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (mod_is_healthy && game_is_active && ari_current_location == FARM_LOCATION_NAME)
	{
		int object_id = Arguments[0]->GetMember("object_id").ToInt64();
		if (object_id == object_name_to_id_map[SUMMONING_CIRCLE_ITEM_NAME])
		{
			int x = Arguments[0]->GetMember("top_left_x").ToInt64();
			int y = Arguments[0]->GetMember("top_left_y").ToInt64();

			auto it = std::find_if(summoning_circle_positions.begin(), summoning_circle_positions.end(),
				[x, y](const std::pair<int, int>& p) {
					return p.first == x && p.second == y;
				});

			if (it != summoning_circle_positions.end()) {
				summoning_circle_positions.erase(it);
			}
		}
	}

	return Result;
}

RValue& GmlScriptOnRoomStartCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_is_healthy && game_is_active && once_per_save_load && ari_current_location == FARM_LOCATION_NAME)
	{
		once_per_save_load = false;
		PruneMissingSummoningCircles();
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_ON_ROOM_START));
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
	if (mod_is_healthy)
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

	if (mod_is_healthy && game_is_active && Result.m_Kind == VALUE_STRING)
		ari_current_location = Result.ToString();

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
	if (mod_is_healthy)
	{
		// Get the save file name.
		std::string save_file = std::string(Arguments[0]->GetRefMember("save_path")->ToString());
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

RValue& GmlScriptSaveGameCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_is_healthy)
	{
		// No save prefix has been detected. This should only happen when a new game is started.
		if (save_prefix.size() == 0)
		{
			// Get the save file name.
			std::string save_file = Arguments[0]->ToString();
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

		if (save_prefix.size() != 0)
			WriteModSaveFile();
	}

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

		mod_is_healthy &= LoadItemIds(global_instance);
		mod_is_healthy &= LoadObjectIds(Self, Other);
		mod_is_healthy &= LoadLocationIds(global_instance);
		mod_is_healthy &= CreateOrLoadModConfigFile();
		ModifyItem(item_name_to_id_map[SUMMONING_CIRCLE_ITEM_NAME]);

		if (!mod_is_healthy)
			g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - The mod loaded in an unhealthy state and will NOT function!", MOD_NAME, VERSION);
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

void CreateHookGmlScriptInteract(AurieStatus& status)
{
	CScript* gml_script_try_object_id_to_string = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_INTERACT,
		(PVOID*)&gml_script_try_object_id_to_string
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_INTERACT);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_INTERACT,
		gml_script_try_object_id_to_string->m_Functions->m_ScriptFunction,
		GmlScriptInteractCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_INTERACT);
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

void CreateHookGmlScriptPlayText(AurieStatus& status)
{
	CScript* gml_script_play_text = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_PLAY_TEXT,
		(PVOID*)&gml_script_play_text
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_PLAY_TEXT);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_PLAY_TEXT,
		gml_script_play_text->m_Functions->m_ScriptFunction,
		GmlScriptPlayTextCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_PLAY_TEXT);
	}
}

void CreateHookGmlScriptWriteFurnitureToLocation(AurieStatus& status)
{
	CScript* gml_script_write_furniture_to_location = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_WRITE_FURNITURE_TO_LOCATION,
		(PVOID*)&gml_script_write_furniture_to_location
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_WRITE_FURNITURE_TO_LOCATION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_WRITE_FURNITURE_TO_LOCATION,
		gml_script_write_furniture_to_location->m_Functions->m_ScriptFunction,
		GmlScriptWriteFurnitureToLocationCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_WRITE_FURNITURE_TO_LOCATION);
	}
}

void CreateHookGmlScriptEraseObjectRenderer(AurieStatus& status)
{
	CScript* gml_script_erase_object_renderer = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_ERASE_OBJECT_RENDERER,
		(PVOID*)&gml_script_erase_object_renderer
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ERASE_OBJECT_RENDERER);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_ERASE_OBJECT_RENDERER,
		gml_script_erase_object_renderer->m_Functions->m_ScriptFunction,
		GmlScriptEraseObjectRendererCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ERASE_OBJECT_RENDERER);
	}
}

void CreateHookGmlScriptOnRoomStart(AurieStatus& status)
{
	CScript* gml_script_on_room_start = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_ON_ROOM_START,
		(PVOID*)&gml_script_on_room_start
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ON_ROOM_START);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_ON_ROOM_START,
		gml_script_on_room_start->m_Functions->m_ScriptFunction,
		GmlScriptOnRoomStartCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ON_ROOM_START);
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

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[%s %s] - Plugin starting...", MOD_NAME, VERSION);

	CreateObjectCallback(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptInteract(status);
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

	CreateHookGmlScriptPlayText(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptWriteFurnitureToLocation(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptEraseObjectRenderer(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptOnRoomStart(status);
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

	CreateHookGmlScriptLoadGame(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptSaveGame(status);
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
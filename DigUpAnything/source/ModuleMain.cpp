#pragma comment(lib, "Xinput.lib")
#include <Windows.h>
#include <Xinput.h>
#include <map>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

static const char* const MOD_NAME = "DigUpAnything";
static const char* const VERSION = "1.2.0";
static const char* const ACTIVATION_BUTTON_KEY = "activation_button";
static const std::string VALID_ITEM_LOCALIZATION_KEY = "mods/DigUpAnything/valid_item";
static const std::string DISABLED_ITEM_LOCALIZATION_KEY = "mods/DigUpAnything/disabled_item";
static const std::string UNRECOGNIZED_ITEM_LOCALIZATION_KEY = "mods/DigUpAnything/unrecognized_item";
static const std::string ITEM_NOT_ACQUIRED_LOCALIZATION_KEY = "mods/DigUpAnything/item_not_acquired";
static const std::string UNACQUIRED_ITEM_ALREADY_SPAWNED_LOCALIZATION_KEY = "mods/DigUpAnything/unacquired_item_already_spawned";
static const char* const GML_SCRIPT_GIVE_ARI_ITEM = "gml_Script_give_item@Ari@Ari";
static const char* const GML_SCRIPT_CHOOSE_RANDOM_ARTIFACT = "gml_Script_choose_random_artifact@Archaeology@Archaeology";
static const char* const GML_SCRIPT_GET_LOCALIZER = "gml_Script_get@Localizer@Localizer";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const char* const GML_SCRIPT_ON_DRAW_GUI = "gml_Script_on_draw_gui@Display@Display";
static const char* const GML_SCRIPT_CREATE_NOTIFICATION = "gml_Script_create_notification";
static const std::string DISABLED_ITEMS[] = { "animal_cosmetic", "cosmetic", "crafting_scroll", "purse", "recipe_scroll", "unidentified_artifact" };
static const std::string DEFAULT_ACTIVATION_BUTTON = "F11";
static const std::string ALLOWED_ACTIVATION_BUTTONS[] = {
	"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
	"NUMPAD_0", "NUMPAD_1", "NUMPAD_2", "NUMPAD_3", "NUMPAD_4", "NUMPAD_5", "NUMPAD_6", "NUMPAD_7", "NUMPAD_8", "NUMPAD_9",
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
	"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
	"INSERT", "DELETE", "HOME", "PAGE_UP", "PAGE_DOWN", "NUM_LOCK", "SCROLL_LOCK", "CAPS_LOCK", "PAUSE_BREAK",
	"GAMEPAD_A", "GAMEPAD_B", "GAMEPAD_X", "GAMEPAD_Y", "GAMEPAD_LEFT_SHOULDER", "GAMEPAD_RIGHT_SHOULDER", "GAMEPAD_LEFT_TRIGGER", "GAMEPAD_RIGHT_TRIGGER", "GAMEPAD_DPAD_UP", "GAMEPAD_DPAD_DOWN", "GAMEPAD_DPAD_LEFT", "GAMEPAD_DPAD_RIGHT", "GAMEPAD_LEFT_STICK", "GAMEPAD_RIGHT_STICK", "GAMEPAD_BACK", "GAMEPAD_START"
};

static YYTKInterface* g_ModuleInterface = nullptr;
static RValue __YYTK;
static int item_id = 0;
static bool load_on_start = true;
static bool localize_items = false;
static bool override_next_dig_spot = false;
static bool duplicate_item = false;
static bool activation_button_is_controller_key = false;
static int activation_button_int_value = -1;
static bool processing_user_input = false;
static std::string activation_button = DEFAULT_ACTIVATION_BUTTON;
static std::map<std::string, int> item_name_to_id_map = {};
static std::map<int, std::string> item_id_to_name_map = {};
static std::map<std::string, std::string> item_name_to_localized_name_map = {};
static std::map<std::string, std::string> lowercase_localized_name_to_item_name_map = {};
static std::vector<std::string> unacquired_items_spawned = {};

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

std::string trim(std::string s) {
	// Trim leading
	s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), [](unsigned char ch) {
		return std::isspace(ch);
	}));

	// Trim trailing
	s.erase(std::find_if_not(s.rbegin(), s.rend(), [](unsigned char ch) {
		return std::isspace(ch);
	}).base(), s.end());

	return s;
}

void ResetStaticFields()
{
	item_id = 0;
	override_next_dig_spot = false;
	duplicate_item = false;
	std::vector<std::string> unacquired_items_spawned = {};
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

bool GameWindowHasFocus()
{
	RValue window_has_focus = g_ModuleInterface->CallBuiltin("window_has_focus", {});
	return RValueAsBool(window_has_focus);
}

int ActivationButtonToVirtualKey()
{
	// Function Keys
	if (activation_button == "F1")
		return VK_F1;
	if (activation_button == "F2")
		return VK_F2;
	if (activation_button == "F3")
		return VK_F3;
	if (activation_button == "F4")
		return VK_F4;
	if (activation_button == "F5")
		return VK_F5;
	if (activation_button == "F6")
		return VK_F6;
	if (activation_button == "F7")
		return VK_F7;
	if (activation_button == "F8")
		return VK_F8;
	if (activation_button == "F9")
		return VK_F9;
	if (activation_button == "F10")
		return VK_F10;
	if (activation_button == "F11")
		return VK_F11;
	if (activation_button == "F12")
		return VK_F12;

	// Numpad
	if (activation_button == "NUMPAD_0")
		return VK_NUMPAD0;
	if (activation_button == "NUMPAD_1")
		return VK_NUMPAD1;
	if (activation_button == "NUMPAD_2")
		return VK_NUMPAD2;
	if (activation_button == "NUMPAD_3")
		return VK_NUMPAD3;
	if (activation_button == "NUMPAD_4")
		return VK_NUMPAD4;
	if (activation_button == "NUMPAD_5")
		return VK_NUMPAD5;
	if (activation_button == "NUMPAD_6")
		return VK_NUMPAD6;
	if (activation_button == "NUMPAD_7")
		return VK_NUMPAD7;
	if (activation_button == "NUMPAD_8")
		return VK_NUMPAD8;
	if (activation_button == "NUMPAD_9")
		return VK_NUMPAD9;

	// Numbers
	if (activation_button == "0")
		return '0';
	if (activation_button == "1")
		return '1';
	if (activation_button == "2")
		return '2';
	if (activation_button == "3")
		return '3';
	if (activation_button == "4")
		return '4';
	if (activation_button == "5")
		return '5';
	if (activation_button == "6")
		return '6';
	if (activation_button == "7")
		return '7';
	if (activation_button == "8")
		return '8';
	if (activation_button == "9")
		return '9';

	// Letters
	if (activation_button == "A")
		return 'A';
	if (activation_button == "B")
		return 'B';
	if (activation_button == "C")
		return 'C';
	if (activation_button == "D")
		return 'D';
	if (activation_button == "E")
		return 'E';
	if (activation_button == "F")
		return 'F';
	if (activation_button == "G")
		return 'G';
	if (activation_button == "H")
		return 'H';
	if (activation_button == "I")
		return 'I';
	if (activation_button == "J")
		return 'J';
	if (activation_button == "K")
		return 'K';
	if (activation_button == "L")
		return 'L';
	if (activation_button == "M")
		return 'M';
	if (activation_button == "N")
		return 'N';
	if (activation_button == "O")
		return 'O';
	if (activation_button == "P")
		return 'P';
	if (activation_button == "Q")
		return 'Q';
	if (activation_button == "R")
		return 'R';
	if (activation_button == "S")
		return 'S';
	if (activation_button == "T")
		return 'T';
	if (activation_button == "U")
		return 'U';
	if (activation_button == "V")
		return 'V';
	if (activation_button == "W")
		return 'W';
	if (activation_button == "X")
		return 'X';
	if (activation_button == "Y")
		return 'Y';
	if (activation_button == "Z")
		return 'Z';

	// Special
	if (activation_button == "INSERT")
		return VK_INSERT;
	if (activation_button == "DELETE")
		return VK_DELETE;
	if (activation_button == "HOME")
		return VK_HOME;
	if (activation_button == "PAGE_UP")
		return VK_PRIOR;
	if (activation_button == "PAGE_DOWN")
		return VK_NEXT;
	if (activation_button == "NUM_LOCK")
		return VK_NUMLOCK;
	if (activation_button == "SCROLL_LOCK")
		return VK_SCROLL;
	if (activation_button == "CAPS_LOCK")
		return VK_CAPITAL;
	if (activation_button == "PAUSE_BREAK")
		return VK_PAUSE;
}

int ActivationButtonToControllerKey()
{
	if (activation_button == "GAMEPAD_A")
		return XINPUT_GAMEPAD_A;
	if (activation_button == "GAMEPAD_B")
		return XINPUT_GAMEPAD_B;
	if (activation_button == "GAMEPAD_X")
		return XINPUT_GAMEPAD_X;
	if (activation_button == "GAMEPAD_Y")
		return XINPUT_GAMEPAD_Y;
	if (activation_button == "GAMEPAD_LEFT_SHOULDER")
		return XINPUT_GAMEPAD_LEFT_SHOULDER;
	if (activation_button == "GAMEPAD_RIGHT_SHOULDER")
		return XINPUT_GAMEPAD_RIGHT_SHOULDER;
	if (activation_button == "GAMEPAD_DPAD_UP")
		return XINPUT_GAMEPAD_DPAD_UP;
	if (activation_button == "GAMEPAD_DPAD_DOWN")
		return XINPUT_GAMEPAD_DPAD_DOWN;
	if (activation_button == "GAMEPAD_DPAD_LEFT")
		return XINPUT_GAMEPAD_DPAD_LEFT;
	if (activation_button == "GAMEPAD_DPAD_RIGHT")
		return XINPUT_GAMEPAD_DPAD_RIGHT;
	if (activation_button == "GAMEPAD_LEFT_STICK")
		return XINPUT_GAMEPAD_LEFT_THUMB;
	if (activation_button == "GAMEPAD_RIGHT_STICK")
		return XINPUT_GAMEPAD_RIGHT_THUMB;
	if (activation_button == "GAMEPAD_BACK")
		return XINPUT_GAMEPAD_BACK;
	if (activation_button == "GAMEPAD_START")
		return XINPUT_GAMEPAD_START;
	return -1;
}

bool CheckControllerInput()
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	if (XInputGetState(0, &state) == ERROR_SUCCESS)
	{
		WORD buttons = state.Gamepad.wButtons;

		if (activation_button_int_value >= 0)
		{
			if (buttons & activation_button_int_value)
			{
				processing_user_input = true;
				return true;
			}
		}
		else if (activation_button == "GAMEPAD_LEFT_TRIGGER")
		{
			BYTE left_trigger = state.Gamepad.bLeftTrigger;
			if (left_trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			{
				processing_user_input = true;
				return true;
			}
		}
		else if (activation_button == "GAMEPAD_RIGHT_TRIGGER")
		{
			BYTE right_trigger = state.Gamepad.bRightTrigger;
			if (right_trigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			{
				processing_user_input = true;
				return true;
			}
		}
	}

	return false;
}

bool CheckVirtualKeyInput()
{
	RValue key_pressed = g_ModuleInterface->CallBuiltin("keyboard_check_pressed", { activation_button_int_value });
	if (RValueAsBool(key_pressed))
	{
		processing_user_input = true;
		return true;
	}
	return false;
}

void ConfigureActivationButton()
{
	if (activation_button.find("GAMEPAD") != std::string::npos)
	{
		activation_button_is_controller_key = true;
		activation_button_int_value = ActivationButtonToControllerKey();
	}
	else
	{
		activation_button_is_controller_key = false;
		activation_button_int_value = ActivationButtonToVirtualKey();
	}
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

void LogDefaultConfigValues()
{
	activation_button = DEFAULT_ACTIVATION_BUTTON;

	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, ACTIVATION_BUTTON_KEY, DEFAULT_ACTIVATION_BUTTON);
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

		// Try to find the mod_data/DigUpAnything directory.
		std::string dig_up_anything_folder = mod_data_folder + "\\DigUpAnything";
		if (!std::filesystem::exists(dig_up_anything_folder))
		{
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"DigUpAnything\" directory was not found. Creating directory: %s", MOD_NAME, VERSION, dig_up_anything_folder.c_str());
			std::filesystem::create_directory(dig_up_anything_folder);
		}

		// Try to find the mod_data/DigUpAnything/DigUpAnything.json config file.
		std::string config_file = dig_up_anything_folder + "\\" + "DigUpAnything.json";
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
					if (json_object.contains(ACTIVATION_BUTTON_KEY))
					{
						activation_button = json_object[ACTIVATION_BUTTON_KEY];
						auto allowed_button = std::find(std::begin(ALLOWED_ACTIVATION_BUTTONS), std::end(ALLOWED_ACTIVATION_BUTTONS), activation_button);
						if (allowed_button == std::end(ALLOWED_ACTIVATION_BUTTONS))
						{
							g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Invalid \"%s\" value (%s) in mod configuration file: %s", MOD_NAME, VERSION, ACTIVATION_BUTTON_KEY, activation_button, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Make sure the value is one of the supported keys!", MOD_NAME, VERSION);
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, ACTIVATION_BUTTON_KEY, DEFAULT_ACTIVATION_BUTTON.c_str());
							activation_button = DEFAULT_ACTIVATION_BUTTON;
						}
						else
						{
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using CUSTOM \"%s\" value: %s!", MOD_NAME, VERSION, ACTIVATION_BUTTON_KEY, activation_button.c_str());
						}
					}
					else
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s!", MOD_NAME, VERSION, ACTIVATION_BUTTON_KEY, config_file.c_str());
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Using DEFAULT \"%s\" value: %s!", MOD_NAME, VERSION, ACTIVATION_BUTTON_KEY, DEFAULT_ACTIVATION_BUTTON.c_str());
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

			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"DigUpAnything.json\" file was not found. Creating file: %s", MOD_NAME, VERSION, config_file.c_str());
			json default_json = {
				{ACTIVATION_BUTTON_KEY, DEFAULT_ACTIVATION_BUTTON}
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

void DisplayNotification(CInstance* Self, CInstance* Other, std::string localization_key)
{
	CScript* gml_script_create_notification = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CREATE_NOTIFICATION,
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
			std::string modal_text =
				"DigUpAnything v" + std::string(VERSION) + "\r\n" +
				"------------------------------\r\n" +
				"How many of the item would you like?\r\n"
				"Input a number between 1 and 999.\r\n";

			RValue user_input = g_ModuleInterface->CallBuiltin(
				"get_integer",
				{
					modal_text,
					1
				}
			);
			
			double value = user_input.m_Real;
			if (value < 1)
				value = 1;
			if (value > 999)
				value = 999;

			Arguments[1]->m_Real = value;
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Spawning %d of the requested item.", MOD_NAME, VERSION, static_cast<int>(value));
		}
		else
		{
			unacquired_items_spawned.push_back(item_id_to_name_map[item_id]);
			DisplayNotification(Self, Other, ITEM_NOT_ACQUIRED_LOCALIZATION_KEY);
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The requested item hasn't been acquired before. Only 1 may be spawned.", MOD_NAME, VERSION);
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GIVE_ARI_ITEM));
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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_CHOOSE_RANDOM_ARTIFACT));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (override_next_dig_spot)
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Modified the artifact spot!", MOD_NAME, VERSION);
		Result.m_i64 = item_id;
		override_next_dig_spot = false;
		duplicate_item = true;
	}

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
	if (load_on_start)
	{
		CreateOrLoadConfigFile();
		ConfigureActivationButton();
		CreateOrGetGlobalYYTKVariable();
		CreateModInfoInGlobalYYTKVariable();

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
				item_id_to_name_map[RValueAsInt(item_id)] = recipe_key.AsString().data();
				item_name_to_localized_name_map[recipe_key.AsString().data()] = name_key.AsString().data();
			}
		}
		if (item_name_to_id_map.size() > 0)
		{
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Loaded data for %d items!", MOD_NAME, VERSION, item_name_to_id_map.size());
		}
		else {
			g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to load data for items!", MOD_NAME, VERSION);
		}

		localize_items = true;
		load_on_start = false;
	}
	else
	{
		ResetStaticFields();
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

RValue& GmlScriptOnDrawGuiCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (GameWindowHasFocus() && !processing_user_input)
	{
		bool activate = false;
		if (activation_button_is_controller_key && CheckControllerInput())
			activate = true;
		if (!activation_button_is_controller_key && CheckVirtualKeyInput())
			activate = true;

		if (activate)
		{
			std::string modal_text =
				"DigUpAnything v" + std::string(VERSION) + "\r\n" +
				"------------------------------\r\n" +
				"Input the desired item's Display Name or Internal Name.\r\n" +
				"Case (capitalization) does not matter.\r\n" +
				"Example 1 (Display Name): Tea with Lemon\r\n" +
				"Example 2 (Display Name): tea with lemon\r\n" +
				"Example 3 (Internal Name): cup_of_tea\r\n" +
				"More Info: https://github.com/AnnaNomoly/YYToolkit/tree/stable/DigUpAnything";

			RValue user_input = g_ModuleInterface->CallBuiltin(
				"get_string",
				{
					modal_text,
					"Tea with Lemon"
				}
			);

			// Convert the user input to lowercase.
			std::string user_input_str = trim(user_input.AsString().data());
			std::transform(user_input_str.begin(), user_input_str.end(), user_input_str.begin(), [](unsigned char c) { return std::tolower(c); });

			// Check if it was a localized item name.
			if (lowercase_localized_name_to_item_name_map.count(user_input_str) > 0)
				user_input_str = lowercase_localized_name_to_item_name_map[user_input_str];

			if (item_name_to_id_map.count(user_input_str) > 0)
			{
				UpdateUnacquiredItemsSpawnedList(item_name_to_id_map[user_input_str]);
				auto unacquired_item_already_spawned = std::find(std::begin(unacquired_items_spawned), std::end(unacquired_items_spawned), user_input_str);
				auto disabled_item = std::find(std::begin(DISABLED_ITEMS), std::end(DISABLED_ITEMS), user_input_str);

				if (unacquired_item_already_spawned != std::end(unacquired_items_spawned))
				{
					DisplayNotification(Self, Other, UNACQUIRED_ITEM_ALREADY_SPAWNED_LOCALIZATION_KEY);
					g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Ignoring duplicate unacquired item: %s", MOD_NAME, VERSION, user_input_str.c_str());
				}
				else if (disabled_item != std::end(DISABLED_ITEMS))
				{
					DisplayNotification(Self, Other, DISABLED_ITEM_LOCALIZATION_KEY);
					g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Ignoring disabled item: %s", MOD_NAME, VERSION, user_input_str.c_str());
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

					item_id = item_name_to_id_map[user_input_str];
					override_next_dig_spot = true;
					DisplayNotification(Self, Other, VALID_ITEM_LOCALIZATION_KEY);
					g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Next artifact spot will be: %s", MOD_NAME, VERSION, user_input_str.c_str());
				}
			}
			else
			{
				DisplayNotification(Self, Other, UNRECOGNIZED_ITEM_LOCALIZATION_KEY);
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Ignoring unrecognized item: %s", MOD_NAME, VERSION, user_input_str.c_str());
			}
		}

		processing_user_input = false;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_ON_DRAW_GUI));
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
		GML_SCRIPT_GIVE_ARI_ITEM,
		(PVOID*)&gml_script_give_item
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GIVE_ARI_ITEM);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GIVE_ARI_ITEM,
		gml_script_give_item->m_Functions->m_ScriptFunction,
		GmlScriptGiveItemCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GIVE_ARI_ITEM);
	}
}

void CreateHookGmlScriptChooseRandomArtifact(AurieStatus& status)
{
	CScript* gml_script_choose_random_artifact = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CHOOSE_RANDOM_ARTIFACT,
		(PVOID*)&gml_script_choose_random_artifact
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CHOOSE_RANDOM_ARTIFACT);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_CHOOSE_RANDOM_ARTIFACT,
		gml_script_choose_random_artifact->m_Functions->m_ScriptFunction,
		GmlScriptChooseRandomArtifactCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CHOOSE_RANDOM_ARTIFACT);
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

void CreateHookGmlScriptOnDrawGui(AurieStatus& status)
{
	CScript* gml_script_on_draw_gui = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_ON_DRAW_GUI,
		(PVOID*)&gml_script_on_draw_gui
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ON_DRAW_GUI);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_ON_DRAW_GUI,
		gml_script_on_draw_gui->m_Functions->m_ScriptFunction,
		GmlScriptOnDrawGuiCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ON_DRAW_GUI);
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

	CreateHookGmlScriptGiveItem(status);
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

	CreateHookGmlScriptOnDrawGui(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Plugin started!", MOD_NAME, VERSION);
	return AURIE_SUCCESS;
}
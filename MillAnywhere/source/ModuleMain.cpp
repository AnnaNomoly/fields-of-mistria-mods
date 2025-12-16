#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <YYToolkit/YYTK_Shared.hpp> // YYTK v4
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

static const char* const MOD_NAME = "MillAnywhere";
static const char* const VERSION = "1.1.1";
static const char* const ACTIVATION_BUTTON_KEY = "activation_button";
static const char* const GML_SCRIPT_CREATE_MILL_MENU = "gml_Script_anon@382@gml_Object_obj_mill_menu_Create_0";
static const char* const GML_SCRIPT_GET_WEATHER = "gml_Script_get_weather@WeatherManager@Weather";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const char* const GML_SCRIPT_ON_DRAW_GUI = "gml_Script_on_draw_gui@Display@Display";
static const std::string DEFAULT_ACTIVATION_BUTTON = "F12";
static const std::string ALLOWED_ACTIVATION_BUTTONS[] = {
	"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
	"NUMPAD_0", "NUMPAD_1", "NUMPAD_2", "NUMPAD_3", "NUMPAD_4", "NUMPAD_5", "NUMPAD_6", "NUMPAD_7", "NUMPAD_8", "NUMPAD_9",
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
	"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
	"INSERT", "DELETE", "HOME", "PAGE_UP", "PAGE_DOWN", "NUM_LOCK", "SCROLL_LOCK", "CAPS_LOCK", "PAUSE_BREAK",
	"GAMEPAD_A", "GAMEPAD_B", "GAMEPAD_X", "GAMEPAD_Y", "GAMEPAD_LEFT_SHOULDER", "GAMEPAD_RIGHT_SHOULDER", "GAMEPAD_LEFT_TRIGGER", "GAMEPAD_RIGHT_TRIGGER", "GAMEPAD_DPAD_UP", "GAMEPAD_DPAD_DOWN", "GAMEPAD_DPAD_LEFT", "GAMEPAD_DPAD_RIGHT", "GAMEPAD_LEFT_STICK", "GAMEPAD_RIGHT_STICK", "GAMEPAD_SELECT", "GAMEPAD_START"
};

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_on_start = true;
static bool game_is_active = false;
static bool activation_button_is_controller_key = false;
static int activation_button_int_value = -1;
static bool processing_user_input = false;
static std::string activation_button = DEFAULT_ACTIVATION_BUTTON;

bool RValueAsBool(RValue value)
{
	if (value.m_Kind == VALUE_REAL && value.m_Real == 1)
		return true;
	if (value.m_Kind == VALUE_BOOL && value.m_Real == 1)
		return true;
	return false;
}

bool GameIsPaused()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);
	RValue paused = global_instance->GetMember("__pause_status");
	return paused.ToInt64() > 0;
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
		return 0x8001;
	if (activation_button == "GAMEPAD_B")
		return 0x8002;
	if (activation_button == "GAMEPAD_X")
		return 0x8003;
	if (activation_button == "GAMEPAD_Y")
		return 0x8004;
	if (activation_button == "GAMEPAD_LEFT_SHOULDER")
		return 0x8005;
	if (activation_button == "GAMEPAD_RIGHT_SHOULDER")
		return 0x8006;
	if (activation_button == "GAMEPAD_LEFT_TRIGGER")
		return 0x8007;
	if (activation_button == "GAMEPAD_RIGHT_TRIGGER")
		return 0x8008;
	if (activation_button == "GAMEPAD_SELECT")
		return 0x8009;
	if (activation_button == "GAMEPAD_START")
		return 0x800A;
	if (activation_button == "GAMEPAD_LEFT_STICK")
		return 0x800B;
	if (activation_button == "GAMEPAD_RIGHT_STICK")
		return 0x800C;
	if (activation_button == "GAMEPAD_DPAD_UP")
		return 0x800D;
	if (activation_button == "GAMEPAD_DPAD_DOWN")
		return 0x800E;
	if (activation_button == "GAMEPAD_DPAD_LEFT")
		return 0x800F;
	if (activation_button == "GAMEPAD_DPAD_RIGHT")
		return 0x8010;
	return -1;
}

int GetGamepadSlotNumber()
{
	for (int i = 0; i < 12; i++)
	{
		RValue gamepad_is_connected = g_ModuleInterface->CallBuiltin("gamepad_is_connected", { i });
		if (gamepad_is_connected.ToBoolean())
		{
			return i;
		}
	}

	return -1;
}

bool CheckControllerInput()
{
	int gamepad_slot = GetGamepadSlotNumber();
	if (gamepad_slot != -1)
	{
		RValue button_pressed = g_ModuleInterface->CallBuiltin("gamepad_button_check_pressed", { gamepad_slot, activation_button_int_value });
		return button_pressed.ToBoolean();
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

json CreateConfigJson(bool use_defaults)
{
	json config_json = {
		{ ACTIVATION_BUTTON_KEY, use_defaults ? DEFAULT_ACTIVATION_BUTTON : activation_button }
	};
	return config_json;
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

		// Try to find the mod_data/MillAnywhere directory.
		std::string mill_anywhere_folder = mod_data_folder + "\\MillAnywhere";
		if (!std::filesystem::exists(mill_anywhere_folder))
		{
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"MillAnywhere\" directory was not found. Creating directory: %s", MOD_NAME, VERSION, mill_anywhere_folder.c_str());
			std::filesystem::create_directory(mill_anywhere_folder);
		}

		// Try to find the mod_data/MillAnywhere/MillAnywhere.json config file.
		bool update_config_file = false;
		std::string config_file = mill_anywhere_folder + "\\" + "MillAnywhere.json";
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

				update_config_file = true;
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

			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"MillAnywhere.json\" file was not found. Creating file: %s", MOD_NAME, VERSION, config_file.c_str());

			json default_config_json = CreateConfigJson(true);
			std::ofstream out_stream(config_file);
			out_stream << std::setw(4) << default_config_json << std::endl;
			out_stream.close();

			LogDefaultConfigValues();
		}

		if (update_config_file)
		{
			json config_json = CreateConfigJson(false);
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
		LogDefaultConfigValues();
	}
}

void OpenMillMenu(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_mill_menu = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CREATE_MILL_MENU,
		(PVOID*)&gml_script_mill_menu
	);

	RValue retval;
	gml_script_mill_menu->m_Functions->m_ScriptFunction(
		Self,
		Other,
		retval,
		0,
		nullptr
	);
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

RValue& GmlScriptSetupMainScreenCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	game_is_active = false;

	if (load_on_start)
	{
		CreateOrLoadConfigFile();
		ConfigureActivationButton();

		load_on_start = false;
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
	if (GameWindowHasFocus() && !GameIsPaused() && !processing_user_input && game_is_active)
	{
		bool activate = false;
		if (activation_button_is_controller_key && CheckControllerInput())
			activate = true;
		if (!activation_button_is_controller_key && CheckVirtualKeyInput())
			activate = true;

		if (activate)
			OpenMillMenu(Self, Other);

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
	CScript* gml_script_try_location_id_to_string = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_ON_DRAW_GUI,
		(PVOID*)&gml_script_try_location_id_to_string
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ON_DRAW_GUI);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_ON_DRAW_GUI,
		gml_script_try_location_id_to_string->m_Functions->m_ScriptFunction,
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

	CreateHookGmlScriptGetWeather(status);
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
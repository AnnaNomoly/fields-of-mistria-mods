#include <iostream>
#include <fstream>
#include <codecvt>
#include <shlobj.h>
#include <filesystem>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static const char* const VERSION = "1.0.0";
static const int DEFAULT_FRIENDSHIP_MULTIPLIER = 5;

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_on_start = true;
static int friendship_multiplier = DEFAULT_FRIENDSHIP_MULTIPLIER;

std::string wstr_to_string(std::wstring wstr)
{
	std::vector<char> buf(wstr.size());
	std::use_facet<std::ctype<wchar_t>>(std::locale{}).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());

	return std::string(buf.data(), buf.size());
}

RValue& GmlScriptAddHeartPointsAnimalCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	int original_heart_points = static_cast<int>(Arguments[0]->m_Real);
	if (original_heart_points < 0)
	{
		original_heart_points = 0;
		Arguments[0]->m_Real = 0.0;
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Prevented animal heart points from being reduced!", VERSION);
	}

	int modified_heart_points = std::round(Arguments[0]->m_Real * friendship_multiplier);
	Arguments[0]->m_Real = static_cast<double>(modified_heart_points);
	if (modified_heart_points > 0)
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Boosted animal heart points added from %d to %d!", VERSION, original_heart_points, modified_heart_points);
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_add_heart_points@PlayerAnimal@Animal"));
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
		try
		{
			// Check if we can find the FieldsOfMistria directory in APP_DATA_LOCAL
			wchar_t* localAppDataFolder;
			if (SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_CREATE, NULL, &localAppDataFolder) == S_OK)
			{
				// Check if the SaveAnywhere mod directory exists.
				std::string mod_data_folder = wstr_to_string(localAppDataFolder) + "\\FieldsOfMistria\\mod_data\\AnimalFriends";
				if (std::filesystem::exists(mod_data_folder))
				{
					// Read from the save file in mod_data.
					std::string config_file = mod_data_folder + "\\" + "friendship_boost.txt";
					std::ifstream file(config_file);
					if (file.good())
					{
						
						std::string value_from_file;
						file >> value_from_file;

						try
						{
							int number = std::stoi(value_from_file);
							if (number <= 0 || number > 100)
							{
								g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - Invalid integer value (%d) in mod configuration file: %s", VERSION, number, config_file.c_str());
								g_ModuleInterface->Print(CM_LIGHTYELLOW, "[AnimalFriends %s] - Make sure the value is a valid integer between 1 and 100 (inclusive)!", VERSION);
								g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Using default friendship multiplier: %d!", VERSION, DEFAULT_FRIENDSHIP_MULTIPLIER);
							}
							else
							{
								friendship_multiplier = number;
								g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Successfully loaded integer value (%d) in mod configuration file: %s", VERSION, number, config_file.c_str());
								g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Using custom friendship multiplier: %d!", VERSION, number);
							}
						}
						catch (...)
						{
							g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - Failed to parse integer from input string (%s) in mod configuration file: %s", VERSION, value_from_file, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[AnimalFriends %s] - Make sure the only value in that file is a valid integer!", VERSION);
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Using default friendship multiplier: %d!", VERSION, DEFAULT_FRIENDSHIP_MULTIPLIER);
						}
					}
					file.close();
				}
				else
				{
					g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Using default friendship multiplier: %d!", VERSION, DEFAULT_FRIENDSHIP_MULTIPLIER);
				}
			}
			else
			{
				g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Using default friendship multiplier: %d!", VERSION, DEFAULT_FRIENDSHIP_MULTIPLIER);
			}
		}
		catch (...)
		{
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Using default friendship multiplier: %d!", VERSION, DEFAULT_FRIENDSHIP_MULTIPLIER);
		}

		load_on_start = false;
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

void CreateHookGmlScriptAddHeartPointsAnimal(AurieStatus& status)
{
	CScript* gml_script_add_heart_points = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_add_heart_points@PlayerAnimal@Animal",
		(PVOID*)&gml_script_add_heart_points
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - Failed to get script (gml_Script_add_heart_points@PlayerAnimal@Animal)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_add_heart_points@PlayerAnimal@Animal",
		gml_script_add_heart_points->m_Functions->m_ScriptFunction,
		GmlScriptAddHeartPointsAnimalCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - Failed to hook script (gml_Script_add_heart_points@PlayerAnimal@Animal)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - Failed to get script (gml_Script_setup_main_screen)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - Failed to hook script (gml_Script_setup_main_screen)!", VERSION);
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

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[AnimalFriends %s] - Plugin starting...", VERSION);

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - Exiting due to failure on start!", VERSION);
		return status;
	}
	
	CreateHookGmlScriptAddHeartPointsAnimal(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Plugin started!", VERSION);
	return AURIE_SUCCESS;
}
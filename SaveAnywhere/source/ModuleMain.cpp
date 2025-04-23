#include <map>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <shlobj.h>
#include <filesystem>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static const char* const VERSION = "1.0.2";
static const std::string MINES_ENTRY = "mines_entry";
static const std::string DUNGEON = "dungeon";
static const std::string WATER_SEAL = "water_seal";
static const std::string EARTH_SEAL = "earth_seal";
static const std::string FIRE_SEAL = "fire_seal";
static const std::string RUINS_SEAL = "ruins_seal";
static const double UNSET_INT = -1;

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_on_start = true;
static bool mod_healthy = false;
static bool mod_save = false;
static int ari_x = UNSET_INT;
static int ari_y = UNSET_INT;
static int ari_room_id = UNSET_INT;
static int saved_x = UNSET_INT;
static int saved_y = UNSET_INT;
static std::string saved_room_name = "";
static bool wait_to_teleport_ari = false;
static bool ready_to_teleport_ari = false;
static bool wait_to_reposition_ari = false;
static bool ready_to_reposition_ari = false;
static std::string save_prefix = "";
static std::string save_folder = "";
static std::string mod_folder = "";
static std::map<int, std::string> location_id_to_name_map = {};
static std::map<std::string, int> location_name_to_id_map = {};

void ResetStaticFields(bool returnedToTitleScreen)
{
	if (returnedToTitleScreen)
	{
		save_prefix = "";
	}

	mod_save = false;
	ari_x = UNSET_INT;
	ari_y = UNSET_INT;
	ari_room_id = UNSET_INT;
	saved_x = UNSET_INT;
	saved_y = UNSET_INT;
	saved_room_name = "";
	wait_to_teleport_ari = false;
	ready_to_teleport_ari = false;
	wait_to_reposition_ari = false;
	ready_to_reposition_ari = false;
}

void WriteModFile()
{
	std::string file_name = mod_folder + "\\" + save_prefix + "autosave.sav";
	std::ofstream outfile(file_name, std::ios::out);
	if (outfile.is_open())
	{
		bool dungeon_location_override = false;
		if (location_id_to_name_map.count(ari_room_id) > 0)
		{
			if (location_id_to_name_map[ari_room_id] == DUNGEON)
				dungeon_location_override = true;
			if (location_id_to_name_map[ari_room_id] == WATER_SEAL)
				dungeon_location_override = true;
			if (location_id_to_name_map[ari_room_id] == EARTH_SEAL)
				dungeon_location_override = true;
			if (location_id_to_name_map[ari_room_id] == FIRE_SEAL)
				dungeon_location_override = true;
			if (location_id_to_name_map[ari_room_id] == RUINS_SEAL)
				dungeon_location_override = true;
		}

		if (dungeon_location_override)
		{
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[SaveAnywhere %s] - You are currently in the dungeon! Your location will be saved as \"mines_entry\" to avoid errors.", VERSION);
			outfile << MINES_ENTRY + " " + std::to_string(216) + " " + std::to_string(198);
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[SaveAnywhere %s] - Saved your game at the fallback location: \"mines_entry\".", VERSION);
		}
		else {
			outfile << location_id_to_name_map[ari_room_id] + " " + std::to_string(ari_x) + " " + std::to_string(ari_y);
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[SaveAnywhere %s] - Saved your game at your current location: \"%s\".", VERSION, location_id_to_name_map[ari_room_id]);
		}
	}
	outfile.close();
}

void SaveGame(CInstance* Self, CInstance* Other)
{
	mod_save = true;

	CScript* gml_script_save_game = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_save_game",
		(PVOID*)&gml_script_save_game
	);

	std::string file_name = save_folder + "\\" + save_prefix + "autosave.sav";
	RValue result;
	RValue argument = file_name;
	RValue* argument_ptr = &argument;

	gml_script_save_game->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &argument_ptr }
	);
}

void DisplaySaveNotification(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_save_game_notification = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_create_save_notification",
		(PVOID*)&gml_script_save_game_notification
	);

	RValue result2;
	gml_script_save_game_notification->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result2,
		0,
		nullptr
	);
}

bool LoadLocationIds()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	// Load locations.
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

	if (location_id_to_name_map.size() > 0 && location_name_to_id_map.size() > 0)
	{
		return true;
	}
	else {
		g_ModuleInterface->Print(CM_LIGHTRED, "[SaveAnywhere %s] - Failed to load location IDs!", VERSION);
		return false;
	}
}

void handle_eptr(std::exception_ptr eptr)
{
	try {
		if (eptr) {
			std::rethrow_exception(eptr);
		}
	}
	catch (const std::exception& e) {
		g_ModuleInterface->Print(CM_LIGHTRED, "[SaveAnywhere %s] - Error: %s", VERSION, e.what());
	}
}

std::string wstr_to_string(std::wstring wstr)
{
	std::vector<char> buf(wstr.size());
	std::use_facet<std::ctype<wchar_t>>(std::locale{}).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());

	return std::string(buf.data(), buf.size());
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

	if (mod_healthy)
	{
		RValue x;
		g_ModuleInterface->GetBuiltin("x", self, NULL_INDEX, x);
		ari_x = x.m_Real;

		RValue y;
		g_ModuleInterface->GetBuiltin("y", self, NULL_INDEX, y);
		ari_y = y.m_Real;

		if (wait_to_teleport_ari && ready_to_teleport_ari)
		{
			CScript* gml_script_goto_location_id = nullptr;
			g_ModuleInterface->GetNamedRoutinePointer(
				"gml_Script_goto_location_id",
				(PVOID*)&gml_script_goto_location_id
			);

			RValue result;
			RValue location_id = location_name_to_id_map[saved_room_name];
			RValue* location_id_ptr = &location_id;
			gml_script_goto_location_id->m_Functions->m_ScriptFunction(
				self,
				other,
				result,
				1,
				{ &location_id_ptr }
			);

			saved_room_name = "";
			wait_to_teleport_ari = false;
			ready_to_teleport_ari = false;
			wait_to_reposition_ari = true;
		}
		else if (wait_to_reposition_ari && ready_to_reposition_ari) {
			RValue x = saved_x;
			RValue y = saved_y;

			g_ModuleInterface->SetBuiltin("x", self, NULL_INDEX, x);
			g_ModuleInterface->SetBuiltin("y", self, NULL_INDEX, y);

			saved_x = UNSET_INT;
			saved_y = UNSET_INT;
			wait_to_reposition_ari = false;
			ready_to_reposition_ari = false;
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
	if (!mod_save)
	{
		// No save prefix has been detected. This should only happen when a new game is started.
		if (save_prefix.size() == 0)
		{
			// Get the save file name.
			std::string save_file = Arguments[0]->AsString().data();
			std::size_t index = save_file.find_last_of("/");
			std::string save_name = save_file.substr(index + 1);

			// Check it's a valid value.
			if (save_name.find("undefined") == std::string::npos)
			{
				// Get the save prefix.
				index = save_name.find_last_of("-");
				save_prefix = save_name.substr(0, index + 1);
			}
		}
	}
	else
	{
		mod_save = false;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_save_game"));
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
		std::string arg0_str = std::string(Arguments[0]->m_Object->at("save_path").AsString().data());
		std::size_t index = arg0_str.find_last_of("/");
		std::string save_name = arg0_str.substr(index + 1);

		// Get the save prefix.
		index = save_name.find_last_of("-");
		save_prefix = save_name.substr(0, index + 1);

		if (save_name.find("autosave") != std::string::npos)
		{
			// Read from the save file in mod_data.
			std::ifstream file(mod_folder + "\\" + save_name);
			if (file.good())
			{
				file >> saved_room_name >> saved_x >> saved_y;
				wait_to_teleport_ari = true;

				g_ModuleInterface->Print(CM_LIGHTGREEN, "[SaveAnywhere %s] - Loaded your saved location: \"%s\".", VERSION, saved_room_name.c_str());
			}
			file.close();
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_load_game"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptShowRoomTitleCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_healthy)
	{
		if (wait_to_reposition_ari) {
			ready_to_reposition_ari = true;
		}
		if (wait_to_teleport_ari) {
			ready_to_teleport_ari = true;
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_show_room_title"));
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
	if (mod_healthy)
	{
		if(Arguments[0]->m_Kind == VALUE_REAL)
			ari_room_id = Arguments[0]->m_Real;
		if (Arguments[0]->m_Kind == VALUE_INT64)
			ari_room_id = Arguments[0]->m_i64;

		if (GetAsyncKeyState(VK_HOME) & 1)
		{
			if (save_prefix.size() != 0)
			{
				WriteModFile();
				SaveGame(Self, Other);
				DisplaySaveNotification(Self, Other);
			}
			else
			{
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[SaveAnywhere %s] - Game was NOT saved! An autosave file has not been created. If this is a new file you must save at the bed at least once.", VERSION);
			}
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_try_location_id_to_string"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptEndDayCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_healthy)
	{
		// Remove the mod file for this save (if one exists).
		std::string file_name = mod_folder + "\\" + save_prefix + "autosave.sav";
		std::remove(file_name.c_str());

		ResetStaticFields(false);
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_end_day"));
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
		std::exception_ptr eptr;
		try
		{
			// Try to find the AppData\Local directory.
			wchar_t* localAppDataFolder;
			if (SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_CREATE, NULL, &localAppDataFolder) == S_OK)
			{
				// Try to find the AppData\Local\FieldsOfMistria directory.
				std::string fields_of_mistria_folder = wstr_to_string(localAppDataFolder) + "\\FieldsOfMistria";
				if (std::filesystem::exists(fields_of_mistria_folder))
				{
					// Try to find the mod_data directory.
					std::string current_dir = std::filesystem::current_path().string();
					std::string mod_data_folder = current_dir + "\\mod_data";
					if (!std::filesystem::exists(mod_data_folder))
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[SaveAnywhere %s] - The \"mod_data\" directory was not found. Creating directory: %s", VERSION, mod_data_folder.c_str());
						std::filesystem::create_directory(mod_data_folder);
					}

					// Try to find the mod_data\SaveAnywhere directory.
					std::string save_anywhere_folder = mod_data_folder + "\\SaveAnywhere";
					if (!std::filesystem::exists(save_anywhere_folder))
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[SaveAnywhere %s] - The \"SaveAnywhere\" directory was not found. Creating directory: %s", VERSION, save_anywhere_folder.c_str());
						std::filesystem::create_directory(save_anywhere_folder);
					}

					save_folder = fields_of_mistria_folder + "\\saves";
					mod_folder = save_anywhere_folder;
					mod_healthy = true;
				}
				else
				{
					g_ModuleInterface->Print(CM_LIGHTRED, "[SaveAnywhere %s] - Failed to find the \"%s\" directory.", VERSION, "AppData\\Local\\FieldsOfMistria");
				}
			}
			else
			{
				g_ModuleInterface->Print(CM_LIGHTRED, "[SaveAnywhere %s] - Failed to find the \"%s\" directory.", VERSION, "AppData\\Local");
			}
		}
		catch (...)
		{
			g_ModuleInterface->Print(CM_LIGHTRED, "[SaveAnywhere %s] - An error occurred when locating the mod directory.", VERSION);

			eptr = std::current_exception();
			handle_eptr(eptr);
		}

		mod_healthy = mod_healthy && LoadLocationIds();
		if (!mod_healthy)
		{
			g_ModuleInterface->Print(CM_LIGHTRED, "[SaveAnywhere %s] - The mod is not healthy and will NOT function!", VERSION);
		}

		load_on_start = false;
	}
	else
	{
		if (mod_healthy)
		{
			ResetStaticFields(true);
		}
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

void CreateHookObject(AurieStatus& status)
{
	status = g_ModuleInterface->CreateCallback(
		g_ArSelfModule,
		EVENT_OBJECT_CALL,
		ObjectCallback,
		0
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SaveAnywhere %s] - Failed to hook (ObjectCallback)!", VERSION);
	}
}

void CreateHookGmlScriptSaveGame(AurieStatus& status)
{
	CScript* gml_script_save_game = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_save_game",
		(PVOID*)&gml_script_save_game
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[SaveAnywhere %s] - Failed to get script (gml_Script_save_game)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_save_game",
		gml_script_save_game->m_Functions->m_ScriptFunction,
		GmlScriptSaveGameCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[SaveAnywhere %s] - Failed to hook script (gml_Script_save_game)!", VERSION);
	}
}

void CreateHookGmlScriptLoadGame(AurieStatus& status)
{
	CScript* gml_script_load_game = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_load_game",
		(PVOID*)&gml_script_load_game
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[SaveAnywhere %s] - Failed to get script (gml_Script_load_game)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_load_game",
		gml_script_load_game->m_Functions->m_ScriptFunction,
		GmlScriptLoadGameCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[SaveAnywhere %s] - Failed to hook script (gml_Script_load_game)!", VERSION);
	}
}

void CreateHookGmlScriptShowRoomTitle(AurieStatus& status)
{
	CScript* gml_script_show_room_title = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_show_room_title",
		(PVOID*)&gml_script_show_room_title
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SaveAnywhere %s] - Failed to get script (gml_Script_show_room_title)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_show_room_title",
		gml_script_show_room_title->m_Functions->m_ScriptFunction,
		GmlScriptShowRoomTitleCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SaveAnywhere %s] - Failed to hook script (gml_Script_show_room_title)!", VERSION);
	}
}

void CreateHookGmlScriptTryLocationIdToString(AurieStatus& status)
{
	CScript* gml_script_try_location_id_to_string = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_try_location_id_to_string",
		(PVOID*)&gml_script_try_location_id_to_string
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[SaveAnywhere %s] - Failed to get script (gml_Script_try_location_id_to_string)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_try_location_id_to_string",
		gml_script_try_location_id_to_string->m_Functions->m_ScriptFunction,
		GmlScriptTryLocationIdToStringCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[SaveAnywhere %s] - Failed to hook script (gml_Script_try_location_id_to_string)!", VERSION);
	}
}

void CreateHookGmlScriptEndDay(AurieStatus& status)
{
	CScript* gml_script_end_day = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_end_day",
		(PVOID*)&gml_script_end_day
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[SaveAnywhere %s] - Failed to get script (gml_Script_end_day)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_end_day",
		gml_script_end_day->m_Functions->m_ScriptFunction,
		GmlScriptEndDayCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[SaveAnywhere %s] - Failed to hook script (gml_Script_end_day)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[SaveAnywhere %s] - Failed to get script (gml_Script_setup_main_screen)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[SaveAnywhere %s] - Failed to hook script (gml_Script_setup_main_screen)!", VERSION);
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

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[SaveAnywhere %s] - Plugin starting...", VERSION);

	CreateHookObject(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SaveAnywhere %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptSaveGame(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SaveAnywhere %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptLoadGame(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SaveAnywhere %s] - Exiting due to failure on start!", VERSION);
		return status;
	}
	
	CreateHookGmlScriptShowRoomTitle(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SaveAnywhere %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptTryLocationIdToString(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SaveAnywhere %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptEndDay(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SaveAnywhere %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SaveAnywhere %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	return AURIE_SUCCESS;
}
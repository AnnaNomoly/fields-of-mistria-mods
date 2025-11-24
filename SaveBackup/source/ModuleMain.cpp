#include <ctime>
#include <chrono>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <shlobj.h>
#include <windows.h>
#include <miniz/miniz.h>
#include <nlohmann/json.hpp>
#include <YYToolkit/YYTK_Shared.hpp> // YYTK v4
using namespace Aurie;
using namespace YYTK;

static const char* const MOD_NAME = "SaveBackup";
static const char* const VERSION = "1.0.0";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const std::string FIELDS_OF_MISTRIA_FOLDER_NAME = "FieldsOfMistria";
static const std::string SAVE_FOLDER_NAME = "saves";

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_on_start = true;
static std::filesystem::path local_app_data_folder = "";
static std::filesystem::path saves_folder = "";
static std::filesystem::path mod_folder = "";

void CreateOrLoadModFolder()
{
	// Try to find the mod_data directory.
	std::wstring current_dir = std::filesystem::current_path().wstring();
	std::wstring mod_data_folder = current_dir + L"\\mod_data";
	if (!std::filesystem::exists(mod_data_folder))
	{
		g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"mod_data\" directory was not found. Creating directory: %s", MOD_NAME, VERSION, mod_data_folder.c_str());
		std::filesystem::create_directory(mod_data_folder);
	}

	// Try to find the mod_data/SaveBackup directory.
	std::wstring save_backup_folder = mod_data_folder + L"\\SaveBackup";
	if (!std::filesystem::exists(save_backup_folder))
	{
		g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"SaveBackup\" directory was not found. Creating directory: %s", MOD_NAME, VERSION, save_backup_folder.c_str());
		std::filesystem::create_directory(save_backup_folder);
	}

	mod_folder = save_backup_folder;
}

void GetLocalAppDataFolder()
{
	PWSTR path = nullptr;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &path)))
	{
		local_app_data_folder = path;
		CoTaskMemFree(path);
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Successfully found the LocalAppData folder: %s", MOD_NAME, VERSION, local_app_data_folder.string().c_str());
		return;
	}

	g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to find LocalAppData folder!", MOD_NAME, VERSION);
}

bool CheckFoldersExist()
{
	// LocalAppData
	if (!fs::exists(local_app_data_folder) || !fs::is_directory(local_app_data_folder))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to find the LocalAppData folder!", MOD_NAME, VERSION);
		return false;
	}
		
		
	// LocalAppData/FieldsOfMistria
	std::filesystem::path mistria_folder = local_app_data_folder / FIELDS_OF_MISTRIA_FOLDER_NAME;
	if (!fs::exists(mistria_folder) || !fs::is_directory(mistria_folder))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to find the LocalAppData/FieldsOfMistria folder!", MOD_NAME, VERSION);
		return false;
	}

	// LocalAppData/FieldsOfMistria/saves
	saves_folder = mistria_folder / SAVE_FOLDER_NAME;
	if (!fs::exists(saves_folder) || !fs::is_directory(saves_folder))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to find the LocalAppData/FieldsOfMistria/saves folder!", MOD_NAME, VERSION);
		return false;
	}

	// Mod folder
	if (!fs::exists(mod_folder) || !fs::is_directory(mod_folder))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to find the mod_data/SaveBackup folder!", MOD_NAME, VERSION);
		return false;
	}

	return true;
}

std::string GetCurrentISODateTime()
{
	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(now);

	std::tm utc_tm{};
	gmtime_s(&utc_tm, &t);

	char buffer[30];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H-%M-%SZ", &utc_tm);
	return std::string(buffer);
}

std::chrono::system_clock::time_point ParseISODateTime(const std::string& datetime_str) {
	std::tm tm = {};
	std::istringstream ss(datetime_str);
	ss >> std::get_time(&tm, "%Y-%m-%dT%H-%M-%SZ");
	if (ss.fail()) {
		throw std::runtime_error("Failed to parse datetime: " + datetime_str);
	}
	// Treat as UTC time
	time_t time = _mkgmtime(&tm);
	return std::chrono::system_clock::from_time_t(time);
}

bool endsWith(const std::string& str, const std::string& suffix) {
	return str.size() >= suffix.size() &&
		str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

void BackupSaves()
{
	// Collect all save filepaths.
	std::map<std::filesystem::path, std::filesystem::path> save_filepaths = {};
	for (const auto& entry : fs::directory_iterator(saves_folder))
	{
		if (entry.is_regular_file())
		{
			std::filesystem::path save_file_path = entry.path();
			std::filesystem::path original_filename = entry.path().filename();

			if (endsWith(original_filename.string(), ".sav"))
				save_filepaths[original_filename] = save_file_path;
		}
	}

	if (save_filepaths.size() == 0)
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - No save files found in folder: ", MOD_NAME, VERSION, saves_folder.c_str());
		g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - A save backup ZIP will not be created!", MOD_NAME, VERSION);
		return;
	}

	std::string zip_filename = GetCurrentISODateTime() + ".zip";
	std::filesystem::path zip_filepath = mod_folder / zip_filename;

	// Prepare ZIP archive structure
	mz_zip_archive zip;
	memset(&zip, 0, sizeof(zip));

	// Initialize a new archive for writing
	if (!mz_zip_writer_init_file(&zip, zip_filepath.string().c_str(), 0)) {
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to initialize ZIP file writer!", MOD_NAME, VERSION);
		g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - A save backup ZIP will not be created!", MOD_NAME, VERSION);
		return;
	}

	// Iterate over every save file.
	int save_files_zipped = 0;
	for (const auto& entry : save_filepaths)
	{
		std::string save_filepath = entry.second.string();
		std::string save_filename = entry.first.string();

		// Add file into the ZIP
		if (mz_zip_writer_add_file(&zip, save_filename.c_str(), save_filepath.c_str(), nullptr, 0, MZ_BEST_COMPRESSION))
			save_files_zipped++;
		else
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Failed to ZIP save file: %s", MOD_NAME, VERSION, save_filepath.c_str());

	}

	// Finalize and close the ZIP archive
	if (save_files_zipped > 0)
	{
		if (!mz_zip_writer_finalize_archive(&zip))
			g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to finalize ZIP archive!", MOD_NAME, VERSION);

		mz_zip_writer_end(&zip);
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Successfully backed up (%d) saves in the ZIP file: %s", MOD_NAME, VERSION, save_files_zipped, zip_filepath.string().c_str());
	}
	else
	{
		mz_zip_writer_end(&zip);
		std::remove(zip_filepath.string().c_str());
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - The ZIP file had no save files added. The backup was not created!", MOD_NAME, VERSION);
	}
}

void PruneOldBackups()
{
	std::map<std::chrono::system_clock::time_point, std::string> all_backups_map;

	for (const auto& entry : fs::directory_iterator(mod_folder))
	{
		if (entry.is_regular_file())
		{
			std::filesystem::path filename = entry.path().filename();
			if (filename.string().contains(".zip"))
				all_backups_map[ParseISODateTime(filename.string())] = filename.string();
		}
	}

	while (all_backups_map.size() > 10) {
		auto it = all_backups_map.begin();
		std::filesystem::path zip_filepath = mod_folder / it->second;

		g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Removing old backup: %s", MOD_NAME, VERSION, zip_filepath.string().c_str());
		std::remove(zip_filepath.string().c_str());
		all_backups_map.erase(it);
	}
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
		load_on_start = false;
		CreateOrLoadModFolder();
		GetLocalAppDataFolder();
	}

	if (CheckFoldersExist())
	{
		BackupSaves();
		PruneOldBackups();
	}
	else
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - One or more necessary folders could not be found. Unable create save backups!", MOD_NAME, VERSION);
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

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Plugin started!", MOD_NAME, VERSION);
	return AURIE_SUCCESS;
}
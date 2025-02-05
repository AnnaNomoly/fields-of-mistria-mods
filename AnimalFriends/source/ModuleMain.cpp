#include <iostream>
#include <fstream>
#include <codecvt>
#include <shlobj.h>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

static const char* const VERSION = "1.1.0";
static const char* const FRIENDSHIP_MULTIPLIER_KEY = "friendship_multiplier";
static const char* const AUTO_PET_KEY = "auto_pet";
static const char* const AUTO_FEED_KEY = "auto_feed";
static const char* const PREVENT_FRIENDSHIP_LOSS_KEY = "prevent_friendship_loss";
static const int DEFAULT_FRIENDSHIP_MULTIPLIER = 5;
static const bool DEFAULT_PREVENT_FRIENDSHIP_LOSS = true;
static const bool DEFAULT_AUTO_PET = false;
static const bool DEFAULT_AUTO_FEED = false;

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_on_start = true;
static bool once_per_day = true;
static int friendship_multiplier = DEFAULT_FRIENDSHIP_MULTIPLIER;
static bool prevent_friendship_loss = DEFAULT_PREVENT_FRIENDSHIP_LOSS;
static bool auto_pet = DEFAULT_AUTO_PET;
static bool auto_feed = DEFAULT_AUTO_FEED;

void handle_eptr(std::exception_ptr eptr)
{
	try {
		if (eptr) {
			std::rethrow_exception(eptr);
		}
	}
	catch (const std::exception& e) {
		g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - Error: %s", VERSION, e.what());
	}
}

std::string wstr_to_string(std::wstring wstr)
{
	std::vector<char> buf(wstr.size());
	std::use_facet<std::ctype<wchar_t>>(std::locale{}).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());

	return std::string(buf.data(), buf.size());
}

void LogDefaultConfigValues()
{
	friendship_multiplier = DEFAULT_FRIENDSHIP_MULTIPLIER;
	prevent_friendship_loss = DEFAULT_PREVENT_FRIENDSHIP_LOSS;
	auto_pet = DEFAULT_AUTO_PET;
	auto_feed = DEFAULT_AUTO_FEED;

	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[AnimalFriends %s] - Using DEFAULT \"%s\" value: %d!", VERSION, FRIENDSHIP_MULTIPLIER_KEY, DEFAULT_FRIENDSHIP_MULTIPLIER);
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[AnimalFriends %s] - Using DEFAULT \"%s\" value: %s!", VERSION, PREVENT_FRIENDSHIP_LOSS_KEY, DEFAULT_PREVENT_FRIENDSHIP_LOSS ? "true" : "false");
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[AnimalFriends %s] - Using DEFAULT \"%s\" value: %s!", VERSION, AUTO_PET_KEY, DEFAULT_AUTO_PET ? "true" : "false");
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[AnimalFriends %s] - Using DEFAULT \"%s\" value: %s!", VERSION, AUTO_FEED_KEY, DEFAULT_AUTO_FEED ? "true" : "false");
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

	if (strstr(self->m_Object->m_Name, "obj_ari"))
	{
		if (once_per_day)
		{
			if (auto_pet || auto_feed)
			{
				CScript* gml_Script_get_all_animals = nullptr;
				g_ModuleInterface->GetNamedRoutinePointer(
					"gml_Script_get_all_animals",
					(PVOID*)&gml_Script_get_all_animals
				);

				RValue all_animals;
				gml_Script_get_all_animals->m_Functions->m_ScriptFunction(
					self,
					other,
					all_animals,
					0,
					nullptr
				);

				if (all_animals.m_Kind == VALUE_OBJECT)
				{
					RValue __buffer = all_animals.at("__buffer");
					if (__buffer.m_Kind == VALUE_ARRAY)
					{
						size_t size = 0;
						g_ModuleInterface->GetArraySize(__buffer, size);

						for (size_t i = 0; i < size; i++)
						{
							RValue entry = __buffer[i];
							RValue _true = 1.0;

							if (auto_feed)
							{
								// Get the animal's current eaten flag.
								RValue has_eaten = g_ModuleInterface->CallBuiltin(
									"struct_get", {
										entry, "has_eaten"
									}
								);

								if (has_eaten.m_Kind == VALUE_BOOL && has_eaten.m_Real == 0.0)
								{
									// Set the animal's pet flag to true.
									g_ModuleInterface->CallBuiltin(
										"struct_set", {
											entry, "has_eaten", _true
										}
									);

									// Get the animal's current heart points.
									RValue original_heart_points = g_ModuleInterface->CallBuiltin(
										"struct_get", {
											entry, "heart_points"
										}
									);

									if (original_heart_points.m_Real < 1800.0)
									{
										// Increase the animal's heart points.
										RValue new_heart_points = original_heart_points.m_Real + (5 * friendship_multiplier);
										if (new_heart_points.m_Real > 1800.0)
											new_heart_points.m_Real = 1800.0;

										g_ModuleInterface->CallBuiltin(
											"struct_set", {
												entry, "heart_points", new_heart_points
											}
										);

										g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - AUTO FEEDER fed an animal, and boosted it's heart points from %d to %d!", VERSION, static_cast<int>(original_heart_points.m_Real), static_cast<int>(new_heart_points.m_Real));
									}
									else
									{
										g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - AUTO FEEDER fed an animal, but it's already at MAX heart points!", VERSION);
									}
								}
							}

							if (auto_pet)
							{
								// Get the animal's current pet flag.
								RValue has_been_pat = g_ModuleInterface->CallBuiltin(
									"struct_get", {
										entry, "has_been_pat"
									}
								);

								if (has_been_pat.m_Kind == VALUE_BOOL && has_been_pat.m_Real == 0.0)
								{
									// Set the animal's pet flag to true.
									g_ModuleInterface->CallBuiltin(
										"struct_set", {
											entry, "has_been_pat", _true
										}
									);

									// Get the animal's current heart points.
									RValue original_heart_points = g_ModuleInterface->CallBuiltin(
										"struct_get", {
											entry, "heart_points"
										}
									);

									if (original_heart_points.m_Real < 1800.0)
									{
										// Increase the animal's heart points.
										RValue new_heart_points = original_heart_points.m_Real + (5 * friendship_multiplier);
										if (new_heart_points.m_Real > 1800.0)
											new_heart_points.m_Real = 1800.0;

										g_ModuleInterface->CallBuiltin(
											"struct_set", {
												entry, "heart_points", new_heart_points
											}
										);

										g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - AUTO PETTER pet an animal, and boosted it's heart points from %d to %d!", VERSION, static_cast<int>(original_heart_points.m_Real), static_cast<int>(new_heart_points.m_Real));
									}
									else
									{
										g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - AUTO PETTER pet an animal, but it's already at MAX heart points!", VERSION);
									}
								}
							}
						}
					}
				}
			}

			once_per_day = false;
		}
	}
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
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Prevented an animal's heart points from being reduced!", VERSION);
	}

	int modified_heart_points = std::round(Arguments[0]->m_Real * friendship_multiplier);
	Arguments[0]->m_Real = static_cast<double>(modified_heart_points);
	if (modified_heart_points > 0)
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Boosted an animal's heart points from %d to %d!", VERSION, original_heart_points, modified_heart_points);
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
	once_per_day = true;

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
					// Try to find the AppData\Local\FieldsOfMistria\mod_data directory.
					std::string mod_data_folder = fields_of_mistria_folder + "\\mod_data";
					if (!std::filesystem::exists(mod_data_folder))
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[AnimalFriends %s] - The \"mod_data\" directory was not found. Creating directory: %s", VERSION, mod_data_folder.c_str());
						std::filesystem::create_directory(mod_data_folder);
					}

					// Try to find the AppData\Local\FieldsOfMistria\mod_data\AnimalFriends directory.
					std::string animal_friends_folder = mod_data_folder + "\\AnimalFriends";
					if (!std::filesystem::exists(animal_friends_folder))
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[AnimalFriends %s] - The \"AnimalFriends\" directory was not found. Creating directory: %s", VERSION, animal_friends_folder.c_str());
						std::filesystem::create_directory(animal_friends_folder);
					}

					// Try to find the AppData\Local\FieldsOfMistria\mod_data\AnimalFriends\AnimalFriends.json config file.
					std::string config_file = animal_friends_folder + "\\" + "AnimalFriends.json";
					std::ifstream in_stream(config_file);
					if (in_stream.good())
					{
						try
						{
							json json_object = json::parse(in_stream);

							// Check if the json_object is empty.
							if (json_object.empty())
							{
								g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - No values found in mod configuration file: %s!", VERSION, config_file.c_str());
								g_ModuleInterface->Print(CM_LIGHTYELLOW, "[AnimalFriends %s] - Add your desired values to the configuration file, otherwise defaults will be used.", VERSION);
								LogDefaultConfigValues();
							}
							else
							{
								// Try loading the friendship_multiplier value.
								if (json_object.contains(FRIENDSHIP_MULTIPLIER_KEY))
								{
									friendship_multiplier = json_object[FRIENDSHIP_MULTIPLIER_KEY];
									if (friendship_multiplier <= 0 || friendship_multiplier > 100)
									{
										g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - Invalid \"%s\" value (%d) in mod configuration file: %s", VERSION, FRIENDSHIP_MULTIPLIER_KEY, friendship_multiplier, config_file.c_str());
										g_ModuleInterface->Print(CM_LIGHTYELLOW, "[AnimalFriends %s] - Make sure the value is a valid integer between 1 and 100 (inclusive)!", VERSION);
										g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Using DEFAULT \"%s\" value: %d!", VERSION, FRIENDSHIP_MULTIPLIER_KEY, DEFAULT_FRIENDSHIP_MULTIPLIER);
										friendship_multiplier = DEFAULT_FRIENDSHIP_MULTIPLIER;
									}
									else
									{
										g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Using CUSTOM \"%s\" value: %d!", VERSION, FRIENDSHIP_MULTIPLIER_KEY, friendship_multiplier);
									}
								}
								else
								{
									g_ModuleInterface->Print(CM_LIGHTYELLOW, "[AnimalFriends %s] - Missing \"%s\" value in mod configuration file: %s!", VERSION, FRIENDSHIP_MULTIPLIER_KEY, config_file.c_str());
									g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Using DEFAULT \"%s\" value: %d!", VERSION, FRIENDSHIP_MULTIPLIER_KEY, DEFAULT_FRIENDSHIP_MULTIPLIER);
								}

								// Try loading the prevent_friendship_loss value.
								if (json_object.contains(PREVENT_FRIENDSHIP_LOSS_KEY))
								{
									prevent_friendship_loss = json_object[PREVENT_FRIENDSHIP_LOSS_KEY];
									g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Using CUSTOM \"%s\" value: %s!", VERSION, PREVENT_FRIENDSHIP_LOSS_KEY, prevent_friendship_loss ? "true" : "false");
								}
								else
								{
									g_ModuleInterface->Print(CM_LIGHTYELLOW, "[AnimalFriends %s] - Missing \"%s\" value in mod configuration file: %s!", VERSION, PREVENT_FRIENDSHIP_LOSS_KEY, config_file.c_str());
									g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Using DEFAULT \"%s\" value: %s!", VERSION, PREVENT_FRIENDSHIP_LOSS_KEY, DEFAULT_PREVENT_FRIENDSHIP_LOSS ? "true" : "false");
								}

								// Try loading the auto_pet value.
								if (json_object.contains(AUTO_PET_KEY))
								{
									auto_pet = json_object[AUTO_PET_KEY];
									g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Using CUSTOM \"%s\" value: %s!", VERSION, AUTO_PET_KEY, auto_pet ? "true" : "false");
								}
								else
								{
									g_ModuleInterface->Print(CM_LIGHTYELLOW, "[AnimalFriends %s] - Missing \"%s\" value in mod configuration file: %s!", VERSION, AUTO_PET_KEY, config_file.c_str());
									g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Using DEFAULT \"%s\" value: %s!", VERSION, AUTO_PET_KEY, DEFAULT_AUTO_PET ? "true" : "false");
								}

								// Try loading the auto_feed value.
								if (json_object.contains(AUTO_FEED_KEY))
								{
									auto_feed = json_object[AUTO_FEED_KEY];
									g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Using CUSTOM \"%s\" value: %s!", VERSION, AUTO_FEED_KEY, auto_feed ? "true" : "false");
								}
								else
								{
									g_ModuleInterface->Print(CM_LIGHTYELLOW, "[AnimalFriends %s] - Missing \"%s\" value in mod configuration file: %s!", VERSION, AUTO_FEED_KEY, config_file.c_str());
									g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Using DEFAULT \"%s\" value: %s!", VERSION, AUTO_FEED_KEY, DEFAULT_AUTO_FEED ? "true" : "false");
								}
							}
						}
						catch (...)
						{
							eptr = std::current_exception();
							handle_eptr(eptr);

							g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - Failed to parse JSON from configuration file: %s", VERSION, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[AnimalFriends %s] - Make sure the file is valid JSON!", VERSION);
							LogDefaultConfigValues();
						}

						in_stream.close();
					}
					else
					{
						in_stream.close();

						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[AnimalFriends %s] - The \"AnimalFriends.json\" file was not found. Creating file: %s", VERSION, config_file.c_str());
						json default_json = {
							{FRIENDSHIP_MULTIPLIER_KEY, DEFAULT_FRIENDSHIP_MULTIPLIER},
							{PREVENT_FRIENDSHIP_LOSS_KEY, DEFAULT_PREVENT_FRIENDSHIP_LOSS},
							{AUTO_PET_KEY, DEFAULT_AUTO_PET},
							{AUTO_FEED_KEY, DEFAULT_AUTO_FEED}
						};
						
						std::ofstream out_stream(config_file);
						out_stream << std::setw(4) << default_json << std::endl;
						out_stream.close();

						LogDefaultConfigValues();
					}
				}
				else
				{
					g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - Failed to find the \"%s\" directory.", VERSION, "AppData\\Local\\FieldsOfMistria");
					LogDefaultConfigValues();
				}
			}
			else
			{
				g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - Failed to find the \"%s\" directory.", VERSION, "AppData\\Local");
				LogDefaultConfigValues();
			}
		}
		catch (...)
		{
			eptr = std::current_exception();
			handle_eptr(eptr);

			g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - An error occurred loading the mod configuration file.", VERSION);
			LogDefaultConfigValues();
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

RValue& GmlScriptOnNewDayCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	once_per_day = true;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_on_new_day@Ari@Ari"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

void CreateHookEventObject(AurieStatus& status)
{
	status = g_ModuleInterface->CreateCallback(
		g_ArSelfModule,
		EVENT_OBJECT_CALL,
		ObjectCallback,
		0
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - Failed to hook EVENT_OBJECT_CALL!", VERSION);
	}
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

void CreateHookGmlScriptOnNewDay(AurieStatus& status)
{
	CScript* gml_script_on_new_day = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_on_new_day@Ari@Ari",
		(PVOID*)&gml_script_on_new_day
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - Failed to get script (gml_Script_on_new_day@Ari@Ari)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_on_new_day@Ari@Ari",
		gml_script_on_new_day->m_Functions->m_ScriptFunction,
		GmlScriptOnNewDayCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - Failed to hook script (gml_Script_on_new_day@Ari@Ari)!", VERSION);
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

	CreateHookEventObject(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

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

	CreateHookGmlScriptOnNewDay(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[AnimalFriends %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[AnimalFriends %s] - Plugin started!", VERSION);
	return AURIE_SUCCESS;
}
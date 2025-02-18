#include <random>
#include <fstream>
#include <codecvt>
#include <iostream>
#include <shlobj.h>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

static const char* const VERSION = "1.1.0";
static const char* const SPELL_COST_KEY = "spell_cost";
static const char* const DAMAGE_ARI_KEY = "damage_ari";
static const char* const IGNORE_MIMICS_KEY = "ignore_mimics";
static const char* const SOUND_EFFECTS_KEY = "sound_effects";
static const char* const SCREEN_FLASH_KEY = "screen_flash";
static const int DEFAULT_SPELL_COST_VALUE = 2;
static const bool DEFAULT_DAMAGE_ARI_VALUE = true;
static const bool DEFAULT_IGNORE_MIMICS_VALUE = false;
static const bool DEFAULT_SOUND_EFFECTS_VALUE = true;
static const bool DEFAULT_SCREEN_FLASH_VALUE = true;
static const int FULL_RESTORE_SPELL_ID = 0;
static const int GROWTH_SPELL_ID = 1;
static const int SUMMON_RAIN_SPELL_ID = 2;

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_on_start = true;
static int spell_cost = DEFAULT_SPELL_COST_VALUE;
static bool damage_ari_option = DEFAULT_DAMAGE_ARI_VALUE;
static bool ignore_mimics_option = DEFAULT_IGNORE_MIMICS_VALUE;
static bool sound_effects_option = DEFAULT_SOUND_EFFECTS_VALUE;
static bool screen_flash_option = DEFAULT_SCREEN_FLASH_VALUE;

static double ari_current_mana = -1.0;
static int pinned_spell = -1;
static int quake_magnitude = 0;
static int flash_frames = 0;
static int rumble_frames = 0;
static bool screen_flash = false;
static bool rumble_sound = false;
static bool quake_spell_active = false;
static bool room_loaded = false;
static bool modify_mana = false;
static bool modify_health = false;
static bool in_dungeon = false;
static std::mt19937 generator(std::random_device{}());

void handle_eptr(std::exception_ptr eptr)
{
	try {
		if (eptr) {
			std::rethrow_exception(eptr);
		}
	}
	catch (const std::exception& e) {
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Error: %s", VERSION, e.what());
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
	spell_cost = DEFAULT_SPELL_COST_VALUE;
	damage_ari_option = DEFAULT_DAMAGE_ARI_VALUE;
	ignore_mimics_option = DEFAULT_IGNORE_MIMICS_VALUE;
	sound_effects_option = DEFAULT_SOUND_EFFECTS_VALUE;
	screen_flash_option = DEFAULT_SCREEN_FLASH_VALUE;

	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[QuakeSpell %s] - Using DEFAULT \"%s\" value: %d!", VERSION, SPELL_COST_KEY, DEFAULT_SPELL_COST_VALUE);
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[QuakeSpell %s] - Using DEFAULT \"%s\" value: %s!", VERSION, DAMAGE_ARI_KEY, DEFAULT_DAMAGE_ARI_VALUE ? "true" : "false");
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[QuakeSpell %s] - Using DEFAULT \"%s\" value: %s!", VERSION, IGNORE_MIMICS_KEY, DEFAULT_IGNORE_MIMICS_VALUE ? "true" : "false");
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[QuakeSpell %s] - Using DEFAULT \"%s\" value: %s!", VERSION, SOUND_EFFECTS_KEY, DEFAULT_SOUND_EFFECTS_VALUE ? "true" : "false");
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[QuakeSpell %s] - Using DEFAULT \"%s\" value: %s!", VERSION, SCREEN_FLASH_KEY, DEFAULT_SCREEN_FLASH_VALUE ? "true" : "false");
}

void ResetStaticFields(bool returnedToTitleScreen)
{
	if (returnedToTitleScreen)
	{
		ari_current_mana = -1.0;
		pinned_spell = -1;
	}

	quake_magnitude = 0;
	flash_frames = 0;
	rumble_frames = 0;
	screen_flash = false;
	rumble_sound = false;
	quake_spell_active = false;
	room_loaded = false;
	modify_mana = false;
	modify_health = false;
	in_dungeon = false;
}

void ModifyHealth(CInstance* Self, CInstance* Other, int value)
{
	CScript* gml_script_modify_health = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_modify_health@Ari@Ari",
		(PVOID*)&gml_script_modify_health
	);

	RValue result;
	RValue health_modifier = value;
	RValue* health_modifier_ptr = &health_modifier;

	gml_script_modify_health->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &health_modifier_ptr }
	);
}

void PlaySoundEffect(const char* sound_name, int priority)
{
	const auto sound_index = g_ModuleInterface->CallBuiltin(
		"asset_get_index",
		{ sound_name }
	);

	g_ModuleInterface->CallBuiltin(
		"audio_play_sound",
		{ sound_index, priority, false }
	);
}

void FrameCallback(FWFrame& FrameContext)
{
	UNREFERENCED_PARAMETER(FrameContext);

	if (screen_flash)
	{
		if (flash_frames > 3) // 10, 6
		{
			if (flash_frames == 4)
			{
				PlaySoundEffect("snd_AriLowHealthWarning", 100);
			}
			screen_flash = false;
			flash_frames = 0;
		}
		else {
			flash_frames++;
		}
	}

	if (rumble_sound)
	{
		if (rumble_frames > 120)
		{
			rumble_sound = false;
			rumble_frames = 0;
		}
		else {
			if(rumble_frames % 15 == 0) // 30
				PlaySoundEffect("snd_EarthbreakerRockBreak1", 100);
			else if(rumble_frames % 10 == 0) // 20
				PlaySoundEffect("snd_EarthbreakerRockBreak2", 100);
			else if(rumble_frames % 5 == 0) // 10
				PlaySoundEffect("snd_EarthbreakerRockBreak3", 100);
			rumble_frames++;
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

	if (quake_spell_active && strstr(self->m_Object->m_Name, "obj_monster"))
	{
		if(strstr(self->m_Object->m_Name, "obj_monster_mimic"))
		{
			if (!ignore_mimics_option)
			{
				RValue hit_points = self->at("hit_points");
				if (hit_points.m_Kind != VALUE_UNSET && hit_points.m_Kind != VALUE_UNDEFINED)
				{
					self->at("hit_points").m_Real = 0;
				}
			}
		}
		else
		{
			RValue hit_points = self->at("hit_points");
			if (hit_points.m_Kind != VALUE_UNSET && hit_points.m_Kind != VALUE_UNDEFINED)
			{
				self->at("hit_points").m_Real = 0;
			}
		}
	}

	if (modify_health && strstr(self->m_Object->m_Name, "obj_ari"))
	{
		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		ModifyHealth(global_instance->at("__ari").m_Object, self, quake_magnitude * 10 * -1);
		modify_health = false;

		g_ModuleInterface->Print(CM_LIGHTGREEN, "[QuakeSpell %s] - Ari took %d damage from the quake!", VERSION, quake_magnitude * 10);
	}
}

RValue& GmlScriptSetPinnedSpell(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if(Arguments[0]->m_Kind == VALUE_INT64)
		pinned_spell = Arguments[0]->m_i64;
	if (Arguments[0]->m_Kind == VALUE_REAL)
		pinned_spell = Arguments[0]->m_Real;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_set_pinned_spell@Ari@Ari"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptCanCastSpell(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_can_cast_spell"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (pinned_spell == FULL_RESTORE_SPELL_ID)
	{
		if (in_dungeon && !quake_spell_active && ari_current_mana >= spell_cost)
		{
			Result.m_Real = 1.0;
		}
		else {
			Result.m_Real = 0.0;
		}
	}

	return Result;
}

RValue& GmlScriptCastSpell(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (pinned_spell == FULL_RESTORE_SPELL_ID)
	{
		std::uniform_int_distribution<int> distribution(5, 9); // TODO: Decide if this range should be configurable.
		quake_magnitude = distribution(generator);

		if(damage_ari_option)
			modify_health = true;
		if(screen_flash_option)
			screen_flash = true;
		if(sound_effects_option)
			rumble_sound = true;

		modify_mana = true;
		quake_spell_active = true;
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[QuakeSpell %s] - Quake spell cast! Magnitude: %d", VERSION, quake_magnitude);
		return Result;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_cast_spell"));
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
					// Try to find the AppData\Local\FieldsOfMistria\mod_data directory.
					std::string mod_data_folder = fields_of_mistria_folder + "\\mod_data";
					if (!std::filesystem::exists(mod_data_folder))
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[QuakeSpell %s] - The \"mod_data\" directory was not found. Creating directory: %s", VERSION, mod_data_folder.c_str());
						std::filesystem::create_directory(mod_data_folder);
					}

					// Try to find the AppData\Local\FieldsOfMistria\mod_data\QuakeSpell directory.
					std::string quake_spell_folder = mod_data_folder + "\\QuakeSpell";
					if (!std::filesystem::exists(quake_spell_folder))
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[QuakeSpell %s] - The \"QuakeSpell\" directory was not found. Creating directory: %s", VERSION, quake_spell_folder.c_str());
						std::filesystem::create_directory(quake_spell_folder);
					}

					// Try to find the AppData\Local\FieldsOfMistria\mod_data\QuakeSpell\QuakeSpell.json config file.
					std::string config_file = quake_spell_folder + "\\" + "QuakeSpell.json";
					std::ifstream in_stream(config_file);
					if (in_stream.good())
					{
						try
						{
							json json_object = json::parse(in_stream);

							// Check if the json_object is empty.
							if (json_object.empty())
							{
								g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - No values found in mod configuration file: %s!", VERSION, config_file.c_str());
								g_ModuleInterface->Print(CM_LIGHTYELLOW, "[QuakeSpell %s] - Add your desired values to the configuration file, otherwise defaults will be used.", VERSION);
								LogDefaultConfigValues();
							}
							else
							{
								// Try loading the spell_cost value.
								if (json_object.contains(SPELL_COST_KEY))
								{
									spell_cost = json_object[SPELL_COST_KEY];
									if (spell_cost < 0 || spell_cost > 12)
									{
										g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Invalid \"%s\" value (%d) in mod configuration file: %s", VERSION, SPELL_COST_KEY, spell_cost, config_file.c_str());
										g_ModuleInterface->Print(CM_LIGHTYELLOW, "[QuakeSpell %s] - This value MUST be a valid integer between 0 and 12 (inclusive)!", VERSION);
										g_ModuleInterface->Print(CM_LIGHTGREEN, "[QuakeSpell %s] - Using DEFAULT \"%s\" value: %d!", VERSION, SPELL_COST_KEY, DEFAULT_SPELL_COST_VALUE);
										spell_cost = DEFAULT_SPELL_COST_VALUE;
									}
									else
									{
										g_ModuleInterface->Print(CM_LIGHTGREEN, "[QuakeSpell %s] - Using CUSTOM \"%s\" value: %d!", VERSION, SPELL_COST_KEY, spell_cost);
									}
								}
								else
								{
									g_ModuleInterface->Print(CM_LIGHTYELLOW, "[QuakeSpell %s] - Missing \"%s\" value in mod configuration file: %s!", VERSION, SPELL_COST_KEY, config_file.c_str());
									g_ModuleInterface->Print(CM_LIGHTGREEN, "[QuakeSpell %s] - Using DEFAULT \"%s\" value: %d!", VERSION, SPELL_COST_KEY, DEFAULT_SPELL_COST_VALUE);
									spell_cost = DEFAULT_SPELL_COST_VALUE;
								}

								// Try loading the damage_ari value.
								if (json_object.contains(DAMAGE_ARI_KEY))
								{
									damage_ari_option = json_object[DAMAGE_ARI_KEY];
									g_ModuleInterface->Print(CM_LIGHTGREEN, "[QuakeSpell %s] - Using CUSTOM \"%s\" value: %s!", VERSION, DAMAGE_ARI_KEY, damage_ari_option ? "true" : "false");
								}
								else
								{
									g_ModuleInterface->Print(CM_LIGHTYELLOW, "[QuakeSpell %s] - Missing \"%s\" value in mod configuration file: %s!", VERSION, DAMAGE_ARI_KEY, config_file.c_str());
									g_ModuleInterface->Print(CM_LIGHTGREEN, "[QuakeSpell %s] - Using DEFAULT \"%s\" value: %s!", VERSION, DAMAGE_ARI_KEY, DEFAULT_DAMAGE_ARI_VALUE ? "true" : "false");
									damage_ari_option = DEFAULT_DAMAGE_ARI_VALUE;
								}

								// Try loading the ignore_mimics value.
								if (json_object.contains(IGNORE_MIMICS_KEY))
								{
									ignore_mimics_option = json_object[IGNORE_MIMICS_KEY];
									g_ModuleInterface->Print(CM_LIGHTGREEN, "[QuakeSpell %s] - Using CUSTOM \"%s\" value: %s!", VERSION, IGNORE_MIMICS_KEY, ignore_mimics_option ? "true" : "false");
								}
								else
								{
									g_ModuleInterface->Print(CM_LIGHTYELLOW, "[QuakeSpell %s] - Missing \"%s\" value in mod configuration file: %s!", VERSION, IGNORE_MIMICS_KEY, config_file.c_str());
									g_ModuleInterface->Print(CM_LIGHTGREEN, "[QuakeSpell %s] - Using DEFAULT \"%s\" value: %s!", VERSION, IGNORE_MIMICS_KEY, DEFAULT_IGNORE_MIMICS_VALUE ? "true" : "false");
									ignore_mimics_option = DEFAULT_IGNORE_MIMICS_VALUE;
								}

								// Try loading the sound_effects value.
								if (json_object.contains(SOUND_EFFECTS_KEY))
								{
									sound_effects_option = json_object[SOUND_EFFECTS_KEY];
									g_ModuleInterface->Print(CM_LIGHTGREEN, "[QuakeSpell %s] - Using CUSTOM \"%s\" value: %s!", VERSION, SOUND_EFFECTS_KEY, sound_effects_option ? "true" : "false");
								}
								else
								{
									g_ModuleInterface->Print(CM_LIGHTYELLOW, "[QuakeSpell %s] - Missing \"%s\" value in mod configuration file: %s!", VERSION, SOUND_EFFECTS_KEY, config_file.c_str());
									g_ModuleInterface->Print(CM_LIGHTGREEN, "[QuakeSpell %s] - Using DEFAULT \"%s\" value: %s!", VERSION, SOUND_EFFECTS_KEY, DEFAULT_SOUND_EFFECTS_VALUE ? "true" : "false");
									sound_effects_option = DEFAULT_SOUND_EFFECTS_VALUE;
								}

								// Try loading the screen_flash value.
								if (json_object.contains(SCREEN_FLASH_KEY))
								{
									screen_flash_option = json_object[SCREEN_FLASH_KEY];
									g_ModuleInterface->Print(CM_LIGHTGREEN, "[QuakeSpell %s] - Using CUSTOM \"%s\" value: %s!", VERSION, SCREEN_FLASH_KEY, screen_flash_option ? "true" : "false");
								}
								else
								{
									g_ModuleInterface->Print(CM_LIGHTYELLOW, "[QuakeSpell %s] - Missing \"%s\" value in mod configuration file: %s!", VERSION, SCREEN_FLASH_KEY, config_file.c_str());
									g_ModuleInterface->Print(CM_LIGHTGREEN, "[QuakeSpell %s] - Using DEFAULT \"%s\" value: %s!", VERSION, SCREEN_FLASH_KEY, DEFAULT_SCREEN_FLASH_VALUE ? "true" : "false");
									screen_flash_option = DEFAULT_SCREEN_FLASH_VALUE;
								}
							}
						}
						catch (...)
						{
							eptr = std::current_exception();
							handle_eptr(eptr);

							g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to parse JSON from configuration file: %s", VERSION, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[QuakeSpell %s] - Make sure the file is valid JSON!", VERSION);
							LogDefaultConfigValues();
						}

						in_stream.close();
					}
					else
					{
						in_stream.close();

						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[QuakeSpell %s] - The \"QuakeSpell.json\" file was not found. Creating file: %s", VERSION, config_file.c_str());
						json default_json = {
							{SPELL_COST_KEY, DEFAULT_SPELL_COST_VALUE},
							{DAMAGE_ARI_KEY, DEFAULT_DAMAGE_ARI_VALUE},
							{IGNORE_MIMICS_KEY, DEFAULT_IGNORE_MIMICS_VALUE},
							{SOUND_EFFECTS_KEY, DEFAULT_SOUND_EFFECTS_VALUE},
							{SCREEN_FLASH_KEY, DEFAULT_SCREEN_FLASH_VALUE}
						};

						std::ofstream out_stream(config_file);
						out_stream << std::setw(4) << default_json << std::endl;
						out_stream.close();

						LogDefaultConfigValues();
					}
				}
				else
				{
					g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to find the \"%s\" directory.", VERSION, "AppData\\Local\\FieldsOfMistria");
					LogDefaultConfigValues();
				}
			}
			else
			{
				g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to find the \"%s\" directory.", VERSION, "AppData\\Local");
				LogDefaultConfigValues();
			}
		}
		catch (...)
		{
			eptr = std::current_exception();
			handle_eptr(eptr);

			g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - An error occurred loading the mod configuration file.", VERSION);
			LogDefaultConfigValues();
		}

		load_on_start = false;
	}

	ResetStaticFields(true);

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

RValue& GmlScriptNewDayCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	ResetStaticFields(false);

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_try_end_of_day_status_to_string"));
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
	room_loaded = true;
	
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


RValue& GmlScriptGetManaCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_get_mana@Ari@Ari"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (room_loaded && Result.m_Kind != VALUE_UNSET)
	{
		ari_current_mana = Result.m_Real;
	}

	return Result;
}

RValue& GmlScriptModifyManaCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (modify_mana)
	{
		Arguments[0]->m_Real = (spell_cost * -1.0);
		modify_mana = false;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_modify_mana@Ari@Ari"));
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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_on_draw_gui@Display@Display"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (screen_flash)
	{
		RValue window_width = g_ModuleInterface->CallBuiltin("window_get_width", {});
		RValue window_height = g_ModuleInterface->CallBuiltin("window_get_height", {});

		g_ModuleInterface->CallBuiltin(
			"draw_set_color", {
				255  // c_white(16777215)
			}
		);

		g_ModuleInterface->CallBuiltin(
			"draw_rectangle", {
				0, 0, static_cast<int>(window_width.m_Real), static_cast<int>(window_height.m_Real), false
			}
		);
	}

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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_try_location_id_to_string"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	std::string location = Result.AsString().data();
	if (location == "dungeon")
		in_dungeon = true;
	else
		in_dungeon = false;

	return Result;
}

RValue& GmlScriptFadeOutCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	ResetStaticFields(false);

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_fade_out@ScreenFaderMenu@ScreenFaderMenu"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

void CreateFrameCallback(AurieModule* Module, AurieStatus& status)
{
	status = g_ModuleInterface->CreateCallback(
		Module,
		EVENT_FRAME,
		FrameCallback,
		0
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to hook (EVENT_FRAME)!", VERSION);
	}
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to hook (EVENT_OBJECT_CALL)!", VERSION);
	}
}

void CreateHookGmlScriptSetPinnedSpell(AurieStatus& status)
{
	CScript* gml_script_set_pinned_spell = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_set_pinned_spell@Ari@Ari",
		(PVOID*)&gml_script_set_pinned_spell
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to get script (gml_Script_set_pinned_spell@Ari@Ari)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_set_pinned_spell@Ari@Ari",
		gml_script_set_pinned_spell->m_Functions->m_ScriptFunction,
		GmlScriptSetPinnedSpell,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to hook script (gml_Script_set_pinned_spell@Ari@Ari)!", VERSION);
	}
}

void CreateHookGmlScriptCanCastSpell(AurieStatus& status)
{
	CScript* gml_script_can_cast_spell = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_can_cast_spell",
		(PVOID*)&gml_script_can_cast_spell
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to get script (gml_Script_can_cast_spell)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_can_cast_spell",
		gml_script_can_cast_spell->m_Functions->m_ScriptFunction,
		GmlScriptCanCastSpell,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to hook script (gml_Script_can_cast_spell)!", VERSION);
	}
}

void CreateHookGmlScriptCastSpell(AurieStatus& status)
{
	CScript* gml_script_cast_spell = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_cast_spell",
		(PVOID*)&gml_script_cast_spell
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to get script (gml_Script_cast_spell)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_cast_spell",
		gml_script_cast_spell->m_Functions->m_ScriptFunction,
		GmlScriptCastSpell,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to hook script (gml_Script_enchantern_state_to_string)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to get script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to hook script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
	}
}

void CreateHookGmlScriptNewDay(AurieStatus& status)
{
	CScript* gml_script_new_day = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_try_end_of_day_status_to_string",
		(PVOID*)&gml_script_new_day
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to get script (gml_Script_new_day)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_try_end_of_day_status_to_string",
		gml_script_new_day->m_Functions->m_ScriptFunction,
		GmlScriptNewDayCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to hook script (gml_Script_new_day)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to get script (gml_Script_show_room_title)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to hook script (gml_Script_show_room_title)!", VERSION);
	}
}

void CreateHookGmlScriptGetMana(AurieStatus& status)
{
	CScript* gml_script_get_mana = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_get_mana@Ari@Ari",
		(PVOID*)&gml_script_get_mana
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to get script (gml_Script_get_mana@Ari@Ari)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_get_mana@Ari@Ari",
		gml_script_get_mana->m_Functions->m_ScriptFunction,
		GmlScriptGetManaCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to hook script (gml_Script_get_mana@Ari@Ari)!", VERSION);
	}
}

void CreateHookGmlScriptModifyMana(AurieStatus& status)
{
	CScript* gml_script_get_mana = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_modify_mana@Ari@Ari",
		(PVOID*)&gml_script_get_mana
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to get script (gml_Script_modify_mana@Ari@Ari)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_modify_mana@Ari@Ari",
		gml_script_get_mana->m_Functions->m_ScriptFunction,
		GmlScriptModifyManaCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to hook script (gml_Script_modify_mana@Ari@Ari)!", VERSION);
	}
}

void CreateHookGmlScriptOnDrawGui(AurieStatus& status)
{
	CScript* gml_script_on_draw_gui = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_on_draw_gui@Display@Display",
		(PVOID*)&gml_script_on_draw_gui
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to get script (gml_Script_on_draw_gui@Display@Display)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_on_draw_gui@Display@Display",
		gml_script_on_draw_gui->m_Functions->m_ScriptFunction,
		GmlScriptOnDrawGuiCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to hook script (gml_Script_on_draw_gui@Display@Display)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to get script (gml_Script_try_location_id_to_string)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to hook script (gml_Script_try_location_id_to_string)!", VERSION);
	}
}

void CreateHookGmlScriptFadeOut(AurieStatus& status)
{
	CScript* gml_script_try_location_id_to_string = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_fade_out@ScreenFaderMenu@ScreenFaderMenu",
		(PVOID*)&gml_script_try_location_id_to_string
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to get script (gml_Script_fade_out@ScreenFaderMenu@ScreenFaderMenu)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_fade_out@ScreenFaderMenu@ScreenFaderMenu",
		gml_script_try_location_id_to_string->m_Functions->m_ScriptFunction,
		GmlScriptFadeOutCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Failed to hook script (gml_Script_fade_out@ScreenFaderMenu@ScreenFaderMenu)!", VERSION);
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

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[QuakeSpell %s] - Plugin starting...", VERSION);

	CreateFrameCallback(Module, status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateObjectCallback(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptSetPinnedSpell(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptCanCastSpell(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptCastSpell(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptNewDay(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptShowRoomTitle(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptGetMana(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptModifyMana(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptOnDrawGui(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptTryLocationIdToString(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptFadeOut(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[QuakeSpell %s] - Plugin started!", VERSION);
	return AURIE_SUCCESS;
}
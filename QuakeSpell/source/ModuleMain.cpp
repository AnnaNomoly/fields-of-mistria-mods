#include <random>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static const int FULL_RESTORE_SPELL_ID = 0;
static const int GROWTH_SPELL_ID = 1;
static const int SUMMON_RAIN_SPELL_ID = 2;
static const double QUAKE_SPELL_COST = 2.0;

static YYTKInterface* g_ModuleInterface = nullptr;
static double ari_current_mana = -1.0;
static int pinned_spell = -1;
static int flash_frames = 0;
static int rumble_frames = 0;
static int quake_magnitude = 0;
static int monster_kill_counter = 0;
static bool quake_spell_active = false;
static bool room_loaded = false;
static bool modify_mana = false;
static bool modify_health = false;
static bool in_dungeon = false;
static bool screen_flash = false;
static bool rumble_sound = false;


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
			quake_spell_active = false;
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
		if (monster_kill_counter < quake_magnitude) {
			RValue hit_points = self->at("hit_points");
			if (hit_points.m_Kind != VALUE_UNSET && hit_points.m_Kind != VALUE_UNDEFINED)
			{
				self->at("hit_points").m_Real = 0;
			}
			monster_kill_counter++;
		}
	}

	if (modify_health && strstr(self->m_Object->m_Name, "obj_ari"))
	{
		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		CScript* gml_script_modify_health = nullptr;
		g_ModuleInterface->GetNamedRoutinePointer(
			"gml_Script_modify_health@Ari@Ari",
			(PVOID*)&gml_script_modify_health
		);

		RValue result;
		RValue* health_penalty = new RValue(quake_magnitude * 10 * -1);

		gml_script_modify_health->m_Functions->m_ScriptFunction(
			global_instance->at("__ari").m_Object,
			self,
			result,
			1,
			{ &health_penalty }
		);

		delete health_penalty;
		modify_health = false;

		g_ModuleInterface->Print(CM_LIGHTGREEN, "[QuakeSpell] - Ari took %d damage from the quake!", quake_magnitude * 10);
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
		if (in_dungeon && !quake_spell_active && ari_current_mana >= QUAKE_SPELL_COST)
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
		// Generate a random int. I hate C++.
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(5, 9);
		quake_magnitude = distr(gen);

		modify_mana = true;
		modify_health = true;
		screen_flash = true;
		rumble_sound = true;
		quake_spell_active = true;
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[QuakeSpell] - Quake spell cast! Magnitude: %d", quake_magnitude);
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
	ari_current_mana = -1.0;
	pinned_spell = -1;
	quake_magnitude = 0;
	monster_kill_counter = 0;
	flash_frames = 0;
	rumble_frames = 0;
	screen_flash = false;
	rumble_sound = false;
	quake_spell_active = false;
	room_loaded = false;
	modify_mana = false;
	modify_health = false;
	in_dungeon = false;

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
	quake_magnitude = 0;
	monster_kill_counter = 0;
	flash_frames = 0;
	rumble_frames = 0;
	screen_flash = false;
	rumble_sound = false;
	quake_spell_active = false;
	room_loaded = false;
	modify_mana = false;
	modify_health = false;
	in_dungeon = false;

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
		Arguments[0]->m_Real = (QUAKE_SPELL_COST * -1.0);
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
	quake_magnitude = 0;
	monster_kill_counter = 0;
	flash_frames = 0;
	rumble_frames = 0;
	screen_flash = false;
	rumble_sound = false;
	quake_spell_active = false;
	room_loaded = false;
	modify_mana = false;
	modify_health = false;
	in_dungeon = false;

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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to hook (EVENT_FRAME)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to hook (EVENT_OBJECT_CALL)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to get script (gml_Script_set_pinned_spell@Ari@Ari)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to hook script (gml_Script_set_pinned_spell@Ari@Ari)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to get script (gml_Script_can_cast_spell)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to hook script (gml_Script_can_cast_spell)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to get script (gml_Script_cast_spell)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to hook script (gml_Script_enchantern_state_to_string)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to get script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to hook script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to get script (gml_Script_new_day)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to hook script (gml_Script_new_day)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to get script (gml_Script_show_room_title)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to hook script (gml_Script_show_room_title)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to get script (gml_Script_get_mana@Ari@Ari)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to hook script (gml_Script_get_mana@Ari@Ari)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to get script (gml_Script_modify_mana@Ari@Ari)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to hook script (gml_Script_modify_mana@Ari@Ari)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to get script (gml_Script_on_draw_gui@Display@Display)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to hook script (gml_Script_on_draw_gui@Display@Display)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to get script (gml_Script_try_location_id_to_string)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to hook script (gml_Script_try_location_id_to_string)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to get script (gml_Script_fade_out@ScreenFaderMenu@ScreenFaderMenu)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Failed to hook script (gml_Script_fade_out@ScreenFaderMenu@ScreenFaderMenu)!");
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

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[QuakeSpell] - Plugin starting...");

	CreateFrameCallback(Module, status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Exiting due to failure on start!");
		return status;
	}

	CreateObjectCallback(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptSetPinnedSpell(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptCanCastSpell(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptCastSpell(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptNewDay(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptShowRoomTitle(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptGetMana(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptModifyMana(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptOnDrawGui(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptTryLocationIdToString(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptFadeOut(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[QuakeSpell] - Exiting due to failure on start!");
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[QuakeSpell] - Plugin started!");
	return AURIE_SUCCESS;
}
#include <unordered_set>
#include <YYToolkit/YYTK_Shared.hpp> // YYTK v4
using namespace Aurie;
using namespace YYTK;

static const char* const MOD_NAME = "FauxSigils";
static const char* const VERSION = "1.0.0";
static const char* const GML_SCRIPT_CREATE_NOTIFICATION = "gml_Script_create_notification";
static const char* const GML_SCRIPT_SPAWN_LADDER = "gml_Script_spawn_ladder@DungeonRunner@DungeonRunner";
static const char* const GML_SCRIPT_REGISTER_STATUS_EFFECT = "gml_Script_register@StatusEffectManager@StatusEffectManager";
static const char* const GML_SCRIPT_STATUS_EFFECT_MANAGER_UPDATE = "gml_Script_update@StatusEffectManager@StatusEffectManager";
static const char* const GML_SCRIPT_SPAWN_MONSTER = "gml_Script_spawn_monster";
static const char* const GML_SCRIPT_UPDATE_TOOLBAR_MENU = "gml_Script_update@ToolbarMenu@ToolbarMenu";
static const char* const GML_SCRIPT_GET_ITEM_UI_ICON = "gml_Script_get_ui_icon@anon@4053@LiveItem@LiveItem";
static const char* const GML_SCRIPT_DAMAGE = "gml_Script_damage@gml_Object_obj_damage_receiver_Create_0";
static const char* const GML_SCRIPT_USE_ITEM = "gml_Script_use_item";
static const char* const GML_SCRIPT_HELD_ITEM = "gml_Script_held_item@Ari@Ari";
static const char* const GML_SCRIPT_GET_WEATHER = "gml_Script_get_weather@WeatherManager@Weather";
static const char* const GML_SCRIPT_GO_TO_ROOM = "gml_Script_goto_gm_room";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";

static const std::string SIGIL_RESTRICTED_NOTIFICATION_KEY = "Notifications/Mods/Faux Sigils/sigil_restricted";
static const std::string SIGIL_LIMIT_NOTIFICATION_KEY = "Notifications/Mods/Faux Sigils/sigil_limit";
static const std::string CONCEALMENT_LOST_NOTIFICATION_KEY = "Notifications/Mods/Faux Sigils/Sigils/concealment/deactivated";

static enum class Sigils {
	CONCEALMENT,
	FORTIFICATION,
	FORTUNE,
	PROTECTION,
	RAGE,
	REDEMPTION,
	SILENCE,
	STRENGTH
};

static const std::string FAUX_SIGIL_OF_CONCEALMENT_NAME = "faux_sigil_of_concealment";
static const std::string FAUX_SIGIL_OF_FORTIFICATION_NAME = "faux_sigil_of_fortification";
static const std::string FAUX_SIGIL_OF_FORTUNE_NAME = "faux_sigil_of_fortune";
static const std::string FAUX_SIGIL_OF_PROTECTION_NAME = "faux_sigil_of_protection";
static const std::string FAUX_SIGIL_OF_RAGE_NAME = "faux_sigil_of_rage";
static const std::string FAUX_SIGIL_OF_REDEMPTION_NAME = "faux_sigil_of_redemption";
static const std::string FAUX_SIGIL_OF_SILENCE_NAME = "faux_sigil_of_silence";
static const std::string FAUX_SIGIL_OF_STRENGTH_NAME = "faux_sigil_of_strength";

static const std::map<std::string, Sigils> item_name_to_sigil_map = {
	{ FAUX_SIGIL_OF_CONCEALMENT_NAME, Sigils::CONCEALMENT },
	{ FAUX_SIGIL_OF_FORTIFICATION_NAME, Sigils::FORTIFICATION },
	{ FAUX_SIGIL_OF_FORTUNE_NAME, Sigils::FORTUNE },
	{ FAUX_SIGIL_OF_PROTECTION_NAME, Sigils::PROTECTION },
	{ FAUX_SIGIL_OF_RAGE_NAME, Sigils::RAGE },
	{ FAUX_SIGIL_OF_REDEMPTION_NAME, Sigils::REDEMPTION },
	{ FAUX_SIGIL_OF_SILENCE_NAME, Sigils::SILENCE },
	{ FAUX_SIGIL_OF_STRENGTH_NAME, Sigils::STRENGTH },
};

static YYTKInterface* g_ModuleInterface = nullptr;
static CInstance* global_instance = nullptr;
static bool load_on_start = true;
static bool unlock_recipes = true;
static bool sigil_item_used = false;
static bool sigil_of_silence = false;
static double ari_x = -1;
static double ari_y = -1;
static int held_item_id = -1;
static std::string ari_current_gm_room = "";
static std::unordered_set<Sigils> active_sigils = {};
static std::map<Sigils, int> sigil_to_item_id_map = {};
static std::map<int, Sigils> item_id_to_sigil_map = {};
static std::map<std::string, int> player_state_to_id_map = {};
static std::map<std::string, int> monster_name_to_id_map = {};
static std::map<std::string, int> status_effect_name_to_id_map = {};
static std::map<std::string, uint64_t> notification_name_to_last_display_time_map = {}; // Tracks when a notification was last displayed.
static std::map<std::string, std::vector<CInstance*>> script_name_to_reference_map = {}; // Vector<CInstance*> holds references to Self and Other for each script.

void ResetStaticFields()
{
	unlock_recipes = true;
	sigil_item_used = false;
	sigil_of_silence = false;
	ari_x = -1;
	ari_y = -1;
	held_item_id = -1;
	ari_current_gm_room = "";
	active_sigils.clear();
	notification_name_to_last_display_time_map.clear();
	script_name_to_reference_map.clear();
}

bool GameIsPaused()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);
	RValue paused = global_instance->GetMember("__pause_status");
	return paused.ToInt64() > 0;
}

uint64_t GetCurrentSystemTime() {
	return duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

bool StructVariableExists(RValue the_struct, const char* variable_name)
{
	RValue struct_exists = g_ModuleInterface->CallBuiltin(
		"struct_exists",
		{ the_struct, variable_name }
	);

	return struct_exists.ToBoolean();
}

RValue StructVariableSet(RValue the_struct, const char* variable_name, RValue value)
{
	return g_ModuleInterface->CallBuiltin(
		"struct_set",
		{ the_struct, variable_name, value }
	);
}

void LoadPlayerStates()
{
	size_t array_length;
	RValue player_states = global_instance->GetMember("__player_state__");
	g_ModuleInterface->GetArraySize(player_states, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* player_state;
		g_ModuleInterface->GetArrayEntry(player_states, i, player_state);

		player_state_to_id_map[player_state->ToString()] = i;
	}
}

void LoadItems()
{
	size_t array_length;
	RValue item_data = global_instance->GetMember("__item_data");
	g_ModuleInterface->GetArraySize(item_data, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* item;
		g_ModuleInterface->GetArrayEntry(item_data, i, item);

		RValue name_key = item->GetMember("name_key"); // The item's localization key
		if (name_key.m_Kind != VALUE_NULL && name_key.m_Kind != VALUE_UNDEFINED && name_key.m_Kind != VALUE_UNSET)
		{
			int item_id = item->GetMember("item_id").ToInt64();
			std::string item_name = item->GetMember("recipe_key").ToString(); // The internal item name

			// Sigil items
			if (item_name_to_sigil_map.contains(item_name))
			{
				sigil_to_item_id_map[item_name_to_sigil_map.at(item_name)] = item_id;
				item_id_to_sigil_map[item_id] = item_name_to_sigil_map.at(item_name);

				*item->GetRefMember("health_modifier") = 0;
			}
		}
	}
}

void LoadMonsters()
{
	size_t array_length;
	RValue monster_names = global_instance->GetMember("__monster_id__");
	g_ModuleInterface->GetArraySize(monster_names, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* monster_name;
		g_ModuleInterface->GetArrayEntry(monster_names, i, monster_name);

		monster_name_to_id_map[monster_name->ToString()] = i;
	}
}

void LoadStatusEffects()
{
	size_t array_length;
	RValue status_effects = global_instance->GetMember("__status_effect_id__");
	g_ModuleInterface->GetArraySize(status_effects, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* status_effect;
		g_ModuleInterface->GetArrayEntry(status_effects, i, status_effect);

		status_effect_name_to_id_map[status_effect->ToString()] = i;
	}
}

bool AriCurrentGmRoomIsDungeonFloor()
{
	return ari_current_gm_room.contains("rm_mines") && ari_current_gm_room != "rm_mines_entry" && !ari_current_gm_room.contains("seal");
}

void UnlockRecipe(int item_id, CInstance* Self, CInstance* Other)
{
	RValue __ari = *global_instance->GetRefMember("__ari");
	RValue recipe_unlocks = *__ari.GetRefMember("recipe_unlocks");
	bool new_recipe_unlocked = false;

	if (recipe_unlocks[item_id].m_Real == 0.0)
	{
		recipe_unlocks[item_id] = 1.0; // This value is ultimately what unlocks the recipe.
		new_recipe_unlocked = true;
	}
}

void CreateNotification(bool ignore_cooldown, std::string notification_localization_str, CInstance* Self, CInstance* Other)
{
	uint64_t current_system_time = GetCurrentSystemTime();
	if (ignore_cooldown || current_system_time > notification_name_to_last_display_time_map[notification_localization_str] + 5000)
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

		notification_name_to_last_display_time_map[notification_localization_str] = current_system_time;
	}
}

void SpawnLadder(CInstance* Self, CInstance* Other, int64_t x_coord, int64_t y_coord)
{
	CScript* gml_Script_spawn_ladder = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SPAWN_LADDER,
		(PVOID*)&gml_Script_spawn_ladder
	);

	RValue x = (x_coord * 2) / 16;
	RValue y = (y_coord * 2) / 16;
	RValue* x_ptr = &x;
	RValue* y_ptr = &y;
	RValue* rvalue_array[2] = { x_ptr, y_ptr };
	RValue retval;
	gml_Script_spawn_ladder->m_Functions->m_ScriptFunction(
		Self,
		Other,
		retval,
		2,
		rvalue_array
	);
}

void RegisterStatusEffect(CInstance* Self, CInstance* Other, RValue status_effect_id, RValue amount, RValue start, RValue finish)
{
	CScript* gml_script_register_status_effect = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_REGISTER_STATUS_EFFECT,
		(PVOID*)&gml_script_register_status_effect
	);

	RValue result;
	RValue* status_effect_id_ptr = &status_effect_id;
	RValue* amount_ptr = &amount;
	RValue* start_ptr = &start;
	RValue* finish_ptr = &finish;
	RValue* argument_array[4] = { status_effect_id_ptr, amount_ptr, start_ptr, finish_ptr };

	gml_script_register_status_effect->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		4,
		argument_array
	);
}

bool FairyBuffIsActive()
{
	RValue ari = global_instance->GetMember("__ari");
	RValue status_effects = ari.GetMember("status_effects");
	RValue effects = status_effects.GetMember("effects");
	RValue inner = effects.GetMember("inner");

	if (StructVariableExists(inner, std::to_string(status_effect_name_to_id_map["fairy"]).c_str()))
	{
		RValue fairy_status = inner.GetMember(std::to_string(status_effect_name_to_id_map["fairy"]));
		return fairy_status.m_Kind == VALUE_OBJECT;
	}

	return false;
}

double GetInvulnerabilityHits()
{
	RValue ari = global_instance->GetMember("__ari");
	return ari.GetMember("invulnerable_hits").ToDouble(); 
}

void SetInvulnerabilityHits(double amount)
{
	RValue ari = *global_instance->GetRefMember("__ari");
	double invulnerability_hits = ari.GetMember("invulnerable_hits").ToDouble();

	if (amount == 0)
		*ari.GetRefMember("invulnerable_hits") = amount;
	else
		*ari.GetRefMember("invulnerable_hits") = invulnerability_hits + amount;
}

void UpdateToolbarMenu(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_update_toolbar_menu = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_UPDATE_TOOLBAR_MENU,
		(PVOID*)&gml_script_update_toolbar_menu
	);

	RValue result;
	gml_script_update_toolbar_menu->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		0,
		nullptr
	);
}

RValue GetDynamicItemSprite(int item_id)
{
	if (item_id == sigil_to_item_id_map[Sigils::CONCEALMENT])
	{
		if (!GameIsPaused() && (active_sigils.contains(Sigils::CONCEALMENT) || !AriCurrentGmRoomIsDungeonFloor()))
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_faux_sigil_of_concealment_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_faux_sigil_of_concealment" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::FORTIFICATION])
	{
		if (!GameIsPaused() && (active_sigils.contains(Sigils::FORTIFICATION) || !AriCurrentGmRoomIsDungeonFloor()))
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_faux_sigil_of_fortification_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_faux_sigil_of_fortification" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::FORTUNE])
	{
		if (!GameIsPaused() && (active_sigils.contains(Sigils::FORTUNE) || !AriCurrentGmRoomIsDungeonFloor()))
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_faux_sigil_of_fortune_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_faux_sigil_of_fortune" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::PROTECTION])
	{
		if (!GameIsPaused() && (active_sigils.contains(Sigils::PROTECTION) || !AriCurrentGmRoomIsDungeonFloor() || GetInvulnerabilityHits() > 0))
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_faux_sigil_of_protection_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_faux_sigil_of_protection" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::RAGE])
	{
		if (!GameIsPaused() && (active_sigils.contains(Sigils::RAGE) || !AriCurrentGmRoomIsDungeonFloor()))
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_faux_sigil_of_rage_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_faux_sigil_of_rage" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::REDEMPTION])
	{
		if (!GameIsPaused() && (active_sigils.contains(Sigils::REDEMPTION) || !AriCurrentGmRoomIsDungeonFloor() || FairyBuffIsActive()))
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_faux_sigil_of_redemption_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_faux_sigil_of_redemption" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::SILENCE])
	{
		if (!GameIsPaused() && (active_sigils.contains(Sigils::SILENCE) || !AriCurrentGmRoomIsDungeonFloor()))
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_faux_sigil_of_silence_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_faux_sigil_of_silence" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::STRENGTH])
	{
		if (!GameIsPaused() && (active_sigils.contains(Sigils::STRENGTH) || !AriCurrentGmRoomIsDungeonFloor()))
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_faux_sigil_of_strength_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_faux_sigil_of_strength" });
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

	if (strstr(self->m_Object->m_Name, "obj_ari"))
	{
		RValue x;
		g_ModuleInterface->GetBuiltin("x", self, NULL_INDEX, x);
		ari_x = x.ToDouble();

		RValue y;
		g_ModuleInterface->GetBuiltin("y", self, NULL_INDEX, y);
		ari_y = y.ToDouble();


		// Process used items.
		RValue ari = self->ToRValue();
		if (StructVariableExists(ari, "fsm"))
		{
			RValue fsm = ari.GetMember("fsm");

			if (StructVariableExists(fsm, "state"))
			{
				RValue state = fsm.GetMember("state");
				if (StructVariableExists(state, "state_id"))
				{
					RValue state_id = state.GetMember("state_id");
					if (state_id.ToInt64() == player_state_to_id_map["hold_to_use"])
					{
						if (StructVariableExists(state, "did_action"))
						{
							RValue did_action = state.GetMember("did_action");
							if (did_action.ToBoolean())
							{
								if (sigil_item_used) // Necessary since did_action==true will get called a few times when the item is used.
								{
									sigil_item_used = false;

									if (held_item_id == sigil_to_item_id_map[Sigils::CONCEALMENT])
										active_sigils.insert(Sigils::CONCEALMENT);
									else if (held_item_id == sigil_to_item_id_map[Sigils::FORTIFICATION])
										active_sigils.insert(Sigils::FORTIFICATION);
									else if (held_item_id == sigil_to_item_id_map[Sigils::FORTUNE])
									{
										active_sigils.insert(Sigils::FORTUNE);
										SpawnLadder(global_instance->GetRefMember("__ari")->ToInstance(), self, ari_x, ari_y);
									}
									else if (held_item_id == sigil_to_item_id_map[Sigils::PROTECTION])
									{
										std::vector<CInstance*> refs = script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_UPDATE];

										active_sigils.insert(Sigils::PROTECTION);
										RegisterStatusEffect(refs[0], refs[1], status_effect_name_to_id_map["guardians_shield"], RValue(), 1, 2147483647.0);
										SetInvulnerabilityHits(2);
									}
									else if (held_item_id == sigil_to_item_id_map[Sigils::RAGE])
										active_sigils.insert(Sigils::RAGE);
									else if (held_item_id == sigil_to_item_id_map[Sigils::REDEMPTION])
									{
										std::vector<CInstance*> refs = script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_UPDATE];

										active_sigils.insert(Sigils::REDEMPTION);
										RegisterStatusEffect(refs[0], refs[1], status_effect_name_to_id_map["fairy"], RValue(), 1, 2147483647.0);
									}
									else if (held_item_id == sigil_to_item_id_map[Sigils::SILENCE])
										active_sigils.insert(Sigils::SILENCE);
									else if (held_item_id == sigil_to_item_id_map[Sigils::STRENGTH])
										active_sigils.insert(Sigils::STRENGTH);

									if (script_name_to_reference_map.contains(GML_SCRIPT_UPDATE_TOOLBAR_MENU))
										UpdateToolbarMenu(script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][0], script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][1]);
								}
							}
						}
					}
				}
			}
		}
	}

	if (strstr(self->m_Object->m_Name, "obj_monster"))
	{
		RValue monster = self->ToRValue();
		if (StructVariableExists(monster, "monster_id"))
		{
			RValue monster_id = *monster.GetRefMember("monster_id");

			bool is_valid_monster_object = false;
			for (const auto& entry : monster_name_to_id_map) {
				if (entry.second == monster_id.ToInt64()) {
					is_valid_monster_object = true;
					break;
				}
			}

			if (is_valid_monster_object)
			{
				// Sigil of Concealment
				if (active_sigils.contains(Sigils::CONCEALMENT))
				{
					if (StructVariableExists(monster, "config"))
					{
						RValue config = *monster.GetRefMember("config");
						RValue hit_points = monster.GetMember("hit_points");
						if (!StructVariableExists(monster, "__deep_dungeon__conceal_hit_points"))
							StructVariableSet(monster, "__deep_dungeon__conceal_hit_points", hit_points);

						RValue original_hit_points = monster.GetMember("__deep_dungeon__conceal_hit_points");
						if (hit_points.ToDouble() == original_hit_points.ToDouble())
							StructVariableSet(monster, "aggro", false);
						else
						{
							active_sigils.erase(Sigils::CONCEALMENT);
							CreateNotification(false, CONCEALMENT_LOST_NOTIFICATION_KEY, nullptr, nullptr);

							if (script_name_to_reference_map.contains(GML_SCRIPT_UPDATE_TOOLBAR_MENU))
								UpdateToolbarMenu(script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][0], script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][1]);
						}
					}
				}

				// Sigil of Rage
				if (active_sigils.contains(Sigils::RAGE))
				{
					if (StructVariableExists(monster, "config") && StructVariableExists(monster, "hit_points"))
					{
						RValue config = *monster.GetRefMember("config");
						RValue hit_points = monster.GetMember("hit_points");
						if (!StructVariableExists(monster, "__deep_dungeon__rage_hit_points"))
							StructVariableSet(monster, "__deep_dungeon__rage_hit_points", hit_points);

						RValue original_hit_points = monster.GetMember("__deep_dungeon__rage_hit_points");
						if (hit_points.ToDouble() != original_hit_points.ToDouble())
							*monster.GetRefMember("hit_points") = 0;
					}
				}
			}
		}
	}
}

RValue& GmlScriptStatusEffectManagerUpdateCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (!script_name_to_reference_map.contains(GML_SCRIPT_STATUS_EFFECT_MANAGER_UPDATE))
		script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_UPDATE] = { Self, Other };

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_STATUS_EFFECT_MANAGER_UPDATE));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptSpawnMonsterCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	// Sigil of Silence
	if (sigil_of_silence)
		return Result;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_SPAWN_MONSTER));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptUpdateToolbarMenuCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (!script_name_to_reference_map.contains(GML_SCRIPT_UPDATE_TOOLBAR_MENU))
		script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU] = { Self, Other };

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_UPDATE_TOOLBAR_MENU));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptGetUiIconCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_ITEM_UI_ICON));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (Self != nullptr)
	{
		RValue self = Self->ToRValue();
		if (StructVariableExists(self, "item_id"))
		{
			int item_id = self.GetMember("item_id").ToInt64();

			if (item_id_to_sigil_map.contains(item_id))
				Result = GetDynamicItemSprite(item_id);
		}
	}

	return Result;
}

RValue& GmlScriptDamageCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	// Sigil of Fortification
	if (active_sigils.contains(Sigils::FORTIFICATION))
	{
		if (!StructVariableExists(*Arguments[0], "__deep_dungeon__fortification_applied")) // Prevents monster attacks that "persist" from repeatedly getting Fortification applied
		{
			RValue target = Arguments[0]->GetMember("target");
			if (target.ToInt64() == 1) // Ari
			{
				double damage = Arguments[0]->GetMember("damage").ToDouble();
				int penalty = std::trunc(damage * 0.50); // 50% reduced damage
				*Arguments[0]->GetRefMember("damage") = damage - penalty;
			}

			StructVariableSet(*Arguments[0], "__deep_dungeon__fortification_applied", true);
		}
	}

	// Sigil of Strength
	if (active_sigils.contains(Sigils::STRENGTH))
	{
		RValue target = Arguments[0]->GetMember("target");
		if (target.ToInt64() != 1) // Everything not Ari
		{
			if (!StructVariableExists(*Arguments[0], "__deep_dungeon__strength_applied"))
			{
				double damage = std::trunc(Arguments[0]->GetMember("damage").ToDouble() * 1.5); // 50% increased damage
				*Arguments[0]->GetRefMember("damage") = damage;
				StructVariableSet(*Arguments[0], "__deep_dungeon__strength_applied", true);
			}
		}
	}

	// Sigil of Rage
	if (active_sigils.contains(Sigils::RAGE))
	{
		RValue target = Arguments[0]->GetMember("target");
		if (target.ToInt64() != 1) // Everything not Ari
		{
			double damage = Arguments[0]->GetMember("damage").ToDouble();
			if (damage != 0) // Not a miss
			{
				*Arguments[0]->GetRefMember("critical") = true;
				*Arguments[0]->GetRefMember("damage") = 9999.0;
			}
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_DAMAGE));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptUseItemCallback(
	IN CInstance* Self, // Changes depending on the invocation context. For world interactables like a fountain, Self->m_Object->m_Name == "obj_world_fountain". For Ari using an item, Self->m_Object == NULL.
	IN CInstance* Other, // Changes depending on the invocation context. For world interactables like a fountain, Other->m_Object->m_Name == "Game". For Ari using an item, Other->m_Object->m_Name == "obj_ari".
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (AriCurrentGmRoomIsDungeonFloor())
	{
		if (Self->m_Object == NULL && strstr(Other->m_Object->m_Name, "obj_ari"))
		{
			// Sigil Already Used
			if (item_id_to_sigil_map.contains(held_item_id) && active_sigils.contains(item_id_to_sigil_map[held_item_id]))
			{
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - That sigil is already active!", MOD_NAME, VERSION);
				CreateNotification(false, SIGIL_LIMIT_NOTIFICATION_KEY, Self, Other);
				return Result;
			}

			// Protection Already Active
			if (held_item_id == sigil_to_item_id_map[Sigils::PROTECTION] && GetInvulnerabilityHits() > 0)
			{
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - That sigil is already active!", MOD_NAME, VERSION);
				CreateNotification(false, SIGIL_LIMIT_NOTIFICATION_KEY, Self, Other);
				return Result;
			}

			// Redemption Already Active
			if (held_item_id == sigil_to_item_id_map[Sigils::REDEMPTION] && FairyBuffIsActive())
			{
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - That sigil is already active!", MOD_NAME, VERSION);
				CreateNotification(false, SIGIL_LIMIT_NOTIFICATION_KEY, Self, Other);
				return Result;
			}
		}
	}
	else
	{
		if (Self->m_Object == NULL && strstr(Other->m_Object->m_Name, "obj_ari"))
		{
			// Faux Sigil Items
			if (item_id_to_sigil_map.contains(held_item_id))
			{
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - You may only use Faux Sigils inside the dungeon!", MOD_NAME, VERSION);
				CreateNotification(false, SIGIL_RESTRICTED_NOTIFICATION_KEY, Self, Other);
				return Result;
			}
		}
	}

	// Sigil Item
	sigil_item_used = false;
	if (item_id_to_sigil_map.contains(held_item_id))
		sigil_item_used = true;

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

	if (Result.m_Kind != VALUE_UNDEFINED)
	{
		int item_id = Result.GetMember("item_id").ToInt64();
		if (held_item_id != item_id)
			held_item_id = item_id;
	}

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
	if (unlock_recipes)
	{
		unlock_recipes = false;
		for (auto& entry : sigil_to_item_id_map)
			UnlockRecipe(entry.second, Self, Other);
	}

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

RValue& GmlScriptGoToRoomCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GO_TO_ROOM));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	RValue gm_room = Result.GetMember("gm_room");
	RValue room_name = g_ModuleInterface->CallBuiltin("room_get_name", { gm_room });
	ari_current_gm_room = room_name.ToString();

	if (active_sigils.contains(Sigils::SILENCE))
		sigil_of_silence = true;
	else
		sigil_of_silence = false;
	active_sigils.clear();

	if (script_name_to_reference_map.contains(GML_SCRIPT_UPDATE_TOOLBAR_MENU))
		UpdateToolbarMenu(script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][0], script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][1]);

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
		load_on_start = false;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		LoadItems();
		LoadMonsters();
		LoadPlayerStates();
		LoadStatusEffects();
	}
	else
		ResetStaticFields();

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

void CreateHookGmlScriptStatusEffectManagerUpdate(AurieStatus& status)
{
	CScript* gml_script_status_effect_manager_update = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_STATUS_EFFECT_MANAGER_UPDATE,
		(PVOID*)&gml_script_status_effect_manager_update
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_STATUS_EFFECT_MANAGER_UPDATE);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_STATUS_EFFECT_MANAGER_UPDATE,
		gml_script_status_effect_manager_update->m_Functions->m_ScriptFunction,
		GmlScriptStatusEffectManagerUpdateCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_STATUS_EFFECT_MANAGER_UPDATE);
	}
}

void CreateHookGmlScriptSpawnMonster(AurieStatus& status)
{
	CScript* gml_script_spawn_monster = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SPAWN_MONSTER,
		(PVOID*)&gml_script_spawn_monster
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_SPAWN_MONSTER);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_SPAWN_MONSTER,
		gml_script_spawn_monster->m_Functions->m_ScriptFunction,
		GmlScriptSpawnMonsterCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_SPAWN_MONSTER);
	}
}

void CreateHookGmlScriptUpdateToolbarMenu(AurieStatus& status)
{
	CScript* gml_script_update_toolbar_menu = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_UPDATE_TOOLBAR_MENU,
		(PVOID*)&gml_script_update_toolbar_menu
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_UPDATE_TOOLBAR_MENU);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_UPDATE_TOOLBAR_MENU,
		gml_script_update_toolbar_menu->m_Functions->m_ScriptFunction,
		GmlScriptUpdateToolbarMenuCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_UPDATE_TOOLBAR_MENU);
	}
}

void CreateHookGmlScriptGetUiIcon(AurieStatus& status)
{
	CScript* gml_script_get_ui_icon = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_ITEM_UI_ICON,
		(PVOID*)&gml_script_get_ui_icon
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_ITEM_UI_ICON);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_ITEM_UI_ICON,
		gml_script_get_ui_icon->m_Functions->m_ScriptFunction,
		GmlScriptGetUiIconCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_ITEM_UI_ICON);
	}
}

void CreateHookGmlScriptDamage(AurieStatus& status)
{
	CScript* gml_script_damage = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_DAMAGE,
		(PVOID*)&gml_script_damage
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_DAMAGE);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_DAMAGE,
		gml_script_damage->m_Functions->m_ScriptFunction,
		GmlScriptDamageCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_DAMAGE);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_USE_ITEM);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_USE_ITEM);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_HELD_ITEM);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_HELD_ITEM);
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

void CreateHookGmlScriptGoToRoom(AurieStatus& status)
{
	CScript* gml_script_go_to_room = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GO_TO_ROOM,
		(PVOID*)&gml_script_go_to_room
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GO_TO_ROOM);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GO_TO_ROOM,
		gml_script_go_to_room->m_Functions->m_ScriptFunction,
		GmlScriptGoToRoomCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GO_TO_ROOM);
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

	CreateObjectCallback(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptStatusEffectManagerUpdate(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptSpawnMonster(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptUpdateToolbarMenu(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptGetUiIcon(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptDamage(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptUseItem(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptHeldItem(status);
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

	CreateHookGmlScriptGoToRoom(status);
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
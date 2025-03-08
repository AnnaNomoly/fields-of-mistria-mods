#include <map>
#include <deque>
#include <random>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static const int SIX_AM_IN_SECONDS = 21600;
static const int EIGHT_PM_IN_SECONDS = 72000;
static const int END_OF_DAY_IN_SECONDS = 93600;
static const int THIRTY_SECONDS = 30;
static const int ONE_MINUTE_IN_SECONDS = 60;
static const int THIRY_MINUTES_IN_SECONDS = 1800;
static const int ONE_HOUR_IN_SECONDS = 3600;
static const int HUNGER_LOST_PER_TICK = -1;
static const int SANITY_LOST_PER_TICK = -1;
static const int HUNGER_HEALTH_LOST_PER_TICK = -10;
static const int SANITY_HEALTH_LOST_PER_TICK = -1;
static const int STARTING_HUNGER_VALUE = 15; //100;
static const int STARTING_SANITY_VALUE = 100;
static const int SPICE_OF_LIFE_QUEUE_SIZE = 10;
static const int ADRENALINE_RUSH_SANITY_RECOVERY = 5;
static const char* GUARDIANS_SHIELD = "guardians_shield";
static const char* DONT_STARVE_INTRODUCTION_LETTER = "dont_starve_introduction";
static const char* GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE = "gml_Script_deserialize@StatusEffectManager@StatusEffectManager";
static const std::string IGNORED_ITEMS[] = {
	"balors_crate", "confiscated_coffee", "dungeon_fountain_health", "dungeon_fountain_stamina", "horse_potion", "lurid_colored_drink",
	"ryis_lumber", "soup_of_the_day", "soup_of_the_day_gold", "stinky_stamina_potion", "unusual_seed", "world_fountain"
};
static const std::map<std::string, bool> DUNGEON_LOCATIONS = {
	{"deep_woods", true},
	{"dungeon", true},
	{"earth_seal", true},
	{"fire_seal", true},
	{"mines_entry", true},
	{"water_seal", true}
};
static const std::map<std::string, bool> LOCATION_SANITY_LOSS_MAP = {
	{"adelines_bedroom", false},
	{"adelines_office", false},
	{"aldaria", true},
	{"balors_room", false},
	{"bathhouse", false},
	{"bathhouse_bath", false},
	{"bathhouse_bedroom", false},
	{"bathhouse_change_room", false},
	{"beach", true},
	{"blacksmith_room_left", false},
	{"blacksmith_room_right", false},
	{"blacksmith_store", false},
	{"celines_room", false},
	{"clinic_b1", false},
	{"clinic_f1", false},
	{"clinic_f2", false},
	{"deep_woods", true},
	{"dells_bedroom", false},
	{"dungeon", true},
	{"earth_seal", true},
	{"eastern_road", true},
	{"eilands_bedroom", false},
	{"eilands_office", false},
	{"elsies_bedroom", false},
	{"errols_bedroom", false},
	{"farm", true},
	{"fire_seal", true},
	{"general_store_home", false},
	{"general_store_store", false},
	{"haydens_bedroom", false},
	{"haydens_farm", true},
	{"haydens_house", false},
	{"holt_and_noras_bedroom", false},
	{"inn", false},
	{"jo_and_hemlocks_room", false},
	{"landens_house_f1", false},
	{"landens_house_f2", false},
	{"large_barn", false},
	{"large_coop", false},
	{"lucs_room", false},
	{"manor_house_dining_room", false},
	{"manor_house_entry", false},
	{"maples_room", false},
	{"medium_barn", false},
	{"medium_coop", false},
	{"mill", false},
	{"mines_entry", true},
	{"museum_entry", false},
	{"narrows", true},
	{"player_home", false},
	{"player_home_east", false},
	{"player_home_north", false},
	{"player_home_west", false},
	{"reinas_room", false},
	{"seridias_chamber", true},
	{"small_barn", false},
	{"small_coop", false},
	{"summit", true},
	{"terithias_house", false},
	{"town", true},
	{"water_seal", true},
	{"western_ruins", true}
};

static YYTKInterface* g_ModuleInterface = nullptr;
static std::map<std::string, std::vector<CInstance*>> script_name_to_reference_map; // Vector<CInstance*> holds references to Self and Other for each script. 
static bool run_once = true;
static bool localize_items = true;
static std::string ari_current_location = "";
static bool ari_is_in_dungeon = false;
static int ari_hunger_value = STARTING_HUNGER_VALUE;
static int ari_sanity_value = STARTING_SANITY_VALUE;
static bool is_hunger_tracked_time_interval = false;
static bool is_sanity_tracked_time_interval = false;
static bool snapshot_position = false; // Indicates when to snapshot Ari's current position.
static bool rollback_position = false; // Indicates when to rollback Ari's position.
static int time_of_last_hunger_tick = 0;
static int time_of_last_sanity_tick = 0;
static int held_item_id = -1;
static bool game_is_active = false;
static int hunger_stamina_health_penatly = 0;

static std::map<std::string, int> item_name_to_restoration_map = {}; // Excludes cooked dishes
static std::map<std::string, int> recipe_name_to_stars_map = {}; // Only cooked dishes

// Random Noise
static std::mt19937 generator(std::random_device{}());
static std::uniform_int_distribution<int> distribution_1_25(1, 25);
static std::uniform_int_distribution<int> distribution_1_1000(1, 1000);
static std::vector<std::vector<std::vector<int>>> noise_masks = {};
static int total_noise_masks = 100;
static int current_mask = 0;
static int window_width = 0;
static int window_height = 0;
static size_t frame_counter = 0;

// Position Rollback
static int rollback_position_x = -1;
static int rollback_position_y = -1;

// Spice of Life
static std::deque<std::string> food_queue = {};
static std::string localized_item_name = "";
static std::map<std::string, std::string> localized_item_name_to_internal_name_map = {};
static std::map<std::string, double> item_name_to_original_stamina_recovery_map = {};

// Perks & Status Effects
static std::map<std::string, int64_t> perk_name_to_id_map = {};
static std::map<std::string, int64_t> status_effect_name_to_id_map = {};
static bool priestess_shield_active = false;
static bool is_priestess_shield_tracked_time_interval = false;
static int time_of_last_priestess_shield_tick = 0; // Blood Pact perk timer.

std::string FoodQueueToString()
{
	std::string food_queue_string = "[";
	for (int i = 0; i < food_queue.size(); i++)
	{
		food_queue_string += food_queue[i];
		if (i != food_queue.size() - 1)
			food_queue_string += ", ";
	}

	food_queue_string += "]";
	return food_queue_string;
}

void UpdateFoodQueue(std::string item_name)
{
	if (food_queue.size() == SPICE_OF_LIFE_QUEUE_SIZE)
		food_queue.pop_front();
	food_queue.push_back(item_name);

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Food queue updated: %s", FoodQueueToString().c_str());
}

int GetFoodQueueOccurrences(std::string item_name)
{
	return std::count(food_queue.begin(), food_queue.end(), item_name);
}

double GetFoodPenalty(std::string item_name, int occurrence_offset)
{
	int occurrences = GetFoodQueueOccurrences(item_name) + occurrence_offset;
	double penalty;
	if (occurrences <= 1)
		penalty = 0.0;
	if (occurrences == 2)
		penalty = 0.1;
	if (occurrences == 3)
		penalty = 0.3;
	if (occurrences == 4)
		penalty = 0.5;
	if (occurrences == 5)
		penalty = 0.7;
	if (occurrences >= 6)
		penalty = 0.9;

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - The stamina restoration penalty for %s was %d percent!", item_name.c_str(), static_cast<int>(penalty * 100));
	return penalty;
}

RValue GetLocalizedString(CInstance* Self, CInstance* Other, std::string localization_key)
{
	CScript* gml_script_get_localizer = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_get@Localizer@Localizer",
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

RValue PerkActive(CInstance* Self, CInstance* Other, int64_t perk_id)
{
	CScript* gml_script_perk_active = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_perk_active@Ari@Ari",
		(PVOID*)&gml_script_perk_active
	);

	RValue result;
	RValue input = perk_id;
	RValue* input_ptr = &input;
	gml_script_perk_active->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &input_ptr }
	);

	return result;
}

RValue GetMaxHealth(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_get_max_health = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_get_max_health@Ari@Ari",
		(PVOID*)&gml_script_get_max_health
	);

	RValue max_health;
	gml_script_get_max_health->m_Functions->m_ScriptFunction(
		Self,
		Other,
		max_health,
		0,
		nullptr
	);

	return max_health;
}

RValue GetCurrentHealth(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_get_health = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_get_health@Ari@Ari",
		(PVOID*)&gml_script_get_health
	);

	RValue current_health;
	gml_script_get_health->m_Functions->m_ScriptFunction(
		Self,
		Other,
		current_health,
		0,
		nullptr
	);

	return current_health;
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

void SetHealth(CInstance* Self, CInstance* Other, int value)
{
	CScript* gml_script_set_health = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_set_health@Ari@Ari",
		(PVOID*)&gml_script_set_health
	);

	RValue result;
	RValue health_value = value;
	RValue* health_value_ptr = &health_value;

	gml_script_set_health->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &health_value_ptr }
	);
}

RValue ItemIdToString(CInstance* Self, CInstance* Other, int id)
{
	CScript* gml_script_item_id_to_name = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_try_item_id_to_string",
		(PVOID*)&gml_script_item_id_to_name
	);

	RValue item_name;
	RValue item_id = id;
	RValue* item_id_ptr = &item_id;
	gml_script_item_id_to_name->m_Functions->m_ScriptFunction(
		Self,
		Other,
		item_name,
		1,
		{ &item_id_ptr }
	);

	return item_name;
}

RValue StringToItemId(CInstance* Self, CInstance* Other, std::string name)
{
	CScript* gml_script_name_to_item_id = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_try_string_to_item_id",
		(PVOID*)&gml_script_name_to_item_id
	);

	RValue item_id;
	RValue item_name = name;
	RValue* item_name_ptr = &item_name;
	gml_script_name_to_item_id->m_Functions->m_ScriptFunction(
		Self,
		Other,
		item_id,
		1,
		{ &item_name_ptr }
	);

	return item_id;
}

void SetItemStaminaModifier(int item_id, double stamina_value)
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	RValue __item_data = global_instance->at("__item_data");
	RValue item = g_ModuleInterface->CallBuiltin("array_get", { __item_data, item_id });

	g_ModuleInterface->CallBuiltin("struct_set", { item, "stamina_modifier", stamina_value });
}

void ResetAllItemStaminaModifiers(CInstance* Self, CInstance* Other)
{
	if (!item_name_to_original_stamina_recovery_map.empty())
	{
		for (const auto& map_entry : item_name_to_original_stamina_recovery_map) {
			RValue item_id = StringToItemId(Self, Other, map_entry.first);
			if (item_id.m_Kind == VALUE_INT64)
			{
				SetItemStaminaModifier(item_id.m_i64, map_entry.second);
			}
		}
	}
}

void CancelStatusEffect(CInstance* Self, CInstance* Other, RValue status_effect_id)
{
	CScript* gml_script_cancel_status_effect = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_cancel@StatusEffectManager@StatusEffectManager",
		(PVOID*)&gml_script_cancel_status_effect
	);

	RValue result;
	RValue* status_effect_id_ptr = &status_effect_id;

	gml_script_cancel_status_effect->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &status_effect_id_ptr }
	);
}

void RegisterStatusEffect(CInstance* Self, CInstance* Other, RValue status_effect_id, RValue amount, RValue start, RValue finish)
{
	CScript* gml_script_register_status_effect = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_register@StatusEffectManager@StatusEffectManager",
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

bool AriHasInvulnerableHits()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	RValue __ari = global_instance->at("__ari").m_Object;
	RValue invulnerable_hits_exists = g_ModuleInterface->CallBuiltin("struct_exists", { __ari, "invulnerable_hits" });
	if (invulnerable_hits_exists.m_Kind == VALUE_BOOL && invulnerable_hits_exists.m_Real == 1)
	{
		RValue invulnerable_hits = g_ModuleInterface->CallBuiltin("struct_get", { __ari, "invulnerable_hits" });
		if (invulnerable_hits.m_Kind == VALUE_REAL && invulnerable_hits.m_Real > 0)
			return true;
	}
	return false;
}

void ActivatePriestessShield(CInstance* Self, CInstance* Other)
{
	// Set invulnerability hit on the global instance.
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	RValue __ari = global_instance->at("__ari").m_Object;
	RValue invulnerable_hits_exists = g_ModuleInterface->CallBuiltin("struct_exists", { __ari, "invulnerable_hits" });
	if (invulnerable_hits_exists.m_Kind == VALUE_BOOL && invulnerable_hits_exists.m_Real == 1)
	{
		RValue invulnerable_hits = g_ModuleInterface->CallBuiltin("struct_get", { __ari, "invulnerable_hits" });
		if (invulnerable_hits.m_Kind == VALUE_REAL && invulnerable_hits.m_Real == 0)
		{
			invulnerable_hits = 1.0;
			g_ModuleInterface->CallBuiltin("struct_set", { __ari, "invulnerable_hits", invulnerable_hits });

			// Register the buff so the icon is displayed.
			RValue status_effect_id = status_effect_name_to_id_map[GUARDIANS_SHIELD];
			RValue amount = RValue();
			RValue start = 1;
			RValue finish = 2147483647.0; // "End of Time" value
			RegisterStatusEffect(Self, Other, status_effect_id, amount, start, finish);

			priestess_shield_active = true;
		}
		//else
		//{
		//	RValue invulnerable_hits = 1.0;
		//	g_ModuleInterface->CallBuiltin("struct_set", { __ari, "invulnerable_hits", invulnerable_hits });
		//}
	}
	//else
	//{
	//	RValue invulnerable_hits = 1.0;
	//	g_ModuleInterface->CallBuiltin("struct_set", { __ari, "invulnerable_hits", invulnerable_hits });
	//}
}

void CancelPriestessShield(CInstance* Self, CInstance* Other)
{
	// Set invulnerability hits on the global instance.
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	RValue __ari = global_instance->at("__ari").m_Object;
	RValue invulnerable_hits_exists = g_ModuleInterface->CallBuiltin("struct_exists", { __ari, "invulnerable_hits" });
	if (invulnerable_hits_exists.m_Kind == VALUE_BOOL && invulnerable_hits_exists.m_Real == 1)
	{
		RValue invulnerable_hits = g_ModuleInterface->CallBuiltin("struct_get", { __ari, "invulnerable_hits" });
		if (invulnerable_hits.m_Kind == VALUE_REAL && invulnerable_hits.m_Real > 0)
		{
			invulnerable_hits = 0.0;
			g_ModuleInterface->CallBuiltin("struct_set", { __ari, "invulnerable_hits", invulnerable_hits });
		}
	}

	// Cancel the buff so the icon.
	RValue status_effect_id = status_effect_name_to_id_map[GUARDIANS_SHIELD];
	CancelStatusEffect(Self, Other, status_effect_id);

	priestess_shield_active = false;
}

void SendMail(std::string mail_name_str)
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	RValue __ari = global_instance->at("__ari").m_Object;
	RValue inbox = __ari.at("inbox");
	RValue contents = inbox.at("contents");
	RValue contents_buffer = contents.at("__buffer");

	RValue mail;
	RValue mail_items_taken = false;
	RValue mail_name = mail_name_str;
	RValue mail_read = false;
	g_ModuleInterface->GetRunnerInterface().StructCreate(&mail);
	g_ModuleInterface->GetRunnerInterface().StructAddRValue(&mail, "items_taken", &mail_items_taken);
	g_ModuleInterface->GetRunnerInterface().StructAddRValue(&mail, "name", &mail_name);
	g_ModuleInterface->GetRunnerInterface().StructAddRValue(&mail, "read", &mail_read);

	g_ModuleInterface->CallBuiltin(
		"array_push",
		{ contents_buffer, mail }
	);

	RValue size = g_ModuleInterface->CallBuiltin(
		"struct_get",
		{ contents, "__count" }
	);
	RValue new_size = 1.0 + size.m_Real;

	g_ModuleInterface->CallBuiltin(
		"struct_set",
		{ contents, "__count", new_size }
	);
	g_ModuleInterface->CallBuiltin(
		"struct_set",
		{ contents, "__internal_size", new_size }
	);
}

bool MailExists(std::string mail_name_str)
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	RValue __ari = global_instance->at("__ari").m_Object;
	RValue inbox = __ari.at("inbox");
	RValue contents = inbox.at("contents");
	RValue contents_buffer = contents.at("__buffer");

	size_t size = 0;
	g_ModuleInterface->GetArraySize(contents_buffer, size);

	for (size_t i = 0; i < size; i++)
	{
		RValue* entry = nullptr;
		g_ModuleInterface->GetArrayEntry(contents_buffer, i, entry);

		RValue name = entry->at("name");
		if (strstr(name.AsString().data(), mail_name_str.c_str()))
			return true;
	}

	return false;
}

bool IsNight(int clock_time)
{
	if (clock_time >= EIGHT_PM_IN_SECONDS)
		return true;
	return false;
}

bool AriIsAtSanityLossLocation()
{
	if (ari_current_location.length() > 0)
		if (LOCATION_SANITY_LOSS_MAP.count(ari_current_location) > 0)
			return LOCATION_SANITY_LOSS_MAP.at(ari_current_location);
	return false;
}

bool AriIsHungry()
{
	return ari_hunger_value <= 0;
}

bool AriIsInsane()
{
	return ari_sanity_value <= 0;
}

bool GameIsPaused()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);
	RValue paused = global_instance->at("__pause_status");
	return paused.m_i64 > 0;
}

void GenerateNoiseMasks()
{
	RValue window_get_width = g_ModuleInterface->CallBuiltin(
		"window_get_width",
		{}
	);
	window_width = window_get_width.m_Real;

	RValue window_get_height = g_ModuleInterface->CallBuiltin(
		"window_get_height",
		{}
	);
	window_height = window_get_height.m_Real;

	g_ModuleInterface->Print(CM_LIGHTPURPLE, "[DontStarve %s] - Generating random noise for screen static effect. This may take several seconds...");
	for (int num_masks = 0; num_masks < total_noise_masks; num_masks++)
	{
		std::vector<std::vector<int>> noise = {};
		for (int i = 0; i < window_width; i++)
		{
			for (int j = 0; j < window_height; j++)
			{
				if (distribution_1_1000(generator) == 1000)
					noise.push_back({ i, j });
			}
		}
		noise_masks.push_back(noise);
	}
}

void FrameCallback(FWFrame& FrameContext)
{
	UNREFERENCED_PARAMETER(FrameContext);
	frame_counter++;
	if (frame_counter == 600)
		frame_counter = 0;
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

	if (strstr(self->m_Object->m_Name, "obj_monster"))
	{
		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		// Adrenaline Rush perk.
		int perk_id = perk_name_to_id_map["generous_in_defeat"];
		RValue perk_active = PerkActive(global_instance->at("__ari").m_Object, self, perk_id);
		if (perk_active.m_Kind == VALUE_BOOL && perk_active.m_Real == 1.0)
		{
			RValue exists = g_ModuleInterface->CallBuiltin("struct_exists", { self, "__dont_starve__processed_monster_death" });
			if (exists.m_Kind == VALUE_BOOL && exists.m_Real == 0)
			{
				RValue hit_points_exists = g_ModuleInterface->CallBuiltin("struct_exists", { self, "hit_points" });
				if (hit_points_exists.m_Kind == VALUE_BOOL && hit_points_exists.m_Real == 1)
				{
					RValue hit_points = self->at("hit_points");
					if (hit_points.m_Kind == VALUE_REAL && hit_points.m_Real <= 0)
					{
						g_ModuleInterface->CallBuiltin("struct_set", { self, "__dont_starve__processed_monster_death", true });

						// Adjust sanity points.
						ari_sanity_value += ADRENALINE_RUSH_SANITY_RECOVERY;
						if (ari_sanity_value > STARTING_SANITY_VALUE)
							ari_sanity_value = STARTING_SANITY_VALUE;

						// Debug print
						g_ModuleInterface->Print(CM_LIGHTPURPLE, "(DEBUG) Adrenaline Rush for slaying monster: %s", self->m_Object->m_Name); // TODO: Remove this debug print out
					}
				}
			}
		}
	}

	if (strstr(self->m_Object->m_Name, "obj_ari"))
	{
		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		// Time passed while Ari is hungry penalty.
		if (is_hunger_tracked_time_interval)
		{
			if (AriIsHungry())
			{
				ModifyHealth(global_instance->at("__ari").m_Object, self, HUNGER_HEALTH_LOST_PER_TICK);
				g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Time has passed while your hunger meter is depleted! Decreased health by %d!", HUNGER_HEALTH_LOST_PER_TICK);
			}
			is_hunger_tracked_time_interval = false;
		}

		// Time passed while Ari is insane.
		if (is_sanity_tracked_time_interval)
		{
			if (AriIsInsane())
			{
				ModifyHealth(global_instance->at("__ari").m_Object, self, SANITY_HEALTH_LOST_PER_TICK);
				g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Time has passed while your sanity meter is depleted! Decreased health by %d!", SANITY_HEALTH_LOST_PER_TICK);
			}
			is_sanity_tracked_time_interval = false;
		}

		// Stamina used while Ari is hungry penalty.
		if (hunger_stamina_health_penatly < 0) // TODO: Check this is working with recent changes.
		{
			ModifyHealth(global_instance->at("__ari").m_Object, self, hunger_stamina_health_penatly);
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - You used stamina while your hunger meter is depleted! Decreased health by %d!", hunger_stamina_health_penatly);
			hunger_stamina_health_penatly = 0;
		}

		// Snapshot Ari's current position.
		if (!GameIsPaused() && snapshot_position)
		{
			RValue x;
			g_ModuleInterface->GetBuiltin("x", self, NULL_INDEX, x);
			rollback_position_x = x.m_Real;

			RValue y;
			g_ModuleInterface->GetBuiltin("y", self, NULL_INDEX, y);
			rollback_position_y = y.m_Real;

			snapshot_position = false;
		}

		// Randomly rollback Ari's position.
		if (!GameIsPaused() && AriIsInsane() && distribution_1_1000(generator) % 1000 == 0)
		{
			RValue x = rollback_position_x;
			g_ModuleInterface->SetBuiltin("x", self, NULL_INDEX, x);

			RValue y = rollback_position_y;
			g_ModuleInterface->SetBuiltin("y", self, NULL_INDEX, y);
		}

		// Blood Pact perk.
		int perk_id = perk_name_to_id_map["guardians_shield"];
		RValue perk_active = PerkActive(global_instance->at("__ari").m_Object, self, perk_id);
		if (perk_active.m_Kind == VALUE_BOOL && perk_active.m_Real == 1.0)
		{
			// Prevent HP from exceeding modified max while Blood Pact perk is active.
			RValue ari_max_health = GetMaxHealth(global_instance->at("__ari").m_Object, self);
			RValue ari_current_health = GetCurrentHealth(global_instance->at("__ari").m_Object, self);

			double current_health = ari_current_health.m_Real;
			double effective_max_health = ari_max_health.m_Real - 20;
			if (current_health > effective_max_health)
				SetHealth(global_instance->at("__ari").m_Object, self, effective_max_health);

			// Activate Priestess Shield.
			if (is_priestess_shield_tracked_time_interval)
			{
				ActivatePriestessShield(script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE][0], script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE][1]);
				is_priestess_shield_tracked_time_interval = false;
			}

			// Cancel Priestess Shield.
			if (priestess_shield_active && (!AriHasInvulnerableHits() || !ari_is_in_dungeon))
				CancelPriestessShield(script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE][0], script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE][1]);
		}
		else
		{
			// Cancel Priestess Shield.
			if (priestess_shield_active || AriHasInvulnerableHits())
				CancelPriestessShield(script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE][0], script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE][1]);

			is_priestess_shield_tracked_time_interval = false;
		}
	}
}

RValue& GmlScriptGetMinutesCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (game_is_active)
	{
		// Set the most recent ticks to 6AM after end of day.
		if (time_of_last_hunger_tick == END_OF_DAY_IN_SECONDS)
			time_of_last_hunger_tick = SIX_AM_IN_SECONDS;
		if (time_of_last_sanity_tick == END_OF_DAY_IN_SECONDS)
			time_of_last_sanity_tick = SIX_AM_IN_SECONDS;

		// Hunger ticks every 30m.
		if (Arguments[0]->m_i64 % THIRY_MINUTES_IN_SECONDS == 0 && !is_hunger_tracked_time_interval && (Arguments[0]->m_i64 - time_of_last_hunger_tick) >= THIRY_MINUTES_IN_SECONDS) {
			is_hunger_tracked_time_interval = true;
			time_of_last_hunger_tick = Arguments[0]->m_i64;

			// Adjust hunger
			ari_hunger_value += HUNGER_LOST_PER_TICK;
			if (ari_hunger_value < 0)
				ari_hunger_value = 0;
		}

		// Sanity ticks every 30s.
		if (Arguments[0]->m_i64 % THIRTY_SECONDS == 0 && !is_sanity_tracked_time_interval && (Arguments[0]->m_i64 - time_of_last_sanity_tick) >= THIRTY_SECONDS)
		{
			is_sanity_tracked_time_interval = true;
			time_of_last_sanity_tick = Arguments[0]->m_i64;

			// Adjust sanity
			if (ari_is_in_dungeon)
			{
				ari_sanity_value += SANITY_LOST_PER_TICK;
				if (ari_sanity_value < 0)
					ari_sanity_value = 0;
			}
			else if (IsNight(Arguments[0]->m_i64) && AriIsAtSanityLossLocation())
			{
				ari_sanity_value += SANITY_LOST_PER_TICK;
				if (ari_sanity_value < 0)
					ari_sanity_value = 0;
			}
			else
			{
				ari_sanity_value -= SANITY_LOST_PER_TICK; // Recover sanity.
				if (ari_sanity_value > STARTING_SANITY_VALUE)
					ari_sanity_value = STARTING_SANITY_VALUE;
			}
		}

		// Every 1m snapshot Ari's position.
		if (Arguments[0]->m_i64 % ONE_MINUTE_IN_SECONDS == 0)
		{
			snapshot_position = true;
		}

		// Blood Pact perk.
		if (!ari_is_in_dungeon)
		{
			time_of_last_priestess_shield_tick = Arguments[0]->m_i64;
			is_priestess_shield_tracked_time_interval = false;
		}
		else
		{
			if (!is_priestess_shield_tracked_time_interval && (Arguments[0]->m_i64 - time_of_last_priestess_shield_tick) >= ONE_HOUR_IN_SECONDS)
			{
				is_priestess_shield_tracked_time_interval = true;
				time_of_last_priestess_shield_tick = Arguments[0]->m_i64;
			}
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_get_minutes"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptModifyHealthCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	// Health was gained by some recovery source (item, fountain, etc).
	if (Arguments[0]->m_Real >= 0)
	{
		RValue held_item_name = ItemIdToString(Self, Other, held_item_id);
		if (held_item_name.m_Kind == VALUE_STRING)
		{
			std::string held_item_name_string = held_item_name.AsString().data();

			// These are special items used by the mod for restoring mod-specific values.
			if (held_item_name_string.compare("confiscated_coffee") == 0 || held_item_name_string.compare("lurid_colored_drink") == 0)
			{
				if (held_item_name_string.compare("confiscated_coffee") == 0)
				{
					ari_hunger_value += 35;
					if (ari_hunger_value > 100)
						ari_hunger_value = 100;
				}

				if (held_item_name_string.compare("lurid_colored_drink") == 0)
				{
					ari_sanity_value += 50;
					if (ari_sanity_value > 100)
						ari_sanity_value = 100;
				}
			}
			// Any other items in the game.
			else
			{
				int hunger_modifier = 0;
				if (recipe_name_to_stars_map.count(held_item_name_string) > 0)
				{
					hunger_modifier = 2 * (recipe_name_to_stars_map[held_item_name_string] * 10); // Testing 2x multiplier for balancing.
				}
				else if (item_name_to_restoration_map.count(held_item_name_string) > 0)
				{
					hunger_modifier = 2 * item_name_to_restoration_map[held_item_name_string]; // Testing 2x multiplier for balancing.
				}

				if (hunger_modifier > 0)
				{
					ari_hunger_value += hunger_modifier;
					if (ari_hunger_value > 100)
						ari_hunger_value = 100;

					g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Increased hunger meter by %d!", hunger_modifier);
				}
			}
		}
	}

	// Ari lost health in the dungeon.
	if (Arguments[0]->m_Real < 0 && ari_is_in_dungeon)
	{
		ari_sanity_value += Arguments[0]->m_Real;
		if (ari_sanity_value < 0) 
			ari_sanity_value = 0;

		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Ari took damage in the dungeon! Decreased sanity meter by %d!", static_cast<int>(Arguments[0]->m_Real));
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_modify_health@Ari@Ari"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptModifyStaminaCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (Arguments[0]->m_Real >= 0) {
		RValue held_item_name = ItemIdToString(Self, Other, held_item_id);
		if (held_item_name.m_Kind == VALUE_STRING)
		{
			std::string item_name_string = held_item_name.AsString().data();

			// The item is NOT a cooked dish or other edible.
			if (recipe_name_to_stars_map.count(item_name_string) <= 0 && item_name_to_restoration_map.count(item_name_string) <= 0)
			{
				int modifier = 2 * Arguments[0]->m_Real; // Testing 2x multiplier for balancing.
				ari_hunger_value += modifier;
				if (ari_hunger_value > 100)
					ari_hunger_value = 100;

				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[DontStarve] - Item not in lookup dictionaries: (%d) %s", held_item_id, item_name_string.c_str());
				g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Increased hunger meter by %d!", modifier);
			}
			// The item IS a cooked dish or other edible.
			else
			{
				auto disabled_item = std::find(std::begin(IGNORED_ITEMS), std::end(IGNORED_ITEMS), item_name_string);
				if (disabled_item == std::end(IGNORED_ITEMS))
				{
					UpdateFoodQueue(item_name_string);
					double food_penalty = GetFoodPenalty(item_name_string, 0);
					double penalty_multiplier = 1.0 - food_penalty;
					double result = trunc(penalty_multiplier * Arguments[0]->m_Real);
					if (result == 0)
						result = 1.0; // Minimum stamina after penalty
					Arguments[0]->m_Real = result;
				}
			}
		}
		else
		{
			int modifier = 2 * Arguments[0]->m_Real; // Testing 2x multiplier for balancing.
			ari_hunger_value += modifier;
			if (ari_hunger_value > 100)
				ari_hunger_value = 100;

			g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Increased hunger meter by %d!", modifier);
		}
	}
	else {
		int new_hunger_value = ari_hunger_value + Arguments[0]->m_Real;
		if (new_hunger_value < 0) {
			ari_hunger_value = 0;
			hunger_stamina_health_penatly += new_hunger_value;
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Ari is hungry! Using health in place of stamina.");
		}
		else {
			ari_hunger_value = new_hunger_value;
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Decreased hunger meter by %d!", static_cast<int>(Arguments[0]->m_Real));
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_modify_stamina@Ari@Ari"));
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

	// If we should be drawing the HUD.
	if (game_is_active && !GameIsPaused())
	{
		int y_health_bar_offset = 100;
		int sanity_bar_offset = 50;

		// Hunger Bar Icon
		RValue hunger_bar_icon_sprite_index = g_ModuleInterface->CallBuiltin(
			"asset_get_index", {
				"spr_ui_hud_hunger_bar_icon"
			}
		);

		g_ModuleInterface->CallBuiltin(
			"draw_sprite", {
				hunger_bar_icon_sprite_index, 1, 10, (122 + y_health_bar_offset)
			}
		);

		// Hunger Bar (Fill)
		g_ModuleInterface->CallBuiltin(
			"draw_set_color", {
			 4235519  // c_orange
			}
		);

		int x1 = 50 + 9;
		int y1 = 115 + 5 + y_health_bar_offset;
		int x2 = x1 + ari_hunger_value * 2;
		int y2 = 115 + 40 + y_health_bar_offset;
		g_ModuleInterface->CallBuiltin(
			"draw_rectangle", {
				x1, y1, x2, y2, false
			}
		);

		// Hunger Bar (Black)
		g_ModuleInterface->CallBuiltin(
			"draw_set_color", {
			 0  // c_black
			}
		);

		int _x1 = x2 + 1;
		int _y1 = 115 + 5 + y_health_bar_offset;
		int _x2 = _x1 + ((100 - ari_hunger_value) * 2);
		int _y2 = 115 + 40 + y_health_bar_offset;
		g_ModuleInterface->CallBuiltin(
			"draw_rectangle", {
				_x1, _y1, _x2, _y2, false
			}
		);

		// Hunger Bar (Border)
		RValue hunger_bar_sprite_index = g_ModuleInterface->CallBuiltin(
			"asset_get_index", {
				"spr_ui_hud_hunger_bar"
			}
		);

		g_ModuleInterface->CallBuiltin(
			"draw_sprite", {
				hunger_bar_sprite_index, 1, 50, (115 + y_health_bar_offset)
			}
		);

		// Hunger Bar Label
		g_ModuleInterface->CallBuiltin(
			"draw_set_color", {
			 16777215  // c_white
			}
		);

		g_ModuleInterface->CallBuiltin(
			"draw_set_font",
			{
				8 //font
			}
		);

		g_ModuleInterface->CallBuiltin(
			"draw_text_transformed", {
				140, (125 + y_health_bar_offset), std::to_string(ari_hunger_value) + "%", 3, 3, 0
			}
		);

		// Sanity Bar Icon
		RValue sanity_bar_icon_sprite_index = g_ModuleInterface->CallBuiltin(
			"asset_get_index", {
				"spr_ui_hud_sanity_bar_icon"
			}
		);

		g_ModuleInterface->CallBuiltin(
			"draw_sprite", {
				sanity_bar_icon_sprite_index, 1, 10, (122 + y_health_bar_offset + sanity_bar_offset)
			}
		);
		
		// Sanity Bar (Fill)
		g_ModuleInterface->CallBuiltin(
			"draw_set_color", {
			 8388736  // c_purple
			}
		);

		x1 = 50 + 9;
		y1 = 115 + 5 + y_health_bar_offset + sanity_bar_offset;
		x2 = x1 + ari_sanity_value * 2;
		y2 = 115 + 40 + y_health_bar_offset + sanity_bar_offset;

		g_ModuleInterface->CallBuiltin(
			"draw_rectangle", {
				x1, y1, x2, y2, false
			}
		);

		// Sanity Bar (Black)
		g_ModuleInterface->CallBuiltin(
			"draw_set_color", {
			 0  // c_black
			}
		);

		_x1 = x2 + 1;
		_y1 = 115 + 5 + y_health_bar_offset + sanity_bar_offset;
		_x2 = _x1 + ((100 - ari_sanity_value) * 2);
		_y2 = 115 + 40 + y_health_bar_offset + sanity_bar_offset;
		g_ModuleInterface->CallBuiltin(
			"draw_rectangle", {
				_x1, _y1, _x2, _y2, false
			}
		);

		// Sanity Bar (Border)
		RValue sanity_bar_sprite_index = g_ModuleInterface->CallBuiltin(
			"asset_get_index", {
				"spr_ui_hud_hunger_bar"
			}
		);

		g_ModuleInterface->CallBuiltin(
			"draw_sprite", {
				sanity_bar_sprite_index, 1, 50, (115 + y_health_bar_offset + sanity_bar_offset)
			}
		);

		// Sanity Bar Label
		g_ModuleInterface->CallBuiltin(
			"draw_set_color", {
			 16777215  // c_white
			}
		);

		g_ModuleInterface->CallBuiltin(
			"draw_set_font",
			{
				8 //font
			}
		);

		g_ModuleInterface->CallBuiltin(
			"draw_text_transformed", {
				140, (125 + y_health_bar_offset + sanity_bar_offset), std::to_string(ari_sanity_value) + "%", 3, 3, 0
			}
		);
	}

	// Random Noise (Insanity Effect)
	if (game_is_active && ari_sanity_value <= 30 && frame_counter == 0)
	{
		// Draw semi-transparent overlay
		g_ModuleInterface->CallBuiltin(
			"draw_set_alpha",
			{ 0.7 }
		);

		g_ModuleInterface->CallBuiltin(
			"draw_set_color", {
				4210752
			}
		);

		g_ModuleInterface->CallBuiltin(
			"draw_rectangle", 
			{ 0, 0, window_width, window_height, false }
		);

		// Draw static noise
		g_ModuleInterface->CallBuiltin(
			"draw_set_alpha",
			{ 1.0 }
		);

		g_ModuleInterface->CallBuiltin(
			"draw_set_color", {
			 0  // c_black
			}
		);

		for (size_t x = 0; x < noise_masks[current_mask].size(); x++)
		{
			g_ModuleInterface->CallBuiltin(
				"draw_rectangle", {
					noise_masks[current_mask][x][0] - 2, noise_masks[current_mask][x][1] - 2, noise_masks[current_mask][x][0] + 2, noise_masks[current_mask][x][1] + 2, false
				}
			);
		}

		current_mask++;
		if (current_mask == total_noise_masks)
			current_mask = 0;
	}

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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_held_item@Ari@Ari"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (Result.m_Kind != VALUE_UNDEFINED)
	{
		if (held_item_id != Result.at("item_id").m_i64)
		{
			held_item_id = Result.at("item_id").m_i64;
		}
	}

	return Result;
}

RValue& GmlScriptUseItemCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (held_item_id >= 0)
	{
		RValue item_name = ItemIdToString(Self, Other, held_item_id);
		if (item_name.m_Kind == VALUE_STRING)
		{
			std::string item_name_string = item_name.AsString().data();
			if (recipe_name_to_stars_map.count(item_name_string) > 0 || item_name_to_restoration_map.count(item_name_string) > 0)
			{
				if (item_name_to_original_stamina_recovery_map.count(item_name_string) > 0)
				{
					double stamina_recovery = item_name_to_original_stamina_recovery_map[item_name_string];
					SetItemStaminaModifier(held_item_id, stamina_recovery);
				}
			}
		}
	}
	
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_use_item"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

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
	snapshot_position = true;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_on_room_start@WeatherManager@Weather"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptPlayConversationCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	// Cthulu Speak (Insanity Effect)
	if (ari_sanity_value <= 15)
	{
		int random = distribution_1_25(generator);
		std::string conversation_name = "Conversations/Mods/DontStarve/Cthuvian/" + std::to_string(random);
		RValue custom_dialog = conversation_name;
		RValue* custom_dialog_ptr = &custom_dialog;
		Arguments[0] = custom_dialog_ptr;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_play_text@TextboxMenu@TextboxMenu"));
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
	if (run_once)
	{
		// Load items.
		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		RValue __item_data = global_instance->at("__item_data");

		size_t array_length;
		g_ModuleInterface->GetArraySize(__item_data, array_length);

		for (size_t i = 0; i < array_length; i++)
		{
			RValue* array_element;
			g_ModuleInterface->GetArrayEntry(__item_data, i, array_element);

			RValue name_key = array_element->at("name_key");
			if (name_key.m_Kind != VALUE_NULL && name_key.m_Kind != VALUE_UNDEFINED && name_key.m_Kind != VALUE_UNSET)
			{
				RValue recipe_key = array_element->at("recipe_key");

				if (strstr(name_key.AsString().data(), "cooked_dishes"))
				{
					RValue recipe = array_element->at("recipe");
					if (recipe.m_Kind != VALUE_NULL && recipe.m_Kind != VALUE_UNDEFINED && recipe.m_Kind != VALUE_UNSET)
					{
						if (recipe_name_to_stars_map.count(recipe_key.AsString().data()) <= 0)
						{
							RValue stars = array_element->at("stars");
							if (stars.m_Kind != VALUE_NULL && stars.m_Kind != VALUE_UNDEFINED && stars.m_Kind != VALUE_UNSET)
								recipe_name_to_stars_map[recipe_key.AsString().data()] = stars.m_Real;
							else
							{
								g_ModuleInterface->Print(CM_LIGHTYELLOW, "[DontStarve %s] - Missing \"stars\" data for recipe: %s", recipe_key.AsString().data());
								recipe_name_to_stars_map[recipe_key.AsString().data()] = 1;
							}
						}
					}

					RValue stamina_modifier = array_element->at("stamina_modifier");
					if (stamina_modifier.m_Kind == VALUE_REAL)
						item_name_to_original_stamina_recovery_map[recipe_key.AsString().data()] = stamina_modifier.m_Real;
				}
				else
				{
					RValue edible = array_element->at("edible");
					if (edible.m_Kind == VALUE_BOOL && edible.m_Real == 1.0)
					{
						if (item_name_to_restoration_map.count(recipe_key.AsString().data()) <= 0)
						{
							RValue stamina_modifier = array_element->at("stamina_modifier");
							item_name_to_restoration_map[recipe_key.AsString().data()] = stamina_modifier.m_Real;
						}

						RValue stamina_modifier = array_element->at("stamina_modifier");
						if (stamina_modifier.m_Kind == VALUE_REAL)
							item_name_to_original_stamina_recovery_map[recipe_key.AsString().data()] = stamina_modifier.m_Real;
					}
				}
			}
		}

		// Load perks.
		RValue perks = global_instance->at("__perk__");
		g_ModuleInterface->GetArraySize(perks, array_length);
		for (size_t i = 0; i < array_length; i++)
		{
			RValue* array_element;
			g_ModuleInterface->GetArrayEntry(perks, i, array_element);

			perk_name_to_id_map[array_element->AsString().data()] = i;
		}

		// Load status effects.
		RValue status_effects = global_instance->at("__status_effect_id__");
		g_ModuleInterface->GetArraySize(status_effects, array_length);
		for (size_t i = 0; i < array_length; i++)
		{
			RValue* array_element;
			g_ModuleInterface->GetArrayEntry(status_effects, i, array_element);

			status_effect_name_to_id_map[array_element->AsString().data()] = i;
		}

		GenerateNoiseMasks();
		run_once = false;
	}
	else
	{
		ResetAllItemStaminaModifiers(Self, Other);
		is_hunger_tracked_time_interval = false;
		snapshot_position = false;
		rollback_position = false;
		time_of_last_hunger_tick = 0;
		time_of_last_sanity_tick = 0;
		held_item_id = -1;
		game_is_active = false;
		hunger_stamina_health_penatly = 0;
		ari_hunger_value = STARTING_HUNGER_VALUE;
		ari_sanity_value = STARTING_SANITY_VALUE;
		ari_current_location = "";
		rollback_position_x = -1;
		rollback_position_y = -1;
		food_queue = {};
		localized_item_name = "";
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

RValue& GmlScriptGetWeatherCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (!game_is_active)
	{
		game_is_active = true;

		if (!MailExists(DONT_STARVE_INTRODUCTION_LETTER))
			SendMail(DONT_STARVE_INTRODUCTION_LETTER);
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_get_weather@WeatherManager@Weather"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptSetTimeCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (time_of_last_hunger_tick == 0)
		time_of_last_hunger_tick = Arguments[0]->m_i64;
	if (time_of_last_sanity_tick == 0)
		time_of_last_sanity_tick = Arguments[0]->m_i64;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_set_time@Clock@Clock"));
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
	is_hunger_tracked_time_interval = false;
	snapshot_position = false;
	rollback_position = false;
	time_of_last_hunger_tick = 0;
	time_of_last_sanity_tick = 0;
	held_item_id = -1;
	game_is_active = false;
	ari_sanity_value = STARTING_SANITY_VALUE;
	ari_current_location = "";
	rollback_position_x = -1;
	rollback_position_y = -1;

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

	if (game_is_active)
		if (Result.m_Kind == VALUE_STRING)
			ari_current_location = Result.AsString().data();

	return Result;
}

RValue& GmlScriptIsDungeonRoomCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_is_dungeon_room"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active && Result.m_Kind == VALUE_BOOL)
	{
		if (Result.m_Real == 0)
			ari_is_in_dungeon = false;
		else
			ari_is_in_dungeon = true;
	}

	return Result;
}

RValue& GmlScriptGetDisplayNameCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_get_display_name@anon@1883@LiveItem@LiveItem"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	localized_item_name = Result.AsString().data();
	return Result;
}

RValue& GmlScriptGetDisplayDescriptionCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_get_display_description@anon@2798@LiveItem@LiveItem"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (localized_item_name.size() > 0)
	{
		if (localized_item_name_to_internal_name_map.count(localized_item_name) > 0)
		{
			// Add how many have been recently eaten to the item description.
			std::string internal_item_name = localized_item_name_to_internal_name_map[localized_item_name];
			int occurrences = GetFoodQueueOccurrences(internal_item_name);
			if (occurrences > 0)
			{
				std::string new_description = "Recently Eaten: " + std::to_string(occurrences) + "\n\n" + Result.AsString().data();
				Result = new_description;
			}

			// Modify the item's stamina recovery.
			if (item_name_to_original_stamina_recovery_map.count(internal_item_name) > 0)
			{
				double item_original_stamina_recovery = item_name_to_original_stamina_recovery_map[internal_item_name];

				RValue item_id = StringToItemId(Self, Other, internal_item_name);
				if (item_id.m_Kind == VALUE_INT64)
				{
					double food_penalty = GetFoodPenalty(internal_item_name, 1);
					double penalty_multiplier = 1.0 - food_penalty;
					double adjusted_stamina_recovery = trunc(penalty_multiplier * item_original_stamina_recovery);
					if (adjusted_stamina_recovery == 0)
						adjusted_stamina_recovery = 1.0;

					SetItemStaminaModifier(item_id.m_i64, adjusted_stamina_recovery);
				}
			}
		}
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

		// Load items.
		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		RValue __item_data = global_instance->at("__item_data");

		size_t array_length;
		g_ModuleInterface->GetArraySize(__item_data, array_length);

		for (size_t i = 0; i < array_length; i++)
		{
			RValue* array_element;
			g_ModuleInterface->GetArrayEntry(__item_data, i, array_element);

			RValue name_key = array_element->at("name_key");
			if (name_key.m_Kind != VALUE_NULL && name_key.m_Kind != VALUE_UNDEFINED && name_key.m_Kind != VALUE_UNSET)
			{
				RValue recipe_key = array_element->at("recipe_key");

				// Localize all of the cooked dishes.
				if (strstr(name_key.AsString().data(), "cooked_dishes"))
				{
					RValue localized_name = GetLocalizedString(Self, Other, name_key.AsString().data());
					localized_item_name_to_internal_name_map[localized_name.AsString().data()] = recipe_key.AsString().data();
				}
				else
				{
					// Localize all of the edible items.
					RValue edible = array_element->at("edible");
					if (edible.m_Kind == VALUE_BOOL && edible.m_Real == 1.0)
					{
						RValue localized_name = GetLocalizedString(Self, Other, name_key.AsString().data());
						localized_item_name_to_internal_name_map[localized_name.AsString().data()] = recipe_key.AsString().data();
					}
				}
			}
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_get@Localizer@Localizer"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptStatusEffectManagerDeserializeCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (script_name_to_reference_map.count(GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE) == 0)
	{
		std::vector<CInstance*> script_refs = { Self, Other };
		script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE] = script_refs;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE));
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to hook (EVENT_OBJECT_CALL)!");
	}
}

void CreateHookGmlScriptGetMinutes(AurieStatus &status)
{
	CScript* gml_script_get_minutes = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_get_minutes",
		(PVOID*)&gml_script_get_minutes
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to get script (gml_Script_get_minutes)!");
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_get_minutes",
		gml_script_get_minutes->m_Functions->m_ScriptFunction,
		GmlScriptGetMinutesCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to hook script (gml_Script_get_minutes)!");
	}
}

void CreateHookGmlScriptModifyHealth(AurieStatus& status)
{
	CScript* gml_script_modify_stamina = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_modify_health@Ari@Ari",
		(PVOID*)&gml_script_modify_stamina
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to get script (gml_Script_modify_health@Ari@Ari)!");
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_modify_health@Ari@Ari",
		gml_script_modify_stamina->m_Functions->m_ScriptFunction,
		GmlScriptModifyHealthCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to hook script (gml_Script_modify_health@Ari@Ari)!");
	}
}

void CreateHookGmlScriptModifyStamina(AurieStatus &status)
{
	CScript* gml_script_modify_stamina = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_modify_stamina@Ari@Ari",
		(PVOID*)&gml_script_modify_stamina
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to get script (gml_Script_modify_stamina@Ari@Ari)!");
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_modify_stamina@Ari@Ari",
		gml_script_modify_stamina->m_Functions->m_ScriptFunction,
		GmlScriptModifyStaminaCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to hook script (gml_Script_modify_stamina@Ari@Ari)!");
	}
}

void CreateHookGmlScriptOnDrawGui(AurieStatus &status)
{
	CScript* gml_script_on_draw_gui = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_on_draw_gui@Display@Display",
		(PVOID*)&gml_script_on_draw_gui
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Failed to get script (gml_Script_on_draw_gui@Display@Display)!");
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
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Failed to hook script (gml_Script_on_draw_gui@Display@Display)!");
	}
}

void CreateHookGmlScriptHeldItem(AurieStatus& status)
{
	CScript* gml_script_held_item = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_held_item@Ari@Ari",
		(PVOID*)&gml_script_held_item
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Failed to get script (gml_Script_held_item@Ari@Ari)!");
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_held_item@Ari@Ari",
		gml_script_held_item->m_Functions->m_ScriptFunction,
		GmlScriptHeldItemCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Failed to hook script (gml_Script_held_item@Ari@Ari)!");
	}
}

void CreateHookGmlScriptUseItem(AurieStatus& status)
{
	CScript* gml_script_use_item = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_use_item",
		(PVOID*)&gml_script_use_item
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Failed to get script (gml_Script_use_item)!");
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_use_item",
		gml_script_use_item->m_Functions->m_ScriptFunction,
		GmlScriptUseItemCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Failed to hook script (gml_Script_use_item)!");
	}
}

void CreateHookGmlScriptOnRoomStart(AurieStatus& status)
{
	CScript* gml_script_held_item = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_on_room_start@WeatherManager@Weather",
		(PVOID*)&gml_script_held_item
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Failed to get script (gml_Script_on_room_start@WeatherManager@Weather)!");
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_on_room_start@WeatherManager@Weather",
		gml_script_held_item->m_Functions->m_ScriptFunction,
		GmlScriptOnRoomStartCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Failed to hook script (gml_Script_on_room_start@WeatherManager@Weather)!");
	}
}

void CreateHookGmlScriptPlayConversation(AurieStatus& status)
{
	CScript* gml_script_play_conversation = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_play_text@TextboxMenu@TextboxMenu",
		(PVOID*)&gml_script_play_conversation
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to get script (gml_Script_play_conversation)!");
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_play_text@TextboxMenu@TextboxMenu",
		gml_script_play_conversation->m_Functions->m_ScriptFunction,
		GmlScriptPlayConversationCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to hook script (gml_Script_play_conversation)!");
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
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Failed to get script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!");
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
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Failed to hook script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!");
	}
}

void CreateHookGmlScriptGetWeather(AurieStatus& status)
{
	CScript* gml_script_get_weather = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_get_weather@WeatherManager@Weather",
		(PVOID*)&gml_script_get_weather
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Failed to get script (gml_Script_get_weather@WeatherManager@Weather)!");
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_get_weather@WeatherManager@Weather",
		gml_script_get_weather->m_Functions->m_ScriptFunction,
		GmlScriptGetWeatherCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Failed to hook script (gml_Script_get_weather@WeatherManager@Weather)!");
	}
}

void CreateHookGmlScriptSetTime(AurieStatus& status)
{
	CScript* gml_script_set_time = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_set_time@Clock@Clock",
		(PVOID*)&gml_script_set_time
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Failed to get script (gml_Script_set_time@Clock@Clock)!");
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_set_time@Clock@Clock",
		gml_script_set_time->m_Functions->m_ScriptFunction,
		GmlScriptSetTimeCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Failed to hook script gml_Script_set_time@Clock@Clock)!");
	}
}

void CreateHookGmlScriptEndDay(AurieStatus& status)
{
	CScript* gml_script = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_end_day",
		(PVOID*)&gml_script
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Failed to get script (gml_Script_end_day)!");
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_end_day",
		gml_script->m_Functions->m_ScriptFunction,
		GmlScriptEndDayCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Failed to hook script (gml_Script_end_day)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to get script (gml_Script_try_location_id_to_string)!");
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to hook script (gml_Script_try_location_id_to_string)!");
	}
}

void CreateHookGmlScriptIsDungeonRoom(AurieStatus& status)
{
	CScript* gml_script_try_location_id_to_string = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_is_dungeon_room",
		(PVOID*)&gml_script_try_location_id_to_string
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to get script (gml_Script_is_dungeon_room)!");
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_is_dungeon_room",
		gml_script_try_location_id_to_string->m_Functions->m_ScriptFunction,
		GmlScriptIsDungeonRoomCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to hook script (gml_Script_is_dungeon_room)!");
	}
}

void CreateHookGmlScriptGetDisplayName(AurieStatus& status)
{
	CScript* gml_script_get_display_description = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_get_display_name@anon@1883@LiveItem@LiveItem",
		(PVOID*)&gml_script_get_display_description
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to get script (gml_Script_get_display_name@anon@1883@LiveItem@LiveItem)!");
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_get_display_name@anon@1883@LiveItem@LiveItem",
		gml_script_get_display_description->m_Functions->m_ScriptFunction,
		GmlScriptGetDisplayNameCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to hook script (gml_Script_get_display_name@anon@1883@LiveItem@LiveItem)!");
	}
}

void CreateHookGmlScriptGetDisplayDescription(AurieStatus& status)
{
	CScript* gml_script_get_display_description = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_get_display_description@anon@2798@LiveItem@LiveItem",
		(PVOID*)&gml_script_get_display_description
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to get script (gml_Script_get_display_description@anon@2798@LiveItem@LiveItem)!");
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_get_display_description@anon@2798@LiveItem@LiveItem",
		gml_script_get_display_description->m_Functions->m_ScriptFunction,
		GmlScriptGetDisplayDescriptionCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to hook script (gml_Script_get_display_description@anon@2798@LiveItem@LiveItem)!");
	}
}

void CreateHookGmlScriptGetLocalizer(AurieStatus& status)
{
	CScript* gml_script_get_localizer = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_get@Localizer@Localizer",
		(PVOID*)&gml_script_get_localizer
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to get script (gml_Script_get@Localizer@Localizer)!");
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_get@Localizer@Localizer",
		gml_script_get_localizer->m_Functions->m_ScriptFunction,
		GmlScriptGetLocalizerCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to hook script (gml_Script_get@Localizer@Localizer)!");
	}
}

void CreateHookGmlScriptStatusEffectManagerDeserialize(AurieStatus& status)
{
	CScript* gml_script_try_despawn_stars = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE,
		(PVOID*)&gml_script_try_despawn_stars
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to get script (%s)!", GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE,
		gml_script_try_despawn_stars->m_Functions->m_ScriptFunction,
		GmlScriptStatusEffectManagerDeserializeCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to hook script (%s)!", GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE);
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

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[DontStarve] - Plugin starting...");

	CreateObjectCallback(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptGetMinutes(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptModifyHealth(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptModifyStamina(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptOnDrawGui(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptHeldItem(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptUseItem(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptOnRoomStart(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptPlayConversation(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptGetWeather(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptSetTime(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptEndDay(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptTryLocationIdToString(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptIsDungeonRoom(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptGetDisplayName(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptGetDisplayDescription(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptGetLocalizer(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	CreateHookGmlScriptStatusEffectManagerDeserialize(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Exiting due to failure on start!");
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Plugin started!");
	return AURIE_SUCCESS;
}
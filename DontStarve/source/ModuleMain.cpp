#include <map>
#include <random>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static const int SIX_AM_IN_SECONDS = 21600;
static const int FIVE_MINUTES_IN_SECONDS = 300;
static const int THIRY_MINUTES_IN_SECONDS = 1800;
static const int HUNGER_LOST_PER_TICK = -1;
static const int HEALTH_LOST_PER_TICK = -10;
static const int STARTING_HUNGER_VALUE = 100;

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_items = true;
static int ari_hunger_value = STARTING_HUNGER_VALUE;
static bool ari_is_hungry = false;
static bool is_tracked_time_interval = false;
static bool snapshot_position = false; // Indicates when to snapshot Ari's current position.
static bool rollback_position = false; // Indicates when to rollback Ari's position.
static int time_of_last_tick = 0; // TODO: Set this on file load.
static int held_item_id = -1;
static bool game_is_active = false;
static bool health_bar_visible = false;
static bool use_health_instead_of_stamina = false;
static int hunger_stamina_health_penatly = 0;

static std::map<std::string, int> item_name_to_restoration_map = {}; // Excludes cooked dishes
static std::map<std::string, int> recipe_name_to_stars_map = {}; // Only cooked dishes

// Random Noise
static std::mt19937 generator(std::random_device{}());
static std::uniform_int_distribution<int> distribution(1, 1000);
//static std::vector<std::vector<bool>> noise_masks = {};
static std::vector<std::vector<std::vector<int>>> noise_masks = {};
static int total_noise_masks = 100;
static int current_mask = 0;
static int window_width = 0;
static int window_height = 0;
static size_t frame_counter = 0;

// Position Rollback
static int rollback_position_x = -1;
static int rollback_position_y = -1;

void FrameCallback(FWFrame& FrameContext)
{
	UNREFERENCED_PARAMETER(FrameContext);
	frame_counter++;
	if (frame_counter == 600)
		frame_counter = 0;
}

//--------------------------------------------------------------------------
bool EnumFunction(
	IN const char* MemberName,
	IN OUT RValue* Value
)
{
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "Member Name: %s", MemberName);
	return false;
}
//--------------------------------------------------------------------------

RValue ItemIdToString(CInstance* Self, CInstance* Other, int id)
{
	CScript* gml_script_item_id_to_name = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_item_id_to_string",
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

bool GameIsPaused()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);
	RValue paused = global_instance->at("__pause_status");
	return paused.m_i64 > 0;
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

	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	// Handle health penalties (when Ari is hungry).
	if (is_tracked_time_interval || use_health_instead_of_stamina) {
		CScript* gml_script_modify_health = nullptr;
		g_ModuleInterface->GetNamedRoutinePointer(
			"gml_Script_modify_health@Ari@Ari",
			(PVOID*)&gml_script_modify_health
		);

		// Time tick when Ari is hungry.
		if (is_tracked_time_interval && ari_is_hungry) {
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Hunger meter depleted! Decreased health by %d!", HEALTH_LOST_PER_TICK);

			RValue result;
			RValue* health_penalty = new RValue(HEALTH_LOST_PER_TICK);
			
			gml_script_modify_health->m_Functions->m_ScriptFunction(
				global_instance->at("__ari").m_Object,
				self,
				result,
				1,
				{ &health_penalty }
			);
			
			delete health_penalty;
		}

		// Stamina used when Ari is hungry.
		if (use_health_instead_of_stamina) {
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Hunger meter depleted! Decreased health by %d!", hunger_stamina_health_penatly);

			RValue* health_penalty = new RValue(hunger_stamina_health_penatly);
			RValue result;
			gml_script_modify_health->m_Functions->m_ScriptFunction(
				global_instance->at("__ari").m_Object,
				self,
				result,
				1,
				{ &health_penalty }
			);

			delete health_penalty;
			hunger_stamina_health_penatly = 0;
		}

		use_health_instead_of_stamina = false;
		is_tracked_time_interval = false;
	}

	// Get Ari maximum health.
	CScript* gml_script_modify_health = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_get_max_health@Ari@Ari",
		(PVOID*)&gml_script_modify_health
	);

	RValue ari_max_health;
	gml_script_modify_health->m_Functions->m_ScriptFunction(
		global_instance->at("__ari").m_Object,
		self,
		ari_max_health,
		0,
		nullptr
	);

	// Get Ari current health.
	CScript* gml_script_get_health = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_get_health@Ari@Ari",
		(PVOID*)&gml_script_get_health
	);

	RValue ari_current_health;
	gml_script_get_health->m_Functions->m_ScriptFunction(
		global_instance->at("__ari").m_Object,
		self,
		ari_current_health,
		0,
		nullptr
	);

	// Flag when the health bar is visible.
	if (ari_current_health.m_Real < ari_max_health.m_Real) // TODO: Add OR condition for if Ari is in the dungeon
		health_bar_visible = true;
	else
		health_bar_visible = false;

	// Randomly rollback Ari's position.
	if (!GameIsPaused() && distribution(generator) % 1000 == 0)
	{
		RValue x = rollback_position_x;
		g_ModuleInterface->SetBuiltin("x", self, NULL_INDEX, x);

		RValue y = rollback_position_y;
		g_ModuleInterface->SetBuiltin("y", self, NULL_INDEX, y);
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
}

RValue& GmlScriptGetMinutesCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_get_minutes"));

	// Set the most recent tick to 6AM on file load.
	if (game_is_active && time_of_last_tick == 0) {
		time_of_last_tick = SIX_AM_IN_SECONDS;
	}

	// If the current time tick is a 30m interval.
	if (Arguments[0]->m_i64 % THIRY_MINUTES_IN_SECONDS == 0 && !is_tracked_time_interval && (Arguments[0]->m_i64 - time_of_last_tick) >= THIRY_MINUTES_IN_SECONDS) {
		is_tracked_time_interval = true;
		time_of_last_tick = Arguments[0]->m_i64;
		
		ari_hunger_value += HUNGER_LOST_PER_TICK;
		if (ari_hunger_value > 0) {
			ari_is_hungry = false;
		}
		else {
			ari_is_hungry = true;
			ari_hunger_value = 0;
		}
	}

	// Every 5m snapshot Ari's position.
	if (Arguments[0]->m_i64 % FIVE_MINUTES_IN_SECONDS == 0)
	{
		snapshot_position = true;
	}

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
	RValue held_item_name = ItemIdToString(Self, Other, held_item_id);
	if (held_item_name.m_Kind == VALUE_STRING)
	{
		int hunger_modifier = 0;
		if (recipe_name_to_stars_map.count(held_item_name.AsString().data()) > 0)
		{
			hunger_modifier = 2 * (recipe_name_to_stars_map[held_item_name.AsString().data()] * 10); // Testing 2x multiplier for balancing.
		}
		else if (item_name_to_restoration_map.count(held_item_name.AsString().data()) > 0)
		{
			hunger_modifier = item_name_to_restoration_map[held_item_name.AsString().data()];
		}
		//else
		//{
		//	hunger_modifier = 2 * Arguments[0]->m_Real; // Testing 2x multiplier for balancing.
		//	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[DontStarve] - Item not in lookup dictionaries: (%d) %s", held_item_id, held_item_name.AsString().data());
		//}

		if (hunger_modifier > 0)
		{
			ari_hunger_value += hunger_modifier;
			if (ari_hunger_value > 100) {
				ari_hunger_value = 100;
			}
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Increased hunger meter by %d!", hunger_modifier);
		}
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
			// The item is NOT a cooked dish or other edible.
			if (recipe_name_to_stars_map.count(held_item_name.AsString().data()) <= 0 && item_name_to_restoration_map.count(held_item_name.AsString().data()) <= 0)
			{
				int modifier = 2 * Arguments[0]->m_Real; // Testing 2x multiplier for balancing.
				ari_hunger_value += modifier;
				if (ari_hunger_value > 100) {
					ari_hunger_value = 100;
				}
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[DontStarve] - Item not in lookup dictionaries: (%d) %s", held_item_id, held_item_name.AsString().data());
				g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Increased hunger meter by %d!", modifier);
			}
		}
		else
		{
			int modifier = 2 * Arguments[0]->m_Real; // Testing 2x multiplier for balancing.
			ari_hunger_value += modifier;
			if (ari_hunger_value > 100) {
				ari_hunger_value = 100;
			}
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Increased hunger meter by %d!", modifier);
		}
	}
	else {
		int new_hunger_value = ari_hunger_value + Arguments[0]->m_Real;
		if (new_hunger_value < 0) {
			use_health_instead_of_stamina = true;
			hunger_stamina_health_penatly += new_hunger_value;
			ari_hunger_value = 0;
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

	// TODO: Look into using Archie's code to detect a sprite's X,Y position and offset to figure out position and size for hunger meter and icon.

	// Don't let hunger meter fall below 0.
	if (ari_hunger_value < 0)
	{
		ari_hunger_value = 0;
	}

	// If we should be drawing the HUD.
	if (game_is_active && !GameIsPaused())
	{
		int y_health_bar_offset = 0;
		if (health_bar_visible)
		{
			y_health_bar_offset = 50;
		}

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

		//g_ModuleInterface->Print(CM_AQUA, "[DontStarve] - Orange Rectangle Coordinates: %d, %d, %d, %d", x1, y1, x2, y2);

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

		//g_ModuleInterface->Print(CM_AQUA, "[DontStarve] - Black Rectangle Coordinates: %d, %d, %d, %d", _x1, _y1, _x2, _y2);

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
				140, (118 + y_health_bar_offset), std::to_string(ari_hunger_value) + "%", 3, 3, 0
			}
		);



	}

	// Random Noise
	if (game_is_active && frame_counter == 0)
	{
		// Draw semi-transparent overlay
		g_ModuleInterface->CallBuiltin(
			"draw_set_alpha",
			{ 0.5 }
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


		// GENERATE RANDOM NOISE
		//RValue window_width = g_ModuleInterface->CallBuiltin(
		//	"window_get_width",
		//	{}
		//);

		//RValue window_height = g_ModuleInterface->CallBuiltin(
		//	"window_get_height",
		//	{}
		//);

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
			//g_ModuleInterface->CallBuiltin(
			//	"draw_point",
			//	{ noise_masks[current_mask][x][0], noise_masks[current_mask][x][1] }
			//);

			g_ModuleInterface->CallBuiltin(
				"draw_rectangle", {
					noise_masks[current_mask][x][0] - 2, noise_masks[current_mask][x][1] - 2, noise_masks[current_mask][x][0] + 2, noise_masks[current_mask][x][1] + 2, false
				}
			);
		}

		//int count = 0;
		//for (int i = 0; i < window_width; i++)
		//{
		//	for (int j = 0; j < window_height; j++)
		//	{
		//		if (noise_masks[current_mask][count])
		//		{
		//			g_ModuleInterface->CallBuiltin(
		//				"draw_point",
		//				{ i, j }
		//			);
		//		}

		//		count += 1;
		//	}
		//}

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

RValue& GmlScriptInventoryRemoveCallback(
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

	// Function: gml_Script_pop@InventorySlot@Inventory
	// Result: VALUE_OBJECT
	/*
		Member Name: auto_use
		Member Name: cosmetic
		Member Name: gold_to_gain
		Member Name: animal_cosmetic
		Member Name: item_id
		Member Name: toString
		Member Name: inner_item
		Member Name: infusion
		Member Name: prototype
	*/
	//g_ModuleInterface->EnumInstanceMembers(Result, EnumFunction);
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
	// Override the dialog.
	RValue custom_dialog = "Conversations/Mods/DontStarve/Cthuvian/1";
	RValue* custom_dialog_ptr = &custom_dialog;
	Arguments[0] = custom_dialog_ptr;

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
	if (load_items)
	{
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
						//RValue item_id = recipe.at("item_id");
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
					}
				}
			}
		}

		// Generate noise masks.
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

		for (int num_masks = 0; num_masks < total_noise_masks; num_masks++)
		{
			//std::vector<bool> noise = {};
			std::vector<std::vector<int>> noise = {};
			for (int i = 0; i < window_width; i++)
			{
				for (int j = 0; j < window_height; j++)
				{
					if (distribution(generator) == 1000)
						noise.push_back({ i, j });
					//	noise.push_back(true);
					//else
					//	noise.push_back(false);
				}
			}
			noise_masks.push_back(noise);
		}
		//-------------------------------------------------

		load_items = false;
	}
	else
	{
		game_is_active = false;
		time_of_last_tick = 0;
		ari_hunger_value = STARTING_HUNGER_VALUE;
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
	game_is_active = true;

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
	if (time_of_last_tick == 0)
	{
		time_of_last_tick = Arguments[0]->m_i64;
	}

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
	time_of_last_tick = 0;

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

void CreateHookGmlScriptInventoryRemove(AurieStatus& status)
{
	CScript* gml_script_held_item = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_on_room_start@WeatherManager@Weather",
		(PVOID*)&gml_script_held_item
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Failed to get script (gml_Script_remove@anon@2174@__Inventory@Inventory)!");
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_on_room_start@WeatherManager@Weather",
		gml_script_held_item->m_Functions->m_ScriptFunction,
		GmlScriptInventoryRemoveCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Failed to hook script (gml_Script_remove@anon@2174@__Inventory@Inventory)!");
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

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[DontStarve] - Hello from PluginEntry!");

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DontStarve] - Failed to obtain YYTK interface!");
	}

	g_ModuleInterface->CreateCallback(
		g_ArSelfModule,
		EVENT_OBJECT_CALL,
		ObjectCallback,
		0
	);

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

	CreateHookGmlScriptInventoryRemove(status);
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

	return AURIE_SUCCESS;
}
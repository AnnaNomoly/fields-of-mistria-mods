#include <map>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static const char* const MOD_NAME = "StatueOfBoons";
static const char* const VERSION = "1.0.0";
static const char* const GML_SCRIPT_TRY_STRING_TO_ITEM_ID = "gml_Script_try_string_to_item_id";
static const char* const GML_SCRIPT_TRY_OBJECT_ID_TO_STRING = "gml_Script_try_object_id_to_string";
static const char* const GML_SCRIPT_SPAWN_BUG = "gml_Script_spawn_bug";
static const char* const GML_SCRIPT_GET_FISHING_CELEBRATION_DATA = "gml_Script_get_celebration_data_essence_exp@anon@14736@Fish@Fish";
static const char* const GML_SCRIPT_GET_DIVING_CELEBRATION_DATA = "gml_Script_get_celebration_data@anon@15509@DiveSpot@Fish";
static const char* const GML_SCRIPT_GIVE_ARI_ITEM = "gml_Script_give_item@Ari@Ari";
static const char* const GML_SCRIPT_GET_MOVE_SPEED = "gml_Script_get_move_speed@Ari@Ari";
static const char* const GML_SCRIPT_INTERACT = "gml_Script_interact";
static const char* const GML_SCRIPT_SHOW_ROOM_TITLE = "gml_Script_show_room_title";
static const char* const GML_SCRIPT_CREATE_BUG = "gml_Script_setup@gml_Object_obj_bug_Create_0"; // TODO: This script isn't needed -- gml_Script_goto_gm_room
static const char* const GML_SCRIPT_ADD_HEART_POINTS = "gml_Script_add_heart_points@Npc@Npc";
static const char* const GML_SCRIPT_MODIFY_STAMINA = "gml_Script_modify_stamina@Ari@Ari";
static const char* const GML_SCRIPT_TRY_LOCATION_ID_TO_STRING = "gml_Script_try_location_id_to_string";
static const char* const GML_SCRIPT_ON_NEW_DAY = "gml_Script_on_new_day@Ari@Ari";
static const char* const GML_SCRIPT_PLAY_TEXT = "gml_Script_play_text@TextboxMenu@TextboxMenu";
static const char* const GML_SCRIPT_PLAY_CONVERSATION = "gml_Script_play_conversation";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const std::string LOCATION_FARM = "farm";
static const std::string OBJECT_CATEGORY_BUSH = "bush";
static const std::string OBJECT_CATEGORY_CROP = "crop";
static const std::string OBJECT_CATEGORY_ROCK = "rock";
static const std::string OBJECT_CATEGORY_STUMP = "stump";
static const std::string OBJECT_CATEGORY_TREE = "tree";
static const std::string CUSTOM_ITEM_NAME = "dragon_fairy";
static const std::string CUSTOM_OBJECT_NAME = "statue_of_boons";
static const std::string STATUE_OF_BOONS_CONVERSATION_KEY = "Conversations/Mods/Statue of Boons/statue_of_boons";
static const std::string STATUE_OF_BOONS_PLACEHOLDER_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/placeholder";
static const std::string STATUE_OF_BOONS_BOON_OF_SPEED_GRANTED_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_speed/granted"; // "Boon of the Wind"
static const std::string STATUE_OF_BOONS_BOON_OF_FORAGE_GRANTED_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_forage/granted"; // "Boon of the Land"
static const std::string STATUE_OF_BOONS_BOON_OF_FISHING_GRANTED_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_fishing/granted"; // "Boon of the Sea"
static const std::string STATUE_OF_BOONS_BOON_OF_BUTTERFLY_GRANTED_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_butterfly/granted"; // "Boon of the Dragons"
static const std::string STATUE_OF_BOONS_BOON_OF_FRIENDSHIP_GRANTED_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_friendship/granted"; // "Boon of the Moon"
static const std::string STATUE_OF_BOONS_BOON_OF_STAMINA_GRANTED_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_stamina/granted"; // "Boon of the Sun"
static const std::string STATUE_OF_BOONS_BOON_OF_SPEED_ACTIVE_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_speed/active";
static const std::string STATUE_OF_BOONS_BOON_OF_FORAGE_ACTIVE_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_forage/active";
static const std::string STATUE_OF_BOONS_BOON_OF_FISHING_ACTIVE_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_fishing/active";
static const std::string STATUE_OF_BOONS_BOON_OF_BUTTERFLY_ACTIVE_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_butterfly/active";
static const std::string STATUE_OF_BOONS_BOON_OF_FRIENDSHIP_ACTIVE_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_friendship/active";
static const std::string STATUE_OF_BOONS_BOON_OF_STAMINA_ACTIVE_DIALOGUE_KEY = "Conversations/Mods/Statue of Boons/boon_of_stamina/active";
static const std::string MYSTERIOUS_BUTTERFLY_DETECTED_DIALOGUE_KEY = "Notifications/Mods/Statue of Boons/boon_of_butterfly/detected";
static const std::map<std::string, std::vector<std::vector<int>>> CUSTOM_BUG_SPAWN_LOCATIONS = {
	{"western_ruins", {{158,127},{177,98},{193, 164}}},
	{"eastern_road", {{63,69},{131,69},{31,127},{190,95},{114,144},{39,186},{111,241},{32,259}}}
};

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_on_start = true;
static bool game_is_active = false;
static bool custom_object_used = false;
static std::string ari_current_location = "";
static RValue custom_conversation_value;
static RValue* custom_conversation_value_ptr = nullptr;
static RValue custom_dialogue_value;
static RValue* custom_dialogue_value_ptr = nullptr;
static bool boon_of_speed = false;
static bool boon_of_forage = false;
static bool boon_of_fishing = false;
static bool boon_of_butterfly = false;
static bool boon_of_friendship = false;
static bool boon_of_stamina = false;
static bool modify_items_added = false;
static bool spawning_dragon_fairy = false; // Used to track when the bug is being spawned during script call stack
static int dragon_fairy_item_id = -1;
static std::map<std::string, int> object_category_to_id_map = {};
static std::vector<int> bounty_boon_objects = {};
static std::map<int, std::string> object_id_to_name_map = {};

// DEBUG VARS------------------
static int interact_count = 0;
static int bug_spawn_count = 0;
//-----------------------------

int RValueAsInt(RValue value)
{
	if (value.m_Kind == VALUE_REAL)
		return static_cast<int>(value.m_Real);
	if (value.m_Kind == VALUE_INT64)
		return static_cast<int>(value.m_i64);
	if (value.m_Kind == VALUE_INT32)
		return static_cast<int>(value.m_i32);
}

bool RValueAsBool(RValue value)
{
	if (value.m_Kind == VALUE_REAL && value.m_Real == 1)
		return true;
	if (value.m_Kind == VALUE_BOOL && value.m_Real == 1)
		return true;
	return false;
}

bool StructVariableExists(RValue the_struct, const char* variable_name)
{
	RValue struct_exists = g_ModuleInterface->CallBuiltin(
		"struct_exists",
		{ the_struct, variable_name }
	);

	return RValueAsBool(struct_exists);
}

RValue StructVariableGet(RValue the_struct, const char* variable_name)
{
	return g_ModuleInterface->CallBuiltin(
		"struct_get",
		{ the_struct, variable_name }
	);
}

RValue StructVariableSet(RValue the_struct, const char* variable_name, RValue value)
{
	return g_ModuleInterface->CallBuiltin(
		"struct_set",
		{ the_struct, variable_name, value }
	);
}

RValue TryStringToItemId(CInstance* Self, CInstance* Other, std::string item_name_str)
{
	CScript* gml_script_try_string_to_item_id = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_TRY_STRING_TO_ITEM_ID,
		(PVOID*)&gml_script_try_string_to_item_id
	);

	RValue item_id;
	RValue item_name = item_name_str;
	RValue* item_name_ptr = &item_name;

	gml_script_try_string_to_item_id->m_Functions->m_ScriptFunction(
		Self,
		Other,
		item_id,
		1,
		{ &item_name_ptr }
	);

	return item_id;
}

void LoadObjectCategories()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	size_t array_length = 0;
	RValue object_categories = global_instance->at("__object_category__");
	g_ModuleInterface->GetArraySize(object_categories, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* object_category;
		g_ModuleInterface->GetArrayEntry(object_categories, i, object_category);

		std::string object_category_str = object_category->AsString().data();
		object_category_to_id_map[object_category_str] = i;
	}
}

void LoadObjectIds(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_try_object_id_to_string = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_TRY_OBJECT_ID_TO_STRING,
		(PVOID*)&gml_script_try_object_id_to_string
	);

	for (int i = 0; i < 5000; i++)
	{
		RValue result;
		RValue object_id = i;
		RValue* object_id_ptr = &object_id;
		gml_script_try_object_id_to_string->m_Functions->m_ScriptFunction(
			Self,
			Other,
			result,
			1,
			{ &object_id_ptr }
		);

		if (result.m_Kind == VALUE_STRING)
			object_id_to_name_map[i] = result.AsString().data();
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Loaded %d objects!", MOD_NAME, VERSION, static_cast<int>(object_id_to_name_map.size()));
}

void LoadObjectItemData()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	size_t array_length = 0;
	RValue node_prototypes = global_instance->at("__node_prototypes");
	g_ModuleInterface->GetArraySize(node_prototypes, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* node_prototype;
		g_ModuleInterface->GetArrayEntry(node_prototypes, i, node_prototype);

		bool object_is_eligible = false;
		int object_id = RValueAsInt(StructVariableGet(*node_prototype, "object_id"));
		int category_id = RValueAsInt(StructVariableGet(*node_prototype, "category_id"));

		if (category_id == object_category_to_id_map[OBJECT_CATEGORY_BUSH] || category_id == object_category_to_id_map[OBJECT_CATEGORY_CROP])		
			object_is_eligible = true;
		if (category_id == object_category_to_id_map[OBJECT_CATEGORY_ROCK] || category_id == object_category_to_id_map[OBJECT_CATEGORY_STUMP] || category_id == object_category_to_id_map[OBJECT_CATEGORY_TREE])
			object_is_eligible = true;

		if (object_is_eligible)
		{
			auto it = std::find(bounty_boon_objects.begin(), bounty_boon_objects.end(), object_id);
			if (it == bounty_boon_objects.end())
				bounty_boon_objects.push_back(object_id);
		}
	}
}

void ModifyBug(CInstance* Self)
{
	//if (!StructVariableExists(Self, "__statue_of_boons__processed_bug"))
	//{
	//	if (StructVariableExists(Self, "item_id") && StructVariableExists(Self, "idle_sprite") && StructVariableExists(Self, "move_sprite"))
	//	{
	//		StructVariableSet(Self, "__statue_of_boons__processed_bug", true);
	//		StructVariableSet(Self, "item_id", dragon_fairy_item_id);

	//		RValue spr_insect_dragon_fairy_entity_idle = g_ModuleInterface->CallBuiltin(
	//			"asset_get_index", {
	//				"spr_insect_dragonfairy_entity_idle"
	//			}
	//		);
	//		StructVariableSet(Self, "idle_sprite", spr_insect_dragon_fairy_entity_idle);

	//		RValue spr_insect_dragon_fairy_entity_move = g_ModuleInterface->CallBuiltin(
	//			"asset_get_index", {
	//				"spr_insect_dragonfairy_entity_move"
	//			}
	//		);
	//		StructVariableSet(Self, "move_sprite", spr_insect_dragon_fairy_entity_move);
	//	}
	//}
}

RValue SpawnBug(CInstance* Self, CInstance* Other, int x_coord, int y_coord)
{
	CScript* gml_script_spawn_bug = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SPAWN_BUG,
		(PVOID*)&gml_script_spawn_bug
	);

	RValue result;
	RValue x = x_coord;
	RValue y = y_coord;
	RValue undefined;
	RValue* x_ptr = &x;
	RValue* y_ptr = &y;
	RValue* undefined_ptr = &undefined;
	RValue* argument_array[3] = { x_ptr, y_ptr, undefined_ptr };

	spawning_dragon_fairy = true;
	gml_script_spawn_bug->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		3,
		argument_array
	);

	return result;
}

void ResetStaticFields()
{
	static bool game_is_active = false;
	static bool custom_object_used = false;
	static std::string ari_current_location = "";
	static RValue custom_conversation_value;
	static RValue* custom_conversation_value_ptr = nullptr;
	static RValue custom_dialogue_value;
	static RValue* custom_dialogue_value_ptr = nullptr;
	static bool boon_of_speed = false;
	static bool boon_of_forage = false;
	static bool boon_of_fishing = false;
	static bool boon_of_butterfly = false;
	static bool boon_of_friendship = false;
	static bool boon_of_stamina = false;
	static bool modify_items_added = false;
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

	if (strstr(self->m_Object->m_Name, "obj_bug"))
	{
		// TODO: Add logic for if boon is active, we're in the randomly selected zone, and the bug hasn't already been spawned
		ModifyBug(self);
	}
}

RValue& GmlScriptGetFishingCelebrationDataCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if(boon_of_fishing)
		modify_items_added = true;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_FISHING_CELEBRATION_DATA));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptGetDivingCelebrationDataCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (boon_of_fishing)
		modify_items_added = true;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_DIVING_CELEBRATION_DATA));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptGiveItemCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (modify_items_added)
	{
		modify_items_added = false;
		if (ari_current_location != LOCATION_FARM)
		{
			Arguments[1]->m_Real = Arguments[1]->m_Real * 2;
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Doubled the item obtained.", MOD_NAME, VERSION);
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GIVE_ARI_ITEM));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptGetMoveSpeedCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (!game_is_active)
		game_is_active = true;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_MOVE_SPEED));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (boon_of_speed)
		Result.m_Real += 0.5;
		
	return Result; // 2.0 is default run speed, max run speed boost from gear is 20% => 2.4
}

RValue& GmlScriptInteractCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	//DEBUG----------
	if (game_is_active)
	{
		if (CUSTOM_BUG_SPAWN_LOCATIONS.count(ari_current_location) > 0)
		{
			std::vector<std::vector<int>> bug_spawn_locations = CUSTOM_BUG_SPAWN_LOCATIONS.at(ari_current_location);
			for (size_t i = 0; i < bug_spawn_locations.size(); i++)
			{
				std::vector<int> coordinates = bug_spawn_locations[i];
				SpawnBug(Self, Other, coordinates[0], coordinates[1]);
			}
		}
	}
	//---------------

	RValue object = Arguments[0]->m_Object;
	int object_id = RValueAsInt(object.at("object_id"));

	auto it = std::find(bounty_boon_objects.begin(), bounty_boon_objects.end(), object_id);
	if (it != bounty_boon_objects.end())
	{
		if(boon_of_forage)
			modify_items_added = true;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_INTERACT));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	/*
	RValue object = Arguments[0]->m_Object;
	if (StructVariableExists(object, "prototype"))
	{
		RValue prototype = StructVariableGet(object, "prototype");
		if (StructVariableExists(prototype, "object_id"))
		{
			RValue object_id = StructVariableGet(prototype, "object_id");
			int object_id_int = RValueAsInt(object_id);
			std::string object_name = object_id_to_name_map[object_id_int];
			int temp = 5;
		}
	}
	
	int object_id = RValueAsInt(object.at("object_id"));
	std::string object_name = object_id_to_name_map[object_id];
	if (object_name.find(CUSTOM_OBJECT_NAME) != std::string::npos)
		custom_object_used = true;

	int temp = 5;
	*/
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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_SHOW_ROOM_TITLE));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptCreateBugCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (spawning_dragon_fairy)
	{
		spawning_dragon_fairy = false;
		int fairy_bee_item_id = RValueAsInt(TryStringToItemId(Self, Other, "dragon_fairy")); // TODO: Testing spawning the custom bug directly
		Arguments[0]->m_i64 = fairy_bee_item_id;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_CREATE_BUG));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;  
}

RValue& GmlScriptAddHeartPointsCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_ADD_HEART_POINTS));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (boon_of_friendship)
		Arguments[0]->m_Real = ceil(1.5 * Arguments[0]->m_Real);

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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_MODIFY_STAMINA));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (boon_of_stamina)
		if (Arguments[0]->m_Real < 0)
			Arguments[0]->m_Real = 0;

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

RValue& GmlScriptOnNewDayCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_ON_NEW_DAY));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	// TODO: Reset Ari's movement speed if necessary
	//ResetStaticFields();

	return Result;
}

RValue& GmlScriptPlayTextCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	std::string conversation_name = Arguments[0]->AsString().data();
	if (conversation_name.find(STATUE_OF_BOONS_PLACEHOLDER_DIALOGUE_KEY) != std::string::npos)
	{
		// TODO: Grant boons and stuff.

		// DEBUG-----------------------------------------------------------------------------
		if (interact_count == 0)
		{
			custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_SPEED_GRANTED_DIALOGUE_KEY;
			custom_dialogue_value_ptr = &custom_dialogue_value;
			Arguments[0] = custom_dialogue_value_ptr;
		}
		if (interact_count == 1)
		{
			custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_FORAGE_GRANTED_DIALOGUE_KEY;
			custom_dialogue_value_ptr = &custom_dialogue_value;
			Arguments[0] = custom_dialogue_value_ptr;
		}
		if (interact_count == 2)
		{
			custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_FISHING_GRANTED_DIALOGUE_KEY;
			custom_dialogue_value_ptr = &custom_dialogue_value;
			Arguments[0] = custom_dialogue_value_ptr;
		}

		if (interact_count == 3)
		{
			custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_BUTTERFLY_GRANTED_DIALOGUE_KEY;
			custom_dialogue_value_ptr = &custom_dialogue_value;
			Arguments[0] = custom_dialogue_value_ptr;
		}
		if (interact_count == 4)
		{
			custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_FRIENDSHIP_GRANTED_DIALOGUE_KEY;
			custom_dialogue_value_ptr = &custom_dialogue_value;
			Arguments[0] = custom_dialogue_value_ptr;
		}
		if (interact_count == 5)
		{
			custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_STAMINA_GRANTED_DIALOGUE_KEY;
			custom_dialogue_value_ptr = &custom_dialogue_value;
			Arguments[0] = custom_dialogue_value_ptr;
		}
		if (interact_count == 6)
		{
			custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_SPEED_ACTIVE_DIALOGUE_KEY;
			custom_dialogue_value_ptr = &custom_dialogue_value;
			Arguments[0] = custom_dialogue_value_ptr;
		}
		if (interact_count == 7)
		{
			custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_FORAGE_ACTIVE_DIALOGUE_KEY;
			custom_dialogue_value_ptr = &custom_dialogue_value;
			Arguments[0] = custom_dialogue_value_ptr;
		}
		if (interact_count == 8)
		{
			custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_FISHING_ACTIVE_DIALOGUE_KEY;
			custom_dialogue_value_ptr = &custom_dialogue_value;
			Arguments[0] = custom_dialogue_value_ptr;
		}
		if (interact_count == 9)
		{
			custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_BUTTERFLY_ACTIVE_DIALOGUE_KEY;
			custom_dialogue_value_ptr = &custom_dialogue_value;
			Arguments[0] = custom_dialogue_value_ptr;
		}
		if (interact_count == 10)
		{
			custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_FRIENDSHIP_ACTIVE_DIALOGUE_KEY;
			custom_dialogue_value_ptr = &custom_dialogue_value;
			Arguments[0] = custom_dialogue_value_ptr;
		}
		if (interact_count == 11)
		{
			custom_dialogue_value = STATUE_OF_BOONS_BOON_OF_STAMINA_ACTIVE_DIALOGUE_KEY;
			custom_dialogue_value_ptr = &custom_dialogue_value;
			Arguments[0] = custom_dialogue_value_ptr;
		}
		if (interact_count == 12)
		{
			custom_dialogue_value = MYSTERIOUS_BUTTERFLY_DETECTED_DIALOGUE_KEY;
			custom_dialogue_value_ptr = &custom_dialogue_value;
			Arguments[0] = custom_dialogue_value_ptr;
		}
		interact_count++;
		if (interact_count > 12)
			interact_count = 0;
		//---------------------------------------------------------------------------------------
	}
	

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_PLAY_TEXT));
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
	if (custom_object_used)
	{
		custom_object_used = false;
		custom_conversation_value = STATUE_OF_BOONS_CONVERSATION_KEY;
		custom_conversation_value_ptr = &custom_conversation_value;
		Arguments[1] = custom_conversation_value_ptr;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_PLAY_CONVERSATION));
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
	ResetStaticFields();

	if (load_on_start)
	{
		load_on_start = false;
		dragon_fairy_item_id = RValueAsInt(TryStringToItemId(Self, Other, CUSTOM_ITEM_NAME));
		LoadObjectCategories();
		LoadObjectIds(Self, Other);
		LoadObjectItemData();

		//if (!object_id_to_name_map.empty())
		//	mod_healthy = true;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_SETUP_MAIN_SCREEN));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	// TODO

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

void CreateHookGmlScriptGetFishingCelebrationData(AurieStatus& status)
{
	CScript* gml_script_get_fishing_celebration_data = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_FISHING_CELEBRATION_DATA,
		(PVOID*)&gml_script_get_fishing_celebration_data
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_FISHING_CELEBRATION_DATA);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_FISHING_CELEBRATION_DATA,
		gml_script_get_fishing_celebration_data->m_Functions->m_ScriptFunction,
		GmlScriptGetFishingCelebrationDataCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_FISHING_CELEBRATION_DATA);
	}
}

void CreateHookGmlScriptGetDivingCelebrationData(AurieStatus& status)
{
	CScript* gml_script_get_diving_celebration_data = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_DIVING_CELEBRATION_DATA,
		(PVOID*)&gml_script_get_diving_celebration_data
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_DIVING_CELEBRATION_DATA);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_DIVING_CELEBRATION_DATA,
		gml_script_get_diving_celebration_data->m_Functions->m_ScriptFunction,
		GmlScriptGetDivingCelebrationDataCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_DIVING_CELEBRATION_DATA);
	}
}

void CreateHookGmlScriptGiveItem(AurieStatus& status)
{
	CScript* gml_script_give_item = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GIVE_ARI_ITEM,
		(PVOID*)&gml_script_give_item
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GIVE_ARI_ITEM);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GIVE_ARI_ITEM,
		gml_script_give_item->m_Functions->m_ScriptFunction,
		GmlScriptGiveItemCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GIVE_ARI_ITEM);
	}
}

void CreateHookGmlScriptGetMoveSpeed(AurieStatus& status)
{
	CScript* gml_script_get_move_speed = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_MOVE_SPEED,
		(PVOID*)&gml_script_get_move_speed
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_MOVE_SPEED);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_MOVE_SPEED,
		gml_script_get_move_speed->m_Functions->m_ScriptFunction,
		GmlScriptGetMoveSpeedCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_MOVE_SPEED);
	}
}

void CreateHookGmlScriptInteract(AurieStatus& status)
{
	CScript* gml_script_interact = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_INTERACT,
		(PVOID*)&gml_script_interact
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_INTERACT);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_INTERACT,
		gml_script_interact->m_Functions->m_ScriptFunction,
		GmlScriptInteractCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_INTERACT);
	}
}

void CreateHookGmlScriptShowRoomTitle(AurieStatus& status)
{
	CScript* gml_script_create_bug = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SHOW_ROOM_TITLE,
		(PVOID*)&gml_script_create_bug
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_SHOW_ROOM_TITLE);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_SHOW_ROOM_TITLE,
		gml_script_create_bug->m_Functions->m_ScriptFunction,
		GmlScriptShowRoomTitleCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_SHOW_ROOM_TITLE);
	}
}

void CreateHookGmlScriptCreateBug(AurieStatus& status)
{
	CScript* gml_script_create_bug = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CREATE_BUG,
		(PVOID*)&gml_script_create_bug
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CREATE_BUG);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_CREATE_BUG,
		gml_script_create_bug->m_Functions->m_ScriptFunction,
		GmlScriptCreateBugCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CREATE_BUG);
	}
}

void CreateHookGmlScriptAddHeartPoints(AurieStatus& status)
{
	CScript* gml_script_add_heart_points = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_ADD_HEART_POINTS,
		(PVOID*)&gml_script_add_heart_points
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ADD_HEART_POINTS);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_ADD_HEART_POINTS,
		gml_script_add_heart_points->m_Functions->m_ScriptFunction,
		GmlScriptAddHeartPointsCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ADD_HEART_POINTS);
	}
}

void CreateHookGmlScriptModifyStamina(AurieStatus& status)
{
	CScript* gml_script_modify_stamina = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_MODIFY_STAMINA,
		(PVOID*)&gml_script_modify_stamina
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_MODIFY_STAMINA);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_MODIFY_STAMINA,
		gml_script_modify_stamina->m_Functions->m_ScriptFunction,
		GmlScriptModifyStaminaCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_MODIFY_STAMINA);
	}
}

void CreateHookGmlScriptTryLocationIdToString(AurieStatus& status)
{
	CScript* gml_script_try_location_id_to_string = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_TRY_LOCATION_ID_TO_STRING,
		(PVOID*)&gml_script_try_location_id_to_string
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_TRY_LOCATION_ID_TO_STRING);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_TRY_LOCATION_ID_TO_STRING,
		gml_script_try_location_id_to_string->m_Functions->m_ScriptFunction,
		GmlScriptTryLocationIdToStringCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_TRY_LOCATION_ID_TO_STRING);
	}
}

void CreateHookGmlScriptOnNewDay(AurieStatus& status)
{
	CScript* gml_script_on_new_day = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_ON_NEW_DAY,
		(PVOID*)&gml_script_on_new_day
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ON_NEW_DAY);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_ON_NEW_DAY,
		gml_script_on_new_day->m_Functions->m_ScriptFunction,
		GmlScriptOnNewDayCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ON_NEW_DAY);
	}
}

void CreateHookGmlScriptPlayText(AurieStatus& status)
{
	CScript* gml_script_play_text = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_PLAY_TEXT,
		(PVOID*)&gml_script_play_text
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_PLAY_TEXT);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_PLAY_TEXT,
		gml_script_play_text->m_Functions->m_ScriptFunction,
		GmlScriptPlayTextCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_PLAY_TEXT);
	}
}

void CreateHookGmlScriptPlayConversation(AurieStatus& status)
{
	CScript* gml_script_play_conversation = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_PLAY_CONVERSATION,
		(PVOID*)&gml_script_play_conversation
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_PLAY_CONVERSATION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_PLAY_CONVERSATION,
		gml_script_play_conversation->m_Functions->m_ScriptFunction,
		GmlScriptPlayConversationCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_PLAY_CONVERSATION);
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

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[%s %s] - Plugin starting...", MOD_NAME, VERSION);

	CreateObjectCallback(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	//CreateHookGmlSlashNode(status);
	//if (!AurieSuccess(status))
	//{
	//	g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
	//	return status;
	//}

	CreateHookGmlScriptGetFishingCelebrationData(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptGetDivingCelebrationData(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptGiveItem(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptGetMoveSpeed(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptInteract(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptShowRoomTitle(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptCreateBug(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptAddHeartPoints(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptModifyStamina(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptTryLocationIdToString(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptOnNewDay(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptPlayText(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptPlayConversation(status);
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
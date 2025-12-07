#include <random>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <magic_enum/magic_enum.hpp>
#include <YYToolkit/YYTK_Shared.hpp> // YYTK v4
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

// C++ 17 Boost-style hash combine
struct pair_hash {
	std::size_t operator()(const std::pair<int, int>& p) const noexcept {
		std::size_t seed = 0;

		auto h1 = std::hash<int>{}(p.first);
		seed ^= h1 + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);

		auto h2 = std::hash<int>{}(p.second);
		seed ^= h2 + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);

		return seed;
	}
};

static const char* const MOD_NAME = "DeepDungeon";
static const char* const VERSION = "0.2.0";
static const char* const GML_SCRIPT_GET_LOCALIZER = "gml_Script_get@Localizer@Localizer";
static const char* const GML_SCRIPT_SPAWN_LADDER = "gml_Script_spawn_ladder@DungeonRunner@DungeonRunner";
static const char* const GML_SCRIPT_CREATE_NOTIFICATION = "gml_Script_create_notification";
static const char* const GML_SCRIPT_PLAY_CONVERSATION = "gml_Script_play_conversation";
static const char* const GML_SCRIPT_SPAWN_TUTORIAL = "gml_Script_spawn_tutorial";
static const char* const GML_SCRIPT_UPDATE_TOOLBAR_MENU = "gml_Script_update@ToolbarMenu@ToolbarMenu";
static const char* const GML_SCRIPT_ENTER_DUNGEON = "gml_Script_enter_dungeon";
static const char* const GML_SCRIPT_CANCEL_STATUS_EFFECT = "gml_Script_cancel@StatusEffectManager@StatusEffectManager";
static const char* const GML_SCRIPT_REGISTER_STATUS_EFFECT = "gml_Script_register@StatusEffectManager@StatusEffectManager";
static const char* const GML_SCRIPT_GET_MAX_HEALTH = "gml_Script_get_max_health@Ari@Ari";
static const char* const GML_SCRIPT_GET_HEALTH = "gml_Script_get_health@Ari@Ari";
static const char* const GML_SCRIPT_SET_HEALTH = "gml_Script_set_health@Ari@Ari";
static const char* const GML_SCRIPT_MODIFY_HEALTH = "gml_Script_modify_health@Ari@Ari";
static const char* const GML_SCRIPT_GET_STAMINA = "gml_Script_get_stamina@Ari@Ari";
static const char* const GML_SCRIPT_MODIFY_STAMINA = "gml_Script_modify_stamina@Ari@Ari";
static const char* const GML_SCRIPT_GET_MANA = "gml_Script_get_mana@Ari@Ari";
static const char* const GML_SCRIPT_MODIFY_MANA = "gml_Script_modify_mana@Ari@Ari";
static const char* const GML_SCRIPT_CAST_SPELL = "gml_Script_cast_spell";
static const char* const GML_SCRIPT_SPAWN_MONSTER = "gml_Script_spawn_monster";
static const char* const GML_SCRIPT_CAN_CAST_SPELL = "gml_Script_can_cast_spell";
static const char* const GML_SCRIPT_GET_MOVE_SPEED = "gml_Script_get_move_speed@Ari@Ari";
static const char* const GML_SCRIPT_DAMAGE = "gml_Script_damage@gml_Object_obj_damage_receiver_Create_0";
static const char* const GML_SCRIPT_INTERACT = "gml_Script_interact";
static const char* const GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE = "gml_Script_deserialize@StatusEffectManager@StatusEffectManager";
static const char* const GML_SCRIPT_TAKE_PRESS = "gml_Script_take_press@Input@Input";
static const char* const GML_SCRIPT_CHECK_VALUE = "gml_Script_check_value@Input@Input";
static const char* const GML_SCRIPT_ATTEMPT_INTERACT = "gml_Script_attempt_interact@gml_Object_par_interactable_Create_0";
static const char* const GML_SCRIPT_USE_ITEM = "gml_Script_use_item";
static const char* const GML_SCRIPT_HELD_ITEM = "gml_Script_held_item@Ari@Ari";
static const char* const GML_SCRIPT_DROP_ITEM = "gml_Script_drop_item";
static const char* const GML_SCRIPT_GET_MINUTES = "gml_Script_update@Clock@Clock";
static const char* const GML_SCRIPT_PLAY_TEXT = "gml_Script_play_text@TextboxMenu@TextboxMenu";
static const char* const GML_SCRIPT_GET_WEATHER = "gml_Script_get_weather@WeatherManager@Weather";
static const char* const GML_SCRIPT_TRY_LOCATION_ID_TO_STRING = "gml_Script_try_location_id_to_string";
static const char* const GML_SCRIPT_ON_DUNGEON_ROOM_START = "gml_Script_on_room_start@DungeonRunner@DungeonRunner";
static const char* const GML_SCRIPT_GO_TO_ROOM = "gml_Script_goto_gm_room";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const char* const GML_SCRIPT_GET_EQUIPMENT_BONUS_FROM = "gml_Script_get_equipment_bonus_from@Ari@Ari";
static const char* const GML_SCRIPT_HUD_SHOULD_SHOW = "gml_Script_hud_should_show";
static const char* const GML_SCRIPT_ON_DRAW_GUI = "gml_Script_on_draw_gui@Display@Display";
static const char* const GML_SCRIPT_DISPLAY_RESIZE = "gml_Script_resize_amount@Display@Display";
static const char* const GML_SCRIPT_GET_ITEM_UI_ICON = "gml_Script_get_ui_icon@anon@4053@LiveItem@LiveItem";
static const char* const GML_SCRIPT_CREATE_ITEM_PROTOTYPES = "gml_Script_create_item_prototypes";
static const char* const GML_SCRIPT_STOP_ROOM_TITLE = "gml_Script_stop_room_title";
static const char* const GML_SCRIPT_DESERIALIZE_LIVE_ITEM = "gml_Script_deserialize_live_item";
static const char* const GML_SCRIPT_GET_TREASURE_FROM_DISTRIBUTION = "gml_Script_get_treasure_from_distribution";
static const std::string SIGIL_OF_ALTERATION_NAME = "sigil_of_alteration";
static const std::string SIGIL_OF_CONCEALMENT_NAME = "sigil_of_concealment";
static const std::string SIGIL_OF_FORTIFICATION_NAME = "sigil_of_fortification";
static const std::string SIGIL_OF_FORTUNE_NAME = "sigil_of_fortune";
static const std::string SIGIL_OF_PROTECTION_NAME = "sigil_of_protection";
static const std::string SIGIL_OF_RAGE_NAME = "sigil_of_rage";
static const std::string SIGIL_OF_REDEMPTION_NAME = "sigil_of_redemption";
static const std::string SIGIL_OF_SAFETY_NAME = "sigil_of_safety";
static const std::string SIGIL_OF_SERENITY_NAME = "sigil_of_serenity";
static const std::string SIGIL_OF_SILENCE_NAME = "sigil_of_silence";
static const std::string SIGIL_OF_STRENGTH_NAME = "sigil_of_strength";
static const std::string SIGIL_OF_TEMPTATION_NAME = "sigil_of_temptation";
static const std::string SUSTAINING_POTION_NAME = "sustaining_potion";
static const std::string HEALTH_SALVE_NAME = "health_salve";
static const std::string STAMINA_SALVE_NAME = "stamina_salve";
static const std::string MANA_SALVE_NAME = "mana_salve";
static const std::string MISTPOOL_SWORD_NAME = "sword_scrap_metal";
static const std::string MISTPOOL_HELMET_NAME = "scrap_metal_helmet";
static const std::string MISTPOOL_CHESTPIECE_NAME = "scrap_metal_chestpiece";
static const std::string MISTPOOL_PANTS_NAME = "scrap_metal_pants";
static const std::string MISTPOOL_BOOTS_NAME = "scrap_metal_boots";
static const std::string MISTPOOL_GLOVES_NAME = "scrap_metal_ring";
static const std::string CURSED_HELMET_NAME = "cursed_helmet";
static const std::string CURSED_CHESTPIECE_NAME = "cursed_chestpiece";
static const std::string CURSED_PANTS_NAME = "cursed_pants";
static const std::string CURSED_BOOTS_NAME = "cursed_boots";
static const std::string CURSED_GLOVES_NAME = "cursed_gloves";
static const std::string CURSED_BRACELET_NAME = "cursed_bracelet";
static const std::string CLERIC_HELMET_NAME = "cleric_helmet";
static const std::string CLERIC_CHESTPIECE_NAME = "cleric_chestpiece";
static const std::string CLERIC_GLOVES_NAME = "cleric_gloves";
static const std::string CLERIC_PANTS_NAME = "cleric_pants";
static const std::string CLERIC_BOOTS_NAME = "cleric_boots";
// TODO: Class gear
static const std::string TREASURE_CHEST_WOOD_NAME = "treasure_chest_wood";
static const std::string TREASURE_CHEST_COPPER_NAME = "treasure_chest_copper";
static const std::string TREASURE_CHEST_SILVER_NAME = "treasure_chest_silver";
static const std::string TREASURE_CHEST_GOLD_NAME = "treasure_chest_gold";
static const std::string TIDE_CAVERNS_KEY_NAME = "tide_caverns_key";
static const std::string DEEP_EARTH_KEY_NAME = "deep_earth_key";
static const std::string LAVA_CAVES_KEY_NAME = "lava_caves_key";
static const std::string RUINS_KEY_NAME = "ruins_key";
static const std::string WATER_ORB_NAME = "water_orb";
// TODO: Orbs
static const std::string LIFT_KEY_RESTRICTED_NOTIFICATION_KEY = "Notifications/Mods/Deep Dungeon/lift_key_restricted";
static const std::string SIGIL_LIMIT_NOTIFICATION_KEY = "Notifications/Mods/Deep Dungeon/sigil_limit";
static const std::string SALVE_LIMIT_NOTIFICATION_KEY = "Notifications/Mods/Deep Dungeon/salve_limit";
static const std::string ITEM_PENALTY_NOTIFICATION_KEY = "Notifications/Mods/Deep Dungeon/item_penalty";
static const std::string ITEM_PROHIBITED_NOTIFICATION_KEY = "Notifications/Mods/Deep Dungeon/item_prohibited";
static const std::string ITEM_RESTRICTED_NOTIFICATION_KEY = "Notifications/Mods/Deep Dungeon/item_restricted";
static const std::string CONCEALMENT_LOST_NOTIFICATION_KEY = "Notifications/Mods/Deep Dungeon/Sigils/concealment/deactivated";
static const std::string INHIBITED_PENALTY_NOTIFICATION_KEY = "Notifications/Mods/Deep Dungeon/inhibited_penalty";
static const std::string CONFUSING_TRAP_NOTIFICATION_KEY = "Notifications/Mods/Deep Dungeon/Traps/confusing";
static const std::string DISORIENTING_TRAP_NOTIFICATION_KEY = "Notifications/Mods/Deep Dungeon/Traps/disorienting";
static const std::string EXPLODING_TRAP_NOTIFICATION_KEY = "Notifications/Mods/Deep Dungeon/Traps/exploding";
static const std::string INHIBITING_TRAP_NOTIFICATION_KEY = "Notifications/Mods/Deep Dungeon/Traps/inhibiting";
static const std::string LURING_TRAP_NOTIFICATION_KEY = "Notifications/Mods/Deep Dungeon/Traps/luring";
static const std::string FLOOR_ENCHANTMENT_CONVERSATION_KEY = "Conversations/Mods/Deep Dungeon/floor_enchantments";
static const std::string DREAD_BEAST_WARNING_CONVERSATION_KEY = "Conversations/Mods/Deep Dungeon/dread_beast_warning";
static const std::string FLOOR_ENCHANTMENT_AND_DREAD_BEAST_WARNING_CONVERSATION_KEY = "Conversations/Mods/Deep Dungeon/dread_beast_warning_and_floor_enchantments";
static const std::string PROGRESSION_MODE_ELEVATOR_LOCKED_CONVERSATION_KEY = "Conversations/Mods/Deep Dungeon/Progression Mode/elevator_locked";
static const std::string OFFERINGS_PLACEHOLDER_TEXT_KEY = "Conversations/Mods/Deep Dungeon/placeholders/offerings/result";
static const std::string FLOOR_ENCHANTMENT_PLACEHOLDER_TEXT_KEY = "Conversations/Mods/Deep Dungeon/placeholders/floor_enchantments/init";
static const std::string DREAD_BEAST_WARNING_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Special/dread";
static const std::string GLOOM_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Negative/gloom";
static const std::string HP_PENALTY_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Negative/hp_penalty";
static const std::string EXHAUSTION_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Negative/exhaustion";
static const std::string AMNESIA_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Negative/amnesia";
static const std::string ITEM_PENALTY_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Negative/item_penalty";
static const std::string DISTORTION_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Negative/distortion";
static const std::string DAMAGE_DOWN_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Negative/damage_down";
static const std::string GRAVITY_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Negative/gravity";
static const std::string FEY_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Positive/fey";
static const std::string RESTORATION_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Positive/restoration";
static const std::string SECOND_WIND_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Positive/second_wind";
static const std::string HASTE_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Floor Enchantments/Positive/haste";

static const std::string DREAD_OFFERING_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Offerings/Negative/dread";
static const std::string INNER_FIRE_OFFERING_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Offerings/Positive/inner_fire";
static const std::string LEECH_OFFERING_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Offerings/Positive/leech";
static const std::string PERIL_OFFERING_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Offerings/Negative/peril";
static const std::string RECKONING_OFFERING_LOCALIZED_TEXT_KEY = "Conversations/Mods/Deep Dungeon/Offerings/Negative/reckoning";

static const int TWO_MINUTES_IN_SECONDS = 120;
static const int TRAP_ACTIVATION_DISTANCE = 16;

static enum class ClassArmor {
	CLERIC,
	WIZARD,
	PALADIN,
	ELDRITCH_KNIGHT,
	ROGUE
};

static enum class AriResources {
	HEALTH,
	STAMINA,
	MANA
};

static enum class DungeonBiomes {
	UPPER,
	TIDE_CAVERNS,
	DEEP_EARTH,
	LAVA_CAVES,
	RUINS
};

static enum class FloorEnchantments {
	// Negative effects
	HP_PENALTY, // Group 1
	EXHAUSTION, // Group 1
	AMNESIA, // Group 2
	ITEM_PENALTY, // Group 2
	DISTORTION, // Group 2
	DAMAGE_DOWN, // Group 2
	GRAVITY, // Group 1
	GLOOM, // Group 3

	// Positive effects
	RESTORATION, // Group 1
	SECOND_WIND, // Group 1
	HASTE, // Group 1
	FEY, // Group 3
};

static enum class Offerings {
	DREAD, // TODO: Effect exists, need to apply it to a monster being spawned.
	INNER_FIRE,
	LEECH,
	PERIL,
	RECKONING,
};

static enum class Sigils {
	RAGE,
	STRENGTH,
	FORTIFICATION,
	PROTECTION,
	SERENITY,
	SAFETY,
	SILENCE,
	FORTUNE,
	REDEMPTION,
	ALTERATION,
	CONCEALMENT,
	TEMPTATION
};

static enum class Traps {
	CONFUSING,
	DISORIENTING,
	EXPLODING,
	INHIBITING,
	LURING
};

static const std::unordered_set<std::string> DUNGEON_TREASURE_CHEST_NAMES = {
	TREASURE_CHEST_WOOD_NAME,
	TREASURE_CHEST_COPPER_NAME,
	TREASURE_CHEST_SILVER_NAME,
	TREASURE_CHEST_GOLD_NAME
};

static const std::vector<std::string> MISTPOOL_ARMOR_NAMES = {
	MISTPOOL_HELMET_NAME,
	MISTPOOL_CHESTPIECE_NAME,
	MISTPOOL_PANTS_NAME,
	MISTPOOL_BOOTS_NAME,
	MISTPOOL_GLOVES_NAME
};

static const std::unordered_set<std::string> CLASS_ARMOR_NAMES = {
	// TODO: Add other class armor
	CLERIC_HELMET_NAME, CLERIC_CHESTPIECE_NAME, CLERIC_GLOVES_NAME, CLERIC_PANTS_NAME, CLERIC_BOOTS_NAME
};

static const std::map<ClassArmor, std::unordered_set<std::string>> CLASS_ARMOR_TO_NAMES_MAP = {
	{ ClassArmor::CLERIC, { CLERIC_HELMET_NAME, CLERIC_CHESTPIECE_NAME, CLERIC_GLOVES_NAME, CLERIC_PANTS_NAME, CLERIC_BOOTS_NAME } },
	// TODO: Add other class armor
};

static const std::vector<FloorEnchantments> GROUP_ONE_FLOOR_ENCHANTMENTS = {
	FloorEnchantments::HP_PENALTY,
	FloorEnchantments::EXHAUSTION,
	FloorEnchantments::GRAVITY,
	FloorEnchantments::RESTORATION,
	FloorEnchantments::SECOND_WIND,
	FloorEnchantments::HASTE,
};

static const std::vector<FloorEnchantments> GROUP_TWO_FLOOR_ENCHANTMENTS = {
	FloorEnchantments::AMNESIA,
	FloorEnchantments::ITEM_PENALTY,
	FloorEnchantments::DISTORTION,
	FloorEnchantments::DAMAGE_DOWN,
};

static const std::vector<FloorEnchantments> GROUP_THREE_FLOOR_ENCHANTMENTS = {
	FloorEnchantments::GLOOM,
	FloorEnchantments::FEY
};

static const std::map<std::string, Sigils> item_name_to_sigil_map = {
	{ SIGIL_OF_ALTERATION_NAME, Sigils::ALTERATION },
	{ SIGIL_OF_CONCEALMENT_NAME, Sigils::CONCEALMENT },
	{ SIGIL_OF_FORTIFICATION_NAME, Sigils::FORTIFICATION },
	{ SIGIL_OF_FORTUNE_NAME, Sigils::FORTUNE },
	{ SIGIL_OF_PROTECTION_NAME, Sigils::PROTECTION },
	{ SIGIL_OF_RAGE_NAME, Sigils::RAGE },
	{ SIGIL_OF_REDEMPTION_NAME, Sigils::REDEMPTION },
	{ SIGIL_OF_SAFETY_NAME, Sigils::SAFETY },
	{ SIGIL_OF_SERENITY_NAME, Sigils::SERENITY },
	{ SIGIL_OF_SILENCE_NAME, Sigils::SILENCE },
	{ SIGIL_OF_STRENGTH_NAME, Sigils::STRENGTH },
	{ SIGIL_OF_TEMPTATION_NAME, Sigils::TEMPTATION }
};

static const std::map<std::string, std::vector<std::pair<int, int>>> TRAP_SPAWN_POINTS = {
	{ "rm_mines_upper_floor1", {
		{ 224 + 8, 144 + 8 },
		{ 192 + 8, 208 + 8 },
		{ 176 + 8, 368 + 8 },
		{ 288 + 8, 352 + 8 },
		{ 384 + 8, 288 + 8 },
		{ 368 + 8, 208 + 8 },
	}},
	{ "rm_mines_upper_elevator5", {
		{ 64 + 8, 240 + 8 },
		{ 176 + 8, 192 + 8 },
		{ 272 + 8, 96 + 8 },
		{ 352 + 8, 64 + 8 },
		{ 400 + 8, 128 + 8 },
		{ 336 + 8, 160 + 8 },
	}},
	{ "rm_mines_upper_elevator10", {
		{ 352 + 8, 352 + 8 },
		{ 400 + 8, 208 + 8 },
		{ 320 + 8, 240 + 8 },
		{ 96 + 8, 256 + 8 },
		{ 112 + 8, 160 + 8 },
		{ 128 + 8, 80 + 8 },
	}},
	{ "rm_mines_upper_elevator15", {
		{ 80 + 8, 144 + 8 },
		{ 112 + 8, 288 + 8 },
		{ 224 + 8, 368 + 8 },
		{ 304 + 8, 128 + 8 },
		{ 352 + 8, 208 + 8 },
		{ 352 + 8, 352 + 8 },
	}},
	{ "rm_mines_upper_slimetime", {
		{ 352 + 8, 320 + 8 },
		{ 272 + 8, 432 + 8 },
		{ 192 + 8, 384 + 8 },
		{ 144 + 8, 320 + 8 },
		{ 176 + 8, 256 + 8 },
		{ 256 + 8, 208 + 8 },
	}},
	{ "rm_mines_upper_sock", {
		{ 208 + 8, 192 + 8 },
		{ 240 + 8, 256 + 8 },
		{ 240 + 8, 416 + 8 },
		{ 272 + 8, 496 + 8 },
		{ 352 + 8, 560 + 8 },
		{ 400 + 8, 496 + 8 },
	}},
	{ "rm_mines_upper_boomerang", {
		{ 304 + 8, 336 + 8 },
		{ 240 + 8, 304 + 8 },
		{ 128 + 8, 224 + 8 },
		{ 208 + 8, 160 + 8 },
		{ 272 + 8, 160 + 8 },
		{ 320 + 8, 128 + 8 },
	}},
	{ "rm_mines_upper_worm", {
		{ 112 + 8, 144 + 8 },
		{ 304 + 8, 160 + 8 },
		{ 384 + 8, 240 + 8 },
		{ 416 + 8, 336 + 8 },
		{ 400 + 8, 432 + 8 },
		{ 528 + 8, 464 + 8 },
	}},
	{ "rm_mines_upper_ponds", {
		{ 304 + 8, 432 + 8 },
		{ 384 + 8, 512 + 8 },
		{ 192 + 8, 368 + 8 },
		{ 368 + 8, 320 + 8 },
		{ 304 + 8, 240 + 8 },
		{ 352 + 8, 128 + 8 },
	}},
	{ "rm_mines_upper_pond", {
		{ 192 + 8, 400 + 8 },
		{ 192 + 8, 240 + 8 },
		{ 336 + 8, 144 + 8 },
		{ 448 + 8, 288 + 8 },
		{ 352 + 8, 384 + 8 },
		{ 416 + 8, 448 + 8 },
	}},
	{ "rm_mines_upper_snake", {
		{ 320 + 8, 144 + 8 },
		{ 192 + 8, 112 + 8 },
		{ 160 + 8, 336 + 8 },
		{ 400 + 8, 336 + 8 },
		{ 448 + 8, 448 + 8 },
		{ 336 + 8, 512 + 8 },
	}},
	{ "rm_mines_upper_four", {
		{ 144 + 8, 144 + 8 },
		{ 224 + 8, 368 + 8 },
		{ 384 + 8, 352 + 8 },
		{ 464 + 8, 272 + 8 },
		{ 464 + 8, 448 + 8 },
		{ 560 + 8, 512 + 8 },
	}},
	{ "rm_mines_upper_amoeba", {
		{ 272 + 8, 464 + 8 },
		{ 352 + 8, 400 + 8 },
		{ 512 + 8, 432 + 8 },
		{ 496 + 8, 304 + 8 },
		{ 528 + 8, 240 + 8 },
		{ 640 + 8, 256 + 8 },
	}},
	{ "rm_mines_upper_wishbone", {
		{ 256 + 8, 512 + 8 },
		{ 352 + 8, 464 + 8 },
		{ 336 + 8, 368 + 8 },
		{ 272 + 8, 240 + 8 },
		{ 400 + 8, 160 + 8 },
		{ 480 + 8, 256 + 8 },
	}},
	{ "rm_mines_upper_canada", {
		{ 192 + 8, 320 + 8 },
		{ 208 + 8, 208 + 8 },
		{ 336 + 8, 192 + 8 },
		{ 336 + 8, 272 + 8 },
		{ 448 + 8, 208 + 8 },
		{ 464 + 8, 336 + 8 },
	}},
	{ "rm_mines_upper_staple", {
		{ 496 + 8, 320 + 8 },
		{ 416 + 8, 208 + 8 },
		{ 336 + 8, 256 + 8 },
		{ 240 + 8, 192 + 8 },
		{ 144 + 8, 224 + 8 },
		{ 176 + 8, 368 + 8 },
	}},
	{ "rm_mines_upper_pillars", {
		{ 112 + 8, 48 + 8 },
		{ 336 + 8, 112 + 8 },
		{ 176 + 8, 208 + 8 },
		{ 48 + 8, 240 + 8 },
		{ 240 + 8, 336 + 8 },
		{ 368 + 8, 288 + 8 },
	}},
	{ "rm_mines_upper_path", {
		{ 208 + 8, 352 + 8 },
		{ 368 + 8, 256 + 8 },
		{ 384 + 8, 112 + 8 },
		{ 272 + 8, 192 + 8 },
		{ 160 + 8, 64 + 8 },
		{ 80 + 8, 224 + 8 },
	}},
	{ "rm_mines_upper_stream", {
		{ 320 + 8, 80 + 8 },
		{ 272 + 8, 160 + 8 },
		{ 192 + 8, 112 + 8 },
		{ 176 + 8, 288 + 8 },
		{ 208 + 8, 352 + 8 },
		{ 80 + 8, 352 + 8 },
	}},
	{ "rm_mines_upper_muscle", {
		{ 192 + 8, 544 + 8 },
		{ 272 + 8, 496 + 8 },
		{ 320 + 8, 416 + 8 },
		{ 400 + 8, 304 + 8 },
		{ 416 + 8, 224 + 8 },
		{ 336 + 8, 160 + 8 },
	}},
	{ "rm_mines_upper_crossroad", {
		{ 416 + 8, 208 + 8 },
		{ 144 + 8, 224 + 8 },
		{ 192 + 8, 304 + 8 },
		{ 240 + 8, 400 + 8 },
		{ 416 + 8, 432 + 8 },
		{ 208 + 8, 480 + 8 },
	}},
	{ "rm_mines_upper_pits", {
		{ 128 + 8, 320 + 8 },
		{ 160 + 8, 384 + 8 },
		{ 240 + 8, 400 + 8 },
		{ 256 + 8, 320 + 8 },
		{ 288 + 8, 480 + 8 },
		{ 368 + 8, 320 + 8 },
	}},
	{ "rm_mines_upper_formerelevator", {
		{ 80 + 8, 224 + 8 },
		{ 112 + 8, 64 + 8 },
		{ 176 + 8, 304 + 8 },
		{ 320 + 8, 96 + 8 },
		{ 368 + 8, 320 + 8 },
		{ 400 + 8, 192 + 8 },
	}},
	//{ "", {
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//}},
	//{ "", {
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//}},
	//{ "", {
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//}},
	//{ "", {
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//}},
	//{ "", {
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//	{ 0 + 8, 0 + 8 },
	//}},
};

static YYTKInterface* g_ModuleInterface = nullptr;
static CInstance* global_instance = nullptr;
static bool load_on_start = true;
static bool localize_mod_text = false;
static bool game_is_active = false;
static bool progression_mode = true; // TODO: Make this configurable
static bool biome_reward = false;
static bool sigil_item_used = false;
static bool lift_key_used = false; // TODO
static bool fire_breath_cast = false;
static bool reckoning_applied = false;
static bool fairy_buff_applied = false;
static bool is_restoration_tracked_interval = false;
static bool is_second_wind_tracked_interval = false;
static bool offering_chance_occurred = false;
static bool obj_dungeon_elevator_focused = false;
static bool obj_dungeon_ladder_down_focused = false;
static double ari_x = -1;
static double ari_y = -1;
static double floor_number = 0;
static int current_time_in_seconds = -1;
static int time_of_last_restoration_tick = -1;
static int time_of_last_second_wind_tick = -1;
static int held_item_id = -1;
static int sigil_of_silence_count = 0;
static int sigil_of_alteration_count = 0;

static std::string ari_current_location = "";
static std::string ari_current_gm_room = "";
static std::unordered_set<int> lift_key_items = {};
static std::unordered_set<int> restricted_items = {};
static std::unordered_set<int> deep_dungeon_items = {};
static std::map<Sigils, int> sigil_to_item_id_map = {};
static std::map<int, Sigils> item_id_to_sigil_map = {};
static std::map<std::string, int> perk_name_to_id_map = {};
static std::map<int, int> spell_id_to_default_cost_map = {};
static std::map<std::string, int> salve_name_to_id_map = {};
static std::map<int, std::string> object_id_to_name_map = {};
static std::map<std::string, int> player_state_to_id_map = {};
static std::map<std::string, int> monster_name_to_id_map = {};
static std::map<std::string, int> tutorial_name_to_id_map = {};
static std::map<std::string, int> infusion_name_to_id_map = {};
static std::map<std::string, int> status_effect_name_to_id_map = {};
static std::map<std::string, int> mistpool_gear_to_item_id_map = {}; // TODO: Remove this. Use item_name_to_id_map instead.
static std::map<std::string, int> cursed_gear_to_item_id_map = {}; // TODO: Remove this. Use item_name_to_id_map instead.
static std::map<std::string, int> item_name_to_id_map = {};
std::unordered_set<std::pair<int,int>, pair_hash> floor_trap_positions = {};
static std::unordered_set<int> salves_used = {};
static std::unordered_set<Traps> active_traps = {};
static std::unordered_set<Sigils> active_sigils = {};
static std::unordered_set<Offerings> queued_offerings = {};
static std::unordered_set<Offerings> active_offerings = {};
static std::unordered_set<FloorEnchantments> active_floor_enchantments = {};
static std::map<Traps, int> active_traps_to_value_map = {}; // Used to track value's for active traps.
static std::map<Offerings, std::string> offerings_to_localized_string_map = {};
static std::map<FloorEnchantments, std::string> floor_enchantments_to_localized_string_map = {};
static std::map<AriResources, int> ari_resource_to_value_map = {}; // Used to track Ari's current resource levels.
static std::map<AriResources, bool> ari_resource_to_penalty_map = {}; // Used to track penalties from offerings to apply.
static std::map<std::string, std::vector<CInstance*>> script_name_to_reference_map; // Vector<CInstance*> holds references to Self and Other for each script.
static std::map<std::string, std::unordered_set<int>> dungeon_biome_to_candidate_monsters_map = {}; // Holds the candidate monster spawns for each dungeon biome.
static std::map<int, std::string> floor_number_to_biome_name_map = {}; // Maps floor numbers to the dungeon biome name.
static std::vector<CInstance*> current_floor_monsters = {};
static std::map<std::string, uint64_t> notification_name_to_last_display_time_map = {}; // Tracks when a notification was last displayed.
static std::map<int, RValue> item_id_to_prototype_map = {};

// GUI
static double window_width = 0;
static double window_height = 0;
static bool show_dashes = false;
static bool show_danger_banner = false;
static bool fade_initialized = false;
static uint64_t fade_start_time = 0;

void ResetFade()
{
	fade_initialized = false;
	fade_start_time = 0;
}

void ResetCustomDrawFields()
{
	show_dashes = false;
	show_danger_banner = false;
	ResetFade();
}

void ResetStaticFields(bool returned_to_title_screen)
{
	if (returned_to_title_screen)
	{
		game_is_active = false;
		is_restoration_tracked_interval = false;
		is_second_wind_tracked_interval = false;
		ari_x = -1;
		ari_y = -1;
		floor_number = 0;
		current_time_in_seconds = -1;
		time_of_last_restoration_tick = -1;
		time_of_last_second_wind_tick = -1;
		held_item_id = -1;
		ari_current_location = "";
		ari_current_gm_room = "";
		script_name_to_reference_map.clear();
	}

	sigil_item_used = false;
	fire_breath_cast = false;
	reckoning_applied = false;
	fairy_buff_applied = false;
	offering_chance_occurred = false;
	obj_dungeon_elevator_focused = false;
	obj_dungeon_ladder_down_focused = false;
	sigil_of_silence_count = 0;
	sigil_of_alteration_count = 0;
	salves_used.clear();
	active_sigils.clear();
	queued_offerings.clear();
	active_offerings.clear();
	active_floor_enchantments.clear();
	active_traps.clear();
	active_traps_to_value_map.clear();
	floor_trap_positions.clear();
	current_floor_monsters.clear();
	ResetCustomDrawFields();
}
bool GameIsPaused()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);
	RValue paused = global_instance->GetMember("__pause_status");
	return paused.m_i64 > 0;
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

double GetWindowWidth()
{
	RValue window_width = g_ModuleInterface->CallBuiltin("window_get_width",{});
	return window_width.ToDouble();
}

double GetWindowHeight()
{
	RValue window_height = g_ModuleInterface->CallBuiltin("window_get_height", {});
	return window_height.ToDouble();
}

uint64_t GetCurrentSystemTime() {
	return duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

double GetDistance(int x1, int y1, int x2, int y2) {
	return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

std::pair<int, int> GetCenterOffset(int screen_center_x, int screen_center_y, int image_width, int image_height) {
	int offset_x = screen_center_x - image_width / 2;
	int offset_y = screen_center_y - image_height / 2;
	return { offset_x, offset_y };
}

std::vector<double> GetCenter(std::vector<double> topLeft, std::vector<double> bottomRight)
{
	double x = (topLeft[0] + bottomRight[0]) / 2.0;
	double y = (topLeft[1] + bottomRight[1]) / 2.0;
	std::vector<double> center = { x, y };
	return center;
}

void DrawRectangle(int color, float x1, float y1, float x2, float y2, bool outline)
{
	g_ModuleInterface->CallBuiltin(
		"draw_set_color", {
		 color
		}
	);

	g_ModuleInterface->CallBuiltin(
		"draw_rectangle", {
			x1, y1, x2, y2, outline
		}
	);
}

void DrawImage(int x, int y, int transparency)
{
	RValue sprite_index = g_ModuleInterface->CallBuiltin(
		"asset_get_index", {
			"aldarian_danger_banner"
		}
	);

	g_ModuleInterface->CallBuiltin(
		"draw_sprite_ext", {
			sprite_index, -1, x, y, 1, 1, 0, 16777215, transparency / 100.0
		}
	);
}

void FadeInImage(double seconds_per_cycle, int repeat_count) {
	// Window corners
	std::vector<double> window_top_left = { 0.0, 0.0 };
	std::vector<double> window_top_right = { window_width, 0.0 };
	std::vector<double> window_bottom_left = { 0.0, window_height };
	std::vector<double> window_bottom_right = { window_width, window_height };

	// Window center
	auto center = GetCenter(window_top_left, window_bottom_right);
	auto centered_offset = GetCenterOffset(center[0], center[1], 400, 120);

	if (!fade_initialized) {
		fade_start_time = GetCurrentSystemTime();
		fade_initialized = true;
	}

	double cycle_ms = seconds_per_cycle * 1000.0;
	uint64_t elapsed = GetCurrentSystemTime() - fade_start_time;

	int current_cycle = elapsed / cycle_ms;

	if (current_cycle >= repeat_count)
		return;

	double cycle_position = static_cast<double>(elapsed % static_cast<int>(cycle_ms));
	double half_cycle = cycle_ms / 2.0;
	int transparency = 0;

	if (cycle_position <= half_cycle) {
		// Fade in: 0 -> 100
		transparency = static_cast<int>((cycle_position / half_cycle) * 100);
	}
	else {
		// Fade out: 100 -> 0
		double fade_out_pos = cycle_position - half_cycle;
		transparency = static_cast<int>(((half_cycle - fade_out_pos) / half_cycle) * 100);
	}

	transparency = std::clamp(transparency, 0, 100);
	DrawImage(centered_offset.first, centered_offset.second, transparency);
}

void DrawDashedBorder(
	float dash_len,
	float dash_thk,
	float speed,          // pixels per second
	float screen_width,
	float screen_height,
	uint64_t current_time_ms
) {
	// Static dash state
	static std::vector<float> dash_positions;  // positions around perimeter
	static bool initialized = false;

	float top_len = screen_width;
	float right_len = screen_height;
	float bottom_len = screen_width;
	float left_len = screen_height;
	float perimeter = 2.0f * (screen_width + screen_height);

	// Desired dash spacing (avg)
	float target_spacing = perimeter / 40.0f;

	if (!initialized) {
		// Compute dash count per edge
		int top_count = static_cast<int>(std::floor(top_len / target_spacing));
		int right_count = static_cast<int>(std::floor(right_len / target_spacing));
		int bottom_count = static_cast<int>(std::floor(bottom_len / target_spacing));
		int left_count = static_cast<int>(std::floor(left_len / target_spacing));

		// Store total dashes spaced along perimeter
		dash_positions.clear();
		for (int i = 0; i < top_count; ++i)
			dash_positions.push_back((top_len / top_count) * i);
		for (int i = 0; i < right_count; ++i)
			dash_positions.push_back(top_len + (right_len / right_count) * i);
		for (int i = 0; i < bottom_count; ++i)
			dash_positions.push_back(top_len + right_len + (bottom_len / bottom_count) * i);
		for (int i = 0; i < left_count; ++i)
			dash_positions.push_back(top_len + right_len + bottom_len + (left_len / left_count) * i);

		initialized = true;
	}

	// Time-based offset
	static uint64_t last_time = current_time_ms;
	float delta_sec = (current_time_ms - last_time) / 1000.0f;
	last_time = current_time_ms;

	static float offset = 0.0f;
	offset += speed * delta_sec;
	if (offset > perimeter) offset -= perimeter;

	// Draw all dashes with animated offset
	for (float base_pos : dash_positions) {
		float pos = base_pos + offset;
		if (pos >= perimeter) pos -= perimeter;

		if (pos < top_len) {
			float x = pos;
			DrawRectangle(255, x, 0.0f, x + dash_len, dash_thk, false);
		}
		else if (pos < top_len + right_len) {
			float y = pos - top_len;
			DrawRectangle(255, screen_width - dash_thk, y, screen_width, y + dash_len, false);
		}
		else if (pos < top_len + right_len + bottom_len) {
			float x = screen_width - (pos - (top_len + right_len));
			DrawRectangle(255, x, screen_height - dash_thk, x + dash_len, screen_height, false);
		}
		else {
			float y = screen_height - (pos - (top_len + right_len + bottom_len));
			DrawRectangle(255, 0.0f, y, dash_thk, y + dash_len, false);
		}
	}
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

void LoadPerks()
{
	size_t array_length;
	RValue perk_names = global_instance->GetMember("__perk__");
	g_ModuleInterface->GetArraySize(perk_names, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* perk_name;
		g_ModuleInterface->GetArrayEntry(perk_names, i, perk_name);

		perk_name_to_id_map[perk_name->ToString()] = i;
	}
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

void DisableAllPerks()
{
	std::unordered_set<int> perks_to_disable = {};

	std::vector<std::string> struct_field_names = {};
	auto GetStructFieldNames = [&](IN const char* MemberName, IN OUT RValue* Value) {
		struct_field_names.push_back(MemberName);
		return false;
	};
	
	RValue dragon_shrine_data = global_instance->GetMember("__dragon_shrine_data");
	RValue inner = dragon_shrine_data.GetMember("inner");
	
	// Combat Perks
	RValue combat = inner.GetMember("combat");
	g_ModuleInterface->EnumInstanceMembers(combat, GetStructFieldNames);
	for (std::string field_name : struct_field_names)
	{
		if (field_name.contains("tier"))
		{
			size_t array_length;
			RValue tier = combat.GetMember(field_name);
			g_ModuleInterface->GetArraySize(tier, array_length);

			for (size_t i = 0; i < array_length; i++)
			{
				RValue* entry;
				g_ModuleInterface->GetArrayEntry(tier, i, entry);

				perks_to_disable.insert(entry->GetMember("perk").ToInt64());
			}
		}
	}

	// Mining Perks
	struct_field_names = {};
	RValue mining = inner.GetMember("mining");
	g_ModuleInterface->EnumInstanceMembers(mining, GetStructFieldNames);
	for (std::string field_name : struct_field_names)
	{
		if (field_name.contains("tier"))
		{
			size_t array_length;
			RValue tier = mining.GetMember(field_name);
			g_ModuleInterface->GetArraySize(tier, array_length);

			for (size_t i = 0; i < array_length; i++)
			{
				RValue* entry;
				g_ModuleInterface->GetArrayEntry(tier, i, entry);

				perks_to_disable.insert(entry->GetMember("perk").ToInt64());
			}
		}
	}

	// Cooking Perks
	perks_to_disable.insert(perk_name_to_id_map["snacktime"]);

	RValue __ari = *global_instance->GetRefMember("__ari");
	RValue __ari_perks_active = *__ari.GetRefMember("perks_active");
	
	for(int perk : perks_to_disable)
		__ari_perks_active[perk] = false;
}

void LoadTutorials()
{
	size_t array_length;
	RValue tutorials = global_instance->GetMember("__tutorial__");
	g_ModuleInterface->GetArraySize(tutorials, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* tutorial;
		g_ModuleInterface->GetArrayEntry(tutorials, i, tutorial);

		tutorial_name_to_id_map[tutorial->ToString()] = i;
	}
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

void LoadInfusions()
{
	size_t array_length;
	RValue infusions = global_instance->GetMember("__infusion__");
	g_ModuleInterface->GetArraySize(infusions, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* infusion;
		g_ModuleInterface->GetArrayEntry(infusions, i, infusion);

		infusion_name_to_id_map[infusion->ToString()] = i;
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

void LoadDungeonBiomeCandidateMonsters()
{
	RValue biomes = global_instance->GetMember("__fiddle").GetMember("__inner").GetMember("inner").GetMember("dungeons").GetMember("dungeons").GetMember("biomes");
	int max_floors = global_instance->GetMember("__fiddle").GetMember("__inner").GetMember("inner").GetMember("dungeons").GetMember("misc").GetMember("max_floors").ToInt64() + 1;

	size_t biomes_length;
	g_ModuleInterface->GetArraySize(biomes, biomes_length);

	for (size_t i = 0; i < biomes_length; i++)
	{
		RValue* biome;
		g_ModuleInterface->GetArrayEntry(biomes, i, biome);

		int floor = biome->GetMember("floor").ToInt64();
		std::string biome_name = biome->GetMember("name").ToString();
		RValue enemies = biome->GetMember("votes").GetMember("enemy");

		size_t enemies_length;
		g_ModuleInterface->GetArraySize(enemies, enemies_length);

		for (size_t j = 0; j < enemies_length; j++)
		{
			RValue* enemy;
			g_ModuleInterface->GetArrayEntry(enemies, j, enemy);

			std::string enemy_name = enemy->GetMember("object").ToString();

			if (monster_name_to_id_map.contains(enemy_name))
				dungeon_biome_to_candidate_monsters_map[biome_name].insert(monster_name_to_id_map[enemy_name]);
			else
				g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] Failed to look up enemy name: %s", MOD_NAME, VERSION, enemy_name.c_str());
		}

		if (i < biomes_length - 1)
		{
			RValue* next_biome;
			g_ModuleInterface->GetArrayEntry(biomes, i + 1, next_biome);

			int next_biome_floor = next_biome->GetMember("floor").ToInt64();
			for (size_t j = floor; j < next_biome_floor; j++)
				floor_number_to_biome_name_map[j] = biome_name;
		}
		else
		{
			for (size_t j = floor; j <= max_floors; j++)
				floor_number_to_biome_name_map[j] = biome_name;
		}
	}
}

void LoadObjectIds()
{
	size_t array_length;
	RValue objects = global_instance->GetMember("__object_id__");
	g_ModuleInterface->GetArraySize(objects, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* object;
		g_ModuleInterface->GetArrayEntry(objects, i, object);

		object_id_to_name_map[i] = object->ToString();
	}
}

void LoadItems()
{
	std::unordered_set<std::string> lift_keys = { TIDE_CAVERNS_KEY_NAME, DEEP_EARTH_KEY_NAME, LAVA_CAVES_KEY_NAME, RUINS_KEY_NAME };
	std::vector<std::string> custom_potions = { SUSTAINING_POTION_NAME, HEALTH_SALVE_NAME, STAMINA_SALVE_NAME, MANA_SALVE_NAME }; // TODO: Change to unordered_set
	std::vector<std::string> cursed_armor = { CURSED_HELMET_NAME, CURSED_CHESTPIECE_NAME, CURSED_PANTS_NAME, CURSED_BOOTS_NAME, CURSED_GLOVES_NAME, CURSED_BRACELET_NAME }; // TODO: Change to unordered_set

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
			item_name_to_id_map[item_name] = item_id;

			// Sigil items
			if (item_name_to_sigil_map.contains(item_name))
			{
				deep_dungeon_items.insert(item_id);
				sigil_to_item_id_map[item_name_to_sigil_map.at(item_name)] = item_id;
				item_id_to_sigil_map[item_id] = item_name_to_sigil_map.at(item_name);

				*item->GetRefMember("health_modifier") = 0;
			}

			// Lift keys
			if (lift_keys.contains(item_name))
				lift_key_items.insert(item_id);

			// Custom potions
			for (std::string custom_potion : custom_potions)
			{
				if (item_name == custom_potion)
				{
					deep_dungeon_items.insert(item_id);
					salve_name_to_id_map[item_name] = item_id;
				}
			}

			// Cursed armor
			for (std::string cursed_armor_name : cursed_armor)
				if (item_name == cursed_armor_name)
					cursed_gear_to_item_id_map[cursed_armor_name] = item_id;

			// All consumable items
			if (name_key.ToString().contains("cooked_dishes"))
				restricted_items.insert(item_id);
			else
			{
				RValue edible = item->GetMember("edible");
				if (edible.m_Kind == VALUE_BOOL && edible.m_Real == 1.0)
					restricted_items.insert(item_id);
			}

			// All armor and weapons
			if (StructVariableExists(*item, "tags"))
			{
				RValue tags = item->GetMember("tags");
				RValue buffer = tags.GetMember("__buffer");

				size_t array_length = 0;
				g_ModuleInterface->GetArraySize(buffer, array_length);
				for (size_t i = 0; i < array_length; i++)
				{
					RValue* array_element;
					g_ModuleInterface->GetArrayEntry(buffer, i, array_element);

					if (array_element->ToString() == "armor")
					{
						if (!CLASS_ARMOR_NAMES.contains(item_name))
							*item->GetRefMember("defense") = 0;

						for (std::string mistpool_armor_name : MISTPOOL_ARMOR_NAMES)
							if(item_name == mistpool_armor_name)
								mistpool_gear_to_item_id_map[mistpool_armor_name] = item_id;
					}

					if (array_element->ToString() == "weapon")
					{
						if (item_name == MISTPOOL_SWORD_NAME)
						{
							deep_dungeon_items.insert(item_id);
							mistpool_gear_to_item_id_map[MISTPOOL_SWORD_NAME] = item_id;
						}
						else
						{
							*item->GetRefMember("damage") = 0;
							restricted_items.insert(item_id);
						}
					}

					if (array_element->ToString() == "bomb")
					{
						*item->GetRefMember("damage") = 0;
						restricted_items.insert(item_id);
					}
				}
			}

			// All snake oils
			if (item_name.contains("snake_oil"))
			{
				/*
				   - [string] __infusion__[1] = 'fire_sword'
				   - [string] __infusion__[4] = 'ice_sword'
				   - [string] __infusion__[16] = 'venom_sword'
				*/
				int default_infusion = item->GetMember("default_infusion").ToInt64();
				if (default_infusion == infusion_name_to_id_map["fire_sword"] || default_infusion == infusion_name_to_id_map["ice_sword"] || default_infusion == infusion_name_to_id_map["venom_sword"])
					restricted_items.insert(item_id);
			}
		}
	}
}

void MarkDungeonTutorialUnseen()
{
	RValue ari = *global_instance->GetRefMember("__ari");
	RValue tutorials_seen = *ari.GetRefMember("tutorials_seen");

	RValue* mines_tutorial;
	g_ModuleInterface->GetArrayEntry(tutorials_seen, tutorial_name_to_id_map["mines"], mines_tutorial);

	*mines_tutorial = false; // TESTING - Check this works
}

void ModifyMistpoolWeaponSprites()
{
	// Sprite indexes for the "Scrap Metal Sword" which is used as the Mistpool Sword.
	RValue spr_weapon_sword_scrap_metal_tool_combo1_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_scrap_metal_tool_combo1_e" });
	RValue spr_weapon_sword_scrap_metal_tool_combo1_n = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_scrap_metal_tool_combo1_n" });
	RValue spr_weapon_sword_scrap_metal_tool_combo1_s = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_scrap_metal_tool_combo1_s" });
	RValue spr_weapon_sword_scrap_metal_tool_combo2_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_scrap_metal_tool_combo2_e" });
	RValue spr_weapon_sword_scrap_metal_tool_combo2_n = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_scrap_metal_tool_combo2_n" });
	RValue spr_weapon_sword_scrap_metal_tool_combo2_s = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_scrap_metal_tool_combo2_s" });
	RValue spr_weapon_sword_scrap_metal_tool_combo3_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_scrap_metal_tool_combo3_e" });
	RValue spr_weapon_sword_scrap_metal_tool_combo3_n = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_scrap_metal_tool_combo3_n" });
	RValue spr_weapon_sword_scrap_metal_tool_combo3_s = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_scrap_metal_tool_combo3_s" });
	RValue spr_weapon_sword_scrap_metal_tool_down_attack_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_scrap_metal_tool_down_attack_e" });
	RValue spr_ui_item_tool_scrap_metal_sword = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_tool_scrap_metal_sword" });

	if (floor_number < 20)
	{
		RValue spr_weapon_sword_worn_tool_combo1_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_worn_tool_combo1_e" });
		RValue spr_weapon_sword_worn_tool_combo1_n = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_worn_tool_combo1_n" });
		RValue spr_weapon_sword_worn_tool_combo1_s = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_worn_tool_combo1_s" });
		RValue spr_weapon_sword_worn_tool_combo2_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_worn_tool_combo2_e" });
		RValue spr_weapon_sword_worn_tool_combo2_n = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_worn_tool_combo2_n" });
		RValue spr_weapon_sword_worn_tool_combo2_s = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_worn_tool_combo2_s" });
		RValue spr_weapon_sword_worn_tool_combo3_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_worn_tool_combo3_e" });
		RValue spr_weapon_sword_worn_tool_combo3_n = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_worn_tool_combo3_n" });
		RValue spr_weapon_sword_worn_tool_combo3_s = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_worn_tool_combo3_s" });
		RValue spr_weapon_sword_worn_tool_down_attack_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_worn_tool_down_attack_e" });
		RValue spr_ui_item_tool_rusty_sword = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_tool_rusty_sword" });

		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo1_e, spr_weapon_sword_worn_tool_combo1_e });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo1_n, spr_weapon_sword_worn_tool_combo1_n });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo1_s, spr_weapon_sword_worn_tool_combo1_s });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo2_e, spr_weapon_sword_worn_tool_combo2_e });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo2_n, spr_weapon_sword_worn_tool_combo2_n });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo2_s, spr_weapon_sword_worn_tool_combo2_s });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo3_e, spr_weapon_sword_worn_tool_combo3_e });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo3_n, spr_weapon_sword_worn_tool_combo3_n });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo3_s, spr_weapon_sword_worn_tool_combo3_s });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_down_attack_e, spr_weapon_sword_worn_tool_down_attack_e });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_ui_item_tool_scrap_metal_sword, spr_ui_item_tool_rusty_sword });
	}
	else if (floor_number < 40)
	{
		RValue spr_weapon_sword_verdigris_tool_combo1_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_verdigris_tool_combo1_e" });
		RValue spr_weapon_sword_verdigris_tool_combo1_n = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_verdigris_tool_combo1_n" });
		RValue spr_weapon_sword_verdigris_tool_combo1_s = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_verdigris_tool_combo1_s" });
		RValue spr_weapon_sword_verdigris_tool_combo2_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_verdigris_tool_combo2_e" });
		RValue spr_weapon_sword_verdigris_tool_combo2_n = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_verdigris_tool_combo2_n" });
		RValue spr_weapon_sword_verdigris_tool_combo2_s = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_verdigris_tool_combo2_s" });
		RValue spr_weapon_sword_verdigris_tool_combo3_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_verdigris_tool_combo3_e" });
		RValue spr_weapon_sword_verdigris_tool_combo3_n = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_verdigris_tool_combo3_n" });
		RValue spr_weapon_sword_verdigris_tool_combo3_s = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_verdigris_tool_combo3_s" });
		RValue spr_weapon_sword_verdigris_tool_down_attack_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_verdigris_tool_down_attack_e" });
		RValue spr_ui_item_tool_verdigris_sword = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_tool_verdigris_sword" });

		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo1_e, spr_weapon_sword_verdigris_tool_combo1_e });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo1_n, spr_weapon_sword_verdigris_tool_combo1_n });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo1_s, spr_weapon_sword_verdigris_tool_combo1_s });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo2_e, spr_weapon_sword_verdigris_tool_combo2_e });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo2_n, spr_weapon_sword_verdigris_tool_combo2_n });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo2_s, spr_weapon_sword_verdigris_tool_combo2_s });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo3_e, spr_weapon_sword_verdigris_tool_combo3_e });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo3_n, spr_weapon_sword_verdigris_tool_combo3_n });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo3_s, spr_weapon_sword_verdigris_tool_combo3_s });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_down_attack_e, spr_weapon_sword_verdigris_tool_down_attack_e });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_ui_item_tool_scrap_metal_sword, spr_ui_item_tool_verdigris_sword });
	}
	else if (floor_number < 60)
	{
		RValue spr_weapon_sword_crystal_tool_combo1_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_crystal_tool_combo1_e" });
		RValue spr_weapon_sword_crystal_tool_combo1_n = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_crystal_tool_combo1_n" });
		RValue spr_weapon_sword_crystal_tool_combo1_s = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_crystal_tool_combo1_s" });
		RValue spr_weapon_sword_crystal_tool_combo2_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_crystal_tool_combo2_e" });
		RValue spr_weapon_sword_crystal_tool_combo2_n = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_crystal_tool_combo2_n" });
		RValue spr_weapon_sword_crystal_tool_combo2_s = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_crystal_tool_combo2_s" });
		RValue spr_weapon_sword_crystal_tool_combo3_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_crystal_tool_combo3_e" });
		RValue spr_weapon_sword_crystal_tool_combo3_n = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_crystal_tool_combo3_n" });
		RValue spr_weapon_sword_crystal_tool_combo3_s = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_crystal_tool_combo3_s" });
		RValue spr_weapon_sword_crystal_tool_down_attack_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_crystal_tool_down_attack_e" });
		RValue spr_ui_item_tool_crystal_sword = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_tool_crystal_sword" });

		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo1_e, spr_weapon_sword_crystal_tool_combo1_e });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo1_n, spr_weapon_sword_crystal_tool_combo1_n });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo1_s, spr_weapon_sword_crystal_tool_combo1_s });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo2_e, spr_weapon_sword_crystal_tool_combo2_e });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo2_n, spr_weapon_sword_crystal_tool_combo2_n });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo2_s, spr_weapon_sword_crystal_tool_combo2_s });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo3_e, spr_weapon_sword_crystal_tool_combo3_e });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo3_n, spr_weapon_sword_crystal_tool_combo3_n });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo3_s, spr_weapon_sword_crystal_tool_combo3_s });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_down_attack_e, spr_weapon_sword_crystal_tool_down_attack_e });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_ui_item_tool_scrap_metal_sword, spr_ui_item_tool_crystal_sword });
	}
	else
	{
		RValue spr_weapon_sword_tarnished_gold_tool_combo1_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_tarnished_gold_tool_combo1_e" });
		RValue spr_weapon_sword_tarnished_gold_tool_combo1_n = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_tarnished_gold_tool_combo1_n" });
		RValue spr_weapon_sword_tarnished_gold_tool_combo1_s = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_tarnished_gold_tool_combo1_s" });
		RValue spr_weapon_sword_tarnished_gold_tool_combo2_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_tarnished_gold_tool_combo2_e" });
		RValue spr_weapon_sword_tarnished_gold_tool_combo2_n = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_tarnished_gold_tool_combo2_n" });
		RValue spr_weapon_sword_tarnished_gold_tool_combo2_s = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_tarnished_gold_tool_combo2_s" });
		RValue spr_weapon_sword_tarnished_gold_tool_combo3_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_tarnished_gold_tool_combo3_e" });
		RValue spr_weapon_sword_tarnished_gold_tool_combo3_n = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_tarnished_gold_tool_combo3_n" });
		RValue spr_weapon_sword_tarnished_gold_tool_combo3_s = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_tarnished_gold_tool_combo3_s" });
		RValue spr_weapon_sword_tarnished_gold_tool_down_attack_e = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_weapon_sword_tarnished_gold_tool_down_attack_e" });
		RValue spr_ui_item_tool_tarnished_gold_sword = g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_tool_tarnished_gold_sword" });

		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo1_e, spr_weapon_sword_tarnished_gold_tool_combo1_e });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo1_n, spr_weapon_sword_tarnished_gold_tool_combo1_n });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo1_s, spr_weapon_sword_tarnished_gold_tool_combo1_s });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo2_e, spr_weapon_sword_tarnished_gold_tool_combo2_e });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo2_n, spr_weapon_sword_tarnished_gold_tool_combo2_n });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo2_s, spr_weapon_sword_tarnished_gold_tool_combo2_s });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo3_e, spr_weapon_sword_tarnished_gold_tool_combo3_e });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo3_n, spr_weapon_sword_tarnished_gold_tool_combo3_n });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_combo3_s, spr_weapon_sword_tarnished_gold_tool_combo3_s });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_weapon_sword_scrap_metal_tool_down_attack_e, spr_weapon_sword_tarnished_gold_tool_down_attack_e });
		g_ModuleInterface->CallBuiltin("sprite_assign", { spr_ui_item_tool_scrap_metal_sword, spr_ui_item_tool_tarnished_gold_sword });
	}
}

void ScaleMistpoolWeapon(bool in_dungeon)
{
	RValue item_data = *global_instance->GetRefMember("__item_data");
	
	RValue* sword_scrap_metal;
	g_ModuleInterface->GetArrayEntry(item_data, mistpool_gear_to_item_id_map["sword_scrap_metal"], sword_scrap_metal);

	if (in_dungeon)
	{
		double damage = std::trunc(floor_number / 4.0) + 3;
		*sword_scrap_metal->GetRefMember("damage") = damage;
	}
	else
		*sword_scrap_metal->GetRefMember("damage") = 0;
}

void ScaleMistpoolArmor(bool in_dungeon)
{
	std::vector<std::string> mistpool_armor_names = { "scrap_metal_helmet", "scrap_metal_chestpiece", "scrap_metal_pants", "scrap_metal_boots", "scrap_metal_ring" };

	RValue item_data = *global_instance->GetRefMember("__item_data");
	for (std::string mistpool_armor_name : mistpool_armor_names)
	{
		RValue* mistpool_armor_piece;
		g_ModuleInterface->GetArrayEntry(item_data, mistpool_gear_to_item_id_map[mistpool_armor_name], mistpool_armor_piece);

		if (in_dungeon)
		{
			double defense = std::trunc(floor_number / 20.0);
			*mistpool_armor_piece->GetRefMember("defense") = defense;
		}
		else
			*mistpool_armor_piece->GetRefMember("defense") = 0;
	}
}

std::map<ClassArmor, int> CountEquippedClassArmor()
{
	RValue ari = global_instance->GetMember("__ari");
	RValue armor = ari.GetMember("armor");
	RValue slots = armor.GetMember("slots");
	RValue buffer = slots.GetMember("__buffer");

	size_t array_length;
	g_ModuleInterface->GetArraySize(buffer, array_length);

	std::map<ClassArmor, int> class_armor_equipped = {};
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_entry;
		g_ModuleInterface->GetArrayEntry(buffer, i, array_entry);

		if (StructVariableExists(*array_entry, "item"))
		{
			RValue item = array_entry->GetMember("item");
			if (StructVariableExists(item, "prototype"))
			{
				RValue prototype = item.GetMember("prototype");
				if (StructVariableExists(prototype, "recipe_key"))
				{
					RValue recipe_key = prototype.GetMember("recipe_key");
					for (const auto& class_armor : CLASS_ARMOR_TO_NAMES_MAP)
					{
						if (CLASS_ARMOR_TO_NAMES_MAP.at(class_armor.first).contains(recipe_key.ToString()))
							class_armor_equipped[class_armor.first]++;
					}
				}
			}
		}
	}

	return class_armor_equipped;
}

std::map<int, int> GetClassArmorInfusions()
{
	std::map<int, int> class_armor_infusions = {};

	RValue ari = global_instance->GetMember("__ari");
	RValue armor = ari.GetMember("armor");
	RValue slots = armor.GetMember("slots");
	RValue buffer = slots.GetMember("__buffer");

	size_t array_length;
	g_ModuleInterface->GetArraySize(buffer, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_entry;
		g_ModuleInterface->GetArrayEntry(buffer, i, array_entry);

		if (StructVariableExists(*array_entry, "item"))
		{
			RValue item = array_entry->GetMember("item");
			if (StructVariableExists(item, "infusion") && StructVariableExists(item, "prototype"))
			{
				RValue infusion = item.GetMember("infusion");
				RValue prototype = item.GetMember("prototype");
				if (StructVariableExists(prototype, "recipe_key") && CLASS_ARMOR_NAMES.contains(prototype.GetMember("recipe_key").ToString()))
					class_armor_infusions[infusion.ToInt64()]++;
			}
		}
	}
	
	return class_armor_infusions;
}

void LoadSpells()
{
	size_t array_length = 0;
	RValue spells = global_instance->GetMember("__spells");
	g_ModuleInterface->GetArraySize(spells, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(spells, i, array_element);

		spell_id_to_default_cost_map[i] = array_element->GetMember("cost").ToInt64();
	}
}

void ModifySpellCosts(bool reset_cost) {
	size_t array_length = 0;
	RValue spells = global_instance->GetMember("__spells");
	g_ModuleInterface->GetArraySize(spells, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(spells, i, array_element);

		int cost = reset_cost ? spell_id_to_default_cost_map[i] : spell_id_to_default_cost_map[i] - static_cast<int>(spell_id_to_default_cost_map[i] * 0.5);
		*array_element->GetRefMember("cost") = cost;
	}
}

RValue LocalizeString(CInstance* Self, CInstance* Other, std::string localization_key)
{
	CScript* gml_script_get_localizer = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_LOCALIZER,
		(PVOID*)&gml_script_get_localizer
	);

	RValue result;
	RValue input = RValue(localization_key);
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

void SpawnMonster(CInstance* Self, CInstance* Other, int room_x, int room_y, int monster_id)
{
	CScript* gml_script_spawn_monster = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SPAWN_MONSTER,
		(PVOID*)&gml_script_spawn_monster
	);

	RValue x = room_x;
	RValue y = room_y;
	RValue monster = monster_id;

	RValue result;
	RValue* x_ptr = &x;
	RValue* y_ptr = &y;
	RValue* monster_ptr = &monster;
	RValue* arguments[3] = { x_ptr, y_ptr, monster_ptr };

	gml_script_spawn_monster->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		3,
		arguments
	);
}

void CreateNotification(std::string notification_localization_str, CInstance* Self, CInstance* Other)
{
	uint64_t current_system_time = GetCurrentSystemTime();
	if (current_system_time > notification_name_to_last_display_time_map[notification_localization_str] + 5000)
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

void PlayConversation(std::string conversation_localization_str, CInstance* Self, CInstance* Other)
{
	CScript* gml_script_play_conversation = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_PLAY_CONVERSATION,
		(PVOID*)&gml_script_play_conversation
	);

	RValue zero = 0;
	RValue conversation = RValue(conversation_localization_str);
	RValue undefined;

	RValue* zero_ptr = &zero;
	RValue* conversation_ptr = &conversation;
	RValue* undefined_ptr = &undefined;

	RValue result;
	RValue* arguments[4] = { zero_ptr, conversation_ptr, undefined_ptr, undefined_ptr };

	gml_script_play_conversation->m_Functions->m_ScriptFunction(
		Self,
		Self,
		result,
		4,
		arguments
	);
}

void SpawnTutorial(std::string tutorial_name, CInstance* Self, CInstance* Other)
{
	CScript* gml_script_create_notification = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SPAWN_TUTORIAL,
		(PVOID*)&gml_script_create_notification
	);

	RValue result;
	RValue notification = tutorial_name_to_id_map[tutorial_name];
	RValue* notification_ptr = &notification;
	gml_script_create_notification->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &notification_ptr }
	);
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

RValue DeserializeLiveItem(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_deserialize_live_item = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_DESERIALIZE_LIVE_ITEM,
		(PVOID*)&gml_script_deserialize_live_item
	);

	std::map<std::string, RValue> rvalue_map = {
		{ "cosmetic", RValue() }, // UNDEFINED
		{ "item_id", RValue("sword_scrap_metal")}, // STRING
		{ "infusion", RValue() }, // UNDEFINED
		{ "animal_cosmetic", RValue() }, // UNDEFINED
		{ "date_photo", RValue() }, // UNDEFINED
		{ "inner_item", RValue() }, // UNDEFINED
		{ "gold_to_gain", RValue() }, // UNDEFINED
		{ "auto_use", false }, // BOOL
		{ "pet_cosmetic_set_name", RValue() } // UNDEFINED
	};

	RValue result;
	RValue input = rvalue_map;
	RValue* input_ptr = &input;
	gml_script_deserialize_live_item->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &input_ptr }
	);

	return result;
}

void DropItem(int item_id, double x_coord, double y_coord, CInstance* Self, CInstance* Other)
{
	RValue item = DeserializeLiveItem(Self, Other);
	*item.GetRefMember("prototype") = item_id_to_prototype_map[item_id];
	*item.GetRefMember("item_id") = item_id;

	CScript* gml_script_drop_item = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_DROP_ITEM,
		(PVOID*)&gml_script_drop_item
	);

	RValue x = x_coord;
	RValue y = y_coord;
	RValue undefined;

	RValue* item_ptr = &item;
	RValue* x_ptr = &x;
	RValue* y_ptr = &y;
	RValue* undefined_ptr = &undefined;

	RValue result;
	RValue* arguments[4] = { item_ptr, x_ptr, y_ptr, undefined_ptr };

	gml_script_drop_item->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		4,
		arguments
	);
}

void EnterDungeon(double dungeon_level, CInstance* Self, CInstance* Other)
{
	CScript* gml_script_enter_dungeon = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_ENTER_DUNGEON,
		(PVOID*)&gml_script_enter_dungeon
	);

	RValue result;
	RValue level = dungeon_level;
	RValue undefined;
	RValue* level_ptr = &level;
	RValue* undefined_ptr = &undefined;
	RValue* arguments[3] = { level_ptr, undefined_ptr, undefined_ptr };

	gml_script_enter_dungeon->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		3,
		arguments
	);
}

bool AriCurrentGmRoomIsDungeonFloor()
{
	return ari_current_gm_room.contains("rm_mines") && ari_current_gm_room != "rm_mines_entry" && !ari_current_gm_room.contains("seal");
}

void SetFloorNumber()
{
	if (ari_current_gm_room == "rm_mines_upper_floor1")
		floor_number = 1;
	else if (ari_current_gm_room == "rm_mines_upper_elevator5")
		floor_number = 5;
	else if (ari_current_gm_room == "rm_mines_upper_elevator10")
		floor_number = 10;
	else if (ari_current_gm_room == "rm_mines_upper_elevator15")
		floor_number = 15;
	else if (ari_current_gm_room == "rm_water_seal")
		floor_number = 20;
	else if (ari_current_gm_room == "rm_mines_tide_floor21")
		floor_number = 21;
	else if (ari_current_gm_room == "rm_mines_tide_elevator25")
		floor_number = 25;
	else if (ari_current_gm_room == "rm_mines_tide_elevator30")
		floor_number = 30;
	else if (ari_current_gm_room == "rm_mines_tide_elevator35")
		floor_number = 35;
	else if (ari_current_gm_room == "rm_earth_seal")
		floor_number = 40;
	else if (ari_current_gm_room == "rm_mines_deep_41")
		floor_number = 41;
	else if (ari_current_gm_room == "rm_mines_deep_45")
		floor_number = 45;
	else if (ari_current_gm_room == "rm_mines_deep_50")
		floor_number = 50;
	else if (ari_current_gm_room == "rm_mines_deep_55")
		floor_number = 55;
	else if (ari_current_gm_room == "rm_fire_seal")
		floor_number = 60;
	else if (ari_current_gm_room == "rm_mines_lava_61")
		floor_number = 61;
	else if (ari_current_gm_room == "rm_mines_lava_65")
		floor_number = 65;
	else if (ari_current_gm_room == "rm_mines_lava_70")
		floor_number = 70;
	else if (ari_current_gm_room == "rm_mines_lava_75")
		floor_number = 75;
	else if (ari_current_gm_room == "rm_ruins_seal")
		floor_number = 80;
	// TODO: Add Ruins floors when released.
	else
		floor_number++;
}

RValue GetDynamicItemSprite(int item_id)
{
	if (item_id == mistpool_gear_to_item_id_map[MISTPOOL_SWORD_NAME])
	{
		if (active_traps.contains(Traps::INHIBITING) || !AriCurrentGmRoomIsDungeonFloor())
		{
			if (floor_number < 20)
				return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_tool_rusty_sword_disabled" });
			else if (floor_number < 40)
				return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_tool_verdigris_sword_disabled" });
			else if (floor_number < 60)
				return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_tool_crystal_sword_disabled" });
			else
				return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_tool_tarnished_gold_sword_disabled" });
		}
		else
		{
			if (floor_number < 20)
				return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_tool_rusty_sword" });
			else if (floor_number < 40)
				return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_tool_verdigris_sword" });
			else if (floor_number < 60)
				return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_tool_crystal_sword" });
			else
				return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_tool_tarnished_gold_sword" });
		}
	}
	if (item_id == mistpool_gear_to_item_id_map[MISTPOOL_HELMET_NAME])
	{
		if (floor_number < 20)
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_helmet_tier_1" });
		else if (floor_number < 40)
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_helmet_tier_2" });
		else if (floor_number < 60)
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_helmet_tier_3" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_helmet_tier_4" });
	}
	if (item_id == mistpool_gear_to_item_id_map[MISTPOOL_CHESTPIECE_NAME])
	{
		if (floor_number < 20)
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_chestpiece_tier_1" });
		else if (floor_number < 40)
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_chestpiece_tier_2" });
		else if (floor_number < 60)
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_chestpiece_tier_3" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_chestpiece_tier_4" });
	}
	if (item_id == mistpool_gear_to_item_id_map[MISTPOOL_GLOVES_NAME])
	{
		if (floor_number < 20)
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_gloves_tier_1" });
		else if (floor_number < 40)
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_gloves_tier_2" });
		else if (floor_number < 60)
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_gloves_tier_3" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_gloves_tier_4" });
	}
	if (item_id == mistpool_gear_to_item_id_map[MISTPOOL_PANTS_NAME])
	{
		if (floor_number < 20)
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_pants_tier_1" });
		else if (floor_number < 40)
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_pants_tier_2" });
		else if (floor_number < 60)
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_pants_tier_3" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_pants_tier_4" });
	}
	if (item_id == mistpool_gear_to_item_id_map[MISTPOOL_BOOTS_NAME])
	{
		if (floor_number < 20)
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_boots_tier_1" });
		else if (floor_number < 40)
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_boots_tier_2" });
		else if (floor_number < 60)
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_boots_tier_3" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_equipment_mistpool_boots_tier_4" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::ALTERATION])
	{
		if (active_sigils.contains(Sigils::ALTERATION) || active_floor_enchantments.contains(FloorEnchantments::ITEM_PENALTY) || !AriCurrentGmRoomIsDungeonFloor())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_alteration_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_alteration" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::CONCEALMENT])
	{
		if (active_sigils.contains(Sigils::CONCEALMENT) || active_floor_enchantments.contains(FloorEnchantments::ITEM_PENALTY) || !AriCurrentGmRoomIsDungeonFloor())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_concealment_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_concealment" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::FORTIFICATION])
	{
		if (active_sigils.contains(Sigils::FORTIFICATION) || active_floor_enchantments.contains(FloorEnchantments::ITEM_PENALTY) || !AriCurrentGmRoomIsDungeonFloor())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_fortification_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_fortification" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::FORTUNE])
	{
		if (active_sigils.contains(Sigils::FORTUNE) || active_floor_enchantments.contains(FloorEnchantments::ITEM_PENALTY) || !AriCurrentGmRoomIsDungeonFloor())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_fortune_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_fortune" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::PROTECTION])
	{
		if (active_sigils.contains(Sigils::PROTECTION) || active_floor_enchantments.contains(FloorEnchantments::ITEM_PENALTY) || !AriCurrentGmRoomIsDungeonFloor())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_protection_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_protection" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::RAGE])
	{
		if (active_sigils.contains(Sigils::RAGE) || active_floor_enchantments.contains(FloorEnchantments::ITEM_PENALTY) || !AriCurrentGmRoomIsDungeonFloor())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_rage_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_rage" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::REDEMPTION])
	{
		if (active_sigils.contains(Sigils::REDEMPTION) || active_floor_enchantments.contains(FloorEnchantments::ITEM_PENALTY) || !AriCurrentGmRoomIsDungeonFloor())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_redemption_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_redemption" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::SAFETY])
	{
		if (active_sigils.contains(Sigils::SAFETY) || active_floor_enchantments.contains(FloorEnchantments::ITEM_PENALTY) || !AriCurrentGmRoomIsDungeonFloor())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_safety_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_safety" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::SERENITY])
	{
		if (active_sigils.contains(Sigils::SERENITY) || !AriCurrentGmRoomIsDungeonFloor())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_serenity_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_serenity" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::SILENCE])
	{
		if (active_sigils.contains(Sigils::SILENCE) || active_floor_enchantments.contains(FloorEnchantments::ITEM_PENALTY) || !AriCurrentGmRoomIsDungeonFloor())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_silence_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_silence" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::STRENGTH])
	{
		if (active_sigils.contains(Sigils::STRENGTH) || active_floor_enchantments.contains(FloorEnchantments::ITEM_PENALTY) || !AriCurrentGmRoomIsDungeonFloor())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_strength_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_strength" });
	}
	if (item_id == sigil_to_item_id_map[Sigils::TEMPTATION])
	{
		if (active_sigils.contains(Sigils::TEMPTATION) || active_floor_enchantments.contains(FloorEnchantments::ITEM_PENALTY) || !AriCurrentGmRoomIsDungeonFloor())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_temptation_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_sigil_of_temptation" });
	}
	if (item_id == salve_name_to_id_map[SUSTAINING_POTION_NAME])
	{
		if (active_floor_enchantments.contains(FloorEnchantments::ITEM_PENALTY) || !AriCurrentGmRoomIsDungeonFloor())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_potion_sustain_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_potion_sustain" });
	}
	if (item_id == salve_name_to_id_map[HEALTH_SALVE_NAME])
	{
		if (salves_used.contains(item_id) || active_floor_enchantments.contains(FloorEnchantments::ITEM_PENALTY) || !AriCurrentGmRoomIsDungeonFloor())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_salve_health_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_salve_health" });
	}
	if (item_id == salve_name_to_id_map[STAMINA_SALVE_NAME])
	{
		if (salves_used.contains(item_id) || active_floor_enchantments.contains(FloorEnchantments::ITEM_PENALTY) || !AriCurrentGmRoomIsDungeonFloor())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_salve_stamina_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_salve_stamina" });
	}
	if (item_id == salve_name_to_id_map[MANA_SALVE_NAME])
	{
		if (salves_used.contains(item_id) || active_floor_enchantments.contains(FloorEnchantments::ITEM_PENALTY) || !AriCurrentGmRoomIsDungeonFloor())
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_salve_mana_disabled" });
		else
			return g_ModuleInterface->CallBuiltin("asset_get_index", { "spr_ui_item_salve_mana" });
	}
}

std::unordered_set<FloorEnchantments> RandomFloorEnchantments(bool is_first_floor, DungeonBiomes dungeon_biome)
{
	static thread_local std::mt19937 random_generator(std::random_device{}());
	std::uniform_int_distribution<size_t> zero_to_ninety_nine_distribution(0, 99);

	std::unordered_set<FloorEnchantments> random_floor_enchantments = {};

	if (is_first_floor)
	{
		const std::vector<FloorEnchantments> FIRST_FLOOR_POSSIBLE_ENCHANTMENTS = {
			FloorEnchantments::RESTORATION, FloorEnchantments::SECOND_WIND, FloorEnchantments::HASTE
		};

		std::uniform_int_distribution<size_t> first_floor_distribution(0, FIRST_FLOOR_POSSIBLE_ENCHANTMENTS.size() - 1);
		return { FIRST_FLOOR_POSSIBLE_ENCHANTMENTS[first_floor_distribution(random_generator)]};
	}
	
	if (dungeon_biome == DungeonBiomes::UPPER)
	{
		// 50% chance for Group 1
		int group_one_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_one_chance < 50)
		{
			std::uniform_int_distribution<size_t> group_one_distribution(0, GROUP_ONE_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.insert(GROUP_ONE_FLOOR_ENCHANTMENTS[group_one_distribution(random_generator)]);
		}

		// 25% chance for Group 2
		int group_two_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_two_chance < 25)
		{
			std::uniform_int_distribution<size_t> group_two_distribution(0, GROUP_TWO_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.insert(GROUP_TWO_FLOOR_ENCHANTMENTS[group_two_distribution(random_generator)]);
		}
	}

	if (dungeon_biome == DungeonBiomes::TIDE_CAVERNS)
	{
		// 65% chance for Group 1
		int group_one_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_one_chance < 65)
		{
			std::uniform_int_distribution<size_t> group_one_distribution(0, GROUP_ONE_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.insert(GROUP_ONE_FLOOR_ENCHANTMENTS[group_one_distribution(random_generator)]);
		}

		// 40% chance for Group 2
		int group_two_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_two_chance < 40)
		{
			std::uniform_int_distribution<size_t> group_two_distribution(0, GROUP_TWO_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.insert(GROUP_TWO_FLOOR_ENCHANTMENTS[group_two_distribution(random_generator)]);
		}

		// 25% chance for Group 3
		int group_three_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_three_chance < 25)
		{
			int gloom_chance = zero_to_ninety_nine_distribution(random_generator);
			if(gloom_chance < 60) // 60% chance for Gloom
				random_floor_enchantments.insert(FloorEnchantments::GLOOM);
			else // 40% chance for Fey
				random_floor_enchantments.insert(FloorEnchantments::FEY);
		}
	}

	if (dungeon_biome == DungeonBiomes::DEEP_EARTH)
	{
		// 45% chance for Group 1
		int group_one_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_one_chance < 45)
		{
			std::uniform_int_distribution<size_t> group_one_distribution(0, GROUP_ONE_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.insert(GROUP_ONE_FLOOR_ENCHANTMENTS[group_one_distribution(random_generator)]);
		}

		// 65% chance for Group 2
		int group_two_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_two_chance < 65)
		{
			std::uniform_int_distribution<size_t> group_two_distribution(0, GROUP_TWO_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.insert(GROUP_TWO_FLOOR_ENCHANTMENTS[group_two_distribution(random_generator)]);
		}

		// 30% chance for Group 3
		int group_three_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_three_chance < 30)
		{
			int gloom_chance = zero_to_ninety_nine_distribution(random_generator);
			if (gloom_chance < 70) // 70% chance for Gloom
				random_floor_enchantments.insert(FloorEnchantments::GLOOM);
			else // 30% chance for Fey
				random_floor_enchantments.insert(FloorEnchantments::FEY);
		}
	}

	if (dungeon_biome == DungeonBiomes::LAVA_CAVES)
	{
		// 60% chance for Group 1
		int group_one_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_one_chance < 60)
		{
			std::uniform_int_distribution<size_t> group_one_distribution(0, GROUP_ONE_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.insert(GROUP_ONE_FLOOR_ENCHANTMENTS[group_one_distribution(random_generator)]);
		}

		// 75% chance for Group 2
		int group_two_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_two_chance < 75)
		{
			std::uniform_int_distribution<size_t> group_two_distribution(0, GROUP_TWO_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.insert(GROUP_TWO_FLOOR_ENCHANTMENTS[group_two_distribution(random_generator)]);
		}

		// 35% chance for Group 3
		int group_three_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_three_chance < 35)
		{
			int gloom_chance = zero_to_ninety_nine_distribution(random_generator);
			if (gloom_chance < 80) // 80% chance for Gloom
				random_floor_enchantments.insert(FloorEnchantments::GLOOM);
			else // 20% chance for Fey
				random_floor_enchantments.insert(FloorEnchantments::FEY);
		}
	}

	if (dungeon_biome == DungeonBiomes::RUINS)
	{
		// 65% chance for Group 1
		int group_one_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_one_chance < 65)
		{
			std::uniform_int_distribution<size_t> group_one_distribution(0, GROUP_ONE_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.insert(GROUP_ONE_FLOOR_ENCHANTMENTS[group_one_distribution(random_generator)]);
		}

		// 75% chance for Group 2
		int group_two_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_two_chance < 75)
		{
			std::uniform_int_distribution<size_t> group_two_distribution(0, GROUP_TWO_FLOOR_ENCHANTMENTS.size() - 1);
			random_floor_enchantments.insert(GROUP_TWO_FLOOR_ENCHANTMENTS[group_two_distribution(random_generator)]);
		}

		// 40% chance for Group 3
		int group_three_chance = zero_to_ninety_nine_distribution(random_generator);
		if (group_three_chance < 40)
		{
			int gloom_chance = zero_to_ninety_nine_distribution(random_generator);
			if (gloom_chance < 90) // 90% chance for Gloom
				random_floor_enchantments.insert(FloorEnchantments::GLOOM);
			else // 10% chance for Fey
				random_floor_enchantments.insert(FloorEnchantments::FEY);
		}
	}

	return random_floor_enchantments;
}

void GenerateFloorTraps()
{
	if (TRAP_SPAWN_POINTS.contains(ari_current_gm_room))
	{
		static thread_local std::mt19937 random_generator(std::random_device{}());
		std::vector<std::pair<int, int>> spawn_points = TRAP_SPAWN_POINTS.at(ari_current_gm_room);

		if (spawn_points.empty())
			return;

		int min_traps = 2;
		int biome_adjusted_max_traps = min(6, std::trunc(floor_number / 20) + 2); // Scale the number of traps per floor with progression. Capped at 6.

		std::uniform_int_distribution<size_t> traps_for_room_distribution(min_traps, biome_adjusted_max_traps);
		int random_trap_count = traps_for_room_distribution(random_generator);

		if (active_offerings.contains(Offerings::PERIL))
		{
			random_trap_count += 2; // PERIL adds two traps
			if (random_trap_count > 6)
				random_trap_count = 6; // Capped at 6
		}

		int count = 0;
		for (int i = 0; i < random_trap_count; i++)
		{
			std::uniform_int_distribution<size_t> random_number_of_traps_for_current_room(0, spawn_points.size() - 1);
			int random_index = random_number_of_traps_for_current_room(random_generator);
			floor_trap_positions.insert(spawn_points[random_index]);
			spawn_points.erase(spawn_points.begin() + random_index);
		}
	}
}

std::vector<int> GenerateRandomMonstersIdsForCurrentFloor(int min, int max)
{
	static thread_local std::mt19937 random_generator(std::random_device{}());
	std::uniform_int_distribution<size_t> random_monster_count_distribution(min, max);
	std::vector<int> candidate_monsters(dungeon_biome_to_candidate_monsters_map[floor_number_to_biome_name_map[floor_number]].begin(), dungeon_biome_to_candidate_monsters_map[floor_number_to_biome_name_map[floor_number]].end());

	std::vector<int> random_monsters = {};
	int monsters_to_spawn = random_monster_count_distribution(random_generator);
	
	for (int i = 0; i < monsters_to_spawn; i++)
	{
		std::uniform_int_distribution<size_t> random_monster_distribution(0, candidate_monsters.size() - 1);
		random_monsters.push_back(candidate_monsters[random_monster_distribution(random_generator)]);
	}

	return random_monsters;
}

void CancelStatusEffect(CInstance* Self, CInstance* Other, RValue status_effect_id)
{
	CScript* gml_script_cancel_status_effect = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CANCEL_STATUS_EFFECT,
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

void CancelAllStatusEffects()
{
	std::vector<CInstance*> refs = script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE];

	for (int i = 0; i <= status_effect_name_to_id_map.size(); i++)
		CancelStatusEffect(refs[0], refs[1], i);
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

RValue GetMaxHealth(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_get_max_health = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_MAX_HEALTH,
		(PVOID*)&gml_script_get_max_health
	);

	RValue result;
	gml_script_get_max_health->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		0,
		nullptr
	);

	return result;
}

RValue GetHealth(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_get_health = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_HEALTH,
		(PVOID*)&gml_script_get_health
	);

	RValue result;
	gml_script_get_health->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		0,
		nullptr
	);

	return result;
}

void SetHealth(CInstance* Self, CInstance* Other, int value)
{
	CScript* gml_script_set_health = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SET_HEALTH,
		(PVOID*)&gml_script_set_health
	);

	RValue result;
	RValue health_modifier = value;
	RValue* health_modifier_ptr = &health_modifier;

	gml_script_set_health->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &health_modifier_ptr }
	);
}

void ModifyHealth(CInstance* Self, CInstance* Other, int value)
{
	CScript* gml_script_modify_health = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_MODIFY_HEALTH,
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

RValue GetStamina(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_get_stamina = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_STAMINA,
		(PVOID*)&gml_script_get_stamina
	);

	RValue result;
	gml_script_get_stamina->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		0,
		nullptr
	);

	return result;
}

void ModifyStamina(CInstance* Self, CInstance* Other, int value)
{
	CScript* gml_script_modify_stamina = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_MODIFY_STAMINA,
		(PVOID*)&gml_script_modify_stamina
	);

	RValue result;
	RValue stamina_modifier = value;
	RValue* stamina_modifier_ptr = &stamina_modifier;

	gml_script_modify_stamina->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &stamina_modifier_ptr }
	);
}

RValue GetMana(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_get_mana = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_MANA,
		(PVOID*)&gml_script_get_mana
	);

	RValue result;
	gml_script_get_mana->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		0,
		nullptr
	);

	return result;
}

void ModifyMana(CInstance* Self, CInstance* Other, int value)
{
	CScript* gml_script_modify_mana = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_MODIFY_MANA,
		(PVOID*)&gml_script_modify_mana
	);

	RValue result;
	RValue mana_modifier = value;
	RValue* mana_modifier_ptr = &mana_modifier;

	gml_script_modify_mana->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &mana_modifier_ptr }
	);
}

void CastSpell(CInstance* Self, CInstance* Other, int spell_id)
{
	CScript* gml_script_cast_spell = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CAST_SPELL,
		(PVOID*)&gml_script_cast_spell
	);

	RValue result;
	RValue health_modifier = spell_id;
	RValue* health_modifier_ptr = &health_modifier;

	gml_script_cast_spell->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &health_modifier_ptr }
	);
}

void ApplyFloorTraps(CInstance* Self, CInstance* Other)
{
	// Prune traps that have fully applied.
	if (active_traps.contains(Traps::CONFUSING) && active_traps_to_value_map[Traps::CONFUSING] < current_time_in_seconds)
	{
		g_ModuleInterface->Print(CM_LIGHTPURPLE, "Confusing Trap Ended: %d", current_time_in_seconds);
		active_traps.erase(Traps::CONFUSING);
		active_traps_to_value_map.erase(Traps::CONFUSING);
	}
	if (active_traps.contains(Traps::DISORIENTING) && active_traps_to_value_map[Traps::DISORIENTING] < current_time_in_seconds)
	{
		g_ModuleInterface->Print(CM_LIGHTPURPLE, "Disorienting Trap Ended: %d", current_time_in_seconds);
		active_traps.erase(Traps::DISORIENTING);
		active_traps_to_value_map.erase(Traps::DISORIENTING);
	}
	if (active_traps.contains(Traps::INHIBITING) && active_traps_to_value_map[Traps::INHIBITING] < current_time_in_seconds)
	{
		g_ModuleInterface->Print(CM_LIGHTPURPLE, "Inhibiting Trap Ended: %d", current_time_in_seconds);
		active_traps.erase(Traps::INHIBITING);
		active_traps_to_value_map.erase(Traps::INHIBITING);

		if (script_name_to_reference_map.contains(GML_SCRIPT_UPDATE_TOOLBAR_MENU))
			UpdateToolbarMenu(script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][0], script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][1]);
	}

	// Process traps in range.
	for (auto floor_trap = floor_trap_positions.begin(); floor_trap != floor_trap_positions.end();) {
		double distance = GetDistance(ari_x, ari_y, floor_trap->first, floor_trap->second);
		if (distance <= 16)
		{
			static thread_local std::mt19937 random_generator(std::random_device{}());
			std::uniform_int_distribution<size_t> random_trap_distribution(0, magic_enum::enum_count<Traps>() - 1);
			
			Traps trap = magic_enum::enum_value<Traps>(random_trap_distribution(random_generator));
			active_traps.insert(trap);
			
			if (trap == Traps::CONFUSING)
			{
				PlaySoundEffect("snd_bark_o_o", 100);
				CreateNotification(CONFUSING_TRAP_NOTIFICATION_KEY, Self, Other);
				g_ModuleInterface->Print(CM_LIGHTPURPLE, "Confusing Trap Started: %d", current_time_in_seconds);

				if (!active_traps_to_value_map.contains(Traps::CONFUSING))
					active_traps_to_value_map[Traps::CONFUSING] = current_time_in_seconds + 1200; // 20m
				else
					active_traps_to_value_map[Traps::CONFUSING] += 1200;
			}
			if (trap == Traps::DISORIENTING)
			{
				PlaySoundEffect("snd_interactable_scan", 100);
				CreateNotification(DISORIENTING_TRAP_NOTIFICATION_KEY, Self, Other);
				g_ModuleInterface->Print(CM_LIGHTPURPLE, "Disorienting Trap Started: %d", current_time_in_seconds);

				if (!active_traps_to_value_map.contains(Traps::DISORIENTING))
					active_traps_to_value_map[Traps::DISORIENTING] = current_time_in_seconds + 600; // 10m
				else
					active_traps_to_value_map[Traps::DISORIENTING] += 600;
			}
			if (trap == Traps::EXPLODING)
			{
				PlaySoundEffect("snd_Explosion_CaveReverb", 100);
				CreateNotification(EXPLODING_TRAP_NOTIFICATION_KEY, Self, Other);
			}
			if (trap == Traps::INHIBITING)
			{
				PlaySoundEffect("snd_bark_surprised", 100);
				CreateNotification(INHIBITING_TRAP_NOTIFICATION_KEY, Self, Other);
				g_ModuleInterface->Print(CM_LIGHTPURPLE, "Inhibiting Trap Started: %d", current_time_in_seconds);

				if (script_name_to_reference_map.contains(GML_SCRIPT_UPDATE_TOOLBAR_MENU))
					UpdateToolbarMenu(script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][0], script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][1]);

				if (!active_traps_to_value_map.contains(Traps::INHIBITING))
					active_traps_to_value_map[Traps::INHIBITING] = current_time_in_seconds + 900; // 15m
				else
					active_traps_to_value_map[Traps::INHIBITING] += 900;
			}
			if (trap == Traps::LURING)
			{
				PlaySoundEffect("snd_ScrollRaise", 100);
				CreateNotification(LURING_TRAP_NOTIFICATION_KEY, Self, Other);
				std::vector<int> random_monsters = GenerateRandomMonstersIdsForCurrentFloor(2, 2); // TODO: Scale the number of monsters spawned?
				std::uniform_int_distribution<int> random_position_offset_distribution(-12, 12);

				for (int i = 0; i < random_monsters.size(); i++)
					SpawnMonster(Self, Other, floor_trap->first + random_position_offset_distribution(random_generator), floor_trap->second + random_position_offset_distribution(random_generator), random_monsters[i]);
			}

			floor_trap = floor_trap_positions.erase(floor_trap);
		}
		else 
			++floor_trap;
	}
}

void ApplyOfferingPenalties(CInstance* Self, CInstance* Other)
{
	if (ari_resource_to_penalty_map[AriResources::HEALTH])
		ModifyHealth(Self, Other, -25);
	if (ari_resource_to_penalty_map[AriResources::STAMINA])
		ModifyStamina(Self, Other, -20);
	if (ari_resource_to_penalty_map[AriResources::MANA])
		ModifyMana(Self, Other, -1);

	ari_resource_to_penalty_map.clear();
}

void TrackAriResources(CInstance* Self, CInstance* Other)
{
	ari_resource_to_value_map[AriResources::HEALTH] = GetHealth(Self, Other).ToInt64();
	ari_resource_to_value_map[AriResources::STAMINA] = GetStamina(Self, Other).ToInt64();
	ari_resource_to_value_map[AriResources::MANA] = GetMana(Self, Other).ToInt64();
}

void GenerateTreasureChestLoot(std::string object_name, CInstance* Self, CInstance* Other)
{
	static thread_local std::mt19937 random_generator(std::random_device{}());
	std::uniform_int_distribution<size_t> zero_to_nintey_nine_distribution(0, 99);
	std::uniform_int_distribution<size_t> random_sigil_distribution(0, magic_enum::enum_count<Sigils>() - 1);

	// Sigils
	std::vector<int> sigil_roll_success_thresholds = {};
	if (object_name == TREASURE_CHEST_WOOD_NAME)
		sigil_roll_success_thresholds = { 50, 25, 0, 0 };
	else if (object_name == TREASURE_CHEST_COPPER_NAME)
		sigil_roll_success_thresholds = { 75, 50, 10, 0 };
	else if (object_name == TREASURE_CHEST_SILVER_NAME)
		sigil_roll_success_thresholds = { 100, 50, 25, 0 };
	else if (object_name == TREASURE_CHEST_GOLD_NAME)
		sigil_roll_success_thresholds = { 100, 100, 25, 10 };

	std::unordered_set<Sigils> sigils_spawned = {};
	for (size_t i = 0; i < sigil_roll_success_thresholds.size(); i++)
	{
		int roll_for_drop = zero_to_nintey_nine_distribution(random_generator);
		if (roll_for_drop < sigil_roll_success_thresholds[i])
		{
			Sigils random_sigil = magic_enum::enum_value<Sigils>(random_sigil_distribution(random_generator));
			while(sigils_spawned.contains(random_sigil))
				random_sigil = magic_enum::enum_value<Sigils>(random_sigil_distribution(random_generator));

			sigils_spawned.insert(random_sigil);
			DropItem(sigil_to_item_id_map[random_sigil], ari_x, ari_y, Self, Other);
		}
	}

	// Cursed Armor
	int cursed_armor_roll_success_threshold = 0;
	if (object_name == TREASURE_CHEST_WOOD_NAME)
		cursed_armor_roll_success_threshold = 1;
	else if (object_name == TREASURE_CHEST_COPPER_NAME)
		cursed_armor_roll_success_threshold = 2;
	else if (object_name == TREASURE_CHEST_SILVER_NAME)
		cursed_armor_roll_success_threshold = 3;
	else if (object_name == TREASURE_CHEST_GOLD_NAME)
		cursed_armor_roll_success_threshold = 4;

	int roll_for_drop = zero_to_nintey_nine_distribution(random_generator);
	if (roll_for_drop < cursed_armor_roll_success_threshold)
	{
		if (floor_number < 20) // Upper Mines
			DropItem(cursed_gear_to_item_id_map[CURSED_CHESTPIECE_NAME], ari_x, ari_y, Self, Other);
		else if (floor_number < 40) // Tide Caverns
			DropItem(cursed_gear_to_item_id_map[CURSED_HELMET_NAME], ari_x, ari_y, Self, Other);
		else if (floor_number < 60) // Deep Earth
			DropItem(cursed_gear_to_item_id_map[CURSED_GLOVES_NAME], ari_x, ari_y, Self, Other);
		else if (floor_number < 80) // Lava Caves
			DropItem(cursed_gear_to_item_id_map[CURSED_PANTS_NAME], ari_x, ari_y, Self, Other);
		else if (floor_number < 100) // Ruins
			DropItem(cursed_gear_to_item_id_map[CURSED_BOOTS_NAME], ari_x, ari_y, Self, Other);
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

		// Progression Mode Biome Cleared Rewards
		if (progression_mode && biome_reward && ari_x != 0 && ari_y != 0 && script_name_to_reference_map.contains(GML_SCRIPT_DROP_ITEM))
		{
			// Upper Mines
			if (ari_current_gm_room == "rm_water_seal") 
			{
				biome_reward = false;
				DropItem(cursed_gear_to_item_id_map[CURSED_CHESTPIECE_NAME], ari_x, ari_y, script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][0], script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][1]);
				DropItem(item_name_to_id_map[TIDE_CAVERNS_KEY_NAME], ari_x, ari_y, script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][0], script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][1]);
			}
			// Tide Caverns
			else if (ari_current_gm_room == "rm_earth_seal")
			{
				biome_reward = false;
				DropItem(cursed_gear_to_item_id_map[CURSED_HELMET_NAME], ari_x, ari_y, script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][0], script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][1]);
				DropItem(item_name_to_id_map[DEEP_EARTH_KEY_NAME], ari_x, ari_y, script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][0], script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][1]);
			}
			// Deep Earth
			else if (ari_current_gm_room == "rm_fire_seal")
			{
				biome_reward = false;
				DropItem(cursed_gear_to_item_id_map[CURSED_GLOVES_NAME], ari_x, ari_y, script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][0], script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][1]);
				DropItem(item_name_to_id_map[LAVA_CAVES_KEY_NAME], ari_x, ari_y, script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][0], script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][1]);
			}
			// Lava Caves
			else if (ari_current_gm_room == "rm_ruins_seal") 
			{
				biome_reward = false;
				DropItem(cursed_gear_to_item_id_map[CURSED_PANTS_NAME], ari_x, ari_y, script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][0], script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][1]);
				DropItem(item_name_to_id_map[RUINS_KEY_NAME], ari_x, ari_y, script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][0], script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][1]);

			}
			// Ruins
			else if (ari_current_gm_room == "not_yet_implemented") // TODO: Use room name when its implemented
			{
				biome_reward = false;
				DropItem(cursed_gear_to_item_id_map[CURSED_BOOTS_NAME], ari_x, ari_y, script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][0], script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][1]);
				// TODO: Drop some special kind of key?
			}
		}

		// Floor Traps
		if (active_traps.contains(Traps::EXPLODING))
		{
			RValue health = GetHealth(global_instance->GetRefMember("__ari")->ToInstance(), self);
			int penalty = std::trunc(health.ToDouble() * 0.8);
			SetHealth(global_instance->GetRefMember("__ari")->ToInstance(), self, health.ToDouble() - penalty);
			active_traps.erase(Traps::EXPLODING);

			for (CInstance* monster : current_floor_monsters)
			{
				if (StructVariableExists(monster, "hit_points") && StructVariableExists(monster, "move"))
				{
					RValue monster_x;
					RValue monster_y;
					g_ModuleInterface->GetBuiltin("x", monster, NULL_INDEX, monster_x);
					g_ModuleInterface->GetBuiltin("y", monster, NULL_INDEX, monster_y);

					// TODO: See if it's possible to directly check the distance between the trap and the monsters.
					double distance = GetDistance(ari_x, ari_y, monster_x.ToInt64(), monster_y.ToInt64());
					if (distance <= 32)
					{
						double hit_points = monster->GetMember("hit_points").ToDouble();
						if (std::isfinite(hit_points))
						{
							int monster_hp_penalty = std::trunc(hit_points * 0.8);
							*monster->GetRefMember("hit_points") = max(0, hit_points - monster_hp_penalty);
						}						
					}
				}
			}
		}
		
		ApplyOfferingPenalties(global_instance->GetRefMember("__ari")->ToInstance(), self);

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
								if (sigil_item_used) // TODO: This might not be neccessary
								{
									sigil_item_used = false;

									if (held_item_id == sigil_to_item_id_map[Sigils::ALTERATION])
										active_sigils.insert(Sigils::ALTERATION);
									if (held_item_id == sigil_to_item_id_map[Sigils::CONCEALMENT])
										active_sigils.insert(Sigils::CONCEALMENT);
									if (held_item_id == sigil_to_item_id_map[Sigils::FORTIFICATION])
										active_sigils.insert(Sigils::FORTIFICATION);
									if (held_item_id == sigil_to_item_id_map[Sigils::FORTUNE])
									{
										active_sigils.insert(Sigils::FORTUNE);
										SpawnLadder(global_instance->GetRefMember("__ari")->ToInstance(), self, ari_x, ari_y);
									}
									if (held_item_id == sigil_to_item_id_map[Sigils::PROTECTION])
									{
										std::vector<CInstance*> refs = script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE];

										active_sigils.insert(Sigils::PROTECTION);
										RegisterStatusEffect(refs[0], refs[1], status_effect_name_to_id_map["guardians_shield"], RValue(), 1, 2147483647.0);
										SetInvulnerabilityHits(2);
									}
									if (held_item_id == sigil_to_item_id_map[Sigils::RAGE])
										active_sigils.insert(Sigils::RAGE);
									if (held_item_id == sigil_to_item_id_map[Sigils::REDEMPTION])
									{
										std::vector<CInstance*> refs = script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE];

										active_sigils.insert(Sigils::REDEMPTION);
										RegisterStatusEffect(refs[0], refs[1], status_effect_name_to_id_map["fairy"], RValue(), 1, 2147483647.0);
									}
									if (held_item_id == sigil_to_item_id_map[Sigils::SAFETY])
									{
										floor_trap_positions.clear();
										active_sigils.insert(Sigils::SAFETY);
									}
									if (held_item_id == sigil_to_item_id_map[Sigils::SERENITY])
									{
										// TODO: Check for other Floor Enchantments and Offering effects to cancel as implemented
										if (active_floor_enchantments.contains(FloorEnchantments::FEY))
										{
											std::vector<CInstance*> refs = script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE];

											ModifySpellCosts(true);
											CancelStatusEffect(refs[0], refs[1], status_effect_name_to_id_map["fairy"]);
										}

										active_floor_enchantments.clear();
										active_sigils.insert(Sigils::SERENITY);
									}
									if (held_item_id == sigil_to_item_id_map[Sigils::SILENCE])
										active_sigils.insert(Sigils::SILENCE);
									if (held_item_id == sigil_to_item_id_map[Sigils::STRENGTH])
										active_sigils.insert(Sigils::STRENGTH);
									if (held_item_id == sigil_to_item_id_map[Sigils::TEMPTATION])
										active_sigils.insert(Sigils::TEMPTATION);

									if (script_name_to_reference_map.contains(GML_SCRIPT_UPDATE_TOOLBAR_MENU))
										UpdateToolbarMenu(script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][0], script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][1]);
								}
								else if (held_item_id == salve_name_to_id_map[HEALTH_SALVE_NAME] || held_item_id == salve_name_to_id_map[STAMINA_SALVE_NAME] || held_item_id == salve_name_to_id_map[MANA_SALVE_NAME])
								{
									salves_used.insert(held_item_id);

									if (script_name_to_reference_map.contains(GML_SCRIPT_UPDATE_TOOLBAR_MENU))
										UpdateToolbarMenu(script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][0], script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][1]);
								}
								else if (lift_key_used)
								{
									lift_key_used = false;

									if (held_item_id == item_name_to_id_map[TIDE_CAVERNS_KEY_NAME])
										EnterDungeon(19, script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE][0], script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE][1]);
									else if (held_item_id == item_name_to_id_map[DEEP_EARTH_KEY_NAME])
										EnterDungeon(39, script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE][0], script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE][1]);
									else if (held_item_id == item_name_to_id_map[LAVA_CAVES_KEY_NAME])
										EnterDungeon(59, script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE][0], script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE][1]);
									else if (held_item_id == item_name_to_id_map[RUINS_KEY_NAME])
										EnterDungeon(79, script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE][0], script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE][1]);
								}
							}
						}
					}
				}
			}
		}

		// Restoration & Cleric Armor Bonus
		if (is_restoration_tracked_interval)
		{
			ModifyHealth(global_instance->GetRefMember("__ari")->ToInstance(), self, max(1, CountEquippedClassArmor()[ClassArmor::CLERIC]));
			is_restoration_tracked_interval = false;
		}

		// Second Wind
		if (is_second_wind_tracked_interval)
		{
			ModifyStamina(global_instance->GetRefMember("__ari")->ToInstance(), self, 1);
			is_second_wind_tracked_interval = false;
		}

		// HP Penalty
		if (active_floor_enchantments.contains(FloorEnchantments::HP_PENALTY))
		{
			RValue max_health = GetMaxHealth(global_instance->GetRefMember("__ari")->ToInstance(), self);
			RValue current_health = GetHealth(global_instance->GetRefMember("__ari")->ToInstance(), self);

			int penalty = std::trunc(max_health.ToDouble() * 0.25);
			int adjusted_max_health = max_health.ToInt64() - penalty;
			if (current_health.ToInt64() > adjusted_max_health)
				SetHealth(global_instance->GetRefMember("__ari")->ToInstance(), self, adjusted_max_health);
		}

		// Fey
		if (active_floor_enchantments.contains(FloorEnchantments::FEY))
		{
			if (!fairy_buff_applied)
			{
				std::vector<CInstance*> refs = script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE];
				RegisterStatusEffect(refs[0], refs[1], status_effect_name_to_id_map["fairy"], RValue(), 1, 2147483647.0);
				fairy_buff_applied = true;
			}
			ModifySpellCosts(false);
		}

		// Inner Fire
		if (active_offerings.contains(Offerings::INNER_FIRE) && !fire_breath_cast)
		{
			fire_breath_cast = true;
			CastSpell(global_instance->GetRefMember("__ari")->ToInstance(), self, 0);
		}

		// Reckoning
		if (active_offerings.contains(Offerings::RECKONING) && !reckoning_applied)
		{
			reckoning_applied = true;
			SetHealth(global_instance->GetRefMember("__ari")->ToInstance(), self, 1);
		}

		TrackAriResources(global_instance->GetRefMember("__ari")->ToInstance(), self);
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
				// Track the monster
				if (!StructVariableExists(monster, "__deep_dungeon__current_floor_monsters") && StructVariableExists(monster, "hit_points"))
				{
					current_floor_monsters.push_back(self); // TESTING
					StructVariableSet(monster, "__deep_dungeon__current_floor_monsters", true);
				}

				// Loot drops
				if (!StructVariableExists(monster, "__deep_dungeon__loot_drop") && StructVariableExists(monster, "hit_points"))
				{
					double hit_points = monster.GetMember("hit_points").ToDouble();
					if (std::isfinite(hit_points) && hit_points <= 0 && script_name_to_reference_map.contains(GML_SCRIPT_DROP_ITEM))
					{
						static thread_local std::mt19937 random_generator(std::random_device{}());
						std::uniform_int_distribution<size_t> zero_to_nintey_nine_distribution(0, 99);
						// TODO: Should beast coin drops have RNG?
						bool drop_lift_key = zero_to_nintey_nine_distribution(random_generator) < 2 ? true : false; // TODO: What should the drop rate be for lift keys?

						if (floor_number < 20) // Upper Mines
						{
							DropItem(item_name_to_id_map["beast_coin_tiny"], ari_x, ari_y, script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][0], script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][1]);
						}
						else if (floor_number < 40) // Tide Caverns
						{
							DropItem(item_name_to_id_map["beast_coin_small"], ari_x, ari_y, script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][0], script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][1]);
							if (progression_mode && drop_lift_key) DropItem(item_name_to_id_map[TIDE_CAVERNS_KEY_NAME], ari_x, ari_y, script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][0], script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][1]);
						}
						else if (floor_number < 60) // Deep Earth
						{
							DropItem(item_name_to_id_map["beast_coin_medium"], ari_x, ari_y, script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][0], script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][1]);
							if (progression_mode && drop_lift_key) DropItem(item_name_to_id_map[DEEP_EARTH_KEY_NAME], ari_x, ari_y, script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][0], script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][1]);
						}
						else if (floor_number < 80) // Lava Caves
						{
							DropItem(item_name_to_id_map["beast_coin_large"], ari_x, ari_y, script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][0], script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][1]);
							if (progression_mode && drop_lift_key) DropItem(item_name_to_id_map[LAVA_CAVES_KEY_NAME], ari_x, ari_y, script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][0], script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][1]);
						}
						else if (floor_number < 100) // Ruins
						{
							DropItem(item_name_to_id_map["beast_coin_giant"], ari_x, ari_y, script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][0], script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][1]);
							if (progression_mode && drop_lift_key) DropItem(item_name_to_id_map[RUINS_KEY_NAME], ari_x, ari_y, script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][0], script_name_to_reference_map[GML_SCRIPT_DROP_ITEM][1]);
						}

						StructVariableSet(monster, "__deep_dungeon__loot_drop", true);
					}
				}

				// Gloom
				if (active_floor_enchantments.contains(FloorEnchantments::GLOOM))
				{
					if (!StructVariableExists(monster, "__deep_dungeon__gloom_applied") && StructVariableExists(monster, "hit_points"))
					{
						double hit_points = monster.GetMember("hit_points").ToDouble();
						if (std::isfinite(hit_points))
						{
							*monster.GetRefMember("hit_points") = hit_points * 2;
							StructVariableSet(monster, "__deep_dungeon__gloom_applied", true);
						}
					}
				}

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
							CreateNotification(CONCEALMENT_LOST_NOTIFICATION_KEY, nullptr, nullptr);

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

				// Reckoning
				if (active_offerings.contains(Offerings::RECKONING))
				{
					if (!StructVariableExists(monster, "__deep_dungeon__reckoning_applied") && StructVariableExists(monster, "hit_points"))
					{
						*monster.GetRefMember("hit_points") = 1;
						StructVariableSet(monster, "__deep_dungeon__reckoning_applied", true);
					}
				}
			}
		}
	}

	/*
	std::string name = self->m_Object->m_Name;
	if (name == "obj_monster_clod")
	{
		RValue monster = self->ToRValue();
		RValue monster_id = monster.GetMember("monster_id");
		if (monster_id.ToInt64() == 17)
		{
			RValue wait_to_change_attack_pattern_exists = g_ModuleInterface->CallBuiltin("struct_exists", { monster, "__deep_dungeon__wait_to_change_attack_pattern" });
			if (!wait_to_change_attack_pattern_exists.ToBoolean())
				StructVariableSet(monster, "__deep_dungeon__wait_to_change_attack_pattern", false);
			RValue wait_to_change_attack_pattern = monster.GetMember("__deep_dungeon__wait_to_change_attack_pattern");

			RValue custom_attack_pattern_exists = g_ModuleInterface->CallBuiltin("struct_exists", { monster, "__deep_dungeon__custom_attack_pattern" });
			if (!custom_attack_pattern_exists.ToBoolean())
			{
				StructVariableSet(monster, "__deep_dungeon__custom_attack_pattern", 0);
				if (StructVariableExists(monster, "config"))
				{
					RValue config = *monster.GetRefMember("config");
					*config.GetRefMember("attack_sequence") = 20.0;
					*config.GetRefMember("attack_legion") = 10.0;
					*config.GetRefMember("projectile_speed") = 3.5;
				}
			}

			if (StructVariableExists(monster, "aggro"))
			{
				RValue aggro = monster.GetMember("aggro"); // BOOL
				int temp = 5;
			}

			if (StructVariableExists(monster, "fsm"))
			{
				RValue state_id = monster.GetMember("fsm").GetMember("state").GetMember("state_id"); // TODO: Don't hard-code these. Get them from the monster's __rockclod_state__ global.
				if (state_id.ToInt64() == 4) // Attack
					*monster.GetRefMember("__deep_dungeon__wait_to_change_attack_pattern") = false;

				if (state_id.ToInt64() == 5 && !wait_to_change_attack_pattern.ToBoolean()) // Tired
				{
					*monster.GetRefMember("__deep_dungeon__wait_to_change_attack_pattern") = true;

					int custom_attack_pattern = monster.GetMember("__deep_dungeon__custom_attack_pattern").ToInt64() + 1;
					if (custom_attack_pattern > 2)
						custom_attack_pattern = 0;

					if (custom_attack_pattern == 0)
					{
						if (StructVariableExists(monster, "config"))
						{
							// Shoots a wall of 10 pellets repeatedly 5 times
							RValue config = *monster.GetRefMember("config");
							*config.GetRefMember("attack_sequence") = 5.0;
							*config.GetRefMember("attack_legion") = 10.0;
							*config.GetRefMember("attack_sequence_turn") = -1.0;
							*config.GetRefMember("attack_sequence_image_speed") = -1.0;
							*config.GetRefMember("projectile_speed") = 3.0;
							*config.GetRefMember("split_distance") = -1.0;
							*config.GetRefMember("split_depth") = -1.0;
							*config.GetRefMember("split_angle") = -1.0;
						}
					}
					if (custom_attack_pattern == 1)
					{
						if (StructVariableExists(monster, "config"))
						{
							// Rotates 18-degrees at a time while shooting 5 pellets in a small cone
							RValue config = *monster.GetRefMember("config");
							*config.GetRefMember("attack_sequence") = 20.0;
							*config.GetRefMember("attack_legion") = 5.0;
							*config.GetRefMember("attack_sequence_turn") = 18.0;
							*config.GetRefMember("attack_sequence_image_speed") = 2.0;
							*config.GetRefMember("projectile_speed") = 3.0;
							*config.GetRefMember("split_distance") = -1.0;
							*config.GetRefMember("split_depth") = -1.0;
							*config.GetRefMember("split_angle") = -1.0;
						}
					}
					if (custom_attack_pattern == 2)
					{
						if (StructVariableExists(monster, "config"))
						{
							// Shoots a single pellet that then splits into many that repeatedly split
							RValue config = *monster.GetRefMember("config");
							*config.GetRefMember("attack_sequence") = 5.0;
							*config.GetRefMember("attack_legion") = 1.0;
							*config.GetRefMember("attack_sequence_turn") = -1.0;
							*config.GetRefMember("attack_sequence_image_speed") = -1.0;
							*config.GetRefMember("projectile_speed") = 3.0;
							*config.GetRefMember("split_distance") = 20.0;
							*config.GetRefMember("split_depth") = 5.0;
							*config.GetRefMember("split_angle") = 20.0;
						}
					}

					*monster.GetRefMember("__deep_dungeon__custom_attack_pattern") = custom_attack_pattern;
				}
			}
		}
	}
	*/
}

RValue& GmlScriptCancelStatusEffectCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	// Protection
	if (active_sigils.contains(Sigils::PROTECTION) && Arguments[0]->ToInt64() == status_effect_name_to_id_map["guardians_shield"])
	{
		active_sigils.erase(Sigils::PROTECTION);

		if (script_name_to_reference_map.contains(GML_SCRIPT_UPDATE_TOOLBAR_MENU))
			UpdateToolbarMenu(script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][0], script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][1]);
	}

	// Redemption
	if (active_sigils.contains(Sigils::REDEMPTION) && Arguments[0]->ToInt64() == status_effect_name_to_id_map["fairy"])
	{
		active_sigils.erase(Sigils::REDEMPTION);

		if (script_name_to_reference_map.contains(GML_SCRIPT_UPDATE_TOOLBAR_MENU))
			UpdateToolbarMenu(script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][0], script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][1]);
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_CANCEL_STATUS_EFFECT));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptRegisterStatusEffectCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (AriCurrentGmRoomIsDungeonFloor())
	{
		if (Arguments[0]->ToInt64() == status_effect_name_to_id_map["restorative"])
		{
			int finish = Arguments[3]->ToInt64();
			*Arguments[3] = finish - 3600; // Reduce the duration of Restoration
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_REGISTER_STATUS_EFFECT));
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
	// Exhaustion
	if (active_floor_enchantments.contains(FloorEnchantments::EXHAUSTION))
	{
		if (Arguments[0]->ToDouble() < 0)
		{
			double modified_stamina_cost = Arguments[0]->ToDouble() * 2;
			*Arguments[0] = modified_stamina_cost;
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_MODIFY_STAMINA));
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
	static thread_local std::mt19937 random_generator(std::random_device{}());
	std::uniform_int_distribution<size_t> zero_to_nintey_nine_distribution(0, 99);

	// Sigil of Silence
	if (active_sigils.contains(Sigils::SILENCE))
	{
		int chance_to_activate = zero_to_nintey_nine_distribution(random_generator);
		int activation_threshold = 100;
		for (int i = 0; i < sigil_of_silence_count; i++)
		{
			activation_threshold /= 3;
		}

		bool activate = false;
		if (activation_threshold == 100)
			activate = true;
		else if (chance_to_activate < activation_threshold)
			activate = true;

		sigil_of_silence_count++;
		if(activate)
			return Result;
	}
		
	// Sigil of Alteration
	if (active_sigils.contains(Sigils::ALTERATION))
	{
		int chance_to_activate = zero_to_nintey_nine_distribution(random_generator);

		int activation_threshold = 100;
		for (int i = 0; i < sigil_of_alteration_count; i++)
		{
			activation_threshold /= 2;
		}

		bool activate = false;
		if (activation_threshold == 100)
			activate = true;
		else if(chance_to_activate < activation_threshold)
			activate = true;

		if (activate)
		{
			int random = zero_to_nintey_nine_distribution(random_generator);

			if (random < 40) // 40% chance for flame spirit
				*Arguments[2] = monster_name_to_id_map["spirit"];
			else // 60% chance for mimic
				*Arguments[2] = monster_name_to_id_map["mimic"];
		}

		sigil_of_alteration_count++;
	}

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

RValue& GmlScriptCanCastSpellCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_CAN_CAST_SPELL));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	// Amnesia
	if (active_floor_enchantments.contains(FloorEnchantments::AMNESIA))
	{
		Result = 0.0;
	}

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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_MOVE_SPEED));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	// Gravity
	if (active_floor_enchantments.contains(FloorEnchantments::GRAVITY))
		Result = 1.25; // TODO: Trying 1.25 instead of 1.0 for balancing

	// Haste
	if (active_floor_enchantments.contains(FloorEnchantments::HASTE))
		Result = 3.0;
	
	return Result; // 2.0 is default run speed
}

RValue& GmlScriptDamageCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	static thread_local std::mt19937 random_generator(std::random_device{}());
	std::uniform_int_distribution<size_t> zero_to_one_distribution(0, 1);

	// Distortion
	if (active_floor_enchantments.contains(FloorEnchantments::DISTORTION))
	{
		RValue target = Arguments[0]->GetMember("target");
		if (target.ToInt64() != 1) // Everything not Ari
		{
			bool miss = zero_to_one_distribution(random_generator);
			if (miss)
			{
				*Arguments[0]->GetRefMember("damage") = 0.0;
				*Arguments[0]->GetRefMember("critical") = false;
				*Arguments[0]->GetRefMember("knockback") = false;
				//*Arguments[0]->GetRefMember("frozen") = true; // Frozen debuff
				//*Arguments[0]->GetRefMember("venomous") = true; // Poison debuff
				//*Arguments[0]->GetRefMember("electrocute_kind") = 0; // Paralysis debuff (doesn't seem to affect monsters)
				//*Arguments[0]->GetRefMember("can_pick_grid_objects") = true; // Pick node objects
				//*Arguments[0]->GetRefMember("can_chop_grid_objects") = true; // Chop node objects
			}
		}
	}

	// Damage Down
	if (active_floor_enchantments.contains(FloorEnchantments::DAMAGE_DOWN))
	{
		RValue target = Arguments[0]->GetMember("target");
		if (target.ToInt64() != 1) // Everything not Ari
		{
			if (!StructVariableExists(*Arguments[0], "__deep_dungeon__damage_down_applied"))
			{
				double damage = Arguments[0]->GetMember("damage").ToDouble();
				int penalty = std::trunc(damage * 0.30); // 30% reduced damage
				*Arguments[0]->GetRefMember("damage") = damage - penalty;
				StructVariableSet(*Arguments[0], "__deep_dungeon__damage_down_applied", true);
			}
		}
	}

	// Gloom
	if (active_floor_enchantments.contains(FloorEnchantments::GLOOM))
	{
		if (!StructVariableExists(*Arguments[0], "__deep_dungeon__gloom_applied")) // Prevents monster attacks that "persist" from repeatedly getting Gloom applied
		{
			RValue target = Arguments[0]->GetMember("target");
			if (target.ToInt64() == 1) // Ari
			{
				double damage = std::trunc(Arguments[0]->GetMember("damage").ToDouble() * 1.5); // 50% increased damage
				*Arguments[0]->GetRefMember("damage") = damage;
			}
			else
			{
				double damage = Arguments[0]->GetMember("damage").ToDouble();
				int penalty = std::trunc(damage * 0.50); // 50% reduced damage
				*Arguments[0]->GetRefMember("damage") = damage - penalty;
			}

			StructVariableSet(*Arguments[0], "__deep_dungeon__gloom_applied", true);
		}
	}

	// Sigil of Fortification
	if (active_sigils.contains(Sigils::FORTIFICATION))
	{
		if (!StructVariableExists(*Arguments[0], "__deep_dungeon__fortification_applied")) // Prevents monster attacks that "persist" from repeatedly getting Gloom applied
		{
			RValue target = Arguments[0]->GetMember("target");
			if (target.ToInt64() == 1) // Ari
			{
				double damage = Arguments[0]->GetMember("damage").ToDouble();
				int penalty = std::trunc(damage * 0.40); // 40% reduced damage
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
				double damage = std::trunc(Arguments[0]->GetMember("damage").ToDouble() * 1.3); // 30% increased damage
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

RValue& GmlScriptInteractCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_INTERACT));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);
	return Result;
}

RValue& GmlScriptStatusEffectManagerCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (!script_name_to_reference_map.contains(GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE))
		script_name_to_reference_map[GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE] = { Self, Other };
	
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

RValue& GmlScriptTakePressCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_TAKE_PRESS));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	// Chance for an Offering event when using a ladder on a dungeon floor.
	if (game_is_active && obj_dungeon_ladder_down_focused && Arguments[0]->ToInt64() == 6 && Result.ToBoolean() && !offering_chance_occurred)
	{
		static thread_local std::mt19937 random_generator(std::random_device{}());
		std::uniform_int_distribution<size_t> zero_to_nine_distribution(0, 9);
		
		int roll = zero_to_nine_distribution(random_generator); // 10% chance for an Offering event
		if (active_sigils.contains(Sigils::TEMPTATION) || roll == 0)
		{ 
			std::uniform_int_distribution<size_t> random_ari_resource_distribution(0, magic_enum::enum_count<AriResources>() - 1);
			AriResources resource = magic_enum::enum_value<AriResources>(random_ari_resource_distribution(random_generator));
			if (resource == AriResources::HEALTH && ari_resource_to_value_map[AriResources::HEALTH] > 25)
				PlayConversation("Conversations/Mods/Deep Dungeon/offering/health", Self, Other);
			if (resource == AriResources::STAMINA && ari_resource_to_value_map[AriResources::STAMINA] > 20)
				PlayConversation("Conversations/Mods/Deep Dungeon/offering/stamina", Self, Other);
			if (resource == AriResources::MANA && ari_resource_to_value_map[AriResources::MANA] > 1)
				PlayConversation("Conversations/Mods/Deep Dungeon/offering/mana", Self, Other);

			Result = false;
		}

		offering_chance_occurred = true;
	}
	// Disable the elevator in progression mode.
	else if (game_is_active && progression_mode && (ari_current_gm_room.contains("rm_mines") || ari_current_gm_room.contains("seal")) && obj_dungeon_elevator_focused && Arguments[0]->ToInt64() == 6 && Result.ToBoolean())
	{
		PlayConversation(PROGRESSION_MODE_ELEVATOR_LOCKED_CONVERSATION_KEY, Self, Other);
		Result = false;
	}

	return Result;
}

RValue& GmlScriptCheckValueCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (game_is_active && !GameIsPaused() && active_traps.contains(Traps::CONFUSING) && Arguments[0]->ToInt64() <= 4 && Arguments[0]->ToInt64() >= 1)
	{
		// Confusion Trap
		int new_value = 0;
		if (Arguments[0]->ToInt64() == 1)
			new_value = 2;
		if (Arguments[0]->ToInt64() == 2)
			new_value = 1;
		if (Arguments[0]->ToInt64() == 3)
			new_value = 4;
		if (Arguments[0]->ToInt64() == 4)
			new_value = 3;
		*Arguments[0] = new_value;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_check_value@Input@Input"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptAttemptInteractCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (game_is_active && Self->m_Object != nullptr)
	{
		std::string self_name = Self->m_Object->m_Name;

		if (self_name == "obj_dungeon_ladder_down")
		{
			if (floor_number != 0)
				obj_dungeon_ladder_down_focused = true;
			else
				obj_dungeon_ladder_down_focused = false;

			obj_dungeon_elevator_focused = false;
		}
		else if (self_name == "obj_dungeon_elevator")
		{
			if (progression_mode)
				obj_dungeon_elevator_focused = true;
			else
				obj_dungeon_elevator_focused = false;

			obj_dungeon_ladder_down_focused = false;
		}
		else
		{
			obj_dungeon_ladder_down_focused = false;
			obj_dungeon_elevator_focused = false;
		}
	}
	else
	{
		obj_dungeon_ladder_down_focused = false;
		obj_dungeon_elevator_focused = false;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_ATTEMPT_INTERACT));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

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
	if (game_is_active)
	{
		bool is_offering = false;
		std::string conversation_name = Arguments[0]->ToString();

		if (conversation_name == "Conversations/Mods/Deep Dungeon/offering/2/health")
		{
			is_offering = true;
			ari_resource_to_penalty_map[AriResources::HEALTH] = true;
		}
			
		if (conversation_name == "Conversations/Mods/Deep Dungeon/offering/2/stamina")
		{
			is_offering = true;
			ari_resource_to_penalty_map[AriResources::STAMINA] = true;
		}
			
		if (conversation_name == "Conversations/Mods/Deep Dungeon/offering/2/mana")
		{
			is_offering = true;
			ari_resource_to_penalty_map[AriResources::MANA] = true;
		}
			
		if (is_offering)
		{
			// Pick a random offering effect
			static thread_local std::mt19937 random_generator(std::random_device{}());
			std::uniform_int_distribution<size_t> random_offering_distribution(0, magic_enum::enum_count<Offerings>() - 1);
			Offerings offering = magic_enum::enum_value<Offerings>(random_offering_distribution(random_generator));
			queued_offerings.insert(offering);
		}
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

RValue& GmlScriptUseItemCallback(
	IN CInstance* Self, // Changes depending on the invocation context. For world interactables like a fountain, Self->m_Object->m_Name == "obj_world_fountain". For Ari using an item, Self->m_Object == NULL.
	IN CInstance* Other, // Changes depending on the invocation context. For world interactables like a fountain, Other->m_Object->m_Name == "Game". For Ari using an item, Other->m_Object->m_Name == "obj_ari".
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	// Lift Keys
	if (Self->m_Object == NULL && strstr(Other->m_Object->m_Name, "obj_ari") && lift_key_items.contains(held_item_id))
	{
		if (ari_current_gm_room != "rm_mines_entry")
		{
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - You are only allowed to use a lift key at the mines entrance!", MOD_NAME, VERSION);
			CreateNotification(LIFT_KEY_RESTRICTED_NOTIFICATION_KEY, Self, Other);
			return Result;
		}
	}

	// Inhibiting Trap
	if (active_traps.contains(Traps::INHIBITING))
	{
		if (Self->m_Object == NULL && strstr(Other->m_Object->m_Name, "obj_ari"))
		{
			if (held_item_id == mistpool_gear_to_item_id_map[MISTPOOL_SWORD_NAME])
			{
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - You are unable to use the Mistpool Sword due to the Inhibiting Trap's effect!", MOD_NAME, VERSION);
				CreateNotification(INHIBITED_PENALTY_NOTIFICATION_KEY, Self, Other);
				return Result;
			}
		}
	}

	// Item Penalty
	if (active_floor_enchantments.contains(FloorEnchantments::ITEM_PENALTY))
	{
		if (Self->m_Object == NULL && strstr(Other->m_Object->m_Name, "obj_ari"))
		{
			if (held_item_id != sigil_to_item_id_map[Sigils::SERENITY] && restricted_items.contains(held_item_id))
			{
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - You are unable to use that item due to the Item Penalty floor enchantment!!", MOD_NAME, VERSION);
				CreateNotification(ITEM_PENALTY_NOTIFICATION_KEY, Self, Other);
				return Result;
			}
		}
	}
	
	if(AriCurrentGmRoomIsDungeonFloor())
	{
		if (Self->m_Object == NULL && strstr(Other->m_Object->m_Name, "obj_ari"))
		{
			// Sigil Already Used
			if (item_id_to_sigil_map.contains(held_item_id) && active_sigils.contains(item_id_to_sigil_map[held_item_id]))
			{
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - That sigil is already active!", MOD_NAME, VERSION);
				CreateNotification(SIGIL_LIMIT_NOTIFICATION_KEY, Self, Other);
				return Result;
			}

			// Salve Limit
			if (salves_used.contains(held_item_id))
			{
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - You have already used that type of salve on the current floor!", MOD_NAME, VERSION);
				CreateNotification(SALVE_LIMIT_NOTIFICATION_KEY, Self, Other);
				return Result;
			}

			// Dungeon's Curse
			if (!deep_dungeon_items.contains(held_item_id) && restricted_items.contains(held_item_id))
			{
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - That item is prohibited in the Deep Dungeon!", MOD_NAME, VERSION);
				CreateNotification(ITEM_PROHIBITED_NOTIFICATION_KEY, Self, Other);
				return Result;
			}
		}
	}
	else
	{
		if (Self->m_Object == NULL && strstr(Other->m_Object->m_Name, "obj_ari"))
		{
			// Deep Dungeon Exclusive Items
			if (deep_dungeon_items.contains(held_item_id))
			{
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - You may only use Deep Dungeon specific items inside the dungeon!", MOD_NAME, VERSION);
				CreateNotification(ITEM_RESTRICTED_NOTIFICATION_KEY, Self, Other);
				return Result;
			}
		}
	}

	// Sigil Item
	sigil_item_used = false;
	if(item_id_to_sigil_map.contains(held_item_id))
		sigil_item_used = true;

	// Lift Key Item
	lift_key_used = false;
	if(lift_key_items.contains(held_item_id))
		lift_key_used = true;

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

RValue& GmlScriptDropItemCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (!script_name_to_reference_map.contains(GML_SCRIPT_DROP_ITEM))
		script_name_to_reference_map[GML_SCRIPT_DROP_ITEM] = {Self, Other};

	if (ari_current_gm_room.contains("rm_mines"))
	{
		bool chance_to_spawn_glowstone = false;

		if (Arguments[0]->m_Kind == VALUE_ARRAY)
		{
			size_t array_length;
			g_ModuleInterface->GetArraySize(*Arguments[0], array_length);

			for (size_t i = 0; i < array_length; i++)
			{
				RValue* array_element;
				g_ModuleInterface->GetArrayEntry(*Arguments[0], i, array_element);

				if (StructVariableExists(*array_element, "item_id"))
				{
					int item_id = array_element->GetMember("item_id").ToInt64();
					if (item_id == item_name_to_id_map["ore_stone"])
						chance_to_spawn_glowstone = true;
				}
			}
		}
		else if (Arguments[0]->m_Kind == VALUE_INT64 && Arguments[0]->ToInt64() == item_name_to_id_map["ore_stone"])
			chance_to_spawn_glowstone = true;

		// TODO: Test if there should be some RNG for dropping glowstone.
		if (chance_to_spawn_glowstone)
		{
			if (floor_number < 20) // Upper Mines
				DropItem(item_name_to_id_map["glow_stone_tiny"], ari_x, ari_y, Self, Other);
			else if (floor_number < 40) // Tide Caverns
				DropItem(item_name_to_id_map["glow_stone_small"], ari_x, ari_y, Self, Other);
			else if (floor_number < 60) // Deep Earth
				DropItem(item_name_to_id_map["glow_stone_medium"], ari_x, ari_y, Self, Other);
			else if (floor_number < 80) // Lava Caves
				DropItem(item_name_to_id_map["glow_stone_large"], ari_x, ari_y, Self, Other);
			else if (floor_number < 100) // Ruins
				DropItem(item_name_to_id_map["glow_stone_giant"], ari_x, ari_y, Self, Other);
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_DROP_ITEM));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptGetMinutesCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_MINUTES));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active)
	{
		RValue time = global_instance->GetMember("__clock").GetMember("time");
		current_time_in_seconds = time.ToInt64();

		ApplyFloorTraps(Self, Other);

		// Restoration
		if (active_floor_enchantments.contains(FloorEnchantments::RESTORATION) || (AriCurrentGmRoomIsDungeonFloor() && CountEquippedClassArmor()[ClassArmor::CLERIC] > 0))
		{
			if (!is_restoration_tracked_interval && (current_time_in_seconds - time_of_last_restoration_tick) >= TWO_MINUTES_IN_SECONDS)
			{
				is_restoration_tracked_interval = true;
				time_of_last_restoration_tick = current_time_in_seconds;
			}
		}

		// Second Wind
		if (active_floor_enchantments.contains(FloorEnchantments::SECOND_WIND))
		{
			if (!is_second_wind_tracked_interval && (current_time_in_seconds - time_of_last_second_wind_tick) >= TWO_MINUTES_IN_SECONDS)
			{
				is_second_wind_tracked_interval = true;
				time_of_last_second_wind_tick = current_time_in_seconds;
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
	if (localize_mod_text)
	{
		localize_mod_text = false;

		floor_enchantments_to_localized_string_map[FloorEnchantments::GLOOM] = LocalizeString(Self, Other, GLOOM_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::HP_PENALTY] = LocalizeString(Self, Other, HP_PENALTY_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::EXHAUSTION] = LocalizeString(Self, Other, EXHAUSTION_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::AMNESIA] = LocalizeString(Self, Other, AMNESIA_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::ITEM_PENALTY] = LocalizeString(Self, Other, ITEM_PENALTY_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::DISTORTION] = LocalizeString(Self, Other, DISTORTION_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::DAMAGE_DOWN] = LocalizeString(Self, Other, DAMAGE_DOWN_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::GRAVITY] = LocalizeString(Self, Other, GRAVITY_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::FEY] = LocalizeString(Self, Other, FEY_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::RESTORATION] = LocalizeString(Self, Other, RESTORATION_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::SECOND_WIND] = LocalizeString(Self, Other, SECOND_WIND_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();
		floor_enchantments_to_localized_string_map[FloorEnchantments::HASTE] = LocalizeString(Self, Other, HASTE_FLOOR_ENCHANTMENT_LOCALIZED_TEXT_KEY).ToString();

		offerings_to_localized_string_map[Offerings::DREAD] = LocalizeString(Self, Other, DREAD_OFFERING_LOCALIZED_TEXT_KEY).ToString();
		offerings_to_localized_string_map[Offerings::INNER_FIRE] = LocalizeString(Self, Other, INNER_FIRE_OFFERING_LOCALIZED_TEXT_KEY).ToString();
		offerings_to_localized_string_map[Offerings::LEECH] = LocalizeString(Self, Other, LEECH_OFFERING_LOCALIZED_TEXT_KEY).ToString();
		offerings_to_localized_string_map[Offerings::PERIL] = LocalizeString(Self, Other, PERIL_OFFERING_LOCALIZED_TEXT_KEY).ToString();
		offerings_to_localized_string_map[Offerings::RECKONING] = LocalizeString(Self, Other, RECKONING_OFFERING_LOCALIZED_TEXT_KEY).ToString();
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_LOCALIZER));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active)
	{
		if (Arguments[0]->ToString() == FLOOR_ENCHANTMENT_PLACEHOLDER_TEXT_KEY)
		{
			std::string custom_text = "";
			for (auto it = active_floor_enchantments.begin(); it != active_floor_enchantments.end();)
			{
				custom_text += floor_enchantments_to_localized_string_map[*it];

				if (++it != active_floor_enchantments.end())
					custom_text += "\n";
			}

			Result = RValue(custom_text);
			return Result;
		}
		if (Arguments[0]->ToString() == OFFERINGS_PLACEHOLDER_TEXT_KEY)
		{
			std::string custom_text = "";
			for (auto it = queued_offerings.begin(); it != queued_offerings.end();)
			{
				custom_text += offerings_to_localized_string_map[*it];

				if (++it != queued_offerings.end())
					custom_text += "\n";
			}

			Result = RValue(custom_text);
			return Result;
		}
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
	if (!game_is_active)
	{
		game_is_active = true;
		//MarkDungeonTutorialUnseen(); // TODO: Only do this once per save file.
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

RValue& GmlScriptTryLocationIdToStringCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_TRY_LOCATION_ID_TO_STRING));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active && Result.m_Kind == VALUE_STRING)
		ari_current_location = Result.ToString();

	return Result;
}

RValue& GmlScriptOnDungeonRoomStartCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	// TODO: Run logic to actually undo all active floor enchantments.
	// TODO: Remove all buffs.
	ResetCustomDrawFields();
	salves_used.clear();
	active_sigils.clear();
	active_floor_enchantments.clear();
	active_offerings = queued_offerings;
	queued_offerings.clear();

	// TESTING - reset trap vars
	active_traps.clear();
	active_traps_to_value_map.clear();
	floor_trap_positions.clear();
	current_floor_monsters.clear();

	show_dashes = active_offerings.contains(Offerings::DREAD);
	show_danger_banner = active_offerings.contains(Offerings::DREAD);
	
	DisableAllPerks();
	ModifySpellCosts(true);
	ScaleMistpoolArmor(true);
	ScaleMistpoolWeapon(true);
	CancelAllStatusEffects();
	SetInvulnerabilityHits(0);
	reckoning_applied = false;
	fairy_buff_applied = false;
	offering_chance_occurred = false;
	sigil_of_silence_count = 0;
	sigil_of_alteration_count = 0;

	// Toggle reward on seal rooms in progression mode
	if (progression_mode && ari_current_gm_room.contains("seal"))
		biome_reward = true;

	// TODO: Confirm what "milestone" rooms are and determine if they should be removed from this conditional.
	if (ari_current_gm_room != "rm_mines_entry" && ari_current_gm_room.find("seal") == std::string::npos && ari_current_gm_room.find("ritual") == std::string::npos && ari_current_gm_room.find("treasure") == std::string::npos && ari_current_gm_room.find("milestone") == std::string::npos)
	{
		GenerateFloorTraps(); // TODO: Testing this

		if (ari_current_gm_room == "rm_mines_upper_floor1")
			active_floor_enchantments = RandomFloorEnchantments(true, DungeonBiomes::UPPER);
		else if (ari_current_gm_room.find("rm_mines_upper") != std::string::npos)
			active_floor_enchantments = RandomFloorEnchantments(false, DungeonBiomes::UPPER);
		else if (ari_current_gm_room.find("rm_mines_tide") != std::string::npos)
			active_floor_enchantments = RandomFloorEnchantments(false, DungeonBiomes::TIDE_CAVERNS);
		else if (ari_current_gm_room.find("rm_mines_deep") != std::string::npos)
			active_floor_enchantments = RandomFloorEnchantments(false, DungeonBiomes::DEEP_EARTH);
		else if (ari_current_gm_room.find("rm_mines_lava") != std::string::npos)
			active_floor_enchantments = RandomFloorEnchantments(false, DungeonBiomes::LAVA_CAVES);
		else if (ari_current_gm_room.find("rm_mines_ruins") != std::string::npos)
			active_floor_enchantments = RandomFloorEnchantments(false, DungeonBiomes::RUINS);

		if (!active_floor_enchantments.empty() && active_offerings.contains(Offerings::DREAD))
			PlayConversation(FLOOR_ENCHANTMENT_AND_DREAD_BEAST_WARNING_CONVERSATION_KEY, Self, Other);
		else if (!active_floor_enchantments.empty())
			PlayConversation(FLOOR_ENCHANTMENT_CONVERSATION_KEY, Self, Other);
		else if(active_offerings.contains(Offerings::DREAD))
			PlayConversation(DREAD_BEAST_WARNING_CONVERSATION_KEY, Self, Other);

		for (FloorEnchantments floor_enchantment : active_floor_enchantments)
		{
			if (floor_enchantment == FloorEnchantments::RESTORATION)
				time_of_last_restoration_tick = current_time_in_seconds;
			if (floor_enchantment == FloorEnchantments::SECOND_WIND)
				time_of_last_second_wind_tick = current_time_in_seconds;
		}

		if (CountEquippedClassArmor()[ClassArmor::CLERIC] > 0)
			time_of_last_restoration_tick = current_time_in_seconds;

		if (script_name_to_reference_map.contains(GML_SCRIPT_UPDATE_TOOLBAR_MENU))
			UpdateToolbarMenu(script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][0], script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][1]);
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_ON_DUNGEON_ROOM_START));
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

	RValue gm_room = Result.GetMember("gm_room"); //StructVariableGet(Result, "gm_room"); 
	RValue room_name = g_ModuleInterface->CallBuiltin("room_get_name", { gm_room });
	ari_current_gm_room = room_name.ToString();

	if (ari_current_gm_room.contains("rm_mines") && ari_current_gm_room != "rm_mines_entry")
		SetFloorNumber();
	else
		floor_number = 0;

	ModifyMistpoolWeaponSprites();

	if (script_name_to_reference_map.contains(GML_SCRIPT_UPDATE_TOOLBAR_MENU))
		UpdateToolbarMenu(script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][0], script_name_to_reference_map[GML_SCRIPT_UPDATE_TOOLBAR_MENU][1]);

	if (ari_current_location == "dungeon" && (!ari_current_gm_room.contains("rm_mines") || ari_current_gm_room == "rm_mines_entry")) // TODO: Don't use ari_current_location
	{
		// TODO: Run logic to actually undo all active floor enchantments.
		// TOOD: Remove all buffs.
		ResetCustomDrawFields();
		salves_used.clear();
		active_sigils.clear();
		active_floor_enchantments.clear();
		active_offerings.clear(); // Different than OnDungeonRoomStart
		queued_offerings.clear();
		active_traps.clear();
		active_traps_to_value_map.clear();
		floor_trap_positions.clear();
		current_floor_monsters.clear();
		ModifySpellCosts(true);
		ScaleMistpoolArmor(true);
		ScaleMistpoolWeapon(true);
		CancelAllStatusEffects();
		SetInvulnerabilityHits(0);
		fire_breath_cast = false; // Different than OnDungeonRoomStart
		reckoning_applied = false;
		fairy_buff_applied = false;
		offering_chance_occurred = false;
		sigil_of_silence_count = 0;
		sigil_of_alteration_count = 0;
	}
	else
		active_offerings.clear(); // TESTING -- Confirm this clears stuff like Reckoning before new monsters spawn and get modified

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
		localize_mod_text = true;
		g_ModuleInterface->GetGlobalInstance(&global_instance);
		window_width = GetWindowWidth();
		window_height = GetWindowHeight();

		LoadPerks();
		LoadSpells();
		LoadStatusEffects();
		LoadInfusions();
		LoadObjectIds();
		LoadItems();
		LoadMonsters();
		LoadDungeonBiomeCandidateMonsters();
		LoadPlayerStates();
		LoadTutorials();
		// TODO: Load other stuff
	}
	else
		ResetStaticFields(true);

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

RValue& GmlScriptGetEquipmentBonusFromCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	// Argument[0]: INT64 == 3 // ID of the infusion, see __infusion__ global, 3 == Hasty
	// Argument[1]: String == "amount" // Unused

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_EQUIPMENT_BONUS_FROM));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (AriCurrentGmRoomIsDungeonFloor())
	{
		int infusion_id = Arguments[0]->ToInt64();

		// Leech
		if (active_offerings.contains(Offerings::LEECH) && infusion_id == infusion_name_to_id_map["leeching"])
			Result = 1;

		// Dungeon's Curse: Prevent infusions on armor and tools from applying.
		if (infusion_id == infusion_name_to_id_map["fortified"] || infusion_id == infusion_name_to_id_map["hasty"] || infusion_id == infusion_name_to_id_map["lightweight"] || infusion_id == infusion_name_to_id_map["tireless"])
			Result = 0;

		// Class armor bonuses
		std::map<int, int> class_armor_infusions = GetClassArmorInfusions();
		if (class_armor_infusions.contains(infusion_id))
		{
			if (infusion_id == infusion_name_to_id_map["fortified"])
				Result = class_armor_infusions[infusion_id] * 4; // TODO: Confirm the game returns 4 * <foritifed_count>
			if (infusion_id == infusion_name_to_id_map["hasty"])
				Result = class_armor_infusions[infusion_id] * 0.04; // TODO: Figure out what value the game uses here
			if (infusion_id == infusion_name_to_id_map["tireless"])
				Result = class_armor_infusions[infusion_id] * 4; // TODO: Confirm the game returns 4 * <tireless_count>
		}
	}

	// Result: REAL == 0.20 // The value of the infusion bonus from all gear (0.20 for 5 Hasty armor pieces)
	return Result;
}

RValue& GmlScriptHudShouldShowCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_HUD_SHOULD_SHOW));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (active_traps.contains(Traps::DISORIENTING))
		Result = false;

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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_ON_DRAW_GUI));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active && !GameIsPaused())
	{
		// Gloom
		auto gloom = std::find(active_floor_enchantments.begin(), active_floor_enchantments.end(), FloorEnchantments::GLOOM);
		if (gloom != active_floor_enchantments.end())
		{
			// Draw semi-transparent overlay
			g_ModuleInterface->CallBuiltin(
				"draw_set_alpha",
				{ 0.45 }
			);

			g_ModuleInterface->CallBuiltin(
				"draw_set_color", {
					8388736 // c_purple
				}
			);

			g_ModuleInterface->CallBuiltin(
				"draw_rectangle",
				{ 0, 0, window_width, window_height, false }
			);
		}

		// Danger Floor Border
		if (show_dashes)
		{
			DrawDashedBorder(
				20.0f,    // dash length in pixels
				4.0f,     // dash thickness
				80.0f,    // speed pixels per second
				window_width,
				window_height,
				GetCurrentSystemTime()
			);
		}

		// Danger Floor Banner
		if (show_danger_banner)
			FadeInImage(2, 2); // 3, 2
	}

	return Result;
}

RValue& GmlScriptDisplayResizeCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_DISPLAY_RESIZE));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	window_width = GetWindowWidth();
	window_height = GetWindowHeight();

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
			if(deep_dungeon_items.contains(item_id))
				Result = GetDynamicItemSprite(item_id);
		}
	}

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

RValue& GmlScriptCreateItemPrototypesCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_CREATE_ITEM_PROTOTYPES));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	size_t array_length;
	g_ModuleInterface->GetArraySize(Result, array_length);

	// Load all items.
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(Result, i, array_element);

		item_id_to_prototype_map[i] = *array_element;
	}

	return Result;
}

// TODO: Remove this hook
RValue& GmlScriptStopRoomTitleCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_STOP_ROOM_TITLE));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptDeserializeLiveItemCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (!script_name_to_reference_map.contains(GML_SCRIPT_DESERIALIZE_LIVE_ITEM))
		script_name_to_reference_map[GML_SCRIPT_DESERIALIZE_LIVE_ITEM] = { Self, Other };

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_DESERIALIZE_LIVE_ITEM));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptGetTreasureFromDistributionCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{ 
	if (Self != nullptr && StructVariableExists(Self, "object_id"))
	{
		int object_id = Self->GetMember("object_id").ToInt64();
		if (object_id_to_name_map.contains(object_id))
		{
			std::string object_name = object_id_to_name_map[object_id];
			if (DUNGEON_TREASURE_CHEST_NAMES.contains(object_name))
				GenerateTreasureChestLoot(object_name, Self, Other);
		}
	}
	else if (Other != nullptr && StructVariableExists(Other, "object_id"))
	{
		int object_id = Other->GetMember("object_id").ToInt64();
		if (object_id_to_name_map.contains(object_id))
		{
			std::string object_name = object_id_to_name_map[object_id];
			if (DUNGEON_TREASURE_CHEST_NAMES.contains(object_name))
				GenerateTreasureChestLoot(object_name, Self, Other);
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_TREASURE_FROM_DISTRIBUTION));
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

void CreateHookGmlScriptCancelStatusEffect(AurieStatus& status)
{
	CScript* gml_script_cancel_status_effect = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CANCEL_STATUS_EFFECT,
		(PVOID*)&gml_script_cancel_status_effect
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CANCEL_STATUS_EFFECT);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_CANCEL_STATUS_EFFECT,
		gml_script_cancel_status_effect->m_Functions->m_ScriptFunction,
		GmlScriptCancelStatusEffectCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CANCEL_STATUS_EFFECT);
	}
}

void CreateHookGmlScriptRegisterStatusEffect(AurieStatus& status)
{
	CScript* gml_script_register_status_effect = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_REGISTER_STATUS_EFFECT,
		(PVOID*)&gml_script_register_status_effect
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_REGISTER_STATUS_EFFECT);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_REGISTER_STATUS_EFFECT,
		gml_script_register_status_effect->m_Functions->m_ScriptFunction,
		GmlScriptRegisterStatusEffectCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_REGISTER_STATUS_EFFECT);
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

void CreateHookGmlScriptCanCastSpell(AurieStatus& status)
{
	CScript* gml_script_can_cast_spell = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CAN_CAST_SPELL,
		(PVOID*)&gml_script_can_cast_spell
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CAN_CAST_SPELL);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_CAN_CAST_SPELL,
		gml_script_can_cast_spell->m_Functions->m_ScriptFunction,
		GmlScriptCanCastSpellCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CAN_CAST_SPELL);
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

void CreateHookGmlScriptStatusEffectManager(AurieStatus& status)
{
	CScript* gml_script_status_effect_manager = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE,
		(PVOID*)&gml_script_status_effect_manager
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE,
		gml_script_status_effect_manager->m_Functions->m_ScriptFunction,
		GmlScriptStatusEffectManagerCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_STATUS_EFFECT_MANAGER_DESERIALIZE);
	}
}

void CreateHookGmlScriptTakePress(AurieStatus& status)
{
	CScript* gml_script_take_press = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_TAKE_PRESS,
		(PVOID*)&gml_script_take_press
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_TAKE_PRESS);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_TAKE_PRESS,
		gml_script_take_press->m_Functions->m_ScriptFunction,
		GmlScriptTakePressCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_TAKE_PRESS);
	}
}

void CreateHookGmlScriptCheckValue(AurieStatus& status)
{
	CScript* gml_script_check_value = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CHECK_VALUE,
		(PVOID*)&gml_script_check_value
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CHECK_VALUE);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_CHECK_VALUE,
		gml_script_check_value->m_Functions->m_ScriptFunction,
		GmlScriptCheckValueCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CHECK_VALUE);
	}
}

void CreateHookGmlScriptAttemptInteract(AurieStatus& status)
{
	CScript* gml_script_attempt_interact = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_ATTEMPT_INTERACT,
		(PVOID*)&gml_script_attempt_interact
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ATTEMPT_INTERACT);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_ATTEMPT_INTERACT,
		gml_script_attempt_interact->m_Functions->m_ScriptFunction,
		GmlScriptAttemptInteractCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ATTEMPT_INTERACT);
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

void CreateHookGmlScriptDropItem(AurieStatus& status)
{
	CScript* gml_script_drop_item = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_DROP_ITEM,
		(PVOID*)&gml_script_drop_item
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_DROP_ITEM);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_DROP_ITEM,
		gml_script_drop_item->m_Functions->m_ScriptFunction,
		GmlScriptDropItemCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_DROP_ITEM);
	}
}

void CreateHookGmlScriptGetMinutes(AurieStatus& status)
{
	CScript* gml_script_get_minutes = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_MINUTES,
		(PVOID*)&gml_script_get_minutes
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_MINUTES);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_MINUTES,
		gml_script_get_minutes->m_Functions->m_ScriptFunction,
		GmlScriptGetMinutesCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_MINUTES);
	}
}

void CreateHookGmlScriptGetLocalizer(AurieStatus& status)
{
	CScript* gml_script_get_localizer = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_LOCALIZER,
		(PVOID*)&gml_script_get_localizer
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_LOCALIZER);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_LOCALIZER,
		gml_script_get_localizer->m_Functions->m_ScriptFunction,
		GmlScriptGetLocalizerCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_LOCALIZER);
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

void CreateHookGmlScriptOnDungeonRoomStart(AurieStatus& status)
{
	CScript* gml_script_on_dungeon_room_start = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_ON_DUNGEON_ROOM_START,
		(PVOID*)&gml_script_on_dungeon_room_start
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ON_DUNGEON_ROOM_START);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_ON_DUNGEON_ROOM_START,
		gml_script_on_dungeon_room_start->m_Functions->m_ScriptFunction,
		GmlScriptOnDungeonRoomStartCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ON_DUNGEON_ROOM_START);
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

void CreateHookGmlScriptGetEquipmentBonusFrom(AurieStatus& status)
{
	CScript* gml_script_get_equipment_bonus_from = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_EQUIPMENT_BONUS_FROM,
		(PVOID*)&gml_script_get_equipment_bonus_from
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_EQUIPMENT_BONUS_FROM);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_EQUIPMENT_BONUS_FROM,
		gml_script_get_equipment_bonus_from->m_Functions->m_ScriptFunction,
		GmlScriptGetEquipmentBonusFromCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_EQUIPMENT_BONUS_FROM);
	}
}

void CreateHookGmlScriptHudShouldShow(AurieStatus& status)
{
	CScript* gml_script_hud_should_show = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_HUD_SHOULD_SHOW,
		(PVOID*)&gml_script_hud_should_show
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_HUD_SHOULD_SHOW);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_HUD_SHOULD_SHOW,
		gml_script_hud_should_show->m_Functions->m_ScriptFunction,
		GmlScriptHudShouldShowCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_HUD_SHOULD_SHOW);
	}
}

void CreateHookGmlScriptOnDrawGui(AurieStatus& status)
{
	CScript* gml_script_on_draw_gui = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_ON_DRAW_GUI,
		(PVOID*)&gml_script_on_draw_gui
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ON_DRAW_GUI);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_ON_DRAW_GUI,
		gml_script_on_draw_gui->m_Functions->m_ScriptFunction,
		GmlScriptOnDrawGuiCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ON_DRAW_GUI);
	}
}

void CreateHookGmlScriptDisplayResize(AurieStatus& status)
{
	CScript* gml_script_display_resize = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_DISPLAY_RESIZE,
		(PVOID*)&gml_script_display_resize
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_DISPLAY_RESIZE);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_DISPLAY_RESIZE,
		gml_script_display_resize->m_Functions->m_ScriptFunction,
		GmlScriptDisplayResizeCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_DISPLAY_RESIZE);
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

void CreateHookGmlScriptCreateItemPrototypes(AurieStatus& status)
{
	CScript* gml_script_create_item_prototypes = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CREATE_ITEM_PROTOTYPES,
		(PVOID*)&gml_script_create_item_prototypes
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CREATE_ITEM_PROTOTYPES);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_CREATE_ITEM_PROTOTYPES,
		gml_script_create_item_prototypes->m_Functions->m_ScriptFunction,
		GmlScriptCreateItemPrototypesCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CREATE_ITEM_PROTOTYPES);
	}
}

void CreateHookGmlScriptStopRoomTitle(AurieStatus& status)
{
	CScript* gml_script_stop_room_title = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_STOP_ROOM_TITLE,
		(PVOID*)&gml_script_stop_room_title
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_STOP_ROOM_TITLE);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_STOP_ROOM_TITLE,
		gml_script_stop_room_title->m_Functions->m_ScriptFunction,
		GmlScriptStopRoomTitleCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_STOP_ROOM_TITLE);
	}
}

void CreateHookGmlScriptDeserializeLiveItem(AurieStatus& status)
{
	CScript* gml_script_deserialize_live_item = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_DESERIALIZE_LIVE_ITEM,
		(PVOID*)&gml_script_deserialize_live_item
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_DESERIALIZE_LIVE_ITEM);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_DESERIALIZE_LIVE_ITEM,
		gml_script_deserialize_live_item->m_Functions->m_ScriptFunction,
		GmlScriptDeserializeLiveItemCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_DESERIALIZE_LIVE_ITEM);
	}
}

void CreateHookGmlScriptGetTreasureFromDistribution(AurieStatus& status)
{
	CScript* gml_script_get_treasure_from_distribution = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_TREASURE_FROM_DISTRIBUTION,
		(PVOID*)&gml_script_get_treasure_from_distribution
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_TREASURE_FROM_DISTRIBUTION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_TREASURE_FROM_DISTRIBUTION,
		gml_script_get_treasure_from_distribution->m_Functions->m_ScriptFunction,
		GmlScriptGetTreasureFromDistributionCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_TREASURE_FROM_DISTRIBUTION);
	}
}

EXPORTED AurieStatus ModuleInitialize(
	IN AurieModule* Module,
	IN const fs::path& ModulePath
)
{
	UNREFERENCED_PARAMETER(ModulePath);

	AurieStatus status = AURIE_SUCCESS;

	Sigils x = Sigils::RAGE;

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

	CreateHookGmlScriptCancelStatusEffect(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptRegisterStatusEffect(status);
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

	CreateHookGmlScriptSpawnMonster(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptCanCastSpell(status);
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

	CreateHookGmlScriptDamage(status);
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

	CreateHookGmlScriptStatusEffectManager(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptTakePress(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptCheckValue(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptAttemptInteract(status);
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

	CreateHookGmlScriptDropItem(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptGetMinutes(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptGetLocalizer(status);
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

	CreateHookGmlScriptTryLocationIdToString(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptOnDungeonRoomStart(status);
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

	CreateHookGmlScriptGetEquipmentBonusFrom(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptHudShouldShow(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptOnDrawGui(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptDisplayResize(status);
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

	CreateHookGmlScriptUpdateToolbarMenu(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptCreateItemPrototypes(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptStopRoomTitle(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptDeserializeLiveItem(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptGetTreasureFromDistribution(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Plugin started!", MOD_NAME, VERSION);
	return AURIE_SUCCESS;
}
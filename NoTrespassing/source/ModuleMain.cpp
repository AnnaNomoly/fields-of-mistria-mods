#include <fstream>
#include <iostream>
#include <filesystem>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <YYToolkit/YYTK_Shared.hpp> // YYTK v4
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

static const char* const MOD_NAME = "NoTrespassing";
static const char* const VERSION = "1.0.0";
static const char* const GML_SCRIPT_CREATE_NOTIFICATION = "gml_Script_create_notification";
static const char* const GML_SCRIPT_PLAY_CONVERSATION = "gml_Script_play_conversation";
static const char* const GML_SCRIPT_GET_LOCALIZER = "gml_Script_get@Localizer@Localizer";
static const char* const GML_SCRIPT_GET_WEATHER = "gml_Script_get_weather@WeatherManager@Weather";
static const char* const GML_SCRIPT_ATTEMPT_INTERACT = "gml_Script_attempt_interact@gml_Object_par_interactable_Create_0";
static const char* const GML_SCRIPT_TAKE_PRESS = "gml_Script_take_press@Input@Input";
static const char* const GML_SCRIPT_GO_TO_ROOM = "gml_Script_goto_gm_room";
static const char* const GML_SCRIPT_VERTIGO_DRAW_WITH_COLOR = "gml_Script_vertigo_draw_with_color";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const std::string ANNA_PORTRAIT_SPRITE_NAME = "spr_portrait_anna_spring_neutral";
static const std::string ANNA_EASTER_EGG_CONVERSATION = "Conversations/Mods/No Trespassing/anna_easter_egg";
static const std::string ROOM_RESTRICTED_CONVERSATION = "Conversations/Mods/No Trespassing/room_restricted";
static const std::string ROOM_RESTRICTED_PLACEHOLDER_LOCALIZATION_KEY = "Conversations/Mods/No Trespassing/placeholders/room_restricted";
static const std::string BALOR_ROOM_RESTRICTED_LOCALIZATION_KEY = "Conversations/Mods/No Trespassing/balor/room_restricted";
static const std::string JOSEPHINE_HEMLOCK_ROOM_RESTRICTED_LOCALIZATION_KEY = "Conversations/Mods/No Trespassing/josephine_hemlock/room_restricted";
static const std::string LUC_ROOM_RESTRICTED_LOCALIZATION_KEY = "Conversations/Mods/No Trespassing/luc/room_restricted";
static const std::string MAPLE_ROOM_RESTRICTED_LOCALIZATION_KEY = "Conversations/Mods/No Trespassing/maple/room_restricted";
static const std::string REINA_ROOM_RESTRICTED_LOCALIZATION_KEY = "Conversations/Mods/No Trespassing/reina/room_restricted";

static const std::unordered_set<std::string> RELEVANT_GM_ROOMS = { // TODO: Remove this.
	"rm_narrows" // Errol's House (door)
	"rm_beach" // Terithia's House (door)
	"rm_inn", // Balor's Room (open), Reina's Room (door), Maple's Room (door), Luc's Room (door), Josephine & Hemlock's Room (door)
	"rm_bathhouse", // Juniper's Room (open)
	"rm_town", // Celine's Room (door)
	"rm_haydens_house", // TBD (unsure where his room actually is)
	"rm_landens_house_f2", // Landen's Room (open), Ryis' Room (open)
	"rm_clinic_f1", // Valen's Room (open)
	"rm_general_store_home", // Holt & Nora's Room (door), Dell's Room (door)
	"rm_manor_house_entry", // Adeline's Room (door), Eiland's Room (door), Elsie's Room (door)
};
static const std::map<std::string, std::string> NPC_NAME_TO_ROOM_RESTRICTED_LOCALIZATION_KEY = {
	{ "balor", BALOR_ROOM_RESTRICTED_LOCALIZATION_KEY },
	{ "josephine", JOSEPHINE_HEMLOCK_ROOM_RESTRICTED_LOCALIZATION_KEY },
	{ "hemlock", JOSEPHINE_HEMLOCK_ROOM_RESTRICTED_LOCALIZATION_KEY },
	{ "luc", LUC_ROOM_RESTRICTED_LOCALIZATION_KEY },
	{ "maple", MAPLE_ROOM_RESTRICTED_LOCALIZATION_KEY },
	{ "reina", REINA_ROOM_RESTRICTED_LOCALIZATION_KEY }
};

static YYTKInterface* g_ModuleInterface = nullptr;
static CInstance* global_instance = nullptr;
static int ari_x = -1;
static int ari_y = -1;
static bool load_on_start = true;
static bool game_is_active = false;
static bool reposition_ari = false;
static bool display_dialogue = false;
static bool obj_door_focused = false;
static std::string restricted_npc_room = "";
static std::string ari_current_gm_room = "";
static std::map<std::string, int> npc_name_to_id_map = {};
static std::map<int, std::string> npc_id_to_name_map = {};
static struct NpcRoom {
	int entrance_x;
	int entrance_y;
	bool has_door;
	std::optional<int> reposition_x;
	std::optional<int> reposition_y;
	std::unordered_set<std::string> inhabitants;
};
static std::map<std::string, std::vector<NpcRoom>> gm_room_to_npc_rooms = {};

void CreateNpcRooms()
{
	gm_room_to_npc_rooms["rm_inn"].push_back(NpcRoom(40,  256, false, 72,			256,		  { "balor" }));
	gm_room_to_npc_rooms["rm_inn"].push_back(NpcRoom(88,  120, true,  std::nullopt, std::nullopt, { "josephine", "hemlock" }));
	gm_room_to_npc_rooms["rm_inn"].push_back(NpcRoom(184, 120, true,  std::nullopt, std::nullopt, { "luc" }));
	gm_room_to_npc_rooms["rm_inn"].push_back(NpcRoom(328, 120, true,  std::nullopt, std::nullopt, { "maple" }));
	gm_room_to_npc_rooms["rm_inn"].push_back(NpcRoom(424, 120, true,  std::nullopt, std::nullopt, { "reina" }));
}

void ResetStaticFields(bool returned_to_title_screen)
{
	if (returned_to_title_screen)
	{
		ari_x = -1;
		ari_y = -1;
		game_is_active = false;
	}

	reposition_ari = false;
	display_dialogue = false;
	obj_door_focused = false;
	restricted_npc_room = "";
	ari_current_gm_room = "";
}

void PrintError(std::exception_ptr eptr)
{
	try {
		if (eptr) {
			std::rethrow_exception(eptr);
		}
	}
	catch (const std::exception& e) {
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Error: %s", MOD_NAME, VERSION, e.what());
	}
}

json CreateConfigJson(bool use_defaults)
{

}

bool GameIsPaused()
{
	RValue paused = global_instance->GetMember("__pause_status");
	return paused.ToInt64() > 0;
}

bool IsNumeric(RValue value)
{
	return value.m_Kind == VALUE_INT32 || value.m_Kind == VALUE_INT64 || value.m_Kind == VALUE_REAL;
}

bool IsObject(RValue value)
{
	return value.m_Kind == VALUE_OBJECT;
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

double GetDistance(int x1, int y1, int x2, int y2) {
	return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

void LoadNpcNames()
{
	size_t array_length = 0;
	RValue npcs = global_instance->GetMember("__npc_id__");
	g_ModuleInterface->GetArraySize(npcs, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(npcs, i, array_element);
		npc_name_to_id_map[array_element->ToString()] = i;
		npc_id_to_name_map[i] = array_element->ToString();
	}
}

int GetHeartLevelForNpc(std::string npc_name)
{
	RValue* npc;
	RValue npc_database = global_instance->GetMember("__npc_database");
	g_ModuleInterface->GetArrayEntry(npc_database, npc_name_to_id_map[npc_name], npc);

	int heart_points = npc->GetMember("heart_points").ToInt64();
	if (heart_points >= 1755)
		return 10;
	if (heart_points >= 1400)
		return 9;
	if (heart_points >= 1125)
		return 8;
	if (heart_points >= 900)
		return 7;
	if (heart_points >= 705)
		return 6;
	if (heart_points >= 530)
		return 5;
	if (heart_points >= 390)
		return 4;
	if (heart_points >= 280)
		return 3;
	if (heart_points >= 180)
		return 2;
	if (heart_points >= 80)
		return 1;
	return 0;
}

void CreateNotification(std::string notification_localization_str, CInstance* Self, CInstance* Other)
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

	if (game_is_active)
	{
		RValue x;
		g_ModuleInterface->GetBuiltin("x", self, NULL_INDEX, x);
		ari_x = x.ToInt64();

		RValue y;
		g_ModuleInterface->GetBuiltin("y", self, NULL_INDEX, y);
		ari_y = y.ToInt64();

		if(gm_room_to_npc_rooms.contains(ari_current_gm_room) && restricted_npc_room == "")
		{
			for (NpcRoom npc_room : gm_room_to_npc_rooms[ari_current_gm_room])
			{
				if (!npc_room.has_door)
				{
					int distance = GetDistance(ari_x, ari_y, npc_room.entrance_x, npc_room.entrance_y);
					if (distance <= 16) // TODO: Tune this
					{
						bool room_restricted = true;
						for (std::string npc_name : npc_room.inhabitants)
						{
							if (GetHeartLevelForNpc(npc_name) >= 2)
							{
								room_restricted = false;
								break;
							}
						}
						
						if (room_restricted)
						{
							RValue reposition_x = *npc_room.reposition_x;
							RValue reposition_y = *npc_room.reposition_y;
							g_ModuleInterface->SetBuiltin("x", self, NULL_INDEX, reposition_x);
							g_ModuleInterface->SetBuiltin("y", self, NULL_INDEX, reposition_y);
							ari_x = reposition_x.ToInt64();
							ari_y = reposition_y.ToInt64();

							restricted_npc_room = *npc_room.inhabitants.begin();
							PlayConversation(ROOM_RESTRICTED_CONVERSATION, global_instance->GetRefMember("__ari")->ToInstance(), self);
							break;
						}
					}
				}
			}
		}
	}
}

RValue& GmlScriptPlayConversationCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (game_is_active && ari_current_gm_room == "rm_inn" && ArgumentCount >= 2 && Arguments[1]->m_Kind == VALUE_STRING)
	{
		if (Arguments[1]->ToString() == "Conversations/flavor_text/inn_locked_door")
			*Arguments[1] = RValue(ANNA_EASTER_EGG_CONVERSATION);
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

RValue& GmlScriptGetLocalizerCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (game_is_active)
	{
		std::string localization_key = Arguments[0]->ToString();
		if (localization_key == ROOM_RESTRICTED_PLACEHOLDER_LOCALIZATION_KEY)
		{
			if (NPC_NAME_TO_ROOM_RESTRICTED_LOCALIZATION_KEY.contains(restricted_npc_room))
				*Arguments[0] = RValue(NPC_NAME_TO_ROOM_RESTRICTED_LOCALIZATION_KEY.at(restricted_npc_room));

			restricted_npc_room = "";
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_LOCALIZER));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active && ari_current_gm_room == "rm_inn" && Arguments[0]->ToString() == "npcs/seridia/name")
		Result = RValue("Anna");

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

RValue& GmlScriptAttemptInteractCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (game_is_active && Self->m_Object != nullptr && gm_room_to_npc_rooms.contains(ari_current_gm_room))
	{
		std::string self_name = Self->m_Object->m_Name;

		if (self_name == "obj_door")
			obj_door_focused = true;
		else
			obj_door_focused = false;
	}
	else
	{
		obj_door_focused = false;
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

	if (game_is_active && !GameIsPaused() && obj_door_focused && Arguments[0]->ToInt64() == 6 && Result.ToBoolean())
	{
		for (NpcRoom npc_room : gm_room_to_npc_rooms[ari_current_gm_room])
		{
			if (npc_room.has_door)
			{
				int distance = GetDistance(ari_x, ari_y, npc_room.entrance_x, npc_room.entrance_y);
				if (distance <= 32) // TODO: Tune this
				{
					bool room_restricted = true;
					for (std::string npc_name : npc_room.inhabitants)
					{
						if (GetHeartLevelForNpc(npc_name) >= 2)
						{
							room_restricted = false;
							break;
						}
					}

					if (room_restricted)
					{
						restricted_npc_room = *npc_room.inhabitants.begin();
						PlayConversation(ANNA_EASTER_EGG_CONVERSATION, Self, Other);

						Result = false;
						return Result;
					}
				}
			}
		}
	}
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

	return Result;
}

RValue& GmlScriptVertigoDrawWithColorCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (game_is_active && ari_current_gm_room == "rm_inn")
	{
		RValue type = g_ModuleInterface->CallBuiltin("asset_get_type", { *Arguments[0] });
		if (type.ToInt64() == 1) // asset_sprite
		{
			RValue sprite_name = g_ModuleInterface->CallBuiltin("sprite_get_name", { *Arguments[0] });
			if (sprite_name.ToString() == "spr_portrait_seridia_priestess_neutral")
			{
				RValue replacement_sprite = g_ModuleInterface->CallBuiltin("asset_get_index", { ANNA_PORTRAIT_SPRITE_NAME.c_str() });
				if (replacement_sprite.m_Kind == VALUE_REF)
					*Arguments[0] = replacement_sprite;
			}
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_VERTIGO_DRAW_WITH_COLOR));
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
		load_on_start = false;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		CreateNpcRooms();
		LoadNpcNames();
	}
	else
	{
		ResetStaticFields(true);
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

void CreateHookGmlScriptVertigoDrawWithColor(AurieStatus& status)
{
	CScript* gml_script_vertigo_draw_with_color = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_VERTIGO_DRAW_WITH_COLOR,
		(PVOID*)&gml_script_vertigo_draw_with_color
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_VERTIGO_DRAW_WITH_COLOR);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_VERTIGO_DRAW_WITH_COLOR,
		gml_script_vertigo_draw_with_color->m_Functions->m_ScriptFunction,
		GmlScriptVertigoDrawWithColorCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_VERTIGO_DRAW_WITH_COLOR);
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

	CreateHookGmlScriptPlayConversation(status);
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

	CreateHookGmlScriptAttemptInteract(status);
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

	CreateHookGmlScriptGoToRoom(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptVertigoDrawWithColor(status);
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
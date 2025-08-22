#include <unordered_set>
#include <fstream>
#include <YYToolkit/YYTK_Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static const char* const MOD_NAME = "Elegance";
static const char* const VERSION = "1.0.0";
static const char* const GML_SCRIPT_CREATE_NOTIFICATION = "gml_Script_create_notification";
static const char* const GML_SCRIPT_CLOSE_TEXTBOX = "gml_Script_begin_close@TextboxMenu@TextboxMenu";
static const char* const GML_SCRIPT_PLAY_CONVERSATION = "gml_Script_play_conversation";
static const char* const GML_SCRIPT_GET_GOSSIP_SELECTIONS = "gml_Script_get_gossip_selections";
static const char* const GML_SCRIPT_ADD_NPC_HEART_POINTS = "gml_Script_add_heart_points@Npc@Npc";
static const char* const GML_SCRIPT_GOSSIP_MENU_ON_CLOSE = "gml_Script_on_close@GossipMenu@GossipMenu";
static const char* const GML_SCRIPT_GET_WEATHER = "gml_Script_get_weather@WeatherManager@Weather";
static const char* const GML_SCRIPT_END_DAY = "gml_Script_end_day";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const std::string ELEGANCE_WONT_GOSSIP_CONVERSATION_KEY = "Conversations/Mods/Elegance/wont_gossip";
static const std::string ELEGANCE_GOSSIP_ONCE_MORE_NOTIFICATION_KEY = "Notifications/Mods/Elegance/gossip_once_more";
static const std::string ELEGANT_HAT_ITEM_NAME = "elegant_hat";
static const std::string ELEGANT_DRESS_ITEM_NAME = "elegant_dress";
static const std::string ELEGANT_SHOES_ITEM_NAME = "elegant_shoes";
static const std::string TEAPOT_HAT_ITEM_NAME = "head_teapot";
static const std::unordered_set<std::string> ELEGANT_SET_ITEMS = { ELEGANT_HAT_ITEM_NAME, ELEGANT_DRESS_ITEM_NAME, ELEGANT_SHOES_ITEM_NAME };
static const std::unordered_set<std::string> INELEGANT_ITEMS = { TEAPOT_HAT_ITEM_NAME };
static const std::string ELSIE_NO_GOSSIP_CONVERSATION_KEY = "Conversations/gossip/no_gossip";

static YYTKInterface* g_ModuleInterface = nullptr;
static CInstance* global_instance = nullptr;
static bool game_is_active = false;
static bool inelegant_cosmetics_equipped = false;
static bool wont_gossip = false;
static int elegant_cosmetics_equipped = 0;
static bool can_gossip_again = true;
static std::map<std::string, int> item_name_to_id_map = {};

void ResetStaticFields(bool returned_to_title_screen)
{
	if (returned_to_title_screen)
	{
		game_is_active = false;
		inelegant_cosmetics_equipped = false;
		wont_gossip = false;
		elegant_cosmetics_equipped = 0;
	}
	
	can_gossip_again = true;
}

void ResetHasGossipedToday()
{
	global_instance->ToRValue()["__ari"]["has_gossiped_today"] = false;
}

bool InelegantItemsEquipped()
{
	RValue __ari = *global_instance->GetRefMember("__ari");
	RValue preset_index_selected = *__ari.GetRefMember("preset_index_selected");
	RValue presets = *__ari.GetRefMember("presets");
	RValue buffer = *presets.GetRefMember("__buffer");

	RValue* selelected_buffer_entry; // Ari's current cosmetics
	g_ModuleInterface->GetArrayEntry(buffer, preset_index_selected.m_Real, selelected_buffer_entry);

	RValue assets = *selelected_buffer_entry->GetRefMember("assets");
	RValue inner_buffer = *assets.GetRefMember("__buffer");
	size_t inner_buffer_size;
	g_ModuleInterface->GetArraySize(inner_buffer, inner_buffer_size);

	for (int i = 0; i < inner_buffer_size; i++)
	{
		RValue* equipped_cosmetic;
		g_ModuleInterface->GetArrayEntry(inner_buffer, i, equipped_cosmetic);

		RValue equipped_cosmetic_name = *equipped_cosmetic->GetRefMember("name");
		if (INELEGANT_ITEMS.contains(equipped_cosmetic_name.ToString()))
			return true;
	}

	return false;
}

int CountElegantItemsEquipped()
{
	RValue __ari = *global_instance->GetRefMember("__ari");
	RValue preset_index_selected = *__ari.GetRefMember("preset_index_selected");
	RValue presets = *__ari.GetRefMember("presets");
	RValue buffer = *presets.GetRefMember("__buffer");

	RValue* selelected_buffer_entry; // Ari's current cosmetics
	g_ModuleInterface->GetArrayEntry(buffer, preset_index_selected.m_Real, selelected_buffer_entry);

	RValue assets = *selelected_buffer_entry->GetRefMember("assets");
	RValue inner_buffer = *assets.GetRefMember("__buffer");
	size_t inner_buffer_size;
	g_ModuleInterface->GetArraySize(inner_buffer, inner_buffer_size);

	int elegant_set_items_equipped = 0;
	for (int i = 0; i < inner_buffer_size; i++)
	{
		RValue* equipped_cosmetic;
		g_ModuleInterface->GetArrayEntry(inner_buffer, i, equipped_cosmetic);

		RValue equipped_cosmetic_name = *equipped_cosmetic->GetRefMember("name");
		if(ELEGANT_SET_ITEMS.contains(equipped_cosmetic_name.ToString()))
			elegant_set_items_equipped++;
	}

	return elegant_set_items_equipped;
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

void CloseTextbox(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_close_textbox = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CLOSE_TEXTBOX,
		(PVOID*)&gml_script_close_textbox
	);

	RValue result;
	gml_script_close_textbox->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		0,
		nullptr
	);
}

void CloseGossipMenu(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_close_gossip_menu = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GOSSIP_MENU_ON_CLOSE,
		(PVOID*)&gml_script_close_gossip_menu
	);

	RValue result;
	gml_script_close_gossip_menu->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		0,
		nullptr
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
		elegant_cosmetics_equipped = CountElegantItemsEquipped();
		inelegant_cosmetics_equipped = InelegantItemsEquipped();
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
	if (game_is_active && wont_gossip)
	{
		if (ELSIE_NO_GOSSIP_CONVERSATION_KEY == Arguments[1]->ToString())
		{
			wont_gossip = false;
			*Arguments[1] = RValue(ELEGANCE_WONT_GOSSIP_CONVERSATION_KEY);
		}
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

RValue& GmlScriptPlayTextCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_GOSSIP_SELECTIONS));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active && inelegant_cosmetics_equipped)
	{
		wont_gossip = true;
		Result = g_ModuleInterface->CallBuiltin("array_create", { 0 });
	}
	
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
	if (elegant_cosmetics_equipped == 1)
	{
		*Arguments[0] = Arguments[0]->ToDouble() + 1;
		g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Your elegance earns you 1 additional heart point!", MOD_NAME, VERSION);
	}
	if (elegant_cosmetics_equipped == 2)
	{
		*Arguments[0] = Arguments[0]->ToDouble() + 3;
		g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Your elegance earns you 3 additional heart point!", MOD_NAME, VERSION);
	}
	if (elegant_cosmetics_equipped == 3)
	{
		*Arguments[0] = Arguments[0]->ToDouble() + 5;
		g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Your elegance earns you 5 additional heart point!", MOD_NAME, VERSION);
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_ADD_NPC_HEART_POINTS));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptGossipMenuOnCloseCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GOSSIP_MENU_ON_CLOSE));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (elegant_cosmetics_equipped == 3 && can_gossip_again)
	{
		ResetHasGossipedToday();
		can_gossip_again = false;

		g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Your elegance? allows you to gossip once more today!", MOD_NAME, VERSION);
		CreateNotification(ELEGANCE_GOSSIP_ONCE_MORE_NOTIFICATION_KEY, Self, Other);
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

RValue& GmlScriptEndDayCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_END_DAY));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	ResetStaticFields(false);
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
	g_ModuleInterface->GetGlobalInstance(&global_instance);
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

void CreateHookGmlScriptPlayText(AurieStatus& status)
{
	CScript* gml_script_play_text = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_GOSSIP_SELECTIONS,
		(PVOID*)&gml_script_play_text
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_GOSSIP_SELECTIONS);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_GOSSIP_SELECTIONS,
		gml_script_play_text->m_Functions->m_ScriptFunction,
		GmlScriptPlayTextCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_GOSSIP_SELECTIONS);
	}
}

void CreateHookGmlScriptAddHeartPoints(AurieStatus& status)
{
	CScript* gml_script_add_heart_points = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_ADD_NPC_HEART_POINTS,
		(PVOID*)&gml_script_add_heart_points
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ADD_NPC_HEART_POINTS);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_ADD_NPC_HEART_POINTS,
		gml_script_add_heart_points->m_Functions->m_ScriptFunction,
		GmlScriptAddHeartPointsCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ADD_NPC_HEART_POINTS);
	}
}

void CreateHookGmlScriptGossipMenuOnClose(AurieStatus& status)
{
	CScript* gml_script_gossip_menu_on_close = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GOSSIP_MENU_ON_CLOSE,
		(PVOID*)&gml_script_gossip_menu_on_close
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GOSSIP_MENU_ON_CLOSE);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GOSSIP_MENU_ON_CLOSE,
		gml_script_gossip_menu_on_close->m_Functions->m_ScriptFunction,
		GmlScriptGossipMenuOnCloseCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GOSSIP_MENU_ON_CLOSE);
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

void CreateHookGmlScriptEndDay(AurieStatus& status)
{
	CScript* gml_script_end_day = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_END_DAY,
		(PVOID*)&gml_script_end_day
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_END_DAY);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_END_DAY,
		gml_script_end_day->m_Functions->m_ScriptFunction,
		GmlScriptEndDayCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_END_DAY);
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

	CreateHookGmlScriptPlayText(status);
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

	CreateHookGmlScriptGossipMenuOnClose(status);
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

	CreateHookGmlScriptEndDay(status);
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
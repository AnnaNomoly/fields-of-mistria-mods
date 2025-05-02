#include <map>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static const char* const MOD_NAME = "ShipIt";
static const char* const VERSION = "1.0.0";
static const char* const GML_SCRIPT_TRY_OBJECT_ID_TO_STRING = "gml_Script_try_object_id_to_string";
static const char* const GML_SCRIPT_INTERACT = "gml_Script_interact";
static const char* const GML_SCRIPT_PLAY_CONVERSATION = "gml_Script_play_conversation";
static const char* const GML_SCRIPT_PLAY_TEXT = "gml_Script_play_text@TextboxMenu@TextboxMenu";
static const char* const GML_SCRIPT_GET_LOCALIZER = "gml_Script_get@Localizer@Localizer";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const char* const GML_SCRIPT_SELL_SHIPPING_BIN_ITEMS = "gml_Script_sell_shipping_bin_items";
static const std::string CUSTOM_OBJECT_NAME = "balor_crystal_ball";
static const std::string SELL_SHIPPING_BIN_DIALOGUE_KEY = "Conversations/Mods/Sell Items/sell_shipping_bin_items/1";
static const std::string SELL_SHIPPING_BIN_CONVERSATION_KEY = "Conversations/Mods/Sell Items/sell_shipping_bin_items";
static const std::string ITEMS_SOLD_NOTIFICATION_KEY = "Notifications/Mods/Sell Items/items_sold";
static const std::string VALUE_PLACEHOLDER_TEXT = "<VALUE>";

static YYTKInterface* g_ModuleInterface = nullptr;
static bool mod_healthy = false;
static bool load_on_start = true;
static std::map<int, std::string> object_id_to_name_map = {};
static bool custom_object_used = false;
static RValue custom_conversation_value;
static RValue* custom_conversation_value_ptr = nullptr;
static RValue gold_earned;

void ResetStaticFields()
{
	custom_object_used = false;
	custom_conversation_value = "";
	custom_conversation_value_ptr = nullptr;
	gold_earned = 0;
}

int RValueAsInt(RValue value)
{
	if (value.m_Kind == VALUE_REAL)
		return static_cast<int>(value.m_Real);
	if(value.m_Kind == VALUE_INT64)
		return static_cast<int>(value.m_i64);
	if (value.m_Kind == VALUE_INT32)
		return static_cast<int>(value.m_i32);
}

void LoadObjectIds(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_try_object_id_to_string = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_TRY_OBJECT_ID_TO_STRING,
		(PVOID*)&gml_script_try_object_id_to_string
	);

	for (int i = 0; i < 3000; i++)
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

	//CInstance* global_instance = nullptr;
	//g_ModuleInterface->GetGlobalInstance(&global_instance);

	//RValue __object_id__ = global_instance->at("__object_id__");
	//RValue array_length = g_ModuleInterface->CallBuiltin("array_length", { __object_id__ });
	//for (int i = 0; i < array_length.m_Real; i++)
	//{
	//	RValue object_name = g_ModuleInterface->CallBuiltin("array_get", { __object_id__, i });
	//	object_id_to_name_map[i] = object_name.AsString().data();
	//}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Loaded %d objects!", MOD_NAME, VERSION, static_cast<int>(object_id_to_name_map.size()));
}

void DisplayNotification(CInstance* Self, CInstance* Other, std::string localization_key)
{
	CScript* gml_script_create_notification = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_create_notification",
		(PVOID*)&gml_script_create_notification
	);

	RValue result;
	RValue notification = localization_key;
	RValue* notification_ptr = &notification;
	gml_script_create_notification->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &notification_ptr }
	);
}

void SellShippingBinItems(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_sell_shipping_bin_items = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SELL_SHIPPING_BIN_ITEMS,
		(PVOID*)&gml_script_sell_shipping_bin_items
	);

	RValue result;
	gml_script_sell_shipping_bin_items->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		0,
		nullptr
	);

	gold_earned = result.at("gold_earned"); // VALUE_REAL
	DisplayNotification(Self, Other, ITEMS_SOLD_NOTIFICATION_KEY);
}

RValue& GmlScriptInteractCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_healthy)
	{
		RValue object = Arguments[0]->m_Object;
		int object_id = RValueAsInt(object.at("object_id"));
		std::string object_name = object_id_to_name_map[object_id];

		if (object_name.find(CUSTOM_OBJECT_NAME) != std::string::npos)
			custom_object_used = true;
	}

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

RValue& GmlScriptPlayConversationCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_healthy && custom_object_used)
	{
		custom_object_used = false;
		custom_conversation_value = SELL_SHIPPING_BIN_CONVERSATION_KEY;
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

RValue& GmlScriptGetLocalizerCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_LOCALIZER));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (mod_healthy)
	{
		std::string localization_key = Arguments[0]->AsString().data();
		if (localization_key.find(ITEMS_SOLD_NOTIFICATION_KEY) != std::string::npos)
		{
			std::string result_str = Result.AsString().data();
			std::string value_str = std::to_string(RValueAsInt(gold_earned));

			// Replace the <VALUE> placeholder.
			size_t value_placeholder_index = result_str.find(VALUE_PLACEHOLDER_TEXT);
			if (value_placeholder_index != std::string::npos)
				result_str.replace(value_placeholder_index, VALUE_PLACEHOLDER_TEXT.length(), value_str);
			Result = result_str;
		}
	}

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
	if (mod_healthy)
	{
		std::string conversation_name = Arguments[0]->AsString().data();
		if (conversation_name.find(SELL_SHIPPING_BIN_DIALOGUE_KEY) != std::string::npos)
		{
			SellShippingBinItems(Self, Other);
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
		LoadObjectIds(Self, Other);

		if (!object_id_to_name_map.empty())
			mod_healthy = true;
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

void CreateHookGmlScriptInteract(AurieStatus& status)
{
	CScript* gml_script_play_conversation = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_INTERACT,
		(PVOID*)&gml_script_play_conversation
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_INTERACT);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_INTERACT,
		gml_script_play_conversation->m_Functions->m_ScriptFunction,
		GmlScriptInteractCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_INTERACT);
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

	CreateHookGmlScriptInteract(status);
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

	CreateHookGmlScriptGetLocalizer(status);
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
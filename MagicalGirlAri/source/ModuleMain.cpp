#include <unordered_set>
#include <fstream>
#include <YYToolkit/YYTK_Shared.hpp> // YYTK v4
using namespace Aurie;
using namespace YYTK;

static const char* const MOD_NAME = "MagicalGirlAri";
static const char* const VERSION = "1.0.0";
static const char* const GML_SCRIPT_PLAY_CONVERSATION = "gml_Script_play_conversation";
static const char* const GML_SCRIPT_NPC_RECEIVE_GIFT = "gml_Script_receive_gift@gml_Object_par_NPC_Create_0";
static const char* const GML_SCRIPT_MODIFY_MANA = "gml_Script_modify_mana@Ari@Ari";
static const char* const GML_SCRIPT_GET_WEATHER = "gml_Script_get_weather@WeatherManager@Weather";
static const char* const GML_SCRIPT_ARI_ON_NEW_DAY = "gml_Script_on_new_day@Ari@Ari";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const std::string JUNIPER_EASTER_EGG_CONVERSATION_KEY = "Conversations/Mods/MagicalGirlAri/juniper_contract";
static const std::string SAILOR_MISTRIA_DRESS_ITEM_NAME = "sailor_mistria";
static const std::string SAILOR_MISTRIA_HEELS_ITEM_NAME = "sailor_mistria_heels";
static const std::string SAILOR_MISTRIA_BOOTS_ITEM_NAME = "sailor_mistria_boots";
static const std::string PRINCESS_TWINTAILS_HAIR_NAME = "princess_twintails";
static const std::unordered_set<std::string> MAGICAL_GIRL_ARI_COSMETIC_NAMES = { SAILOR_MISTRIA_DRESS_ITEM_NAME, SAILOR_MISTRIA_HEELS_ITEM_NAME, SAILOR_MISTRIA_BOOTS_ITEM_NAME, PRINCESS_TWINTAILS_HAIR_NAME };

static YYTKInterface* g_ModuleInterface = nullptr;
static CInstance* global_instance = nullptr;
static bool load_on_start = true;
static bool game_is_active = false;
static bool is_new_day = false;
static bool modify_juniper_conversation = false;
static int pinned_spell = -1;
static int magical_girl_ari_cosmetics_equipped = 0;
static int mana_potion_item_id = -1;
static std::map<int, int> spell_id_to_default_cost_map = {};

void ResetStaticFields(bool returned_to_title_screen)
{
	if (returned_to_title_screen)
	{
		game_is_active = false;
		is_new_day = false;
		modify_juniper_conversation = false;
		pinned_spell = -1;
		magical_girl_ari_cosmetics_equipped = 0;
	}
}

void LoadManaPotionItemId()
{
	size_t array_length = 0;
	RValue item_id_array = global_instance->GetMember("__item_id__");
	g_ModuleInterface->GetArraySize(item_id_array, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(item_id_array, i, array_element);

		std::string item_name = array_element->ToString();
		if (item_name == "mana_potion")
		{
			mana_potion_item_id = i;
			return;
		}
	}
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

void ModifySpellCosts(int mana_cost_reduction) {
	size_t array_length = 0;
	RValue spells = global_instance->GetMember("__spells");
	g_ModuleInterface->GetArraySize(spells, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(spells, i, array_element);

		*array_element->GetRefMember("cost") = spell_id_to_default_cost_map[i] - mana_cost_reduction;
	}
}

int CountMagicalGirlAriCosmeticsEquipped()
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

	int magical_girl_ari_cosmetics_equipped = 0;
	for (int i = 0; i < inner_buffer_size; i++)
	{
		RValue* equipped_cosmetic;
		g_ModuleInterface->GetArrayEntry(inner_buffer, i, equipped_cosmetic);

		RValue equipped_cosmetic_name = *equipped_cosmetic->GetRefMember("name");
		if (MAGICAL_GIRL_ARI_COSMETIC_NAMES.contains(equipped_cosmetic_name.ToString()))
			magical_girl_ari_cosmetics_equipped++;
	}

	return magical_girl_ari_cosmetics_equipped;
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
		int count = CountMagicalGirlAriCosmeticsEquipped();;
		if (magical_girl_ari_cosmetics_equipped != count)
		{
			magical_girl_ari_cosmetics_equipped = count;
			ModifySpellCosts(magical_girl_ari_cosmetics_equipped > 2 ? 2 : magical_girl_ari_cosmetics_equipped);
		}
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook (EVENT_OBJECT_CALL)!", MOD_NAME, VERSION);
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
	if (game_is_active)
	{
		// Conversations/Bank/Juniper/Gift Lines/gift_lines/
		std::string conversation_name = Arguments[1]->ToString();

		if (conversation_name.contains("gift_lines"))
		{
			if (conversation_name.contains("Conversations/Bank/Juniper"))
			{
				if (modify_juniper_conversation)
				{
					modify_juniper_conversation = false;
					*Arguments[1] = RValue(JUNIPER_EASTER_EGG_CONVERSATION_KEY);
				}
			}
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

RValue& GmlScriptNpcReceiveGiftCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (Self != NULL && Self->m_Object != NULL)
	{
		std::string npc_name = Self->m_Object->m_Name;
		if (npc_name == "obj_juniper")
		{
			if(ArgumentCount == 0 || ArgumentCount > 1)
				g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Expected 1 argument but got %d for %s!", MOD_NAME, VERSION, ArgumentCount, GML_SCRIPT_NPC_RECEIVE_GIFT);

			if (ArgumentCount == 1 && Arguments[0]->m_Kind == VALUE_OBJECT)
			{
				RValue item_id = Arguments[0]->GetMember("item_id");
				if (item_id.ToInt64() == mana_potion_item_id && magical_girl_ari_cosmetics_equipped >= 3)
					modify_juniper_conversation = true;
			}
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_NPC_RECEIVE_GIFT));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

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
	if (is_new_day)
	{
		if (magical_girl_ari_cosmetics_equipped >= 3)
		{
			double mana_modifier = Arguments[0]->m_Real;
			Arguments[0]->m_Real = mana_modifier + 1;
		}

		is_new_day = false;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_MODIFY_MANA));
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

RValue& GmlScriptAriOnNewDayCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	is_new_day = true;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_ARI_ON_NEW_DAY));
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
		g_ModuleInterface->GetGlobalInstance(&global_instance);
		LoadSpells();
		LoadManaPotionItemId();
		load_on_start = false;
	}

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

void CreateHookGmlScriptNpcReceiveGift(AurieStatus& status)
{
	CScript* gml_script_npc_receive_gift = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_NPC_RECEIVE_GIFT,
		(PVOID*)&gml_script_npc_receive_gift
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_NPC_RECEIVE_GIFT);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_NPC_RECEIVE_GIFT,
		gml_script_npc_receive_gift->m_Functions->m_ScriptFunction,
		GmlScriptNpcReceiveGiftCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_NPC_RECEIVE_GIFT);
	}
}

void CreateHookGmlScriptModifyMana(AurieStatus& status)
{
	CScript* gml_script_modify_mana = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_MODIFY_MANA,
		(PVOID*)&gml_script_modify_mana
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_MODIFY_MANA);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_MODIFY_MANA,
		gml_script_modify_mana->m_Functions->m_ScriptFunction,
		GmlScriptModifyManaCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_MODIFY_MANA);
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

void CreateHookGmlScriptAriOnNewDay(AurieStatus& status)
{
	CScript* gml_script_ari_on_new_day = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_ARI_ON_NEW_DAY,
		(PVOID*)&gml_script_ari_on_new_day
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ARI_ON_NEW_DAY);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_ARI_ON_NEW_DAY,
		gml_script_ari_on_new_day->m_Functions->m_ScriptFunction,
		GmlScriptAriOnNewDayCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ARI_ON_NEW_DAY);
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

	CreateHookGmlScriptNpcReceiveGift(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptModifyMana(status);
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

	CreateHookGmlScriptAriOnNewDay(status);
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
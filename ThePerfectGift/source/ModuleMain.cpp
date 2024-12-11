#include <map>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <shlobj.h>
#include <filesystem>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static const char* const VERSION = "1.0.0";
static const std::string ADELINE_GIFTS[] = { "animal_cosmetic", "cosmetic", "crafting_scroll", "recipe_scroll", "unidentified_artifact" };
static const std::map<std::string, std::vector<std::string>> GIFT_DIALOG_MAP = {
	{ "Conversations/Bank/Adeline/Banked Lines/inn_work/inn_work/init", { "adeline", "wildberry_scone" } },
	{ "Conversations/Bank/Adeline/Banked Lines/loves_paperwork/loves_paperwork/init", { "adeline", "paper" } },
	{ "Conversations/Bank/Adeline/Banked Lines/wine_cellar/wine_cellar/1", { "adeline", "red_wine" } },
	{ "Conversations/Bank/Adeline/Banked Lines/wine_cellar/wine_cellar/init", { "adeline", "coffee" } },
	{ "Conversations/Bank/Adeline/Market Lines/market_darcy_4/market_darcy_4/init", { "adeline", "spicy_cheddar_biscuit" } },
	{ "Conversations/Festival Lines/Adeline/shooting_star/shooting_star_romantic_follow_up_A/1", { "adeline", "coffee" } },
	{ "Cutscenes/Story Events/adeline_quest_board/adeline_quest_board/15", { "adeline", "tulip" } },
	{ "Cutscenes/Story Events/adeline_quest_board/adeline_quest_board/17", { "adeline", "tulip" } },
};

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_items = true;
static boolean unlock_gift = false; // Used to indicate when a gift preference has been mentioned in dialog
static std::string gift_npc = "";
static std::string gift_item = "";
static std::map<std::string, int> item_name_to_id_map = {};

// DEBUG ONLY ---------------------------------------------------------------
static int debug_counter = 0;
bool EnumFunction(
	IN const char* MemberName,
	IN OUT RValue* Value
)
{
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "Member Name: %s", MemberName);
	return false;
}
//---------------------------------------------------------------------------

std::string wstr_to_string(std::wstring wstr)
{
	std::vector<char> buf(wstr.size());
	std::use_facet<std::ctype<wchar_t>>(std::locale{}).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());

	return std::string(buf.data(), buf.size());
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

	if (unlock_gift)
	{
		if (strcmp(gift_npc.c_str(), "adeline") == 0)
		{
			if (strstr(self->m_Object->m_Name, "obj_adeline"))
			{
				RValue me_exists = g_ModuleInterface->CallBuiltin(
					"struct_exists", {
						self, "me"
					}
				);

				if (me_exists.m_Kind == VALUE_BOOL && me_exists.m_Real == 1)
				{
					RValue me = self->at("me");
					RValue gifts_given = me.at("gifts_given");
					RValue inner = gifts_given.at("inner");

					RValue x = g_ModuleInterface->CallBuiltin(
						"struct_get", {
							inner, std::to_string(item_name_to_id_map[gift_item]).c_str()
						}
					);

					if (x.m_Kind == VALUE_UNDEFINED)
					{
						RValue zero = 0.0;
						g_ModuleInterface->CallBuiltin(
							"struct_set", {
								inner, std::to_string(item_name_to_id_map[gift_item]).c_str(), zero
							}
						);
					}

					unlock_gift = false;
				}
			}
		}
	}


	//if (strstr(self->m_Object->m_Name, "obj_adeline"))
	//{
	//	g_ModuleInterface->Print(CM_WHITE, "========== %s ==========", "obj_adeline");
	//	g_ModuleInterface->EnumInstanceMembers(self, EnumFunction);
	//	g_ModuleInterface->Print(CM_WHITE, "==============================");

	//	RValue me = self->at("me");
	//	g_ModuleInterface->Print(CM_WHITE, "========== %s ==========", "me");
	//	g_ModuleInterface->EnumInstanceMembers(me, EnumFunction);
	//	g_ModuleInterface->Print(CM_WHITE, "==============================");

	//	RValue gifts_given = me.at("gifts_given");
	//	g_ModuleInterface->Print(CM_WHITE, "========== %s ==========", "gifts_given");
	//	g_ModuleInterface->EnumInstanceMembers(gifts_given, EnumFunction);
	//	g_ModuleInterface->Print(CM_WHITE, "==============================");

	//	RValue inner = gifts_given.at("inner");
	//	g_ModuleInterface->Print(CM_WHITE, "========== %s ==========", "inner");
	//	g_ModuleInterface->EnumInstanceMembers(inner, EnumFunction);
	//	g_ModuleInterface->Print(CM_WHITE, "==============================");

	//	//RValue tulip = inner.at("1588");

	//	RValue zero = 0.0;
	//	g_ModuleInterface->CallBuiltin(
	//		"struct_set", {
	//			inner, "1588", zero
	//		}
	//	);
	//}
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
		for (int64_t i = 0; i < 2000; i++)
		{
			CScript* gml_script_try_item_id_to_string = nullptr;
			g_ModuleInterface->GetNamedRoutinePointer(
				"gml_Script_try_item_id_to_string",
				(PVOID*)&gml_script_try_item_id_to_string
			);

			RValue* item_id = new RValue(i);
			RValue item_name;
			gml_script_try_item_id_to_string->m_Functions->m_ScriptFunction(
				Self,
				Other,
				item_name,
				1,
				{ &item_id }
			);
			delete item_id;

			if (item_name.m_Kind != VALUE_NULL && item_name.m_Kind != VALUE_UNSET && item_name.m_Kind != VALUE_UNDEFINED)
			{
				const char* item_name_str = item_name.AsString().data();
				if (item_name_to_id_map.count(item_name_str) <= 0)
				{
					item_name_to_id_map[item_name_str] = i;
				}
			}
		}
		if (item_name_to_id_map.size() > 0)
		{
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[ThePerfectGift %s] - Loaded data for %d items!", VERSION, item_name_to_id_map.size());
		}
		else {
			g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to load data for items!", VERSION);
		}

		load_items = false;
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

RValue& GmlScriptTranslateCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (debug_counter == 0)
	{
		RValue test = "Conversations/Bank/Adeline/Banked Lines/wine_cellar/wine_cellar/init";
		Arguments[0] = &test;
	}
	if (debug_counter == 1)
	{
		RValue test = "Conversations/Bank/Adeline/Banked Lines/wine_cellar/wine_cellar/1";
		Arguments[0] = &test;
	}
	debug_counter++;

	std::string dialog_string = Arguments[0]->AsString().data();
	if (GIFT_DIALOG_MAP.count(dialog_string) > 0)
	{
		unlock_gift = true;
		gift_npc = GIFT_DIALOG_MAP.at(dialog_string)[0];
		gift_item = GIFT_DIALOG_MAP.at(dialog_string)[1];
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_say@TextboxMenu@TextboxMenu"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to get script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to hook script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
	}
}

void CreateHookGmlScriptTranslate(AurieStatus& status)
{
	CScript* gml_script_translate = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_say@TextboxMenu@TextboxMenu",
		(PVOID*)&gml_script_translate
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to get script (gml_Script_say@TextboxMenu@TextboxMenu)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_say@TextboxMenu@TextboxMenu",
		gml_script_translate->m_Functions->m_ScriptFunction,
		GmlScriptTranslateCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to hook script (gml_Script_say@TextboxMenu@TextboxMenu)!", VERSION);
	}
}

void CreateHookEventObject(AurieStatus& status)
{
	status = g_ModuleInterface->CreateCallback(
		g_ArSelfModule,
		EVENT_OBJECT_CALL,
		ObjectCallback,
		0
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Failed to hook EVENT_OBJECT_CALL!", VERSION);
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

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[ThePerfectGift %s] - Plugin starting...", VERSION);

	CreateHookEventObject(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptTranslate(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[ThePerfectGift %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[ThePerfectGift %s] - Plugin started!", VERSION);
	return AURIE_SUCCESS;
}
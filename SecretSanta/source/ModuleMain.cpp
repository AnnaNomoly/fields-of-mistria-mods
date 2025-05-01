#include <random>
#include <fstream>
#include <nlohmann/json.hpp>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

static const char* const YYTK_KEY = "__YYTK";
static const char* const DIG_UP_ANYTHING_KEY = "DigUpAnything";
static const char* const SECRET_SANTA_KEY = "SecretSanta";
static const char* const IGNORE_NEXT_DIG_SPOT_KEY = "ignore_next_dig_spot";
static const char* const VERSION = "1.1.0";
static const double UNSET_INT = -1;
static const double UNSET_DOUBLE = -1.0;
static const std::string GIFTS[] = {
	"berry_bowl", "beet_soup", "fried_rice", "vegetable_pot_pie", "floral_tea",
	"tulip_cake", "sushi_platter", "lobster_roll", "summer_salad", "vegetable_quiche"
};
static const std::string ACTIVE_NPC_NAMES[] = {
	"adeline", "balor", "celine", "darcy", "dell", "dozy", "eiland",
	"elsie", "errol", "hayden", "hemlock", "henrietta", "holt", "josephine",
	"juniper", "landen", "luc", "louis", "maple", "march", "merri",
	"olric", "nora", "reina", "ryis", "terithia", "valen", "vera"
};

// JSON key names
static const std::string SENDER_KEY = "sender";
static const std::string RECIPIENT_KEY = "recipient";
static const std::string MAIL_SENT_KEY = "mail_sent";

// Gif received dialogue.
static const std::string ADELINE_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Adeline/init";
static const std::string BALOR_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Balor/init";
static const std::string CELINE_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Celine/init";
static const std::string DARCY_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Darcy/init";
static const std::string DELL_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Dell/init";
static const std::string DOZY_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Dozy/init";
static const std::string EILAND_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Eiland/init";
static const std::string ELSIE_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Elsie/init";
static const std::string ERROL_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Errol/init";
static const std::string HAYDEN_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Hayden/init";
static const std::string HEMLOCK_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Hemlock/init";
static const std::string HENRIETTA_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Henrietta/init";
static const std::string HOLT_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Holt/init";
static const std::string JOSEPHINE_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Josephine/init";
static const std::string JUNIPER_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Juniper/init";
static const std::string LANDEN_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Landen/init";
static const std::string LUC_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Luc/init";
static const std::string LOUIS_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Louis/init";
static const std::string MAPLE_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Maple/init";
static const std::string MARCH_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/March/init";
static const std::string MERRI_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Merri/init";
static const std::string OLRIC_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Olric/init";
static const std::string NORA_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Nora/init";
static const std::string REINA_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Reina/init";
static const std::string RYIS_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Ryis/init";
static const std::string TERITHIA_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Terithia/init";
static const std::string VALEN_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Valen/init";
static const std::string VERA_GIFT_RECEIVED_DIALOGUE_KEY = "Conversations/Mods/Secret Santa/Vera/init";

static YYTKInterface* g_ModuleInterface = nullptr;
static RValue __YYTK;
static bool load_on_start = true;
static bool mod_healthy = true;
static int day = UNSET_DOUBLE;
static int season = UNSET_DOUBLE;
static int year = UNSET_DOUBLE;
static int stinky_stamina_potion_id = UNSET_INT;
static std::string save_prefix = "";
static std::string mod_folder = "";
static std::string secret_santa_sender = "";
static std::string secret_santa_recipient = "";
static json json_object = json::object();
static RValue custom_dialogue_value;
static RValue* custom_dialogue_value_ptr = nullptr;

void PrintException(std::exception_ptr eptr)
{
	try {
		if (eptr) {
			std::rethrow_exception(eptr);
		}
	}
	catch (const std::exception& e) {
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Error: %s", VERSION, e.what());
	}
}

std::string FormatDateString(int day, int season, int year)
{
	return std::to_string(day) + "-" + std::to_string(season) + "-" + std::to_string(year);
}

void ReadModFile()
{
	// Read the custom mod data file.
	if (save_prefix.length() != 0 && mod_folder.length() != 0)
	{
		std::exception_ptr eptr;
		try
		{
			std::ifstream in_stream(mod_folder + "\\" + save_prefix + ".json");
			if (in_stream.good())
			{
				json_object = json::parse(in_stream);
			}
			else
			{
				json_object = {};
			}
		}
		catch (...)
		{
			mod_healthy = false;
			g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - An error occurred reading the mod file.", VERSION);

			eptr = std::current_exception();
			PrintException(eptr);
		}
	}
	else
	{
		mod_healthy = false;
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - No save prefix detected when trying to read the mod file!", VERSION);
	}
}

void SaveModFile()
{
	// Write the custom mod data file.
	if (save_prefix.length() != 0 && mod_folder.length() != 0)
	{
		std::exception_ptr eptr;
		try
		{
			std::ofstream out_stream(mod_folder + "\\" + save_prefix + ".json");
			out_stream << std::setw(4) << json_object << std::endl;
			out_stream.close();
			g_ModuleInterface->Print(CM_LIGHTGREEN, "[SecretSanta %s] - Saved the mod file!", VERSION);
		}
		catch (...)
		{
			mod_healthy = false;
			g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - An error occurred writing the mod file.", VERSION);

			eptr = std::current_exception();
			PrintException(eptr);
		}
	}
	else
	{
		mod_healthy = false;
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - No save prefix detected when trying to save the mod file!", VERSION);
	}
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

int GetNpcId(std::string npc_name)
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	RValue __npc_id__ = global_instance->at("__npc_id__");
	size_t size = 0;
	g_ModuleInterface->GetArraySize(__npc_id__, size);

	for (size_t i = 0; i < size; i++)
	{
		RValue* element = nullptr;
		g_ModuleInterface->GetArrayEntry(__npc_id__, i, element);

		std::string element_str = element->AsString().data();
		if (element_str == npc_name)
		{
			return static_cast<int>(i);
		}
	}

	return UNSET_INT;
}

void AddHeartPoints(std::string npc_name)
{
	int npc_id = GetNpcId(npc_name);
	if (npc_id != UNSET_INT)
	{
		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		RValue __npc_database = global_instance->at("__npc_database");
		RValue npc_database_0 = __npc_database[npc_id];
		RValue original_heart_points = npc_database_0.at("heart_points");
		RValue modified_heart_points = original_heart_points.m_Real + 20.0;

		g_ModuleInterface->CallBuiltin(
			"struct_set", {
				npc_database_0, "heart_points", modified_heart_points
			}
		);

		g_ModuleInterface->Print(CM_LIGHTGREEN, "[SecretSanta %s] - Added %d bonus heart points for NPC: %s", VERSION, 20, npc_name.c_str());
	}
	else
	{
		g_ModuleInterface->Print(CM_LIGHTYELLOW, "[SecretSanta %s] - Unable to lookup ID for NPC: %s", VERSION, npc_name.c_str());
	}

}

int GetItemId(CInstance* self, CInstance* other, std::string item_name)
{
	CScript* gml_script_try_string_to_item_id = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_try_string_to_item_id",
		(PVOID*)&gml_script_try_string_to_item_id
	);

	RValue result;
	RValue argument = item_name;
	RValue* argument_ptr = &argument;
	gml_script_try_string_to_item_id->m_Functions->m_ScriptFunction(
		self,
		other,
		result,
		1,
		{ &argument_ptr }
	);

	if (result.m_Kind == VALUE_INT64)
		return result.m_i64;
	return UNSET_INT;
}

bool RValueAsBool(RValue value)
{
	if (value.m_Kind == VALUE_BOOL && value.m_Real == 1)
		return true;
	return false;
}

bool GlobalVariableExists(const char* variable_name)
{
	RValue global_variable_exists = g_ModuleInterface->CallBuiltin(
		"variable_global_exists",
		{ variable_name }
	);

	return RValueAsBool(global_variable_exists);
}

RValue GlobalVariableGet(const char* variable_name)
{
	return g_ModuleInterface->CallBuiltin(
		"variable_global_get",
		{ variable_name }
	);
}

RValue GlobalVariableSet(const char* variable_name, RValue value)
{
	return g_ModuleInterface->CallBuiltin(
		"variable_global_set",
		{ variable_name, value }
	);
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

void CreateOrGetGlobalYYTKVariable()
{
	if (!GlobalVariableExists(YYTK_KEY))
	{
		g_ModuleInterface->GetRunnerInterface().StructCreate(&__YYTK);
		GlobalVariableSet(YYTK_KEY, __YYTK);
	}
	else
		__YYTK = GlobalVariableGet(YYTK_KEY);
}

void CreateModInfoInGlobalYYTKVariable()
{
	if (!StructVariableExists(__YYTK, SECRET_SANTA_KEY))
	{
		RValue SecretSanta;
		RValue version = VERSION;
		RValue ignore_next_dig_spot = false;
		g_ModuleInterface->GetRunnerInterface().StructCreate(&SecretSanta);
		g_ModuleInterface->GetRunnerInterface().StructAddRValue(&SecretSanta, "version", &version);
		g_ModuleInterface->GetRunnerInterface().StructAddRValue(&SecretSanta, IGNORE_NEXT_DIG_SPOT_KEY, &ignore_next_dig_spot);
		g_ModuleInterface->GetRunnerInterface().StructAddRValue(&__YYTK, SECRET_SANTA_KEY, &SecretSanta);
	}
}

bool IgnoreNextDigSpot()
{
	if (GlobalVariableExists(YYTK_KEY))
	{
		RValue __YYTK = GlobalVariableGet(YYTK_KEY);
		if (StructVariableExists(__YYTK, SECRET_SANTA_KEY))
		{
			RValue SecretSanta = StructVariableGet(__YYTK, SECRET_SANTA_KEY);
			if (StructVariableExists(SecretSanta, IGNORE_NEXT_DIG_SPOT_KEY))
			{
				RValue ignore_next_dig_spot = StructVariableGet(SecretSanta, IGNORE_NEXT_DIG_SPOT_KEY);
				return RValueAsBool(ignore_next_dig_spot);
			}
		}
	}

	return false;
}

void ResetStaticFields(bool returnedToTitleScreen)
{
	if (returnedToTitleScreen)
	{
		day = UNSET_DOUBLE;
		season = UNSET_DOUBLE;
		year = UNSET_DOUBLE;
		save_prefix = "";
		secret_santa_sender = "";
		secret_santa_recipient = "";
		json_object = {};
		custom_dialogue_value = "";
		custom_dialogue_value_ptr = nullptr;
	}
}

RValue& GmlScriptCalendarDayCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_day@Calendar@Calendar"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (mod_healthy)
	{
		// Result is a VALUE_REAL that is the 0-indexed calendar day
		day = Result.m_Real + 1;
	}

	return Result;
}

RValue& GmlScriptCalendarSeasonCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_season@Calendar@Calendar"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (mod_healthy)
	{
		// Result is a VALUE_REAL that is the 0-indexed calendar season
		season = Result.m_Real + 1;
	}

	return Result;
}

RValue& GmlScriptCalendarYearCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_year@Calendar@Calendar"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (mod_healthy)
	{
		// Result is a VALUE_REAL that is the 0-indexed calendar year
		year = Result.m_Real + 1;
	}

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
	if (mod_healthy)
	{
		if (season == 4 && day == 27)
		{
			std::string conversation_name = Arguments[0]->AsString().data();
			std::string conversation_name_lowercase = conversation_name;
			std::transform(conversation_name_lowercase.begin(), conversation_name_lowercase.end(), conversation_name_lowercase.begin(), [](unsigned char c) { return std::tolower(c); });

			// Dialog involves the secret santa recipient.
			if (conversation_name_lowercase.find(secret_santa_recipient) != std::string::npos)
			{
				// Dialog is a gift line.
				if (conversation_name.find("gift_lines") != std::string::npos)
				{
					// Speaker is Adeline.
					if (conversation_name.find("Conversations/Bank/Adeline") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = ADELINE_GIFT_RECEIVED_DIALOGUE_KEY;
						custom_dialogue_value_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialogue_value_ptr;
					}

					// Speaker is Balor.
					if (conversation_name.find("Conversations/Bank/Balor") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = BALOR_GIFT_RECEIVED_DIALOGUE_KEY;
						custom_dialogue_value_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialogue_value_ptr;
					}

					// Speaker is Celine.
					if (conversation_name.find("Conversations/Bank/Celine") != std::string::npos)
					{
						custom_dialogue_value = CELINE_GIFT_RECEIVED_DIALOGUE_KEY;
						custom_dialogue_value_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialogue_value_ptr;
					}

					// Speaker is Darcy.
					if (conversation_name.find("Conversations/Bank/Darcy") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = DARCY_GIFT_RECEIVED_DIALOGUE_KEY;
						custom_dialogue_value_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialogue_value_ptr;
					}

					// Speaker is Dell.
					if (conversation_name.find("Conversations/Bank/Dell") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = DELL_GIFT_RECEIVED_DIALOGUE_KEY;
						custom_dialogue_value_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialogue_value_ptr;
					}

					// Speaker is Dozy.
					if (conversation_name.find("Conversations/Bank/Dozy") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = DOZY_GIFT_RECEIVED_DIALOGUE_KEY;
						custom_dialogue_value_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialogue_value_ptr;
					}

					// Speaker is Eiland.
					if (conversation_name.find("Conversations/Bank/Eiland") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = EILAND_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Elsie.
					if (conversation_name.find("Conversations/Bank/Elsie") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = ELSIE_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Errol.
					if (conversation_name.find("Conversations/Bank/Errol") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = ERROL_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Hayden.
					if (conversation_name.find("Conversations/Bank/Hayden") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = HAYDEN_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Hemlock.
					if (conversation_name.find("Conversations/Bank/Hemlock") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = HEMLOCK_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Henrietta.
					if (conversation_name.find("Conversations/Bank/Henrietta") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = HENRIETTA_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Holt.
					if (conversation_name.find("Conversations/Bank/Holt") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = HOLT_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Josephine.
					if (conversation_name.find("Conversations/Bank/Josephine") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = JOSEPHINE_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Juniper.
					if (conversation_name.find("Conversations/Bank/Juniper") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = JUNIPER_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Landen.
					if (conversation_name.find("Conversations/Bank/Landen") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = LANDEN_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Luc.
					if (conversation_name.find("Conversations/Bank/Luc") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = LUC_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Louis.
					if (conversation_name.find("Conversations/Bank/Louis") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = LOUIS_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Maple.
					if (conversation_name.find("Conversations/Bank/Maple") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = MAPLE_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is March.
					if (conversation_name.find("Conversations/Bank/March") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = MARCH_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Merri.
					if (conversation_name.find("Conversations/Bank/Merri") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = MERRI_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Olric.
					if (conversation_name.find("Conversations/Bank/Olric") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = OLRIC_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Nora.
					if (conversation_name.find("Conversations/Bank/Nora") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = NORA_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Reina.
					if (conversation_name.find("Conversations/Bank/Reina") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = REINA_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Ryis.
					if (conversation_name.find("Conversations/Bank/Ryis") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = RYIS_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Terithia.
					if (conversation_name.find("Conversations/Bank/Terithia") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = TERITHIA_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Valen.
					if (conversation_name.find("Conversations/Bank/Valen") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = VALEN_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Vera.
					if (conversation_name.find("Conversations/Bank/Vera") != std::string::npos)
					{
						// Override the gift dialog.
						custom_dialogue_value = VERA_GIFT_RECEIVED_DIALOGUE_KEY;
						RValue* custom_dialog_ptr = &custom_dialogue_value;
						Arguments[0] = custom_dialog_ptr;
					}

					// Increase heart points.
					AddHeartPoints(secret_santa_recipient);
				}
			}
		}
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
	ResetStaticFields(true);

	if (load_on_start)
	{
		CreateOrGetGlobalYYTKVariable();
		CreateModInfoInGlobalYYTKVariable();

		std::exception_ptr eptr;
		try
		{
			// Try to find the mod_data directory.
			std::string current_dir = std::filesystem::current_path().string();
			std::string mod_data_folder = current_dir + "\\mod_data";
			if (!std::filesystem::exists(mod_data_folder))
			{
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[SecretSanta %s] - The \"mod_data\" directory was not found. Creating directory: %s", VERSION, mod_data_folder.c_str());
				std::filesystem::create_directory(mod_data_folder);
			}

			// Try to find the mod_data\SecretSanta directory.
			std::string secret_santa_folder = mod_data_folder + "\\SecretSanta";
			if (!std::filesystem::exists(secret_santa_folder))
			{
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[SecretSanta %s] - The \"SecretSanta\" directory was not found. Creating directory: %s", VERSION, secret_santa_folder.c_str());
				std::filesystem::create_directory(secret_santa_folder);
			}

			mod_folder = secret_santa_folder;
		}
		catch (...)
		{
			g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - An error occurred when locating the mod directory.", VERSION);

			eptr = std::current_exception();
			PrintException(eptr);
			mod_healthy = false;
		}

		int item_id = GetItemId(Self, Other, "stinky_stamina_potion");
		if (item_id != UNSET_INT)
		{
			stinky_stamina_potion_id = item_id;
		}
		else
		{
			g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to look up item ID for: stinky_stamina_potion.", VERSION);
			mod_healthy = false;
		}

		if (!mod_healthy)
		{
			g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - The mod has an unhealthy configuration and will NOT function!", VERSION);
		}

		load_on_start = false;
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

RValue& GmlScriptShowRoomTitleCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_healthy)
	{
		// Formatted date strings.
		std::string current_year_winter_20_date_string = FormatDateString(20, 4, year);
		std::string current_year_winter_26_date_string = FormatDateString(26, 4, year);
		std::string current_year_winter_27_date_string = FormatDateString(27, 4, year);
		std::string current_year_object_key = "year-" + std::to_string(year);

		// Check if the JSON object doesn't have the current year.
		if (!json_object.contains(current_year_object_key))
		{
			// Randomly determine the secret santa sender and recipient.
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> distr(0, static_cast<int>(ACTIVE_NPC_NAMES->size() - 1));
			int random_sender = distr(gen);
			int random_recipient = distr(gen);

			// Create the current year object.
			json current_year_object = json::object();
			current_year_object[SENDER_KEY] = ACTIVE_NPC_NAMES[random_sender];
			current_year_object[RECIPIENT_KEY] = ACTIVE_NPC_NAMES[random_recipient];

			// Create the date objects.
			json current_year_winter_20_object = json::object();
			current_year_winter_20_object[MAIL_SENT_KEY] = false;

			json current_year_winter_26_object = json::object();
			current_year_winter_26_object[MAIL_SENT_KEY] = false;

			json current_year_winter_27_object = json::object();
			current_year_winter_27_object[MAIL_SENT_KEY] = false;

			// Add the date objects to the current year object.
			current_year_object[current_year_winter_20_date_string] = current_year_winter_20_object;
			current_year_object[current_year_winter_26_date_string] = current_year_winter_26_object;
			current_year_object[current_year_winter_27_date_string] = current_year_winter_27_object;

			// Update the JSON object.
			json_object[current_year_object_key] = current_year_object;
		}

		// Set the static sender and recipient fields using the JSON data.
		secret_santa_sender = json_object[current_year_object_key][SENDER_KEY];
		secret_santa_recipient = json_object[current_year_object_key][RECIPIENT_KEY];

		if (season == 4) // winter
		{
			if (!MailExists("secret_santa_first_year"))
				SendMail("secret_santa_first_year");

			if (day == 20)
			{
				bool mail_sent = json_object[current_year_object_key][current_year_winter_20_date_string][MAIL_SENT_KEY];
				if (!mail_sent)
				{
					std::string mail_name_str = "secret_santa_notice_" + secret_santa_recipient;
					SendMail(mail_name_str);

					json_object[current_year_object_key][current_year_winter_20_date_string][MAIL_SENT_KEY] = true;
				}
			}

			if (day == 26)
			{
				bool mail_sent = json_object[current_year_object_key][current_year_winter_26_date_string][MAIL_SENT_KEY];
				if (!mail_sent)
				{
					std::string mail_name_str = "secret_santa_reminder_" + secret_santa_recipient;
					SendMail(mail_name_str);

					json_object[current_year_object_key][current_year_winter_26_date_string][MAIL_SENT_KEY] = true;
				}
			}

			if (day == 27)
			{
				bool mail_sent = json_object[current_year_object_key][current_year_winter_27_date_string][MAIL_SENT_KEY];
				if (!mail_sent)
				{
					// Randomly select a gift.
					std::random_device rd;
					std::mt19937 gen(rd());
					std::uniform_int_distribution<> distr(0, static_cast<int>(GIFTS->size() - 1));
					int random_gift = distr(gen);

					auto random_gift_str = GIFTS[random_gift];

					std::string mail_name_str = "secret_santa_" + secret_santa_sender + "_" + random_gift_str;
					SendMail(mail_name_str);

					json_object[current_year_object_key][current_year_winter_27_date_string][MAIL_SENT_KEY] = true;
				}
			}
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_show_room_title"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptLoadGameCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_healthy)
	{
		// Get the save file name.
		std::string save_file = std::string(Arguments[0]->m_Object->at("save_path").AsString().data());
		std::size_t save_file_name_delimiter_index = save_file.find_last_of("/");
		std::string save_name = save_file.substr(save_file_name_delimiter_index + 1);

		// Get the save prefix.
		std::size_t first_hyphen_index = save_name.find_first_of("-") + 1;
		std::size_t second_hyphen_index = save_name.find_last_of("-");
		save_prefix = save_name.substr(first_hyphen_index, (second_hyphen_index - first_hyphen_index));

		// Read from the custom mod data file.
		ReadModFile();
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_load_game"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptSaveGameCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_healthy)
	{
		// No save prefix has been detected. This should only happen when a new game is started.
		if (save_prefix.size() == 0)
		{
			// Get the save file name.
			std::string save_file = Arguments[0]->AsString().data();
			std::size_t save_file_name_delimiter_index = save_file.find_last_of("/");
			std::string save_name = save_file.substr(save_file_name_delimiter_index + 1);

			// Check it's a valid value.
			if (save_name.find("undefined") == std::string::npos)
			{
				// Get the save prefix.
				std::size_t first_hyphen_index = save_name.find_first_of("-") + 1;
				std::size_t second_hyphen_index = save_name.find_last_of("-");
				save_prefix = save_name.substr(first_hyphen_index, (second_hyphen_index - first_hyphen_index));
			}
		}

		SaveModFile();
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_save_game"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptChooseRandomArtifactCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_choose_random_artifact@Archaeology@Archaeology"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (mod_healthy)
	{
		if (!IgnoreNextDigSpot())
		{
			if (season == 4) // winter
			{
				// Randomly roll 1/20 chance to get a Magical Snowflake item.
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> distr(1, 10);
				int random_int = distr(gen);

				if (random_int == 7)
				{
					g_ModuleInterface->Print(CM_LIGHTGREEN, "[SecretSanta %s] - You found a Magical Snowflake!", VERSION);
					Result.m_i64 = stinky_stamina_potion_id;
				}
			}
		}
		else
		{
			RValue SecretSanta = StructVariableGet(__YYTK, "SecretSanta");
			RValue ignore_next_dig_spot = false;
			StructVariableSet(SecretSanta, "ignore_next_dig_spot", ignore_next_dig_spot);
		}
	}

	return Result;
}

void CreateHookGmlScriptCalendarDay(AurieStatus& status)
{
	CScript* gml_script_calendar_day = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_day@Calendar@Calendar",
		(PVOID*)&gml_script_calendar_day
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to get script (gml_Script_day@Calendar@Calendar)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_day@Calendar@Calendar",
		gml_script_calendar_day->m_Functions->m_ScriptFunction,
		GmlScriptCalendarDayCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to hook script (gml_Script_day@Calendar@Calendar)!", VERSION);
	}
}

void CreateHookGmlScriptCalendarSeason(AurieStatus& status)
{
	CScript* gml_script_calendar_season = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_season@Calendar@Calendar",
		(PVOID*)&gml_script_calendar_season
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to get script (gml_Script_season@Calendar@Calendar)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_season@Calendar@Calendar",
		gml_script_calendar_season->m_Functions->m_ScriptFunction,
		GmlScriptCalendarSeasonCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to hook script (gml_Script_season@Calendar@Calendar)!", VERSION);
	}
}

void CreateHookGmlScriptCalendarYear(AurieStatus& status)
{
	CScript* gml_script_calendar_year = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_year@Calendar@Calendar",
		(PVOID*)&gml_script_calendar_year
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to get script (gml_Script_year@Calendar@Calendar)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_year@Calendar@Calendar",
		gml_script_calendar_year->m_Functions->m_ScriptFunction,
		GmlScriptCalendarYearCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to hook script (gml_Script_year@Calendar@Calendar)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to get script (gml_Script_play_conversation)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to hook script (gml_Script_play_conversation)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to get script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to hook script (gml_Script_setup_main_screen@TitleMenu@TitleMenu)!", VERSION);
	}
}

void CreateHookGmlScriptShowRoomTitle(AurieStatus& status)
{
	CScript* gml_Script_on_new_day = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_show_room_title",
		(PVOID*)&gml_Script_on_new_day
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to get script (gml_Script_show_room_title)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_show_room_title",
		gml_Script_on_new_day->m_Functions->m_ScriptFunction,
		GmlScriptShowRoomTitleCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to hook script (gml_Script_show_room_title)!", VERSION);
	}
}

void CreateHookGmlScriptLoadGame(AurieStatus& status)
{
	CScript* gml_script_load_game = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_load_game",
		(PVOID*)&gml_script_load_game
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to get script (gml_Script_load_game)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_load_game",
		gml_script_load_game->m_Functions->m_ScriptFunction,
		GmlScriptLoadGameCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to hook script (gml_Script_load_game)!", VERSION);
	}
}

void CreateHookGmlScriptSaveGame(AurieStatus& status)
{
	CScript* gml_script_save_game = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_save_game",
		(PVOID*)&gml_script_save_game
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to get script (gml_Script_save_game)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_save_game",
		gml_script_save_game->m_Functions->m_ScriptFunction,
		GmlScriptSaveGameCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to hook script (gml_Script_save_game)!", VERSION);
	}
}

void CreateHookGmlScriptChooseRandomArtifact(AurieStatus& status)
{
	CScript* gml_script_choose_random_artifact = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_choose_random_artifact@Archaeology@Archaeology",
		(PVOID*)&gml_script_choose_random_artifact
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to get script (gml_Script_choose_random_artifact@Archaeology@Archaeology)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_choose_random_artifact@Archaeology@Archaeology",
		gml_script_choose_random_artifact->m_Functions->m_ScriptFunction,
		GmlScriptChooseRandomArtifactCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to hook script (gml_Script_choose_random_artifact@Archaeology@Archaeology)!", VERSION);
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

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[SecretSanta %s] - Plugin starting...", VERSION);

	CreateHookGmlScriptCalendarDay(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptCalendarSeason(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptCalendarYear(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptPlayConversation(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptSetupMainScreen(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptShowRoomTitle(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptLoadGame(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptSaveGame(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptChooseRandomArtifact(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[SecretSanta %s] - Plugin started!", VERSION);
	return AURIE_SUCCESS;
}
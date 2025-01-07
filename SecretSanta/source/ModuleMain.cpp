#include <random>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <shlobj.h>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

static const char* const VERSION = "1.0.0";
static const double MAX_HEART_POINTS = 705; // 6 hearts is the current max friendship as of v0.12. See "misc/npc_heart_point_table" in fiddle file for heart points per level.
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
static const std::string GIFT_GIVEN_KEY = "gift_given";

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_on_start = true;
static bool mod_healthy = false;
static int day = UNSET_DOUBLE;
static int season = UNSET_DOUBLE;
static int year = UNSET_DOUBLE;
static std::string save_file = "";
static std::string config_file = "";
static std::string secret_santa_sender = "";
static std::string secret_santa_recipient = "";
static json json_object = json::object();

void handle_eptr(std::exception_ptr eptr)
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

std::string wstr_to_string(std::wstring wstr)
{
	std::vector<char> buf(wstr.size());
	std::use_facet<std::ctype<wchar_t>>(std::locale{}).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());

	return std::string(buf.data(), buf.size());
}

std::string GetSaveHash()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	RValue __ari = global_instance->at("__ari").m_Object;

	std::string name = __ari.at("name").AsString().data();
	std::string farm_name = __ari.at("farm_name").AsString().data();
	std::string birthday = std::to_string(static_cast<int>(__ari.at("birthday").m_Real));

	std::string concat = name + farm_name + birthday;
	size_t hash = std::hash<std::string>{}(concat);
	return std::to_string(hash);
}

std::string FormatDateString(int day, int season, int year)
{
	return std::to_string(day) + "-" + std::to_string(season) + "-" + std::to_string(year);
}

void CreateDefaultJson()
{
	json default_json = {};
	json_object = default_json;
}

void CreateDefaultConfigFile()
{
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[SecretSanta %s] - The \"SecretSanta.json\" file was not found. Creating file: %s", VERSION, config_file.c_str());
	CreateDefaultJson();

	std::ofstream out_stream(config_file);
	out_stream << std::setw(4) << json_object << std::endl;
	out_stream.close();
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
	
	return -1;
}

void AddHeartPoints(std::string npc_name)
{
	int npc_id = GetNpcId(npc_name);
	if (npc_id != -1)
	{
		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		RValue __npc_database = global_instance->at("__npc_database");
		RValue npc_database_0 = __npc_database[npc_id];
		RValue original_heart_points = npc_database_0.at("heart_points");

		RValue modified_heart_points = original_heart_points.m_Real + 20.0;
		if (modified_heart_points.m_Real > MAX_HEART_POINTS)
			modified_heart_points.m_Real = MAX_HEART_POINTS;

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
			const char* conversation_name = Arguments[0]->AsString().data(); 
			std::string conversation_name_lowercase = conversation_name;
			std::transform(conversation_name_lowercase.begin(), conversation_name_lowercase.end(), conversation_name_lowercase.begin(), [](unsigned char c) { return std::tolower(c); });

			// Dialog involves the secret santa recipient.
			if (conversation_name_lowercase.find(secret_santa_recipient) != std::string::npos)
			{
				bool is_secret_santa_gift = false;

				// Dialog is a gift line.
				if (strstr(conversation_name, "gift_lines"))
				{
					is_secret_santa_gift = true;
				}

				if (is_secret_santa_gift)
				{
					// Speaker is Adeline.
					if (strstr(conversation_name, "Conversations/Bank/Adeline"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Adeline/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Balor.
					if (strstr(conversation_name, "Conversations/Bank/Balor"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Balor/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Celine.
					if (strstr(conversation_name, "Conversations/Bank/Celine"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Celine/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Darcy.
					if (strstr(conversation_name, "Conversations/Bank/Darcy"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Darcy/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Dell.
					if (strstr(conversation_name, "Conversations/Bank/Dell"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Dell/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Dozy.
					if (strstr(conversation_name, "Conversations/Bank/Dozy"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Dozy/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Eiland.
					if (strstr(conversation_name, "Conversations/Bank/Eiland"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Eiland/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Elsie.
					if (strstr(conversation_name, "Conversations/Bank/Elsie"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Elsie/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Errol.
					if (strstr(conversation_name, "Conversations/Bank/Errol"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Errol/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Hayden.
					if (strstr(conversation_name, "Conversations/Bank/Hayden"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Hayden/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Hemlock.
					if (strstr(conversation_name, "Conversations/Bank/Hemlock"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Hemlock/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Henrietta.
					if (strstr(conversation_name, "Conversations/Bank/Henrietta"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Henrietta/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Holt.
					if (strstr(conversation_name, "Conversations/Bank/Holt"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Holt/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Josephine.
					if (strstr(conversation_name, "Conversations/Bank/Josephine"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Josephine/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Juniper.
					if (strstr(conversation_name, "Conversations/Bank/Juniper"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Juniper/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Landen.
					if (strstr(conversation_name, "Conversations/Bank/Landen"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Landen/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Luc.
					if (strstr(conversation_name, "Conversations/Bank/Luc"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Luc/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Louis.
					if (strstr(conversation_name, "Conversations/Bank/Louis"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Louis/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Maple.
					if (strstr(conversation_name, "Conversations/Bank/Maple"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Maple/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is March.
					if (strstr(conversation_name, "Conversations/Bank/March"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/March/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Merri.
					if (strstr(conversation_name, "Conversations/Bank/Merri"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Merri/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Olric.
					if (strstr(conversation_name, "Conversations/Bank/Olric"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Olric/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Nora.
					if (strstr(conversation_name, "Conversations/Bank/Nora"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Nora/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Reina.
					if (strstr(conversation_name, "Conversations/Bank/Reina"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Reina/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Ryis.
					if (strstr(conversation_name, "Conversations/Bank/Ryis"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Ryis/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Terithia.
					if (strstr(conversation_name, "Conversations/Bank/Terithia"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Terithia/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Valen.
					if (strstr(conversation_name, "Conversations/Bank/Valen"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Valen/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
						Arguments[0] = custom_dialog_ptr;
					}

					// Speaker is Vera.
					if (strstr(conversation_name, "Conversations/Bank/Vera"))
					{
						// Override the gift dialog.
						RValue custom_dialog = "Conversations/Bank/Vera/Secret Santa/init";
						RValue* custom_dialog_ptr = &custom_dialog;
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
	day = UNSET_DOUBLE;
	season = UNSET_DOUBLE;
	year = UNSET_DOUBLE;
	save_file = "";
	secret_santa_sender = "";
	secret_santa_recipient = "";

	if (load_on_start)
	{
		std::exception_ptr eptr;
		try
		{
			// Try to find the AppData\Local directory.
			wchar_t* localAppDataFolder;
			if (SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_CREATE, NULL, &localAppDataFolder) == S_OK)
			{
				// Try to find the AppData\Local\FieldsOfMistria directory.
				std::string fields_of_mistria_folder = wstr_to_string(localAppDataFolder) + "\\FieldsOfMistria";
				if (std::filesystem::exists(fields_of_mistria_folder))
				{
					// Try to find the AppData\Local\FieldsOfMistria\mod_data directory.
					std::string mod_data_folder = fields_of_mistria_folder + "\\mod_data";
					if (!std::filesystem::exists(mod_data_folder))
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[SecretSanta %s] - The \"mod_data\" directory was not found. Creating directory: %s", VERSION, mod_data_folder.c_str());
						std::filesystem::create_directory(mod_data_folder);
					}

					// Try to find the AppData\Local\FieldsOfMistria\mod_data\SecretSanta directory.
					std::string secret_santa_folder = mod_data_folder + "\\SecretSanta";
					if (!std::filesystem::exists(secret_santa_folder))
					{
						g_ModuleInterface->Print(CM_LIGHTYELLOW, "[SecretSanta %s] - The \"SecretSanta\" directory was not found. Creating directory: %s", VERSION, secret_santa_folder.c_str());
						std::filesystem::create_directory(secret_santa_folder);
					}

					// Try to find the AppData\Local\FieldsOfMistria\mod_data\SecretSanta\SecretSanta.json config file.
					config_file = secret_santa_folder + "\\" + "SecretSanta.json";
					std::ifstream in_stream(config_file);
					if (in_stream.good())
					{
						try
						{
							json_object = json::parse(in_stream);
							mod_healthy = true;
							g_ModuleInterface->Print(CM_LIGHTGREEN, "[SecretSanta %s] - Loaded configuration file: %s", VERSION, config_file.c_str());
						}
						catch (...)
						{
							eptr = std::current_exception();
							handle_eptr(eptr);

							g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to parse JSON from configuration file: %s", VERSION, config_file.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[SecretSanta %s] - Make sure the file is valid JSON!", VERSION);
						}

						in_stream.close();
					}
					else
					{
						in_stream.close();

						// Create the new config file.
						CreateDefaultConfigFile();
						mod_healthy = true;
					}
				}
				else
				{
					g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to find the \"%s\" directory.", VERSION, "AppData\\Local\\FieldsOfMistria");
				}
			}
			else
			{
				g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to find the \"%s\" directory.", VERSION, "AppData\\Local");
			}
		}
		catch (...)
		{
			g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - An error occurred loading the mod configuration file.", VERSION);
			
			eptr = std::current_exception();
			handle_eptr(eptr);
		}

		load_on_start = false;

		if (!mod_healthy)
		{
			g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - The mod has an unhealthy configuration and will NOT function!", VERSION);
		}
	}
	else
	{
		if (mod_healthy)
		{
			// Reload the config file.
			std::ifstream in_stream(config_file);
			json_object = json::parse(in_stream);
		}
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

RValue& GmlScriptOnNewDayCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_healthy)
	{
		// The hash value used to group different saves for the same playthrough.
		std::string save_hash = GetSaveHash();

		// Formatted date strings.
		std::string current_year_winter_20_date_string = FormatDateString(20, 4, year);
		std::string current_year_winter_26_date_string = FormatDateString(26, 4, year);
		std::string current_year_winter_27_date_string = FormatDateString(27, 4, year);
		std::string current_year_object_key = "year-" + std::to_string(year);

		// Check if an entry for the save hash doesn't exist in the JSON.
		if (!json_object.contains(save_hash))
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
			current_year_winter_27_object[GIFT_GIVEN_KEY] = false;

			// Create the save file object.
			json save_file_object = json::object();
			save_file_object[current_year_winter_20_date_string] = current_year_winter_20_object;
			save_file_object[current_year_winter_26_date_string] = current_year_winter_26_object;
			save_file_object[current_year_winter_27_date_string] = current_year_winter_27_object;

			// Create the save hash object.
			json save_hash_object = json::object();
			save_hash_object[current_year_object_key] = current_year_object;
			save_hash_object[save_file] = save_file_object;

			// Write values back.
			json_object[save_hash] = save_hash_object;
		}

		json save_hash_object = json_object[save_hash];

		// Check if an entry for the current year didn't exist in the save hash object.
		if (!save_hash_object.contains(current_year_object_key))
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

			// Write the values back.
			save_hash_object[current_year_object_key] = current_year_object;
			json_object[save_hash] = save_hash_object;
		}

		// Check if an entry for the save file didn't exist in the save hash object.
		if (!save_hash_object.contains(save_file))
		{
			// Create the date objects.
			json current_year_winter_20_object = json::object();
			current_year_winter_20_object[MAIL_SENT_KEY] = false;

			json current_year_winter_26_object = json::object();
			current_year_winter_26_object[MAIL_SENT_KEY] = false;

			json current_year_winter_27_object = json::object();
			current_year_winter_27_object[MAIL_SENT_KEY] = false;
			current_year_winter_27_object[GIFT_GIVEN_KEY] = false;

			// Create the save file object.
			json save_file_object = json::object();
			save_file_object[current_year_winter_20_date_string] = current_year_winter_20_object;
			save_file_object[current_year_winter_26_date_string] = current_year_winter_26_object;
			save_file_object[current_year_winter_27_date_string] = current_year_winter_27_object;

			// Write values back.
			save_hash_object[save_file] = save_file_object;
			json_object[save_hash] = save_hash_object;
		}

		json save_file_object = save_hash_object[save_file];

		// Check if Winter 20 data for the current save file doesn't exist.
		if (!save_file_object.contains(current_year_winter_20_date_string))
		{
			// Create the date object.
			json current_year_winter_20_object = json::object();
			current_year_winter_20_object[MAIL_SENT_KEY] = false;

			// Write values back.
			save_file_object[current_year_winter_20_date_string] = current_year_winter_20_object;
			save_hash_object[save_file] = save_file_object;
			json_object[save_hash] = save_hash_object;
		}

		// Check if Winter 26 data for the current save file doesn't exist.
		if (!save_file_object.contains(current_year_winter_26_date_string))
		{
			// Create the date object.
			json current_year_winter_26_object = json::object();
			current_year_winter_26_object[MAIL_SENT_KEY] = false;

			// Write values back.
			save_file_object[current_year_winter_26_date_string] = current_year_winter_26_object;
			save_hash_object[save_file] = save_file_object;
			json_object[save_hash] = save_hash_object;
		}

		// Check if Winter 27 data for the current save file doesn't exist.
		if (!save_file_object.contains(current_year_winter_27_date_string))
		{
			// Create the date object.
			json current_year_winter_27_object = json::object();
			current_year_winter_27_object[MAIL_SENT_KEY] = false;
			current_year_winter_27_object[GIFT_GIVEN_KEY] = false;

			// Write values back.
			save_file_object[current_year_winter_27_date_string] = current_year_winter_27_object;
			save_hash_object[save_file] = save_file_object;
			json_object[save_hash] = save_hash_object;
		}

		secret_santa_sender = json_object[save_hash][current_year_object_key][SENDER_KEY];
		secret_santa_recipient = json_object[save_hash][current_year_object_key][RECIPIENT_KEY];

		if (season == 4) // winter
		{
			if (day == 20)
			{
				bool mail_sent = json_object[save_hash][save_file][current_year_winter_20_date_string][MAIL_SENT_KEY];
				if (!mail_sent)
				{
					std::string mail_name_str = "secret_santa_notice_" + secret_santa_recipient;
					SendMail(mail_name_str);

					json_object[save_hash][save_file][current_year_winter_20_date_string][MAIL_SENT_KEY] = true;
				}
			}

			if (day == 26)
			{
				bool mail_sent = json_object[save_hash][save_file][current_year_winter_26_date_string][MAIL_SENT_KEY];
				if (!mail_sent)
				{
					std::string mail_name_str = "secret_santa_reminder_" + secret_santa_recipient;
					SendMail(mail_name_str);

					json_object[save_hash][save_file][current_year_winter_26_date_string][MAIL_SENT_KEY] = true;
				}
			}

			if (day == 27)
			{
				bool mail_sent = json_object[save_hash][save_file][current_year_winter_27_date_string][MAIL_SENT_KEY];
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

					json_object[save_hash][save_file][current_year_winter_27_date_string][MAIL_SENT_KEY] = true;
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
		// Get the file name of the save.
		std::string arg0_str = std::string(Arguments[0]->m_Object->at("save_path").AsString().data());
		std::size_t index = arg0_str.find_last_of("/");
		save_file = arg0_str.substr(index + 1);
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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_save_game"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (mod_healthy)
	{
		// Update the config file.
		std::ofstream out_stream(config_file);
		out_stream << std::setw(4) << json_object << std::endl;
		out_stream.close();
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

void CreateHookGmlScriptOnNewDay(AurieStatus& status)
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
		GmlScriptOnNewDayCallback,
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

	CreateHookGmlScriptOnNewDay(status);
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
	
	g_ModuleInterface->Print(CM_LIGHTGREEN, "[SecretSanta %s] - Plugin started!", VERSION);
	return AURIE_SUCCESS;
}
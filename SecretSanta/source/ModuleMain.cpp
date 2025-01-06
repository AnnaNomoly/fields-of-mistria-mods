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
static const std::string ADELINE = "adeline";
static const std::string BALOR = "balor";
static const std::string CELINE = "celine";
static const std::string DARCY = "darcy";
static const std::string DELL = "dell";
static const std::string DOZY = "dozy";
static const std::string EILAND = "eiland";
static const std::string ELSIE = "elsie";
static const std::string ERROL = "errol";
static const std::string HAYDEN = "hayden";
static const std::string HEMLOCK = "hemlock";
static const std::string HENRIETTA = "henrietta";
static const std::string HOLT = "holt";
static const std::string JOSEPHINE = "josephine";
static const std::string JUNIPER = "juniper";
static const std::string LANDEN = "landen";
static const std::string LUC = "luc";
static const std::string LOUIS = "louis";
static const std::string MAPLE = "maple";
static const std::string MARCH = "march";
static const std::string MERRI = "merri";
static const std::string OLRIC = "olric";
static const std::string NORA = "nora";
static const std::string REINA = "reina";
static const std::string RYIS = "ryis";
static const std::string TERITHIA = "terithia";
static const std::string VALEN = "valen";
static const std::string VERA = "vera";
static const double UNSET_DOUBLE = -1.0;
static const std::string NPC_NAMES[] = { 
	ADELINE, BALOR, CELINE, DARCY, DELL, DOZY, EILAND,
	ELSIE, ERROL, HAYDEN, HEMLOCK, HENRIETTA, HOLT, JOSEPHINE,
	JUNIPER, LANDEN, LUC, LOUIS, MAPLE, MARCH, MERRI,
	OLRIC, NORA, REINA, RYIS, TERITHIA, VALEN, VERA
};

// JSON key names
static const std::string SENDER_KEY = "sender";
static const std::string RECIPIENT_KEY = "recipient";
static const std::string MAIL_SENT_KEY = "mail_sent";
static const std::string GIFT_GIVEN_KEY = "gift_given";
static const std::string ADELINE_GIFTS_KEY = "adeline_gifts";

// JSON default values
static const std::vector<std::string> DEFAULT_ADELINE_GIFTS = {"berry_bowl", "beet_soup", "fried_rice", "vegetable_pot_pie", "floral_tea", "tulip_cake", "sushi_platter", "lobster_roll", "summer_salad", "vegetable_quiche"};

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
static std::vector<std::string> adeline_gifts = DEFAULT_ADELINE_GIFTS;
static json json_object = json::object();

// DEBUG--------------------------------------------------------------------
bool EnumFunction(
	IN const char* MemberName,
	IN OUT RValue* Value
)
{
	g_ModuleInterface->Print(CM_LIGHTYELLOW, "Member Name: %s", MemberName);
	return false;
}
//--------------------------------------------------------------------------

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
	json default_json = {
		{ADELINE_GIFTS_KEY, DEFAULT_ADELINE_GIFTS}
	};
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
	RValue mail_name = mail_name_str; //"ari_birthday_celine";
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
		if (GetAsyncKeyState(VK_F1) & 1)
		{
			CInstance* global_instance = nullptr;
			g_ModuleInterface->GetGlobalInstance(&global_instance);

			RValue __ari = global_instance->at("__ari").m_Object;
			RValue inbox = __ari.at("inbox");
			RValue contents = inbox.at("contents");
			RValue contents_buffer = contents.at("__buffer");

			RValue mail;
			RValue mail_items_taken = false;
			RValue mail_name = "ari_birthday_celine";
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

	// Result is a VALUE_REAL that is the 0-indexed calendar day
	day = Result.m_Real + 1;
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

	// Result is a VALUE_REAL that is the 0-indexed calendar season
	season = Result.m_Real + 1;
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

	// Result is a VALUE_REAL that is the 0-indexed calendar year
	year = Result.m_Real + 1;
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
	// "Conversations/Bank/Dell/Gift Lines/gift_lines/neutral_gift"
	const char* conversation_name = Arguments[1]->AsString().data(); // debugging - possibly the name of the conversation
	if (strstr(conversation_name, "Conversations/Bank/Celine"))
	{
		if (strstr(conversation_name, "gift_lines/neutral_gift"))
		{
			g_ModuleInterface->Print(CM_LIGHTPURPLE, "[Debug] - Detected netural gift for Celine.");



		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_play_conversation"));
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

							// Check if the json_object is empty.
							if (json_object.empty())
							{
								g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - No values found in mod configuration file: %s!", VERSION, config_file.c_str());
								g_ModuleInterface->Print(CM_LIGHTYELLOW, "[SecretSanta %s] - Using default values.", VERSION);
								CreateDefaultJson();
							}
							else
							{
								// Load Adeline gifts.
								if (json_object.contains(ADELINE_GIFTS_KEY) && json_object[ADELINE_GIFTS_KEY].is_array())
								{
									adeline_gifts = json_object[ADELINE_GIFTS_KEY];
									if (adeline_gifts.size() == 0)
									{
										json_object[ADELINE_GIFTS_KEY] = DEFAULT_ADELINE_GIFTS;
										adeline_gifts = DEFAULT_ADELINE_GIFTS;
									}
								}
								else
								{
									json_object[ADELINE_GIFTS_KEY] = DEFAULT_ADELINE_GIFTS;
									adeline_gifts = DEFAULT_ADELINE_GIFTS;
								}
								
								mod_healthy = true;
							}
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
			g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - The mod has an unhealthy configuration and will not function!", VERSION);
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_send@Inbox@Inbox"));
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
	// The hash value used to group different saves for the same playthrough.
	std::string save_hash = GetSaveHash();

	// Check if an entry for the save hash doesn't exist in the JSON.
	if (!json_object.contains(save_hash))
	{
		// Randomly determine the secret santa sender and recipient.
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(0, static_cast<int>(NPC_NAMES->size()));
		int random_sender = distr(gen);
		int random_recipient = distr(gen);

		secret_santa_sender = NPC_NAMES->at(random_sender);
		secret_santa_recipient = NPC_NAMES->at(random_recipient);

		// Create the date objects.
		std::string current_year_winter_21_date_string = FormatDateString(21, 4, year);
		json current_year_winter_21_object = json::object();
		current_year_winter_21_object[SENDER_KEY] = secret_santa_sender;
		current_year_winter_21_object[RECIPIENT_KEY] = secret_santa_recipient;
		current_year_winter_21_object[MAIL_SENT_KEY] = false;

		std::string current_year_winter_27_date_string = FormatDateString(27, 4, year);
		json current_year_winter_27_object = json::object();
		current_year_winter_27_object[MAIL_SENT_KEY] = false;

		std::string current_year_winter_28_date_string = FormatDateString(28, 4, year);
		json current_year_winter_28_object = json::object();
		current_year_winter_28_object[MAIL_SENT_KEY] = false;
		current_year_winter_28_object[GIFT_GIVEN_KEY] = false;

		// Create the save file object.
		json save_file_object = json::object();
		save_file_object[current_year_winter_21_date_string] = current_year_winter_21_object;
		save_file_object[current_year_winter_27_date_string] = current_year_winter_27_object;
		save_file_object[current_year_winter_28_date_string] = current_year_winter_28_object;

		// Create the save hash object.
		json save_hash_object = json::object();
		save_hash_object[save_file] = save_file_object;

		// Write values back.
		json_object[save_hash] = save_hash_object;
	}

	json save_hash_object = json_object[save_hash];
	std::string current_year_winter_21_date_string = FormatDateString(21, 4, year);
	std::string current_year_winter_27_date_string = FormatDateString(27, 4, year);
	std::string current_year_winter_28_date_string = FormatDateString(28, 4, year);

	// Check if an entry for the save file didn't exist in the save hash object.
	if (!save_hash_object.contains(save_file))
	{
		// Randomly determine the secret santa sender and recipient.
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(0, static_cast<int>(NPC_NAMES->size()));
		int random_sender = distr(gen);
		int random_recipient = distr(gen);

		secret_santa_sender = NPC_NAMES->at(random_sender);
		secret_santa_recipient = NPC_NAMES->at(random_recipient);

		// Create the date objects.
		json current_year_winter_21_object = json::object();
		current_year_winter_21_object[SENDER_KEY] = secret_santa_sender;
		current_year_winter_21_object[RECIPIENT_KEY] = secret_santa_recipient;
		current_year_winter_21_object[MAIL_SENT_KEY] = false;

		json current_year_winter_27_object = json::object();
		current_year_winter_27_object[MAIL_SENT_KEY] = false;

		json current_year_winter_28_object = json::object();
		current_year_winter_28_object[MAIL_SENT_KEY] = false;
		current_year_winter_28_object[GIFT_GIVEN_KEY] = false;

		// Create the save file object.
		json save_file_object = json::object();
		save_file_object[current_year_winter_21_date_string] = current_year_winter_21_object;
		save_file_object[current_year_winter_27_date_string] = current_year_winter_27_object;
		save_file_object[current_year_winter_28_date_string] = current_year_winter_28_object;

		// Write values back.
		save_hash_object[save_file] = save_file_object;
		json_object[save_hash] = save_hash_object;
	}

	json save_file_object = save_hash_object[save_file]; // TODO: Write back

	// Check if Winter 21 data for the current save file doesn't exist.
	if (!save_file_object.contains(current_year_winter_21_date_string))
	{
		// Randomly determine the secret santa sender and recipient.
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(0, static_cast<int>(NPC_NAMES->size()));
		int random_sender = distr(gen);
		int random_recipient = distr(gen);

		secret_santa_sender = NPC_NAMES->at(random_sender);
		secret_santa_recipient = NPC_NAMES->at(random_recipient);

		// Create the date object.
		json current_year_winter_21_object = json::object();
		current_year_winter_21_object[SENDER_KEY] = secret_santa_sender;
		current_year_winter_21_object[RECIPIENT_KEY] = secret_santa_recipient;
		current_year_winter_21_object[MAIL_SENT_KEY] = false;

		// Write values back.
		save_file_object[current_year_winter_21_date_string] = current_year_winter_21_object;
		save_hash_object[save_file] = save_file_object;
		json_object[save_hash] = save_hash_object;
	}

	// Check if Winter 27 data for the current save file doesn't exist.
	if (!save_file_object.contains(current_year_winter_27_date_string))
	{
		// Create the date object.
		json current_year_winter_27_object = json::object();
		current_year_winter_27_object[MAIL_SENT_KEY] = false;

		// Write values back.
		save_file_object[current_year_winter_27_date_string] = current_year_winter_27_object;
		save_hash_object[save_file] = save_file_object;
		json_object[save_hash] = save_hash_object;
	}

	// Check if Winter 28 data for the current save file doesn't exist.
	if (!save_file_object.contains(current_year_winter_28_date_string))
	{
		// Create the date object.
		json current_year_winter_28_object = json::object();
		current_year_winter_28_object[MAIL_SENT_KEY] = false;
		current_year_winter_28_object[GIFT_GIVEN_KEY] = false;

		// Write values back.
		save_file_object[current_year_winter_28_date_string] = current_year_winter_28_object;
		save_hash_object[save_file] = save_file_object;
		json_object[save_hash] = save_hash_object;
	}

	if (season == 4) // winter
	{
		if (day == 21)
		{
			bool mail_sent = json_object[save_hash][save_file][current_year_winter_21_date_string][MAIL_SENT_KEY];
			if (!mail_sent)
			{
				std::string mail_name_str = "secret_santa_notice_" + secret_santa_recipient;
				SendMail(mail_name_str);

				json_object[save_hash][save_file][current_year_winter_21_date_string][MAIL_SENT_KEY] = true;
			}
		}

		if (day == 27)
		{
			bool mail_sent = json_object[save_hash][save_file][current_year_winter_27_date_string][MAIL_SENT_KEY];
			if (!mail_sent)
			{
				std::string mail_name_str = "secret_santa_reminder_" + secret_santa_recipient;
				SendMail(mail_name_str);

				json_object[save_hash][save_file][current_year_winter_27_date_string][MAIL_SENT_KEY] = true;
			}
		}

		if (day == 28)
		{
			bool mail_sent = json_object[save_hash][save_file][current_year_winter_28_date_string][MAIL_SENT_KEY];
			if (!mail_sent)
			{
				std::string mail_name_str = "secret_santa_" + secret_santa_sender + ;
				SendMail(mail_name_str);

				json_object[save_hash][save_file][current_year_winter_28_date_string][MAIL_SENT_KEY] = true;
			}
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_on_new_day@Ari@Ari"));
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
	// Get the file name of the save.
	std::string arg0_str = std::string(Arguments[0]->m_Object->at("save_path").AsString().data());
	std::size_t index = arg0_str.find_last_of("/");
	save_file = arg0_str.substr(index + 1);

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
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to hook EVENT_OBJECT_CALL!", VERSION);
	}
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
		"gml_Script_play_conversation",
		(PVOID*)&gml_script_play_conversation
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to get script (gml_Script_play_conversation)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_play_conversation",
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
		"gml_Script_on_new_day@Ari@Ari",
		(PVOID*)&gml_Script_on_new_day
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to get script (gml_Script_on_new_day@Ari@Ari)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_on_new_day@Ari@Ari",
		gml_Script_on_new_day->m_Functions->m_ScriptFunction,
		GmlScriptOnNewDayCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Failed to hook script (gml_Script_on_new_day@Ari@Ari)!", VERSION);
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

	CreateHookEventObject(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

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

	CreateHookGmlScriptLoadGame(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[SecretSanta %s] - Exiting due to failure on start!", VERSION);
		return status;
	}
	
	g_ModuleInterface->Print(CM_LIGHTGREEN, "[SecretSanta %s] - Plugin started!", VERSION);
	return AURIE_SUCCESS;
}
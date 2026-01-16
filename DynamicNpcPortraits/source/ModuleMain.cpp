#include <map>
#include <fstream>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <YYToolkit/YYTK_Shared.hpp> // YYTK v4
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

static const char* const MOD_NAME = "DynamicNpcPortraits";
static const char* const VERSION = "1.0.1";
static const char* const GML_SCRIPT_IS_DUNGEON_ROOM = "gml_Script_is_dungeon_room";
static const char* const GML_SCRIPT_GO_TO_ROOM = "gml_Script_goto_gm_room";
static const char* const GML_SCRIPT_TRY_LOCATION_ID_TO_STRING = "gml_Script_try_location_id_to_string";
static const char* const GML_SCRIPT_GET_MINUTES = "gml_Script_get_minutes";
static const char* const GML_SCRIPT_CALENDAR_DAY = "gml_Script_day@Calendar@Calendar";
static const char* const GML_SCRIPT_CALENDAR_SEASON = "gml_Script_season@Calendar@Calendar";
static const char* const GML_SCRIPT_CALENDAR_YEAR = "gml_Script_year@Calendar@Calendar";
static const char* const GML_SCRIPT_GET_WEATHER = "gml_Script_get_weather@WeatherManager@Weather";
static const char* const GML_SCRIPT_CUTSCENE_IS_RUNNING = "gml_Script_is_running@Mist@Mist";
static const char* const GML_SCRIPT_VERTIGO_DRAW_WITH_COLOR = "gml_Script_vertigo_draw_with_color";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const int EIGHT_PM_IN_SECONDS = 72000;
static const std::string INDOORS = "indoors";
static const std::string OUTDOORS = "outdoors";
static const std::string MOD_NAME_KEY = "mod_name";
static const std::string DYNAMIC_PORTRAITS_KEY = "dynamic_portraits";
static const std::string CONDITIONS_KEY = "conditions";
static const std::string SPRITE_NAME_KEY = "sprite_name";
static const std::string TIME_OF_DAY_KEY = "time_of_day"; // DAY or NIGHT
static const std::string DAY_OF_WEEK_KEY = "day_of_week";
static const std::string DAY_OF_MONTH_KEY = "day_of_month"; // 1 - 28
static const std::string SEASON_KEY = "season";
static const std::string YEAR_KEY = "year";
static const std::string WEATHER_KEY = "weather";
static const std::string LOCATION_KEY = "location"; // INDOORS, OUTDOORS, <LOCATION_NAME>
static const std::string NPC_KEY = "npc";

static struct DynamicPortrait {
	int time_of_day = -1;
	int week_day = -1;
	int month_day = -1;
	int season = -1;
	int year = -1;
	int weather = -1;
	int location = -1;
	int npc = -1;
	std::string sprite_name;
	std::string_view mod_name;

	bool HasTimeOfDayCondition() { return time_of_day != -1; }
	bool HasWeekDayCondition() { return week_day != -1; }
	bool HasMonthDayCondition() { return month_day != -1; }
	bool HasSeasonCondition() { return season != -1; }
	bool HasYearCondition() { return year != -1; }
	bool HasWeatherCondition() { return weather != -1; }
	bool HasLocationCondition() { return location != -1; }
	bool HasNpcCondition() { return npc != -1; }
};

static YYTKInterface* g_ModuleInterface = nullptr;
static CInstance* global_instance = nullptr;
static bool load_on_start = true;
static bool game_is_active = false;
static bool ari_is_in_dungeon = false;
static bool cutscene_is_running = false;
static int current_time_in_seconds = -1;
static int current_day = -1;
static int current_year = -1;
static int current_season = -1;
static int current_weather = -1;
static int current_location = -1;
static std::string ari_current_gm_room = "";
static std::unordered_set<std::string> mod_names = {};
static std::vector<DynamicPortrait> dynamic_portraits = {};
static std::map<int, bool> location_id_to_outdoor_map = {};
static std::map<std::string, int> daytime_to_id_map = {}; // __daytime__
static std::map<int, std::string> id_to_daytime_map = {}; // __daytime__
static std::map<std::string, int> day_name_to_id_map = {}; // __day__
static std::map<int, std::string> day_id_to_name_map = {}; // __day__
static std::map<std::string, int> season_name_to_id_map = {}; // __season__
static std::map<int, std::string> season_id_to_name_map = {}; // __season__
static std::map<std::string, int> weather_name_to_id_map = {}; // __weather__
static std::map<int, std::string> weather_id_to_name_map = {}; // __weather__
static std::map<std::string, int> location_name_to_id_map = {}; // __location_id__
static std::map<int, std::string> location_id_to_name_map = {}; // __location_id__
static std::map<std::string, int> npc_name_to_id_map = {}; // __npc_id__
static std::map<int, std::string> npc_id_to_name_map = {}; // __npc_id__

void ResetStaticFields(bool title_screen)
{
	if (title_screen)
	{
		game_is_active = false;
		ari_current_gm_room = "";
	}
	
	ari_is_in_dungeon = false;
	cutscene_is_running = false;
	current_time_in_seconds = -1;
	current_day = -1;
	current_season = -1;
	current_year = -1;
	current_weather = -1;
	current_location = -1;
}

std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::size_t start = 0;
	std::size_t end;

	while ((end = s.find(delimiter, start)) != std::string::npos) {
		tokens.push_back(s.substr(start, end - start));
		start = end + 1;
	}

	tokens.push_back(s.substr(start));
	return tokens;
}

bool GameIsPaused()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);
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

std::string to_upper(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::toupper(c); });
	return s;
}

std::string to_lower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return s;
}

void LoadDaytime()
{
	size_t array_length = 0;
	RValue daytime = global_instance->GetMember("__daytime__");
	g_ModuleInterface->GetArraySize(daytime, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(daytime, i, array_element);
		daytime_to_id_map[array_element->ToString()] = i;
		id_to_daytime_map[i] = array_element->ToString();
	}
}

void LoadDays()
{
	size_t array_length = 0;
	RValue days = global_instance->GetMember("__day__");
	g_ModuleInterface->GetArraySize(days, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(days, i, array_element);
		day_name_to_id_map[array_element->ToString()] = i;
		day_id_to_name_map[i] = array_element->ToString();
	}
}

void LoadSeasons()
{
	size_t array_length = 0;
	RValue seasons = global_instance->GetMember("__season__");
	g_ModuleInterface->GetArraySize(seasons, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(seasons, i, array_element);
		season_name_to_id_map[array_element->ToString()] = i;
		season_id_to_name_map[i] = array_element->ToString();
	}
}

void LoadWeather()
{
	size_t array_length = 0;
	RValue weather = global_instance->GetMember("__weather__");
	g_ModuleInterface->GetArraySize(weather, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(weather, i, array_element);
		weather_name_to_id_map[array_element->ToString()] = i;
		weather_id_to_name_map[i] = array_element->ToString();
	}
}

void LoadLocations()
{
	// Location IDs
	RValue location_ids = global_instance->GetMember("__location_id__");
	RValue location_ids_size = g_ModuleInterface->CallBuiltin("array_length", { location_ids });
	for (int i = 0; i < location_ids_size.m_Real; i++)
	{
		RValue location = g_ModuleInterface->CallBuiltin("array_get", { location_ids, i });
		location_name_to_id_map[location.ToString()] = i;
		location_id_to_name_map[i] = location.ToString();
	}

	// Append "indoors" and "outdoors" to location maps.
	location_name_to_id_map[INDOORS] = location_name_to_id_map.size();
	location_name_to_id_map[OUTDOORS] = location_name_to_id_map.size();
	location_id_to_name_map[location_name_to_id_map[INDOORS]] = INDOORS;
	location_id_to_name_map[location_name_to_id_map[OUTDOORS]] = OUTDOORS;

	// Default location data
	RValue locations = global_instance->GetMember("__locations");
	RValue locations_size = g_ModuleInterface->CallBuiltin("array_length", { locations });

	for (int i = 0; i < locations_size.m_Real; i++)
	{
		RValue location = g_ModuleInterface->CallBuiltin("array_get", { locations, i });
		RValue outdoor = g_ModuleInterface->CallBuiltin("struct_get", { location, "outdoor" });
		location_id_to_outdoor_map[i] = outdoor.ToBoolean();
	}
}

void LoadNpcs()
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

bool AriIsIndoors()
{
	if (!game_is_active || current_location == -1)
		return false;

	if (location_id_to_outdoor_map[current_location])
		return false;

	if (ari_is_in_dungeon)
		return false;

	return true;
}

bool IsNight()
{
	return current_time_in_seconds >= EIGHT_PM_IN_SECONDS;
}

bool IsValidTimeOfDay(const std::string& s)
{
	if (s.empty() || s.size() == 0)
		return false;

	std::string s_lower = to_lower(s);
	return daytime_to_id_map.contains(s_lower);
}

bool IsValidDayOfWeek(const std::string& s)
{
	if (s.empty() || s.size() == 0)
		return false;

	std::string s_lower = to_lower(s);
	return day_name_to_id_map.contains(s_lower);
}

bool IsValidSeason(const std::string& s)
{
	if (s.empty() || s.size() == 0)
		return false;

	std::string s_lower = to_lower(s);
	return season_name_to_id_map.contains(s_lower);
}

bool IsValidWeather(const std::string& s)
{
	if (s.empty() || s.size() == 0)
		return false;

	std::string s_lower = to_lower(s);
	return weather_name_to_id_map.contains(s_lower);
}

bool IsValidLocation(const std::string& s)
{
	if (s.empty() || s.size() == 0)
		return false;

	std::string s_lower = to_lower(s);
	return location_name_to_id_map.contains(s_lower);
}

bool IsValidNpc(const std::string& s)
{
	if (s.empty() || s.size() == 0)
		return false;

	std::string s_lower = to_lower(s);
	return npc_name_to_id_map.contains(s_lower);
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

void CreateOrLoadConfigFile()
{
	// Load config file.
	std::exception_ptr eptr;
	try
	{
		// Try to find the mod_data directory.
		std::string current_dir = std::filesystem::current_path().string();
		std::string mod_data_folder = current_dir + "\\mod_data";
		if (!std::filesystem::exists(mod_data_folder))
		{
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"mod_data\" directory was not found. Creating directory: %s", MOD_NAME, VERSION, mod_data_folder.c_str());
			std::filesystem::create_directory(mod_data_folder);
		}

		// Try to find the mod_data/DynamicNpcPortraits directory.
		std::string dynamic_npc_portraits_folder = mod_data_folder + "\\DynamicNpcPortraits";
		if (!std::filesystem::exists(dynamic_npc_portraits_folder))
		{
			g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - The \"DynamicNpcPortraits\" directory was not found. Creating directory: %s", MOD_NAME, VERSION, dynamic_npc_portraits_folder.c_str());
			std::filesystem::create_directory(dynamic_npc_portraits_folder);
		}

		// Iterate over all files in the mod_data/DynamicNpcPortraits directory.
		try
		{
			for (const auto& file : fs::directory_iterator(dynamic_npc_portraits_folder))
			{
				if (file.is_regular_file() && file.path().extension() == ".json")
				{
					std::string filename = file.path().filename().string();
					
					std::ifstream in_stream(file.path());
					if (in_stream.good())
					{
						g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Reading JSON file: %s", MOD_NAME, VERSION, filename.c_str());

						try
						{
							json json_object = json::parse(in_stream);

							// Check if the json_object is empty.
							if (json_object.empty())
							{
								g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - No values found in file: %s", MOD_NAME, VERSION, filename.c_str());
								g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Ignoring file: %s", MOD_NAME, VERSION, filename.c_str());
							}
							else
							{
								// Try loading the mod_name value.
								std::string_view mod_name_view;
								if (json_object.contains(MOD_NAME_KEY) && json_object.at(MOD_NAME_KEY).is_string())
								{
									std::string mod_name = json_object[MOD_NAME_KEY];
									if (!mod_name.empty() && mod_name.size() > 0)
									{
										if(!mod_names.contains(mod_name))
											mod_names.insert(mod_name);
										mod_name_view = *mod_names.find(mod_name);
									}
								}

								// Try loading the conditions value.
								if (json_object.contains(DYNAMIC_PORTRAITS_KEY) && json_object.at(DYNAMIC_PORTRAITS_KEY).is_array())
								{
									std::vector<json> json_dynamic_portraits = json_object[DYNAMIC_PORTRAITS_KEY];
									if (!json_dynamic_portraits.empty())
									{
										for (json json_dynamic_portrait : json_dynamic_portraits)
										{
											if (!json_dynamic_portrait.contains(CONDITIONS_KEY) || !json_dynamic_portrait.at(CONDITIONS_KEY).is_object() || !json_dynamic_portrait.contains(SPRITE_NAME_KEY) || !json_dynamic_portrait.at(SPRITE_NAME_KEY).is_string() || !json_dynamic_portrait.contains(NPC_KEY) || !json_dynamic_portrait.at(NPC_KEY).is_string())
												continue;

											json json_dynamic_portrait_conditions = json_dynamic_portrait[CONDITIONS_KEY];
											if (json_dynamic_portrait_conditions.empty())
												continue;
											
											std::string json_dynamic_portrait_sprite_name = json_dynamic_portrait[SPRITE_NAME_KEY];
											if (json_dynamic_portrait_sprite_name.empty() || json_dynamic_portrait_sprite_name.size() == 0)
												continue;

											std::string json_dynamic_portrait_npc = json_dynamic_portrait[NPC_KEY];
											if (json_dynamic_portrait_npc.empty() || json_dynamic_portrait_npc.size() == 0 || !IsValidNpc(json_dynamic_portrait_npc))
												continue;

											// Test the base_sprite_name using the neutral expression.
											std::string test_base_sprite_neutral_expression = json_dynamic_portrait_sprite_name + "_neutral";
											RValue asset_index = g_ModuleInterface->CallBuiltin("asset_get_index", { test_base_sprite_neutral_expression.c_str() });
											if (asset_index.m_Kind != VALUE_REF)
											{
												g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - The provided base sprite doesn't exist: %s", MOD_NAME, VERSION, json_dynamic_portrait_sprite_name.c_str());
												continue;
											}

											DynamicPortrait dynamic_portrait = DynamicPortrait();
											dynamic_portrait.mod_name = mod_name_view;
											dynamic_portrait.sprite_name = json_dynamic_portrait_sprite_name;
											dynamic_portrait.npc = npc_name_to_id_map[json_dynamic_portrait_npc];

											// time_of_day condition.
											if (json_dynamic_portrait_conditions.contains(TIME_OF_DAY_KEY) && json_dynamic_portrait_conditions.at(TIME_OF_DAY_KEY).is_string())
											{
												std::string time_of_day_str = json_dynamic_portrait_conditions[TIME_OF_DAY_KEY];
												if (IsValidTimeOfDay(time_of_day_str))
													dynamic_portrait.time_of_day = daytime_to_id_map[time_of_day_str];
											}

											// day_of_week condition.
											if (json_dynamic_portrait_conditions.contains(DAY_OF_WEEK_KEY) && json_dynamic_portrait_conditions.at(DAY_OF_WEEK_KEY).is_string())
											{
												std::string day_of_week_str = json_dynamic_portrait_conditions[DAY_OF_WEEK_KEY];
												if (IsValidDayOfWeek(day_of_week_str))
													dynamic_portrait.week_day = day_name_to_id_map[day_of_week_str];
											}

											// day_of_month condition.
											if (json_dynamic_portrait_conditions.contains(DAY_OF_MONTH_KEY) && json_dynamic_portrait_conditions.at(DAY_OF_MONTH_KEY).is_number_integer())
											{
												int day_of_month = json_dynamic_portrait_conditions[DAY_OF_MONTH_KEY];
												if (day_of_month > 0 && day_of_month < 29)
													dynamic_portrait.month_day = day_of_month;
											}

											// season condition.
											if (json_dynamic_portrait_conditions.contains(SEASON_KEY) && json_dynamic_portrait_conditions.at(SEASON_KEY).is_string())
											{
												std::string season_str = json_dynamic_portrait_conditions[SEASON_KEY];
												if (IsValidSeason(season_str))
													dynamic_portrait.season = season_name_to_id_map[season_str];
											}

											// year condition.
											if (json_dynamic_portrait_conditions.contains(YEAR_KEY) && json_dynamic_portrait_conditions.at(YEAR_KEY).is_number_integer())
											{
												int year = json_dynamic_portrait_conditions[YEAR_KEY];
												if (year > 0)
													dynamic_portrait.year = year;
											}

											// weather condition.
											if (json_dynamic_portrait_conditions.contains(WEATHER_KEY) && json_dynamic_portrait_conditions.at(WEATHER_KEY).is_string())
											{
												std::string weather_str = json_dynamic_portrait_conditions[WEATHER_KEY];
												if (IsValidWeather(weather_str))
													dynamic_portrait.weather = weather_name_to_id_map[weather_str];
											}

											// location condition.
											if (json_dynamic_portrait_conditions.contains(LOCATION_KEY) && json_dynamic_portrait_conditions.at(LOCATION_KEY).is_string())
											{
												std::string location_str = json_dynamic_portrait_conditions[LOCATION_KEY];
												if (IsValidLocation(location_str))
													dynamic_portrait.location = location_name_to_id_map[location_str];
													
											}

											// Final validation. Confirm the reminder has at least one valid condition.
											if (dynamic_portrait.HasTimeOfDayCondition() || dynamic_portrait.HasWeekDayCondition() || dynamic_portrait.HasMonthDayCondition() || dynamic_portrait.HasSeasonCondition() || dynamic_portrait.HasYearCondition() || dynamic_portrait.HasWeatherCondition() || dynamic_portrait.HasLocationCondition())
											{
												dynamic_portraits.push_back(dynamic_portrait);
												g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Dynamic Portrait config loaded for sprite: %s", MOD_NAME, VERSION, json_dynamic_portrait_sprite_name.c_str());
											}
										}
									}
									else
									{
										g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - No conditions were set in file: %s", MOD_NAME, VERSION, filename.c_str());
										g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Ignoring the file.", MOD_NAME, VERSION);
									}
								}
								else
								{
									g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Missing \"%s\" value in mod configuration file: %s", MOD_NAME, VERSION, DYNAMIC_PORTRAITS_KEY.c_str(), filename.c_str());
									g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Ignoring the file.", MOD_NAME, VERSION);
								}

							}
						}
						catch (...)
						{
							eptr = std::current_exception();
							PrintError(eptr);

							g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to parse JSON from file: %s", MOD_NAME, VERSION, filename.c_str());
							g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] - Make sure the file is valid JSON!", MOD_NAME, VERSION);
						}

						in_stream.close();
					}
					else
					{
						in_stream.close();
						g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to load JSON file: %s", MOD_NAME, VERSION, filename.c_str());
					}
				}
			}
		}
		catch (...)
		{
			eptr = std::current_exception();
			PrintError(eptr);

			g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Error reading dynamic portrait config files in folder: %s", MOD_NAME, VERSION, dynamic_npc_portraits_folder.c_str());
		}
	}
	catch (...)
	{
		eptr = std::current_exception();
		PrintError(eptr);

		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - An error occurred reading the dynamic portrait config files.", MOD_NAME, VERSION);
	}
}

RValue GetDynamicNpcPortrait(std::string sprite_name)
{
	// Example: spr_portrait_balor_spring_sincere_special
	std::vector<std::string> sprite_name_parts = split(sprite_name, '_');
	std::string npc_name = sprite_name_parts[2];

	std::string expression = "";
	for (int i = 4; i < sprite_name_parts.size(); i++)
	{
		expression += sprite_name_parts[i];
		if (i != sprite_name_parts.size() - 1)
			expression += "_";
	}

	for (DynamicPortrait dynamic_portrait : dynamic_portraits)
	{
		if (npc_name != npc_id_to_name_map[dynamic_portrait.npc])
			continue;

		if (dynamic_portrait.HasTimeOfDayCondition())
		{
			if (dynamic_portrait.time_of_day == daytime_to_id_map["day"] && IsNight())
				continue;
			if (dynamic_portrait.time_of_day == daytime_to_id_map["night"] && !IsNight())
				continue;
		}

		if (dynamic_portrait.HasWeekDayCondition() && ((current_day - 1) % 7) != dynamic_portrait.week_day)
			continue;

		if (dynamic_portrait.HasMonthDayCondition() && current_day != dynamic_portrait.month_day)
			continue;

		if (dynamic_portrait.HasSeasonCondition() && current_season != dynamic_portrait.season)
			continue;

		if (dynamic_portrait.HasYearCondition() && current_year != dynamic_portrait.year)
			continue;

		if (dynamic_portrait.HasWeatherCondition() && current_weather != dynamic_portrait.weather)
			continue;

		if (dynamic_portrait.HasLocationCondition())
		{
			if (dynamic_portrait.location == location_name_to_id_map[OUTDOORS] || dynamic_portrait.location == location_name_to_id_map[INDOORS])
			{
				if (dynamic_portrait.location == location_name_to_id_map[OUTDOORS] && AriIsIndoors())
					continue;
				if (dynamic_portrait.location == location_name_to_id_map[INDOORS] && !AriIsIndoors())
					continue;
			}
			else if (dynamic_portrait.HasLocationCondition() && current_location != dynamic_portrait.location)
				continue;
		}

		std::string sprite_name = dynamic_portrait.sprite_name + "_" + expression;
		RValue sprite = g_ModuleInterface->CallBuiltin("asset_get_index", { sprite_name.c_str() });
		if (sprite.m_Kind == VALUE_REF)
			return sprite;
	}
}

RValue& GmlScriptIsDungeonRoomCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_IS_DUNGEON_ROOM));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active && Result.m_Kind == VALUE_BOOL)
		ari_is_in_dungeon = Result.ToBoolean();

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

	RValue gm_room = Result.GetMember("gm_room");
	RValue room_name = g_ModuleInterface->CallBuiltin("room_get_name", { gm_room });
	ari_current_gm_room = room_name.ToString();
	
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
	if (game_is_active)
		current_location = Arguments[0]->ToInt64();

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_TRY_LOCATION_ID_TO_STRING));
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

	if (game_is_active && !GameIsPaused())
	{
		RValue time = global_instance->GetMember("__clock").GetMember("time");
		current_time_in_seconds = time.ToInt64();
	}

	return Result;
}

RValue& GmlScriptCalendarDayCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_CALENDAR_DAY));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active)
		current_day = Result.ToInt64() + 1; // Result is a VALUE_REAL that is the 0-indexed calendar day

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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_CALENDAR_SEASON));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active)
		current_season = Result.ToInt64(); // Result is a VALUE_REAL that is the 0-indexed calendar season

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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_CALENDAR_YEAR));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active)
		current_year = Result.ToInt64() + 1; // Result is a VALUE_REAL that is the 0-indexed calendar year

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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_WEATHER));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (IsNumeric(Result))
		current_weather = Result.ToInt64();

	game_is_active = true;
	return Result;
}

RValue& GmlScriptCutsceneIsRunningCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_CUTSCENE_IS_RUNNING));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (game_is_active && Result.m_Kind == VALUE_BOOL)
		cutscene_is_running = Result.ToBoolean();

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
	if (game_is_active)
	{
		RValue type = g_ModuleInterface->CallBuiltin("asset_get_type", { *Arguments[0] });
		if (type.ToInt64() == 1) // asset_sprite
		{
			RValue name = g_ModuleInterface->CallBuiltin("sprite_get_name", { *Arguments[0] });
			std::string name_str = name.ToString();

			if (name_str.contains("spr_portrait"))
			{
				RValue dynamic_sprite = GetDynamicNpcPortrait(name_str);
				if (dynamic_sprite.m_Kind == VALUE_REF)
					*Arguments[0] = dynamic_sprite;
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
		
		LoadDaytime();
		LoadDays();
		LoadSeasons();
		LoadWeather();
		LoadLocations();
		LoadNpcs();
		CreateOrLoadConfigFile();
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

void CreateHookGmlScriptIsDungeonRoom(AurieStatus& status)
{
	CScript* gml_script_is_dungeon_room = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_IS_DUNGEON_ROOM,
		(PVOID*)&gml_script_is_dungeon_room
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_IS_DUNGEON_ROOM);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_IS_DUNGEON_ROOM,
		gml_script_is_dungeon_room->m_Functions->m_ScriptFunction,
		GmlScriptIsDungeonRoomCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_IS_DUNGEON_ROOM);
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

void CreateHookGmlScriptCalendarDay(AurieStatus& status)
{
	CScript* gml_script_calendar_day = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CALENDAR_DAY,
		(PVOID*)&gml_script_calendar_day
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CALENDAR_DAY);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_CALENDAR_DAY,
		gml_script_calendar_day->m_Functions->m_ScriptFunction,
		GmlScriptCalendarDayCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CALENDAR_DAY);
	}
}

void CreateHookGmlScriptCalendarSeason(AurieStatus& status)
{
	CScript* gml_script_calendar_season = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CALENDAR_SEASON,
		(PVOID*)&gml_script_calendar_season
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CALENDAR_SEASON);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_CALENDAR_SEASON,
		gml_script_calendar_season->m_Functions->m_ScriptFunction,
		GmlScriptCalendarSeasonCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CALENDAR_SEASON);
	}
}

void CreateHookGmlScriptCalendarYear(AurieStatus& status)
{
	CScript* gml_script_calendar_year = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CALENDAR_YEAR,
		(PVOID*)&gml_script_calendar_year
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CALENDAR_YEAR);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_CALENDAR_YEAR,
		gml_script_calendar_year->m_Functions->m_ScriptFunction,
		GmlScriptCalendarYearCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CALENDAR_YEAR);
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

void CreateHookGmlScriptCutsceneIsRunning(AurieStatus& status)
{
	CScript* gml_script_cutscene_is_running = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_CUTSCENE_IS_RUNNING,
		(PVOID*)&gml_script_cutscene_is_running
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CUTSCENE_IS_RUNNING);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_CUTSCENE_IS_RUNNING,
		gml_script_cutscene_is_running->m_Functions->m_ScriptFunction,
		GmlScriptCutsceneIsRunningCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_CUTSCENE_IS_RUNNING);
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

	CreateHookGmlScriptIsDungeonRoom(status);
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

	CreateHookGmlScriptTryLocationIdToString(status);
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

	CreateHookGmlScriptCalendarDay(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptCalendarSeason(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptCalendarYear(status);
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

	CreateHookGmlScriptCutsceneIsRunning(status);
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
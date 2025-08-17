#include <nlohmann/json.hpp>
#include <YYToolkit/YYTK_Shared.hpp>
using namespace Aurie;
using namespace YYTK;
using json = nlohmann::json;

static const char* const MOD_NAME = "SummoningCircle";
static const char* const VERSION = "1.0.0";
static const char* const GML_SCRIPT_TRY_OBJECT_ID_TO_STRING = "gml_Script_try_object_id_to_string";
static const char* const GML_SCRIPT_CREATE_NOTIFICATION = "gml_Script_create_notification";
static const char* const GML_SCRIPT_TELEPORT_ARI_TO_ROOM = "gml_Script_ari_teleport_to_room"; // Used to teleport Ari.
static const char* const GML_SCRIPT_INTERACT = "gml_Script_interact"; // Used to track when the furniture is used.
static const char* const GML_SCRIPT_WRITE_FURNITURE_TO_LOCATION = "gml_Script_write_furniture_to_location"; // Used to track when the furniture is placed.
static const char* const GML_SCRIPT_PICK_NODE = "gml_Script_pick_node"; // Used to track when the furniture is removed.
static const char* const GML_SCRIPT_ON_ROOM_START = "gml_Script_on_room_start@WeatherManager@Weather";
static const char* const GML_SCRIPT_GET_WEATHER = "gml_Script_get_weather@WeatherManager@Weather";
static const char* const GML_SCRIPT_TRY_LOCATION_ID_TO_STRING = "gml_Script_try_location_id_to_string";
static const char* const GML_SCRIPT_LOAD_GAME = "gml_Script_load_game";
static const char* const GML_SCRIPT_SAVE_GAME = "gml_Script_save_game";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static std::string SUMMONING_CIRCLE_TWO_REQUIRED = "Notifications/Mods/Summoning Circle/two_required";

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_on_start = true;
static bool mod_is_healthy = true;
static bool game_is_active = false;
static bool once_per_save_load = true;
static bool teleport_ari = false;
static std::pair<int, int> teleport_ari_to = {};
static std::string ari_current_location = "";
static std::map<int, std::string> object_id_to_name_map = {};
static std::map<std::string, int> object_name_to_id_map = {};
static std::map<std::string, int> location_name_to_id_map = {};
static std::vector<std::pair<int, int>> summoning_circle_positions = {};

bool LoadObjectIds(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_try_object_id_to_string = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_TRY_OBJECT_ID_TO_STRING,
		(PVOID*)&gml_script_try_object_id_to_string
	);

	for (int i = 0; i < 5000; i++)
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
		{
			object_id_to_name_map[i] = result.ToString();
			object_name_to_id_map[result.ToString()] = i;
		}
			
	}

	if (object_id_to_name_map.size() == 0 || object_name_to_id_map.size() == 0)
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to load ANY object data!", MOD_NAME, VERSION);
		return false;
	}

	if (!object_name_to_id_map.contains("summoning_circle") || !object_id_to_name_map.contains(object_name_to_id_map["summoning_circle"]))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to load summoning_circle object data!", MOD_NAME, VERSION);
		return false;
	}

	return true;
}

bool LoadLocationIds(CInstance* global_instance)
{
	size_t array_length;
	RValue location_ids = global_instance->GetMember("__location_id__");
	g_ModuleInterface->GetArraySize(location_ids, array_length);

	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(location_ids, i, array_element);

		location_name_to_id_map[array_element->ToString()] = i;
	}

	if (location_name_to_id_map.size() == 0)
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to load ANY location data!", MOD_NAME, VERSION);
		return false;
	}

	return true;
}

double CalculateDistance(int x1, int y1, int x2, int y2) {
	return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

void ResetStaticFields(bool return_to_title_screen)
{
	if (return_to_title_screen)
	{
		game_is_active = false;
		once_per_save_load = true;
		teleport_ari = false;
		teleport_ari_to = {};
		ari_current_location = "";
		summoning_circle_positions = {};
	}
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

void TeleportAriToRoom(CInstance* Self, CInstance* Other, int location_id, int x_coordinate, int y_coordinate)
{
	CScript* gml_script_ari_teleport_to_room = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_TELEPORT_ARI_TO_ROOM,
		(PVOID*)&gml_script_ari_teleport_to_room
	);

	RValue retval;
	RValue location = location_id;
	RValue x = x_coordinate;
	RValue y = y_coordinate;
	RValue* location_ptr = &location;
	RValue* x_ptr = &x;
	RValue* y_ptr = &y;
	RValue* argument_array[3] = { location_ptr, x_ptr, y_ptr };
	gml_script_ari_teleport_to_room->m_Functions->m_ScriptFunction(
		Self,
		Other,
		retval,
		3,
		argument_array
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

	if (mod_is_healthy && game_is_active)
	{
		if (teleport_ari)
		{
			int x = (teleport_ari_to.first * 8) + 12; // See notes. The 12 is from: (write_size_x * 8 / 2)
			int y = (teleport_ari_to.second * 8) + 12;

			teleport_ari = false;
			teleport_ari_to = {};
			TeleportAriToRoom(self, other, location_name_to_id_map["farm"], x, y);
		}
	}
}

RValue& GmlScriptInteractCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_is_healthy && game_is_active)
	{
		RValue object = Arguments[0]->ToInstance();
		int object_id = object.GetMember("object_id").ToInt64();
		std::string object_name = object_id_to_name_map[object_id];

		if (object_name == "summoning_circle")
		{
			if (summoning_circle_positions.size() < 2)
			{
				CreateNotification(SUMMONING_CIRCLE_TWO_REQUIRED, Self, Other);
				g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] Two summoning circles are required to use it!", MOD_NAME, VERSION);
			}
			else
			{
				int x = object.GetMember("top_left_x").ToInt64();
				int y = object.GetMember("top_left_y").ToInt64();

				for (auto it : summoning_circle_positions)
				{
					if (it.first != x && it.second != y)
					{
						teleport_ari_to = it;
						break;
					}
				}

				teleport_ari = true;
				g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] Teleporting Ari.", MOD_NAME, VERSION);
				// TODO: Try opening conversation
			}

			return Result;
		}
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

RValue& GmlScriptWriteFurnitureToLocationCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (summoning_circle_positions.size() == 2)
	{
		g_ModuleInterface->Print(CM_LIGHTYELLOW, "[%s %s] You already have two Summoning Circles on your farm!");
		return Result;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_WRITE_FURNITURE_TO_LOCATION));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (mod_is_healthy && game_is_active)
	{
		if (Result.m_Kind != VALUE_UNDEFINED && Result.m_Kind != VALUE_UNSET && Result.m_Kind != VALUE_NULL)
		{
			RValue object_id = Result.GetMember("object_id");
			if (object_id.ToInt64() == object_name_to_id_map["summoning_circle"])
			{
				RValue top_left_x = Result.GetMember("top_left_x");
				RValue top_left_y = Result.GetMember("top_left_y");
				//RValue write_size_x = Result.GetMember("write_size_x");
				//RValue write_size_y = Result.GetMember("write_size_y");

				std::pair<int64_t, int64_t> position = { top_left_x.ToInt64(), top_left_y.ToInt64() };
				summoning_circle_positions.push_back(position);
			}
		}
	}

	return Result;
}

RValue& GmlScriptPickNodeCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_is_healthy && ari_current_location == "farm")
	{
		int x = Arguments[1]->ToInt64(); // Node's X coord
		int y = Arguments[2]->ToInt64(); // Node's Y coord

		int closest_index = 0;
		double min_distance = DBL_MAX;
		for (int i = 0; i < summoning_circle_positions.size(); i++)
		{
			auto position = summoning_circle_positions[i];
			double distance = CalculateDistance(x, y, position.first, position.second); // Testing comparing to top-left coords
			if (distance < min_distance) {
				min_distance = distance;
				closest_index = i;
			}
		}

		summoning_circle_positions.erase(summoning_circle_positions.begin() + closest_index);


		//for (auto pair = summoning_circle_positions.begin(); pair != summoning_circle_positions.end(); ++pair)
		//{
		//	// TODO: Instead, find the summoning circle whose center is the shortest distance to x,y
		//	if (pair->first == x && pair->second == y)
		//	{
		//		summoning_circle_positions.erase(pair);
		//		break;
		//	}
		//}

		


		//RValue node_object_id = Arguments[0]->GetMember("node_object_id");
		//if (node_object_id.m_Kind == VALUE_ARRAY)
		//{
		//	RValue node_object_id_length = g_ModuleInterface->CallBuiltin("array_length", { node_object_id });
		//	for (int i = 0; i < node_object_id_length.m_Real; i++)
		//	{
		//		RValue array_element = g_ModuleInterface->CallBuiltin("array_get", { node_object_id, i });
		//		if (array_element.m_Kind != VALUE_UNDEFINED && array_element.m_Kind != VALUE_UNSET && array_element.m_Kind != VALUE_NULL)
		//		{
		//			if (array_element.ToInt64() == object_name_to_id_map["summoning_circle"])
		//			{
		//				RValue node_top_left_x = Arguments[0]->GetMember("node_top_left_x");
		//				RValue x = g_ModuleInterface->CallBuiltin("array_get", { node_top_left_x, i });

		//				RValue node_top_left_y = Arguments[0]->GetMember("node_top_left_y");
		//				RValue y = g_ModuleInterface->CallBuiltin("array_get", { node_top_left_y, i });

		//				int temp = 1;
		//				std::pair<int, int> position = summoning_circle_positions.back();
		//				summoning_circle_positions.pop_back();
		//			}
		//		}
		//	}
		//}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_PICK_NODE));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptOnRoomStartCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_is_healthy && once_per_save_load && ari_current_location == "farm")
	{
		once_per_save_load = false;

		CRoom* current_room = nullptr;
		if (!AurieSuccess(g_ModuleInterface->GetCurrentRoomData(current_room)))
		{
			g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to obtain the current room!", MOD_NAME, VERSION);
			return Result;
		}

		std::vector<bool> summoning_circle_exists = {};
		for (int i = 0; i < summoning_circle_positions.size(); i++)
			summoning_circle_exists.push_back(false); // Assume it doesn't exist to start

		for (CInstance* inst = current_room->GetMembers().m_ActiveInstances.m_First; inst != nullptr; inst = inst->GetMembers().m_Flink)
		{
			auto map = inst->ToRValue().ToRefMap();
			if (!map.contains("node"))
				continue;

			RValue* nodeValue = map["node"];
			if (!nodeValue || nodeValue->GetKindName() != "struct")
				continue;

			auto nodeRefMap = nodeValue->ToRefMap();
			if (!nodeRefMap.contains("prototype") /*|| !nodeRefMap.contains("hitpoints")*/)
				continue;

			RValue* protoVal = nodeRefMap["prototype"];
			if (!protoVal || protoVal->GetKindName() != "struct")
				continue;

			auto protoMap = protoVal->ToRefMap();
			if (!protoMap.contains("object_id"))
				continue;

			// This works to "scan" for the furniture.
			// To be used when loading in to check if the mod items got unloaded or something caused the furniture to vanish.
			// Each match is a separate furniture node for the matching ID
			int object_id = protoMap["object_id"]->ToInt64();
			if (object_id == object_name_to_id_map["summoning_circle"])
			{
				int x = nodeRefMap["top_left_x"]->ToInt64();
				int y = nodeRefMap["top_left_y"]->ToInt64();

				for (int i = 0; i < summoning_circle_positions.size(); i++)
				{
					if (summoning_circle_positions[i].first == x && summoning_circle_positions[i].second == y)
						summoning_circle_exists[i] = true;
				}
			}
		}

		// Prune missing summoning circles.
		for (int i = 0; i < summoning_circle_exists.size(); i++)
		{
			if (!summoning_circle_exists[i])
				summoning_circle_positions.erase(summoning_circle_positions.begin() + i);
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_ON_ROOM_START));
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
	if (mod_is_healthy)
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

	if (mod_is_healthy && game_is_active && Result.m_Kind == VALUE_STRING)
		ari_current_location = Result.ToString();

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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_LOAD_GAME));
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
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_SAVE_GAME));
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

		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		LoadObjectIds(Self, Other);
		LoadLocationIds(global_instance);

		mod_is_healthy = true;
	}
	// TODO: Reset static fields

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

void CreateHookGmlScriptInteract(AurieStatus& status)
{
	CScript* gml_script_try_object_id_to_string = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_INTERACT,
		(PVOID*)&gml_script_try_object_id_to_string
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_INTERACT);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_INTERACT,
		gml_script_try_object_id_to_string->m_Functions->m_ScriptFunction,
		GmlScriptInteractCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_INTERACT);
	}
}

void CreateHookGmlScriptWriteFurnitureToLocation(AurieStatus& status)
{
	CScript* gml_script_write_furniture_to_location = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_WRITE_FURNITURE_TO_LOCATION,
		(PVOID*)&gml_script_write_furniture_to_location
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_WRITE_FURNITURE_TO_LOCATION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_WRITE_FURNITURE_TO_LOCATION,
		gml_script_write_furniture_to_location->m_Functions->m_ScriptFunction,
		GmlScriptWriteFurnitureToLocationCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_WRITE_FURNITURE_TO_LOCATION);
	}
}

void CreateHookGmlScriptPickNode(AurieStatus& status)
{
	CScript* gml_script_pick_node = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_PICK_NODE,
		(PVOID*)&gml_script_pick_node
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_PICK_NODE);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_PICK_NODE,
		gml_script_pick_node->m_Functions->m_ScriptFunction,
		GmlScriptPickNodeCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_PICK_NODE);
	}
}

void CreateHookGmlScriptOnRoomStart(AurieStatus& status)
{
	CScript* gml_script_on_room_start = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_ON_ROOM_START,
		(PVOID*)&gml_script_on_room_start
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ON_ROOM_START);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_ON_ROOM_START,
		gml_script_on_room_start->m_Functions->m_ScriptFunction,
		GmlScriptOnRoomStartCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_ON_ROOM_START);
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

void CreateHookGmlScriptLoadGame(AurieStatus& status)
{
	CScript* gml_script_load_game = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_LOAD_GAME,
		(PVOID*)&gml_script_load_game
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_LOAD_GAME);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_LOAD_GAME,
		gml_script_load_game->m_Functions->m_ScriptFunction,
		GmlScriptLoadGameCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_LOAD_GAME);
	}
}

void CreateHookGmlScriptSaveGame(AurieStatus& status)
{
	CScript* gml_script_save_game = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SAVE_GAME,
		(PVOID*)&gml_script_save_game
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_SAVE_GAME);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_SAVE_GAME,
		gml_script_save_game->m_Functions->m_ScriptFunction,
		GmlScriptSaveGameCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_SAVE_GAME);
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

	CreateHookGmlScriptInteract(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptWriteFurnitureToLocation(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptPickNode(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptOnRoomStart(status);
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

	CreateHookGmlScriptLoadGame(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptSaveGame(status);
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
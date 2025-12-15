#include <unordered_set>
#include <YYToolkit/YYTK_Shared.hpp> // YYTK v4
using namespace Aurie;
using namespace YYTK;

static const char* const MOD_NAME = "DirectionalAttacks";
static const char* const VERSION = "1.0.1";
static const char* const GML_SCRIPT_FACE_DIR = "gml_Script_face_dir@gml_Object_obj_ari_Create_0";
static const char* const GML_SCRIPT_SET_CARDINAL = "gml_Script_set_cardinal@gml_Object_obj_ari_Create_0";
static const char* const GML_SCRIPT_HELD_ITEM = "gml_Script_held_item@Ari@Ari";
static const char* const GML_SCRIPT_TRY_STRING_TO_ITEM_ID = "gml_Script_try_string_to_item_id";
static const char* const GML_SCRIPT_USE_ITEM = "gml_Script_use_item";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const int UNSET_ITEM_ID = -1;
static const int UNSET_CARDINAL_OVERRIDE = -1;
static const double UNSET_FACE_DIR_OVERRIDE = -1.0;
static const std::vector<std::string> ITEM_NAMES = {
	// Fishing Rods
	 "fishing_rod_worn", "fishing_rod_copper", "fishing_rod_iron", "fishing_rod_silver", "fishing_rod_gold", "fishing_rod_mistril",
	// Nets
	"net_worn", "net_copper", "net_iron", "net_silver", "net_gold", "net_mistril", 
	// Swords
	"sword_worn", "sword_copper", "sword_iron", "sword_silver", "sword_gold", "sword_mistril", "sword_scrap_metal", "sword_verdigris", "sword_crystal", "sword_tarnished_gold"
};

static YYTKInterface* g_ModuleInterface = nullptr;
static bool load_items = true;
static int held_item_id = UNSET_ITEM_ID;
static int cardinal_override = UNSET_CARDINAL_OVERRIDE;
static double face_dir_override = UNSET_FACE_DIR_OVERRIDE;
static std::unordered_set<int> item_ids = {};
static CInstance* ari = nullptr;

std::vector<double> GetCenter(std::vector<double> topLeft, std::vector<double> bottomRight)
{
	double x = (topLeft[0] + bottomRight[0]) / 2.0;
	double y = (topLeft[1] + bottomRight[1]) / 2.0;
	std::vector<double> center = { x, y };
	return center;
}

bool PointInTriangle(std::vector<double> p, std::vector<double> p0, std::vector<double> p1, std::vector<double> p2) {
	auto A = 1.0 / 2.0 * (-p1[1] * p2[0] + p0[1] * (-p1[0] + p2[0]) + p0[0] * (p1[1] - p2[1]) + p1[0] * p2[1]);
	auto sign = A < 0 ? -1 : 1;
	auto s = (p0[1] * p2[0] - p0[0] * p2[1] + (p2[1] - p0[1]) * p[0] + (p0[0] - p2[0]) * p[1]) * sign;
	auto t = (p0[0] * p1[1] - p0[1] * p1[0] + (p0[1] - p1[1]) * p[0] + (p1[0] - p0[0]) * p[1]) * sign;
	return s > 0 && t > 0 && (s + t) < 2 * A * sign;
}

void GetDirectionalValues()
{
	RValue window_get_width = g_ModuleInterface->CallBuiltin(
		"window_get_width",
		{}
	);

	RValue window_get_height = g_ModuleInterface->CallBuiltin(
		"window_get_height",
		{}
	);

	RValue window_mouse_get_x = g_ModuleInterface->CallBuiltin(
		"window_mouse_get_x",
		{}
	);

	RValue window_mouse_get_y = g_ModuleInterface->CallBuiltin(
		"window_mouse_get_y",
		{}
	);

	// Window corners
	std::vector<double> window_top_left = { 0.0, 0.0 };
	std::vector<double> window_top_right = { window_get_width.m_Real, 0.0 };
	std::vector<double> window_bottom_left = { 0.0, window_get_height.m_Real };
	std::vector<double> window_bottom_right = { window_get_width.m_Real, window_get_height.m_Real };

	// Window center
	std::vector<double> center = GetCenter(window_top_left, window_bottom_right);

	// Clicked point
	std::vector<double> clicked = { window_mouse_get_x.m_Real, window_mouse_get_y.m_Real };

	// Check if clicked point is in "north" triangle
	bool clicked_in_north_triangle = PointInTriangle(clicked, center, window_top_left, window_top_right);
	bool clicked_in_south_triangle = PointInTriangle(clicked, center, window_bottom_left, window_bottom_right);
	bool clicked_in_east_triangle = PointInTriangle(clicked, center, window_top_right, window_bottom_right);
	bool clicked_in_west_triangle = PointInTriangle(clicked, center, window_top_left, window_bottom_left);

	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	if (clicked_in_north_triangle)
	{
		cardinal_override = 1;
		face_dir_override = 90.0;
	}

	if (clicked_in_south_triangle)
	{
		cardinal_override = 3;
		face_dir_override = 270.0;
	}

	if (clicked_in_east_triangle)
	{
		cardinal_override = 0;
		face_dir_override = -0.0;
	}

	if (clicked_in_west_triangle)
	{
		cardinal_override = 2;
		face_dir_override = 180.0;
	}
}

void FaceDir(CInstance* Self, CInstance* Other)
{
	if (face_dir_override != UNSET_FACE_DIR_OVERRIDE)
	{
		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		CScript* gml_script_face_dir = nullptr;
		g_ModuleInterface->GetNamedRoutinePointer(
			GML_SCRIPT_FACE_DIR,
			(PVOID*)&gml_script_face_dir
		);

		RValue result;
		RValue argument = face_dir_override;
		RValue* argument_ptr = &argument;

		gml_script_face_dir->m_Functions->m_ScriptFunction(
			ari,
			Other,
			result,
			1,
			{ &argument_ptr }
		);
	}

}

void SetCardinal(CInstance* Self, CInstance* Other)
{
	if (cardinal_override != UNSET_CARDINAL_OVERRIDE)
	{
		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		CScript* gml_script_set_cardinal = nullptr;
		g_ModuleInterface->GetNamedRoutinePointer(
			GML_SCRIPT_SET_CARDINAL,
			(PVOID*)&gml_script_set_cardinal
		);

		RValue result;
		RValue argument = cardinal_override;
		argument.m_i64 = cardinal_override;
		argument.m_Kind = VALUE_INT64;
		RValue* argument_ptr = &argument;

		gml_script_set_cardinal->m_Functions->m_ScriptFunction(
			ari,
			Other,
			result,
			1,
			{ &argument_ptr }
		);
	}
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
		RValue exists = g_ModuleInterface->CallBuiltin(
			"struct_exists",
			{ self, "set_cardinal" }
		);

		if (exists.m_Kind == VALUE_BOOL && exists.m_Real == 1.0)
			ari = self;
	}
}

RValue& GmlScriptHeldItemCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_HELD_ITEM));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (Result.m_Kind != VALUE_UNDEFINED && held_item_id != Result.GetMember("item_id").ToInt64())
		held_item_id = Result.GetMember("item_id").ToInt64();

	return Result;
}

RValue& GmlScriptUseItemCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (ari != nullptr && held_item_id != UNSET_ITEM_ID)
	{
		auto it = std::find(item_ids.begin(), item_ids.end(), held_item_id);
		if (it != item_ids.end()) {
			GetDirectionalValues();
			FaceDir(Self, Other);
			SetCardinal(Self, Other);
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_USE_ITEM));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	cardinal_override = UNSET_CARDINAL_OVERRIDE;
	face_dir_override = UNSET_FACE_DIR_OVERRIDE;

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
	ari = nullptr;
	held_item_id = UNSET_ITEM_ID;

	if (load_items)
	{
		for (std::string item_name : ITEM_NAMES)
		{
			CScript* gml_script_try_string_to_item_id = nullptr;
			g_ModuleInterface->GetNamedRoutinePointer(
				GML_SCRIPT_TRY_STRING_TO_ITEM_ID,
				(PVOID*)&gml_script_try_string_to_item_id
			);

			RValue argument = RValue(item_name);
			RValue* argument_ptr = &argument;
			RValue result;
			gml_script_try_string_to_item_id->m_Functions->m_ScriptFunction(
				Self,
				Other,
				result,
				1,
				{ &argument_ptr }
			);

			if (result.m_Kind == VALUE_INT32 || result.m_Kind == VALUE_INT64 || result.m_Kind != VALUE_REAL)
				item_ids.insert(result.ToInt64());
		}

		load_items = false;
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook (%s)!", MOD_NAME, VERSION, "EVENT_OBJECT_CALL");
	}
}

void CreateHookGmlScriptHeldItem(AurieStatus& status)
{
	CScript* gml_script_held_item = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_HELD_ITEM,
		(PVOID*)&gml_script_held_item
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_HELD_ITEM);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_HELD_ITEM,
		gml_script_held_item->m_Functions->m_ScriptFunction,
		GmlScriptHeldItemCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_HELD_ITEM);
	}
}

void CreateHookGmlScriptUseItem(AurieStatus& status)
{
	CScript* gml_script_use_item = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_USE_ITEM,
		(PVOID*)&gml_script_use_item
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_USE_ITEM);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_USE_ITEM,
		gml_script_use_item->m_Functions->m_ScriptFunction,
		GmlScriptUseItemCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_USE_ITEM);
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

	CreateObjectCallback(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptHeldItem(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptUseItem(status);
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
#include <iostream>
#include <fstream>
#include <codecvt>
#include <shlobj.h>
#include <filesystem>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static const char* const VERSION = "1.0.0";
static const int UNSET_CARDINAL_OVERRIDE = -1;
static const double UNSET_FACE_DIR_OVERRIDE = -1.0;

static YYTKInterface* g_ModuleInterface = nullptr;
static int cardinal_override = UNSET_CARDINAL_OVERRIDE;
static double face_dir_override = UNSET_FACE_DIR_OVERRIDE;

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

RValue& GmlScriptFaceDirCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (face_dir_override != UNSET_FACE_DIR_OVERRIDE)
	{
		Arguments[0]->m_Real = face_dir_override;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_face_dir@gml_Object_obj_ari_Create_0"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptSetCardinalCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (cardinal_override != UNSET_CARDINAL_OVERRIDE)
	{
		Arguments[0]->m_i64 = cardinal_override;
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_set_cardinal@gml_Object_obj_ari_Create_0"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptSetAnimationCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (Arguments[0]->m_i64 == 7) // Attack Animation
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

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_set_animation@gml_Object_obj_ari_Create_0"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (Arguments[0]->m_i64 != 7 && Arguments[0]->m_i64 != 8 && Arguments[0]->m_i64 != 9) // Attack Animation
	{
		cardinal_override = UNSET_CARDINAL_OVERRIDE;
		face_dir_override = UNSET_FACE_DIR_OVERRIDE;
	}

	return Result;
}

RValue& GmlScriptMoveAriCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (cardinal_override != UNSET_CARDINAL_OVERRIDE && face_dir_override != UNSET_FACE_DIR_OVERRIDE)
		return Result;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_move_ari"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);
	return Result;
}

void CreateHookGmlScriptFaceDir(AurieStatus& status)
{
	CScript* gml_script_on_draw_gui = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_face_dir@gml_Object_obj_ari_Create_0",
		(PVOID*)&gml_script_on_draw_gui
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DirectionalAttacks %s] - Failed to get script (gml_Script_face_dir@gml_Object_obj_ari_Create_0)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_face_dir@gml_Object_obj_ari_Create_0",
		gml_script_on_draw_gui->m_Functions->m_ScriptFunction,
		GmlScriptFaceDirCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DirectionalAttacks %s] - Failed to hook script (gml_Script_face_dir@gml_Object_obj_ari_Create_0)!", VERSION);
	}
}

void CreateHookGmlScriptSetCardinal(AurieStatus& status)
{
	CScript* gml_script_on_draw_gui = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_set_cardinal@gml_Object_obj_ari_Create_0",
		(PVOID*)&gml_script_on_draw_gui
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DirectionalAttacks %s] - Failed to get script (gml_Script_set_cardinal@gml_Object_obj_ari_Create_0)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_set_cardinal@gml_Object_obj_ari_Create_0",
		gml_script_on_draw_gui->m_Functions->m_ScriptFunction,
		GmlScriptSetCardinalCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DirectionalAttacks %s] - Failed to hook script (gml_Script_set_cardinal@gml_Object_obj_ari_Create_0)!", VERSION);
	}
}

void CreateHookGmlScriptSetAnimation(AurieStatus& status)
{
	CScript* gml_script_on_draw_gui = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_set_animation@gml_Object_obj_ari_Create_0",
		(PVOID*)&gml_script_on_draw_gui
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DirectionalAttacks %s] - Failed to get script (gml_Script_set_animation@gml_Object_obj_ari_Create_0)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_set_animation@gml_Object_obj_ari_Create_0",
		gml_script_on_draw_gui->m_Functions->m_ScriptFunction,
		GmlScriptSetAnimationCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DirectionalAttacks %s] - Failed to hook script (gml_Script_set_animation@gml_Object_obj_ari_Create_0)!", VERSION);
	}
}

void CreateHookGmlScriptMoveAri(AurieStatus& status)
{
	CScript* gml_script_on_draw_gui = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_move_ari",
		(PVOID*)&gml_script_on_draw_gui
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DirectionalAttacks %s] - Failed to get script (gml_Script_move_ari)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_move_ari",
		gml_script_on_draw_gui->m_Functions->m_ScriptFunction,
		GmlScriptMoveAriCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DirectionalAttacks %s] - Failed to hook script (gml_Script_move_ari)!", VERSION);
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

	g_ModuleInterface->Print(CM_LIGHTAQUA, "[DirectionalAttacks %s] - Plugin starting...", VERSION);

	CreateHookGmlScriptFaceDir(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DirectionalAttacks %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptSetCardinal(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DirectionalAttacks %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptSetAnimation(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DirectionalAttacks %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	CreateHookGmlScriptMoveAri(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[DirectionalAttacks %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[DirectionalAttacks %s] - Plugin started!", VERSION);
	return AURIE_SUCCESS;
}
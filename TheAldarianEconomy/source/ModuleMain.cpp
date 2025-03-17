#include <map>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static const char* const VERSION = "1.0.0";
static const int MAX_DAYS_IN_LEDGER = 7; // The maximum number of days to include in the ledger.
/* 
DAILY THRESHOLDS
================
SOLD	PENALTY
50		5
75		10
100		20
125		40
150		80
Graph the following list of points: (50, 5), (75, 10), (100, 20), (125, 40), (150, 80)

WEEKLY THRESHOLDS
=================
250		5
275		10
300		20
325		40
350		80

Option 1:
Graph the following list of points: (250, 5), (275, 10), (300, 20), (325, 40), (350, 80)

Option 2:
Graph the following list of points: (200, 5), (225, 10), (250, 20), (275, 40), (300, 80)

Eliminated option:
Graph the following list of points: (200, 5), (250, 10), (300, 20), (325, 40), (350, 80)


225		5
250		10
275		20
300		40
325		80

*/

static YYTKInterface* g_ModuleInterface = nullptr;
static std::map<std::string, int> items_sold_ledger = {};

void GetItemsSoldInLastWeek()
{
	items_sold_ledger = {};

	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	RValue __game_stats = global_instance->at("__game_stats");
	RValue items_sold_each_day = __game_stats.at("items_sold_each_day");
	if (items_sold_each_day.m_Kind == VALUE_ARRAY)
	{
		size_t items_sold_each_day_size = 0;
		g_ModuleInterface->GetArraySize(items_sold_each_day, items_sold_each_day_size);
		if (items_sold_each_day_size > 0)
		{
			int start = items_sold_each_day_size - 1;
			int stop = start - 6;
			if (stop < 0)
				stop = 0;

			for (int i = start; i >= stop; i--)
			{
				RValue items_sold = g_ModuleInterface->CallBuiltin("array_get", { items_sold_each_day, i });
				if (items_sold.m_Kind == VALUE_ARRAY)
				{
					size_t items_sold_size = 0;
					g_ModuleInterface->GetArraySize(items_sold, items_sold_size);
					if (items_sold_size > 0)
					{
						for (int j = 0; j < items_sold_size; j++)
						{
							RValue item_sold = g_ModuleInterface->CallBuiltin("array_get", { items_sold, j });
							RValue count = item_sold.at("count");
							RValue item = item_sold.at("item");

							if (items_sold_ledger.count(item.AsString().data()) <= 0)
								items_sold_ledger[item.AsString().data()] = count.m_Real;
							else
								items_sold_ledger[item.AsString().data()] += count.m_Real;
						}
					}
				}
			}
		}
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

	if (!strstr(self->m_Object->m_Name, "obj_ari"))
		return;

	if (GetAsyncKeyState(VK_F2) & 1)
	{
		GetItemsSoldInLastWeek();
		int x = 0;
	}
}

RValue& GmlScriptSellShippingBinItemsCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	GetItemsSoldInLastWeek();
	int x = 0;

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, "gml_Script_sell_shipping_bin_items"));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	GetItemsSoldInLastWeek();
	int y = 0;

	if (Result.m_Kind == VALUE_OBJECT)
	{
		RValue items_sold = Result.at("items_sold"); // VALUE_OBJECT
		RValue gold_earned = Result.at("gold_earned"); // VALUE_REAL
		//g_ModuleInterface->EnumInstanceMembers(items_sold, EnumFunction);
	}

	return Result; 
}

void CreateHookGmlScriptSellShippingBinItems(AurieStatus& status)
{
	CScript* gml_script_sell_shipping_bin_items = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_sell_shipping_bin_items",
		(PVOID*)&gml_script_sell_shipping_bin_items
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[TheAldarianEconomy %s] - Failed to get script (gml_Script_sell_shipping_bin_items)!", VERSION);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		"gml_Script_sell_shipping_bin_items",
		gml_script_sell_shipping_bin_items->m_Functions->m_ScriptFunction,
		GmlScriptSellShippingBinItemsCallback,
		nullptr
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[TheAldarianEconomy %s] - Failed to hook script (gml_Script_sell_shipping_bin_items)!", VERSION);
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

	g_ModuleInterface->Print(CM_LIGHTYELLOW, "[TheAldarianEconomy %s] - Plugin starting...", VERSION);
	
	g_ModuleInterface->CreateCallback(
		g_ArSelfModule,
		EVENT_OBJECT_CALL,
		ObjectCallback,
		0
	);

	CreateHookGmlScriptSellShippingBinItems(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[TheAldarianEconomy %s] - Exiting due to failure on start!", VERSION);
		return status;
	}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[TheAldarianEconomy %s] - Plugin started!", VERSION);
	return AURIE_SUCCESS;
}
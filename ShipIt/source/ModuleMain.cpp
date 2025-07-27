#include <map>
#include <unordered_set>
#include <YYToolkit/Shared.hpp>
using namespace Aurie;
using namespace YYTK;

static const char* const MOD_NAME = "ShipIt";
static const char* const VERSION = "1.1.0";
static const char* const GML_SCRIPT_TRY_OBJECT_ID_TO_STRING = "gml_Script_try_object_id_to_string";
static const char* const GML_SCRIPT_INTERACT = "gml_Script_interact";
static const char* const GML_SCRIPT_PLAY_CONVERSATION = "gml_Script_play_conversation";
static const char* const GML_SCRIPT_PLAY_TEXT = "gml_Script_play_text@TextboxMenu@TextboxMenu";
static const char* const GML_SCRIPT_GET_LOCALIZER = "gml_Script_get@Localizer@Localizer";
static const char* const GML_SCRIPT_SETUP_MAIN_SCREEN = "gml_Script_setup_main_screen@TitleMenu@TitleMenu";
static const char* const GML_SCRIPT_SELL_SHIPPING_BIN_ITEMS = "gml_Script_sell_shipping_bin_items";
static const char* const GML_SCRIPT_INVENTORY_SLOT_POP = "gml_Script_pop@InventorySlot@Inventory";
static const char* const GML_SCRIPT_INVENTORY_SLOT_DRAIN = "gml_Script_drain@InventorySlot@Inventory";
static const char* const GML_SCRIPT_MODIFY_GOLD = "gml_Script_modify_gold@Ari@Ari";
static const char* const GML_SCRIPT_MODIFY_RENOWN = "gml_Script_modify_renown@Ari@Ari";
static const std::string CUSTOM_OBJECT_NAME = "balor_crystal_ball";
static const std::string SELL_SHIPPING_BIN_DIALOGUE_KEY = "Conversations/Mods/Sell Items/sell_shipping_bin_items/1";
static const std::string SELL_SHIPPING_BIN_CONVERSATION_KEY = "Conversations/Mods/Sell Items/sell_shipping_bin_items";
static const std::string ITEMS_SOLD_NOTIFICATION_KEY = "Notifications/Mods/Sell Items/items_sold";
static const std::string VALUE_PLACEHOLDER_TEXT = "<VALUE>";
static const std::string TRASH_ITEM_SOUND_EFFECT_NAME = "snd_UITrashItem";
static const std::string BACK_IN_VOGUE_PERK_NAME = "back_in_vogue";
static const std::string BACK_IN_VOGUE_TWO_PERK_NAME = "back_in_vogue_two";
static const std::string ARCHAEOLOGY = "archaeology";

static YYTKInterface* g_ModuleInterface = nullptr;
static bool mod_healthy = false;
static bool load_on_start = true;
static std::map<int, std::string> object_id_to_name_map = {};
static std::map<int, int> item_id_to_gold_value_map = {};
static std::map<int, int> item_id_to_renown_value_map = {};
static std::map<std::string, bool> active_perk_map = {};
static std::map<std::string, int64_t> perk_name_to_id_map = {};
static std::unordered_set<int> items_with_archaeology_tag = {};
static int gold_to_gain = 0;
static int renown_to_gain = 0;
static bool gain_gold_or_renown = false;
static bool custom_object_used = false;
static RValue custom_conversation_value;
static RValue* custom_conversation_value_ptr = nullptr;
static RValue gold_earned;
static RValue trash_item_sound_index; // The asset index for "snd_UITrashItem"


int RValueAsInt(RValue value)
{
	if (value.m_Kind == VALUE_REAL)
		return static_cast<int>(value.m_Real);
	if(value.m_Kind == VALUE_INT64)
		return static_cast<int>(value.m_i64);
	if (value.m_Kind == VALUE_INT32)
		return static_cast<int>(value.m_i32);
}

bool RValueAsBool(RValue value)
{
	if (value.m_Kind == VALUE_REAL && value.m_Real == 1)
		return true;
	if (value.m_Kind == VALUE_BOOL && value.m_Real == 1)
		return true;
	return false;
}

bool RValueIsNumeric(RValue value)
{
	if (value.m_Kind == VALUE_REAL || value.m_Kind == VALUE_INT64 || value.m_Kind == VALUE_INT32)
		return true;
	return false;
}

bool StructVariableExists(RValue the_struct, const char* variable_name)
{
	RValue struct_exists = g_ModuleInterface->CallBuiltin(
		"struct_exists",
		{ the_struct, variable_name }
	);

	return RValueAsBool(struct_exists);
}

RValue AssetGetIndex(std::string asset_name)
{
	RValue asset_index = g_ModuleInterface->CallBuiltin(
		"asset_get_index",
		{ asset_name }
	);
	return asset_index;
}

bool AudioIsPlaying(RValue sound_effect_index)
{
	RValue audio_is_playing = g_ModuleInterface->CallBuiltin(
		"audio_is_playing",
		{ sound_effect_index }
	);
	return RValueAsBool(audio_is_playing);
}

void ResetStaticFields()
{
	active_perk_map = {};
	gold_to_gain = 0;
	renown_to_gain = 0;
	gain_gold_or_renown = false;
	custom_object_used = false;
	custom_conversation_value = "";
	custom_conversation_value_ptr = nullptr;
	gold_earned = 0;
	trash_item_sound_index = AssetGetIndex(TRASH_ITEM_SOUND_EFFECT_NAME);
}

void LoadPerks()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	size_t array_length;
	RValue perks = global_instance->at("__perk__");
	g_ModuleInterface->GetArraySize(perks, array_length);
	for (size_t i = 0; i < array_length; i++)
	{
		RValue* array_element;
		g_ModuleInterface->GetArrayEntry(perks, i, array_element);
		perk_name_to_id_map[array_element->AsString().data()] = i;
	}
}

void LoadItemData()
{
	CInstance* global_instance = nullptr;
	g_ModuleInterface->GetGlobalInstance(&global_instance);

	RValue item_data = global_instance->at("__item_data");
	size_t item_data_length;
	g_ModuleInterface->GetArraySize(item_data, item_data_length);

	for (size_t i = 0; i < item_data_length; i++)
	{
		RValue* item;
		g_ModuleInterface->GetArrayEntry(item_data, i, item);
		RValue item_id = item->at("item_id");

		if (StructVariableExists(*item, "value"))
		{
			RValue value = item->at("value");
			if (StructVariableExists(value, "bin"))
			{
				RValue bin = value.at("bin");
				if(RValueIsNumeric(bin))
					item_id_to_gold_value_map[RValueAsInt(item_id)] = RValueAsInt(bin);
				else
					item_id_to_gold_value_map[RValueAsInt(item_id)] = 0;
			}

			if (StructVariableExists(value, "renown"))
			{
				RValue renown = value.at("renown");
				if (RValueIsNumeric(renown))
					item_id_to_renown_value_map[RValueAsInt(item_id)] = RValueAsInt(renown);
				else
					item_id_to_renown_value_map[RValueAsInt(item_id)] = 0;
			}
		}

		if (StructVariableExists(*item, "tags"))
		{
			RValue tags = item->at("tags");
			if (StructVariableExists(tags, "__buffer"))
			{
				RValue buffer = tags.at("__buffer");
				size_t buffer_length;
				g_ModuleInterface->GetArraySize(buffer, buffer_length);

				for (size_t j = 0; j < buffer_length; j++)
				{
					RValue* tag;
					g_ModuleInterface->GetArrayEntry(buffer, j, tag);

					if (tag->AsString().data() == ARCHAEOLOGY)
						items_with_archaeology_tag.insert(RValueAsInt(item_id));
				}
			}
		}
	}
}

void LoadObjectIds(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_try_object_id_to_string = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_TRY_OBJECT_ID_TO_STRING,
		(PVOID*)&gml_script_try_object_id_to_string
	);

	// NOTE: For some reason the __object_id__ global doesn't contain custom objects.
	for (int i = 0; i < 3000; i++)
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
			object_id_to_name_map[i] = result.AsString().data();
	}

	//CInstance* global_instance = nullptr;
	//g_ModuleInterface->GetGlobalInstance(&global_instance);

	//RValue __object_id__ = global_instance->at("__object_id__");
	//RValue array_length = g_ModuleInterface->CallBuiltin("array_length", { __object_id__ });
	//for (int i = 0; i < array_length.m_Real; i++)
	//{
	//	RValue object_name = g_ModuleInterface->CallBuiltin("array_get", { __object_id__, i });
	//	object_id_to_name_map[i] = object_name.AsString().data();
	//}

	g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Loaded %d objects!", MOD_NAME, VERSION, static_cast<int>(object_id_to_name_map.size()));
}

void DisplayNotification(CInstance* Self, CInstance* Other, std::string localization_key)
{
	CScript* gml_script_create_notification = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_create_notification",
		(PVOID*)&gml_script_create_notification
	);

	RValue result;
	RValue notification = localization_key;
	RValue* notification_ptr = &notification;
	gml_script_create_notification->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &notification_ptr }
	);
}

void SellShippingBinItems(CInstance* Self, CInstance* Other)
{
	CScript* gml_script_sell_shipping_bin_items = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_SELL_SHIPPING_BIN_ITEMS,
		(PVOID*)&gml_script_sell_shipping_bin_items
	);

	RValue result;
	gml_script_sell_shipping_bin_items->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		0,
		nullptr
	);

	gold_earned = result.at("gold_earned"); // VALUE_REAL
	g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Gained %d Tesserae from shipping items! Renown will be added at the end-of-day screen.", MOD_NAME, VERSION, static_cast<int>(RValueAsInt(gold_earned)));
	DisplayNotification(Self, Other, ITEMS_SOLD_NOTIFICATION_KEY);
}

void ModifyGold(CInstance* Self, CInstance* Other, int value)
{
	CScript* gml_script_modify_gold = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_MODIFY_GOLD,
		(PVOID*)&gml_script_modify_gold
	);

	RValue result;
	RValue gold_modifier = value;
	RValue* gold_modifier_ptr = &gold_modifier;

	gml_script_modify_gold->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &gold_modifier_ptr }
	);
}

void ModifyRenown(CInstance* Self, CInstance* Other, int value)
{
	CScript* gml_script_modify_renown = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_MODIFY_RENOWN,
		(PVOID*)&gml_script_modify_renown
	);

	RValue result;
	RValue renown_modifier = value;
	RValue* renown_modifier_ptr = &renown_modifier;

	gml_script_modify_renown->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &renown_modifier_ptr }
	);
}

bool PerkActive(CInstance* Self, CInstance* Other, std::string perk_name)
{
	int64_t perk_id = perk_name_to_id_map[perk_name];

	CScript* gml_script_perk_active = nullptr;
	g_ModuleInterface->GetNamedRoutinePointer(
		"gml_Script_perk_active@Ari@Ari",
		(PVOID*)&gml_script_perk_active
	);

	RValue result;
	RValue input = perk_id;
	RValue* input_ptr = &input;
	gml_script_perk_active->m_Functions->m_ScriptFunction(
		Self,
		Other,
		result,
		1,
		{ &input_ptr }
	);

	if (result.m_Kind == VALUE_BOOL && result.m_Real == 1.0)
		return true;
	return false;
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
		if (gain_gold_or_renown)
		{
			CInstance* global_instance = nullptr;
			g_ModuleInterface->GetGlobalInstance(&global_instance);

			if (gold_to_gain > 0)
			{
				ModifyGold(global_instance->at("__ari").m_Object, self, gold_to_gain);
				g_ModuleInterface->Print(CM_LIGHTGREEN, "[%s %s] - Gained %d Tesserae from trashing items!", MOD_NAME, VERSION, gold_to_gain);
				gold_to_gain = 0;
			}

			//if (renown_to_gain > 0)
			//{
			//	ModifyRenown(global_instance->at("__ari").m_Object, self, renown_to_gain);
			//	renown_to_gain = 0;
			//}

			gain_gold_or_renown = false;
		}

		// Track active perks.
		CInstance* global_instance = nullptr;
		g_ModuleInterface->GetGlobalInstance(&global_instance);

		if (PerkActive(global_instance->at("__ari").m_Object, self, BACK_IN_VOGUE_PERK_NAME))
			active_perk_map[BACK_IN_VOGUE_PERK_NAME] = true;
		else
			active_perk_map[BACK_IN_VOGUE_PERK_NAME] = false;
		if (PerkActive(global_instance->at("__ari").m_Object, self, BACK_IN_VOGUE_TWO_PERK_NAME))
			active_perk_map[BACK_IN_VOGUE_TWO_PERK_NAME] = true;
		else
			active_perk_map[BACK_IN_VOGUE_TWO_PERK_NAME] = false;
	}
}

// DEBUG
RValue& GmlScriptModifyGoldCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_MODIFY_GOLD));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

// DEBUG
RValue& GmlScriptModifyRenownCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_MODIFY_RENOWN));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	return Result;
}

RValue& GmlScriptInventorySlotPopCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	// Called when trashing a single item.
	// NOTE: The sound effect was debugged to be playing both before and after the actual script call.

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_INVENTORY_SLOT_POP));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (mod_healthy)
	{
		if (AudioIsPlaying(trash_item_sound_index))
		{
			if (Result.m_Kind == VALUE_OBJECT)
			{
				if (StructVariableExists(Result, "item_id"))
				{
					int item_id = RValueAsInt(Result.at("item_id"));
					if (item_id_to_gold_value_map.count(item_id) > 0)
					{
						double gold_value = item_id_to_gold_value_map[item_id];
						if (active_perk_map[BACK_IN_VOGUE_PERK_NAME] && items_with_archaeology_tag.count(item_id) > 0)
							gold_value *= 1.25;
						if (active_perk_map[BACK_IN_VOGUE_TWO_PERK_NAME] && items_with_archaeology_tag.count(item_id) > 0)
							gold_value *= 1.25;

						int gold_value_final = static_cast<int>(ceil(gold_value));
						gold_to_gain += gold_value_final;
					}
						
					//if (item_id_to_renown_value_map.count(item_id) > 0)
					//	renown_to_gain += item_id_to_renown_value_map[item_id];

					gold_earned = gold_to_gain;
					DisplayNotification(Self, Other, ITEMS_SOLD_NOTIFICATION_KEY);
					gain_gold_or_renown = true;
				}
			}
		}
	}

	return Result;
}

RValue& GmlScriptInventorySlotDrainCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	// Called when trashing multiple items (by holding SHIFT).
	// NOTE: The sound effect was debugged to be playing both before and after the actual script call.

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_INVENTORY_SLOT_DRAIN));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (mod_healthy)
	{
		if (AudioIsPlaying(trash_item_sound_index))
		{
			if (Result.m_Kind == VALUE_OBJECT)
			{
				if (StructVariableExists(Result, "__buffer"))
				{
					RValue buffer = Result.at("__buffer");
					size_t buffer_size;
					g_ModuleInterface->GetArraySize(buffer, buffer_size);

					for (size_t i = 0; i < buffer_size; i++)
					{
						RValue* element;
						g_ModuleInterface->GetArrayEntry(buffer, i, element);

						if (StructVariableExists(*element, "item_id"))
						{
							int item_id = RValueAsInt(element->at("item_id"));
							if (item_id_to_gold_value_map.count(item_id) > 0)
							{
								double gold_value = item_id_to_gold_value_map[item_id];
								if (active_perk_map[BACK_IN_VOGUE_PERK_NAME] && items_with_archaeology_tag.count(item_id) > 0)
									gold_value *= 1.25;
								if (active_perk_map[BACK_IN_VOGUE_TWO_PERK_NAME] && items_with_archaeology_tag.count(item_id) > 0)
									gold_value *= 1.25;

								int gold_value_final = static_cast<int>(ceil(gold_value));
								gold_to_gain += gold_value_final;
							}

							//if (item_id_to_renown_value_map.count(item_id) > 0)
							//	renown_to_gain += item_id_to_renown_value_map[item_id];
						}
					}

					gold_earned = gold_to_gain;
					DisplayNotification(Self, Other, ITEMS_SOLD_NOTIFICATION_KEY);
					gain_gold_or_renown = true;
				}
			}
		}
	}

	return Result;
}

RValue& GmlScriptInteractCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_healthy)
	{
		RValue object = Arguments[0]->m_Object;
		int object_id = RValueAsInt(object.at("object_id"));
		std::string object_name = object_id_to_name_map[object_id];

		if (object_name.find(CUSTOM_OBJECT_NAME) != std::string::npos)
			custom_object_used = true;
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

RValue& GmlScriptPlayConversationCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_healthy && custom_object_used)
	{
		custom_object_used = false;
		custom_conversation_value = SELL_SHIPPING_BIN_CONVERSATION_KEY;
		custom_conversation_value_ptr = &custom_conversation_value;
		Arguments[1] = custom_conversation_value_ptr;
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

RValue& GmlScriptGetLocalizerCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_GET_LOCALIZER));
	original(
		Self,
		Other,
		Result,
		ArgumentCount,
		Arguments
	);

	if (mod_healthy)
	{
		std::string localization_key = Arguments[0]->AsString().data();
		if (localization_key.find(ITEMS_SOLD_NOTIFICATION_KEY) != std::string::npos)
		{
			std::string result_str = Result.AsString().data();
			std::string value_str = std::to_string(RValueAsInt(gold_earned));

			// Replace the <VALUE> placeholder.
			size_t value_placeholder_index = result_str.find(VALUE_PLACEHOLDER_TEXT);
			if (value_placeholder_index != std::string::npos)
				result_str.replace(value_placeholder_index, VALUE_PLACEHOLDER_TEXT.length(), value_str);
			Result = result_str;
		}
	}

	return Result;
}

RValue& GmlScriptPlayTextCallback(
	IN CInstance* Self,
	IN CInstance* Other,
	OUT RValue& Result,
	IN int ArgumentCount,
	IN RValue** Arguments
)
{
	if (mod_healthy)
	{
		std::string conversation_name = Arguments[0]->AsString().data();
		if (conversation_name.find(SELL_SHIPPING_BIN_DIALOGUE_KEY) != std::string::npos)
		{
			SellShippingBinItems(Self, Other);
		}
	}

	const PFUNC_YYGMLScript original = reinterpret_cast<PFUNC_YYGMLScript>(MmGetHookTrampoline(g_ArSelfModule, GML_SCRIPT_PLAY_TEXT));
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
	ResetStaticFields();

	if (load_on_start)
	{
		load_on_start = false;
		LoadItemData();
		LoadPerks();
		LoadObjectIds(Self, Other);

		if (!object_id_to_name_map.empty())
			mod_healthy = true;
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
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook (EVENT_OBJECT_CALL)!", MOD_NAME, VERSION);
	}
}

// DEBUG
void CreateHookGmlScriptModifyGold(AurieStatus& status)
{
	CScript* gml_script = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_MODIFY_GOLD,
		(PVOID*)&gml_script
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_MODIFY_GOLD);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_MODIFY_GOLD,
		gml_script->m_Functions->m_ScriptFunction,
		GmlScriptModifyGoldCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_MODIFY_GOLD);
	}
}

// DEBUG
void CreateHookGmlScriptModifyRenown(AurieStatus& status)
{
	CScript* gml_script = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_MODIFY_RENOWN,
		(PVOID*)&gml_script
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_MODIFY_RENOWN);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_MODIFY_RENOWN,
		gml_script->m_Functions->m_ScriptFunction,
		GmlScriptModifyRenownCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_MODIFY_RENOWN);
	}
}

// DEBUG
void CreateHookGmlScriptInventorySlotPop(AurieStatus& status)
{
	CScript* gml_script = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_INVENTORY_SLOT_POP,
		(PVOID*)&gml_script
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_INVENTORY_SLOT_POP);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_INVENTORY_SLOT_POP,
		gml_script->m_Functions->m_ScriptFunction,
		GmlScriptInventorySlotPopCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_INVENTORY_SLOT_POP);
	}
}

// DEBUG
void CreateHookGmlScriptInventorySlotDrain(AurieStatus& status)
{
	CScript* gml_script = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_INVENTORY_SLOT_DRAIN,
		(PVOID*)&gml_script
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_INVENTORY_SLOT_DRAIN);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_INVENTORY_SLOT_DRAIN,
		gml_script->m_Functions->m_ScriptFunction,
		GmlScriptInventorySlotDrainCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_INVENTORY_SLOT_DRAIN);
	}
}

void CreateHookGmlScriptInteract(AurieStatus& status)
{
	CScript* gml_script_play_conversation = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_INTERACT,
		(PVOID*)&gml_script_play_conversation
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_INTERACT);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_INTERACT,
		gml_script_play_conversation->m_Functions->m_ScriptFunction,
		GmlScriptInteractCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_INTERACT);
	}
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

void CreateHookGmlScriptPlayText(AurieStatus& status)
{
	CScript* gml_script_play_text = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_PLAY_TEXT,
		(PVOID*)&gml_script_play_text
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_PLAY_TEXT);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_PLAY_TEXT,
		gml_script_play_text->m_Functions->m_ScriptFunction,
		GmlScriptPlayTextCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_PLAY_TEXT);
	}
}

void CreateHookGmlScriptGetLocalizer(AurieStatus& status)
{
	CScript* gml_script_get_localizer = nullptr;
	status = g_ModuleInterface->GetNamedRoutinePointer(
		GML_SCRIPT_GET_LOCALIZER,
		(PVOID*)&gml_script_get_localizer
	);

	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to get script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_LOCALIZER);
	}

	status = MmCreateHook(
		g_ArSelfModule,
		GML_SCRIPT_GET_LOCALIZER,
		gml_script_get_localizer->m_Functions->m_ScriptFunction,
		GmlScriptGetLocalizerCallback,
		nullptr
	);


	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Failed to hook script (%s)!", MOD_NAME, VERSION, GML_SCRIPT_GET_LOCALIZER);
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

	CreateHookGmlScriptInventorySlotDrain(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptInventorySlotPop(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	// DEBUG
	CreateHookGmlScriptModifyRenown(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	// DEBUG
	CreateHookGmlScriptModifyGold(status);
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

	CreateHookGmlScriptPlayConversation(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptPlayText(status);
	if (!AurieSuccess(status))
	{
		g_ModuleInterface->Print(CM_LIGHTRED, "[%s %s] - Exiting due to failure on start!", MOD_NAME, VERSION);
		return status;
	}

	CreateHookGmlScriptGetLocalizer(status);
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
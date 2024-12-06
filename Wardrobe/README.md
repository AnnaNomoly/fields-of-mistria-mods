# Wardrobe
A YYTK plugin for FieldsOfMistria that allows you to **unlock any cosmetic** in the game, including **modded ones**. This works on __existing__ saves!

## Instructions
1. Press `F9` to open the input dialog where you can specify one of the **cosmetic names** from the table below.
> **NOTE**: The cosmetic name must be formatted *exactly* as shown in the table below!

2. The mod will output some information to the YYTK console window. If the cosmetic was **INVALID** you will get a warning message. If the cosmetic is **ALREADY UNLOCKED** you will get a different warning message. If the cosmetic is **NEW** you will get a success message and unlock the cosmetic.
> **NOTE**: There will NOT be a "new cosmetic unlocked" window pop-up. Simply check the character customization menu.

## Modded Cosmetics
Modded cosmetics are possible to unlock using this mod. However, you will need to look up their "internal name" in order to do so. This will be found _inside_ the modded cosmetic's `.../<Mod>/outfits/`folder within the `.../FieldsOfMistria/mods/` directory. For example, assuming you installed the `Cute Ribbon` mod, the unzipped mod has the following structure:
```
Cute Ribbon
├─ images
├─ outfits
│  ├─ cute ribbon.json
├─ sprites
├─ stores
├─ manifest.json
```

The `cute ribbon.json` file in the `outfits` folder will contain the "internal name" for the modded items you will need. For the `Cute Ribbon` mod, the content of the JSON file is below for reference. The modded cosmetic's internal name is `cute_ribbon`.
```
{
  "cute_ribbon": {
    "name": "Cute Ribbon",
    "description": "A ribbon that is cute!",
    "ui_slot": "head_gear",
    "default_unlocked": true,
    "ui_sub_category": "head_gear_misc",
    "lutFile": "images/cuteribbon_lut.png",
    "uiItem": "images/cuteribbon_ui.png",
    "outlineFile": "images/cuteribbon_outline.png",
    "animationFiles": {
      "head_gear": "images/cuteribbon"
    }
  }
}
```

## Cosmetic Table (FoM v0.12.1)
| Cosmetic Name |
| ----------- |
| back_gear_basic_backpack |
| back_gear_basic_cape |
| back_gear_dragon_cleric_cape |
| back_gear_noble_cape |
| beard_basic |
| beard_thick |
| dress_maid |
| dress_modest |
| dress_sleeveless_basic |
| dress_spring_festival |
| eyes_closed |
| eyes_default |
| eyes_droopy |
| eyes_eyelashes |
| eyes_serious |
| eyes_small |
| face_gear_blush |
| face_gear_caldosian_moon_earrings |
| face_gear_dragon_cleric_earrings |
| face_gear_face_mask |
| face_gear_flower_earrings |
| face_gear_glasses |
| face_gear_hoop_earrings |
| face_gear_pumpkin_earrings |
| face_gear_reading_glasses |
| face_gear_reading_glasses_small |
| face_gear_stud_earrings |
| face_gear_sunglasses |
| facial_hair_handlebar_moustache |
| facial_hair_moustache |
| facial_hair_short_beard |
| facial_hair_soul_patch |
| facial_hair_stubble |
| hair_afro |
| hair_afro_puffs |
| hair_blown_out_twist |
| hair_braided_pigtails |
| hair_buzzcut |
| hair_cowlick |
| hair_curly_high_top |
| hair_curly_medium |
| hair_curly_medium_fringe |
| hair_curly_pompadour |
| hair_curly_shaved_side |
| hair_curly_short |
| hair_curly_shorter |
| hair_curly_updo |
| hair_dreadlock_twin_buns |
| hair_dreadlocks_bun |
| hair_dreadlocks_long |
| hair_fade |
| hair_medium_boyish |
| hair_medium_half_bun |
| hair_medium_half_bun_fringe |
| hair_medium_pigtails |
| hair_mohawk |
| hair_mushroom |
| hair_parted_mushroom |
| hair_pixie |
| hair_rounded_afro |
| hair_shaggy_bob |
| hair_short_parted_curls |
| hair_short_parted_straight |
| hair_straight_buns_fringed |
| hair_straight_long |
| hair_straight_long_bun |
| hair_straight_long_fringed |
| hair_straight_long_tucked |
| hair_straight_long_twin_buns |
| hair_straight_medium |
| hair_straight_medium_ponytail |
| hair_straight_medium_ponytail_fluffy_bangs |
| hair_straight_pompadour |
| hair_straight_shaved_side |
| hair_textured_buzzcut |
| hair_wavy_long |
| hair_wavy_long_fringed |
| hair_wavy_long_ponytail |
| hair_wavy_long_twin_buns |
| hair_wavy_medium |
| hair_wavy_medium_fringed |
| hair_wavy_medium_twin_buns |
| hair_wavy_rugged |
| head_adventurer_hood |
| head_aviator_goggles |
| head_bandana |
| head_barrel_roller_hat |
| head_baseball_cap |
| head_beekeeper_hat |
| head_beret |
| head_caldosian_hat |
| head_cap_basic |
| head_cat_ears |
| head_circlet_earth |
| head_clips_angel_wing |
| head_clips_bat_wing |
| head_clips_moon |
| head_clips_star |
| head_clips_strawberry |
| head_cloth_helmet |
| head_copper_helmet |
| head_cowboy_hat_basic |
| head_crown_tide |
| head_crystal_helmet |
| head_devil_horns |
| head_dragon_cleric_diadem |
| head_enchantern_hat |
| head_essence_bat_hat |
| head_explorer_hat |
| head_farmer_cap |
| head_fighter_headband |
| head_fishing_hat |
| head_flower_crown |
| head_flower_hat |
| head_flower_top_hat |
| head_halo |
| head_headband_basic |
| head_hijab |
| head_iron_helmet |
| head_lily_pad |
| head_mimic_hat |
| head_miners_helmet |
| head_mini_horns |
| head_mushroom_hat |
| head_noble_hat |
| head_oreclod_hat |
| head_paisley_bandana |
| head_pumpkin_beanie |
| head_rain_hat |
| head_ranger_leaves |
| head_ribbons_basic |
| head_rockclod_hat |
| head_sapling_hat |
| head_scrap_metal_helmet |
| head_short_bow |
| head_silver_helmet |
| head_soup_pot |
| head_special_flower_crown |
| head_sprout_hat |
| head_stalagmite_hat |
| head_striped_bow |
| head_striped_bucket_hat |
| head_sunhat_straw |
| head_tangerine |
| head_teapot |
| head_verdigris_helmet |
| head_witch_hat |
| overalls_basic |
| overalls_farmer |
| overalls_shorts |
| overalls_skirt |
| pants_adventurer |
| pants_basic |
| pants_beekeeper |
| pants_cloth_armor |
| pants_copper_armor |
| pants_crystal_armor |
| pants_explorer |
| pants_iron_armor |
| pants_scrap_metal |
| pants_silver_armor |
| pants_verdigris_armor |
| robe_dragon_cleric |
| robe_earth |
| robe_tide |
| shoes_boots |
| shoes_boots_adventurer |
| shoes_boots_beekeeper |
| shoes_boots_caldosian |
| shoes_boots_cloth_armor |
| shoes_boots_copper_armor |
| shoes_boots_crystal_armor |
| shoes_boots_dragon_cleric |
| shoes_boots_explorer |
| shoes_boots_farmer |
| shoes_boots_fishing |
| shoes_boots_iron_armor |
| shoes_boots_miner |
| shoes_boots_noble |
| shoes_boots_pirate |
| shoes_boots_ranger |
| shoes_boots_scrap_metal |
| shoes_boots_short_basic |
| shoes_boots_silver_armor |
| shoes_boots_verdigris_armor |
| shoes_boots_work |
| shoes_dressy_stockings |
| shoes_sandals_earth |
| shoes_sandals_tide |
| shoes_sandals_worn |
| shoes_sneakers_basic |
| shorts_basic |
| shorts_fishing |
| skirt_long_basic |
| skirt_long_scalloped |
| skirt_maid |
| skirt_pleated_medium |
| skirt_pleated_short |
| skirt_short_basic |
| suit_miner |
| suit_spring_festival |
| top_adventurer |
| top_caldosian_overcoat |
| top_cami |
| top_cloth_armor |
| top_copper_armor |
| top_crystal_armor |
| top_explorer |
| top_fishing_vest |
| top_iron_armor |
| top_jacket_and_tee_basic |
| top_long_sleeve_baseball |
| top_long_sleeve_basic |
| top_long_sleeve_basic_cropped |
| top_long_sleeve_beekeeper |
| top_long_sleeve_striped |
| top_noble_jacket |
| top_noble_tunic |
| top_plaid_flannel |
| top_ranger |
| top_ranger_jacket |
| top_scrap_metal |
| top_silver_armor |
| top_tanktop |
| top_tanktop_buttons |
| top_tee_basic |
| top_tee_oversized |
| top_tee_ringer_basic |
| top_verdigris_armor |
| underwear_bottoms_polkadot |
| underwear_bra |
| underwear_bra_small |
| underwear_briefs |
| underwear_shorts |
| underwear_tank |

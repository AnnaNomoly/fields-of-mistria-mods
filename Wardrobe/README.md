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

## Cosmetic Table (FoM v0.14.4)
| Internal Cosmetic Name | Displayed Cosmetic Name |
| ---------------------- | ----------------------- |
| back_gear_ancient_witch_broom | Ancient Witch Broom |
| back_gear_basic_backpack | Basic Backpack |
| back_gear_basic_cape | Basic Cape |
| back_gear_cabbage_backpack | Cabbage Backpack |
| back_gear_cauliflower_backpack | Cauliflower Backpack |
| back_gear_cloud_backpack | Cloud Backpack |
| back_gear_daikon_radish_backpack | Daikon Radish Backpack |
| back_gear_daisy_backpack | Daisy Backpack |
| back_gear_dragon_cleric_cape | Dragon Cleric Cape |
| back_gear_exalted_cloak | Exalted Cloak |
| back_gear_fire_cloak | Fire Sage Cloak |
| back_gear_frost_lily_backpack | Frost Lily Backpack |
| back_gear_jasmine_backpack | Jasmine Backpack |
| back_gear_mini_wings | Mini Wings |
| back_gear_noble_cape | Noble Cape |
| back_gear_onion_backpack | Onion Backpack |
| back_gear_ornate_round_shield | Ornate Round Shield |
| back_gear_ornate_shield | Ornate Shield |
| back_gear_pumpkin_backpack | Pumpkin Backpack |
| back_gear_rogue_cape | Rogue's Cape |
| back_gear_sheathed_sword | Sheathed Sword |
| back_gear_snowdrop_anemone_backpack | Snowdrop Anemone Backpack |
| back_gear_strawberry_backpack | Strawberry Backpack |
| back_gear_sunflower_backpack | Sunflower Backpack |
| back_gear_sweet_potato_backpack | Sweet Potato Backpack |
| back_gear_tomato_backpack | Tomato Backpack |
| back_gear_turnip_backpack | Turnip Backpack |
| back_gear_watermelon_backpack | Watermelon Backpack |
| back_gear_wheat_backpack | Wheat Backpack |
| beard_basic | Basic Beard |
| beard_thick | Thick Beard |
| dress_berry | Berry Dress |
| dress_court | Court Dress |
| dress_maid | Maid Dress |
| dress_modest | Long Sleeve Dress |
| dress_sleeveless_basic | Sleeveless Dress |
| dress_spring_festival | Spring Festival Dress |
| eyes_closed | Closed Eyes |
| eyes_default | Basic Eyes |
| eyes_droopy | Droopy Eyes |
| eyes_eyelashes | Eyelashes |
| eyes_serious | Serious Eyes |
| eyes_small | Small Eyes |
| face_gear_blush | Blush Face |
| face_gear_caldosian_moon_earrings | Caldosian Traveler's Earrings |
| face_gear_dragon_cleric_earrings | Dragon Cleric Eye Earrings |
| face_gear_face_mask | Face Mask |
| face_gear_flower_earrings | Flower Earrings |
| face_gear_glasses | Glasses |
| face_gear_hoop_earrings | Hoop Earrings |
| face_gear_pumpkin_earrings | Pumpkin Earrings |
| face_gear_reading_glasses | Reading Glasses |
| face_gear_reading_glasses_small | Small Reading Glasses |
| face_gear_stud_earrings | Stud Earrings |
| face_gear_sunglasses | Sunglasses |
| facial_hair_handlebar_moustache | Handlebar Moustache |
| facial_hair_moustache | Moustache |
| facial_hair_short_beard | Short Beard |
| facial_hair_soul_patch | Soul Patch |
| facial_hair_stubble | Stubble |
| hair_afro | Afro |
| hair_afro_puffs | Afro Puffs |
| hair_blown_out_twist | Blown Out Twist |
| hair_braided_pigtails | Braided Pigtails |
| hair_buzzcut | Buzzcut |
| hair_cowlick | Cowlick |
| hair_curly_high_top | Curly High Top |
| hair_curly_medium | Curly Medium |
| hair_curly_medium_fringe | Curly Medium Fringe |
| hair_curly_pompadour | Curly Pompadour |
| hair_curly_shaved_side | Curly Undercut |
| hair_curly_short | Curly Short |
| hair_curly_shorter | Curly Shorter |
| hair_curly_updo | Curly Updo |
| hair_dreadlock_twin_buns | Dreadlock Twin Buns |
| hair_dreadlocks_bun | Dreads Bun |
| hair_dreadlocks_long | Long Dreadlocks |
| hair_fade | Fade |
| hair_medium_boyish | Medium Length |
| hair_medium_half_bun | Medium Half Bun |
| hair_medium_half_bun_fringe | Medium Half Bun Fringe |
| hair_medium_pigtails | Medium Pigtails |
| hair_mohawk | Mohawk |
| hair_mushroom | Mushroom Cut |
| hair_parted_mushroom | Parted Mushroom Cut |
| hair_pixie | Pixie Cut |
| hair_rounded_afro | Rounded Afro |
| hair_shaggy_bob | Shaggy Bob |
| hair_short_parted_curls | Short Parted Curls |
| hair_short_parted_straight | Short Parted Straight |
| hair_straight_buns_fringed | Straight Buns Fringed |
| hair_straight_long | Straight Long |
| hair_straight_long_bun | Straight Long Bun |
| hair_straight_long_fringed | Straight Long Fringe |
| hair_straight_long_tucked | Straight Long Tucked |
| hair_straight_long_twin_buns | Straight Long Twin Buns |
| hair_straight_medium | Straight Medium |
| hair_straight_medium_ponytail | Straight Medium Ponytail |
| hair_straight_medium_ponytail_fluffy_bangs | Straight Medium Fluffy Bangs Ponytail |
| hair_straight_pompadour | Straight Pompadour |
| hair_straight_shaved_side | Straight Undercut |
| hair_textured_buzzcut | Textured Buzz |
| hair_wavy_long | Wavy Long |
| hair_wavy_long_fringed | Wavy Long Fringe |
| hair_wavy_long_ponytail | Wavy Long Ponytail |
| hair_wavy_long_twin_buns | Wavy Long Twin Buns |
| hair_wavy_medium | Wavy Medium |
| hair_wavy_medium_fringed | Wavy Medium Fringe |
| hair_wavy_medium_twin_buns | Wavy Medium Twin Buns |
| hair_wavy_rugged | Wavy Rugged |
| head_adventurer_hood | Adventurer's Hood |
| head_ancient_witch_tiara | Ancient Witch Tiara |
| head_aviator_goggles | Aviator Goggles |
| head_bandana | Bandana |
| head_barrel_roller_hat | Barrel Roller Hat |
| head_baseball_cap | Baseball Cap |
| head_beekeeper_hat | Beekeeper's Hat |
| head_beret | Beret |
| head_berry_crown | Berry Crown |
| head_berry_hat | Berry Hat |
| head_caldosian_hat | Caldosian Traveler's Hat |
| head_cap_basic | Cap |
| head_cat_ears | Cat Ears |
| head_circlet_earth | Sage of Earth Gem Circlet |
| head_clips_angel_wing | Angel Wing Hair Clips |
| head_clips_bat_wing | Bat Wing Hair Clips |
| head_clips_heart | Heart Hair Clips |
| head_clips_moon | Moon Hair Clips |
| head_clips_star | Star Hair Clips |
| head_clips_strawberry | Strawberry Hair Clips |
| head_cloth_helmet | Cloth Helmet |
| head_copper_helmet | Copper Helmet |
| head_cowboy_hat_basic | Cowboy Hat |
| head_crown | Crown |
| head_crown_tide | Crown of the Tide |
| head_crystal_helmet | Crystal Helmet |
| head_dancer_flower | Dancer's Flower |
| head_devil_horns | Devil Horns |
| head_dragon_cleric_diadem | Dragon Cleric Horned Diadem |
| head_enchantern_hat | Mini Lantern Hat |
| head_essence_bat_hat | Essence Bat Hat |
| head_exalted_crown | Exalted Crown |
| head_explorer_hat | Explorer's Hat |
| head_farmer_cap | Farmer's Cap |
| head_fighter_headband | Fighter Headband |
| head_fire_hood | Fire Sage Hood |
| head_fishing_hat | Fishing Hat |
| head_flame_spirit_hat | Flame Spirit Hat |
| head_floating_cloud_headpiece | Floating Cloud Headpiece |
| head_flower_crown | Flower Crown |
| head_flower_hat | Flower Hat |
| head_flower_top_hat | Flower Top Hat |
| head_gold_helmet | Gold Helmet |
| head_halo | Halo |
| head_headband_basic | Headband |
| head_hijab | Hijab |
| head_iron_helmet | Iron Helmet |
| head_lava_cat_hat | Lava Cat Hat |
| head_lily_pad | Lily Pad Hat |
| head_mimic_hat | Mimic Hat |
| head_miners_helmet | Miner's Helmet |
| head_mini_horns | Mini Horns |
| head_mushroom_hat | Mushroom Cap Hat |
| head_noble_hat | Noble Hat |
| head_oreclod_hat | Oreclod Hat |
| head_paisley_bandana | Paisley Bandana |
| head_pumpkin_beanie | Pumpkin Beanie |
| head_rain_hat | Rain Hat |
| head_ranger_leaves | Ranger's Leaf Hair Clips |
| head_ribbons_basic | Ribbons |
| head_rock_stack_hat | Rock Stack Hat |
| head_rockclod_hat | Rockclod Hat |
| head_rogue_hood | Rogue's Hood |
| head_sapling_hat | Sapling Hat |
| head_scrap_metal_helmet | Scrap Metal Helmet |
| head_short_bow | Short Bow |
| head_silver_helmet | Silver Helmet |
| head_soup_pot | Soup Pot Hat |
| head_special_flower_crown | Special Flower Crown |
| head_sprout_hat | Sprout Hat |
| head_stalagmite_hat | Stalagmite Hat |
| head_striped_bow | Striped Bow |
| head_striped_bucket_hat | Striped Bucket Hat |
| head_sunhat_straw | Straw Sunhat |
| head_tangerine | Tangerine Hat |
| head_tarnished_gold_helmet | Tarnished Gold Helmet |
| head_teapot | Teapot Hat |
| head_verdigris_helmet | Verdigris Helmet |
| head_witch_hat | Witch Hat |
| overalls_basic | Overalls |
| overalls_farmer | Farmer's Overalls |
| overalls_shorts | Overall Shorts |
| overalls_skirt | Overall Skirt |
| pants_adventurer | Adventurer's Pants |
| pants_basic | Pants |
| pants_beekeeper | Beekeeper's Pants |
| pants_cloth_armor | Cloth Pants |
| pants_copper_armor | Copper Legplates |
| pants_crystal_armor | Crystal Tassets |
| pants_exalted | Exalted Pants |
| pants_explorer | Explorer's Pants |
| pants_gold_armor | Gold Legplates |
| pants_iron_armor | Iron Legplates |
| pants_rogue | Rogue's Pants |
| pants_scrap_metal | Scrap Metal Pants |
| pants_silver_armor | Silver Legplates |
| pants_tarnished_gold_armor | Tarnished Gold Legplates |
| pants_verdigris_armor | Verdigris Tassets |
| robe_dragon_cleric | Dragon Cleric Robes |
| robe_earth | Sage of Earth Robe |
| robe_fire | Fire Sage Robe |
| robe_tide | Robe of the Waves |
| shoes_boots | Boots |
| shoes_boots_adventurer | Adventurer's Boots |
| shoes_boots_ancient_witch | Ancient Witch Boots |
| shoes_boots_beekeeper | Beekeeper's Boots |
| shoes_boots_caldosian | Caldosian Traveler's High Boots |
| shoes_boots_cloth_armor | Cloth Shoes |
| shoes_boots_copper_armor | Copper Greaves |
| shoes_boots_crystal_armor | Crystal Boots |
| shoes_boots_dragon_cleric | Dragon Cleric Clawed Boots |
| shoes_boots_exalted | Exalted Boots |
| shoes_boots_explorer | Explorer's Boots |
| shoes_boots_farmer | Farmer's Boots |
| shoes_boots_fire | Fire Sage Boots |
| shoes_boots_fishing | Fishing Boots |
| shoes_boots_gold_armor | Gold Greaves |
| shoes_boots_iron_armor | Iron Greaves |
| shoes_boots_miner | Miner's Boots |
| shoes_boots_noble | Noble Boots |
| shoes_boots_pirate | Pirate Boots |
| shoes_boots_ranger | Ranger's Boots |
| shoes_boots_rogue | Rogue's Boots |
| shoes_boots_scrap_metal | Scrap Metal Boots |
| shoes_boots_short_basic | Short Boots |
| shoes_boots_silver_armor | Silver Greaves |
| shoes_boots_tarnished_gold_armor | Tarnished Gold Greaves |
| shoes_boots_verdigris_armor | Verdigris Boots |
| shoes_boots_work | Work Boots |
| shoes_dressy_stockings | Dressy Stockings |
| shoes_sandals_earth | Sage of Earth Sandals |
| shoes_sandals_tide | Sandals of the Water |
| shoes_sandals_worn | Worn Sandals |
| shoes_sneakers_basic | Sneakers |
| shorts_basic | Shorts |
| shorts_fishing | Fishing Shorts |
| skirt_ancient_witch | Ancient Witch Skirt |
| skirt_dancer | Dancer's Skirt |
| skirt_long_basic | Long Skirt |
| skirt_long_scalloped | Scalloped Skirt |
| skirt_maid | Maid Skirt |
| skirt_pleated_medium | Long Pleated Skirt |
| skirt_pleated_short | Short Pleated Skirt |
| skirt_short_basic | Short Skirt |
| suit_berry | Berry Suit |
| suit_court | Court Suit |
| suit_miner | Miner's Jumpsuit |
| suit_spring_festival | Spring Festival Suit |
| top_adventurer | Adventurer's Jerkin |
| top_ancient_witch | Ancient Witch Top |
| top_caldosian_overcoat | Caldosian Traveler's Overcoat |
| top_cami | Cami |
| top_cloth_armor | Cloth Chestpiece |
| top_copper_armor | Copper Armor |
| top_crystal_armor | Crystal Chestpiece |
| top_dancer | Dancer's Jacket |
| top_exalted | Exalted Top |
| top_explorer | Explorer's Jacket |
| top_fishing_vest | Fishing Vest |
| top_gold_armor | Gold Armor |
| top_iron_armor | Iron Armor |
| top_jacket_and_tee_basic | Jacket and Tee |
| top_long_sleeve_baseball | Baseball Shirt |
| top_long_sleeve_basic | Long Sleeve |
| top_long_sleeve_basic_cropped | Cropped Long Sleeve |
| top_long_sleeve_beekeeper | Beekeeper's Shirt |
| top_long_sleeve_striped | Striped Long Sleeve |
| top_noble_jacket | Noble Jacket |
| top_noble_tunic | Noble Tunic |
| top_plaid_flannel | Plaid Flannel |
| top_ranger | Ranger's Short Sleeve Top |
| top_ranger_jacket | Ranger's Jacket |
| top_rogue | Rogue's Top |
| top_scrap_metal | Scrap Metal Chestpiece |
| top_silver_armor | Silver Armor |
| top_tanktop | Tank Top |
| top_tanktop_buttons | Buttoned Tanktop |
| top_tarnished_gold_armor | Tarnished Gold Armor |
| top_tee_basic | T-Shirt |
| top_tee_oversized | Oversized Tee Outfit |
| top_tee_ringer_basic | Ringer Tee |
| top_verdigris_armor | Verdigris Chestpiece |
| underwear_bottoms_polkadot | Polkadot Underwear |
| underwear_bra | Bra |
| underwear_bra_small | Small Bra |
| underwear_briefs | Briefs |
| underwear_shorts | Boxers |
| underwear_tank | Thin Strap Tank Top |

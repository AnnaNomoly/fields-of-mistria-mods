# DigUpAnything
A YYTK plugin for Fields Of Mistria that allows you to modify what **item** Ari will dig up from the next **artifact spot**.

## Instructions
1. Press `F11` to open the input dialog where you can specify one of the **item names** from the table below.

> **NOTE**: The item name must be formatted *exactly* as shown in the table below!

2. Dig up an artifact spot! The item uncovered will be what you input. Any subsequent artifact spots will behave as normal until you repeat step #1.

## About "Internal Item Names"
Items in Fields of Mistria have what I generally refer to as an "internal name" that uniquely identifies them. Unlike their ID (number) that CAN change with each update, the internal name is NOT meant to change, and is therefore an easy way for the developers and us to identify an item.

### Looking Up Internal Names
> **NOTE**: The table below now lists both the *internal* item name AND the *displayed* item name in game.

You may know the name of an item you wish to obtain using this mod, but don't see it in the list below. That's because the *actual* (displayed) item name is NOT always the same as its *internal* name. Let's use `Shiny Bead`, the item animals reward you with, as an example. In the list below we see I made a note indicating that the *internal* item name for it is actually `animal_currency`.

But how did I figure that out? By looking in the game's **localization** file: `localization.json`. This JSON file contains all of the game's translated text for items, conversations, and more! By searching for `"Shiny Bead"` in the file I arrived at the line shown below:
```
"items/other/ranching/animal_currency/name": "Shiny Bead",
```

The `animal_currency` portion of that text is the item's *internal name*. If you check other files for the game, such as the `fiddle.json`, you will also see the *internal names* used as well!


## Item Table (FoM v0.15.0)
* Anything marked **DISABLED** is not supported, as it would crash the game when trying to create the item.
* Anything marked **GLITCHED** is a glitched item that is only partially implemented. It won't crash the game, but is buggy.
* The `unknown_item` is marked **PLACEHOLDER** as it represents the generic item used when a mod is removed from the game and an unrecognized item is loaded.

| Item ID  | Internal Item Name | Displayed Item Name | Notes |
| -------- | ------------------ | ------------------- | ----- |
| 0 | abyssal_chest | Abyssal Chest |
| 1 | acorn | Acorn |
| 2 | adept_kitchen | Level 3 Kitchen |
| 3 | alda_bronze_sword | Alda Bronze Sword |
| 4 | alda_clay_pot | Alda Clay Pot |
| 5 | alda_feather_pendant | Alda Feather Pendant |
| 6 | alda_gem_bracelet | Alda Gem Bracelet |
| 7 | alda_mural_tablet | Alda Mural Tablet |
| 12 | alda_pot_replica | Large Alda Pot |
| 13 | aldarian_gauntlet | Aldarian Gauntlet |
| 14 | aldarian_sword | Aldarian Sword |
| 15 | aldarian_war_banner | Aldarian War Banner |
| 16 | alligator_gar | Alligator Gar |
| 17 | alpaca_plushie | Alpaca Plushie |
| 18 | alpaca_wool | Alpaca Wool |
| 19 | amber_trapped_insect | Amber Trapped Insect |
| 20 | amberjack | Amberjack |
| 21 | anchovy | Anchovy |
| 22 | ancient_crystal_goblet | Ancient Crystal Goblet |
| 23 | ancient_firefly | Ancient Firefly |
| 24 | ancient_gold_coin | Ancient Gold Coin |
| 25 | ancient_horn_circlet | Ancient Horn Circlet |
| 26 | ancient_royal_scepter | Ancient Royal Scepter |
| 27 | ancient_stone_lantern | Ancient Stone Lantern |
| 28 | angel_fish | Angel Fish |
| 29 | animal_cosmetic | MISSING | **DISABLED** |
| 30 | animal_currency | Shiny Bead |
| 31 | animated_bird_fountain | Animated Bird Fountain |
| 32 | animated_snow_globe | Animated Snow Globe |
| 34 | animated_waterfall | Animated Waterfall |
| 35 | ant | Ant |
| 36 | ant_queen | Ant Queen |
| 37 | apiary | Apiary |
| 38 | apple | Apple |
| 39 | apple_honey_curry | Apple Honey Curry |
| 40 | apple_juice | Apple Juice |
| 41 | apple_pie | Apple Pie |
| 42 | archerfish | Archerfish |
| 43 | armored_bass | Armored Bass |
| 44 | artifact_replica_alda_bronze_sword | Replica Alda Bronze Sword |
| 45 | artifact_replica_alda_clay_pot | Replica Alda Clay Pot |
| 46 | artifact_replica_alda_feather_pendant | Replica Alda Feather Pendant |
| 47 | artifact_replica_alda_gem_bracelet | Replica Alda Gem Bracelet |
| 48 | artifact_replica_alda_mural_tablet | Replica Alda Mural Tablet |
| 49 | artifact_replica_aldarian_gauntlet | Replica Aldarian Gauntlet |
| 50 | artifact_replica_aldarian_sword | Replica Aldarian Sword |
| 51 | artifact_replica_aldarian_war_banner | Replica Aldarian War Banner |
| 52 | artifact_replica_amber_trapped_insect | Replica Amber Trapped Insect |
| 53 | artifact_replica_ancient_crystal_goblet | Replica Ancient Crystal Goblet |
| 54 | artifact_replica_ancient_gold_coin | Replica Ancient Gold Coin |
| 55 | artifact_replica_ancient_horn_circlet | Replica Ancient Horn Circlet |
| 56 | artifact_replica_ancient_royal_scepter | Replica Ancient Royal Scepter |
| 57 | artifact_replica_ancient_stone_lantern | Replica Ancient Stone Lantern |
| 58 | artifact_replica_black_tablet | Replica Black Tablet |
| 59 | artifact_replica_caldosian_breastplate | Replica Caldosian Breastplate |
| 60 | artifact_replica_caldosian_drinking_horn | Replica Caldosian Drinking Horn |
| 61 | artifact_replica_caldosian_emperor_bust | Replica Caldosian Emperor Bust |
| 62 | artifact_replica_caldosian_sword | Replica Caldosian Sword |
| 63 | artifact_replica_clay_amphora | Replica Clay Amphora |
| 64 | artifact_replica_coin_lump | Replica Coin Lump |
| 65 | artifact_replica_completely_wrong_map | Replica Completely Wrong Map |
| 66 | artifact_replica_crab_statue | Replica Crab Statue |
| 67 | artifact_replica_criminal_confession | Replica Criminal Confession |
| 68 | artifact_replica_crystal_apple | Replica Crystal Apple |
| 69 | artifact_replica_dense_water | Replica Dense Water |
| 70 | artifact_replica_diamond_backed_mirror | Replica Diamond-Backed Mirror |
| 71 | artifact_replica_dragon_claw | Replica Dragon Claw |
| 72 | artifact_replica_dragon_forged_bracelet | Replica Dragon-Forged Bracelet |
| 73 | artifact_replica_dragon_pact_tablet | Replica Dragon Pact Tablet |
| 74 | artifact_replica_dragon_scale | Replica Dragon Scale |
| 75 | artifact_replica_earth_infused_stone | Replica Earth-Infused Stone |
| 76 | artifact_replica_faceted_rock_gem | Replica Faceted Rock Gem |
| 77 | artifact_replica_family_crest_pendant | Replica Family Crest Pendant |
| 78 | artifact_replica_fire_crystal | Replica Fire Crystal |
| 79 | artifact_replica_flint_arrowhead | Replica Flint Arrowhead |
| 80 | artifact_replica_fossilized_egg | Replica Fossilized Egg |
| 81 | artifact_replica_fossilized_mandrake_root | Replica Fossilized Mandrake Root |
| 82 | artifact_replica_gathering_basket | Replica Gathering Basket |
| 83 | artifact_replica_giant_fish_scale | Replica Giant Fish Scale |
| 84 | artifact_replica_hardened_essence | Replica Hardened Essence |
| 85 | artifact_replica_lost_crown_of_aldaria | Replica Lost Crown of Aldaria |
| 86 | artifact_replica_mermaids_comb | Replica Mermaid's Comb |
| 87 | artifact_replica_message_in_a_bottle | Replica Message in a Bottle |
| 88 | artifact_replica_metal_leaf | Replica Metal Leaf |
| 89 | artifact_replica_meteorite | Replica Meteorite |
| 90 | artifact_replica_miners_helmet | Replica Miner's Helmet |
| 91 | artifact_replica_miners_pickaxe | Replica Miner's Pickaxe |
| 92 | artifact_replica_miners_rucksack | Replica Miner's Rucksack |
| 93 | artifact_replica_miners_slab | Replica Miner's Slab |
| 94 | artifact_replica_mist_crystal | Replica Mist Crystal |
| 95 | artifact_replica_mist_flute | Replica Mist Flute |
| 96 | artifact_replica_mist_scroll | Replica Mist Scroll |
| 97 | artifact_replica_misty_black_mirror | Replica Misty Black Mirror |
| 98 | artifact_replica_misty_feather_quill | Replica Misty Feather Quill |
| 99 | artifact_replica_muttering_cube | Replica Muttering Cube |
| 100 | artifact_replica_obsidian_blade | Replica Obsidian Blade |
| 101 | artifact_replica_perfect_copper_ore | Replica Perfect Copper Ore |
| 102 | artifact_replica_perfect_diamond | Replica Perfect Diamond |
| 103 | artifact_replica_perfect_emerald | Replica Perfect Emerald |
| 104 | artifact_replica_perfect_gold_ore | Replica Perfect Gold Ore |
| 105 | artifact_replica_perfect_iron_ore | Replica Perfect Iron Ore |
| 106 | artifact_replica_perfect_mistril_ore | Replica Perfect Mistril Ore |
| 107 | artifact_replica_perfect_pink_diamond | Replica Perfect Pink Diamond |
| 108 | artifact_replica_perfect_ruby | Replica Perfect Ruby |
| 109 | artifact_replica_perfect_sapphire | Replica Perfect Sapphire |
| 110 | artifact_replica_perfect_silver_ore | Replica Perfect Silver Ore |
| 111 | artifact_replica_petrified_wood | Replica Petrified Wood |
| 112 | artifact_replica_porcelain_figurine | Replica Porcelain Figure |
| 113 | artifact_replica_rainbow_geode | Replica Rainbow Geode |
| 114 | artifact_replica_rainbow_seaweed | Replica Rainbow Seaweed |
| 115 | artifact_replica_really_round_rock | Replica Really Round Rock |
| 116 | artifact_replica_red_obsidian | Replica Red Obsidian |
| 117 | artifact_replica_ritual_beads | Replica Ritual Beads |
| 118 | artifact_replica_ritual_chalice | Replica Ritual Chalice |
| 119 | artifact_replica_ritual_incense_burner | Replica Ritual Incense Burner |
| 120 | artifact_replica_ritual_scepter | Replica Ritual Scepter |
| 121 | artifact_replica_ritual_tablet | Replica Ritual Tablet |
| 122 | artifact_replica_rock_statue | Replica Rock Statue |
| 123 | artifact_replica_rock_with_a_hole | Replica Rock with a Hole |
| 124 | artifact_replica_rubber_fish | Replica Rubber Fish |
| 125 | artifact_replica_rusted_shield | Replica Rusted Shield |
| 126 | artifact_replica_rusted_treasure_chest | Replica Rusted Treasure Chest |
| 127 | artifact_replica_sea_glass | Replica Sea Glass |
| 128 | artifact_replica_seriously_square_stone | Replica Seriously Square Stone |
| 129 | artifact_replica_shortcut_scroll | Replica Shortcut Scroll |
| 130 | artifact_replica_starlight_coral | Replica Starlight Coral |
| 131 | artifact_replica_statuette_of_caldarus | Replica Statuette of Caldarus |
| 132 | artifact_replica_stone_horse | Replica Stone Horse |
| 133 | artifact_replica_stone_shell | Replica Stone Shell |
| 134 | artifact_replica_tidestone | Replica Tidestone |
| 135 | artifact_replica_tin_lunchbox | Replica Tin Lunchbox |
| 136 | artifact_replica_tiny_dinosaur_skeleton | Replica Tiny Dinosaur Skeleton |
| 137 | artifact_replica_tiny_volcano | Replica Tiny Volcano |
| 138 | artifact_replica_trilobite_fossil | Replica Trilobite Fossil |
| 139 | artifact_replica_unknown_dragon_statuette | Replica Unknown Dragon Statuette |
| 140 | artifact_replica_vintage_brush | Replica Vintage Brush |
| 141 | artifact_replica_vintage_cow_bell | Replica Vintage Cow Bell |
| 142 | artifact_replica_vintage_hammer | Replica Vintage Hammer |
| 143 | artifact_replica_vintage_sickle | Replica Vintage Sickle |
| 144 | artifact_replica_vintage_watering_can | Replica Vintage Watering Can |
| 145 | artifact_replica_warm_rock | Replica Warm Rock |
| 146 | artifact_replica_water_sphere | Replica Water Sphere |
| 147 | artifact_replica_weightless_stone | Replica Weightless Stone |
| 148 | artifact_replica_worn_pendant | Replica Worn Pendant |
| 149 | ash_mushroom | Ash Mushroom |
| 150 | auto_feeder | Auto Feeder |
| 151 | autumn_scarecrow | Autumn Scarecrow |
| 152 | axe_copper | Copper Axe |
| 153 | axe_gold | Gold Axe |
| 154 | axe_iron | Iron Axe |
| 155 | axe_mistril | Mistril Axe |
| 156 | axe_silver | Silver Axe |
| 157 | axe_worn | Worn Axe |
| 158 | bait_thief | Bait Thief |
| 159 | baked_potato | Baked Potato |
| 160 | baked_sweetroot | Baked Sweetroot |
| 162 | bakery_bread_basket | Bakery Bread Basket |
| 166 | bakery_cake | Bakery Display Cake |
| 164 | bakery_cake_case | Bakery Dessert Case |
| 168 | bakery_cake_slice | Bakery Display Cake Slice |
| 170 | bakery_cookie_jar | Bakery Cookie Jar |
| 172 | bakery_cutting_board | Bakery Cutting Board |
| 173 | balors_crate | Balor's Crate | **GLITCHED** |
| 174 | barb | Barb |
| 176 | basic_area_rug | Basic Area Rug |
| 180 | basic_bed | Basic Single Bed |
| 178 | basic_bed_double | Basic Double Bed |
| 182 | basic_bookshelf | Basic Bookshelf |
| 184 | basic_chair | Basic Chair |
| 186 | basic_doormat | Basic Doormat |
| 188 | basic_dresser | Basic Dresser |
| 190 | basic_flooring | Basic Flooring |
| 192 | basic_flower_pot | Basic Flower Pot |
| 194 | basic_nightstand | Basic Nightstand |
| 196 | basic_oil_lamp | Basic Oil Lamp |
| 197 | basic_pouch | Bag Upgrade |
| 199 | basic_table | Basic Table |
| 201 | basic_wall_window | Basic Window |
| 203 | basic_wallpaper | Basic Wallpaper |
| 204 | basic_wood | Wood |
| 219 | basic_wood_chest | Basic Wood Chest |
| 220 | basil | Basil |
| 221 | basket | Basket |
| 222 | bath_soap | Bath Soap |
| 224 | bathroom_bench | Bathroom Bench |
| 231 | bathroom_curtain_stand | Bathroom Curtain Stand |
| 233 | bathroom_floor_mirror | Bathroom Floor Mirror |
| 235 | bathroom_sink | Bathroom Sink |
| 239 | bathroom_tile_wall | Bathroom Tile Wall |
| 241 | bathroom_wall_mirror | Bathroom Wall Mirror |
| 242 | bathroom_wall_sconce | Bathroom Wall Sconce |
| 249 | bathroom_wall_towel | Bathroom Wall Towel |
| 251 | bathtub | Bathtub |
| 252 | beach_hopper | Beach Hopper |
| 253 | beer | Beer |
| 254 | beer_mug | Beer Mug |
| 255 | beet | Beet |
| 256 | beet_salad | Beet Salad |
| 257 | beet_soup | Beet Soup |
| 258 | beginner_kitchen | Level 1 Kitchen |
| 259 | bell_berry | Bell Berry |
| 260 | bell_berry_bakewell_tart | Bell Berry Bakewell Tart |
| 261 | berries_and_cream | Berries and Cream |
| 262 | berry_bowl | Berry Bowl |
| 263 | big_bee | Big Bee |
| 264 | big_bell | Big Bell |
| 265 | big_cookie | Big Cookie |
| 266 | biggest_beetle | Biggest Beetle |
| 267 | black_tablet | Black Tablet |
| 268 | blackberry | Blackberry |
| 269 | blackberry_jam | Blackberry Jam |
| 270 | blue_capybara_wall_ribbon | Blue Capybara Wall Ribbon |
| 271 | blue_chicken_wall_ribbon | Blue Chicken Wall Ribbon |
| 272 | blue_conch_shell | Blue Conch Shell |
| 273 | blue_crab | Blue Crab |
| 274 | blue_duck_wall_ribbon | Blue Duck Wall Ribbon |
| 275 | blue_pet_bed | Blue Pet Bed |
| 276 | blue_pet_dish | Blue Pet Dish |
| 277 | blue_rabbit_wall_ribbon | Blue Rabbit Wall Ribbon |
| 278 | blueberry | Blueberry |
| 279 | blueberry_jam | Blueberry Jam |
| 280 | bluefish | Bluefish |
| 281 | bluegill | Blue Gill |
| 282 | bomb | Clod Bomb |
| 283 | bonito | Bonito |
| 284 | bowfish | Bowfish |
| 285 | box_medium | Medium Box |
| 287 | box_small | Small Box |
| 288 | braised_burdock | Braised Burdock |
| 289 | braised_carrots | Braised Carrots |
| 290 | bread | Bread |
| 291 | breaded_catfish | Breaded Catfish |
| 292 | bream | Bream |
| 293 | breath_of_fire | Breath of Flame |
| 294 | breath_of_spring | Breath of Spring |
| 295 | brightbulb_moth | Brightbulb Moth |
| 296 | bristle | Bristle |
| 297 | broccoli | Broccoli |
| 298 | broccoli_salad | Broccoli Salad |
| 306 | broken_pillar | Broken Pillar |
| 307 | bronze_alpaca_trophy | Bronze Alpaca Trophy |
| 308 | bronze_cow_trophy | Bronze Cow Trophy |
| 309 | bronze_horse_trophy | Bronze Horse Trophy |
| 310 | bronze_sheep_trophy | Bronze Sheep Trophy |
| 311 | brown_bullhead | Brown Bullhead |
| 312 | brown_trout | Brown Trout |
| 313 | bucket_brew | Bucket Brew |
| 314 | bug_pheromone_common | Common Bug Pheromones |
| 315 | bug_pheromone_legendary | Legendary Bug Pheromones |
| 316 | bug_pheromone_rare | Rare Bug Pheromones |
| 317 | bug_pheromone_uncommon | Uncommon Bug Pheromones |
| 318 | bull_horn | Bull Horn |
| 319 | bullfrog | Bullfrog |
| 320 | bumblebee | Bumblebee |
| 321 | bumblebee_plushie | Bumblebee Plushie |
| 322 | burbot | Burbot |
| 323 | burdock_root | Burdock Root |
| 324 | butter | Butter |
| 325 | buttered_peas | Buttered Peas |
| 326 | butterfish | Butterfish |
| 327 | butterfly | Butterfly |
| 330 | butterfly_bed | Butterfly Bed |
| 333 | butterfly_double_bed | Butterfly Double Bed |
| 336 | butterfly_painting | Butterfly Painting |
| 339 | butterfly_wall_lamp | Butterfly Wall Lamp |
| 340 | cabbage | Cabbage |
| 341 | cabbage_slaw | Cabbage Slaw |
| 347 | cabin_bed | Rustic Cabin Bed |
| 345 | cabin_bed_double | Rustic Cabin Double Bed |
| 350 | cabin_bookshelf | Rustic Cabin Bookshelf |
| 353 | cabin_chair | Rustic Cabin Chair |
| 356 | cabin_dresser | Rustic Cabin Dresser |
| 359 | cabin_firewood | Rustic Cabin Firewood |
| 362 | cabin_flooring | Rustic Cabin Flooring |
| 365 | cabin_lamp | Rustic Cabin Lamp |
| 368 | cabin_nightstand | Rustic Cabin Nightstand |
| 370 | cabin_stove | Rustic Cabin Stove |
| 373 | cabin_table | Rustic Cabin Table |
| 376 | cabin_wall_shelf | Rustic Cabin Wall Shelf |
| 379 | cabin_wall_window | Rustic Cabin Window |
| 382 | cabin_wallpaper | Rustic Cabin Wallpaper |
| 383 | caldosian_breastplate | Caldosian Breastplate |
| 384 | caldosian_chocolate_cake | Chocolate Cake |
| 385 | caldosian_drinking_horn | Caldosian Drinking Horn |
| 386 | caldosian_emperor_bust | Caldosian Emperor Bust |
| 387 | caldosian_sword | Caldosian Sword |
| 388 | campfire | Campfire |
| 389 | candelabra_seadragon | Candelabra Seadragon |
| 390 | candied_lemon_peel | Candied Lemon Peel |
| 391 | candied_queen_berries | Candied Queen Berries |
| 392 | candied_strawberries | Candied Strawberries |
| 393 | candied_walnuts | Candied Walnuts |
| 394 | candle | Candle |
| 395 | candle_chamberstick | Candle Chamberstick |
| 396 | canned_sardines | Canned Sardines |
| 397 | capybara_plushie | Capybara Plushie |
| 398 | caramel_candy | Caramel Candy |
| 399 | caramelized_moon_fruit | Caramelized Moon Fruit |
| 400 | carp | Carp |
| 401 | carrot | Carrot |
| 402 | cat_treat | Cat Treat |
| 403 | caterpillar | Caterpillar |
| 406 | caterpillar_chair | Caterpillar Chair |
| 407 | catfish | Catfish |
| 408 | catmint | Catmint |
| 409 | cattail | Cattail Fluff |
| 410 | cauliflower | Cauliflower |
| 411 | cauliflower_curry | Cauliflower Curry |
| 412 | cave_eel | Cave Eel |
| 413 | cave_kelp | Jade Dulse |
| 414 | cave_shark | Cave Shark |
| 415 | cave_shrimp | Cave Shrimp |
| 418 | cavern_crystal_lamp | Glowing Crystal Lamp |
| 419 | cavern_emerald_rock | Ornamental Emerald Rock |
| 422 | cavern_floor_brazier | Floor Brazier |
| 423 | cavern_hewn_rock_bookshelf | Hewn Rock Bookshelf |
| 424 | cavern_hewn_rock_table | Hewn Rock Table |
| 426 | cavern_mossy_rug | Mossy Rug |
| 429 | cavern_mushroom_stool | Mushroom Stool |
| 430 | cavern_rock_flooring | Rock Floor |
| 431 | cavern_rock_wallpaper | Rock Wall |
| 432 | cavern_silver_node | Ornamental Silver Node |
| 433 | cavern_stone_arch | Stalactite Arch |
| 436 | cavern_wall_brazier | Wall Brazier |
| 437 | celosia | Celosia |
| 441 | champions_kitchen | Champion's Kitchen |
| 442 | char | Char |
| 443 | cheese | Cheese |
| 444 | cherry | Cherry |
| 445 | cherry_cobbler | Cherry Cobbler |
| 446 | cherry_fish | Cherry Fish |
| 447 | cherry_smoothie | Cherry Smoothie |
| 448 | cherry_tart | Cherry Tart |
| 452 | chess_bishop | Chess Bishop Piece |
| 456 | chess_king | Chess King Piece |
| 460 | chess_knight | Chess Knight Piece |
| 464 | chess_pawn | Chess Pawn Piece |
| 468 | chess_queen | Chess Queen Piece |
| 472 | chess_rook | Chess Rook Piece |
| 473 | chess_scroll_bundle | Chess Piece Scroll Bundle |
| 477 | chess_table | Chess Table |
| 478 | chestnut | Chestnut |
| 479 | chicken_plushie | Chicken Plushie |
| 480 | chickpea | Chickpea |
| 481 | chickpea_curry | Chickpea Curry |
| 482 | chicky_hot_chocolate | Chicky Hot Chocolate |
| 483 | chili_coconut_curry | Chili Coconut Curry |
| 484 | chili_pepper | Chili Pepper |
| 485 | chillipede | Chillipede |
| 486 | chirping_fern | Chirping Fern |
| 487 | chocolate | Chocolate |
| 488 | chrysanthemum | Chrysanthemum |
| 489 | chub | Chub |
| 490 | chum | Chum |
| 491 | cicada | Cicada |
| 492 | cicada_nymph | Cicada Nymph |
| 493 | clam | Clam |
| 494 | clam_chowder | Clam Chowder |
| 495 | clay | Clay |
| 496 | clay_amphora | Clay Amphora |
| 497 | clay_key | Clay Key |
| 498 | cloth_chestpiece | Cloth Chestpiece |
| 499 | cloth_helmet | Cloth Helmet |
| 500 | cloth_pants | Cloth Pants |
| 501 | cloth_shoes | Cloth Shoes |
| 502 | cloth_wristband | Cloth Wristband |
| 503 | coconut | Coconut |
| 504 | coconut_cream_pie | Coconut Cream Pie |
| 505 | coconut_milk | Coconut Milk |
| 506 | cod | Cod |
| 507 | cod_with_thyme | Cod with Thyme |
| 508 | coelacanth | Coelacanth |
| 509 | coffee | Coffee |
| 512 | coffee_mug | Coffee Mug |
| 513 | coin_lump | Coin Lump |
| 555 | color_paving_stone | Color Paving Stone |
| 556 | completely_wrong_map | Completely Wrong Map |
| 557 | confiscated_coffee | Confiscated Coffee |
| 558 | construction_fence | Construction Fence |
| 561 | cooking_frying_pan | Frying Pan |
| 563 | cooking_hanging_pans | Hanging Pans |
| 566 | cooking_stockpot | Stockpot |
| 569 | cooking_tea_kettle | Tea Kettle |
| 570 | cooktop_beetle | Cooktop Beetle |
| 571 | copper_armor | Copper Armor |
| 572 | copper_beetle | Copper Nugget Beetle |
| 573 | copper_greaves | Copper Greaves |
| 574 | copper_helmet | Copper Helmet |
| 575 | copper_ingot | Copper Ingot |
| 576 | copper_legplates | Copper Legplates |
| 577 | copper_ring | Copper Ring |
| 578 | copper_rockfish | Copper Rockfish |
| 579 | coral | Coral |
| 581 | coral_chair | Coral Chair |
| 583 | coral_lamp | Coral Lamp |
| 584 | coral_mantis | Coral Mantis |
| 585 | coral_stone_archway | Coral-Stone Arch |
| 587 | coral_storage_chest | Coral Storage Chest |
| 589 | coral_table | Coral Table |
| 590 | corn | Corn |
| 591 | cornucopia | Cornucopia |
| 592 | corrupted_mistril_boots | Corrupted Mistril Greaves |
| 593 | corrupted_mistril_chestpiece | Corrupted Mistril Armor |
| 594 | corrupted_mistril_helmet | Corrupted Mistril Helmet |
| 595 | corrupted_mistril_pants | Corrupted Mistril Legplates |
| 596 | corrupted_mistril_ring | Corrupted Mistril Ring |
| 597 | cosmetic | MISSING | **DISABLED** |
| 598 | cosmos | Cosmos |
| 602 | cottage_bed | Lovely Cottage Single Bed |
| 601 | cottage_bed_double | Lovely Cottage Double Bed |
| 604 | cottage_chair | Lovely Cottage Chair |
| 606 | cottage_dresser | Lovely Cottage Dresser |
| 608 | cottage_flooring | Lovely Cottage Flooring |
| 610 | cottage_fridge | Lovely Cottage Icebox |
| 612 | cottage_garden_arch | Cottage Garden Arch |
| 615 | cottage_garden_planter | Cottage Garden Planter |
| 617 | cottage_garden_shed | Cottage Garden Shed |
| 620 | cottage_garden_trellis | Cottage Garden Trellis |
| 622 | cottage_garden_workbench | Cottage Garden Workbench |
| 624 | cottage_nightstand | Lovely Cottage Nightstand |
| 626 | cottage_plant_shelf | Lovely Cottage Plant Shelf |
| 628 | cottage_potted_fig | Lovely Cottage Potted Fig |
| 630 | cottage_potted_flowers | Lovely Cottage Potted Flowers |
| 632 | cottage_rug | Lovely Cottage Rug |
| 634 | cottage_table | Lovely Cottage Table |
| 636 | cottage_wall_flowers | Lovely Cottage Wall Flowers |
| 638 | cottage_wall_window | Lovely Cottage Window |
| 640 | cottage_wallpaper | Lovely Cottage Wallpaper |
| 641 | counter_basic_v1 | Basic Counter |
| 642 | counter_basic_v2 | Basic Counter |
| 643 | counter_cabin_cherry | Cabin Counter |
| 644 | counter_cabin_oak | Cabin Counter |
| 645 | counter_cabin_walnut | Cabin Counter |
| 646 | counter_cake_chocolate | Cake Counter |
| 647 | counter_cake_double_chocolate | Cake Counter |
| 648 | counter_cake_strawberry | Cake Counter |
| 649 | counter_cottage_v1 | Lovely Cottage Counter |
| 650 | counter_cottage_v2 | Lovely Cottage Counter |
| 654 | counter_kitchen | Champion's Kitchen Counter |
| 655 | cow_donut | Cow-Shaped Donut |
| 656 | cow_milk | Milk |
| 657 | cow_plushie | Cow Plushie |
| 658 | cozy_kitchen | Level 2 Kitchen |
| 659 | crab | Crab |
| 660 | crab_cakes | Crab Cakes |
| 661 | crab_statue | Crab Statue |
| 667 | craft_fabric_rack | Craft Fabric Rack |
| 673 | craft_rug | Craft Crochet Rug |
| 670 | craft_sewing_tin | Craft Sewing Tin |
| 676 | craft_yarn_basket | Craft Yarn Basket |
| 677 | crafting_scroll | Crafting Scroll: {} | **DISABLED** |
| 678 | cranberry | Cranberry |
| 679 | cranberry_juice | Cranberry Juice |
| 680 | cranberry_orange_scone | Cranberry Orange Scone |
| 681 | crayfish | Crayfish |
| 682 | crayfish_etouffee | Crayfish Etouffee |
| 683 | cream_pet_bed | Cream Pet Bed |
| 684 | cream_pet_dish | Cream Pet Dish |
| 685 | cricket | Cricket |
| 686 | criminal_confession | Criminal Confession |
| 687 | crispy_fried_earthshroom | Crispy Fried Earthshroom |
| 688 | crocus | Crocus |
| 689 | crop_faux_ash_mushroom | Faux Ash Mushroom |
| 690 | crop_faux_basil | Faux Basil |
| 691 | crop_faux_beet | Faux Beet |
| 692 | crop_faux_blue_conch_shell | Faux Blue Conch Shell |
| 693 | crop_faux_breath_of_fire | Faux Breath of Flame |
| 694 | crop_faux_broccoli | Faux Broccoli |
| 695 | crop_faux_burdock_root | Faux Burdock |
| 696 | crop_faux_cabbage | Faux Cabbage |
| 697 | crop_faux_carrot | Faux Carrot |
| 698 | crop_faux_catmint | Faux Catmint |
| 699 | crop_faux_cauliflower | Faux Cauliflower |
| 700 | crop_faux_cave_kelp | Faux Jade Dulse |
| 701 | crop_faux_celosia | Faux Celosia |
| 702 | crop_faux_chestnut | Faux Chestnut |
| 703 | crop_faux_chickpea | Faux Chickpea |
| 704 | crop_faux_chili_pepper | Faux Chili Pepper |
| 705 | crop_faux_chirping_fern | Faux Chirping Fern |
| 706 | crop_faux_chrysanthemum | Faux Chrysanthemum |
| 707 | crop_faux_corn | Faux Corn |
| 708 | crop_faux_cosmos | Faux Cosmos |
| 709 | crop_faux_cranberry | Faux Cranberry |
| 710 | crop_faux_crocus | Faux Crocus |
| 711 | crop_faux_crystal_berries | Faux Crystal Berries |
| 712 | crop_faux_crystal_rose | Faux Crystal Rose |
| 713 | crop_faux_cucumber | Faux Cucumber |
| 714 | crop_faux_daffodil | Faux Daffodil |
| 715 | crop_faux_daikon_radish | Faux Daikon Radish |
| 716 | crop_faux_daisy | Faux Daisy |
| 717 | crop_faux_dandelion | Faux Dandelion |
| 718 | crop_faux_dill | Faux Dill |
| 719 | crop_faux_earthshroom | Faux Earthshroom |
| 720 | crop_faux_essence_blossom | Faux Essence Blossom |
| 721 | crop_faux_ethereal_grass | Faux Ethereal Grass |
| 722 | crop_faux_fennel | Faux Fennel |
| 723 | crop_faux_fiddlehead | Faux Fiddlehead |
| 724 | crop_faux_flame_pepper | Faux Flame Pepper |
| 725 | crop_faux_fog_orchid | Faux Fog Orchid |
| 726 | crop_faux_frost_lily | Faux Frost Lily |
| 727 | crop_faux_garlic | Faux Garlic |
| 728 | crop_faux_heather | Faux Heather |
| 729 | crop_faux_holly | Faux Holly |
| 730 | crop_faux_horseradish | Faux Horseradish |
| 731 | crop_faux_hot_potato | Faux Hot Potato |
| 732 | crop_faux_ice_block | Faux Ice Block |
| 733 | crop_faux_iris | Faux Iris |
| 734 | crop_faux_jasmine | Faux Jasmine |
| 735 | crop_faux_lava_chestnuts | Faux Lava Chestnuts |
| 736 | crop_faux_lilac | Faux Lilac |
| 737 | crop_faux_marigold | Faux Marigold |
| 738 | crop_faux_middlemist | Faux Middlemist |
| 739 | crop_faux_mines_mussels | Faux Mines Mussels |
| 740 | crop_faux_moon_fruit | Faux Moon Fruit |
| 741 | crop_faux_morel_mushroom | Faux Morel Mushroom |
| 742 | crop_faux_mystery_bag | Faux Magic Seed Plant |
| 743 | crop_faux_narrows_moss | Faux Narrows Moss |
| 744 | crop_faux_nettle | Faux Nettle |
| 745 | crop_faux_night_queen | Faux Night Queen |
| 746 | crop_faux_onion | Faux Onion |
| 747 | crop_faux_oregano | Faux Oregano |
| 748 | crop_faux_oyster_mushroom | Faux Oyster Mushroom |
| 749 | crop_faux_peas | Faux Peas |
| 750 | crop_faux_pineshroom | Faux Pineshroom |
| 751 | crop_faux_pink_scallop_shell | Faux Pink Scallop Shell |
| 752 | crop_faux_poinsettia | Faux Poinsettia |
| 753 | crop_faux_potato | Faux Potato |
| 754 | crop_faux_pumpkin | Faux Pumpkin |
| 755 | crop_faux_rice | Faux Rice |
| 756 | crop_faux_rockroot | Faux Rockroot |
| 757 | crop_faux_rosemary | Faux Rosemary |
| 758 | crop_faux_sage | Faux Sage |
| 759 | crop_faux_sand_dollar | Faux Sand Dollar |
| 760 | crop_faux_sea_grapes | Faux Sea Grapes |
| 761 | crop_faux_sesame | Faux Sesame |
| 762 | crop_faux_shadow_flower | Faux Shadow Flower |
| 763 | crop_faux_shale_grass | Faux Shale Grass |
| 764 | crop_faux_snapdragon | Faux Snapdragon |
| 765 | crop_faux_snow_peas | Faux Snow Peas |
| 766 | crop_faux_snowdrop_anemone | Faux Snowdrop Anemone |
| 767 | crop_faux_spell_fruit | Faux Spell Fruit |
| 768 | crop_faux_spirit_mushroom | Faux Spirit Mushroom |
| 769 | crop_faux_spirula_shell | Faux Spirula Shell |
| 770 | crop_faux_strawberry | Faux Strawberry |
| 771 | crop_faux_sugar_cane | Faux Sugar Cane |
| 772 | crop_faux_sunflower | Faux Sunflower |
| 773 | crop_faux_sweet_potato | Faux Sweet Potato |
| 774 | crop_faux_sweetroot | Faux Sweetroot |
| 775 | crop_faux_tea | Faux Tea |
| 776 | crop_faux_temple_flower | Faux Temple Flower |
| 777 | crop_faux_thorn_vine | Faux Thorn Vine |
| 778 | crop_faux_thyme | Faux Thyme |
| 779 | crop_faux_tide_lettuce | Faux Tide Lettuce |
| 780 | crop_faux_tomato | Faux Tomato |
| 781 | crop_faux_tulip | Faux Tulip |
| 782 | crop_faux_turnip | Faux Turnip |
| 783 | crop_faux_upper_mines_mushroom | Faux Upper Mines Mushroom |
| 784 | crop_faux_viola | Faux Viola |
| 785 | crop_faux_void_herb | Faux Void Herb |
| 786 | crop_faux_walnut | Faux Walnut |
| 787 | crop_faux_watermelon | Faux Watermelon |
| 788 | crop_faux_wheat | Faux Wheat |
| 789 | crop_faux_wild_leek | Faux Wild Leek |
| 790 | crop_faux_written_root | Faux Written Root |
| 791 | crop_sign_apple | Apple Sign |
| 792 | crop_sign_basil | Basil Sign |
| 793 | crop_sign_beet | Beet Sign |
| 794 | crop_sign_broccoli | Broccoli Sign |
| 795 | crop_sign_burdock_root | Burdock Root Sign |
| 796 | crop_sign_cabbage | Cabbage Sign |
| 797 | crop_sign_carrot | Carrot Sign |
| 798 | crop_sign_catmint | Catmint Sign |
| 799 | crop_sign_cauliflower | Cauliflower Sign |
| 800 | crop_sign_celosia | Celosia Sign |
| 801 | crop_sign_cherry | Cherry Sign |
| 802 | crop_sign_chickpea | Chickpea Sign |
| 803 | crop_sign_chili_pepper | Chili Pepper Sign |
| 804 | crop_sign_chrysanthemum | Chrysanthemum Sign |
| 805 | crop_sign_coconut | Coconut Sign |
| 806 | crop_sign_corn | Corn Sign |
| 807 | crop_sign_cosmos | Cosmos Sign |
| 808 | crop_sign_cranberry | Cranberry Sign |
| 809 | crop_sign_cucumber | Cucumber Sign |
| 810 | crop_sign_daffodil | Daffodil Sign |
| 811 | crop_sign_daikon_radish | Daikon Radish Sign |
| 812 | crop_sign_daisy | Daisy Sign |
| 813 | crop_sign_dill | Dill Sign |
| 814 | crop_sign_frost_lily | Frost Lily Sign |
| 815 | crop_sign_garlic | Garlic Sign |
| 816 | crop_sign_heather | Heather Sign |
| 817 | crop_sign_iris | Iris Sign |
| 818 | crop_sign_jasmine | Jasmine Sign |
| 819 | crop_sign_lemon | Lemon Sign |
| 820 | crop_sign_lilac | Lilac Sign |
| 821 | crop_sign_marigold | Marigold Sign |
| 822 | crop_sign_moon_fruit | Moon Fruit Sign |
| 823 | crop_sign_mystery_bag | Magic Seed Plant Sign |
| 824 | crop_sign_night_queen | Night queen Sign |
| 825 | crop_sign_onion | Onion Sign |
| 826 | crop_sign_orange | Orange Sign |
| 827 | crop_sign_oregano | Oregano Sign |
| 828 | crop_sign_peach | Peach Sign |
| 829 | crop_sign_pear | Pear Sign |
| 830 | crop_sign_peas | Peas Sign |
| 831 | crop_sign_poinsettia | Poinsettia Sign |
| 832 | crop_sign_pomegranate | Pomegranate Sign |
| 833 | crop_sign_potato | Potato Sign |
| 834 | crop_sign_pumpkin | Pumpkin Sign |
| 835 | crop_sign_rice | Rice Sign |
| 836 | crop_sign_rosemary | Rosemary Sign |
| 837 | crop_sign_sage | Sage Sign |
| 838 | crop_sign_snapdragon | Snapdragon Sign |
| 839 | crop_sign_snow_peas | Snow Peas Sign |
| 840 | crop_sign_snowdrop_anemone | Snowdrop Anemone Sign |
| 841 | crop_sign_strawberry | Strawberry Sign |
| 842 | crop_sign_sugar_cane | Sugar Cane Sign |
| 843 | crop_sign_sunflower | Sunflower Sign |
| 844 | crop_sign_sweet_potato | Sweet Potato Sign |
| 845 | crop_sign_tea | Tea Sign |
| 846 | crop_sign_temple_flower | Temple Flower Sign |
| 847 | crop_sign_thyme | Thyme Sign |
| 848 | crop_sign_tomato | Tomato Sign |
| 849 | crop_sign_tulip | Tulip Sign |
| 850 | crop_sign_turnip | Turnip Sign |
| 851 | crop_sign_viola | Viola Sign |
| 852 | crop_sign_watermelon | Watermelon Sign |
| 853 | crop_sign_wheat | Wheat Sign |
| 854 | crucian_carp | Crucian Carp |
| 855 | crunchy_chickpeas | Crunchy Chickpeas |
| 856 | crystal | Crystal |
| 857 | crystal_apple | Crystal Apple |
| 858 | crystal_berries | Crystal Berries |
| 859 | crystal_berry_pie | Crystal Berry Pie |
| 860 | crystal_boots | Crystal Greaves |
| 861 | crystal_caterpillar | Crystal Caterpillar |
| 862 | crystal_chestpiece | Crystal Chestpiece |
| 863 | crystal_helmet | Crystal Helmet |
| 864 | crystal_ring | Crystal Ring |
| 865 | crystal_rose | Crystal Rose |
| 868 | crystal_string_lights | Crystal Wall String Lights |
| 869 | crystal_tassets | Crystal Tassets |
| 870 | crystal_wing_moth | Crystal Wing Moth |
| 871 | crystalline_cricket | Crystalline Cricket |
| 872 | cucumber | Cucumber |
| 873 | cucumber_salad | Cucumber Salad |
| 874 | cucumber_sandwich | Cucumber Sandwich |
| 875 | cup_of_tea | Tea with Lemon |
| 876 | curry_powder | Curry Powder |
| 877 | dace | Dace |
| 878 | daffodil | Daffodil |
| 879 | daikon_radish | Daikon Radish |
| 880 | daisy | Daisy |
| 881 | dandelion | Dandelion |
| 882 | dart | Dart |
| 883 | date_photo_bath | Bath Date Photo |
| 884 | date_photo_beach | Beach Date Photo |
| 885 | date_photo_deep_woods | Picnic Date Photo |
| 886 | date_photo_gem_cutting | Gem Cutting Date Photo |
| 887 | date_photo_harvest | Harvest Dance Date Photo |
| 888 | date_photo_inn | Inn Date Photo |
| 889 | date_photo_park | Park Date Photo |
| 890 | date_photo_shooting_star | Shooting Star Date Photo |
| 891 | deep_earthworm | Deep Earthworm |
| 892 | deep_sea_soup | Deep Sea Soup |
| 893 | deluxe_curry | Deluxe Curry |
| 894 | deluxe_hay | Deluxe Large Animal Feed |
| 895 | deluxe_icebox_blue | Deluxe Icebox |
| 896 | deluxe_icebox_green | Deluxe Icebox |
| 897 | deluxe_icebox_pink | Deluxe Icebox |
| 898 | deluxe_icebox_white | Deluxe Icebox |
| 899 | deluxe_icebox_yellow | Deluxe Icebox |
| 900 | deluxe_small_animal_feed | Deluxe Small Animal Feed |
| 901 | deluxe_storage_chest_aqua | Deluxe Storage Chest |
| 902 | deluxe_storage_chest_black | Deluxe Storage Chest |
| 903 | deluxe_storage_chest_blue | Deluxe Storage Chest |
| 904 | deluxe_storage_chest_dark_brown | Deluxe Storage Chest |
| 905 | deluxe_storage_chest_gold | Deluxe Storage Chest |
| 906 | deluxe_storage_chest_gray | Deluxe Storage Chest |
| 907 | deluxe_storage_chest_green | Deluxe Storage Chest |
| 908 | deluxe_storage_chest_light_brown | Deluxe Storage Chest |
| 909 | deluxe_storage_chest_orange | Deluxe Storage Chest |
| 910 | deluxe_storage_chest_pink | Deluxe Storage Chest |
| 911 | deluxe_storage_chest_purple | Deluxe Storage Chest |
| 912 | deluxe_storage_chest_red | Deluxe Storage Chest |
| 913 | deluxe_storage_chest_white | Deluxe Storage Chest |
| 914 | dense_water | Dense Water |
| 915 | deviled_eggs | Deviled Eggs |
| 916 | diamond_backed_mirror | Diamond-Backed Mirror |
| 917 | diamond_beetle | Diamond Beetle |
| 918 | dill | Dill |
| 919 | dingy_broken_bottle_v1 | Broken Bottle |
| 920 | dingy_broken_bottle_v2 | Broken Bottle |
| 921 | dingy_broken_plate_v1 | Broken Plate |
| 922 | dingy_broken_plate_v2 | Broken Plate |
| 923 | dingy_chair_v1 | Cardboard Box Chair |
| 924 | dingy_chair_v2 | Cardboard Box Chair |
| 925 | dingy_chipped_cup_v1 | Chipped Cup |
| 926 | dingy_chipped_cup_v2 | Chipped Cup |
| 927 | dingy_couch_v1 | Broken Springs Couch |
| 928 | dingy_couch_v2 | Broken Springs Couch |
| 929 | dingy_flooring_v1 | Dirty Tiles Flooring |
| 930 | dingy_flooring_v2 | Dirty Tiles Flooring |
| 931 | dingy_garbage_bag_v1 | Garbage Bag |
| 932 | dingy_garbage_bag_v2 | Garbage Bag |
| 933 | dingy_mouse_hole_v1 | Mouse Hole |
| 934 | dingy_mouse_hole_v2 | Mouse Hole |
| 935 | dingy_pile_of_laundry_v1 | Pile of Laundry |
| 936 | dingy_pile_of_laundry_v2 | Pile of Laundry |
| 937 | dingy_pile_of_newspapers_v1 | Pile of Newspapers |
| 938 | dingy_pile_of_newspapers_v2 | Pile of Newspapers |
| 939 | dingy_rug_v1 | Tattered Rug |
| 940 | dingy_rug_v2 | Tattered Rug |
| 941 | dingy_string_lights_v1 | Mostly Broken String Lights |
| 942 | dingy_string_lights_v2 | Mostly Broken String Lights |
| 943 | dingy_table_v1 | Rickety Table |
| 944 | dingy_table_v2 | Rickety Table |
| 945 | dingy_wallpaper_v1 | Peeling Wallpaper |
| 946 | dingy_wallpaper_v2 | Peeling Wallpaper |
| 947 | dingy_window_v1 | Broken Window |
| 948 | dingy_window_v2 | Broken Window |
| 949 | dirt_clear | Dirt Tile Clear |
| 950 | dirt_dot_one | Dirt Tile 1 Dot |
| 951 | dirt_dot_three | Dirt Tile 3 Dots |
| 952 | dirt_dot_two | Dirt Tile 2 Dots |
| 953 | dog_treat | Dog Treat |
| 954 | dollhouse_bench | Dollhouse Bench |
| 955 | dough_lad_doll | Doughlad Doll |
| 959 | dragon_altar | Dragon Claw Altar |
| 960 | dragon_claw | Dragon Claw |
| 961 | dragon_forged_bracelet | Dragon-Forged Bracelet |
| 962 | dragon_forged_core | Dragon-Forged Core |
| 963 | dragon_forged_fang | Dragon-Forged Fang |
| 964 | dragon_forged_horn | Dragon-Forged Horn |
| 965 | dragon_forged_powder | Dragon-Forged Powder |
| 966 | dragon_horn_beetle | Dragon Horn Beetle |
| 967 | dragon_horn_mushroom | Dragon Horn Mushroom |
| 968 | dragon_horn_mushroom_with_thyme | Dragon Horn Mushroom with Thyme |
| 969 | dragon_pact_tablet | Dragon Pact Tablet |
| 972 | dragon_priestess_fountain | Dragon Priestess Fountain |
| 973 | dragon_scale | Dragon Scale |
| 975 | dragon_statue_replica | Dragon Statue |
| 976 | dragonfly | Dragonfly |
| 977 | dragonsworn_armor_equipment | Dragonsworn Cuirass |
| 978 | dragonsworn_chest_and_pants | Dragonsworn Chest and Pants |
| 979 | dragonsworn_greaves | Dragonsworn Greaves |
| 980 | dragonsworn_greaves_equipment | Dragonsworn Greaves |
| 981 | dragonsworn_helmet | Dragonsworn Helmet |
| 982 | dragonsworn_helmet_equipment | Dragonsworn Helmet |
| 983 | dragonsworn_legplates_equipment | Dragonsworn Cuisses |
| 984 | dried_squid | Dried Squid |
| 985 | duck_egg | Duck Egg |
| 986 | duck_feather | Duck Feather |
| 987 | duck_mayonnaise | Duck Mayonnaise |
| 988 | duck_plushie | Duck Plushie |
| 989 | dungeon_fountain_health | Fountain Health 33 | **GLITCHED** |
| 990 | dungeon_fountain_stamina | Fountain Stamina 33 | **GLITCHED** |
| 991 | earth_eel | Earth Eel |
| 992 | earth_infused_stone | Earth-Infused Stone |
| 993 | earth_tablet_rubbing | Earth Tablet Rubbing |
| 994 | earthshroom | Earthshroom |
| 995 | egg | Chicken Egg |
| 1001 | emerald_bed | Emerald Double Bed |
| 999 | emerald_chair | Emerald Chair |
| 1002 | emerald_horned_charger | Emerald-Horned Charger |
| 1004 | emerald_nightstand | Emerald Nightstand |
| 1006 | emerald_rug | Emerald Rug |
| 1008 | emerald_wallpaper | Emerald Wallpaper |
| 1009 | espresso | Espresso |
| 1012 | espresso_cup | Espresso Cup |
| 1013 | espresso_machine | Espresso Machine |
| 1014 | essence_blossom | Essence Blossom |
| 1015 | essence_drop | Essence Drop |
| 1016 | essence_stone_giant | Giant Essence Stone |
| 1017 | essence_stone_large | Large Essence Stone |
| 1018 | essence_stone_medium | Medium Essence Stone |
| 1019 | essence_stone_small | Small Essence Stone |
| 1020 | essence_stone_tiny | Tiny Essence Stone |
| 1021 | ethereal_grass | Ethereal Grass |
| 1024 | explorer_bed | Explorer Bed |
| 1027 | explorer_chair | Explorer Chair |
| 1030 | explorer_double_bed | Explorer Double Bed |
| 1033 | explorer_flooring | Explorer Flooring |
| 1036 | explorer_globe | Explorer Globe |
| 1039 | explorer_lamp | Explorer Lamp |
| 1042 | explorer_rug | Explorer Rug |
| 1045 | explorer_trunk_table | Explorer Trunk Table |
| 1048 | explorer_wallpaper | Explorer Wallpaper |
| 1049 | extra_large_decorative_pumpkin_black | Extra Large Decorative Pumpkin |
| 1050 | extra_large_decorative_pumpkin_green | Extra Large Decorative Pumpkin |
| 1051 | extra_large_decorative_pumpkin_orange | Extra Large Decorative Pumpkin |
| 1052 | extra_large_decorative_pumpkin_purple | Extra Large Decorative Pumpkin |
| 1053 | extra_large_decorative_pumpkin_white | Extra Large Decorative Pumpkin |
| 1054 | extra_large_decorative_pumpkin_yellow | Extra Large Decorative Pumpkin |
| 1055 | faceted_rock_gem | Faceted Rock Gem |
| 1056 | fairy_bee | Fairy Bee |
| 1057 | fairy_syrup | Fairy Syrup |
| 1058 | fake_essence | Essence |
| 1063 | fall_crop_sign_scroll_bundle | Fall Crop Sign Scroll Bundle |
| 1067 | fall_double_bed | Fall Double Bed |
| 1062 | fall_single_bed | Fall Single Bed |
| 1071 | fall_table | Fall Dining Table |
| 1072 | family_crest_pendant | Family Crest Pendant |
| 1073 | farm_bridge | Starter Farm Bridge |
| 1074 | farm_house_calendar | Farmhouse Calendar |
| 1075 | fast_food | Fast Food |
| 1076 | feather | Rooster Feather |
| 1077 | fennel | Fennel |
| 1078 | fiber | Fiber |
| 1079 | fiddlehead | Fiddlehead |
| 1080 | field_clear | Grass Tile Clear |
| 1081 | field_flower | Grass Tile Flower |
| 1082 | field_flower_two | Grass Tile 2 Flowers |
| 1083 | field_grass | Grass Tile Blades |
| 1084 | field_grass_two | Field Grass 2 Blades |
| 1085 | fire_crystal | Fire Crystal |
| 1086 | fire_snake_oil | Fire Snake Oil |
| 1087 | fire_tablet_rubbing | Fire Tablet Rubbing |
| 1088 | fire_wasp | Fire Wasp |
| 1089 | firefly | Firefly |
| 1094 | firefly_wall_string_lights | Firefly Wall String Lights |
| 1095 | firesail_fish | Firesail Fish |
| 1096 | fish_bait_common | Common Fish Bait |
| 1097 | fish_bait_rare | Rare Fish Bait |
| 1098 | fish_bait_uncommon | Uncommon Fish Bait |
| 1099 | fish_plush_toy | Fish Plush Toy |
| 1100 | fish_skewer | Fish Skewers |
| 1101 | fish_stew | Fish Stew |
| 1102 | fish_tacos | Fish Tacos |
| 1104 | fishing_bed | Fishing Bed |
| 1106 | fishing_chair | Fishing Chair |
| 1108 | fishing_double_bed | Fishing Double Bed |
| 1110 | fishing_flooring | Fishing Flooring |
| 1112 | fishing_lamp | Lighthouse Lamp |
| 1114 | fishing_oar_wall_decor | Oar Wall Decor |
| 1117 | fishing_rod_copper | Copper Fishing Rod |
| 1118 | fishing_rod_gold | Gold Fishing Rod |
| 1119 | fishing_rod_iron | Iron Fishing Rod |
| 1120 | fishing_rod_mistril | Mistril Fishing Rod |
| 1121 | fishing_rod_silver | Silver Fishing Rod |
| 1122 | fishing_rod_worn | Worn Fishing Rod |
| 1124 | fishing_rug | Fishing Rug |
| 1126 | fishing_table | Fishing Table |
| 1128 | fishing_tank | Fish Tank |
| 1130 | fishing_trophy_shelf | Fishing Trophy Wall Shelf |
| 1116 | fishing_wall_plaque | Faux Fish Wall Plaque |
| 1132 | fishing_wallpaper | Fishing Wallpaper |
| 1134 | fishing_window | Porthole Window |
| 1135 | flame_pepper | Flame Pepper |
| 1136 | flathead_catfish | Flathead Catfish |
| 1137 | flint_arrowhead | Flint Arrowhead |
| 1138 | floral_tea | Floral Tea |
| 1139 | flour | Flour |
| 1140 | flower_bee | Flower Bee |
| 1141 | flower_crown_beetle | Flower Crown Beetle |
| 1142 | fog_orchid | Fog Orchid |
| 1143 | forest_perch | Forest Perch |
| 1144 | forge | Forge |
| 1145 | forge_mistril | Mistril Forge |
| 1146 | fossilized_egg | Fossilized Egg |
| 1147 | fossilized_mandrake_root | Fossilized Mandrake Root |
| 1148 | freshwater_eel | Freshwater Eel |
| 1149 | freshwater_oyster | Freshwater Oyster |
| 1150 | fried_rice | Fried Rice |
| 1151 | frog | Frog |
| 1152 | frost_flutter_butterfly | Frost Flutter Butterfly |
| 1153 | frost_lily | Frost Lily |
| 1154 | fur_bee | Fur Bee |
| 1155 | fuzzy_moth | Fuzzy Moth |
| 1156 | gar | Gar |
| 1157 | garlic | Garlic |
| 1158 | garlic_bread | Garlic Bread |
| 1159 | gathering_basket | Gathering Basket |
| 1160 | gazer | Gazer |
| 1161 | gazpacho | Gazpacho |
| 1162 | gem_shard_caterpillar | Gem Shard Caterpillar |
| 1165 | gemstone_bridge | Gemstone Bridge |
| 1166 | giant_fish_scale | Giant Fish Scale |
| 1167 | giant_jellyfish | Giant Jellyfish |
| 1168 | giant_koi | Giant Koi |
| 1169 | giant_tilapia | Giant Tilapia |
| 1170 | giant_worm | Giant Worm |
| 1171 | glass | Glass |
| 1172 | glass_absinthe | Absinthe Glass |
| 1173 | glass_cocktail | Cocktail Glass |
| 1174 | glass_lemonade | Lemonade Glass |
| 1175 | glass_milk | Milk Glass |
| 1176 | glass_water | Water Glass |
| 1177 | glass_whisky | Whisky Glass |
| 1178 | glowberry | Glowberry |
| 1179 | glowberry_cookies | Glowberry Cookies |
| 1180 | glowing_mushroom | Glowing Mushroom |
| 1181 | goby | Goby |
| 1182 | gold_alpaca_trophy | Gold Alpaca Trophy |
| 1183 | gold_armor | Gold Armor |
| 1184 | gold_cow_trophy | Gold Cow Trophy |
| 1185 | gold_floor_mirror | Gold Floor Mirror |
| 1186 | gold_greaves | Gold Greaves |
| 1187 | gold_helmet | Gold Helmet |
| 1188 | gold_horse_trophy | Gold Horse Trophy |
| 1189 | gold_ingot | Gold Ingot |
| 1190 | gold_legplates | Gold Legplates |
| 1191 | gold_ring | Gold Ring |
| 1192 | gold_sheep_trophy | Gold Sheep Trophy |
| 1193 | gold_wall_mirror | Gold Wall Mirror |
| 1194 | golden_alpaca_wool | Golden Alpaca Wool |
| 1195 | golden_bristle | Golden Bristle |
| 1196 | golden_bull_horn | Golden Bull Horn |
| 1197 | golden_butter | Golden Butter |
| 1198 | golden_cheese | Golden Cheese |
| 1199 | golden_cheesecake | Golden Cheesecake |
| 1200 | golden_cookies | Golden Cookies |
| 1201 | golden_cow_milk | Golden Milk |
| 1202 | golden_duck_egg | Golden Duck Egg |
| 1203 | golden_duck_feather | Golden Duck Feather |
| 1204 | golden_duck_mayonnaise | Golden Duck Mayonnaise |
| 1205 | golden_eel | Golden Eel |
| 1206 | golden_egg | Golden Egg |
| 1207 | golden_feather | Golden Rooster Feather |
| 1208 | golden_horse_hair | Golden Horse Hair |
| 1209 | golden_mayonnaise | Golden Mayonnaise |
| 1210 | golden_rabbit_wool | Golden Rabbit Wool |
| 1211 | golden_sheep_wool | Golden Sheep Wool |
| 1212 | golden_shiner | Golden Shiner |
| 1213 | goldfish | Goldfish |
| 1214 | grape_juice | Grape Juice |
| 1215 | grass_seed | Grass Seed |
| 1216 | grasshopper | Grasshopper |
| 1217 | grayling | Grayling |
| 1218 | green_bottle | Green Bottle |
| 1219 | green_tea | Green Tea |
| 1220 | grilled_cheese | Grilled Cheese |
| 1221 | grilled_corn | Grilled Corn |
| 1222 | grilled_eel_rice_bowl | Grilled Eel Rice Bowl |
| 1223 | grouper | Grouper |
| 1226 | gryphon_statue_replica | Gryphon Statue Replica |
| 1227 | hake | Hake |
| 1228 | halibut | Halibut |
| 1229 | hard_boiled_egg | Hard Boiled Egg |
| 1230 | hard_wood | Hard Wood |
| 1231 | hardened_essence | Hardened Essence |
| 1232 | harvest_day_pie | Special Pie | **GLITCHED** |
| 1233 | harvest_festival_arch | Harvest Festival Arch |
| 1234 | harvest_festival_centerpiece | Harvest Festival Centerpiece |
| 1235 | harvest_plate | Harvest Plate |
| 1237 | haunted_attic_armoire | Haunted Attic Armoire |
| 1239 | haunted_attic_bed | Haunted Attic Bed |
| 1241 | haunted_attic_candle_cluster | Melted Candle Cluster |
| 1243 | haunted_attic_candle_single | Melted Candle |
| 1245 | haunted_attic_chair | Haunted Attic Chair |
| 1247 | haunted_attic_double_bed | Haunted Attic Double Bed |
| 1249 | haunted_attic_dress_form | Haunted Attic Dress Form |
| 1251 | haunted_attic_flooring | Haunted Attic Flooring |
| 1253 | haunted_attic_nightstand | Haunted Attic Nightstand |
| 1255 | haunted_attic_rocking_chair | Haunted Attic Rocking Chair |
| 1257 | haunted_attic_table | Haunted Attic Table |
| 1259 | haunted_attic_wall_cobweb_left | Cobweb Left |
| 1261 | haunted_attic_wall_cobweb_right | Cobweb Right |
| 1263 | haunted_attic_wall_shelf | Haunted Attic Wall Shelf |
| 1265 | haunted_attic_wall_window | Haunted Attic Window |
| 1267 | haunted_attic_wallpaper | Haunted Attic Wallpaper |
| 1268 | hay | Hay |
| 1269 | haydens_weathervane | Hayden's Weathervane |
| 1270 | heal_syrup | Healing Syrup |
| 1271 | heart_crystal | Heart Crystal |
| 1272 | heart_shaped_alpaca_treat | Heart Shaped Alpaca Treat |
| 1273 | heart_shaped_capybara_treat | Heart Shaped Capybara Treat |
| 1274 | heart_shaped_chicken_treat | Heart Shaped Chicken Treat |
| 1275 | heart_shaped_cow_treat | Heart Shaped Cow Treat |
| 1276 | heart_shaped_duck_treat | Heart Shaped Duck Treat |
| 1277 | heart_shaped_horse_treat | Heart Shaped Horse Treat |
| 1278 | heart_shaped_rabbit_treat | Heart Shaped Rabbit Treat |
| 1279 | heart_shaped_sheep_treat | Heart Shaped Sheep Treat |
| 1280 | heather | Heather |
| 1281 | heavy_mist | Heavy Mist |
| 1282 | herb_butter_pasta | Herb Butter Pasta |
| 1283 | herb_salad | Herb Salad |
| 1284 | hermit_crab | Hermit Crab |
| 1285 | hermit_snail | Hermit Snail |
| 1288 | hero_shield | Champion's Shield |
| 1289 | heros_ring | Hero's Ring |
| 1290 | herring | Herring |
| 1293 | herringbone_tile_floor | Herringbone Tile Floor |
| 1294 | hidden_beetle | Hidden Beetle |
| 1295 | hoe_copper | Copper Hoe |
| 1296 | hoe_gold | Gold Hoe |
| 1297 | hoe_iron | Iron Hoe |
| 1298 | hoe_mistril | Mistril Hoe |
| 1299 | hoe_silver | Silver Hoe |
| 1300 | hoe_worn | Worn Hoe |
| 1301 | holly | Holly |
| 1302 | honey | Honey |
| 1303 | honey_curry | Honey Curry |
| 1304 | honey_deluxe | Deluxe Honey |
| 1305 | honey_legendary | Legendary Honey |
| 1306 | honey_premium | Premium Honey |
| 1307 | honeycomb_shelf | Honeycomb Shelf |
| 1308 | horse_hair | Horse Hair |
| 1309 | horse_mackerel | Horse Mackerel |
| 1310 | horse_plushie | Horse Plushie |
| 1311 | horse_potion | MISSING | **GLITCHED** |
| 1312 | horseradish | Horseradish |
| 1313 | horseradish_salmon | Horseradish Salmon |
| 1314 | hot_cocoa | Hot Chocolate |
| 1315 | hot_potato | Hot Potato |
| 1316 | hot_toddy | Hot Toddy |
| 1317 | humble_pie | Humble Pie |
| 1318 | hummingbird_hawk_moth | Hummingbird Hawk Moth |
| 1319 | hydrangea | Hydrangea |
| 1320 | ice_block | Ice Block |
| 1321 | ice_cream_sundae | Ice Cream Sundae |
| 1322 | ice_snake_oil | Ice Snake Oil |
| 1323 | iced_coffee | Iced Coffee |
| 1324 | icehopper | Icehopper |
| 1325 | inchworm | Inchworm |
| 1326 | incredibly_hot_pot | Incredibly Hot Pot |
| 1327 | inn_candle | Inn Candle |
| 1328 | inn_counter | Inn Counter |
| 1329 | inn_cross_stitch | Inn Cross Stitch |
| 1330 | inn_kitchen_shelves | Inn Kitchen Shelves |
| 1331 | inn_menu | Inn Menu |
| 1332 | inn_stool | Inn Stool |
| 1333 | inn_tile_flooring | Inn Tile Flooring |
| 1334 | inn_wooden_flooring | Inn Wooden Flooring |
| 1337 | insect_flooring | Insect Flooring |
| 1340 | insect_rug | Insect Rug |
| 1343 | insect_wallpaper | Insect Wallpaper |
| 1344 | iris | Iris |
| 1345 | iron_armor | Iron Armor |
| 1346 | iron_fish | Iron Fish |
| 1347 | iron_greaves | Iron Greaves |
| 1348 | iron_helmet | Iron Helmet |
| 1349 | iron_ingot | Iron Ingot |
| 1350 | iron_legplates | Iron Legplates |
| 1351 | iron_ring | Iron Ring |
| 1352 | jam_sandwich | Jam Sandwich |
| 1353 | jar_large | Large Jar |
| 1354 | jar_medium | Medium Jar |
| 1355 | jar_pair | Pair of Jars |
| 1356 | jar_small | Small Jar |
| 1357 | jasmine | Jasmine |
| 1358 | jasmine_tea | Jasmine Tea |
| 1359 | jellyfish | Jellyfish |
| 1360 | jewel_beetle | Jewel Beetle |
| 1361 | journal | Diary |
| 1362 | killifish | Killifish |
| 1363 | king_crab | King Crab |
| 1364 | kitchen_garlic_braid | Garlic Braid |
| 1365 | kitchen_hanging_onion_basket | Hanging Onion Basket |
| 1367 | kitchen_herb_planter | Herb Planter |
| 1371 | kitchen_mixing_bowls | Mixing Bowls |
| 1373 | kitchen_oil_and_vinegar_basket | Oil & Vinegar Basket |
| 1374 | kitchen_salt_and_pepper_shakers | Salt & Pepper Shakers |
| 1378 | kitchen_scale | Kitchen Scale |
| 1382 | kitchen_utensil_holder | Utensil Holder |
| 1383 | koi | Koi |
| 1384 | ladder | House Ladder |
| 1385 | ladybug | Ladybug |
| 1388 | ladybug_table | Ladybug Table |
| 1389 | lake_chub | Lake Chub |
| 1390 | lake_trout | Lake Trout |
| 1391 | lamprey | Lamprey |
| 1392 | lantern_moth | Lantern Moth |
| 1393 | large_animal_ball_court | Large Animal Ball Court |
| 1394 | large_barn_black_blueprint | Large White Barn Blueprint |
| 1395 | large_barn_red_blueprint | Large Red Barn Blueprint |
| 1396 | large_coop_black_blueprint | Large White Coop Blueprint |
| 1397 | large_coop_red_blueprint | Large Red Coop Blueprint |
| 1398 | large_decorative_pumpkin_black | Large Decorative Pumpkin |
| 1399 | large_decorative_pumpkin_green | Large Decorative Pumpkin |
| 1400 | large_decorative_pumpkin_orange | Large Decorative Pumpkin |
| 1401 | large_decorative_pumpkin_purple | Large Decorative Pumpkin |
| 1402 | large_decorative_pumpkin_white | Large Decorative Pumpkin |
| 1403 | large_decorative_pumpkin_yellow | Large Decorative Pumpkin |
| 1411 | large_fluffy_rug | Large Fluffy Rug |
| 1412 | large_greenhouse_white_blueprint | Large White Greenhouse Blueprint |
| 1413 | large_greenhouse_wood_blueprint | Large Wood Greenhouse Blueprint |
| 1414 | large_mob_coin | coin | **GLITCHED** |
| 1417 | large_paving_stone | Large Paving Stone |
| 1418 | large_pouch | Bag Upgrade |
| 1419 | large_stone_bundle | Large Bundle of Stone |
| 1420 | large_wood_bundle | Large Bundle of Wood |
| 1421 | latte | Latte |
| 1430 | lava_caves_bed | Obsidian Double Bed |
| 1428 | lava_caves_chair | Obsidian Chair |
| 1436 | lava_caves_chest | Obsidian Storage Chest |
| 1422 | lava_caves_diamond_rock | Ornamental Diamond Rock |
| 1432 | lava_caves_fence | Obsidian Fence |
| 1434 | lava_caves_floor_lamp | Obsidian Floor Lamp |
| 1423 | lava_caves_flooring | Lava Rock Floor |
| 1424 | lava_caves_gold_node | Ornamental Gold Node |
| 1439 | lava_caves_stone_arch | Lava Rock Arch |
| 1438 | lava_caves_table | Obsidian Table |
| 1440 | lava_caves_wallpaper | Lava Rock Wall |
| 1441 | lava_chestnuts | Lava Chestnuts |
| 1442 | lava_piranha | Lava Piranha |
| 1443 | lava_snail | Lava Snail |
| 1444 | lavender_tea | Lavender Tea |
| 1445 | leaf_fish | Leaf Fish |
| 1446 | leafhopper | Leafhopper |
| 1447 | lemon | Lemon |
| 1448 | lemon_pie | Lemon Pie |
| 1449 | lemonade | Lemonade |
| 1450 | lightning_dragonfly | Lightning Dragonfly |
| 1451 | lightning_fish | Lightning Fish |
| 1452 | lilac | Lilac |
| 1453 | loach | Loach |
| 1454 | loaded_baked_potato | Loaded Baked Potato |
| 1455 | loam_caterpillar | Loam Caterpillar |
| 1456 | lobster | Lobster |
| 1457 | lobster_roll | Lobster Roll |
| 1458 | lost_crown_of_aldaria | Lost Crown of Aldaria |
| 1459 | lovely_cottage_scroll_bundle | Lovely Cottage Scroll Bundle |
| 1460 | lovely_seashell | Lovely Seashell |
| 1461 | low_tide_flooring | Low Tide Flooring |
| 1462 | luminescent_crab | Luminescent Crab |
| 1463 | luna_moth | Luna Moth |
| 1464 | lurid_colored_drink | MISSING | **GLITCHED** |
| 1465 | mackerel | Mackerel |
| 1466 | mackerel_sashimi | Mackerel Sashimi |
| 1467 | magic_key | Magic Key |
| 1468 | magma_beetle | Magma Beetle |
| 1469 | mana_potion | Mana Potion |
| 1470 | maples_drawing | Maple's Aldarian Star Drawing |
| 1471 | marigold | Marigold |
| 1472 | marmalade | Marmalade |
| 1473 | massive_minnow | Massive Minnow |
| 1474 | mayonnaise | Mayonnaise |
| 1475 | medium_barn_black_blueprint | Medium White Barn Blueprint |
| 1476 | medium_barn_red_blueprint | Medium Red Barn Blueprint |
| 1477 | medium_coop_black_blueprint | Medium White Coop Blueprint |
| 1478 | medium_coop_red_blueprint | Medium Red Coop Blueprint |
| 1479 | medium_decorative_pumpkin_black | Medium Decorative Pumpkin |
| 1480 | medium_decorative_pumpkin_green | Medium Decorative Pumpkin |
| 1481 | medium_decorative_pumpkin_orange | Medium Decorative Pumpkin |
| 1482 | medium_decorative_pumpkin_purple | Medium Decorative Pumpkin |
| 1483 | medium_decorative_pumpkin_white | Medium Decorative Pumpkin |
| 1484 | medium_decorative_pumpkin_yellow | Medium Decorative Pumpkin |
| 1485 | medium_mob_coin | coin | **GLITCHED** |
| 1491 | mermaid_bed | Mermaid Double Bed |
| 1492 | mermaids_comb | Mermaid's Comb |
| 1493 | message_in_a_bottle | Message in a Bottle |
| 1494 | metal_leaf | Metal Leaf |
| 1495 | meteorite | Meteorite |
| 1496 | middlemist | Middlemist |
| 1497 | mine_cricket | Mine Cricket |
| 1508 | minecart | Minecart |
| 1498 | miners_copper_node | Ornamental Copper Node |
| 1500 | miners_crate_chair | Mines Crate Chair |
| 1502 | miners_crate_chest | Mines Storage Chest |
| 1504 | miners_floor_lamp | Mines Floor Lamp |
| 1505 | miners_flooring | Mines Flooring |
| 1506 | miners_helmet | Miner's Helmet |
| 1509 | miners_mushroom_stew | Miner's Mushroom Stew |
| 1510 | miners_pickaxe | Miner's Pickaxe |
| 1511 | miners_ruby_rock | Ornamental Ruby Rock |
| 1512 | miners_rucksack | Miner's Rucksack |
| 1514 | miners_shelves | Mines Shelf |
| 1515 | miners_slab | Miner's Slab |
| 1517 | miners_wall_lamp | Mines Wall Lamp |
| 1518 | miners_wallpaper | Mines Wall |
| 1519 | mines_mussels | Mines Mussels |
| 1520 | mines_wooden_arch | Mines Arch |
| 1521 | mini_mill | Mini Mill |
| 1522 | mini_whale_shark | Mini-Whale Shark |
| 1523 | minnow | Minnow |
| 1524 | mint_gimlet | Mint Gimlet |
| 1525 | mist_bed_v1 | Mist Bed |
| 1526 | mist_bed_v2 | Mist Bed |
| 1527 | mist_bed_v3 | Mist Bed |
| 1528 | mist_bed_v4 | Mist Bed |
| 1529 | mist_crystal | Mist Crystal |
| 1530 | mist_cushion_v1 | Mist Cushion |
| 1531 | mist_cushion_v2 | Mist Cushion |
| 1532 | mist_cushion_v3 | Mist Cushion |
| 1533 | mist_cushion_v4 | Mist Cushion |
| 1534 | mist_double_bed_v1 | Mist Double Bed |
| 1535 | mist_double_bed_v2 | Mist Double Bed |
| 1536 | mist_double_bed_v3 | Mist Double Bed |
| 1537 | mist_double_bed_v4 | Mist Double Bed |
| 1538 | mist_flute | Mist Flute |
| 1539 | mist_held_item | Invalid Item | **GLITCHED** |
| 1540 | mist_lamp_v1 | Mist Lamp |
| 1541 | mist_lamp_v2 | Mist Lamp |
| 1542 | mist_lamp_v3 | Mist Lamp |
| 1543 | mist_lamp_v4 | Mist Lamp |
| 1544 | mist_pet_bed_v1 | Mist Pet Bed |
| 1545 | mist_pet_bed_v2 | Mist Pet Bed |
| 1546 | mist_pet_bed_v3 | Mist Pet Bed |
| 1547 | mist_pet_bed_v4 | Mist Pet Bed |
| 1548 | mist_rug_v1 | Mist Rug |
| 1549 | mist_rug_v2 | Mist Rug |
| 1550 | mist_rug_v3 | Mist Rug |
| 1551 | mist_rug_v4 | Mist Rug |
| 1552 | mist_scroll | Mist Scroll |
| 1553 | mist_storage_chest_v1 | Mist Storage Chest |
| 1554 | mist_storage_chest_v2 | Mist Storage Chest |
| 1555 | mist_storage_chest_v3 | Mist Storage Chest |
| 1556 | mist_storage_chest_v4 | Mist Storage Chest |
| 1557 | mistmoth | Mistmoth |
| 1558 | mistria_history_book | Mistria History Book |
| 1559 | mistria_history_scroll | Caldosian Decorative Scroll |
| 1560 | mistria_wall_map | Mistria Wall Map |
| 1561 | mistril_armor | Mistril Armor |
| 1562 | mistril_boots | Mistril Greaves |
| 1563 | mistril_helmet | Mistril Helmet |
| 1564 | mistril_ingot | Mistril Ingot |
| 1565 | mistril_legplates | Mistril Legplates |
| 1566 | mistril_ring | Mistril Ring |
| 1567 | misty_black_mirror | Misty Black Mirror |
| 1568 | misty_feather_quill | Misty Feather Quill |
| 1569 | mixed_fruit_juice | Mixed Fruit Juice |
| 1570 | mob_coin | coin | **GLITCHED** |
| 1571 | mocha | Mocha |
| 1572 | monarch_butterfly | Monarch Butterfly |
| 1573 | monster_block | Monster Block |
| 1574 | monster_cookie | Monster Cookies |
| 1575 | monster_core | Monster Core |
| 1576 | monster_fang | Monster Fang |
| 1577 | monster_hoop | Monster Hoop |
| 1578 | monster_horn | Monster Horn |
| 1579 | monster_mash | Monster Mash |
| 1580 | monster_powder | Monster Powder |
| 1581 | monster_shell | Monster Shell |
| 1582 | monster_whisker | Monster Whisker |
| 1583 | monster_wing | Monster Wing |
| 1584 | mont_blanc | Mont Blanc |
| 1585 | moon_fruit | Moon Fruit |
| 1586 | moon_fruit_cake | Moon Fruit Cake |
| 1589 | moon_gate | Moon Gate |
| 1590 | moonlight_bee | Moonlight Bee |
| 1591 | morel_mushroom | Morel Mushroom |
| 1592 | mote_firefly | Mote Firefly |
| 1595 | moth_lamp | Moth Lamp |
| 1596 | mournful_clown_painting | Mournful Clown Painting |
| 1597 | mullet | Mullet |
| 1598 | mushroom_brew | Mushroom Brew |
| 1599 | mushroom_rice | Mushroom Rice |
| 1600 | mushroom_steak_dinner | Mushroom Steak Dinner |
| 1601 | muskie | Muskie |
| 1602 | muttering_cube | Muttering Cube |
| 1603 | narrows_moss | Narrows Moss |
| 1604 | net_copper | Copper Net |
| 1605 | net_gold | Gold Net |
| 1606 | net_iron | Iron Net |
| 1607 | net_mistril | Mistril Net |
| 1608 | net_silver | Silver Net |
| 1609 | net_worn | Worn Net |
| 1610 | nettle | Nettle |
| 1611 | newt | Newt |
| 1612 | night_queen | Night Queen |
| 1613 | noodles | Noodles |
| 1615 | notepad | Notepad |
| 1616 | oarfish | Oarfish |
| 1622 | obelisk | Obelisk |
| 1623 | obsidian | Obsidian |
| 1624 | obsidian_blade | Obsidian Blade |
| 1625 | ocarina_sprite_statue | Animal Sprite Statue |
| 1626 | ocean_sunfish | Ocean Sunfish |
| 1627 | octopus | Octopus |
| 1628 | octopus_plushie | Octopus Plushie |
| 1629 | oil | Oil |
| 1630 | omelet | Omelet |
| 1631 | onion | Onion |
| 1632 | onion_soup | Onion Soup |
| 1633 | orange | Orange |
| 1634 | orange_juice | Orange Juice |
| 1635 | orchid_mantis | Orchid Mantis |
| 1636 | ore_copper | Copper Ore |
| 1637 | ore_diamond | Diamond |
| 1638 | ore_emerald | Emerald |
| 1639 | ore_gold | Gold Ore |
| 1640 | ore_iron | Iron Ore |
| 1641 | ore_mistril | Mistril Ore |
| 1642 | ore_pink_diamond | Pink Diamond |
| 1643 | ore_ruby | Ruby |
| 1644 | ore_sapphire | Sapphire |
| 1645 | ore_silver | Silver Ore |
| 1646 | ore_stone | Stone |
| 1647 | oregano | Oregano |
| 1648 | ornamental_iron_node | Ornamental Iron Node |
| 1649 | ornamental_sapphire_rock | Ornamental Sapphire Rock |
| 1652 | ornate_flagstone | Ornate Flagstone |
| 1655 | ornate_rug_large_rectangle | Ornate Large Rectangle Rug |
| 1658 | ornate_rug_large_round | Ornate Large Round Rug |
| 1661 | ornate_rug_large_square | Ornate Large Square Rug |
| 1664 | ornate_rug_medium_round | Ornate Medium Round Rug |
| 1667 | ornate_rug_medium_square | Ornate Medium Square Rug |
| 1670 | ornate_rug_small | Ornate Small Rug |
| 1671 | oyster_mushroom | Oyster Mushroom |
| 1672 | paddlefish | Paddlefish |
| 1673 | pan_fried_bream | Pan Fried Bream |
| 1674 | pan_fried_salmon | Pan-fried Salmon |
| 1675 | pan_fried_snapper | Pan-fried Snapper |
| 1676 | paper | Paper |
| 1677 | paper_pondshell | Paper Pondshell |
| 1678 | paper_stack | Paper Stack |
| 1679 | parchment_moth | Parchment Moth |
| 1680 | peach | Peach |
| 1681 | peaches_and_cream | Peaches and Cream |
| 1682 | pear | Pear |
| 1683 | pearl_clam | Pearl Clam |
| 1684 | peas | Peas |
| 1685 | peat | Peat |
| 1686 | pebble_minnow | Pebble Minnow |
| 1687 | perch | Perch |
| 1688 | perch_risotto | Perch Risotto |
| 1689 | perfect_copper_ore | Perfect Copper Ore |
| 1690 | perfect_diamond | Perfect Diamond |
| 1691 | perfect_emerald | Perfect Emerald |
| 1692 | perfect_gold_ore | Perfect Gold Ore |
| 1693 | perfect_iron_ore | Perfect Iron Ore |
| 1694 | perfect_mistril_ore | Perfect Mistril Ore |
| 1695 | perfect_pink_diamond | Perfect Pink Diamond |
| 1696 | perfect_ruby | Perfect Ruby |
| 1697 | perfect_sapphire | Perfect Sapphire |
| 1698 | perfect_silver_ore | Perfect Silver Ore |
| 1699 | perpetual_soup_pot | Perpetual Soup Pot |
| 1700 | pet_cosmetic | MISSING | **DISABLED** |
| 1701 | pet_skin_sapling | Pet Skin: Sapling |
| 1702 | petalhopper | Petalhopper |
| 1703 | petrified_wood | Petrified Wood |
| 1704 | pick_axe_copper | Copper Pickaxe |
| 1705 | pick_axe_gold | Gold Pickaxe |
| 1706 | pick_axe_iron | Iron Pickaxe |
| 1707 | pick_axe_mistril | Mistril Pickaxe |
| 1708 | pick_axe_silver | Silver Pickaxe |
| 1709 | pick_axe_worn | Worn Pickaxe |
| 1710 | picket_fence | Picket Fence |
| 1713 | picnic_basket | Picnic Basket |
| 1714 | picnic_donut_plate | Picnic Display Donuts |
| 1715 | picnic_hamburger_plate | Picnic Display Burgers |
| 1718 | picnic_pie_plate | Picnic Display Pie |
| 1721 | picnic_place_setting | Picnic Place Setting |
| 1724 | picnic_rug | Picnic Blanket |
| 1725 | picnic_sandwich_plate | Picnic Display Sandwiches |
| 1726 | picnic_set | Picnic Set |
| 1729 | picnic_sunflower_vase | Picnic Sunflower Vase |
| 1730 | pike | Pike |
| 1731 | pinecone | Pinecone |
| 1732 | pineshroom | Pineshroom |
| 1733 | pineshroom_toast | Pineshroom Toast |
| 1734 | pink_pet_bed | Pink Pet Bed |
| 1735 | pink_pet_dish | Pink Pet Dish |
| 1736 | pink_scallop_shell | Pink Scallop Shell |
| 1737 | pirate_anchor_v1 | Anchor |
| 1738 | pirate_anchor_v2 | Anchor |
| 1739 | pirate_counterfeit_gold_pile_v1 | Counterfeit Gold Pile |
| 1740 | pirate_counterfeit_gold_pile_v2 | Counterfeit Gold Pile |
| 1741 | pirate_nautical_compass_v1 | Nautical Compass |
| 1742 | pirate_nautical_compass_v2 | Nautical Compass |
| 1743 | pirate_ship_in_a_bottle_v1 | Ship in a Bottle |
| 1744 | pirate_ship_in_a_bottle_v2 | Ship in a Bottle |
| 1745 | pirate_wall_flag_v1 | Pirate Flag |
| 1746 | pirate_wall_flag_v2 | Pirate Flag |
| 1747 | pizza | Pizza |
| 1748 | plum_blossom | Plum Blossom |
| 1749 | poached_pear | Poached Pear |
| 1750 | poinsettia | Poinsettia |
| 1751 | poison_snake_oil | Poison Snake Oil |
| 1752 | polar_fly | Polar Fly |
| 1753 | pollock | Pollock |
| 1754 | pomegranate | Pomegranate |
| 1755 | pomegranate_juice | Pomegranate Juice |
| 1756 | pomegranate_sorbet | Pomegranate Sorbet |
| 1757 | pond_skater | Pond Skater |
| 1758 | pond_snail | Pond Snail |
| 1759 | porcelain_figurine | Porcelain Figure |
| 1760 | potato | Potato |
| 1761 | potato_soup | Potato Soup |
| 1762 | praying_mantis | Praying Mantis |
| 1763 | pudding | Pudding |
| 1764 | puddle_spider | Puddle Spider |
| 1765 | puffer_fish | Puffer Fish |
| 1766 | pumpkin | Pumpkin |
| 1767 | pumpkin_pie | Pumpkin Pie |
| 1768 | pumpkin_pile_black | Pumpkin Pile |
| 1769 | pumpkin_pile_green | Pumpkin Pile |
| 1770 | pumpkin_pile_orange | Pumpkin Pile |
| 1771 | pumpkin_pile_purple | Pumpkin Pile |
| 1772 | pumpkin_pile_white | Pumpkin Pile |
| 1773 | pumpkin_pile_yellow | Pumpkin Pile |
| 1774 | pumpkin_stew | Pumpkin Stew |
| 1775 | purple_mushroom | Purple Mushroom |
| 1776 | purse | Purse of Tesserae | **DISABLED** |
| 1777 | quality_hay | Quality Large Animal Feed |
| 1778 | quality_small_animal_feed | Quality Small Animal Feed |
| 1779 | queen_berry | Queen Berry |
| 1780 | queen_berry_pie | Queen Berry Pie |
| 1781 | queen_berry_trophy | Golden Queen Berry Trophy |
| 1782 | queens_birdwing | Queen's Birdwing |
| 1783 | question_mark_butterfly | Question Mark Butterfly |
| 1784 | quiche | Quiche |
| 1785 | rabbit_plushie | Rabbit Plushie |
| 1786 | rabbit_wool | Rabbit Wool |
| 1787 | rainbow_geode | Rainbow Geode |
| 1788 | rainbow_seaweed | Rainbow Seaweed |
| 1789 | rainbow_trout | Rainbow Trout |
| 1790 | razorback | Razorback |
| 1791 | really_round_rock | Really Round Rock |
| 1792 | recipe_scroll | Recipe: {} | **DISABLED** |
| 1793 | red_capybara_wall_ribbon | Red Capybara Wall Ribbon |
| 1794 | red_chicken_wall_ribbon | Red Chicken Wall Ribbon |
| 1795 | red_duck_wall_ribbon | Red Duck Wall Ribbon |
| 1796 | red_obsidian | Red Obsidian |
| 1797 | red_rabbit_wall_ribbon | Red Rabbit Wall Ribbon |
| 1798 | red_snapper | Red Snapper |
| 1799 | red_snapper_sushi | Red Snapper Sushi |
| 1800 | red_toadstool | Red Toadstool |
| 1801 | red_wine | Red Wine |
| 1802 | redhead_worm | Redhead Worm |
| 1803 | refined_stone | Refined Stone |
| 1804 | relic_crab | Relic Crab |
| 1805 | restorative_syrup | Restorative Syrup |
| 1806 | rhinoceros_beetle | Rhinoceros Beetle |
| 1807 | rice | Rice |
| 1808 | rice_stalk | Rice Stalk |
| 1809 | riceball | Rice Ball |
| 1810 | ritual_beads | Ritual Beads |
| 1811 | ritual_chalice | Ritual Chalice |
| 1812 | ritual_incense_burner | Ritual Incense Burner |
| 1813 | ritual_scepter | Ritual Scepter |
| 1814 | ritual_tablet | Ritual Tablet |
| 1815 | river_snail | River Snail |
| 1816 | roach | Roach |
| 1817 | roasted_cauliflower | Roasted Cauliflower |
| 1818 | roasted_chestnuts | Roasted Chestnuts |
| 1819 | roasted_rice_tea | Roasted Rice Tea |
| 1820 | roasted_sweet_potato | Roasted Sweet Potato |
| 1821 | rock_bass | Rock Bass |
| 1822 | rock_guppy | Rock Guppy |
| 1823 | rock_roach | Rock Roach |
| 1824 | rock_salt | Rock Salt |
| 1825 | rock_statue | Rock Statue |
| 1826 | rock_with_a_hole | Rock with a Hole |
| 1827 | rockbiter | Rockbiter |
| 1828 | rockroot | Rockroot |
| 1829 | roly_poly | Roly Poly |
| 1830 | rose | Rose |
| 1831 | rose_hip | Rose Hip |
| 1832 | rose_tea | Rose Tea |
| 1833 | rosehip_jam | Rose Hip Jam |
| 1834 | rosemary | Rosemary |
| 1835 | rosemary_garlic_noodles | Rosemary Garlic Noodles |
| 1836 | rough_gemstone | Rough Gemstone |
| 1844 | round_cushioned_stool | Round Cushioned Stool |
| 1850 | royal_chest | Royal Chest |
| 1851 | rubber_fish | Rubber Fish |
| 1852 | ruins_flooring | Ruins Floor |
| 1853 | ruins_mistril_node | Ornamental Mistril Node |
| 1854 | ruins_pink_diamond_rock | Ornamental Pink Diamond Rock |
| 1855 | ruins_tablet_rubbing | Ruins Tablet Rubbing |
| 1856 | ruins_wallpaper | Ruins Wall |
| 1857 | rusted_shield | Rusted Shield |
| 1858 | rusted_treasure_chest | Rusted Treasure Chest |
| 1859 | rustic_cabin_scroll_bundle | Rustic Cabin Scroll Bundle |
| 1860 | ryis_lumber | Ryis' Lumber | **GLITCHED** |
| 1861 | sage | Sage |
| 1862 | saint_mantis | Saint Mantis |
| 1863 | salmon | Salmon |
| 1864 | salmon_sashimi | Salmon Sashimi |
| 1865 | salted_watermelon | Salted Watermelon |
| 1866 | sand_bug | Sand Bug |
| 1867 | sand_dollar | Sand Dollar |
| 1868 | sand_lance | Sand Lance |
| 1869 | sand_rug | Sand Rug |
| 1870 | sap | Sap |
| 1871 | sapling_apple | Apple Sapling |
| 1872 | sapling_cherry | Cherry Sapling |
| 1873 | sapling_lemon | Lemon Sapling |
| 1874 | sapling_oak | Oak Sapling |
| 1875 | sapling_orange | Orange Sapling |
| 1876 | sapling_peach | Peach Sapling |
| 1877 | sapling_pear | Pear Sapling |
| 1878 | sapling_pine | Pine Sapling |
| 1879 | sapling_pomegranate | Pomegranate Sapling |
| 1880 | sapphire_betta | Sapphire Betta |
| 1881 | sardine | Sardine |
| 1882 | sauger | Sauger |
| 1883 | saury | Saury |
| 1884 | sauteed_snow_peas | Sauteed Snow Peas |
| 1885 | scent_of_spring | Scent of Spring |
| 1886 | scrap_metal_boots | Scrap Metal Boots |
| 1887 | scrap_metal_chestpiece | Scrap Metal Chestpiece |
| 1888 | scrap_metal_helmet | Scrap Metal Helmet |
| 1889 | scrap_metal_pants | Scrap Metal Pants |
| 1890 | scrap_metal_ring | Scrap Metal Ring |
| 1891 | sea_bass | Sea Bass |
| 1892 | sea_bream | Sea Bream |
| 1893 | sea_bream_rice | Sea Bream Rice |
| 1894 | sea_glass | Sea Glass |
| 1895 | sea_grapes | Sea Grapes |
| 1896 | sea_scarab | Sea Scarab |
| 1897 | sea_urchin | Sea Urchin |
| 1898 | seafood_boil | Seafood Boil |
| 1899 | seafood_snow_pea_noodles | Seafood Snow Pea Noodles |
| 1900 | sealing_scroll | Sealing Scroll |
| 1901 | seaweed | Seaweed |
| 1902 | seaweed_salad | Seaweed Salad |
| 1903 | seed_basil | Basil Seed |
| 1904 | seed_beet | Beet Seed |
| 1905 | seed_broccoli | Broccoli Seed |
| 1906 | seed_burdock_root | Burdock Root Seed |
| 1907 | seed_cabbage | Cabbage Seed |
| 1908 | seed_carrot | Carrot Seed |
| 1909 | seed_catmint | Catmint Seed |
| 1910 | seed_cauliflower | Cauliflower Seed |
| 1911 | seed_celosia | Celosia Seed |
| 1912 | seed_chickpea | Chickpea Seed |
| 1913 | seed_chili_pepper | Chili Pepper Seed |
| 1914 | seed_chrysanthemum | Chrysanthemum Seed |
| 1915 | seed_corn | Corn Seed |
| 1916 | seed_cosmos | Cosmos Seed |
| 1917 | seed_cranberry | Cranberry Seed |
| 1918 | seed_cucumber | Cucumber Seed |
| 1919 | seed_daffodil | Daffodil Seed |
| 1920 | seed_daikon_radish | Daikon Radish Seed |
| 1921 | seed_daisy | Daisy Seed |
| 1922 | seed_dill | Dill Seed |
| 1923 | seed_frost_lily | Frost Lily Seed |
| 1924 | seed_garlic | Garlic Seed |
| 1925 | seed_heather | Heather Seed |
| 1926 | seed_iris | Iris Seed |
| 1927 | seed_jasmine | Jasmine Seed |
| 1928 | seed_lilac | Lilac Seed |
| 1929 | seed_magic_key | Seed of Balance |
| 1930 | seed_maker | Seed Maker |
| 1931 | seed_marigold | Marigold Seed |
| 1932 | seed_moon_fruit | Moon Fruit Seed |
| 1933 | seed_mystery_bag | Magic Seed |
| 1934 | seed_night_queen | Night Queen Seed |
| 1935 | seed_onion | Onion Seed |
| 1936 | seed_oregano | Oregano Seed |
| 1937 | seed_peas | Pea Seed |
| 1938 | seed_poinsettia | Poinsettia Seed |
| 1939 | seed_potato | Potato Seed |
| 1940 | seed_pumpkin | Pumpkin Seed |
| 1941 | seed_rice | Rice Seed |
| 1942 | seed_rosemary | Rosemary Seed |
| 1943 | seed_sage | Sage Seed |
| 1944 | seed_snapdragon | Snapdragon Seed |
| 1945 | seed_snow_peas | Snow Peas Seed |
| 1946 | seed_snowdrop_anemone | Snowdrop Anemone Seed |
| 1947 | seed_strawberry | Strawberry Seed |
| 1948 | seed_sugar_cane | Sugar Cane Seed |
| 1949 | seed_sunflower | Sunflower Seed |
| 1950 | seed_sweet_potato | Sweet Potato Seed |
| 1951 | seed_tea | Tea Seed |
| 1952 | seed_temple_flower | Temple Flower Seed |
| 1953 | seed_thyme | Thyme Seed |
| 1954 | seed_tomato | Tomato Seed |
| 1955 | seed_tulip | Tulip Seed |
| 1956 | seed_turnip | Turnip Seed |
| 1957 | seed_viola | Viola Seed |
| 1958 | seed_watermelon | Watermelon Seed |
| 1959 | seed_wheat | Wheat Seed |
| 1960 | seriously_square_stone | Seriously Square Stone |
| 1961 | sesame | Sesame |
| 1962 | sesame_broccoli | Sesame Broccoli |
| 1963 | sesame_tuna_bowl | Sesame Tuna Bowl |
| 1964 | shad | Shad |
| 1965 | shadow_bass | Shadow Bass |
| 1966 | shadow_flower | Shadow Flower |
| 1967 | shale_grass | Shale Grass |
| 1968 | shard_mass | Shard Mass |
| 1969 | shardfin | Shardfin |
| 1970 | shards | Shards |
| 1971 | shark | Shark |
| 1972 | sheep_plushie | Sheep Plushie |
| 1973 | sheep_wool | Sheep Wool |
| 1974 | shortcut_scroll | Shortcut Scroll |
| 1975 | shovel_copper | Copper Shovel |
| 1976 | shovel_gold | Gold Shovel |
| 1977 | shovel_iron | Iron Shovel |
| 1978 | shovel_mistril | Mistril Shovel |
| 1979 | shovel_silver | Silver Shovel |
| 1980 | shovel_worn | Worn Shovel |
| 1981 | shrimp | Shrimp |
| 1982 | silver_alpaca_trophy | Silver Alpaca Trophy |
| 1983 | silver_armor | Silver Armor |
| 1984 | silver_cow_trophy | Silver Cow Trophy |
| 1985 | silver_greaves | Silver Greaves |
| 1986 | silver_helmet | Silver Helmet |
| 1987 | silver_horse_trophy | Silver Horse Trophy |
| 1988 | silver_ingot | Silver Ingot |
| 1989 | silver_legplates | Silver Legplates |
| 1990 | silver_redhorse | Silver Redhorse |
| 1991 | silver_ring | Silver Ring |
| 1992 | silver_sheep_trophy | Silver Sheep Trophy |
| 1993 | silver_squid | Silver Squid |
| 1994 | simmered_daikon | Simmered Daikon |
| 1995 | singing_katydid | Singing Katydid |
| 1996 | sliced_turnip | Sliced Turnip |
| 1997 | small_animal_seesaw | Small Animal Seesaw |
| 1998 | small_barn_black_blueprint | Small White Barn Blueprint |
| 1999 | small_barn_red_blueprint | Small Red Barn Blueprint |
| 2007 | small_bathmat | Small Bathmat |
| 2008 | small_coop_black_blueprint | Small White Coop Blueprint |
| 2009 | small_coop_red_blueprint | Small Red Coop Blueprint |
| 2010 | small_decorative_pumpkin_black | Small Decorative Pumpkin |
| 2011 | small_decorative_pumpkin_green | Small Decorative Pumpkin |
| 2012 | small_decorative_pumpkin_orange | Small Decorative Pumpkin |
| 2013 | small_decorative_pumpkin_purple | Small Decorative Pumpkin |
| 2014 | small_decorative_pumpkin_white | Small Decorative Pumpkin |
| 2015 | small_decorative_pumpkin_yellow | Small Decorative Pumpkin |
| 2016 | small_grass_starter | Grass Starter |
| 2017 | small_greenhouse_white_blueprint | Small White Greenhouse Blueprint |
| 2018 | small_greenhouse_wood_blueprint | Small Wood Greenhouse Blueprint |
| 2019 | small_stone_bundle | Small Bundle of Stone |
| 2023 | small_vase | Small Vase |
| 2024 | small_wood_bundle | Small Bundle of Wood |
| 2025 | smallmouth_bass | Smallmouth Bass |
| 2026 | smoke_moth | Smoke Moth |
| 2027 | smoked_trout_soup | Smoked Trout Soup |
| 2028 | snail | Snail |
| 2029 | snakehead | Snakehead |
| 2030 | snapdragon | Snapdragon |
| 2031 | snapping_turtle | Snapping Turtle |
| 2032 | snow_fish | Snow Fish |
| 2033 | snow_peas | Snow Peas |
| 2034 | snowball_beetle | Snowball Beetle |
| 2035 | snowdrop_anemone | Snowdrop Anemone |
| 2036 | snug_bug | Snug Bug |
| 2037 | sod | Sod |
| 2038 | soup_of_the_day | Soup of the Day | **GLITCHED** |
| 2039 | soup_of_the_day_gold | Soup of the Day Deluxe | **GLITCHED** |
| 2040 | sour_lemon_cake | Lemon Cake |
| 2041 | soy_sauce | Soy Sauce |
| 2042 | speedy_snail | Speedy Snail |
| 2043 | speedy_syrup | Speedy Syrup |
| 2044 | spell_fruit | Spell Fruit |
| 2045 | spell_fruit_parfait | Spell Fruit Parfait |
| 2046 | spicy_cheddar_biscuit | Spicy Cheddar Biscuit |
| 2047 | spicy_corn | Spicy Corn |
| 2048 | spicy_crab_sushi | Spicy Crab Sushi |
| 2049 | spicy_water_chestnuts | Spicy Water Chestnuts |
| 2050 | spirit_mushroom | Spirit Mushroom |
| 2051 | spirit_mushroom_tea | Spirit Mushroom Tea |
| 2052 | spirula_shell | Spirula Shell |
| 2053 | spooky_haybale | Hay Bale |
| 2056 | spring_bed | Spring Bed |
| 2057 | spring_crop_sign_scroll_bundle | Spring Crop Sign Scroll Bundle |
| 2060 | spring_double_bed | Spring Double Bed |
| 2061 | spring_festival_basket | Breath of Spring Basket |
| 2062 | spring_festival_breath_of_spring_wreath | Deluxe Breath of Spring Wreath |
| 2063 | spring_festival_flower_chest | Breath of Spring Storage Chest |
| 2064 | spring_festival_large_planter | Breath of Spring Large Planter |
| 2065 | spring_festival_planter | Breath of Spring Planter |
| 2066 | spring_festival_wreath | Breath of Spring Wreath |
| 2069 | spring_flooring | Spring Flooring |
| 2070 | spring_galette | Spring Galette |
| 2073 | spring_lamp | Spring Lamp |
| 2076 | spring_potted_tree | Spring Potted Tree |
| 2079 | spring_rug | Spring Flower Rug |
| 2080 | spring_salad | Spring Salad |
| 2083 | spring_sofa | Spring Sofa |
| 2086 | spring_wallpaper | Spring Wallpaper |
| 2087 | square_tile_floor | Square Tile Floor |
| 2088 | squid | Squid |
| 2089 | stairs | House Stairs |
| 2090 | stamina_syrup | Stamina Syrup |
| 2091 | stamina_up | Stamina Up |
| 2092 | star_brooch | Star Brooch |
| 2098 | star_cushion | Star Cushion |
| 2103 | star_lantern | Star Lantern |
| 2104 | star_shaped_cookie | Star-Shaped Cookie |
| 2108 | star_viewing_blanket | Star Viewing Blanket |
| 2109 | starlight_coral | Starlight Coral |
| 2111 | starry_flooring | Starry Flooring |
| 2112 | starter_bird_house_red | Birdhouse |
| 2113 | starter_potted_plant | Potted Plant |
| 2114 | starter_scarecrow | Scarecrow |
| 2115 | starter_shipping_box | Shipping Bin |
| 2116 | starter_stone_path | Stone Path |
| 2117 | starter_stone_path_double | Stone Path |
| 2118 | starter_well | Water Well |
| 2119 | starter_wheelbarrow | Wheelbarrow |
| 2120 | starter_wood_fence | Wooden Fence |
| 2121 | statuette_of_caldarus | Statuette of Caldarus |
| 2122 | steamed_broccoli | Steamed Broccoli |
| 2123 | stingray | Stingray |
| 2124 | stinky_stamina_potion | MISSING | **GLITCHED** |
| 2127 | stone_bench | Stone Bench |
| 2130 | stone_dragon_figure | Stone Dragon Figure |
| 2131 | stone_horse | Stone Horse |
| 2134 | stone_lamp | Stone Lamp |
| 2135 | stone_loach | Stone Loach |
| 2136 | stone_shell | Stone Shell |
| 2139 | stone_storage_chest | Stone Storage Chest |
| 2142 | stone_table | Stone Table |
| 2143 | stone_wall | Stone Wall |
| 2144 | strawberries_and_cream | Strawberries and Cream |
| 2145 | strawberry | Strawberry |
| 2146 | strawberry_shortcake | Strawberry Shortcake |
| 2147 | strawhopper | Strawhopper |
| 2148 | striped_bass | Striped Bass |
| 2149 | strobe_firefly | Strobe Firefly |
| 2150 | sturgeon | Sturgeon |
| 2151 | sugar | Sugar |
| 2152 | sugar_cane | Sugar Cane |
| 2153 | sulfur_crab | Sulfur Crab |
| 2155 | summer_bed | Summer Bed |
| 2157 | summer_chair | Summer Chair |
| 2158 | summer_crop_sign_scroll_bundle | Summer Crop Sign Scroll Bundle |
| 2160 | summer_double_bed | Summer Double Bed |
| 2162 | summer_flooring | Summer Flooring |
| 2164 | summer_jar_lamp | Summer Jar Lamp |
| 2166 | summer_rug | Summer Flower Rug |
| 2167 | summer_salad | Summer Salad |
| 2169 | summer_table | Summer Table |
| 2171 | summer_wallpaper | Summer Wallpaper |
| 2173 | summit_wallpaper | Summit Wallpaper |
| 2174 | sunflower | Sunflower |
| 2175 | sunny | Sunny |
| 2176 | sunset_moth | Sunset Moth |
| 2177 | surf_beetle | Surf Beetle |
| 2178 | sushi_platter | Sushi Platter |
| 2179 | sweet_bee | Sweet Bee |
| 2180 | sweet_potato | Sweet Potato |
| 2181 | sweet_potato_pie | Sweet Potato Pie |
| 2182 | sweet_sesame_balls | Sweet Sesame Balls |
| 2183 | sweetfish | Sweetfish |
| 2184 | sweetroot | Sweetroot |
| 2185 | sword_copper | Copper Sword |
| 2186 | sword_corrupted_mistril | Corrupted Mistril Sword |
| 2187 | sword_crystal | Crystal Sword |
| 2188 | sword_dragon_forged | Dragon-Forged Sword |
| 2189 | sword_gold | Gold Sword |
| 2190 | sword_iron | Iron Sword |
| 2191 | sword_mistril | Mistril Sword |
| 2192 | sword_scrap_metal | Scrap Metal Sword |
| 2193 | sword_silver | Silver Sword |
| 2194 | sword_tarnished_gold | Tarnished Gold Sword |
| 2195 | sword_verdigris | Verdigris Sword |
| 2196 | sword_worn | Worn Sword |
| 2197 | swordfish | Swordfish |
| 2198 | tarnished_gold_armor | Tarnished Gold Chestpiece |
| 2199 | tarnished_gold_boots | Tarnished Gold Greaves |
| 2200 | tarnished_gold_helmet | Tarnished Gold Helmet |
| 2201 | tarnished_gold_legplates | Tarnished Gold Legplates |
| 2202 | tarnished_gold_ring | Tarnished Gold Ring |
| 2203 | tarpon | Tarpon |
| 2204 | tea | Tea |
| 2205 | tea_black | Black Tea Cup |
| 2206 | tea_green | Green Tea Cup |
| 2207 | teleportation_pad | Teleportation Circle |
| 2208 | temple_flower | Temple Flower |
| 2209 | terrarium | Terrarium |
| 2210 | tesserae_tree | Tesserae Tree |
| 2211 | tetra | Tetra |
| 2212 | thorn_vine | Thorn Vine |
| 2213 | thyme | Thyme |
| 2214 | tide_lettuce | Tide Lettuce |
| 2215 | tide_salad | Tide Salad |
| 2216 | tide_touched_cavern_wall | Tide-Touched Cavern Wall |
| 2217 | tidestone | Tidestone |
| 2218 | tiger_swallowtail_butterfly | Tiger Swallowtail Butterfly |
| 2219 | tilapia | Tilapia |
| 2222 | tile_roof_fence | Tile Roof Fence |
| 2223 | tin_lunchbox | Tin Lunchbox |
| 2224 | tiny_dinosaur_skeleton | Tiny Dinosaur Skeleton |
| 2225 | tiny_volcano | Tiny Volcano |
| 2226 | toasted_sunflower_seeds | Toasted Sunflower Seeds |
| 2228 | toilet | Toilet |
| 2229 | tomato | Tomato |
| 2230 | tomato_soup | Tomato Soup |
| 2231 | trail_mix | Trail Mix |
| 2234 | training_dummy | Training Dummy |
| 2235 | transparent_jellyfish | Transparent Jellyfish |
| 2236 | treasure_box_copper | Copper Treasure Box |
| 2237 | treasure_box_gold | Golden Treasure Box |
| 2238 | treasure_box_silver | Silver Treasure Box |
| 2239 | treasure_box_wood | Wooden Treasure Box |
| 2240 | tree_coin | coin | **GLITCHED** |
| 2241 | trilobite_fossil | Trilobite Fossil |
| 2242 | trout | Trout |
| 2243 | tulip | Tulip |
| 2244 | tulip_cake | Tulip Cake |
| 2245 | tuna | Tuna |
| 2246 | tuna_sashimi | Tuna Sashimi |
| 2247 | tunnel_millipede | Tunnel Millipede |
| 2248 | turnip | Turnip |
| 2249 | turnip_and_cabbage_salad | Turnip & Cabbage Salad |
| 2250 | turnip_and_potato_gratin | Turnip & Potato Gratin |
| 2251 | turtle | Turtle |
| 2252 | twice_baked_rations | Twice-Baked Rations |
| 2253 | ultimate_hay | Ultimate Large Animal Feed |
| 2254 | ultimate_small_animal_feed | Ultimate Small Animal Feed |
| 2255 | underseaweed | Underseaweed |
| 2256 | unidentified_artifact | Unidentified Artifact | **DISABLED** |
| 2257 | unknown_dragon_statuette | Unknown Dragon Statuette |
| 2258 | unknown_item | Unknown Item | **PLACEHOLDER** |
| 2259 | unusual_seed | Unusual Seed | **GLITCHED** |
| 2260 | upper_mines_mushroom | Upper Mines Mushroom |
| 2261 | vegetable_pot_pie | Vegetable Pot Pie |
| 2262 | vegetable_quiche | Vegetable Quiche |
| 2263 | vegetable_soup | Vegetable Soup |
| 2264 | veggie_sub_sandwich | Veggie Sub Sandwich |
| 2265 | verdigris_boots | Verdigris Boots |
| 2266 | verdigris_chestpiece | Verdigris Chestpiece |
| 2267 | verdigris_helmet | Verdigris Helmet |
| 2268 | verdigris_ring | Verdigris Ring |
| 2269 | verdigris_tassets | Verdigris Tassets |
| 2270 | vintage_brush | Vintage Brush |
| 2271 | vintage_cow_bell | Vintage Cow Bell |
| 2272 | vintage_hammer | Vintage Hammer |
| 2273 | vintage_sickle | Vintage Sickle |
| 2274 | vintage_watering_can | Vintage Watering Can |
| 2275 | viola | Viola |
| 2277 | void_bonsai | Void Bonsai |
| 2278 | void_cake | Void Cake |
| 2280 | void_fireplace | Void Fireplace |
| 2282 | void_flagstone_large | Large Void Flagstone |
| 2284 | void_flagstone_small | Small Void Flagstone |
| 2286 | void_flooring | Void Floor |
| 2287 | void_herb | Void Herb |
| 2289 | void_lamp | Void Lamp |
| 2290 | void_newt | Void Newt |
| 2291 | void_pearl | Void Pearl |
| 2292 | void_powder | Void Powder |
| 2293 | void_set_scroll_bundle | Void Scroll Bundle |
| 2294 | void_snail | Void Snail |
| 2295 | void_stone | Void Stone |
| 2297 | void_storage_chest | Void Storage Chest |
| 2299 | void_throne_1 | Void Throne |
| 2301 | void_throne_2 | Void Throne II |
| 2303 | void_wallpaper | Void Wall |
| 2305 | void_window | Void Window |
| 2306 | voidite | Voidite |
| 2307 | walking_leaf | Walking Leaf |
| 2308 | walking_stick | Walking Stick |
| 2309 | walleye | Walleye |
| 2310 | walnut | Walnut |
| 2311 | warm_rock | Warm Rock |
| 2312 | water_balloon_fish | Water Balloon Fish |
| 2313 | water_chestnut | Water Chestnut |
| 2314 | water_chestnut_fritters | Water Chestnut Fritters |
| 2315 | water_sphere | Water Sphere |
| 2323 | water_sprite | Water Sprite Statue |
| 2318 | water_sprite_large | Large Water Sprite Statue |
| 2324 | water_tablet_rubbing | Water Tablet Rubbing |
| 2325 | waterbug | Waterbug |
| 2326 | waterfly | Waterfly |
| 2327 | watering_can_copper | Copper Watering Can |
| 2328 | watering_can_gold | Gold Watering Can |
| 2329 | watering_can_iron | Iron Watering Can |
| 2330 | watering_can_mistril | Mistril Watering Can |
| 2331 | watering_can_silver | Silver Watering Can |
| 2332 | watering_can_worn | Worn Watering Can |
| 2333 | watermelon | Watermelon |
| 2334 | weather_crystal_ball | Weather Crystal Ball |
| 2335 | weightless_stone | Weightless Stone |
| 2336 | wheat | Wheat |
| 2337 | white_capybara_wall_ribbon | White Capybara Wall Ribbon |
| 2338 | white_chicken_wall_ribbon | White Chicken Wall Ribbon |
| 2339 | white_duck_wall_ribbon | White Duck Wall Ribbon |
| 2340 | white_perch | White Perch |
| 2341 | white_rabbit_wall_ribbon | White Rabbit Wall Ribbon |
| 2342 | white_wine | White Wine |
| 2343 | wild_berries | Wild Berries |
| 2344 | wild_berry_jam | Wild Berry Jam |
| 2345 | wild_grapes | Wild Grapes |
| 2346 | wild_leek | Wild Leek |
| 2347 | wild_mushroom | Cave Mushroom |
| 2348 | wildberry_pie | Wild Berry Pie |
| 2349 | wildberry_scone | Wild Berry Scone |
| 2350 | windleaf_butterfly | Windleaf Butterfly |
| 2351 | wine_glass_red | Red Wine Glass |
| 2352 | wine_glass_rose | Rose Wine Glass |
| 2353 | wine_glass_white | White Wine Glass |
| 2354 | winged_shrimp | Winged Shrimp |
| 2355 | winter_crop_sign_scroll_bundle | Winter Crop Sign Scroll Bundle |
| 2356 | winter_stew | Winter Stew |
| 2357 | wintergreen_berry | Wintergreen Berry |
| 2358 | wintergreen_ice_cream | Wintergreen Ice Cream |
| 2359 | winterpillar | Winterpillar |
| 2361 | witch_queen_bed | Witch Queen Bed |
| 2363 | witch_queen_cauldron | Witch Queen Cauldron |
| 2365 | witch_queen_chair | Witch Queen Chair |
| 2367 | witch_queen_double_bed | Witch Queen Double Bed |
| 2369 | witch_queen_dressing_table | Witch Queen Dressing Table |
| 2371 | witch_queen_flooring | Witch Queen Flooring |
| 2373 | witch_queen_moon_lamp | Witch Queen Moon Lamp |
| 2375 | witch_queen_nightstand | Witch Queen Nightstand |
| 2377 | witch_queen_pillar | Witch Queen Pillar |
| 2379 | witch_queen_rug | Witch Queen Rug |
| 2381 | witch_queen_table | Witch Queen Table |
| 2383 | witch_queen_throne | Witch Queen Throne |
| 2385 | witch_queen_wallpaper | Witch Queen Wallpaper |
| 2386 | woodcrafting_station | Crafting Station |
| 2387 | world_fountain | World Fountain 20 | **GLITCHED** |
| 2388 | worm | Worm |
| 2389 | worn_bed | Worn Bed |
| 2390 | worn_chair | Worn Chair |
| 2391 | worn_fireplace | Worn Fireplace |
| 2392 | worn_flooring | Worn Flooring |
| 2393 | worn_garden_lamp | Worn Garden Lamp |
| 2394 | worn_nightstand | Worn Nightstand |
| 2395 | worn_pendant | Worn Pendant |
| 2396 | worn_rug | Worn Rug |
| 2397 | worn_table | Worn Table |
| 2398 | worn_wallpaper | Worn Wallpaper |
| 2399 | worn_well | Worn Well |
| 2400 | worn_window | Worn Window |
| 2401 | written_root | Written Root |

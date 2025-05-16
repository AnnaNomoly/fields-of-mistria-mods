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


## Item Table (FoM v0.13.4)
* Anything marked **DISABLED** is not supported, as it would crash the game when trying to create the item.
* Anything marked **GLITCHED** is a glitched item that is only partially implemented. It won't crash the game, but is buggy.

| Item ID  | Internal Item Name | Displayed Item Name | Notes |
| -------- | ------------------ | ------------------- | ----- |
| 0 | acorn | Acorn |
| 1 | adept_kitchen | Level 3 Kitchen |
| 2 | alda_bronze_sword | Alda Bronze Sword |
| 3 | alda_clay_pot | Alda Clay Pot |
| 4 | alda_feather_pendant | Alda Feather Pendant |
| 5 | alda_gem_bracelet | Alda Gem Bracelet |
| 6 | alda_mural_tablet | Alda Mural Tablet |
| 11 | alda_pot_replica | Alda Pot Replica |
| 12 | aldarian_gauntlet | Aldarian Gauntlet |
| 13 | aldarian_sword | Aldarian Sword |
| 14 | aldarian_war_banner | Aldarian War Banner |
| 15 | alligator_gar | Alligator Gar |
| 16 | alpaca_plushie | Alpaca Plushie |
| 17 | alpaca_wool | Alpaca Wool |
| 18 | amber_trapped_insect | Amber Trapped Insect |
| 19 | anchovy | Anchovy |
| 20 | ancient_crystal_goblet | Ancient Crystal Goblet |
| 21 | ancient_gold_coin | Ancient Gold Coin |
| 22 | ancient_horn_circlet | Ancient Horn Circlet |
| 23 | ancient_royal_scepter | Ancient Royal Scepter |
| 24 | ancient_stone_lantern | Ancient Stone Lantern |
| 25 | angel_fish | Angel Fish |
| 26 | animal_cosmetic | MISSING | **DISABLED** |
| 27 | animal_currency | Shiny Bead |
| 28 | ant | Ant |
| 29 | apple | Apple |
| 30 | apple_honey_curry | Apple Honey Curry |
| 31 | apple_juice | Apple Juice |
| 32 | apple_pie | Apple Pie |
| 33 | archerfish | Archerfish |
| 34 | armored_bass | Armored Bass |
| 35 | ash_mushroom | Ash Mushroom |
| 36 | axe_copper | Copper Axe |
| 37 | axe_gold | Gold Axe |
| 38 | axe_iron | Iron Axe |
| 39 | axe_mistril | Mistril Axe |
| 40 | axe_silver | Silver Axe |
| 41 | axe_worn | Worn Axe |
| 42 | baked_potato | Baked Potato |
| 43 | baked_sweetroot | Baked Sweetroot |
| 45 | bakery_bread_basket | Bakery Bread Basket |
| 49 | bakery_cake | Bakery Display Cake |
| 47 | bakery_cake_case | Bakery Dessert Case |
| 51 | bakery_cake_slice | Bakery Display Cake Slice |
| 53 | bakery_cookie_jar | Bakery Cookie Jar |
| 55 | bakery_cutting_board | Bakery Cutting Board |
| 56 | balors_crate | Balor's Crate | **GLITCHED** |
| 57 | barb | Barb |
| 59 | basic_area_rug | Basic Area Rug |
| 63 | basic_bed | Basic Single Bed |
| 61 | basic_bed_double | Basic Double Bed |
| 65 | basic_bookshelf | Basic Bookshelf |
| 67 | basic_chair | Basic Chair |
| 69 | basic_doormat | Basic Doormat |
| 71 | basic_dresser | Basic Dresser |
| 73 | basic_flooring | Basic Flooring |
| 75 | basic_flower_pot | Basic Flower Pot |
| 77 | basic_nightstand | Basic Nightstand |
| 79 | basic_oil_lamp | Basic Oil Lamp |
| 80 | basic_pouch | Bag Upgrade |
| 82 | basic_table | Basic Table |
| 84 | basic_wall_window | Basic Window |
| 86 | basic_wallpaper | Basic Wallpaper |
| 87 | basic_wood | Wood |
| 102 | basic_wood_chest | Basic Wood Chest |
| 103 | basil | Basil |
| 104 | basket | Basket |
| 106 | bathroom_bench | Bathroom Bench |
| 113 | bathroom_curtain_stand | Bathroom Curtain Stand |
| 115 | bathroom_floor_mirror | Bathroom Floor Mirror |
| 117 | bathroom_sink | Bathroom Sink |
| 121 | bathroom_tile_wall | Bathroom Tile Wall |
| 123 | bathroom_wall_mirror | Bathroom Wall Mirror |
| 124 | bathroom_wall_sconce | Bathroom Wall Sconce |
| 131 | bathroom_wall_towel | Bathroom Wall Towel |
| 133 | bathtub | Bathtub |
| 134 | beach_hopper | Beach Hopper |
| 135 | beer | Beer |
| 136 | beer_mug | Beer Mug |
| 137 | beet | Beet |
| 138 | beet_salad | Beet Salad |
| 139 | beet_soup | Beet Soup |
| 140 | beginner_kitchen | Level 1 Kitchen |
| 141 | bell_berry | Bell Berry |
| 142 | bell_berry_bakewell_tart | Bell Berry Bakewell Tart |
| 143 | berries_and_cream | Berries and Cream |
| 144 | berry_bowl | Berry Bowl |
| 145 | big_bell | Big Bell |
| 146 | big_cookie | Big Cookie |
| 147 | black_tablet | Black Tablet |
| 148 | blackberry | Blackberry |
| 149 | blackberry_jam | Blackberry Jam |
| 150 | blue_capybara_wall_ribbon | Blue Capybara Wall Ribbon |
| 151 | blue_chicken_wall_ribbon | Blue Chicken Wall Ribbon |
| 152 | blue_conch_shell | Blue Conch Shell |
| 153 | blue_duck_wall_ribbon | Blue Duck Wall Ribbon |
| 154 | blue_pet_bed | Blue Pet Bed |
| 155 | blue_pet_dish | Blue Pet Dish |
| 156 | blue_rabbit_wall_ribbon | Blue Rabbit Wall Ribbon |
| 157 | blueberry | Blueberry |
| 158 | blueberry_jam | Blueberry Jam |
| 159 | bluefish | Bluefish |
| 160 | bluegill | Blue Gill |
| 161 | bonito | Bonito |
| 162 | bowfish | Bowfish |
| 163 | box_medium | Medium Box |
| 165 | box_small | Small Box |
| 166 | braised_burdock | Braised Burdock |
| 167 | braised_carrots | Braised Carrots |
| 168 | bread | Bread |
| 169 | breaded_catfish | Breaded Catfish |
| 170 | bream | Bream |
| 171 | breath_of_fire | Breath of Flame |
| 172 | breath_of_spring | Breath of Spring |
| 173 | brightbulb_moth | Brightbulb Moth |
| 174 | bristle | Bristle |
| 175 | broccoli | Broccoli |
| 176 | broccoli_salad | Broccoli Salad |
| 177 | bronze_alpaca_trophy | Bronze Alpaca Trophy |
| 178 | bronze_cow_trophy | Bronze Cow Trophy |
| 179 | bronze_horse_trophy | Bronze Horse Trophy |
| 180 | bronze_sheep_trophy | Bronze Sheep Trophy |
| 181 | brown_bullhead | Brown Bullhead |
| 182 | brown_trout | Brown Trout |
| 183 | bucket_brew | Bucket Brew |
| 184 | bull_horn | Bull Horn |
| 185 | bullfrog | Bullfrog |
| 186 | bumblebee | Bumblebee |
| 187 | bumblebee_plushie | Bumblebee Plushie |
| 188 | burbot | Burbot |
| 189 | burdock_root | Burdock Root |
| 190 | butter | Butter |
| 191 | buttered_peas | Buttered Peas |
| 192 | butterfish | Butterfish |
| 193 | butterfly | Butterfly |
| 196 | butterfly_bed | Butterfly Bed |
| 199 | butterfly_double_bed | Butterfly Double Bed |
| 202 | butterfly_wall_lamp | Butterfly Wall Lamp |
| 203 | cabbage | Cabbage |
| 204 | cabbage_slaw | Cabbage Slaw |
| 210 | cabin_bed | Rustic Cabin Bed |
| 208 | cabin_bed_double | Rustic Cabin Double Bed |
| 213 | cabin_bookshelf | Rustic Cabin Bookshelf |
| 216 | cabin_chair | Rustic Cabin Chair |
| 219 | cabin_dresser | Rustic Cabin Dresser |
| 222 | cabin_firewood | Rustic Cabin Firewood |
| 225 | cabin_flooring | Rustic Cabin Flooring |
| 228 | cabin_lamp | Rustic Cabin Lamp |
| 231 | cabin_nightstand | Rustic Cabin Nightstand |
| 233 | cabin_stove | Rustic Cabin Stove |
| 236 | cabin_table | Rustic Cabin Table |
| 239 | cabin_wall_shelf | Rustic Cabin Wall Shelf |
| 242 | cabin_wall_window | Rustic Cabin Window |
| 245 | cabin_wallpaper | Rustic Cabin Wallpaper |
| 246 | caldosian_breastplate | Caldosian Breastplate |
| 247 | caldosian_chocolate_cake | Chocolate Cake |
| 248 | caldosian_drinking_horn | Caldosian Drinking Horn |
| 249 | caldosian_emperor_bust | Caldosian Emperor Bust |
| 250 | caldosian_sword | Caldosian Sword |
| 251 | campfire | Campfire |
| 252 | candelabra_seadragon | Candelabra Seadragon |
| 253 | candied_lemon_peel | Candied Lemon Peel |
| 254 | candied_strawberries | Candied Strawberries |
| 255 | candied_walnuts | Candied Walnuts |
| 256 | candle | Candle |
| 257 | candle_chamberstick | Candle Chamberstick |
| 258 | canned_sardines | Canned Sardines |
| 259 | capybara_plushie | Capybara Plushie |
| 260 | caramel_candy | Caramel Candy |
| 261 | caramelized_moon_fruit | Caramelized Moon Fruit |
| 262 | carp | Carp |
| 263 | carrot | Carrot |
| 264 | cat_treat | Cat Treat |
| 265 | caterpillar | Caterpillar |
| 268 | caterpillar_chair | Caterpillar Chair |
| 269 | catfish | Catfish |
| 270 | catmint | Catmint |
| 271 | cattail | Cattail Fluff |
| 272 | cauliflower | Cauliflower |
| 273 | cauliflower_curry | Cauliflower Curry |
| 274 | cave_eel | Cave Eel |
| 275 | cave_kelp | Jade Dulse |
| 276 | cave_shark | Cave Shark |
| 277 | cave_shrimp | Cave Shrimp |
| 280 | cavern_crystal_lamp | Glowing Crystal Lamp |
| 281 | cavern_emerald_rock | Ornamental Emerald Rock |
| 284 | cavern_floor_brazier | Floor Brazier |
| 285 | cavern_hewn_rock_bookshelf | Hewn Rock Bookshelf |
| 286 | cavern_hewn_rock_table | Hewn Rock Table |
| 288 | cavern_mossy_rug | Mossy Rug |
| 291 | cavern_mushroom_stool | Mushroom Stool |
| 292 | cavern_rock_flooring | Rock Floor |
| 293 | cavern_rock_wallpaper | Rock Wall |
| 294 | cavern_silver_node | Ornamental Silver Node |
| 295 | cavern_stone_arch | Stalactite Arch |
| 298 | cavern_wall_brazier | Wall Brazier |
| 299 | celosia | Celosia |
| 300 | char | Char |
| 301 | cheese | Cheese |
| 302 | cherry | Cherry |
| 303 | cherry_cobbler | Cherry Cobbler |
| 304 | cherry_fish | Cherry Fish |
| 305 | cherry_smoothie | Cherry Smoothie |
| 306 | cherry_tart | Cherry Tart |
| 310 | chess_bishop | Chess Bishop Piece |
| 314 | chess_king | Chess King Piece |
| 318 | chess_knight | Chess Knight Piece |
| 322 | chess_pawn | Chess Pawn Piece |
| 326 | chess_queen | Chess Queen Piece |
| 330 | chess_rook | Chess Rook Piece |
| 331 | chess_scroll_bundle | Chess Piece Scroll Bundle |
| 335 | chess_table | Chess Table |
| 336 | chestnut | Chestnut |
| 337 | chicken_plushie | Chicken Plushie |
| 338 | chickpea | Chickpea |
| 339 | chickpea_curry | Chickpea Curry |
| 340 | chicky_hot_chocolate | Chicky Hot Chocolate |
| 341 | chili_coconut_curry | Chili Coconut Curry |
| 342 | chili_pepper | Chili Pepper |
| 343 | chillipede | Chillipede |
| 344 | chocolate | Chocolate |
| 345 | chrysanthemum | Chrysanthemum |
| 346 | chub | Chub |
| 347 | chum | Chum |
| 348 | cicada | Cicada |
| 349 | cicada_nymph | Cicada Nymph |
| 350 | clam | Clam |
| 351 | clam_chowder | Clam Chowder |
| 352 | clay | Clay |
| 353 | clay_key | Clay Key |
| 354 | cloth_chestpiece | Cloth Chestpiece |
| 355 | cloth_helmet | Cloth Helmet |
| 356 | cloth_pants | Cloth Pants |
| 357 | cloth_shoes | Cloth Shoes |
| 358 | cloth_wristband | Cloth Wristband |
| 359 | coconut | Coconut |
| 360 | coconut_cream_pie | Coconut Cream Pie |
| 361 | coconut_milk | Coconut Milk |
| 362 | cod | Cod |
| 363 | cod_with_thyme | Cod with Thyme |
| 364 | coffee | Coffee |
| 367 | coffee_mug | Coffee Mug |
| 368 | coin_lump | Coin Lump |
| 410 | color_paving_stone | Color Paving Stone |
| 411 | completely_wrong_map | Completely Wrong Map |
| 412 | confiscated_coffee | Confiscated Coffee |
| 413 | construction_fence | Construction Fence |
| 414 | cooktop_beetle | Cooktop Beetle |
| 415 | copper_armor | Copper Armor |
| 416 | copper_beetle | Copper Nugget Beetle |
| 417 | copper_greaves | Copper Greaves |
| 418 | copper_helmet | Copper Helmet |
| 419 | copper_ingot | Copper Ingot |
| 420 | copper_legplates | Copper Legplates |
| 421 | copper_ring | Copper Ring |
| 422 | coral | Coral |
| 424 | coral_chair | Coral Chair |
| 426 | coral_lamp | Coral Lamp |
| 427 | coral_mantis | Coral Mantis |
| 428 | coral_stone_archway | Coral-Stone Arch |
| 430 | coral_storage_chest | Coral Storage Chest |
| 432 | coral_table | Coral Table |
| 433 | corn | Corn |
| 434 | cosmetic | MISSING | **DISABLED** |
| 435 | cosmos | Cosmos |
| 439 | cottage_bed | Lovely Cottage Single Bed |
| 438 | cottage_bed_double | Lovely Cottage Double Bed |
| 441 | cottage_chair | Lovely Cottage Chair |
| 443 | cottage_dresser | Lovely Cottage Dresser |
| 445 | cottage_flooring | Lovely Cottage Flooring |
| 447 | cottage_fridge | Lovely Cottage Icebox |
| 449 | cottage_garden_arch | Cottage Garden Arch |
| 452 | cottage_garden_planter | Cottage Garden Planter |
| 454 | cottage_garden_shed | Cottage Garden Shed |
| 457 | cottage_garden_trellis | Cottage Garden Trellis |
| 459 | cottage_garden_workbench | Cottage Garden Workbench |
| 461 | cottage_nightstand | Lovely Cottage Nightstand |
| 463 | cottage_plant_shelf | Lovely Cottage Plant Shelf |
| 465 | cottage_potted_fig | Lovely Cottage Potted Fig |
| 467 | cottage_potted_flowers | Lovely Cottage Potted Flowers |
| 469 | cottage_rug | Lovely Cottage Rug |
| 471 | cottage_table | Lovely Cottage Table |
| 473 | cottage_wall_flowers | Lovely Cottage Wall Flowers |
| 475 | cottage_wall_window | Lovely Cottage Window |
| 477 | cottage_wallpaper | Lovely Cottage Wallpaper |
| 478 | counter_basic_v1 | Basic Counter |
| 479 | counter_basic_v2 | Basic Counter |
| 480 | counter_cabin_cherry | Cabin Counter |
| 481 | counter_cabin_oak | Cabin Counter |
| 482 | counter_cabin_walnut | Cabin Counter |
| 483 | counter_cake_chocolate | Cake Counter |
| 484 | counter_cake_double_chocolate | Cake Counter |
| 485 | counter_cake_strawberry | Cake Counter |
| 486 | counter_cottage_v1 | Lovely Cottage Counter |
| 487 | counter_cottage_v2 | Lovely Cottage Counter |
| 488 | cow_donut | Cow-Shaped Donut |
| 489 | cow_milk | Milk |
| 490 | cow_plushie | Cow Plushie |
| 491 | cozy_kitchen | Level 2 Kitchen |
| 492 | crab | Crab |
| 493 | crab_cakes | Crab Cakes |
| 494 | crab_statue | Crab Statue |
| 495 | crafting_scroll | Crafting Scroll: {} | **DISABLED** |
| 496 | cranberry | Cranberry |
| 497 | cranberry_juice | Cranberry Juice |
| 498 | cranberry_orange_scone | Cranberry Orange Scone |
| 499 | crayfish | Crayfish |
| 500 | crayfish_etouffee | Crayfish Etouffee |
| 501 | cream_pet_bed | Cream Pet Bed |
| 502 | cream_pet_dish | Cream Pet Dish |
| 503 | cricket | Cricket |
| 504 | criminal_confession | Criminal Confession |
| 505 | crispy_fried_earthshroom | Crispy Fried Earthshroom |
| 506 | crocus | Crocus |
| 507 | crop_faux_ash_mushroom | Faux Ash Mushroom |
| 508 | crop_faux_basil | Faux Basil |
| 509 | crop_faux_beet | Faux Beet |
| 510 | crop_faux_blue_conch_shell | Faux Blue Conch Shell |
| 511 | crop_faux_breath_of_fire | Faux Breath of Flame |
| 512 | crop_faux_broccoli | Faux Broccoli |
| 513 | crop_faux_burdock_root | Faux Burdock |
| 514 | crop_faux_cabbage | Faux Cabbage |
| 515 | crop_faux_carrot | Faux Carrot |
| 516 | crop_faux_catmint | Faux Catmint |
| 517 | crop_faux_cauliflower | Faux Cauliflower |
| 518 | crop_faux_cave_kelp | Faux Jade Dulse |
| 519 | crop_faux_celosia | Faux Celosia |
| 520 | crop_faux_chestnut | Faux Chestnut |
| 521 | crop_faux_chickpea | Faux Chickpea |
| 522 | crop_faux_chili_pepper | Faux Chili Pepper |
| 523 | crop_faux_chrysanthemum | Faux Chrysanthemum |
| 524 | crop_faux_corn | Faux Corn |
| 525 | crop_faux_cosmos | Faux Cosmos |
| 526 | crop_faux_cranberry | Faux Cranberry |
| 527 | crop_faux_crocus | Faux Crocus |
| 528 | crop_faux_crystal_berries | Faux Crystal Berries |
| 529 | crop_faux_crystal_rose | Faux Crystal Rose |
| 530 | crop_faux_cucumber | Faux Cucumber |
| 531 | crop_faux_daffodil | Faux Daffodil |
| 532 | crop_faux_daikon_radish | Faux Daikon Radish |
| 533 | crop_faux_daisy | Faux Daisy |
| 534 | crop_faux_dandelion | Faux Dandelion |
| 535 | crop_faux_dill | Faux Dill |
| 536 | crop_faux_earthshroom | Faux Earthshroom |
| 537 | crop_faux_fennel | Faux Fennel |
| 538 | crop_faux_fiddlehead | Faux Fiddlehead |
| 539 | crop_faux_flame_pepper | Faux Flame Pepper |
| 540 | crop_faux_fog_orchid | Faux Fog Orchid |
| 541 | crop_faux_frost_lily | Faux Frost Lily |
| 542 | crop_faux_garlic | Faux Garlic |
| 543 | crop_faux_heather | Faux Heather |
| 544 | crop_faux_holly | Faux Holly |
| 545 | crop_faux_horseradish | Faux Horseradish |
| 546 | crop_faux_hot_potato | Faux Hot Potato |
| 547 | crop_faux_ice_block | Faux Ice Block |
| 548 | crop_faux_iris | Faux Iris |
| 549 | crop_faux_jasmine | Faux Jasmine |
| 550 | crop_faux_lava_chestnuts | Faux Lava Chestnuts |
| 551 | crop_faux_lilac | Faux Lilac |
| 552 | crop_faux_marigold | Faux Marigold |
| 553 | crop_faux_middlemist | Faux Middlemist |
| 554 | crop_faux_mines_mussels | Faux Mines Mussels |
| 555 | crop_faux_moon_fruit | Faux Moon Fruit |
| 556 | crop_faux_morel_mushroom | Faux Morel Mushroom |
| 557 | crop_faux_narrows_moss | Faux Narrows Moss |
| 558 | crop_faux_nettle | Faux Nettle |
| 559 | crop_faux_night_queen | Faux Night Queen |
| 560 | crop_faux_onion | Faux Onion |
| 561 | crop_faux_oregano | Faux Oregano |
| 562 | crop_faux_oyster_mushroom | Faux Oyster Mushroom |
| 563 | crop_faux_peas | Faux Peas |
| 564 | crop_faux_pineshroom | Faux Pineshroom |
| 565 | crop_faux_pink_scallop_shell | Faux Pink Scallop Shell |
| 566 | crop_faux_poinsettia | Faux Poinsettia |
| 567 | crop_faux_potato | Faux Potato |
| 568 | crop_faux_pumpkin | Faux Pumpkin |
| 569 | crop_faux_rice | Faux Rice |
| 570 | crop_faux_rockroot | Faux Rockroot |
| 571 | crop_faux_rosemary | Faux Rosemary |
| 572 | crop_faux_sage | Faux Sage |
| 573 | crop_faux_sand_dollar | Faux Sand Dollar |
| 574 | crop_faux_sea_grapes | Faux Sea Grapes |
| 575 | crop_faux_sesame | Faux Sesame |
| 576 | crop_faux_shadow_flower | Faux Shadow Flower |
| 577 | crop_faux_shale_grass | Faux Shale Grass |
| 578 | crop_faux_snapdragon | Faux Snapdragon |
| 579 | crop_faux_snow_peas | Faux Snow Peas |
| 580 | crop_faux_snowdrop_anemone | Faux Snowdrop Anemone |
| 581 | crop_faux_spirit_mushroom | Faux Spirit Mushroom |
| 582 | crop_faux_spirula_shell | Faux Spirula Shell |
| 583 | crop_faux_strawberry | Faux Strawberry |
| 584 | crop_faux_sugar_cane | Faux Sugar Cane |
| 585 | crop_faux_sunflower | Faux Sunflower |
| 586 | crop_faux_sweet_potato | Faux Sweet Potato |
| 587 | crop_faux_sweetroot | Faux Sweetroot |
| 588 | crop_faux_tea | Faux Tea |
| 589 | crop_faux_temple_flower | Faux Temple Flower |
| 590 | crop_faux_thorn_vine | Faux Thorn Vine |
| 591 | crop_faux_thyme | Faux Thyme |
| 592 | crop_faux_tide_lettuce | Faux Tide Lettuce |
| 593 | crop_faux_tomato | Faux Tomato |
| 594 | crop_faux_tulip | Faux Tulip |
| 595 | crop_faux_turnip | Faux Turnip |
| 596 | crop_faux_upper_mines_mushroom | Faux Upper Mines Mushroom |
| 597 | crop_faux_viola | Faux Viola |
| 598 | crop_faux_walnut | Faux Walnut |
| 599 | crop_faux_watermelon | Faux Watermelon |
| 600 | crop_faux_wheat | Faux Wheat |
| 601 | crop_faux_wild_leek | Faux Wild Leek |
| 602 | crop_sign_apple | Apple Sign |
| 603 | crop_sign_basil | Basil Sign |
| 604 | crop_sign_beet | Beet Sign |
| 605 | crop_sign_broccoli | Broccoli Sign |
| 606 | crop_sign_burdock_root | Burdock Root Sign |
| 607 | crop_sign_cabbage | Cabbage Sign |
| 608 | crop_sign_carrot | Carrot Sign |
| 609 | crop_sign_catmint | Catmint Sign |
| 610 | crop_sign_cauliflower | Cauliflower Sign |
| 611 | crop_sign_celosia | Celosia Sign |
| 612 | crop_sign_cherry | Cherry Sign |
| 613 | crop_sign_chickpea | Chickpea Sign |
| 614 | crop_sign_chili_pepper | Chili Pepper Sign |
| 615 | crop_sign_chrysanthemum | Chrysanthemum Sign |
| 616 | crop_sign_coconut | Coconut Sign |
| 617 | crop_sign_corn | Corn Sign |
| 618 | crop_sign_cosmos | Cosmos Sign |
| 619 | crop_sign_cranberry | Cranberry Sign |
| 620 | crop_sign_cucumber | Cucumber Sign |
| 621 | crop_sign_daffodil | Daffodil Sign |
| 622 | crop_sign_daikon_radish | Daikon Radish Sign |
| 623 | crop_sign_daisy | Daisy Sign |
| 624 | crop_sign_dill | Dill Sign |
| 625 | crop_sign_frost_lily | Frost Lily Sign |
| 626 | crop_sign_garlic | Garlic Sign |
| 627 | crop_sign_heather | Heather Sign |
| 628 | crop_sign_iris | Iris Sign |
| 629 | crop_sign_jasmine | Jasmine Sign |
| 630 | crop_sign_lemon | Lemon Sign |
| 631 | crop_sign_lilac | Lilac Sign |
| 632 | crop_sign_marigold | Marigold Sign |
| 633 | crop_sign_moon_fruit | Moon Fruit Sign |
| 634 | crop_sign_night_queen | Night queen Sign |
| 635 | crop_sign_onion | Onion Sign |
| 636 | crop_sign_orange | Orange Sign |
| 637 | crop_sign_oregano | Oregano Sign |
| 638 | crop_sign_peach | Peach Sign |
| 639 | crop_sign_pear | Pear Sign |
| 640 | crop_sign_peas | Peas Sign |
| 641 | crop_sign_poinsettia | Poinsettia Sign |
| 642 | crop_sign_pomegranate | Pomegranate Sign |
| 643 | crop_sign_potato | Potato Sign |
| 644 | crop_sign_pumpkin | Pumpkin Sign |
| 645 | crop_sign_rice | Rice Sign |
| 646 | crop_sign_rosemary | Rosemary Sign |
| 647 | crop_sign_sage | Sage Sign |
| 648 | crop_sign_snapdragon | Snapdragon Sign |
| 649 | crop_sign_snow_peas | Snow Peas Sign |
| 650 | crop_sign_snowdrop_anemone | Snowdrop Anemone Sign |
| 651 | crop_sign_strawberry | Strawberry Sign |
| 652 | crop_sign_sugar_cane | Sugar Cane Sign |
| 653 | crop_sign_sunflower | Sunflower Sign |
| 654 | crop_sign_sweet_potato | Sweet Potato Sign |
| 655 | crop_sign_tea | Tea Sign |
| 656 | crop_sign_temple_flower | Temple Flower Sign |
| 657 | crop_sign_thyme | Thyme Sign |
| 658 | crop_sign_tomato | Tomato Sign |
| 659 | crop_sign_tulip | Tulip Sign |
| 660 | crop_sign_turnip | Turnip Sign |
| 661 | crop_sign_viola | Viola Sign |
| 662 | crop_sign_watermelon | Watermelon Sign |
| 663 | crop_sign_wheat | Wheat Sign |
| 664 | crucian_carp | Crucian Carp |
| 665 | crunchy_chickpeas | Crunchy Chickpeas |
| 666 | crystal | Crystal |
| 667 | crystal_apple | Crystal Apple |
| 668 | crystal_berries | Crystal Berries |
| 669 | crystal_berry_pie | Crystal Berry Pie |
| 670 | crystal_boots | Crystal Greaves |
| 671 | crystal_caterpillar | Crystal Caterpillar |
| 672 | crystal_chestpiece | Crystal Chestpiece |
| 673 | crystal_helmet | Crystal Helmet |
| 674 | crystal_ring | Crystal Ring |
| 675 | crystal_rose | Crystal Rose |
| 678 | crystal_string_lights | Crystal Wall String Lights |
| 679 | crystal_tassets | Crystal Tassets |
| 680 | crystal_wing_moth | Crystal Wing Moth |
| 681 | crystalline_cricket | Crystalline Cricket |
| 682 | cucumber | Cucumber |
| 683 | cucumber_salad | Cucumber Salad |
| 684 | cucumber_sandwich | Cucumber Sandwich |
| 685 | cup_of_tea | Tea with Lemon |
| 686 | curry_powder | Curry Powder |
| 687 | dace | Dace |
| 688 | daffodil | Daffodil |
| 689 | daikon_radish | Daikon Radish |
| 690 | daisy | Daisy |
| 691 | dandelion | Dandelion |
| 692 | dart | Dart |
| 693 | deep_earthworm | Deep Earthworm |
| 694 | deep_sea_soup | Deep Sea Soup |
| 695 | deluxe_curry | Deluxe Curry |
| 696 | deluxe_hay | Deluxe Large Animal Feed |
| 697 | deluxe_small_animal_feed | Deluxe Small Animal Feed |
| 698 | dense_water | Dense Water |
| 699 | deviled_eggs | Deviled Eggs |
| 700 | diamond_backed_mirror | Diamond-Backed Mirror |
| 701 | diamond_beetle | Diamond Beetle |
| 702 | dill | Dill |
| 703 | dirt_clear | Dirt Tile Clear |
| 704 | dirt_dot_one | Dirt Tile 1 Dot |
| 705 | dirt_dot_three | Dirt Tile 3 Dots |
| 706 | dirt_dot_two | Dirt Tile 2 Dots |
| 707 | dog_treat | Dog Treat |
| 708 | dough_lad_doll | Doughlad Doll |
| 712 | dragon_altar | Dragon Claw Altar |
| 713 | dragon_claw | Dragon Claw |
| 714 | dragon_forged_bracelet | Dragon-Forged Bracelet |
| 715 | dragon_horn_beetle | Dragon Horn Beetle |
| 716 | dragon_horn_mushroom | Dragon Horn Mushroom |
| 717 | dragon_horn_mushroom_with_thyme | Dragon Horn Mushroom with Thyme |
| 718 | dragon_pact_tablet | Dragon Pact Tablet |
| 721 | dragon_priestess_fountain | Dragon Priestess Fountain |
| 722 | dragon_scale | Dragon Scale |
| 724 | dragon_statue_replica | Dragon Statue |
| 725 | dragonfly | Dragonfly |
| 726 | dragonsworn_greaves | Dragonsworn Greaves |
| 727 | dragonsworn_helmet | Dragonsworn Helmet |
| 728 | dried_squid | Dried Squid |
| 729 | duck_egg | Duck Egg |
| 730 | duck_feather | Duck Feather |
| 731 | duck_mayonnaise | Duck Mayonnaise |
| 732 | duck_plushie | Duck Plushie |
| 733 | dungeon_fountain_health | Fountain Health 33 |
| 734 | dungeon_fountain_stamina | Fountain Stamina 33 |
| 735 | earth_eel | Earth Eel |
| 736 | earth_infused_stone | Earth-Infused Stone |
| 737 | earth_tablet_rubbing | Earth Tablet Rubbing |
| 738 | earthshroom | Earthshroom |
| 739 | egg | Chicken Egg |
| 745 | emerald_bed | Emerald Double Bed |
| 743 | emerald_chair | Emerald Chair |
| 746 | emerald_horned_charger | Emerald-Horned Charger |
| 748 | emerald_nightstand | Emerald Nightstand |
| 750 | emerald_rug | Emerald Rug |
| 752 | emerald_wallpaper | Emerald Wallpaper |
| 753 | espresso | Espresso |
| 756 | espresso_cup | Espresso Cup |
| 757 | espresso_machine | Espresso Machine |
| 758 | essence_blossom | Essence Blossom |
| 759 | essence_drop | Essence Drop |
| 760 | essence_stone_large | Large Essence Stone |
| 761 | essence_stone_medium | Medium Essence Stone |
| 762 | essence_stone_small | Small Essence Stone |
| 763 | essence_stone_tiny | Tiny Essence Stone |
| 766 | explorer_bed | Explorer Bed |
| 769 | explorer_chair | Explorer Chair |
| 772 | explorer_double_bed | Explorer Double Bed |
| 775 | explorer_flooring | Explorer Flooring |
| 778 | explorer_globe | Explorer Globe |
| 781 | explorer_lamp | Explorer Lamp |
| 784 | explorer_rug | Explorer Rug |
| 787 | explorer_trunk_table | Explorer Trunk Table |
| 790 | explorer_wallpaper | Explorer Wallpaper |
| 791 | faceted_rock_gem | Faceted Rock Gem |
| 792 | fairy_bee | Fairy Bee |
| 793 | fairy_syrup | Fairy Syrup |
| 794 | fake_essence | Essence |
| 799 | fall_crop_sign_scroll_bundle | Fall Crop Sign Scroll Bundle |
| 803 | fall_double_bed | Fall Double Bed |
| 798 | fall_single_bed | Fall Single Bed |
| 807 | fall_table | Fall Dining Table |
| 808 | family_crest_pendant | Family Crest Pendant |
| 809 | farm_bridge | Starter Farm Bridge |
| 810 | farm_house_calendar | Farmhouse Calendar |
| 811 | feather | Rooster Feather |
| 812 | fennel | Fennel |
| 813 | fiber | Fiber |
| 814 | fiddlehead | Fiddlehead |
| 815 | field_clear | Grass Tile Clear |
| 816 | field_flower | Grass Tile Flower |
| 817 | field_flower_two | Grass Tile 2 Flowers |
| 818 | field_grass | Grass Tile Blades |
| 819 | field_grass_two | Field Grass 2 Blades |
| 820 | fire_crystal | Fire Crystal |
| 821 | fire_tablet_rubbing | Fire Tablet Rubbing |
| 822 | fire_wasp | Fire Wasp |
| 823 | firefly | Firefly |
| 828 | firefly_wall_string_lights | Firefly Wall String Lights |
| 829 | firesail_fish | Firesail Fish |
| 830 | fish_plush_toy | Fish Plush Toy |
| 831 | fish_skewer | Fish Skewers |
| 832 | fish_stew | Fish Stew |
| 833 | fish_tacos | Fish Tacos |
| 835 | fishing_bed | Fishing Bed |
| 837 | fishing_chair | Fishing Chair |
| 839 | fishing_double_bed | Fishing Double Bed |
| 841 | fishing_flooring | Fishing Flooring |
| 843 | fishing_lamp | Lighthouse Lamp |
| 845 | fishing_oar_wall_decor | Oar Wall Decor |
| 848 | fishing_rod_copper | Copper Fishing Rod |
| 849 | fishing_rod_gold | Gold Fishing Rod |
| 850 | fishing_rod_iron | Iron Fishing Rod |
| 851 | fishing_rod_mistril | Mistril Fishing Rod |
| 852 | fishing_rod_silver | Silver Fishing Rod |
| 853 | fishing_rod_worn | Worn Fishing Rod |
| 855 | fishing_rug | Fishing Rug |
| 857 | fishing_table | Fishing Table |
| 859 | fishing_tank | Fish Tank |
| 861 | fishing_trophy_shelf | Fishing Trophy Wall Shelf |
| 847 | fishing_wall_plaque | Faux Fish Wall Plaque |
| 863 | fishing_wallpaper | Fishing Wallpaper |
| 865 | fishing_window | Porthole Window |
| 866 | flame_pepper | Flame Pepper |
| 867 | flathead_catfish | Flathead Catfish |
| 868 | flint_arrowhead | Flint Arrowhead |
| 869 | floral_tea | Floral Tea |
| 870 | flour | Flour |
| 871 | flower_crown_beetle | Flower Crown Beetle |
| 872 | fog_orchid | Fog Orchid |
| 873 | forest_perch | Forest Perch |
| 874 | fossilized_egg | Fossilized Egg |
| 875 | fossilized_mandrake_root | Fossilized Mandrake Root |
| 876 | freshwater_eel | Freshwater Eel |
| 877 | freshwater_oyster | Freshwater Oyster |
| 878 | fried_rice | Fried Rice |
| 879 | frog | Frog |
| 880 | frost_flutter_butterfly | Frost Flutter Butterfly |
| 881 | frost_lily | Frost Lily |
| 882 | fuzzy_moth | Fuzzy Moth |
| 883 | gar | Gar |
| 884 | garlic | Garlic |
| 885 | garlic_bread | Garlic Bread |
| 886 | gathering_basket | Gathering Basket |
| 887 | gazpacho | Gazpacho |
| 888 | gem_shard_caterpillar | Gem Shard Caterpillar |
| 889 | giant_fish_scale | Giant Fish Scale |
| 890 | giant_koi | Giant Koi |
| 891 | giant_tilapia | Giant Tilapia |
| 892 | glass | Glass |
| 893 | glass_absinthe | Absinthe Glass |
| 894 | glass_cocktail | Cocktail Glass |
| 895 | glass_lemonade | Lemonade Glass |
| 896 | glass_milk | Milk Glass |
| 897 | glass_water | Water Glass |
| 898 | glass_whisky | Whisky Glass |
| 899 | glowberry | Glowberry |
| 900 | glowberry_cookies | Glowberry Cookies |
| 901 | glowing_mushroom | Glowing Mushroom |
| 902 | goby | Goby |
| 903 | gold_alpaca_trophy | Gold Alpaca Trophy |
| 904 | gold_armor | Gold Armor |
| 905 | gold_cow_trophy | Gold Cow Trophy |
| 906 | gold_floor_mirror | Gold Floor Mirror |
| 907 | gold_greaves | Gold Greaves |
| 908 | gold_helmet | Gold Helmet |
| 909 | gold_horse_trophy | Gold Horse Trophy |
| 910 | gold_ingot | Gold Ingot |
| 911 | gold_legplates | Gold Legplates |
| 912 | gold_ring | Gold Ring |
| 913 | gold_sheep_trophy | Gold Sheep Trophy |
| 914 | gold_wall_mirror | Gold Wall Mirror |
| 915 | golden_alpaca_wool | Golden Alpaca Wool |
| 916 | golden_bristle | Golden Bristle |
| 917 | golden_bull_horn | Golden Bull Horn |
| 918 | golden_butter | Golden Butter |
| 919 | golden_cheese | Golden Cheese |
| 920 | golden_cheesecake | Golden Cheesecake |
| 921 | golden_cookies | Golden Cookies |
| 922 | golden_cow_milk | Golden Milk |
| 923 | golden_duck_egg | Golden Duck Egg |
| 924 | golden_duck_feather | Golden Duck Feather |
| 925 | golden_duck_mayonnaise | Golden Duck Mayonnaise |
| 926 | golden_egg | Golden Egg |
| 927 | golden_feather | Golden Rooster Feather |
| 928 | golden_horse_hair | Golden Horse Hair |
| 929 | golden_mayonnaise | Golden Mayonnaise |
| 930 | golden_rabbit_wool | Golden Rabbit Wool |
| 931 | golden_sheep_wool | Golden Sheep Wool |
| 932 | golden_shiner | Golden Shiner |
| 933 | goldfish | Goldfish |
| 934 | grape_juice | Grape Juice |
| 935 | grass_seed | Grass Seed |
| 936 | grasshopper | Grasshopper |
| 937 | grayling | Grayling |
| 938 | green_bottle | Green Bottle |
| 939 | green_tea | Green Tea |
| 940 | grilled_cheese | Grilled Cheese |
| 941 | grilled_corn | Grilled Corn |
| 942 | grilled_eel_rice_bowl | Grilled Eel Rice Bowl |
| 943 | grouper | Grouper |
| 946 | gryphon_statue_replica | Gryphon Statue Replica |
| 947 | halibut | Halibut |
| 948 | hard_boiled_egg | Hard Boiled Egg |
| 949 | hard_wood | Hard Wood |
| 950 | hardened_essence | Hardened Essence |
| 951 | harvest_plate | Harvest Plate |
| 953 | haunted_attic_armoire | Haunted Attic Armoire |
| 955 | haunted_attic_bed | Haunted Attic Bed |
| 957 | haunted_attic_candle_cluster | Melted Candle Cluster |
| 959 | haunted_attic_candle_single | Melted Candle |
| 961 | haunted_attic_chair | Haunted Attic Chair |
| 963 | haunted_attic_double_bed | Haunted Attic Double Bed |
| 965 | haunted_attic_dress_form | Haunted Attic Dress Form |
| 967 | haunted_attic_flooring | Haunted Attic Flooring |
| 969 | haunted_attic_nightstand | Haunted Attic Nightstand |
| 971 | haunted_attic_rocking_chair | Haunted Attic Rocking Chair |
| 973 | haunted_attic_table | Haunted Attic Table |
| 975 | haunted_attic_wall_cobweb_left | Cobweb Left |
| 977 | haunted_attic_wall_cobweb_right | Cobweb Right |
| 979 | haunted_attic_wall_shelf | Haunted Attic Wall Shelf |
| 981 | haunted_attic_wall_window | Haunted Attic Window |
| 983 | haunted_attic_wallpaper | Haunted Attic Wallpaper |
| 984 | hay | Hay |
| 985 | haydens_weathervane | Hayden's Weathervane |
| 986 | heal_syrup | Healing Syrup |
| 987 | heart_crystal | Heart Crystal |
| 988 | heart_shaped_alpaca_treat | Heart Shaped Alpaca Treat |
| 989 | heart_shaped_capybara_treat | Heart Shaped Capybara Treat |
| 990 | heart_shaped_chicken_treat | Heart Shaped Chicken Treat |
| 991 | heart_shaped_cow_treat | Heart Shaped Cow Treat |
| 992 | heart_shaped_duck_treat | Heart Shaped Duck Treat |
| 993 | heart_shaped_horse_treat | Heart Shaped Horse Treat |
| 994 | heart_shaped_rabbit_treat | Heart Shaped Rabbit Treat |
| 995 | heart_shaped_sheep_treat | Heart Shaped Sheep Treat |
| 996 | heather | Heather |
| 997 | heavy_mist | Heavy Mist |
| 998 | herb_butter_pasta | Herb Butter Pasta |
| 999 | herb_salad | Herb Salad |
| 1000 | hermit_crab | Hermit Crab |
| 1001 | hermit_snail | Hermit Snail |
| 1002 | herring | Herring |
| 1005 | herringbone_tile_floor | Herringbone Tile Floor |
| 1006 | hoe_copper | Copper Hoe |
| 1007 | hoe_gold | Gold Hoe |
| 1008 | hoe_iron | Iron Hoe |
| 1009 | hoe_mistril | Mistril Hoe |
| 1010 | hoe_silver | Silver Hoe |
| 1011 | hoe_worn | Worn Hoe |
| 1012 | holly | Holly |
| 1013 | honey | Honey |
| 1014 | honey_curry | Honey Curry |
| 1015 | horse_hair | Horse Hair |
| 1016 | horse_mackerel | Horse Mackerel |
| 1017 | horse_plushie | Horse Plushie |
| 1018 | horse_potion | MISSING | **GLITCHED** |
| 1019 | horseradish | Horseradish |
| 1020 | horseradish_salmon | Horseradish Salmon |
| 1021 | hot_cocoa | Hot Chocolate |
| 1022 | hot_potato | Hot Potato |
| 1023 | hot_toddy | Hot Toddy |
| 1024 | humble_pie | Humble Pie |
| 1025 | hummingbird_hawk_moth | Hummingbird Hawk Moth |
| 1026 | hydrangea | Hydrangea |
| 1027 | ice_block | Ice Block |
| 1028 | ice_cream_sundae | Ice Cream Sundae |
| 1029 | iced_coffee | Iced Coffee |
| 1030 | icehopper | Icehopper |
| 1031 | inchworm | Inchworm |
| 1032 | incredibly_hot_pot | Incredibly Hot Pot |
| 1033 | inn_candle | Inn Candle |
| 1034 | inn_counter | Inn Counter |
| 1035 | inn_cross_stitch | Inn Cross Stitch |
| 1036 | inn_kitchen_shelves | Inn Kitchen Shelves |
| 1037 | inn_stool | Inn Stool |
| 1038 | inn_tile_flooring | Inn Tile Flooring |
| 1039 | inn_wooden_flooring | Inn Wooden Flooring |
| 1042 | insect_flooring | Insect Flooring |
| 1045 | insect_rug | Insect Rug |
| 1048 | insect_wallpaper | Insect Wallpaper |
| 1049 | iris | Iris |
| 1050 | iron_armor | Iron Armor |
| 1051 | iron_greaves | Iron Greaves |
| 1052 | iron_helmet | Iron Helmet |
| 1053 | iron_ingot | Iron Ingot |
| 1054 | iron_legplates | Iron Legplates |
| 1055 | iron_ring | Iron Ring |
| 1056 | jam_sandwich | Jam Sandwich |
| 1057 | jar_large | Large Jar |
| 1058 | jar_medium | Medium Jar |
| 1059 | jar_pair | Pair of Jars |
| 1060 | jar_small | Small Jar |
| 1061 | jasmine | Jasmine |
| 1062 | jasmine_tea | Jasmine Tea |
| 1063 | jellyfish | Jellyfish |
| 1064 | jewel_beetle | Jewel Beetle |
| 1065 | journal | Diary |
| 1066 | killifish | Killifish |
| 1067 | king_crab | King Crab |
| 1068 | koi | Koi |
| 1069 | ladybug | Ladybug |
| 1072 | ladybug_table | Ladybug Table |
| 1073 | lake_chub | Lake Chub |
| 1074 | lake_trout | Lake Trout |
| 1075 | lamprey | Lamprey |
| 1076 | lantern_moth | Lantern Moth |
| 1077 | large_animal_ball_court | Large Animal Ball Court |
| 1078 | large_barn_black_blueprint | Large White Barn Blueprint |
| 1079 | large_barn_red_blueprint | Large Red Barn Blueprint |
| 1080 | large_coop_black_blueprint | Large White Coop Blueprint |
| 1081 | large_coop_red_blueprint | Large Red Coop Blueprint |
| 1089 | large_fluffy_rug | Large Fluffy Rug |
| 1090 | large_mob_coin | coin | **GLITCHED** |
| 1093 | large_paving_stone | Large Paving Stone |
| 1094 | large_pouch | Bag Upgrade |
| 1095 | latte | Latte |
| 1104 | lava_caves_bed | Obsidian Double Bed |
| 1102 | lava_caves_chair | Obsidian Chair |
| 1110 | lava_caves_chest | Obsidian Storage Chest |
| 1096 | lava_caves_diamond_rock | Ornamental Diamond Rock |
| 1106 | lava_caves_fence | Obsidian Fence |
| 1108 | lava_caves_floor_lamp | Obsidian Floor Lamp |
| 1097 | lava_caves_flooring | Lava Rock Floor |
| 1098 | lava_caves_gold_node | Ornamental Gold Node |
| 1113 | lava_caves_stone_arch | Lava Rock Arch |
| 1112 | lava_caves_table | Obsidian Table |
| 1114 | lava_caves_wallpaper | Lava Rock Wall |
| 1115 | lava_chestnuts | Lava Chestnuts |
| 1116 | lava_piranha | Lava Piranha |
| 1117 | lava_snail | Lava Snail |
| 1118 | lavender_tea | Lavender Tea |
| 1119 | leaf_fish | Leaf Fish |
| 1120 | leafhopper | Leafhopper |
| 1121 | lemon | Lemon |
| 1122 | lemon_pie | Lemon Pie |
| 1123 | lemonade | Lemonade |
| 1124 | lightning_dragonfly | Lightning Dragonfly |
| 1125 | lightning_fish | Lightning Fish |
| 1126 | lilac | Lilac |
| 1127 | loach | Loach |
| 1128 | loaded_baked_potato | Loaded Baked Potato |
| 1129 | loam_caterpillar | Loam Caterpillar |
| 1130 | lobster | Lobster |
| 1131 | lobster_roll | Lobster Roll |
| 1132 | lost_crown_of_aldaria | Lost Crown of Aldaria |
| 1133 | lovely_cottage_scroll_bundle | Lovely Cottage Scroll Bundle |
| 1134 | low_tide_flooring | Low Tide Flooring |
| 1135 | luna_moth | Luna Moth |
| 1136 | lurid_colored_drink | MISSING | **GLITCHED** |
| 1137 | mackerel | Mackerel |
| 1138 | mackerel_sashimi | Mackerel Sashimi |
| 1139 | magma_beetle | Magma Beetle |
| 1140 | mana_potion | Mana Potion |
| 1141 | marigold | Marigold |
| 1142 | marmalade | Marmalade |
| 1143 | mayonnaise | Mayonnaise |
| 1144 | medium_barn_black_blueprint | Medium White Barn Blueprint |
| 1145 | medium_barn_red_blueprint | Medium Red Barn Blueprint |
| 1146 | medium_coop_black_blueprint | Medium White Coop Blueprint |
| 1147 | medium_coop_red_blueprint | Medium Red Coop Blueprint |
| 1148 | medium_mob_coin | coin | **GLITCHED** |
| 1154 | mermaid_bed | Mermaid Double Bed |
| 1155 | mermaids_comb | Mermaid's Comb |
| 1156 | metal_leaf | Metal Leaf |
| 1157 | meteorite | Meteorite |
| 1158 | middlemist | Middlemist |
| 1159 | mine_cricket | Mine Cricket |
| 1170 | minecart | Minecart |
| 1160 | miners_copper_node | Ornamental Copper Node |
| 1162 | miners_crate_chair | Mines Crate Chair |
| 1164 | miners_crate_chest | Mines Storage Chest |
| 1166 | miners_floor_lamp | Mines Floor Lamp |
| 1167 | miners_flooring | Mines Flooring |
| 1168 | miners_helmet | Miner's Helmet |
| 1171 | miners_mushroom_stew | Miner's Mushroom Stew |
| 1172 | miners_pickaxe | Miner's Pickaxe |
| 1173 | miners_ruby_rock | Ornamental Ruby Rock |
| 1174 | miners_rucksack | Miner's Rucksack |
| 1176 | miners_shelves | Mines Shelf |
| 1177 | miners_slab | Miner's Slab |
| 1179 | miners_wall_lamp | Mines Wall Lamp |
| 1180 | miners_wallpaper | Mines Wall |
| 1181 | mines_mussels | Mines Mussels |
| 1182 | mines_wooden_arch | Mines Arch |
| 1183 | mini_whale_shark | Mini-Whale Shark |
| 1184 | minnow | Minnow |
| 1185 | mint_gimlet | Mint Gimlet |
| 1186 | mist_held_item | Invalid Item | **GLITCHED** |
| 1187 | mistmoth | Mistmoth |
| 1188 | mistria_history_book | Mistria History Book |
| 1189 | mistria_history_scroll | Caldosian Decorative Scroll |
| 1190 | mistria_wall_map | Mistria Wall Map |
| 1191 | mistril_ingot | Mistril Ingot |
| 1192 | mixed_fruit_juice | Mixed Fruit Juice |
| 1193 | mob_coin | coin | **GLITCHED** |
| 1194 | mocha | Mocha |
| 1195 | monarch_butterfly | Monarch Butterfly |
| 1196 | monster_cookie | Monster Cookies |
| 1197 | monster_core | Monster Core |
| 1198 | monster_fang | Monster Fang |
| 1199 | monster_hoop | Monster Hoop |
| 1200 | monster_horn | Monster Horn |
| 1201 | monster_mash | Monster Mash |
| 1202 | monster_powder | Monster Powder |
| 1203 | monster_shell | Monster Shell |
| 1204 | monster_wing | Monster Wing |
| 1205 | mont_blanc | Mont Blanc |
| 1206 | moon_fruit | Moon Fruit |
| 1207 | moon_fruit_cake | Moon Fruit Cake |
| 1210 | moon_gate | Moon Gate |
| 1211 | morel_mushroom | Morel Mushroom |
| 1212 | mote_firefly | Mote Firefly |
| 1215 | moth_lamp | Moth Lamp |
| 1216 | mournful_clown_painting | Mournful Clown Painting |
| 1217 | mullet | Mullet |
| 1218 | mushroom_brew | Mushroom Brew |
| 1219 | mushroom_rice | Mushroom Rice |
| 1220 | mushroom_steak_dinner | Mushroom Steak Dinner |
| 1221 | muskie | Muskie |
| 1222 | muttering_cube | Muttering Cube |
| 1223 | narrows_moss | Narrows Moss |
| 1224 | net_copper | Copper Net |
| 1225 | net_gold | Gold Net |
| 1226 | net_iron | Iron Net |
| 1227 | net_mistril | Mistril Net |
| 1228 | net_silver | Silver Net |
| 1229 | net_worn | Worn Net |
| 1230 | nettle | Nettle |
| 1231 | newt | Newt |
| 1232 | night_queen | Night Queen |
| 1233 | noodles | Noodles |
| 1235 | notepad | Notepad |
| 1236 | oarfish | Oarfish |
| 1237 | obsidian | Obsidian |
| 1238 | obsidian_blade | Obsidian Blade |
| 1239 | ocarina_sprite_statue | Animal Sprite Statue |
| 1240 | ocean_sunfish | Ocean Sunfish |
| 1241 | octopus | Octopus |
| 1242 | octopus_plushie | Octopus Plushie |
| 1243 | oil | Oil |
| 1244 | omelet | Omelet |
| 1245 | onion | Onion |
| 1246 | onion_soup | Onion Soup |
| 1247 | orange | Orange |
| 1248 | orange_juice | Orange Juice |
| 1249 | orchid_mantis | Orchid Mantis |
| 1250 | ore_copper | Copper Ore |
| 1251 | ore_diamond | Diamond |
| 1252 | ore_emerald | Emerald |
| 1253 | ore_gold | Gold Ore |
| 1254 | ore_iron | Iron Ore |
| 1255 | ore_mistril | Mistril Ore |
| 1256 | ore_pink_diamond | Pink Diamond |
| 1257 | ore_ruby | Ruby |
| 1258 | ore_sapphire | Sapphire |
| 1259 | ore_silver | Silver Ore |
| 1260 | ore_stone | Stone |
| 1261 | oregano | Oregano |
| 1262 | ornamental_iron_node | Ornamental Iron Node |
| 1263 | ornamental_sapphire_rock | Ornamental Sapphire Rock |
| 1266 | ornate_flagstone | Ornate Flagstone |
| 1269 | ornate_rug_large_rectangle | Ornate Large Rectangle Rug |
| 1272 | ornate_rug_large_round | Ornate Large Round Rug |
| 1275 | ornate_rug_large_square | Ornate Large Square Rug |
| 1278 | ornate_rug_medium_round | Ornate Medium Round Rug |
| 1281 | ornate_rug_medium_square | Ornate Medium Square Rug |
| 1284 | ornate_rug_small | Ornate Small Rug |
| 1285 | oyster_mushroom | Oyster Mushroom |
| 1286 | paddlefish | Paddlefish |
| 1287 | pan_fried_bream | Pan Fried Bream |
| 1288 | pan_fried_salmon | Pan-fried Salmon |
| 1289 | pan_fried_snapper | Pan-fried Snapper |
| 1290 | paper | Paper |
| 1291 | paper_pondshell | Paper Pondshell |
| 1292 | paper_stack | Paper Stack |
| 1293 | peach | Peach |
| 1294 | peaches_and_cream | Peaches and Cream |
| 1295 | pear | Pear |
| 1296 | pearl_clam | Pearl Clam |
| 1297 | peas | Peas |
| 1298 | peat | Peat |
| 1299 | pebble_minnow | Pebble Minnow |
| 1300 | perch | Perch |
| 1301 | perch_risotto | Perch Risotto |
| 1302 | perfect_copper_ore | Perfect Copper Ore |
| 1303 | perfect_diamond | Perfect Diamond |
| 1304 | perfect_emerald | Perfect Emerald |
| 1305 | perfect_gift | MISSING |
| 1306 | perfect_gold_ore | Perfect Gold Ore |
| 1307 | perfect_iron_ore | Perfect Iron Ore |
| 1308 | perfect_mistril_ore | Perfect Mistril Ore |
| 1309 | perfect_pink_diamond | Perfect Pink Diamond |
| 1310 | perfect_ruby | Perfect Ruby |
| 1311 | perfect_sapphire | Perfect Sapphire |
| 1312 | perfect_silver_ore | Perfect Silver Ore |
| 1313 | pet_cosmetic | MISSING |
| 1314 | petalhopper | Petalhopper |
| 1315 | petrified_wood | Petrified Wood |
| 1316 | pick_axe_copper | Copper Pickaxe |
| 1317 | pick_axe_gold | Gold Pickaxe |
| 1318 | pick_axe_iron | Iron Pickaxe |
| 1319 | pick_axe_mistril | Mistril Pickaxe |
| 1320 | pick_axe_silver | Silver Pickaxe |
| 1321 | pick_axe_worn | Worn Pickaxe |
| 1322 | picket_fence | Picket Fence |
| 1325 | picnic_basket | Picnic Basket |
| 1326 | picnic_donut_plate | Picnic Display Donuts |
| 1327 | picnic_hamburger_plate | Picnic Display Burgers |
| 1330 | picnic_pie_plate | Picnic Display Pie |
| 1333 | picnic_place_setting | Picnic Place Setting |
| 1336 | picnic_rug | Picnic Blanket |
| 1337 | picnic_sandwich_plate | Picnic Display Sandwiches |
| 1340 | picnic_sunflower_vase | Picnic Sunflower Vase |
| 1341 | pike | Pike |
| 1342 | pinecone | Pinecone |
| 1343 | pineshroom | Pineshroom |
| 1344 | pineshroom_toast | Pineshroom Toast |
| 1345 | pink_pet_bed | Pink Pet Bed |
| 1346 | pink_pet_dish | Pink Pet Dish |
| 1347 | pink_scallop_shell | Pink Scallop Shell |
| 1348 | pizza | Pizza |
| 1349 | plum_blossom | Plum Blossom |
| 1350 | poached_pear | Poached Pear |
| 1351 | poinsettia | Poinsettia |
| 1352 | polar_fly | Polar Fly |
| 1353 | pollock | Pollock |
| 1354 | pomegranate | Pomegranate |
| 1355 | pomegranate_juice | Pomegranate Juice |
| 1356 | pomegranate_sorbet | Pomegranate Sorbet |
| 1357 | pond_skater | Pond Skater |
| 1358 | pond_snail | Pond Snail |
| 1359 | potato | Potato |
| 1360 | potato_soup | Potato Soup |
| 1361 | praying_mantis | Praying Mantis |
| 1362 | pudding | Pudding |
| 1363 | puddle_spider | Puddle Spider |
| 1364 | puffer_fish | Puffer Fish |
| 1365 | pumpkin | Pumpkin |
| 1366 | pumpkin_pie | Pumpkin Pie |
| 1367 | pumpkin_stew | Pumpkin Stew |
| 1368 | purple_mushroom | Purple Mushroom |
| 1369 | purse | Purse of Tesserae |
| 1370 | quality_hay | Quality Large Animal Feed |
| 1371 | quality_small_animal_feed | Quality Small Animal Feed |
| 1372 | question_mark_butterfly | Question Mark Butterfly |
| 1373 | quiche | Quiche |
| 1374 | rabbit_plushie | Rabbit Plushie |
| 1375 | rabbit_wool | Rabbit Wool |
| 1376 | rainbow_geode | Rainbow Geode |
| 1377 | rainbow_seaweed | Rainbow Seaweed |
| 1378 | rainbow_trout | Rainbow Trout |
| 1379 | razorback | Razorback |
| 1380 | really_round_rock | Really Round Rock |
| 1381 | recipe_scroll | Recipe: {} | **DISABLED** |
| 1382 | red_capybara_wall_ribbon | Red Capybara Wall Ribbon |
| 1383 | red_chicken_wall_ribbon | Red Chicken Wall Ribbon |
| 1384 | red_duck_wall_ribbon | Red Duck Wall Ribbon |
| 1385 | red_obsidian | Red Obsidian |
| 1386 | red_rabbit_wall_ribbon | Red Rabbit Wall Ribbon |
| 1387 | red_snapper | Red Snapper |
| 1388 | red_snapper_sushi | Red Snapper Sushi |
| 1389 | red_toadstool | Red Toadstool |
| 1390 | red_wine | Red Wine |
| 1391 | redhead_worm | Redhead Worm |
| 1392 | refined_stone | Refined Stone |
| 1393 | relic_crab | Relic Crab |
| 1394 | restorative_syrup | Restorative Syrup |
| 1395 | rhinoceros_beetle | Rhinoceros Beetle |
| 1396 | rice | Rice |
| 1397 | rice_stalk | Rice Stalk |
| 1398 | riceball | Rice Ball |
| 1399 | ritual_beads | Ritual Beads |
| 1400 | ritual_chalice | Ritual Chalice |
| 1401 | ritual_incense_burner | Ritual Incense Burner |
| 1402 | ritual_scepter | Ritual Scepter |
| 1403 | ritual_tablet | Ritual Tablet |
| 1404 | river_snail | River Snail |
| 1405 | roach | Roach |
| 1406 | roasted_cauliflower | Roasted Cauliflower |
| 1407 | roasted_chestnuts | Roasted Chestnuts |
| 1408 | roasted_rice_tea | Roasted Rice Tea |
| 1409 | roasted_sweet_potato | Roasted Sweet Potato |
| 1410 | rock_bass | Rock Bass |
| 1411 | rock_guppy | Rock Guppy |
| 1412 | rock_roach | Rock Roach |
| 1413 | rock_salt | Rock Salt |
| 1414 | rock_statue | Rock Statue |
| 1415 | rock_with_a_hole | Rock with a Hole |
| 1416 | rockbiter | Rockbiter |
| 1417 | rockroot | Rockroot |
| 1418 | roly_poly | Roly Poly |
| 1419 | rose | Rose |
| 1420 | rose_hip | Rose Hip |
| 1421 | rose_tea | Rose Tea |
| 1422 | rosehip_jam | Rose Hip Jam |
| 1423 | rosemary | Rosemary |
| 1424 | rosemary_garlic_noodles | Rosemary Garlic Noodles |
| 1432 | round_cushioned_stool | Round Cushioned Stool |
| 1433 | rubber_fish | Rubber Fish |
| 1434 | rusted_shield | Rusted Shield |
| 1435 | rusted_treasure_chest | Rusted Treasure Chest |
| 1436 | rustic_cabin_scroll_bundle | Rustic Cabin Scroll Bundle |
| 1437 | ryis_lumber | Ryis' Lumber | **GLITCHED** |
| 1438 | sage | Sage |
| 1439 | salmon | Salmon |
| 1440 | salmon_sashimi | Salmon Sashimi |
| 1441 | salted_watermelon | Salted Watermelon |
| 1442 | sand_bug | Sand Bug |
| 1443 | sand_dollar | Sand Dollar |
| 1444 | sand_lance | Sand Lance |
| 1445 | sand_rug | Sand Rug |
| 1446 | sap | Sap |
| 1447 | sapling_apple | Apple Sapling |
| 1448 | sapling_cherry | Cherry Sapling |
| 1449 | sapling_lemon | Lemon Sapling |
| 1450 | sapling_oak | Oak Sapling |
| 1451 | sapling_orange | Orange Sapling |
| 1452 | sapling_peach | Peach Sapling |
| 1453 | sapling_pear | Pear Sapling |
| 1454 | sapling_pine | Pine Sapling |
| 1455 | sapling_pomegranate | Pomegranate Sapling |
| 1456 | sapphire_betta | Sapphire Betta |
| 1457 | sardine | Sardine |
| 1458 | sauger | Sauger |
| 1459 | saury | Saury |
| 1460 | sauteed_snow_peas | Sauteed Snow Peas |
| 1461 | scent_of_spring | Scent of Spring |
| 1462 | scrap_metal_boots | Scrap Metal Boots |
| 1463 | scrap_metal_chestpiece | Scrap Metal Chestpiece |
| 1464 | scrap_metal_helmet | Scrap Metal Helmet |
| 1465 | scrap_metal_pants | Scrap Metal Pants |
| 1466 | scrap_metal_ring | Scrap Metal Ring |
| 1467 | sea_bass | Sea Bass |
| 1468 | sea_bream | Sea Bream |
| 1469 | sea_bream_rice | Sea Bream Rice |
| 1470 | sea_grapes | Sea Grapes |
| 1471 | sea_scarab | Sea Scarab |
| 1472 | seafood_boil | Seafood Boil |
| 1473 | seafood_snow_pea_noodles | Seafood Snow Pea Noodles |
| 1474 | sealing_scroll | Sealing Scroll |
| 1475 | seaweed | Seaweed |
| 1476 | seaweed_salad | Seaweed Salad |
| 1477 | seed_basil | Basil Seed |
| 1478 | seed_beet | Beet Seed |
| 1479 | seed_broccoli | Broccoli Seed |
| 1480 | seed_burdock_root | Burdock Root Seed |
| 1481 | seed_cabbage | Cabbage Seed |
| 1482 | seed_carrot | Carrot Seed |
| 1483 | seed_catmint | Catmint Seed |
| 1484 | seed_cauliflower | Cauliflower Seed |
| 1485 | seed_celosia | Celosia Seed |
| 1486 | seed_chickpea | Chickpea Seed |
| 1487 | seed_chili_pepper | Chili Pepper Seed |
| 1488 | seed_chrysanthemum | Chrysanthemum Seed |
| 1489 | seed_corn | Corn Seed |
| 1490 | seed_cosmos | Cosmos Seed |
| 1491 | seed_cranberry | Cranberry Seed |
| 1492 | seed_cucumber | Cucumber Seed |
| 1493 | seed_daffodil | Daffodil Seed |
| 1494 | seed_daikon_radish | Daikon Radish Seed |
| 1495 | seed_daisy | Daisy Seed |
| 1496 | seed_dill | Dill Seed |
| 1497 | seed_frost_lily | Frost Lily Seed |
| 1498 | seed_garlic | Garlic Seed |
| 1499 | seed_heather | Heather Seed |
| 1500 | seed_iris | Iris Seed |
| 1501 | seed_jasmine | Jasmine Seed |
| 1502 | seed_lilac | Lilac Seed |
| 1503 | seed_marigold | Marigold Seed |
| 1504 | seed_moon_fruit | Moon Fruit Seed |
| 1505 | seed_night_queen | Night Queen Seed |
| 1506 | seed_onion | Onion Seed |
| 1507 | seed_oregano | Oregano Seed |
| 1508 | seed_peas | Pea Seed |
| 1509 | seed_poinsettia | Poinsettia Seed |
| 1510 | seed_potato | Potato Seed |
| 1511 | seed_pumpkin | Pumpkin Seed |
| 1512 | seed_rice | Rice Seed |
| 1513 | seed_rosemary | Rosemary Seed |
| 1514 | seed_sage | Sage Seed |
| 1515 | seed_snapdragon | Snapdragon Seed |
| 1516 | seed_snow_peas | Snow Peas Seed |
| 1517 | seed_snowdrop_anemone | Snowdrop Anemone Seed |
| 1518 | seed_strawberry | Strawberry Seed |
| 1519 | seed_sugar_cane | Sugar Cane Seed |
| 1520 | seed_sunflower | Sunflower Seed |
| 1521 | seed_sweet_potato | Sweet Potato Seed |
| 1522 | seed_tea | Tea Seed |
| 1523 | seed_temple_flower | Temple Flower Seed |
| 1524 | seed_thyme | Thyme Seed |
| 1525 | seed_tomato | Tomato Seed |
| 1526 | seed_tulip | Tulip Seed |
| 1527 | seed_turnip | Turnip Seed |
| 1528 | seed_viola | Viola Seed |
| 1529 | seed_watermelon | Watermelon Seed |
| 1530 | seed_wheat | Wheat Seed |
| 1531 | seriously_square_stone | Seriously Square Stone |
| 1532 | sesame | Sesame |
| 1533 | sesame_broccoli | Sesame Broccoli |
| 1534 | sesame_tuna_bowl | Sesame Tuna Bowl |
| 1535 | shad | Shad |
| 1536 | shadow_bass | Shadow Bass |
| 1537 | shadow_flower | Shadow Flower |
| 1538 | shale_grass | Shale Grass |
| 1539 | shard_mass | Shard Mass |
| 1540 | shardfin | Shardfin |
| 1541 | shards | Shards |
| 1542 | shark | Shark |
| 1543 | sheep_plushie | Sheep Plushie |
| 1544 | sheep_wool | Sheep Wool |
| 1545 | shortcut_scroll | Shortcut Scroll |
| 1546 | shovel_copper | Copper Shovel |
| 1547 | shovel_gold | Gold Shovel |
| 1548 | shovel_iron | Iron Shovel |
| 1549 | shovel_mistril | Mistril Shovel |
| 1550 | shovel_silver | Silver Shovel |
| 1551 | shovel_worn | Worn Shovel |
| 1552 | shrimp | Shrimp |
| 1553 | silver_alpaca_trophy | Silver Alpaca Trophy |
| 1554 | silver_armor | Silver Armor |
| 1555 | silver_cow_trophy | Silver Cow Trophy |
| 1556 | silver_greaves | Silver Greaves |
| 1557 | silver_helmet | Silver Helmet |
| 1558 | silver_horse_trophy | Silver Horse Trophy |
| 1559 | silver_ingot | Silver Ingot |
| 1560 | silver_legplates | Silver Legplates |
| 1561 | silver_redhorse | Silver Redhorse |
| 1562 | silver_ring | Silver Ring |
| 1563 | silver_sheep_trophy | Silver Sheep Trophy |
| 1564 | simmered_daikon | Simmered Daikon |
| 1565 | singing_katydid | Singing Katydid |
| 1566 | sliced_turnip | Sliced Turnip |
| 1567 | small_animal_seesaw | Small Animal Seesaw |
| 1568 | small_barn_black_blueprint | Small White Barn Blueprint |
| 1569 | small_barn_red_blueprint | Small Red Barn Blueprint |
| 1577 | small_bathmat | Small Bathmat |
| 1578 | small_coop_black_blueprint | Small White Coop Blueprint |
| 1579 | small_coop_red_blueprint | Small Red Coop Blueprint |
| 1580 | small_grass_starter | Grass Starter |
| 1584 | small_vase | Small Vase |
| 1585 | smallmouth_bass | Smallmouth Bass |
| 1586 | smoke_moth | Smoke Moth |
| 1587 | smoked_trout_soup | Smoked Trout Soup |
| 1588 | snail | Snail |
| 1589 | snakehead | Snakehead |
| 1590 | snapdragon | Snapdragon |
| 1591 | snapping_turtle | Snapping Turtle |
| 1592 | snow_fish | Snow Fish |
| 1593 | snow_peas | Snow Peas |
| 1594 | snowball_beetle | Snowball Beetle |
| 1595 | snowdrop_anemone | Snowdrop Anemone |
| 1596 | snug_bug | Snug Bug |
| 1597 | sod | Sod |
| 1598 | soup_of_the_day | Soup of the Day | **GLITCHED** |
| 1599 | soup_of_the_day_gold | Soup of the Day Deluxe | **GLITCHED** |
| 1600 | sour_lemon_cake | Lemon Cake |
| 1601 | soy_sauce | Soy Sauce |
| 1602 | speedy_snail | Speedy Snail |
| 1603 | speedy_syrup | Speedy Syrup |
| 1604 | spell_fruit | Spell Fruit |
| 1605 | spell_fruit_parfait | Spell Fruit Parfait |
| 1606 | spicy_cheddar_biscuit | Spicy Cheddar Biscuit |
| 1607 | spicy_corn | Spicy Corn |
| 1608 | spicy_crab_sushi | Spicy Crab Sushi |
| 1609 | spicy_water_chestnuts | Spicy Water Chestnuts |
| 1610 | spirit_mushroom | Spirit Mushroom |
| 1611 | spirit_mushroom_tea | Spirit Mushroom Tea |
| 1612 | spirula_shell | Spirula Shell |
| 1613 | spooky_haybale | Hay Bale |
| 1616 | spring_bed | Spring Bed |
| 1617 | spring_crop_sign_scroll_bundle | Spring Crop Sign Scroll Bundle |
| 1620 | spring_double_bed | Spring Double Bed |
| 1621 | spring_festival_basket | Breath of Spring Basket |
| 1622 | spring_festival_breath_of_spring_wreath | Deluxe Breath of Spring Wreath |
| 1623 | spring_festival_flower_chest | Breath of Spring Storage Chest |
| 1624 | spring_festival_large_planter | Breath of Spring Large Planter |
| 1625 | spring_festival_planter | Breath of Spring Planter |
| 1626 | spring_festival_wreath | Breath of Spring Wreath |
| 1629 | spring_flooring | Spring Flooring |
| 1630 | spring_galette | Spring Galette |
| 1633 | spring_lamp | Spring Lamp |
| 1636 | spring_potted_tree | Spring Potted Tree |
| 1639 | spring_rug | Spring Flower Rug |
| 1640 | spring_salad | Spring Salad |
| 1643 | spring_sofa | Spring Sofa |
| 1646 | spring_wallpaper | Spring Wallpaper |
| 1647 | square_tile_floor | Square Tile Floor |
| 1648 | squid | Squid |
| 1649 | stamina_syrup | Stamina Syrup |
| 1650 | stamina_up | Stamina Up |
| 1651 | star_brooch | Star Brooch |
| 1657 | star_cushion | Star Cushion |
| 1662 | star_lantern | Star Lantern |
| 1663 | star_shaped_cookie | Star-Shaped Cookie |
| 1667 | star_viewing_blanket | Star Viewing Blanket |
| 1668 | starlight_coral | Starlight Coral |
| 1670 | starry_flooring | Starry Flooring |
| 1671 | starter_bird_house_red | Birdhouse |
| 1672 | starter_potted_plant | Potted Plant |
| 1673 | starter_scarecrow | Scarecrow |
| 1674 | starter_shipping_box | Shipping Bin |
| 1675 | starter_stone_path | Stone Path |
| 1676 | starter_stone_path_double | Stone Path |
| 1677 | starter_well | Water Well |
| 1678 | starter_wheelbarrow | Wheelbarrow |
| 1679 | starter_wood_fence | Wooden Fence |
| 1680 | statuette_of_caldarus | Statuette of Caldarus |
| 1681 | steamed_broccoli | Steamed Broccoli |
| 1682 | stingray | Stingray |
| 1683 | stinky_stamina_potion | MISSING | **GLITCHED** |
| 1686 | stone_bench | Stone Bench |
| 1689 | stone_dragon_figure | Stone Dragon Figure |
| 1690 | stone_horse | Stone Horse |
| 1693 | stone_lamp | Stone Lamp |
| 1694 | stone_loach | Stone Loach |
| 1695 | stone_shell | Stone Shell |
| 1698 | stone_storage_chest | Stone Storage Chest |
| 1701 | stone_table | Stone Table |
| 1702 | stone_wall | Stone Wall |
| 1703 | strawberries_and_cream | Strawberries and Cream |
| 1704 | strawberry | Strawberry |
| 1705 | strawberry_shortcake | Strawberry Shortcake |
| 1706 | strawhopper | Strawhopper |
| 1707 | striped_bass | Striped Bass |
| 1708 | strobe_firefly | Strobe Firefly |
| 1709 | sturgeon | Sturgeon |
| 1710 | sugar | Sugar |
| 1711 | sugar_cane | Sugar Cane |
| 1712 | sulfur_crab | Sulfur Crab |
| 1714 | summer_bed | Summer Bed |
| 1716 | summer_chair | Summer Chair |
| 1717 | summer_crop_sign_scroll_bundle | Summer Crop Sign Scroll Bundle |
| 1719 | summer_double_bed | Summer Double Bed |
| 1721 | summer_flooring | Summer Flooring |
| 1723 | summer_jar_lamp | Summer Jar Lamp |
| 1725 | summer_rug | Summer Flower Rug |
| 1726 | summer_salad | Summer Salad |
| 1728 | summer_table | Summer Table |
| 1730 | summer_wallpaper | Summer Wallpaper |
| 1732 | summit_wallpaper | Summit Wallpaper |
| 1733 | sunflower | Sunflower |
| 1734 | sunny | Sunny |
| 1735 | surf_beetle | Surf Beetle |
| 1736 | sushi_platter | Sushi Platter |
| 1737 | sweet_potato | Sweet Potato |
| 1738 | sweet_potato_pie | Sweet Potato Pie |
| 1739 | sweet_sesame_balls | Sweet Sesame Balls |
| 1740 | sweetfish | Sweetfish |
| 1741 | sweetroot | Sweetroot |
| 1742 | sword_copper | Copper Sword |
| 1743 | sword_crystal | Crystal Sword |
| 1744 | sword_gold | Gold Sword |
| 1745 | sword_iron | Iron Sword |
| 1746 | sword_mistril | Mistril Sword |
| 1747 | sword_scrap_metal | Scrap Metal Sword |
| 1748 | sword_silver | Silver Sword |
| 1749 | sword_tarnished_gold | Tarnished Gold Sword |
| 1750 | sword_verdigris | Verdigris Sword |
| 1751 | sword_worn | Worn Sword |
| 1752 | swordfish | Swordfish |
| 1753 | tarnished_gold_armor | Tarnished Gold Chestpiece |
| 1754 | tarnished_gold_boots | Tarnished Gold Greaves |
| 1755 | tarnished_gold_helmet | Tarnished Gold Helmet |
| 1756 | tarnished_gold_legplates | Tarnished Gold Legplates |
| 1757 | tarnished_gold_ring | Tarnished Gold Ring |
| 1758 | tarpon | Tarpon |
| 1759 | tea | Tea |
| 1760 | tea_black | Black Tea Cup |
| 1761 | tea_green | Green Tea Cup |
| 1762 | temple_flower | Temple Flower |
| 1763 | tetra | Tetra |
| 1764 | thorn_vine | Thorn Vine |
| 1765 | thyme | Thyme |
| 1766 | tide_lettuce | Tide Lettuce |
| 1767 | tide_salad | Tide Salad |
| 1768 | tide_touched_cavern_wall | Tide-Touched Cavern Wall |
| 1769 | tidestone | Tidestone |
| 1770 | tiger_swallowtail_butterfly | Tiger Swallowtail Butterfly |
| 1771 | tilapia | Tilapia |
| 1774 | tile_roof_fence | Tile Roof Fence |
| 1775 | tin_lunchbox | Tin Lunchbox |
| 1776 | tiny_dinosaur_skeleton | Tiny Dinosaur Skeleton |
| 1777 | tiny_volcano | Tiny Volcano |
| 1778 | toasted_sunflower_seeds | Toasted Sunflower Seeds |
| 1780 | toilet | Toilet |
| 1781 | tomato | Tomato |
| 1782 | tomato_soup | Tomato Soup |
| 1783 | trail_mix | Trail Mix |
| 1786 | training_dummy | Training Dummy |
| 1787 | transparent_jellyfish | Transparent Jellyfish |
| 1788 | treasure_box_copper | Copper Treasure Box |
| 1789 | treasure_box_gold | Golden Treasure Box |
| 1790 | treasure_box_silver | Silver Treasure Box |
| 1791 | treasure_box_wood | Wooden Treasure Box |
| 1792 | trilobite_fossil | Trilobite Fossil |
| 1793 | trout | Trout |
| 1794 | tulip | Tulip |
| 1795 | tulip_cake | Tulip Cake |
| 1796 | tuna | Tuna |
| 1797 | tuna_sashimi | Tuna Sashimi |
| 1798 | tunnel_millipede | Tunnel Millipede |
| 1799 | turnip | Turnip |
| 1800 | turnip_and_cabbage_salad | Turnip & Cabbage Salad |
| 1801 | turnip_and_potato_gratin | Turnip & Potato Gratin |
| 1802 | turtle | Turtle |
| 1803 | twice_baked_rations | Twice-Baked Rations |
| 1804 | ultimate_hay | Ultimate Large Animal Feed |
| 1805 | ultimate_small_animal_feed | Ultimate Small Animal Feed |
| 1806 | underseaweed | Underseaweed |
| 1807 | unidentified_artifact | Unidentified Artifact | **DISABLED** |
| 1808 | unknown_dragon_statuette | Unknown Dragon Statuette |
| 1809 | unknown_item | Unknown Item | **GLITCHED** |
| 1810 | unusual_seed | Unusual Seed | **GLITCHED** |
| 1811 | upper_mines_mushroom | Upper Mines Mushroom |
| 1812 | vegetable_pot_pie | Vegetable Pot Pie |
| 1813 | vegetable_quiche | Vegetable Quiche |
| 1814 | vegetable_soup | Vegetable Soup |
| 1815 | veggie_sub_sandwich | Veggie Sub Sandwich |
| 1816 | verdigris_boots | Verdigris Boots |
| 1817 | verdigris_chestpiece | Verdigris Chestpiece |
| 1818 | verdigris_helmet | Verdigris Helmet |
| 1819 | verdigris_ring | Verdigris Ring |
| 1820 | verdigris_tassets | Verdigris Tassets |
| 1821 | vintage_brush | Vintage Brush |
| 1822 | vintage_cow_bell | Vintage Cow Bell |
| 1823 | vintage_hammer | Vintage Hammer |
| 1824 | vintage_sickle | Vintage Sickle |
| 1825 | vintage_watering_can | Vintage Watering Can |
| 1826 | viola | Viola |
| 1827 | walking_leaf | Walking Leaf |
| 1828 | walking_stick | Walking Stick |
| 1829 | walleye | Walleye |
| 1830 | walnut | Walnut |
| 1831 | warm_rock | Warm Rock |
| 1832 | water_balloon_fish | Water Balloon Fish |
| 1833 | water_chestnut | Water Chestnut |
| 1834 | water_chestnut_fritters | Water Chestnut Fritters |
| 1835 | water_sphere | Water Sphere |
| 1840 | water_sprite | Water Sprite Statue |
| 1841 | water_tablet_rubbing | Water Tablet Rubbing |
| 1842 | waterbug | Waterbug |
| 1843 | waterfly | Waterfly |
| 1844 | watering_can_copper | Copper Watering Can |
| 1845 | watering_can_gold | Gold Watering Can |
| 1846 | watering_can_iron | Iron Watering Can |
| 1847 | watering_can_mistril | Mistril Watering Can |
| 1848 | watering_can_silver | Silver Watering Can |
| 1849 | watering_can_worn | Worn Watering Can |
| 1850 | watermelon | Watermelon |
| 1851 | weather_crystal_ball | Weather Crystal Ball |
| 1852 | weightless_stone | Weightless Stone |
| 1853 | wheat | Wheat |
| 1854 | white_capybara_wall_ribbon | White Capybara Wall Ribbon |
| 1855 | white_chicken_wall_ribbon | White Chicken Wall Ribbon |
| 1856 | white_duck_wall_ribbon | White Duck Wall Ribbon |
| 1857 | white_perch | White Perch |
| 1858 | white_rabbit_wall_ribbon | White Rabbit Wall Ribbon |
| 1859 | white_wine | White Wine |
| 1860 | wild_berries | Wild Berries |
| 1861 | wild_berry_jam | Wild Berry Jam |
| 1862 | wild_grapes | Wild Grapes |
| 1863 | wild_leek | Wild Leek |
| 1864 | wild_mushroom | Cave Mushroom |
| 1865 | wildberry_pie | Wild Berry Pie |
| 1866 | wildberry_scone | Wild Berry Scone |
| 1867 | windleaf_butterfly | Windleaf Butterfly |
| 1868 | wine_glass_red | Red Wine Glass |
| 1869 | wine_glass_rose | Rose Wine Glass |
| 1870 | wine_glass_white | White Wine Glass |
| 1871 | winter_crop_sign_scroll_bundle | Winter Crop Sign Scroll Bundle |
| 1872 | winter_stew | Winter Stew |
| 1873 | wintergreen_berry | Wintergreen Berry |
| 1874 | wintergreen_ice_cream | Wintergreen Ice Cream |
| 1875 | winterpillar | Winterpillar |
| 1876 | witch_queen_bed | Witch Queen Bed |
| 1877 | witch_queen_cauldron | Witch Queen Cauldron |
| 1878 | witch_queen_chair | Witch Queen Chair |
| 1879 | witch_queen_double_bed | Witch Queen Double Bed |
| 1880 | witch_queen_dressing_table | Witch Queen Dressing Table |
| 1881 | witch_queen_flooring | Witch Queen Flooring |
| 1882 | witch_queen_moon_lamp | Witch Queen Moon Lamp |
| 1883 | witch_queen_nightstand | Witch Queen Nightstand |
| 1884 | witch_queen_pillar | Witch Queen Pillar |
| 1885 | witch_queen_rug | Witch Queen Rug |
| 1886 | witch_queen_table | Witch Queen Table |
| 1887 | witch_queen_throne | Witch Queen Throne |
| 1888 | witch_queen_wallpaper | Witch Queen Wallpaper |
| 1889 | woodcrafting_station | Crafting Station |
| 1890 | world_fountain | World Fountain 20 |
| 1891 | worm | Worm |
| 1892 | worn_bed | Worn Bed |
| 1893 | worn_chair | Worn Chair |
| 1894 | worn_fireplace | Worn Fireplace |
| 1895 | worn_flooring | Worn Flooring |
| 1896 | worn_garden_lamp | Worn Garden Lamp |
| 1897 | worn_nightstand | Worn Nightstand |
| 1898 | worn_rug | Worn Rug |
| 1899 | worn_table | Worn Table |
| 1900 | worn_wallpaper | Worn Wallpaper |
| 1901 | worn_well | Worn Well |
| 1902 | worn_window | Worn Window |

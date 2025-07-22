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


## Item Table (FoM v0.14.0)
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
| 12 | alda_pot_replica | Alda Pot Replica |
| 13 | aldarian_gauntlet | Aldarian Gauntlet |
| 14 | aldarian_sword | Aldarian Sword |
| 15 | aldarian_war_banner | Aldarian War Banner |
| 16 | alligator_gar | Alligator Gar |
| 17 | alpaca_plushie | Alpaca Plushie |
| 18 | alpaca_wool | Alpaca Wool |
| 19 | amber_trapped_insect | Amber Trapped Insect |
| 20 | anchovy | Anchovy |
| 21 | ancient_crystal_goblet | Ancient Crystal Goblet |
| 22 | ancient_gold_coin | Ancient Gold Coin |
| 23 | ancient_horn_circlet | Ancient Horn Circlet |
| 24 | ancient_royal_scepter | Ancient Royal Scepter |
| 25 | ancient_stone_lantern | Ancient Stone Lantern |
| 26 | angel_fish | Angel Fish |
| 27 | animal_cosmetic | MISSING | **DISABLED** |
| 28 | animal_currency | Shiny Bead |
| 29 | animated_snow_globe | Animated Snow Globe |
| 30 | ant | Ant |
| 31 | apple | Apple |
| 32 | apple_honey_curry | Apple Honey Curry |
| 33 | apple_juice | Apple Juice |
| 34 | apple_pie | Apple Pie |
| 35 | archerfish | Archerfish |
| 36 | armored_bass | Armored Bass |
| 37 | ash_mushroom | Ash Mushroom |
| 38 | autumn_scarecrow | Autumn Scarecrow |
| 39 | axe_copper | Copper Axe |
| 40 | axe_gold | Gold Axe |
| 41 | axe_iron | Iron Axe |
| 42 | axe_mistril | Mistril Axe |
| 43 | axe_silver | Silver Axe |
| 44 | axe_worn | Worn Axe |
| 45 | baked_potato | Baked Potato |
| 46 | baked_sweetroot | Baked Sweetroot |
| 48 | bakery_bread_basket | Bakery Bread Basket |
| 52 | bakery_cake | Bakery Display Cake |
| 50 | bakery_cake_case | Bakery Dessert Case |
| 54 | bakery_cake_slice | Bakery Display Cake Slice |
| 56 | bakery_cookie_jar | Bakery Cookie Jar |
| 58 | bakery_cutting_board | Bakery Cutting Board |
| 59 | balors_crate | Balor's Crate | **GLITCHED** |
| 60 | barb | Barb |
| 62 | basic_area_rug | Basic Area Rug |
| 66 | basic_bed | Basic Single Bed |
| 64 | basic_bed_double | Basic Double Bed |
| 68 | basic_bookshelf | Basic Bookshelf |
| 70 | basic_chair | Basic Chair |
| 72 | basic_doormat | Basic Doormat |
| 74 | basic_dresser | Basic Dresser |
| 76 | basic_flooring | Basic Flooring |
| 78 | basic_flower_pot | Basic Flower Pot |
| 80 | basic_nightstand | Basic Nightstand |
| 82 | basic_oil_lamp | Basic Oil Lamp |
| 83 | basic_pouch | Bag Upgrade |
| 85 | basic_table | Basic Table |
| 87 | basic_wall_window | Basic Window |
| 89 | basic_wallpaper | Basic Wallpaper |
| 90 | basic_wood | Wood |
| 105 | basic_wood_chest | Basic Wood Chest |
| 106 | basil | Basil |
| 107 | basket | Basket |
| 108 | bath_soap | Bath Soap |
| 110 | bathroom_bench | Bathroom Bench |
| 117 | bathroom_curtain_stand | Bathroom Curtain Stand |
| 119 | bathroom_floor_mirror | Bathroom Floor Mirror |
| 121 | bathroom_sink | Bathroom Sink |
| 125 | bathroom_tile_wall | Bathroom Tile Wall |
| 127 | bathroom_wall_mirror | Bathroom Wall Mirror |
| 128 | bathroom_wall_sconce | Bathroom Wall Sconce |
| 135 | bathroom_wall_towel | Bathroom Wall Towel |
| 137 | bathtub | Bathtub |
| 138 | beach_hopper | Beach Hopper |
| 139 | beer | Beer |
| 140 | beer_mug | Beer Mug |
| 141 | beet | Beet |
| 142 | beet_salad | Beet Salad |
| 143 | beet_soup | Beet Soup |
| 144 | beginner_kitchen | Level 1 Kitchen |
| 145 | bell_berry | Bell Berry |
| 146 | bell_berry_bakewell_tart | Bell Berry Bakewell Tart |
| 147 | berries_and_cream | Berries and Cream |
| 148 | berry_bowl | Berry Bowl |
| 149 | big_bell | Big Bell |
| 150 | big_cookie | Big Cookie |
| 151 | black_tablet | Black Tablet |
| 152 | blackberry | Blackberry |
| 153 | blackberry_jam | Telepop (Purple) |
| 154 | blue_capybara_wall_ribbon | Blue Capybara Wall Ribbon |
| 155 | blue_chicken_wall_ribbon | Blue Chicken Wall Ribbon |
| 156 | blue_conch_shell | Blue Conch Shell |
| 157 | blue_duck_wall_ribbon | Blue Duck Wall Ribbon |
| 158 | blue_pet_bed | Blue Pet Bed |
| 159 | blue_pet_dish | Blue Pet Dish |
| 160 | blue_rabbit_wall_ribbon | Blue Rabbit Wall Ribbon |
| 161 | blueberry | Blueberry |
| 162 | blueberry_jam | Telepop (Blue) |
| 163 | bluefish | Bluefish |
| 164 | bluegill | Blue Gill |
| 165 | bomb | Clod Bomb |
| 166 | bonito | Bonito |
| 167 | bowfish | Bowfish |
| 168 | box_medium | Medium Box |
| 170 | box_small | Small Box |
| 171 | braised_burdock | Braised Burdock |
| 172 | braised_carrots | Braised Carrots |
| 173 | bread | Bread |
| 174 | breaded_catfish | Breaded Catfish |
| 175 | bream | Bream |
| 176 | breath_of_fire | Breath of Flame |
| 177 | breath_of_spring | Breath of Spring |
| 178 | brightbulb_moth | Brightbulb Moth |
| 179 | bristle | Bristle |
| 180 | broccoli | Broccoli |
| 181 | broccoli_salad | Broccoli Salad |
| 182 | bronze_alpaca_trophy | Bronze Alpaca Trophy |
| 183 | bronze_cow_trophy | Bronze Cow Trophy |
| 184 | bronze_horse_trophy | Bronze Horse Trophy |
| 185 | bronze_sheep_trophy | Bronze Sheep Trophy |
| 186 | brown_bullhead | Brown Bullhead |
| 187 | brown_trout | Brown Trout |
| 188 | bucket_brew | Bucket Brew |
| 189 | bull_horn | Bull Horn |
| 190 | bullfrog | Bullfrog |
| 191 | bumblebee | Bumblebee |
| 192 | bumblebee_plushie | Bumblebee Plushie |
| 193 | burbot | Burbot |
| 194 | burdock_root | Burdock Root |
| 195 | butter | Butter |
| 196 | buttered_peas | Buttered Peas |
| 197 | butterfish | Butterfish |
| 198 | butterfly | Butterfly |
| 201 | butterfly_bed | Butterfly Bed |
| 204 | butterfly_double_bed | Butterfly Double Bed |
| 207 | butterfly_wall_lamp | Butterfly Wall Lamp |
| 208 | cabbage | Cabbage |
| 209 | cabbage_slaw | Cabbage Slaw |
| 215 | cabin_bed | Rustic Cabin Bed |
| 213 | cabin_bed_double | Rustic Cabin Double Bed |
| 218 | cabin_bookshelf | Rustic Cabin Bookshelf |
| 221 | cabin_chair | Rustic Cabin Chair |
| 224 | cabin_dresser | Rustic Cabin Dresser |
| 227 | cabin_firewood | Rustic Cabin Firewood |
| 230 | cabin_flooring | Rustic Cabin Flooring |
| 233 | cabin_lamp | Rustic Cabin Lamp |
| 236 | cabin_nightstand | Rustic Cabin Nightstand |
| 238 | cabin_stove | Rustic Cabin Stove |
| 241 | cabin_table | Rustic Cabin Table |
| 244 | cabin_wall_shelf | Rustic Cabin Wall Shelf |
| 247 | cabin_wall_window | Rustic Cabin Window |
| 250 | cabin_wallpaper | Rustic Cabin Wallpaper |
| 251 | caldosian_breastplate | Caldosian Breastplate |
| 252 | caldosian_chocolate_cake | Chocolate Cake |
| 253 | caldosian_drinking_horn | Caldosian Drinking Horn |
| 254 | caldosian_emperor_bust | Caldosian Emperor Bust |
| 255 | caldosian_sword | Caldosian Sword |
| 256 | campfire | Campfire |
| 257 | candelabra_seadragon | Candelabra Seadragon |
| 258 | candied_lemon_peel | Candied Lemon Peel |
| 259 | candied_queen_berries | Candied Queen Berries |
| 260 | candied_strawberries | Candied Strawberries |
| 261 | candied_walnuts | Candied Walnuts |
| 262 | candle | Candle |
| 263 | candle_chamberstick | Candle Chamberstick |
| 264 | canned_sardines | Canned Sardines |
| 265 | capybara_plushie | Capybara Plushie |
| 266 | caramel_candy | Caramel Candy |
| 267 | caramelized_moon_fruit | Caramelized Moon Fruit |
| 268 | carp | Carp |
| 269 | carrot | Carrot |
| 270 | cat_treat | Cat Treat |
| 271 | caterpillar | Caterpillar |
| 274 | caterpillar_chair | Caterpillar Chair |
| 275 | catfish | Catfish |
| 276 | catmint | Catmint |
| 277 | cattail | Cattail Fluff |
| 278 | cauliflower | Cauliflower |
| 279 | cauliflower_curry | Cauliflower Curry |
| 280 | cave_eel | Cave Eel |
| 281 | cave_kelp | Jade Dulse |
| 282 | cave_shark | Cave Shark |
| 283 | cave_shrimp | Cave Shrimp |
| 286 | cavern_crystal_lamp | Glowing Crystal Lamp |
| 287 | cavern_emerald_rock | Ornamental Emerald Rock |
| 290 | cavern_floor_brazier | Floor Brazier |
| 291 | cavern_hewn_rock_bookshelf | Hewn Rock Bookshelf |
| 292 | cavern_hewn_rock_table | Hewn Rock Table |
| 294 | cavern_mossy_rug | Mossy Rug |
| 297 | cavern_mushroom_stool | Mushroom Stool |
| 298 | cavern_rock_flooring | Rock Floor |
| 299 | cavern_rock_wallpaper | Rock Wall |
| 300 | cavern_silver_node | Ornamental Silver Node |
| 301 | cavern_stone_arch | Stalactite Arch |
| 304 | cavern_wall_brazier | Wall Brazier |
| 305 | celosia | Celosia |
| 309 | champions_kitchen | Champion's Kitchen |
| 310 | char | Char |
| 311 | cheese | Cheese |
| 312 | cherry | Cherry |
| 313 | cherry_cobbler | Cherry Cobbler |
| 314 | cherry_fish | Cherry Fish |
| 315 | cherry_smoothie | Cherry Smoothie |
| 316 | cherry_tart | Cherry Tart |
| 320 | chess_bishop | Chess Bishop Piece |
| 324 | chess_king | Chess King Piece |
| 328 | chess_knight | Chess Knight Piece |
| 332 | chess_pawn | Chess Pawn Piece |
| 336 | chess_queen | Chess Queen Piece |
| 340 | chess_rook | Chess Rook Piece |
| 341 | chess_scroll_bundle | Chess Piece Scroll Bundle |
| 345 | chess_table | Chess Table |
| 346 | chestnut | Chestnut |
| 347 | chicken_plushie | Chicken Plushie |
| 348 | chickpea | Chickpea |
| 349 | chickpea_curry | Chickpea Curry |
| 350 | chicky_hot_chocolate | Chicky Hot Chocolate |
| 351 | chili_coconut_curry | Chili Coconut Curry |
| 352 | chili_pepper | Chili Pepper |
| 353 | chillipede | Chillipede |
| 354 | chocolate | Chocolate |
| 355 | chrysanthemum | Chrysanthemum |
| 356 | chub | Chub |
| 357 | chum | Chum |
| 358 | cicada | Cicada |
| 359 | cicada_nymph | Cicada Nymph |
| 360 | clam | Clam |
| 361 | clam_chowder | Clam Chowder |
| 362 | clay | Clay |
| 363 | clay_key | Clay Key |
| 364 | cloth_chestpiece | Cloth Chestpiece |
| 365 | cloth_helmet | Cloth Helmet |
| 366 | cloth_pants | Cloth Pants |
| 367 | cloth_shoes | Cloth Shoes |
| 368 | cloth_wristband | Cloth Wristband |
| 369 | coconut | Coconut |
| 370 | coconut_cream_pie | Coconut Cream Pie |
| 371 | coconut_milk | Coconut Milk |
| 372 | cod | Cod |
| 373 | cod_with_thyme | Cod with Thyme |
| 374 | coffee | Coffee |
| 377 | coffee_mug | Coffee Mug |
| 378 | coin_lump | Coin Lump |
| 420 | color_paving_stone | Color Paving Stone |
| 421 | completely_wrong_map | Completely Wrong Map |
| 422 | confiscated_coffee | Confiscated Coffee |
| 423 | construction_fence | Construction Fence |
| 426 | cooking_frying_pan | Frying Pan |
| 428 | cooking_hanging_pans | Hanging Pans |
| 431 | cooking_stockpot | Stockpot |
| 434 | cooking_tea_kettle | Tea Kettle |
| 435 | cooktop_beetle | Cooktop Beetle |
| 436 | copper_armor | Copper Armor |
| 437 | copper_beetle | Copper Nugget Beetle |
| 438 | copper_greaves | Copper Greaves |
| 439 | copper_helmet | Copper Helmet |
| 440 | copper_ingot | Copper Ingot |
| 441 | copper_legplates | Copper Legplates |
| 442 | copper_ring | Copper Ring |
| 443 | coral | Coral |
| 445 | coral_chair | Coral Chair |
| 447 | coral_lamp | Coral Lamp |
| 448 | coral_mantis | Coral Mantis |
| 449 | coral_stone_archway | Coral-Stone Arch |
| 451 | coral_storage_chest | Coral Storage Chest |
| 453 | coral_table | Coral Table |
| 454 | corn | Corn |
| 455 | cornucopia | Cornucopia |
| 456 | cosmetic | MISSING | **DISABLED** |
| 457 | cosmos | Cosmos |
| 461 | cottage_bed | Lovely Cottage Single Bed |
| 460 | cottage_bed_double | Lovely Cottage Double Bed |
| 463 | cottage_chair | Lovely Cottage Chair |
| 465 | cottage_dresser | Lovely Cottage Dresser |
| 467 | cottage_flooring | Lovely Cottage Flooring |
| 469 | cottage_fridge | Lovely Cottage Icebox |
| 471 | cottage_garden_arch | Cottage Garden Arch |
| 474 | cottage_garden_planter | Cottage Garden Planter |
| 476 | cottage_garden_shed | Cottage Garden Shed |
| 479 | cottage_garden_trellis | Cottage Garden Trellis |
| 481 | cottage_garden_workbench | Cottage Garden Workbench |
| 483 | cottage_nightstand | Lovely Cottage Nightstand |
| 485 | cottage_plant_shelf | Lovely Cottage Plant Shelf |
| 487 | cottage_potted_fig | Lovely Cottage Potted Fig |
| 489 | cottage_potted_flowers | Lovely Cottage Potted Flowers |
| 491 | cottage_rug | Lovely Cottage Rug |
| 493 | cottage_table | Lovely Cottage Table |
| 495 | cottage_wall_flowers | Lovely Cottage Wall Flowers |
| 497 | cottage_wall_window | Lovely Cottage Window |
| 499 | cottage_wallpaper | Lovely Cottage Wallpaper |
| 500 | counter_basic_v1 | Basic Counter |
| 501 | counter_basic_v2 | Basic Counter |
| 502 | counter_cabin_cherry | Cabin Counter |
| 503 | counter_cabin_oak | Cabin Counter |
| 504 | counter_cabin_walnut | Cabin Counter |
| 505 | counter_cake_chocolate | Cake Counter |
| 506 | counter_cake_double_chocolate | Cake Counter |
| 507 | counter_cake_strawberry | Cake Counter |
| 508 | counter_cottage_v1 | Lovely Cottage Counter |
| 509 | counter_cottage_v2 | Lovely Cottage Counter |
| 513 | counter_kitchen | Champion's Kitchen Counter |
| 514 | cow_donut | Cow-Shaped Donut |
| 515 | cow_milk | Milk |
| 516 | cow_plushie | Cow Plushie |
| 517 | cozy_kitchen | Level 2 Kitchen |
| 518 | crab | Crab |
| 519 | crab_cakes | Crab Cakes |
| 520 | crab_statue | Crab Statue |
| 526 | craft_fabric_rack | Craft Fabric Rack |
| 532 | craft_rug | Craft Crochet Rug |
| 529 | craft_sewing_tin | Craft Sewing Tin |
| 535 | craft_yarn_basket | Craft Yarn Basket |
| 536 | crafting_scroll | Crafting Scroll: {} | **DISABLED** |
| 537 | cranberry | Cranberry |
| 538 | cranberry_juice | Cranberry Juice |
| 539 | cranberry_orange_scone | Cranberry Orange Scone |
| 540 | crayfish | Crayfish |
| 541 | crayfish_etouffee | Crayfish Etouffee |
| 542 | cream_pet_bed | Cream Pet Bed |
| 543 | cream_pet_dish | Cream Pet Dish |
| 544 | cricket | Cricket |
| 545 | criminal_confession | Criminal Confession |
| 546 | crispy_fried_earthshroom | Crispy Fried Earthshroom |
| 547 | crocus | Crocus |
| 548 | crop_faux_ash_mushroom | Faux Ash Mushroom |
| 549 | crop_faux_basil | Faux Basil |
| 550 | crop_faux_beet | Faux Beet |
| 551 | crop_faux_blue_conch_shell | Faux Blue Conch Shell |
| 552 | crop_faux_breath_of_fire | Faux Breath of Flame |
| 553 | crop_faux_broccoli | Faux Broccoli |
| 554 | crop_faux_burdock_root | Faux Burdock |
| 555 | crop_faux_cabbage | Faux Cabbage |
| 556 | crop_faux_carrot | Faux Carrot |
| 557 | crop_faux_catmint | Faux Catmint |
| 558 | crop_faux_cauliflower | Faux Cauliflower |
| 559 | crop_faux_cave_kelp | Faux Jade Dulse |
| 560 | crop_faux_celosia | Faux Celosia |
| 561 | crop_faux_chestnut | Faux Chestnut |
| 562 | crop_faux_chickpea | Faux Chickpea |
| 563 | crop_faux_chili_pepper | Faux Chili Pepper |
| 564 | crop_faux_chrysanthemum | Faux Chrysanthemum |
| 565 | crop_faux_corn | Faux Corn |
| 566 | crop_faux_cosmos | Faux Cosmos |
| 567 | crop_faux_cranberry | Faux Cranberry |
| 568 | crop_faux_crocus | Faux Crocus |
| 569 | crop_faux_crystal_berries | Faux Crystal Berries |
| 570 | crop_faux_crystal_rose | Faux Crystal Rose |
| 571 | crop_faux_cucumber | Faux Cucumber |
| 572 | crop_faux_daffodil | Faux Daffodil |
| 573 | crop_faux_daikon_radish | Faux Daikon Radish |
| 574 | crop_faux_daisy | Faux Daisy |
| 575 | crop_faux_dandelion | Faux Dandelion |
| 576 | crop_faux_dill | Faux Dill |
| 577 | crop_faux_earthshroom | Faux Earthshroom |
| 578 | crop_faux_fennel | Faux Fennel |
| 579 | crop_faux_fiddlehead | Faux Fiddlehead |
| 580 | crop_faux_flame_pepper | Faux Flame Pepper |
| 581 | crop_faux_fog_orchid | Faux Fog Orchid |
| 582 | crop_faux_frost_lily | Faux Frost Lily |
| 583 | crop_faux_garlic | Faux Garlic |
| 584 | crop_faux_heather | Faux Heather |
| 585 | crop_faux_holly | Faux Holly |
| 586 | crop_faux_horseradish | Faux Horseradish |
| 587 | crop_faux_hot_potato | Faux Hot Potato |
| 588 | crop_faux_ice_block | Faux Ice Block |
| 589 | crop_faux_iris | Faux Iris |
| 590 | crop_faux_jasmine | Faux Jasmine |
| 591 | crop_faux_lava_chestnuts | Faux Lava Chestnuts |
| 592 | crop_faux_lilac | Faux Lilac |
| 593 | crop_faux_marigold | Faux Marigold |
| 594 | crop_faux_middlemist | Faux Middlemist |
| 595 | crop_faux_mines_mussels | Faux Mines Mussels |
| 596 | crop_faux_moon_fruit | Faux Moon Fruit |
| 597 | crop_faux_morel_mushroom | Faux Morel Mushroom |
| 598 | crop_faux_mystery_bag | Faux Magic Seed Plant |
| 599 | crop_faux_narrows_moss | Faux Narrows Moss |
| 600 | crop_faux_nettle | Faux Nettle |
| 601 | crop_faux_night_queen | Faux Night Queen |
| 602 | crop_faux_onion | Faux Onion |
| 603 | crop_faux_oregano | Faux Oregano |
| 604 | crop_faux_oyster_mushroom | Faux Oyster Mushroom |
| 605 | crop_faux_peas | Faux Peas |
| 606 | crop_faux_pineshroom | Faux Pineshroom |
| 607 | crop_faux_pink_scallop_shell | Faux Pink Scallop Shell |
| 608 | crop_faux_poinsettia | Faux Poinsettia |
| 609 | crop_faux_potato | Faux Potato |
| 610 | crop_faux_pumpkin | Faux Pumpkin |
| 611 | crop_faux_rice | Faux Rice |
| 612 | crop_faux_rockroot | Faux Rockroot |
| 613 | crop_faux_rosemary | Faux Rosemary |
| 614 | crop_faux_sage | Faux Sage |
| 615 | crop_faux_sand_dollar | Faux Sand Dollar |
| 616 | crop_faux_sea_grapes | Faux Sea Grapes |
| 617 | crop_faux_sesame | Faux Sesame |
| 618 | crop_faux_shadow_flower | Faux Shadow Flower |
| 619 | crop_faux_shale_grass | Faux Shale Grass |
| 620 | crop_faux_snapdragon | Faux Snapdragon |
| 621 | crop_faux_snow_peas | Faux Snow Peas |
| 622 | crop_faux_snowdrop_anemone | Faux Snowdrop Anemone |
| 623 | crop_faux_spirit_mushroom | Faux Spirit Mushroom |
| 624 | crop_faux_spirula_shell | Faux Spirula Shell |
| 625 | crop_faux_strawberry | Faux Strawberry |
| 626 | crop_faux_sugar_cane | Faux Sugar Cane |
| 627 | crop_faux_sunflower | Faux Sunflower |
| 628 | crop_faux_sweet_potato | Faux Sweet Potato |
| 629 | crop_faux_sweetroot | Faux Sweetroot |
| 630 | crop_faux_tea | Faux Tea |
| 631 | crop_faux_temple_flower | Faux Temple Flower |
| 632 | crop_faux_thorn_vine | Faux Thorn Vine |
| 633 | crop_faux_thyme | Faux Thyme |
| 634 | crop_faux_tide_lettuce | Faux Tide Lettuce |
| 635 | crop_faux_tomato | Faux Tomato |
| 636 | crop_faux_tulip | Faux Tulip |
| 637 | crop_faux_turnip | Faux Turnip |
| 638 | crop_faux_upper_mines_mushroom | Faux Upper Mines Mushroom |
| 639 | crop_faux_viola | Faux Viola |
| 640 | crop_faux_walnut | Faux Walnut |
| 641 | crop_faux_watermelon | Faux Watermelon |
| 642 | crop_faux_wheat | Faux Wheat |
| 643 | crop_faux_wild_leek | Faux Wild Leek |
| 644 | crop_sign_apple | Apple Sign |
| 645 | crop_sign_basil | Basil Sign |
| 646 | crop_sign_beet | Beet Sign |
| 647 | crop_sign_broccoli | Broccoli Sign |
| 648 | crop_sign_burdock_root | Burdock Root Sign |
| 649 | crop_sign_cabbage | Cabbage Sign |
| 650 | crop_sign_carrot | Carrot Sign |
| 651 | crop_sign_catmint | Catmint Sign |
| 652 | crop_sign_cauliflower | Cauliflower Sign |
| 653 | crop_sign_celosia | Celosia Sign |
| 654 | crop_sign_cherry | Cherry Sign |
| 655 | crop_sign_chickpea | Chickpea Sign |
| 656 | crop_sign_chili_pepper | Chili Pepper Sign |
| 657 | crop_sign_chrysanthemum | Chrysanthemum Sign |
| 658 | crop_sign_coconut | Coconut Sign |
| 659 | crop_sign_corn | Corn Sign |
| 660 | crop_sign_cosmos | Cosmos Sign |
| 661 | crop_sign_cranberry | Cranberry Sign |
| 662 | crop_sign_cucumber | Cucumber Sign |
| 663 | crop_sign_daffodil | Daffodil Sign |
| 664 | crop_sign_daikon_radish | Daikon Radish Sign |
| 665 | crop_sign_daisy | Daisy Sign |
| 666 | crop_sign_dill | Dill Sign |
| 667 | crop_sign_frost_lily | Frost Lily Sign |
| 668 | crop_sign_garlic | Garlic Sign |
| 669 | crop_sign_heather | Heather Sign |
| 670 | crop_sign_iris | Iris Sign |
| 671 | crop_sign_jasmine | Jasmine Sign |
| 672 | crop_sign_lemon | Lemon Sign |
| 673 | crop_sign_lilac | Lilac Sign |
| 674 | crop_sign_marigold | Marigold Sign |
| 675 | crop_sign_moon_fruit | Moon Fruit Sign |
| 676 | crop_sign_mystery_bag | Magic Seed Plant Sign |
| 677 | crop_sign_night_queen | Night queen Sign |
| 678 | crop_sign_onion | Onion Sign |
| 679 | crop_sign_orange | Orange Sign |
| 680 | crop_sign_oregano | Oregano Sign |
| 681 | crop_sign_peach | Peach Sign |
| 682 | crop_sign_pear | Pear Sign |
| 683 | crop_sign_peas | Peas Sign |
| 684 | crop_sign_poinsettia | Poinsettia Sign |
| 685 | crop_sign_pomegranate | Pomegranate Sign |
| 686 | crop_sign_potato | Potato Sign |
| 687 | crop_sign_pumpkin | Pumpkin Sign |
| 688 | crop_sign_rice | Rice Sign |
| 689 | crop_sign_rosemary | Rosemary Sign |
| 690 | crop_sign_sage | Sage Sign |
| 691 | crop_sign_snapdragon | Snapdragon Sign |
| 692 | crop_sign_snow_peas | Snow Peas Sign |
| 693 | crop_sign_snowdrop_anemone | Snowdrop Anemone Sign |
| 694 | crop_sign_strawberry | Strawberry Sign |
| 695 | crop_sign_sugar_cane | Sugar Cane Sign |
| 696 | crop_sign_sunflower | Sunflower Sign |
| 697 | crop_sign_sweet_potato | Sweet Potato Sign |
| 698 | crop_sign_tea | Tea Sign |
| 699 | crop_sign_temple_flower | Temple Flower Sign |
| 700 | crop_sign_thyme | Thyme Sign |
| 701 | crop_sign_tomato | Tomato Sign |
| 702 | crop_sign_tulip | Tulip Sign |
| 703 | crop_sign_turnip | Turnip Sign |
| 704 | crop_sign_viola | Viola Sign |
| 705 | crop_sign_watermelon | Watermelon Sign |
| 706 | crop_sign_wheat | Wheat Sign |
| 707 | crucian_carp | Crucian Carp |
| 708 | crunchy_chickpeas | Crunchy Chickpeas |
| 709 | crystal | Crystal |
| 710 | crystal_apple | Crystal Apple |
| 711 | crystal_berries | Crystal Berries |
| 712 | crystal_berry_pie | Crystal Berry Pie |
| 713 | crystal_boots | Crystal Greaves |
| 714 | crystal_caterpillar | Crystal Caterpillar |
| 715 | crystal_chestpiece | Crystal Chestpiece |
| 716 | crystal_helmet | Crystal Helmet |
| 717 | crystal_ring | Crystal Ring |
| 718 | crystal_rose | Crystal Rose |
| 721 | crystal_string_lights | Crystal Wall String Lights |
| 722 | crystal_tassets | Crystal Tassets |
| 723 | crystal_wing_moth | Crystal Wing Moth |
| 724 | crystalline_cricket | Crystalline Cricket |
| 725 | cucumber | Cucumber |
| 726 | cucumber_salad | Cucumber Salad |
| 727 | cucumber_sandwich | Cucumber Sandwich |
| 728 | cup_of_tea | Tea with Lemon |
| 729 | curry_powder | Curry Powder |
| 730 | dace | Dace |
| 731 | daffodil | Daffodil |
| 732 | daikon_radish | Daikon Radish |
| 733 | daisy | Daisy |
| 734 | dandelion | Dandelion |
| 735 | dart | Dart |
| 736 | date_photo_bath | Bath Date Photo |
| 737 | date_photo_beach | Beach Date Photo |
| 738 | date_photo_deep_woods | Picnic Date Photo |
| 739 | date_photo_gem_cutting | Gem Cutting Date Photo |
| 740 | date_photo_inn | Inn Date Photo |
| 741 | date_photo_park | Park Date Photo |
| 742 | date_photo_shooting_star | Shooting Star Date Photo |
| 743 | deep_earthworm | Deep Earthworm |
| 744 | deep_sea_soup | Deep Sea Soup |
| 745 | deluxe_curry | Deluxe Curry |
| 746 | deluxe_hay | Deluxe Large Animal Feed |
| 747 | deluxe_icebox_blue | Deluxe Icebox |
| 748 | deluxe_icebox_green | Deluxe Icebox |
| 749 | deluxe_icebox_pink | Deluxe Icebox |
| 750 | deluxe_icebox_white | Deluxe Icebox |
| 751 | deluxe_icebox_yellow | Deluxe Icebox |
| 752 | deluxe_small_animal_feed | Deluxe Small Animal Feed |
| 753 | deluxe_storage_chest_aqua | Deluxe Storage Chest |
| 754 | deluxe_storage_chest_black | Deluxe Storage Chest |
| 755 | deluxe_storage_chest_blue | Deluxe Storage Chest |
| 756 | deluxe_storage_chest_dark_brown | Deluxe Storage Chest |
| 757 | deluxe_storage_chest_gold | Deluxe Storage Chest |
| 758 | deluxe_storage_chest_gray | Deluxe Storage Chest |
| 759 | deluxe_storage_chest_green | Deluxe Storage Chest |
| 760 | deluxe_storage_chest_light_brown | Deluxe Storage Chest |
| 761 | deluxe_storage_chest_orange | Deluxe Storage Chest |
| 762 | deluxe_storage_chest_pink | Deluxe Storage Chest |
| 763 | deluxe_storage_chest_purple | Deluxe Storage Chest |
| 764 | deluxe_storage_chest_red | Deluxe Storage Chest |
| 765 | deluxe_storage_chest_white | Deluxe Storage Chest |
| 766 | dense_water | Dense Water |
| 767 | deviled_eggs | Deviled Eggs |
| 768 | diamond_backed_mirror | Diamond-Backed Mirror |
| 769 | diamond_beetle | Diamond Beetle |
| 770 | dill | Dill |
| 771 | dirt_clear | Dirt Tile Clear |
| 772 | dirt_dot_one | Dirt Tile 1 Dot |
| 773 | dirt_dot_three | Dirt Tile 3 Dots |
| 774 | dirt_dot_two | Dirt Tile 2 Dots |
| 775 | dog_treat | Dog Treat |
| 776 | dollhouse_bench | Dollhouse Bench |
| 777 | dough_lad_doll | Doughlad Doll |
| 781 | dragon_altar | Dragon Claw Altar |
| 782 | dragon_claw | Dragon Claw |
| 783 | dragon_forged_bracelet | Dragon-Forged Bracelet |
| 784 | dragon_horn_beetle | Dragon Horn Beetle |
| 785 | dragon_horn_mushroom | Dragon Horn Mushroom |
| 786 | dragon_horn_mushroom_with_thyme | Dragon Horn Mushroom with Thyme |
| 787 | dragon_pact_tablet | Dragon Pact Tablet |
| 790 | dragon_priestess_fountain | Dragon Priestess Fountain |
| 791 | dragon_scale | Dragon Scale |
| 793 | dragon_statue_replica | Dragon Statue |
| 794 | dragonfly | Dragonfly |
| 795 | dragonsworn_chest_and_pants | Dragonsworn Chest and Pants |
| 796 | dragonsworn_greaves | Dragonsworn Greaves |
| 797 | dragonsworn_helmet | Dragonsworn Helmet |
| 798 | dried_squid | Dried Squid |
| 799 | duck_egg | Duck Egg |
| 800 | duck_feather | Duck Feather |
| 801 | duck_mayonnaise | Duck Mayonnaise |
| 802 | duck_plushie | Duck Plushie |
| 803 | dungeon_fountain_health | Fountain Health 33 | **GLITCHED** |
| 804 | dungeon_fountain_stamina | Fountain Stamina 33 | **GLITCHED** |
| 805 | earth_eel | Earth Eel |
| 806 | earth_infused_stone | Earth-Infused Stone |
| 807 | earth_tablet_rubbing | Earth Tablet Rubbing |
| 808 | earthshroom | Earthshroom |
| 809 | egg | Chicken Egg |
| 815 | emerald_bed | Emerald Double Bed |
| 813 | emerald_chair | Emerald Chair |
| 816 | emerald_horned_charger | Emerald-Horned Charger |
| 818 | emerald_nightstand | Emerald Nightstand |
| 820 | emerald_rug | Emerald Rug |
| 822 | emerald_wallpaper | Emerald Wallpaper |
| 823 | espresso | Espresso |
| 826 | espresso_cup | Espresso Cup |
| 827 | espresso_machine | Espresso Machine |
| 828 | essence_blossom | Essence Blossom |
| 829 | essence_drop | Essence Drop |
| 830 | essence_stone_giant | Giant Essence Stone |
| 831 | essence_stone_large | Large Essence Stone |
| 832 | essence_stone_medium | Medium Essence Stone |
| 833 | essence_stone_small | Small Essence Stone |
| 834 | essence_stone_tiny | Tiny Essence Stone |
| 837 | explorer_bed | Explorer Bed |
| 840 | explorer_chair | Explorer Chair |
| 843 | explorer_double_bed | Explorer Double Bed |
| 846 | explorer_flooring | Explorer Flooring |
| 849 | explorer_globe | Explorer Globe |
| 852 | explorer_lamp | Explorer Lamp |
| 855 | explorer_rug | Explorer Rug |
| 858 | explorer_trunk_table | Explorer Trunk Table |
| 861 | explorer_wallpaper | Explorer Wallpaper |
| 862 | extra_large_decorative_pumpkin_black | Extra Large Decorative Pumpkin |
| 863 | extra_large_decorative_pumpkin_green | Extra Large Decorative Pumpkin |
| 864 | extra_large_decorative_pumpkin_orange | Extra Large Decorative Pumpkin |
| 865 | extra_large_decorative_pumpkin_purple | Extra Large Decorative Pumpkin |
| 866 | extra_large_decorative_pumpkin_white | Extra Large Decorative Pumpkin |
| 867 | extra_large_decorative_pumpkin_yellow | Extra Large Decorative Pumpkin |
| 868 | faceted_rock_gem | Faceted Rock Gem |
| 869 | fairy_bee | Fairy Bee |
| 870 | fairy_syrup | Fairy Syrup |
| 871 | fake_essence | Essence |
| 876 | fall_crop_sign_scroll_bundle | Fall Crop Sign Scroll Bundle |
| 880 | fall_double_bed | Fall Double Bed |
| 875 | fall_single_bed | Fall Single Bed |
| 884 | fall_table | Fall Dining Table |
| 885 | family_crest_pendant | Family Crest Pendant |
| 886 | farm_bridge | Starter Farm Bridge |
| 887 | farm_house_calendar | Farmhouse Calendar |
| 888 | fast_food | Fast Food |
| 889 | feather | Rooster Feather |
| 890 | fennel | Fennel |
| 891 | fiber | Fiber |
| 892 | fiddlehead | Fiddlehead |
| 893 | field_clear | Grass Tile Clear |
| 894 | field_flower | Grass Tile Flower |
| 895 | field_flower_two | Grass Tile 2 Flowers |
| 896 | field_grass | Grass Tile Blades |
| 897 | field_grass_two | Field Grass 2 Blades |
| 898 | fire_crystal | Fire Crystal |
| 899 | fire_snake_oil | Fire Snake Oil |
| 900 | fire_tablet_rubbing | Fire Tablet Rubbing |
| 901 | fire_wasp | Fire Wasp |
| 902 | firefly | Firefly |
| 907 | firefly_wall_string_lights | Firefly Wall String Lights |
| 908 | firesail_fish | Firesail Fish |
| 909 | fish_plush_toy | Fish Plush Toy |
| 910 | fish_skewer | Fish Skewers |
| 911 | fish_stew | Fish Stew |
| 912 | fish_tacos | Fish Tacos |
| 914 | fishing_bed | Fishing Bed |
| 916 | fishing_chair | Fishing Chair |
| 918 | fishing_double_bed | Fishing Double Bed |
| 920 | fishing_flooring | Fishing Flooring |
| 922 | fishing_lamp | Lighthouse Lamp |
| 924 | fishing_oar_wall_decor | Oar Wall Decor |
| 927 | fishing_rod_copper | Copper Fishing Rod |
| 928 | fishing_rod_gold | Gold Fishing Rod |
| 929 | fishing_rod_iron | Iron Fishing Rod |
| 930 | fishing_rod_mistril | Mistril Fishing Rod |
| 931 | fishing_rod_silver | Silver Fishing Rod |
| 932 | fishing_rod_worn | Worn Fishing Rod |
| 934 | fishing_rug | Fishing Rug |
| 936 | fishing_table | Fishing Table |
| 938 | fishing_tank | Fish Tank |
| 940 | fishing_trophy_shelf | Fishing Trophy Wall Shelf |
| 926 | fishing_wall_plaque | Faux Fish Wall Plaque |
| 942 | fishing_wallpaper | Fishing Wallpaper |
| 944 | fishing_window | Porthole Window |
| 945 | flame_pepper | Flame Pepper |
| 946 | flathead_catfish | Flathead Catfish |
| 947 | flint_arrowhead | Flint Arrowhead |
| 948 | floral_tea | Floral Tea |
| 949 | flour | Flour |
| 950 | flower_crown_beetle | Flower Crown Beetle |
| 951 | fog_orchid | Fog Orchid |
| 952 | forest_perch | Forest Perch |
| 953 | fossilized_egg | Fossilized Egg |
| 954 | fossilized_mandrake_root | Fossilized Mandrake Root |
| 955 | freshwater_eel | Freshwater Eel |
| 956 | freshwater_oyster | Freshwater Oyster |
| 957 | fried_rice | Fried Rice |
| 958 | frog | Frog |
| 959 | frost_flutter_butterfly | Frost Flutter Butterfly |
| 960 | frost_lily | Frost Lily |
| 961 | fuzzy_moth | Fuzzy Moth |
| 962 | gar | Gar |
| 963 | garlic | Garlic |
| 964 | garlic_bread | Garlic Bread |
| 965 | gathering_basket | Gathering Basket |
| 966 | gazpacho | Gazpacho |
| 967 | gem_shard_caterpillar | Gem Shard Caterpillar |
| 968 | giant_fish_scale | Giant Fish Scale |
| 969 | giant_koi | Giant Koi |
| 970 | giant_tilapia | Giant Tilapia |
| 971 | glass | Glass |
| 972 | glass_absinthe | Absinthe Glass |
| 973 | glass_cocktail | Cocktail Glass |
| 974 | glass_lemonade | Lemonade Glass |
| 975 | glass_milk | Milk Glass |
| 976 | glass_water | Water Glass |
| 977 | glass_whisky | Whisky Glass |
| 978 | glowberry | Glowberry |
| 979 | glowberry_cookies | Glowberry Cookies |
| 980 | glowing_mushroom | Glowing Mushroom |
| 981 | goby | Goby |
| 982 | gold_alpaca_trophy | Gold Alpaca Trophy |
| 983 | gold_armor | Gold Armor |
| 984 | gold_cow_trophy | Gold Cow Trophy |
| 985 | gold_floor_mirror | Gold Floor Mirror |
| 986 | gold_greaves | Gold Greaves |
| 987 | gold_helmet | Gold Helmet |
| 988 | gold_horse_trophy | Gold Horse Trophy |
| 989 | gold_ingot | Gold Ingot |
| 990 | gold_legplates | Gold Legplates |
| 991 | gold_ring | Gold Ring |
| 992 | gold_sheep_trophy | Gold Sheep Trophy |
| 993 | gold_wall_mirror | Gold Wall Mirror |
| 994 | golden_alpaca_wool | Golden Alpaca Wool |
| 995 | golden_bristle | Golden Bristle |
| 996 | golden_bull_horn | Golden Bull Horn |
| 997 | golden_butter | Golden Butter |
| 998 | golden_cheese | Golden Cheese |
| 999 | golden_cheesecake | Golden Cheesecake |
| 1000 | golden_cookies | Golden Cookies |
| 1001 | golden_cow_milk | Golden Milk |
| 1002 | golden_duck_egg | Golden Duck Egg |
| 1003 | golden_duck_feather | Golden Duck Feather |
| 1004 | golden_duck_mayonnaise | Golden Duck Mayonnaise |
| 1005 | golden_egg | Golden Egg |
| 1006 | golden_feather | Golden Rooster Feather |
| 1007 | golden_horse_hair | Golden Horse Hair |
| 1008 | golden_mayonnaise | Golden Mayonnaise |
| 1009 | golden_rabbit_wool | Golden Rabbit Wool |
| 1010 | golden_sheep_wool | Golden Sheep Wool |
| 1011 | golden_shiner | Golden Shiner |
| 1012 | goldfish | Goldfish |
| 1013 | grape_juice | Grape Juice |
| 1014 | grass_seed | Grass Seed |
| 1015 | grasshopper | Grasshopper |
| 1016 | grayling | Grayling |
| 1017 | green_bottle | Green Bottle |
| 1018 | green_tea | Green Tea |
| 1019 | grilled_cheese | Grilled Cheese |
| 1020 | grilled_corn | Grilled Corn |
| 1021 | grilled_eel_rice_bowl | Grilled Eel Rice Bowl |
| 1022 | grouper | Grouper |
| 1025 | gryphon_statue_replica | Gryphon Statue Replica |
| 1026 | halibut | Halibut |
| 1027 | hard_boiled_egg | Hard Boiled Egg |
| 1028 | hard_wood | Hard Wood |
| 1029 | hardened_essence | Hardened Essence |
| 1030 | harvest_day_pie | Special Pie | **GLITCHED** |
| 1031 | harvest_festival_arch | Harvest Festival Arch |
| 1032 | harvest_festival_centerpiece | Harvest Festival Centerpiece |
| 1033 | harvest_plate | Harvest Plate |
| 1035 | haunted_attic_armoire | Haunted Attic Armoire |
| 1037 | haunted_attic_bed | Haunted Attic Bed |
| 1039 | haunted_attic_candle_cluster | Melted Candle Cluster |
| 1041 | haunted_attic_candle_single | Melted Candle |
| 1043 | haunted_attic_chair | Haunted Attic Chair |
| 1045 | haunted_attic_double_bed | Haunted Attic Double Bed |
| 1047 | haunted_attic_dress_form | Haunted Attic Dress Form |
| 1049 | haunted_attic_flooring | Haunted Attic Flooring |
| 1051 | haunted_attic_nightstand | Haunted Attic Nightstand |
| 1053 | haunted_attic_rocking_chair | Haunted Attic Rocking Chair |
| 1055 | haunted_attic_table | Haunted Attic Table |
| 1057 | haunted_attic_wall_cobweb_left | Cobweb Left |
| 1059 | haunted_attic_wall_cobweb_right | Cobweb Right |
| 1061 | haunted_attic_wall_shelf | Haunted Attic Wall Shelf |
| 1063 | haunted_attic_wall_window | Haunted Attic Window |
| 1065 | haunted_attic_wallpaper | Haunted Attic Wallpaper |
| 1066 | hay | Hay |
| 1067 | haydens_weathervane | Hayden's Weathervane |
| 1068 | heal_syrup | Healing Syrup |
| 1069 | heart_crystal | Heart Crystal |
| 1070 | heart_shaped_alpaca_treat | Heart Shaped Alpaca Treat |
| 1071 | heart_shaped_capybara_treat | Heart Shaped Capybara Treat |
| 1072 | heart_shaped_chicken_treat | Heart Shaped Chicken Treat |
| 1073 | heart_shaped_cow_treat | Heart Shaped Cow Treat |
| 1074 | heart_shaped_duck_treat | Heart Shaped Duck Treat |
| 1075 | heart_shaped_horse_treat | Heart Shaped Horse Treat |
| 1076 | heart_shaped_rabbit_treat | Heart Shaped Rabbit Treat |
| 1077 | heart_shaped_sheep_treat | Heart Shaped Sheep Treat |
| 1078 | heather | Heather |
| 1079 | heavy_mist | Heavy Mist |
| 1080 | herb_butter_pasta | Herb Butter Pasta |
| 1081 | herb_salad | Herb Salad |
| 1082 | hermit_crab | Hermit Crab |
| 1083 | hermit_snail | Hermit Snail |
| 1086 | hero_shield | Champion's Shield |
| 1087 | heros_ring | Hero's Ring |
| 1088 | herring | Herring |
| 1091 | herringbone_tile_floor | Herringbone Tile Floor |
| 1092 | hoe_copper | Copper Hoe |
| 1093 | hoe_gold | Gold Hoe |
| 1094 | hoe_iron | Iron Hoe |
| 1095 | hoe_mistril | Mistril Hoe |
| 1096 | hoe_silver | Silver Hoe |
| 1097 | hoe_worn | Worn Hoe |
| 1098 | holly | Holly |
| 1099 | honey | Honey |
| 1100 | honey_curry | Honey Curry |
| 1101 | horse_hair | Horse Hair |
| 1102 | horse_mackerel | Horse Mackerel |
| 1103 | horse_plushie | Horse Plushie |
| 1104 | horse_potion | MISSING | **GLITCHED** |
| 1105 | horseradish | Horseradish |
| 1106 | horseradish_salmon | Horseradish Salmon |
| 1107 | hot_cocoa | Hot Chocolate |
| 1108 | hot_potato | Hot Potato |
| 1109 | hot_toddy | Hot Toddy |
| 1110 | humble_pie | Humble Pie |
| 1111 | hummingbird_hawk_moth | Hummingbird Hawk Moth |
| 1112 | hydrangea | Hydrangea |
| 1113 | ice_block | Ice Block |
| 1114 | ice_cream_sundae | Ice Cream Sundae |
| 1115 | ice_snake_oil | Ice Snake Oil |
| 1116 | iced_coffee | Iced Coffee |
| 1117 | icehopper | Icehopper |
| 1118 | inchworm | Inchworm |
| 1119 | incredibly_hot_pot | Incredibly Hot Pot |
| 1120 | inn_candle | Inn Candle |
| 1121 | inn_counter | Inn Counter |
| 1122 | inn_cross_stitch | Inn Cross Stitch |
| 1123 | inn_kitchen_shelves | Inn Kitchen Shelves |
| 1124 | inn_menu | Inn Menu |
| 1125 | inn_stool | Inn Stool |
| 1126 | inn_tile_flooring | Inn Tile Flooring |
| 1127 | inn_wooden_flooring | Inn Wooden Flooring |
| 1130 | insect_flooring | Insect Flooring |
| 1133 | insect_rug | Insect Rug |
| 1136 | insect_wallpaper | Insect Wallpaper |
| 1137 | iris | Iris |
| 1138 | iron_armor | Iron Armor |
| 1139 | iron_greaves | Iron Greaves |
| 1140 | iron_helmet | Iron Helmet |
| 1141 | iron_ingot | Iron Ingot |
| 1142 | iron_legplates | Iron Legplates |
| 1143 | iron_ring | Iron Ring |
| 1144 | jam_sandwich | Jam Sandwich |
| 1145 | jar_large | Large Jar |
| 1146 | jar_medium | Medium Jar |
| 1147 | jar_pair | Pair of Jars |
| 1148 | jar_small | Small Jar |
| 1149 | jasmine | Jasmine |
| 1150 | jasmine_tea | Jasmine Tea |
| 1151 | jellyfish | Jellyfish |
| 1152 | jewel_beetle | Jewel Beetle |
| 1153 | journal | Diary |
| 1154 | killifish | Killifish |
| 1155 | king_crab | King Crab |
| 1156 | kitchen_garlic_braid | Garlic Braid |
| 1157 | kitchen_hanging_onion_basket | Hanging Onion Basket |
| 1159 | kitchen_herb_planter | Herb Planter |
| 1163 | kitchen_mixing_bowls | Mixing Bowls |
| 1165 | kitchen_oil_and_vinegar_basket | Oil & Vinegar Basket |
| 1166 | kitchen_salt_and_pepper_shakers | Salt & Pepper Shakers |
| 1170 | kitchen_scale | Kitchen Scale |
| 1174 | kitchen_utensil_holder | Utensil Holder |
| 1175 | koi | Koi |
| 1176 | ladder | House Ladder |
| 1177 | ladybug | Ladybug |
| 1180 | ladybug_table | Ladybug Table |
| 1181 | lake_chub | Lake Chub |
| 1182 | lake_trout | Lake Trout |
| 1183 | lamprey | Lamprey |
| 1184 | lantern_moth | Lantern Moth |
| 1185 | large_animal_ball_court | Large Animal Ball Court |
| 1186 | large_barn_black_blueprint | Large White Barn Blueprint |
| 1187 | large_barn_red_blueprint | Large Red Barn Blueprint |
| 1188 | large_coop_black_blueprint | Large White Coop Blueprint |
| 1189 | large_coop_red_blueprint | Large Red Coop Blueprint |
| 1190 | large_decorative_pumpkin_black | Large Decorative Pumpkin |
| 1191 | large_decorative_pumpkin_green | Large Decorative Pumpkin |
| 1192 | large_decorative_pumpkin_orange | Large Decorative Pumpkin |
| 1193 | large_decorative_pumpkin_purple | Large Decorative Pumpkin |
| 1194 | large_decorative_pumpkin_white | Large Decorative Pumpkin |
| 1195 | large_decorative_pumpkin_yellow | Large Decorative Pumpkin |
| 1203 | large_fluffy_rug | Large Fluffy Rug |
| 1204 | large_greenhouse_white_blueprint | Large White Greenhouse Blueprint |
| 1205 | large_greenhouse_wood_blueprint | Large Wood Greenhouse Blueprint |
| 1206 | large_mob_coin | coin | **GLITCHED** |
| 1209 | large_paving_stone | Large Paving Stone |
| 1210 | large_pouch | Bag Upgrade |
| 1211 | latte | Latte |
| 1220 | lava_caves_bed | Obsidian Double Bed |
| 1218 | lava_caves_chair | Obsidian Chair |
| 1226 | lava_caves_chest | Obsidian Storage Chest |
| 1212 | lava_caves_diamond_rock | Ornamental Diamond Rock |
| 1222 | lava_caves_fence | Obsidian Fence |
| 1224 | lava_caves_floor_lamp | Obsidian Floor Lamp |
| 1213 | lava_caves_flooring | Lava Rock Floor |
| 1214 | lava_caves_gold_node | Ornamental Gold Node |
| 1229 | lava_caves_stone_arch | Lava Rock Arch |
| 1228 | lava_caves_table | Obsidian Table |
| 1230 | lava_caves_wallpaper | Lava Rock Wall |
| 1231 | lava_chestnuts | Lava Chestnuts |
| 1232 | lava_piranha | Lava Piranha |
| 1233 | lava_snail | Lava Snail |
| 1234 | lavender_tea | Lavender Tea |
| 1235 | leaf_fish | Leaf Fish |
| 1236 | leafhopper | Leafhopper |
| 1237 | lemon | Lemon |
| 1238 | lemon_pie | Lemon Pie |
| 1239 | lemonade | Lemonade |
| 1240 | lightning_dragonfly | Lightning Dragonfly |
| 1241 | lightning_fish | Lightning Fish |
| 1242 | lilac | Lilac |
| 1243 | loach | Loach |
| 1244 | loaded_baked_potato | Loaded Baked Potato |
| 1245 | loam_caterpillar | Loam Caterpillar |
| 1246 | lobster | Lobster |
| 1247 | lobster_roll | Lobster Roll |
| 1248 | lost_crown_of_aldaria | Lost Crown of Aldaria |
| 1249 | lovely_cottage_scroll_bundle | Lovely Cottage Scroll Bundle |
| 1250 | lovely_seashell | Lovely Seashell |
| 1251 | low_tide_flooring | Low Tide Flooring |
| 1252 | luna_moth | Luna Moth |
| 1253 | lurid_colored_drink | MISSING | **GLITCHED** |
| 1254 | mackerel | Mackerel |
| 1255 | mackerel_sashimi | Mackerel Sashimi |
| 1256 | magma_beetle | Magma Beetle |
| 1257 | mana_potion | Mana Potion |
| 1258 | maples_drawing | Maple's Aldarian Star Drawing |
| 1259 | marigold | Marigold |
| 1260 | marmalade | Telepop (Orange) |
| 1261 | mayonnaise | Mayonnaise |
| 1262 | medium_barn_black_blueprint | Medium White Barn Blueprint |
| 1263 | medium_barn_red_blueprint | Medium Red Barn Blueprint |
| 1264 | medium_coop_black_blueprint | Medium White Coop Blueprint |
| 1265 | medium_coop_red_blueprint | Medium Red Coop Blueprint |
| 1266 | medium_decorative_pumpkin_black | Medium Decorative Pumpkin |
| 1267 | medium_decorative_pumpkin_green | Medium Decorative Pumpkin |
| 1268 | medium_decorative_pumpkin_orange | Medium Decorative Pumpkin |
| 1269 | medium_decorative_pumpkin_purple | Medium Decorative Pumpkin |
| 1270 | medium_decorative_pumpkin_white | Medium Decorative Pumpkin |
| 1271 | medium_decorative_pumpkin_yellow | Medium Decorative Pumpkin |
| 1272 | medium_mob_coin | coin | **GLITCHED** |
| 1278 | mermaid_bed | Mermaid Double Bed |
| 1279 | mermaids_comb | Mermaid's Comb |
| 1280 | metal_leaf | Metal Leaf |
| 1281 | meteorite | Meteorite |
| 1282 | middlemist | Middlemist |
| 1283 | mine_cricket | Mine Cricket |
| 1294 | minecart | Minecart |
| 1284 | miners_copper_node | Ornamental Copper Node |
| 1286 | miners_crate_chair | Mines Crate Chair |
| 1288 | miners_crate_chest | Mines Storage Chest |
| 1290 | miners_floor_lamp | Mines Floor Lamp |
| 1291 | miners_flooring | Mines Flooring |
| 1292 | miners_helmet | Miner's Helmet |
| 1295 | miners_mushroom_stew | Miner's Mushroom Stew |
| 1296 | miners_pickaxe | Miner's Pickaxe |
| 1297 | miners_ruby_rock | Ornamental Ruby Rock |
| 1298 | miners_rucksack | Miner's Rucksack |
| 1300 | miners_shelves | Mines Shelf |
| 1301 | miners_slab | Miner's Slab |
| 1303 | miners_wall_lamp | Mines Wall Lamp |
| 1304 | miners_wallpaper | Mines Wall |
| 1305 | mines_mussels | Mines Mussels |
| 1306 | mines_wooden_arch | Mines Arch |
| 1307 | mini_whale_shark | Mini-Whale Shark |
| 1308 | minnow | Minnow |
| 1309 | mint_gimlet | Mint Gimlet |
| 1310 | mist_crystal | Mist Crystal |
| 1311 | mist_flute | Mist Flute |
| 1312 | mist_held_item | Invalid Item | **GLITCHED** |
| 1313 | mist_scroll | Mist Scroll |
| 1314 | mistmoth | Mistmoth |
| 1315 | mistria_history_book | Mistria History Book |
| 1316 | mistria_history_scroll | Caldosian Decorative Scroll |
| 1317 | mistria_wall_map | Mistria Wall Map |
| 1318 | mistril_ingot | Mistril Ingot |
| 1319 | misty_black_mirror | Misty Black Mirror |
| 1320 | misty_feather_quill | Misty Feather Quill |
| 1321 | mixed_fruit_juice | Mixed Fruit Juice |
| 1322 | mob_coin | coin | **GLITCHED** |
| 1323 | mocha | Mocha |
| 1324 | monarch_butterfly | Monarch Butterfly |
| 1325 | monster_cookie | Monster Cookies |
| 1326 | monster_core | Monster Core |
| 1327 | monster_fang | Monster Fang |
| 1328 | monster_hoop | Monster Hoop |
| 1329 | monster_horn | Monster Horn |
| 1330 | monster_mash | Monster Mash |
| 1331 | monster_powder | Monster Powder |
| 1332 | monster_shell | Monster Shell |
| 1333 | monster_wing | Monster Wing |
| 1334 | mont_blanc | Mont Blanc |
| 1335 | moon_fruit | Moon Fruit |
| 1336 | moon_fruit_cake | Moon Fruit Cake |
| 1339 | moon_gate | Moon Gate |
| 1340 | morel_mushroom | Morel Mushroom |
| 1341 | mote_firefly | Mote Firefly |
| 1344 | moth_lamp | Moth Lamp |
| 1345 | mournful_clown_painting | Mournful Clown Painting |
| 1346 | mullet | Mullet |
| 1347 | mushroom_brew | Mushroom Brew |
| 1348 | mushroom_rice | Mushroom Rice |
| 1349 | mushroom_steak_dinner | Mushroom Steak Dinner |
| 1350 | muskie | Muskie |
| 1351 | muttering_cube | Muttering Cube |
| 1352 | narrows_moss | Narrows Moss |
| 1353 | net_copper | Copper Net |
| 1354 | net_gold | Gold Net |
| 1355 | net_iron | Iron Net |
| 1356 | net_mistril | Mistril Net |
| 1357 | net_silver | Silver Net |
| 1358 | net_worn | Worn Net |
| 1359 | nettle | Nettle |
| 1360 | newt | Newt |
| 1361 | night_queen | Night Queen |
| 1362 | noodles | Noodles |
| 1364 | notepad | Notepad |
| 1365 | oarfish | Oarfish |
| 1371 | obelisk | Obelisk |
| 1372 | obsidian | Obsidian |
| 1373 | obsidian_blade | Obsidian Blade |
| 1374 | ocarina_sprite_statue | Animal Sprite Statue |
| 1375 | ocean_sunfish | Ocean Sunfish |
| 1376 | octopus | Octopus |
| 1377 | octopus_plushie | Octopus Plushie |
| 1378 | oil | Oil |
| 1379 | omelet | Omelet |
| 1380 | onion | Onion |
| 1381 | onion_soup | Onion Soup |
| 1382 | orange | Orange |
| 1383 | orange_juice | Orange Juice |
| 1384 | orchid_mantis | Orchid Mantis |
| 1385 | ore_copper | Copper Ore |
| 1386 | ore_diamond | Diamond |
| 1387 | ore_emerald | Emerald |
| 1388 | ore_gold | Gold Ore |
| 1389 | ore_iron | Iron Ore |
| 1390 | ore_mistril | Mistril Ore |
| 1391 | ore_pink_diamond | Pink Diamond |
| 1392 | ore_ruby | Ruby |
| 1393 | ore_sapphire | Sapphire |
| 1394 | ore_silver | Silver Ore |
| 1395 | ore_stone | Stone |
| 1396 | oregano | Oregano |
| 1397 | ornamental_iron_node | Ornamental Iron Node |
| 1398 | ornamental_sapphire_rock | Ornamental Sapphire Rock |
| 1401 | ornate_flagstone | Ornate Flagstone |
| 1404 | ornate_rug_large_rectangle | Ornate Large Rectangle Rug |
| 1407 | ornate_rug_large_round | Ornate Large Round Rug |
| 1410 | ornate_rug_large_square | Ornate Large Square Rug |
| 1413 | ornate_rug_medium_round | Ornate Medium Round Rug |
| 1416 | ornate_rug_medium_square | Ornate Medium Square Rug |
| 1419 | ornate_rug_small | Ornate Small Rug |
| 1420 | oyster_mushroom | Oyster Mushroom |
| 1421 | paddlefish | Paddlefish |
| 1422 | pan_fried_bream | Pan Fried Bream |
| 1423 | pan_fried_salmon | Pan-fried Salmon |
| 1424 | pan_fried_snapper | Pan-fried Snapper |
| 1425 | paper | Paper |
| 1426 | paper_pondshell | Paper Pondshell |
| 1427 | paper_stack | Paper Stack |
| 1428 | peach | Peach |
| 1429 | peaches_and_cream | Peaches and Cream |
| 1430 | pear | Pear |
| 1431 | pearl_clam | Pearl Clam |
| 1432 | peas | Peas |
| 1433 | peat | Peat |
| 1434 | pebble_minnow | Pebble Minnow |
| 1435 | perch | Perch |
| 1436 | perch_risotto | Perch Risotto |
| 1437 | perfect_copper_ore | Perfect Copper Ore |
| 1438 | perfect_diamond | Perfect Diamond |
| 1439 | perfect_emerald | Perfect Emerald |
| 1440 | perfect_gold_ore | Perfect Gold Ore |
| 1441 | perfect_iron_ore | Perfect Iron Ore |
| 1442 | perfect_mistril_ore | Perfect Mistril Ore |
| 1443 | perfect_pink_diamond | Perfect Pink Diamond |
| 1444 | perfect_ruby | Perfect Ruby |
| 1445 | perfect_sapphire | Perfect Sapphire |
| 1446 | perfect_silver_ore | Perfect Silver Ore |
| 1447 | pet_cosmetic | MISSING |
| 1448 | petalhopper | Petalhopper |
| 1449 | petrified_wood | Petrified Wood |
| 1450 | pick_axe_copper | Copper Pickaxe |
| 1451 | pick_axe_gold | Gold Pickaxe |
| 1452 | pick_axe_iron | Iron Pickaxe |
| 1453 | pick_axe_mistril | Mistril Pickaxe |
| 1454 | pick_axe_silver | Silver Pickaxe |
| 1455 | pick_axe_worn | Worn Pickaxe |
| 1456 | picket_fence | Picket Fence |
| 1459 | picnic_basket | Picnic Basket |
| 1460 | picnic_donut_plate | Picnic Display Donuts |
| 1461 | picnic_hamburger_plate | Picnic Display Burgers |
| 1464 | picnic_pie_plate | Picnic Display Pie |
| 1467 | picnic_place_setting | Picnic Place Setting |
| 1470 | picnic_rug | Picnic Blanket |
| 1471 | picnic_sandwich_plate | Picnic Display Sandwiches |
| 1472 | picnic_set | Picnic Set |
| 1475 | picnic_sunflower_vase | Picnic Sunflower Vase |
| 1476 | pike | Pike |
| 1477 | pinecone | Pinecone |
| 1478 | pineshroom | Pineshroom |
| 1479 | pineshroom_toast | Pineshroom Toast |
| 1480 | pink_pet_bed | Pink Pet Bed |
| 1481 | pink_pet_dish | Pink Pet Dish |
| 1482 | pink_scallop_shell | Pink Scallop Shell |
| 1483 | pizza | Pizza |
| 1484 | plum_blossom | Plum Blossom |
| 1485 | poached_pear | Poached Pear |
| 1486 | poinsettia | Poinsettia |
| 1487 | poison_snake_oil | Poison Snake Oil |
| 1488 | polar_fly | Polar Fly |
| 1489 | pollock | Pollock |
| 1490 | pomegranate | Pomegranate |
| 1491 | pomegranate_juice | Pomegranate Juice |
| 1492 | pomegranate_sorbet | Pomegranate Sorbet |
| 1493 | pond_skater | Pond Skater |
| 1494 | pond_snail | Pond Snail |
| 1495 | potato | Potato |
| 1496 | potato_soup | Potato Soup |
| 1497 | praying_mantis | Praying Mantis |
| 1498 | pudding | Pudding |
| 1499 | puddle_spider | Puddle Spider |
| 1500 | puffer_fish | Puffer Fish |
| 1501 | pumpkin | Pumpkin |
| 1502 | pumpkin_pie | Pumpkin Pie |
| 1503 | pumpkin_pile_black | Pumpkin Pile |
| 1504 | pumpkin_pile_green | Pumpkin Pile |
| 1505 | pumpkin_pile_orange | Pumpkin Pile |
| 1506 | pumpkin_pile_purple | Pumpkin Pile |
| 1507 | pumpkin_pile_white | Pumpkin Pile |
| 1508 | pumpkin_pile_yellow | Pumpkin Pile |
| 1509 | pumpkin_stew | Pumpkin Stew |
| 1510 | purple_mushroom | Purple Mushroom |
| 1511 | purse | Purse of Tesserae | **DISABLED** |
| 1512 | quality_hay | Quality Large Animal Feed |
| 1513 | quality_small_animal_feed | Quality Small Animal Feed |
| 1514 | queen_berry | Queen Berry |
| 1515 | queen_berry_pie | Queen Berry Pie |
| 1516 | queen_berry_trophy | Golden Queen Berry Trophy |
| 1517 | question_mark_butterfly | Question Mark Butterfly |
| 1518 | quiche | Quiche |
| 1519 | rabbit_plushie | Rabbit Plushie |
| 1520 | rabbit_wool | Rabbit Wool |
| 1521 | rainbow_geode | Rainbow Geode |
| 1522 | rainbow_seaweed | Rainbow Seaweed |
| 1523 | rainbow_trout | Rainbow Trout |
| 1524 | razorback | Razorback |
| 1525 | really_round_rock | Really Round Rock |
| 1526 | recipe_scroll | Recipe: {} | **DISABLED** |
| 1527 | red_capybara_wall_ribbon | Red Capybara Wall Ribbon |
| 1528 | red_chicken_wall_ribbon | Red Chicken Wall Ribbon |
| 1529 | red_duck_wall_ribbon | Red Duck Wall Ribbon |
| 1530 | red_obsidian | Red Obsidian |
| 1531 | red_rabbit_wall_ribbon | Red Rabbit Wall Ribbon |
| 1532 | red_snapper | Red Snapper |
| 1533 | red_snapper_sushi | Red Snapper Sushi |
| 1534 | red_toadstool | Red Toadstool |
| 1535 | red_wine | Red Wine |
| 1536 | redhead_worm | Redhead Worm |
| 1537 | refined_stone | Refined Stone |
| 1538 | relic_crab | Relic Crab |
| 1539 | restorative_syrup | Restorative Syrup |
| 1540 | rhinoceros_beetle | Rhinoceros Beetle |
| 1541 | rice | Rice |
| 1542 | rice_stalk | Rice Stalk |
| 1543 | riceball | Rice Ball |
| 1544 | ritual_beads | Ritual Beads |
| 1545 | ritual_chalice | Ritual Chalice |
| 1546 | ritual_incense_burner | Ritual Incense Burner |
| 1547 | ritual_scepter | Ritual Scepter |
| 1548 | ritual_tablet | Ritual Tablet |
| 1549 | river_snail | River Snail |
| 1550 | roach | Roach |
| 1551 | roasted_cauliflower | Roasted Cauliflower |
| 1552 | roasted_chestnuts | Roasted Chestnuts |
| 1553 | roasted_rice_tea | Roasted Rice Tea |
| 1554 | roasted_sweet_potato | Roasted Sweet Potato |
| 1555 | rock_bass | Rock Bass |
| 1556 | rock_guppy | Rock Guppy |
| 1557 | rock_roach | Rock Roach |
| 1558 | rock_salt | Rock Salt |
| 1559 | rock_statue | Rock Statue |
| 1560 | rock_with_a_hole | Rock with a Hole |
| 1561 | rockbiter | Rockbiter |
| 1562 | rockroot | Rockroot |
| 1563 | roly_poly | Roly Poly |
| 1564 | rose | Rose |
| 1565 | rose_hip | Rose Hip |
| 1566 | rose_tea | Rose Tea |
| 1567 | rosehip_jam | Telepop (Pink) |
| 1568 | rosemary | Rosemary |
| 1569 | rosemary_garlic_noodles | Rosemary Garlic Noodles |
| 1570 | rough_gemstone | Rough Gemstone |
| 1578 | round_cushioned_stool | Round Cushioned Stool |
| 1584 | royal_chest | Royal Chest |
| 1585 | rubber_fish | Rubber Fish |
| 1586 | rusted_shield | Rusted Shield |
| 1587 | rusted_treasure_chest | Rusted Treasure Chest |
| 1588 | rustic_cabin_scroll_bundle | Rustic Cabin Scroll Bundle |
| 1589 | ryis_lumber | Ryis' Lumber | **GLITCHED** |
| 1590 | sage | Sage |
| 1591 | salmon | Salmon |
| 1592 | salmon_sashimi | Salmon Sashimi |
| 1593 | salted_watermelon | Salted Watermelon |
| 1594 | sand_bug | Sand Bug |
| 1595 | sand_dollar | Sand Dollar |
| 1596 | sand_lance | Sand Lance |
| 1597 | sand_rug | Sand Rug |
| 1598 | sap | Sap |
| 1599 | sapling_apple | Apple Sapling |
| 1600 | sapling_cherry | Cherry Sapling |
| 1601 | sapling_lemon | Lemon Sapling |
| 1602 | sapling_oak | Oak Sapling |
| 1603 | sapling_orange | Orange Sapling |
| 1604 | sapling_peach | Peach Sapling |
| 1605 | sapling_pear | Pear Sapling |
| 1606 | sapling_pine | Pine Sapling |
| 1607 | sapling_pomegranate | Pomegranate Sapling |
| 1608 | sapphire_betta | Sapphire Betta |
| 1609 | sardine | Sardine |
| 1610 | sauger | Sauger |
| 1611 | saury | Saury |
| 1612 | sauteed_snow_peas | Sauteed Snow Peas |
| 1613 | scent_of_spring | Scent of Spring |
| 1614 | scrap_metal_boots | Scrap Metal Boots |
| 1615 | scrap_metal_chestpiece | Scrap Metal Chestpiece |
| 1616 | scrap_metal_helmet | Scrap Metal Helmet |
| 1617 | scrap_metal_pants | Scrap Metal Pants |
| 1618 | scrap_metal_ring | Scrap Metal Ring |
| 1619 | sea_bass | Sea Bass |
| 1620 | sea_bream | Sea Bream |
| 1621 | sea_bream_rice | Sea Bream Rice |
| 1622 | sea_grapes | Sea Grapes |
| 1623 | sea_scarab | Sea Scarab |
| 1624 | seafood_boil | Seafood Boil |
| 1625 | seafood_snow_pea_noodles | Seafood Snow Pea Noodles |
| 1626 | sealing_scroll | Sealing Scroll |
| 1627 | seaweed | Seaweed |
| 1628 | seaweed_salad | Seaweed Salad |
| 1629 | seed_basil | Basil Seed |
| 1630 | seed_beet | Beet Seed |
| 1631 | seed_broccoli | Broccoli Seed |
| 1632 | seed_burdock_root | Burdock Root Seed |
| 1633 | seed_cabbage | Cabbage Seed |
| 1634 | seed_carrot | Carrot Seed |
| 1635 | seed_catmint | Catmint Seed |
| 1636 | seed_cauliflower | Cauliflower Seed |
| 1637 | seed_celosia | Celosia Seed |
| 1638 | seed_chickpea | Chickpea Seed |
| 1639 | seed_chili_pepper | Chili Pepper Seed |
| 1640 | seed_chrysanthemum | Chrysanthemum Seed |
| 1641 | seed_corn | Corn Seed |
| 1642 | seed_cosmos | Cosmos Seed |
| 1643 | seed_cranberry | Cranberry Seed |
| 1644 | seed_cucumber | Cucumber Seed |
| 1645 | seed_daffodil | Daffodil Seed |
| 1646 | seed_daikon_radish | Daikon Radish Seed |
| 1647 | seed_daisy | Daisy Seed |
| 1648 | seed_dill | Dill Seed |
| 1649 | seed_frost_lily | Frost Lily Seed |
| 1650 | seed_garlic | Garlic Seed |
| 1651 | seed_heather | Heather Seed |
| 1652 | seed_iris | Iris Seed |
| 1653 | seed_jasmine | Jasmine Seed |
| 1654 | seed_lilac | Lilac Seed |
| 1655 | seed_marigold | Marigold Seed |
| 1656 | seed_moon_fruit | Moon Fruit Seed |
| 1657 | seed_mystery_bag | Magic Seed |
| 1658 | seed_night_queen | Night Queen Seed |
| 1659 | seed_onion | Onion Seed |
| 1660 | seed_oregano | Oregano Seed |
| 1661 | seed_peas | Pea Seed |
| 1662 | seed_poinsettia | Poinsettia Seed |
| 1663 | seed_potato | Potato Seed |
| 1664 | seed_pumpkin | Pumpkin Seed |
| 1665 | seed_rice | Rice Seed |
| 1666 | seed_rosemary | Rosemary Seed |
| 1667 | seed_sage | Sage Seed |
| 1668 | seed_snapdragon | Snapdragon Seed |
| 1669 | seed_snow_peas | Snow Peas Seed |
| 1670 | seed_snowdrop_anemone | Snowdrop Anemone Seed |
| 1671 | seed_strawberry | Strawberry Seed |
| 1672 | seed_sugar_cane | Sugar Cane Seed |
| 1673 | seed_sunflower | Sunflower Seed |
| 1674 | seed_sweet_potato | Sweet Potato Seed |
| 1675 | seed_tea | Tea Seed |
| 1676 | seed_temple_flower | Temple Flower Seed |
| 1677 | seed_thyme | Thyme Seed |
| 1678 | seed_tomato | Tomato Seed |
| 1679 | seed_tulip | Tulip Seed |
| 1680 | seed_turnip | Turnip Seed |
| 1681 | seed_viola | Viola Seed |
| 1682 | seed_watermelon | Watermelon Seed |
| 1683 | seed_wheat | Wheat Seed |
| 1684 | seriously_square_stone | Seriously Square Stone |
| 1685 | sesame | Sesame |
| 1686 | sesame_broccoli | Sesame Broccoli |
| 1687 | sesame_tuna_bowl | Sesame Tuna Bowl |
| 1688 | shad | Shad |
| 1689 | shadow_bass | Shadow Bass |
| 1690 | shadow_flower | Shadow Flower |
| 1691 | shale_grass | Shale Grass |
| 1692 | shard_mass | Shard Mass |
| 1693 | shardfin | Shardfin |
| 1694 | shards | Shards |
| 1695 | shark | Shark |
| 1696 | sheep_plushie | Sheep Plushie |
| 1697 | sheep_wool | Sheep Wool |
| 1698 | shortcut_scroll | Shortcut Scroll |
| 1699 | shovel_copper | Copper Shovel |
| 1700 | shovel_gold | Gold Shovel |
| 1701 | shovel_iron | Iron Shovel |
| 1702 | shovel_mistril | Mistril Shovel |
| 1703 | shovel_silver | Silver Shovel |
| 1704 | shovel_worn | Worn Shovel |
| 1705 | shrimp | Shrimp |
| 1706 | silver_alpaca_trophy | Silver Alpaca Trophy |
| 1707 | silver_armor | Silver Armor |
| 1708 | silver_cow_trophy | Silver Cow Trophy |
| 1709 | silver_greaves | Silver Greaves |
| 1710 | silver_helmet | Silver Helmet |
| 1711 | silver_horse_trophy | Silver Horse Trophy |
| 1712 | silver_ingot | Silver Ingot |
| 1713 | silver_legplates | Silver Legplates |
| 1714 | silver_redhorse | Silver Redhorse |
| 1715 | silver_ring | Silver Ring |
| 1716 | silver_sheep_trophy | Silver Sheep Trophy |
| 1717 | simmered_daikon | Simmered Daikon |
| 1718 | singing_katydid | Singing Katydid |
| 1719 | sliced_turnip | Sliced Turnip |
| 1720 | small_animal_seesaw | Small Animal Seesaw |
| 1721 | small_barn_black_blueprint | Small White Barn Blueprint |
| 1722 | small_barn_red_blueprint | Small Red Barn Blueprint |
| 1730 | small_bathmat | Small Bathmat |
| 1731 | small_coop_black_blueprint | Small White Coop Blueprint |
| 1732 | small_coop_red_blueprint | Small Red Coop Blueprint |
| 1733 | small_decorative_pumpkin_black | Small Decorative Pumpkin |
| 1734 | small_decorative_pumpkin_green | Small Decorative Pumpkin |
| 1735 | small_decorative_pumpkin_orange | Small Decorative Pumpkin |
| 1736 | small_decorative_pumpkin_purple | Small Decorative Pumpkin |
| 1737 | small_decorative_pumpkin_white | Small Decorative Pumpkin |
| 1738 | small_decorative_pumpkin_yellow | Small Decorative Pumpkin |
| 1739 | small_grass_starter | Grass Starter |
| 1740 | small_greenhouse_white_blueprint | Small White Greenhouse Blueprint |
| 1741 | small_greenhouse_wood_blueprint | Small Wood Greenhouse Blueprint |
| 1745 | small_vase | Small Vase |
| 1746 | smallmouth_bass | Smallmouth Bass |
| 1747 | smoke_moth | Smoke Moth |
| 1748 | smoked_trout_soup | Smoked Trout Soup |
| 1749 | snail | Snail |
| 1750 | snakehead | Snakehead |
| 1751 | snapdragon | Snapdragon |
| 1752 | snapping_turtle | Snapping Turtle |
| 1753 | snow_fish | Snow Fish |
| 1754 | snow_peas | Snow Peas |
| 1755 | snowball_beetle | Snowball Beetle |
| 1756 | snowdrop_anemone | Snowdrop Anemone |
| 1757 | snug_bug | Snug Bug |
| 1758 | sod | Sod |
| 1759 | soup_of_the_day | Soup of the Day | **GLITCHED** |
| 1760 | soup_of_the_day_gold | Soup of the Day Deluxe | **GLITCHED** |
| 1761 | sour_lemon_cake | Lemon Cake |
| 1762 | soy_sauce | Soy Sauce |
| 1763 | speedy_snail | Speedy Snail |
| 1764 | speedy_syrup | Speedy Syrup |
| 1765 | spell_fruit | Spell Fruit |
| 1766 | spell_fruit_parfait | Spell Fruit Parfait |
| 1767 | spicy_cheddar_biscuit | Spicy Cheddar Biscuit |
| 1768 | spicy_corn | Spicy Corn |
| 1769 | spicy_crab_sushi | Spicy Crab Sushi |
| 1770 | spicy_water_chestnuts | Spicy Water Chestnuts |
| 1771 | spirit_mushroom | Spirit Mushroom |
| 1772 | spirit_mushroom_tea | Spirit Mushroom Tea |
| 1773 | spirula_shell | Spirula Shell |
| 1774 | spooky_haybale | Hay Bale |
| 1777 | spring_bed | Spring Bed |
| 1778 | spring_crop_sign_scroll_bundle | Spring Crop Sign Scroll Bundle |
| 1781 | spring_double_bed | Spring Double Bed |
| 1782 | spring_festival_basket | Breath of Spring Basket |
| 1783 | spring_festival_breath_of_spring_wreath | Deluxe Breath of Spring Wreath |
| 1784 | spring_festival_flower_chest | Breath of Spring Storage Chest |
| 1785 | spring_festival_large_planter | Breath of Spring Large Planter |
| 1786 | spring_festival_planter | Breath of Spring Planter |
| 1787 | spring_festival_wreath | Breath of Spring Wreath |
| 1790 | spring_flooring | Spring Flooring |
| 1791 | spring_galette | Spring Galette |
| 1794 | spring_lamp | Spring Lamp |
| 1797 | spring_potted_tree | Spring Potted Tree |
| 1800 | spring_rug | Spring Flower Rug |
| 1801 | spring_salad | Spring Salad |
| 1804 | spring_sofa | Spring Sofa |
| 1807 | spring_wallpaper | Spring Wallpaper |
| 1808 | square_tile_floor | Square Tile Floor |
| 1809 | squid | Squid |
| 1810 | stairs | House Stairs |
| 1811 | stamina_syrup | Stamina Syrup |
| 1812 | stamina_up | Stamina Up |
| 1813 | star_brooch | Star Brooch |
| 1819 | star_cushion | Star Cushion |
| 1824 | star_lantern | Star Lantern |
| 1825 | star_shaped_cookie | Star-Shaped Cookie |
| 1829 | star_viewing_blanket | Star Viewing Blanket |
| 1830 | starlight_coral | Starlight Coral |
| 1832 | starry_flooring | Starry Flooring |
| 1833 | starter_bird_house_red | Birdhouse |
| 1834 | starter_potted_plant | Potted Plant |
| 1835 | starter_scarecrow | Scarecrow |
| 1836 | starter_shipping_box | Shipping Bin |
| 1837 | starter_stone_path | Stone Path |
| 1838 | starter_stone_path_double | Stone Path |
| 1839 | starter_well | Water Well |
| 1840 | starter_wheelbarrow | Wheelbarrow |
| 1841 | starter_wood_fence | Wooden Fence |
| 1842 | statuette_of_caldarus | Statuette of Caldarus |
| 1843 | steamed_broccoli | Steamed Broccoli |
| 1844 | stingray | Stingray |
| 1845 | stinky_stamina_potion | MISSING | **GLITCHED** |
| 1848 | stone_bench | Stone Bench |
| 1851 | stone_dragon_figure | Stone Dragon Figure |
| 1852 | stone_horse | Stone Horse |
| 1855 | stone_lamp | Stone Lamp |
| 1856 | stone_loach | Stone Loach |
| 1857 | stone_shell | Stone Shell |
| 1860 | stone_storage_chest | Stone Storage Chest |
| 1863 | stone_table | Stone Table |
| 1864 | stone_wall | Stone Wall |
| 1865 | strawberries_and_cream | Strawberries and Cream |
| 1866 | strawberry | Strawberry |
| 1867 | strawberry_shortcake | Strawberry Shortcake |
| 1868 | strawhopper | Strawhopper |
| 1869 | striped_bass | Striped Bass |
| 1870 | strobe_firefly | Strobe Firefly |
| 1871 | sturgeon | Sturgeon |
| 1872 | sugar | Sugar |
| 1873 | sugar_cane | Sugar Cane |
| 1874 | sulfur_crab | Sulfur Crab |
| 1876 | summer_bed | Summer Bed |
| 1878 | summer_chair | Summer Chair |
| 1879 | summer_crop_sign_scroll_bundle | Summer Crop Sign Scroll Bundle |
| 1881 | summer_double_bed | Summer Double Bed |
| 1883 | summer_flooring | Summer Flooring |
| 1885 | summer_jar_lamp | Summer Jar Lamp |
| 1887 | summer_rug | Summer Flower Rug |
| 1888 | summer_salad | Summer Salad |
| 1890 | summer_table | Summer Table |
| 1892 | summer_wallpaper | Summer Wallpaper |
| 1894 | summit_wallpaper | Summit Wallpaper |
| 1895 | sunflower | Sunflower |
| 1896 | sunny | Sunny |
| 1897 | surf_beetle | Surf Beetle |
| 1898 | sushi_platter | Sushi Platter |
| 1899 | sweet_potato | Sweet Potato |
| 1900 | sweet_potato_pie | Sweet Potato Pie |
| 1901 | sweet_sesame_balls | Sweet Sesame Balls |
| 1902 | sweetfish | Sweetfish |
| 1903 | sweetroot | Sweetroot |
| 1904 | sword_copper | Copper Sword |
| 1905 | sword_crystal | Crystal Sword |
| 1906 | sword_gold | Gold Sword |
| 1907 | sword_iron | Iron Sword |
| 1908 | sword_mistril | Mistril Sword |
| 1909 | sword_scrap_metal | Scrap Metal Sword |
| 1910 | sword_silver | Silver Sword |
| 1911 | sword_tarnished_gold | Tarnished Gold Sword |
| 1912 | sword_verdigris | Verdigris Sword |
| 1913 | sword_worn | Worn Sword |
| 1914 | swordfish | Swordfish |
| 1915 | tarnished_gold_armor | Tarnished Gold Chestpiece |
| 1916 | tarnished_gold_boots | Tarnished Gold Greaves |
| 1917 | tarnished_gold_helmet | Tarnished Gold Helmet |
| 1918 | tarnished_gold_legplates | Tarnished Gold Legplates |
| 1919 | tarnished_gold_ring | Tarnished Gold Ring |
| 1920 | tarpon | Tarpon |
| 1921 | tea | Tea |
| 1922 | tea_black | Black Tea Cup |
| 1923 | tea_green | Green Tea Cup |
| 1924 | teleportation_pad | Teleportation Circle |
| 1925 | temple_flower | Temple Flower |
| 1926 | tetra | Tetra |
| 1927 | thorn_vine | Thorn Vine |
| 1928 | thyme | Thyme |
| 1929 | tide_lettuce | Tide Lettuce |
| 1930 | tide_salad | Tide Salad |
| 1931 | tide_touched_cavern_wall | Tide-Touched Cavern Wall |
| 1932 | tidestone | Tidestone |
| 1933 | tiger_swallowtail_butterfly | Tiger Swallowtail Butterfly |
| 1934 | tilapia | Tilapia |
| 1937 | tile_roof_fence | Tile Roof Fence |
| 1938 | tin_lunchbox | Tin Lunchbox |
| 1939 | tiny_dinosaur_skeleton | Tiny Dinosaur Skeleton |
| 1940 | tiny_volcano | Tiny Volcano |
| 1941 | toasted_sunflower_seeds | Toasted Sunflower Seeds |
| 1943 | toilet | Toilet |
| 1944 | tomato | Tomato |
| 1945 | tomato_soup | Tomato Soup |
| 1946 | trail_mix | Trail Mix |
| 1949 | training_dummy | Training Dummy |
| 1950 | transparent_jellyfish | Transparent Jellyfish |
| 1951 | treasure_box_copper | Copper Treasure Box |
| 1952 | treasure_box_gold | Golden Treasure Box |
| 1953 | treasure_box_silver | Silver Treasure Box |
| 1954 | treasure_box_wood | Wooden Treasure Box |
| 1955 | trilobite_fossil | Trilobite Fossil |
| 1956 | trout | Trout |
| 1957 | tulip | Tulip |
| 1958 | tulip_cake | Tulip Cake |
| 1959 | tuna | Tuna |
| 1960 | tuna_sashimi | Tuna Sashimi |
| 1961 | tunnel_millipede | Tunnel Millipede |
| 1962 | turnip | Turnip |
| 1963 | turnip_and_cabbage_salad | Turnip & Cabbage Salad |
| 1964 | turnip_and_potato_gratin | Turnip & Potato Gratin |
| 1965 | turtle | Turtle |
| 1966 | twice_baked_rations | Twice-Baked Rations |
| 1967 | ultimate_hay | Ultimate Large Animal Feed |
| 1968 | ultimate_small_animal_feed | Ultimate Small Animal Feed |
| 1969 | underseaweed | Underseaweed |
| 1970 | unidentified_artifact | Unidentified Artifact | **DISABLED** |
| 1971 | unknown_dragon_statuette | Unknown Dragon Statuette |
| 1972 | unknown_item | Unknown Item | **PLACEHOLDER** |
| 1973 | unusual_seed | Unusual Seed | **GLITCHED** |
| 1974 | upper_mines_mushroom | Upper Mines Mushroom |
| 1975 | vegetable_pot_pie | Vegetable Pot Pie |
| 1976 | vegetable_quiche | Vegetable Quiche |
| 1977 | vegetable_soup | Vegetable Soup |
| 1978 | veggie_sub_sandwich | Veggie Sub Sandwich |
| 1979 | verdigris_boots | Verdigris Boots |
| 1980 | verdigris_chestpiece | Verdigris Chestpiece |
| 1981 | verdigris_helmet | Verdigris Helmet |
| 1982 | verdigris_ring | Verdigris Ring |
| 1983 | verdigris_tassets | Verdigris Tassets |
| 1984 | vintage_brush | Vintage Brush |
| 1985 | vintage_cow_bell | Vintage Cow Bell |
| 1986 | vintage_hammer | Vintage Hammer |
| 1987 | vintage_sickle | Vintage Sickle |
| 1988 | vintage_watering_can | Vintage Watering Can |
| 1989 | viola | Viola |
| 1990 | walking_leaf | Walking Leaf |
| 1991 | walking_stick | Walking Stick |
| 1992 | walleye | Walleye |
| 1993 | walnut | Walnut |
| 1994 | warm_rock | Warm Rock |
| 1995 | water_balloon_fish | Water Balloon Fish |
| 1996 | water_chestnut | Water Chestnut |
| 1997 | water_chestnut_fritters | Water Chestnut Fritters |
| 1998 | water_sphere | Water Sphere |
| 2003 | water_sprite | Water Sprite Statue |
| 2004 | water_tablet_rubbing | Water Tablet Rubbing |
| 2005 | waterbug | Waterbug |
| 2006 | waterfly | Waterfly |
| 2007 | watering_can_copper | Copper Watering Can |
| 2008 | watering_can_gold | Gold Watering Can |
| 2009 | watering_can_iron | Iron Watering Can |
| 2010 | watering_can_mistril | Mistril Watering Can |
| 2011 | watering_can_silver | Silver Watering Can |
| 2012 | watering_can_worn | Worn Watering Can |
| 2013 | watermelon | Watermelon |
| 2014 | weather_crystal_ball | Weather Crystal Ball |
| 2015 | weightless_stone | Weightless Stone |
| 2016 | wheat | Wheat |
| 2017 | white_capybara_wall_ribbon | White Capybara Wall Ribbon |
| 2018 | white_chicken_wall_ribbon | White Chicken Wall Ribbon |
| 2019 | white_duck_wall_ribbon | White Duck Wall Ribbon |
| 2020 | white_perch | White Perch |
| 2021 | white_rabbit_wall_ribbon | White Rabbit Wall Ribbon |
| 2022 | white_wine | White Wine |
| 2023 | wild_berries | Wild Berries |
| 2024 | wild_berry_jam | Wild Berry Jam |
| 2025 | wild_grapes | Wild Grapes |
| 2026 | wild_leek | Wild Leek |
| 2027 | wild_mushroom | Cave Mushroom |
| 2028 | wildberry_pie | Wild Berry Pie |
| 2029 | wildberry_scone | Wild Berry Scone |
| 2030 | windleaf_butterfly | Windleaf Butterfly |
| 2031 | wine_glass_red | Red Wine Glass |
| 2032 | wine_glass_rose | Rose Wine Glass |
| 2033 | wine_glass_white | White Wine Glass |
| 2034 | winter_crop_sign_scroll_bundle | Winter Crop Sign Scroll Bundle |
| 2035 | winter_stew | Winter Stew |
| 2036 | wintergreen_berry | Wintergreen Berry |
| 2037 | wintergreen_ice_cream | Wintergreen Ice Cream |
| 2038 | winterpillar | Winterpillar |
| 2039 | witch_queen_bed | Witch Queen Bed |
| 2040 | witch_queen_cauldron | Witch Queen Cauldron |
| 2041 | witch_queen_chair | Witch Queen Chair |
| 2042 | witch_queen_double_bed | Witch Queen Double Bed |
| 2043 | witch_queen_dressing_table | Witch Queen Dressing Table |
| 2044 | witch_queen_flooring | Witch Queen Flooring |
| 2045 | witch_queen_moon_lamp | Witch Queen Moon Lamp |
| 2046 | witch_queen_nightstand | Witch Queen Nightstand |
| 2047 | witch_queen_pillar | Witch Queen Pillar |
| 2048 | witch_queen_rug | Witch Queen Rug |
| 2049 | witch_queen_table | Witch Queen Table |
| 2050 | witch_queen_throne | Witch Queen Throne |
| 2051 | witch_queen_wallpaper | Witch Queen Wallpaper |
| 2052 | woodcrafting_station | Crafting Station |
| 2053 | world_fountain | World Fountain 20 | **GLITCHED** |
| 2054 | worm | Worm |
| 2055 | worn_bed | Worn Bed |
| 2056 | worn_chair | Worn Chair |
| 2057 | worn_fireplace | Worn Fireplace |
| 2058 | worn_flooring | Worn Flooring |
| 2059 | worn_garden_lamp | Worn Garden Lamp |
| 2060 | worn_nightstand | Worn Nightstand |
| 2061 | worn_rug | Worn Rug |
| 2062 | worn_table | Worn Table |
| 2063 | worn_wallpaper | Worn Wallpaper |
| 2064 | worn_well | Worn Well |
| 2065 | worn_window | Worn Window |

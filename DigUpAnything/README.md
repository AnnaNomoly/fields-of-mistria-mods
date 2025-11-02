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


## Item Table (FoM v0.14.4)
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
| 29 | animated_bird_fountain | Animated Bird Fountain |
| 30 | animated_snow_globe | Animated Snow Globe |
| 31 | ant | Ant |
| 32 | apple | Apple |
| 33 | apple_honey_curry | Apple Honey Curry |
| 34 | apple_juice | Apple Juice |
| 35 | apple_pie | Apple Pie |
| 36 | archerfish | Archerfish |
| 37 | armored_bass | Armored Bass |
| 38 | ash_mushroom | Ash Mushroom |
| 39 | autumn_scarecrow | Autumn Scarecrow |
| 40 | axe_copper | Copper Axe |
| 41 | axe_gold | Gold Axe |
| 42 | axe_iron | Iron Axe |
| 43 | axe_mistril | Mistril Axe |
| 44 | axe_silver | Silver Axe |
| 45 | axe_worn | Worn Axe |
| 46 | baked_potato | Baked Potato |
| 47 | baked_sweetroot | Baked Sweetroot |
| 49 | bakery_bread_basket | Bakery Bread Basket |
| 53 | bakery_cake | Bakery Display Cake |
| 51 | bakery_cake_case | Bakery Dessert Case |
| 55 | bakery_cake_slice | Bakery Display Cake Slice |
| 57 | bakery_cookie_jar | Bakery Cookie Jar |
| 59 | bakery_cutting_board | Bakery Cutting Board |
| 60 | balors_crate | Balor's Crate | **GLITCHED** |
| 61 | barb | Barb |
| 63 | basic_area_rug | Basic Area Rug |
| 67 | basic_bed | Basic Single Bed |
| 65 | basic_bed_double | Basic Double Bed |
| 69 | basic_bookshelf | Basic Bookshelf |
| 71 | basic_chair | Basic Chair |
| 73 | basic_doormat | Basic Doormat |
| 75 | basic_dresser | Basic Dresser |
| 77 | basic_flooring | Basic Flooring |
| 79 | basic_flower_pot | Basic Flower Pot |
| 81 | basic_nightstand | Basic Nightstand |
| 83 | basic_oil_lamp | Basic Oil Lamp |
| 84 | basic_pouch | Bag Upgrade |
| 86 | basic_table | Basic Table |
| 88 | basic_wall_window | Basic Window |
| 90 | basic_wallpaper | Basic Wallpaper |
| 91 | basic_wood | Wood |
| 106 | basic_wood_chest | Basic Wood Chest |
| 107 | basil | Basil |
| 108 | basket | Basket |
| 109 | bath_soap | Bath Soap |
| 111 | bathroom_bench | Bathroom Bench |
| 118 | bathroom_curtain_stand | Bathroom Curtain Stand |
| 120 | bathroom_floor_mirror | Bathroom Floor Mirror |
| 122 | bathroom_sink | Bathroom Sink |
| 126 | bathroom_tile_wall | Bathroom Tile Wall |
| 128 | bathroom_wall_mirror | Bathroom Wall Mirror |
| 129 | bathroom_wall_sconce | Bathroom Wall Sconce |
| 136 | bathroom_wall_towel | Bathroom Wall Towel |
| 138 | bathtub | Bathtub |
| 139 | beach_hopper | Beach Hopper |
| 140 | beer | Beer |
| 141 | beer_mug | Beer Mug |
| 142 | beet | Beet |
| 143 | beet_salad | Beet Salad |
| 144 | beet_soup | Beet Soup |
| 145 | beginner_kitchen | Level 1 Kitchen |
| 146 | bell_berry | Bell Berry |
| 147 | bell_berry_bakewell_tart | Bell Berry Bakewell Tart |
| 148 | berries_and_cream | Berries and Cream |
| 149 | berry_bowl | Berry Bowl |
| 150 | big_bell | Big Bell |
| 151 | big_cookie | Big Cookie |
| 152 | black_tablet | Black Tablet |
| 153 | blackberry | Blackberry |
| 154 | blackberry_jam | Blackberry Jam |
| 155 | blue_capybara_wall_ribbon | Blue Capybara Wall Ribbon |
| 156 | blue_chicken_wall_ribbon | Blue Chicken Wall Ribbon |
| 157 | blue_conch_shell | Blue Conch Shell |
| 158 | blue_duck_wall_ribbon | Blue Duck Wall Ribbon |
| 159 | blue_pet_bed | Blue Pet Bed |
| 160 | blue_pet_dish | Blue Pet Dish |
| 161 | blue_rabbit_wall_ribbon | Blue Rabbit Wall Ribbon |
| 162 | blueberry | Blueberry |
| 163 | blueberry_jam | Blueberry Jam |
| 164 | bluefish | Bluefish |
| 165 | bluegill | Blue Gill |
| 166 | bomb | Clod Bomb |
| 167 | bonito | Bonito |
| 168 | bowfish | Bowfish |
| 169 | box_medium | Medium Box |
| 171 | box_small | Small Box |
| 172 | braised_burdock | Braised Burdock |
| 173 | braised_carrots | Braised Carrots |
| 174 | bread | Bread |
| 175 | breaded_catfish | Breaded Catfish |
| 176 | bream | Bream |
| 177 | breath_of_fire | Breath of Flame |
| 178 | breath_of_spring | Breath of Spring |
| 179 | brightbulb_moth | Brightbulb Moth |
| 180 | bristle | Bristle |
| 181 | broccoli | Broccoli |
| 182 | broccoli_salad | Broccoli Salad |
| 183 | bronze_alpaca_trophy | Bronze Alpaca Trophy |
| 184 | bronze_cow_trophy | Bronze Cow Trophy |
| 185 | bronze_horse_trophy | Bronze Horse Trophy |
| 186 | bronze_sheep_trophy | Bronze Sheep Trophy |
| 187 | brown_bullhead | Brown Bullhead |
| 188 | brown_trout | Brown Trout |
| 189 | bucket_brew | Bucket Brew |
| 190 | bull_horn | Bull Horn |
| 191 | bullfrog | Bullfrog |
| 192 | bumblebee | Bumblebee |
| 193 | bumblebee_plushie | Bumblebee Plushie |
| 194 | burbot | Burbot |
| 195 | burdock_root | Burdock Root |
| 196 | butter | Butter |
| 197 | buttered_peas | Buttered Peas |
| 198 | butterfish | Butterfish |
| 199 | butterfly | Butterfly |
| 202 | butterfly_bed | Butterfly Bed |
| 205 | butterfly_double_bed | Butterfly Double Bed |
| 208 | butterfly_wall_lamp | Butterfly Wall Lamp |
| 209 | cabbage | Cabbage |
| 210 | cabbage_slaw | Cabbage Slaw |
| 216 | cabin_bed | Rustic Cabin Bed |
| 214 | cabin_bed_double | Rustic Cabin Double Bed |
| 219 | cabin_bookshelf | Rustic Cabin Bookshelf |
| 222 | cabin_chair | Rustic Cabin Chair |
| 225 | cabin_dresser | Rustic Cabin Dresser |
| 228 | cabin_firewood | Rustic Cabin Firewood |
| 231 | cabin_flooring | Rustic Cabin Flooring |
| 234 | cabin_lamp | Rustic Cabin Lamp |
| 237 | cabin_nightstand | Rustic Cabin Nightstand |
| 239 | cabin_stove | Rustic Cabin Stove |
| 242 | cabin_table | Rustic Cabin Table |
| 245 | cabin_wall_shelf | Rustic Cabin Wall Shelf |
| 248 | cabin_wall_window | Rustic Cabin Window |
| 251 | cabin_wallpaper | Rustic Cabin Wallpaper |
| 252 | caldosian_breastplate | Caldosian Breastplate |
| 253 | caldosian_chocolate_cake | Chocolate Cake |
| 254 | caldosian_drinking_horn | Caldosian Drinking Horn |
| 255 | caldosian_emperor_bust | Caldosian Emperor Bust |
| 256 | caldosian_sword | Caldosian Sword |
| 257 | campfire | Campfire |
| 258 | candelabra_seadragon | Candelabra Seadragon |
| 259 | candied_lemon_peel | Candied Lemon Peel |
| 260 | candied_queen_berries | Candied Queen Berries |
| 261 | candied_strawberries | Candied Strawberries |
| 262 | candied_walnuts | Candied Walnuts |
| 263 | candle | Candle |
| 264 | candle_chamberstick | Candle Chamberstick |
| 265 | canned_sardines | Canned Sardines |
| 266 | capybara_plushie | Capybara Plushie |
| 267 | caramel_candy | Caramel Candy |
| 268 | caramelized_moon_fruit | Caramelized Moon Fruit |
| 269 | carp | Carp |
| 270 | carrot | Carrot |
| 271 | cat_treat | Cat Treat |
| 272 | caterpillar | Caterpillar |
| 275 | caterpillar_chair | Caterpillar Chair |
| 276 | catfish | Catfish |
| 277 | catmint | Catmint |
| 278 | cattail | Cattail Fluff |
| 279 | cauliflower | Cauliflower |
| 280 | cauliflower_curry | Cauliflower Curry |
| 281 | cave_eel | Cave Eel |
| 282 | cave_kelp | Jade Dulse |
| 283 | cave_shark | Cave Shark |
| 284 | cave_shrimp | Cave Shrimp |
| 287 | cavern_crystal_lamp | Glowing Crystal Lamp |
| 288 | cavern_emerald_rock | Ornamental Emerald Rock |
| 291 | cavern_floor_brazier | Floor Brazier |
| 292 | cavern_hewn_rock_bookshelf | Hewn Rock Bookshelf |
| 293 | cavern_hewn_rock_table | Hewn Rock Table |
| 295 | cavern_mossy_rug | Mossy Rug |
| 298 | cavern_mushroom_stool | Mushroom Stool |
| 299 | cavern_rock_flooring | Rock Floor |
| 300 | cavern_rock_wallpaper | Rock Wall |
| 301 | cavern_silver_node | Ornamental Silver Node |
| 302 | cavern_stone_arch | Stalactite Arch |
| 305 | cavern_wall_brazier | Wall Brazier |
| 306 | celosia | Celosia |
| 310 | champions_kitchen | Champion's Kitchen |
| 311 | char | Char |
| 312 | cheese | Cheese |
| 313 | cherry | Cherry |
| 314 | cherry_cobbler | Cherry Cobbler |
| 315 | cherry_fish | Cherry Fish |
| 316 | cherry_smoothie | Cherry Smoothie |
| 317 | cherry_tart | Cherry Tart |
| 321 | chess_bishop | Chess Bishop Piece |
| 325 | chess_king | Chess King Piece |
| 329 | chess_knight | Chess Knight Piece |
| 333 | chess_pawn | Chess Pawn Piece |
| 337 | chess_queen | Chess Queen Piece |
| 341 | chess_rook | Chess Rook Piece |
| 342 | chess_scroll_bundle | Chess Piece Scroll Bundle |
| 346 | chess_table | Chess Table |
| 347 | chestnut | Chestnut |
| 348 | chicken_plushie | Chicken Plushie |
| 349 | chickpea | Chickpea |
| 350 | chickpea_curry | Chickpea Curry |
| 351 | chicky_hot_chocolate | Chicky Hot Chocolate |
| 352 | chili_coconut_curry | Chili Coconut Curry |
| 353 | chili_pepper | Chili Pepper |
| 354 | chillipede | Chillipede |
| 355 | chocolate | Chocolate |
| 356 | chrysanthemum | Chrysanthemum |
| 357 | chub | Chub |
| 358 | chum | Chum |
| 359 | cicada | Cicada |
| 360 | cicada_nymph | Cicada Nymph |
| 361 | clam | Clam |
| 362 | clam_chowder | Clam Chowder |
| 363 | clay | Clay |
| 364 | clay_key | Clay Key |
| 365 | cloth_chestpiece | Cloth Chestpiece |
| 366 | cloth_helmet | Cloth Helmet |
| 367 | cloth_pants | Cloth Pants |
| 368 | cloth_shoes | Cloth Shoes |
| 369 | cloth_wristband | Cloth Wristband |
| 370 | coconut | Coconut |
| 371 | coconut_cream_pie | Coconut Cream Pie |
| 372 | coconut_milk | Coconut Milk |
| 373 | cod | Cod |
| 374 | cod_with_thyme | Cod with Thyme |
| 375 | coffee | Coffee |
| 378 | coffee_mug | Coffee Mug |
| 379 | coin_lump | Coin Lump |
| 421 | color_paving_stone | Color Paving Stone |
| 422 | completely_wrong_map | Completely Wrong Map |
| 423 | confiscated_coffee | Confiscated Coffee |
| 424 | construction_fence | Construction Fence |
| 427 | cooking_frying_pan | Frying Pan |
| 429 | cooking_hanging_pans | Hanging Pans |
| 432 | cooking_stockpot | Stockpot |
| 435 | cooking_tea_kettle | Tea Kettle |
| 436 | cooktop_beetle | Cooktop Beetle |
| 437 | copper_armor | Copper Armor |
| 438 | copper_beetle | Copper Nugget Beetle |
| 439 | copper_greaves | Copper Greaves |
| 440 | copper_helmet | Copper Helmet |
| 441 | copper_ingot | Copper Ingot |
| 442 | copper_legplates | Copper Legplates |
| 443 | copper_ring | Copper Ring |
| 444 | coral | Coral |
| 446 | coral_chair | Coral Chair |
| 448 | coral_lamp | Coral Lamp |
| 449 | coral_mantis | Coral Mantis |
| 450 | coral_stone_archway | Coral-Stone Arch |
| 452 | coral_storage_chest | Coral Storage Chest |
| 454 | coral_table | Coral Table |
| 455 | corn | Corn |
| 456 | cornucopia | Cornucopia |
| 457 | cosmetic | MISSING | **DISABLED** |
| 458 | cosmos | Cosmos |
| 462 | cottage_bed | Lovely Cottage Single Bed |
| 461 | cottage_bed_double | Lovely Cottage Double Bed |
| 464 | cottage_chair | Lovely Cottage Chair |
| 466 | cottage_dresser | Lovely Cottage Dresser |
| 468 | cottage_flooring | Lovely Cottage Flooring |
| 470 | cottage_fridge | Lovely Cottage Icebox |
| 472 | cottage_garden_arch | Cottage Garden Arch |
| 475 | cottage_garden_planter | Cottage Garden Planter |
| 477 | cottage_garden_shed | Cottage Garden Shed |
| 480 | cottage_garden_trellis | Cottage Garden Trellis |
| 482 | cottage_garden_workbench | Cottage Garden Workbench |
| 484 | cottage_nightstand | Lovely Cottage Nightstand |
| 486 | cottage_plant_shelf | Lovely Cottage Plant Shelf |
| 488 | cottage_potted_fig | Lovely Cottage Potted Fig |
| 490 | cottage_potted_flowers | Lovely Cottage Potted Flowers |
| 492 | cottage_rug | Lovely Cottage Rug |
| 494 | cottage_table | Lovely Cottage Table |
| 496 | cottage_wall_flowers | Lovely Cottage Wall Flowers |
| 498 | cottage_wall_window | Lovely Cottage Window |
| 500 | cottage_wallpaper | Lovely Cottage Wallpaper |
| 501 | counter_basic_v1 | Basic Counter |
| 502 | counter_basic_v2 | Basic Counter |
| 503 | counter_cabin_cherry | Cabin Counter |
| 504 | counter_cabin_oak | Cabin Counter |
| 505 | counter_cabin_walnut | Cabin Counter |
| 506 | counter_cake_chocolate | Cake Counter |
| 507 | counter_cake_double_chocolate | Cake Counter |
| 508 | counter_cake_strawberry | Cake Counter |
| 509 | counter_cottage_v1 | Lovely Cottage Counter |
| 510 | counter_cottage_v2 | Lovely Cottage Counter |
| 514 | counter_kitchen | Champion's Kitchen Counter |
| 515 | cow_donut | Cow-Shaped Donut |
| 516 | cow_milk | Milk |
| 517 | cow_plushie | Cow Plushie |
| 518 | cozy_kitchen | Level 2 Kitchen |
| 519 | crab | Crab |
| 520 | crab_cakes | Crab Cakes |
| 521 | crab_statue | Crab Statue |
| 527 | craft_fabric_rack | Craft Fabric Rack |
| 533 | craft_rug | Craft Crochet Rug |
| 530 | craft_sewing_tin | Craft Sewing Tin |
| 536 | craft_yarn_basket | Craft Yarn Basket |
| 537 | crafting_scroll | Crafting Scroll: {} | **DISABLED** |
| 538 | cranberry | Cranberry |
| 539 | cranberry_juice | Cranberry Juice |
| 540 | cranberry_orange_scone | Cranberry Orange Scone |
| 541 | crayfish | Crayfish |
| 542 | crayfish_etouffee | Crayfish Etouffee |
| 543 | cream_pet_bed | Cream Pet Bed |
| 544 | cream_pet_dish | Cream Pet Dish |
| 545 | cricket | Cricket |
| 546 | criminal_confession | Criminal Confession |
| 547 | crispy_fried_earthshroom | Crispy Fried Earthshroom |
| 548 | crocus | Crocus |
| 549 | crop_faux_ash_mushroom | Faux Ash Mushroom |
| 550 | crop_faux_basil | Faux Basil |
| 551 | crop_faux_beet | Faux Beet |
| 552 | crop_faux_blue_conch_shell | Faux Blue Conch Shell |
| 553 | crop_faux_breath_of_fire | Faux Breath of Flame |
| 554 | crop_faux_broccoli | Faux Broccoli |
| 555 | crop_faux_burdock_root | Faux Burdock |
| 556 | crop_faux_cabbage | Faux Cabbage |
| 557 | crop_faux_carrot | Faux Carrot |
| 558 | crop_faux_catmint | Faux Catmint |
| 559 | crop_faux_cauliflower | Faux Cauliflower |
| 560 | crop_faux_cave_kelp | Faux Jade Dulse |
| 561 | crop_faux_celosia | Faux Celosia |
| 562 | crop_faux_chestnut | Faux Chestnut |
| 563 | crop_faux_chickpea | Faux Chickpea |
| 564 | crop_faux_chili_pepper | Faux Chili Pepper |
| 565 | crop_faux_chrysanthemum | Faux Chrysanthemum |
| 566 | crop_faux_corn | Faux Corn |
| 567 | crop_faux_cosmos | Faux Cosmos |
| 568 | crop_faux_cranberry | Faux Cranberry |
| 569 | crop_faux_crocus | Faux Crocus |
| 570 | crop_faux_crystal_berries | Faux Crystal Berries |
| 571 | crop_faux_crystal_rose | Faux Crystal Rose |
| 572 | crop_faux_cucumber | Faux Cucumber |
| 573 | crop_faux_daffodil | Faux Daffodil |
| 574 | crop_faux_daikon_radish | Faux Daikon Radish |
| 575 | crop_faux_daisy | Faux Daisy |
| 576 | crop_faux_dandelion | Faux Dandelion |
| 577 | crop_faux_dill | Faux Dill |
| 578 | crop_faux_earthshroom | Faux Earthshroom |
| 579 | crop_faux_fennel | Faux Fennel |
| 580 | crop_faux_fiddlehead | Faux Fiddlehead |
| 581 | crop_faux_flame_pepper | Faux Flame Pepper |
| 582 | crop_faux_fog_orchid | Faux Fog Orchid |
| 583 | crop_faux_frost_lily | Faux Frost Lily |
| 584 | crop_faux_garlic | Faux Garlic |
| 585 | crop_faux_heather | Faux Heather |
| 586 | crop_faux_holly | Faux Holly |
| 587 | crop_faux_horseradish | Faux Horseradish |
| 588 | crop_faux_hot_potato | Faux Hot Potato |
| 589 | crop_faux_ice_block | Faux Ice Block |
| 590 | crop_faux_iris | Faux Iris |
| 591 | crop_faux_jasmine | Faux Jasmine |
| 592 | crop_faux_lava_chestnuts | Faux Lava Chestnuts |
| 593 | crop_faux_lilac | Faux Lilac |
| 594 | crop_faux_marigold | Faux Marigold |
| 595 | crop_faux_middlemist | Faux Middlemist |
| 596 | crop_faux_mines_mussels | Faux Mines Mussels |
| 597 | crop_faux_moon_fruit | Faux Moon Fruit |
| 598 | crop_faux_morel_mushroom | Faux Morel Mushroom |
| 599 | crop_faux_mystery_bag | Faux Magic Seed Plant |
| 600 | crop_faux_narrows_moss | Faux Narrows Moss |
| 601 | crop_faux_nettle | Faux Nettle |
| 602 | crop_faux_night_queen | Faux Night Queen |
| 603 | crop_faux_onion | Faux Onion |
| 604 | crop_faux_oregano | Faux Oregano |
| 605 | crop_faux_oyster_mushroom | Faux Oyster Mushroom |
| 606 | crop_faux_peas | Faux Peas |
| 607 | crop_faux_pineshroom | Faux Pineshroom |
| 608 | crop_faux_pink_scallop_shell | Faux Pink Scallop Shell |
| 609 | crop_faux_poinsettia | Faux Poinsettia |
| 610 | crop_faux_potato | Faux Potato |
| 611 | crop_faux_pumpkin | Faux Pumpkin |
| 612 | crop_faux_rice | Faux Rice |
| 613 | crop_faux_rockroot | Faux Rockroot |
| 614 | crop_faux_rosemary | Faux Rosemary |
| 615 | crop_faux_sage | Faux Sage |
| 616 | crop_faux_sand_dollar | Faux Sand Dollar |
| 617 | crop_faux_sea_grapes | Faux Sea Grapes |
| 618 | crop_faux_sesame | Faux Sesame |
| 619 | crop_faux_shadow_flower | Faux Shadow Flower |
| 620 | crop_faux_shale_grass | Faux Shale Grass |
| 621 | crop_faux_snapdragon | Faux Snapdragon |
| 622 | crop_faux_snow_peas | Faux Snow Peas |
| 623 | crop_faux_snowdrop_anemone | Faux Snowdrop Anemone |
| 624 | crop_faux_spirit_mushroom | Faux Spirit Mushroom |
| 625 | crop_faux_spirula_shell | Faux Spirula Shell |
| 626 | crop_faux_strawberry | Faux Strawberry |
| 627 | crop_faux_sugar_cane | Faux Sugar Cane |
| 628 | crop_faux_sunflower | Faux Sunflower |
| 629 | crop_faux_sweet_potato | Faux Sweet Potato |
| 630 | crop_faux_sweetroot | Faux Sweetroot |
| 631 | crop_faux_tea | Faux Tea |
| 632 | crop_faux_temple_flower | Faux Temple Flower |
| 633 | crop_faux_thorn_vine | Faux Thorn Vine |
| 634 | crop_faux_thyme | Faux Thyme |
| 635 | crop_faux_tide_lettuce | Faux Tide Lettuce |
| 636 | crop_faux_tomato | Faux Tomato |
| 637 | crop_faux_tulip | Faux Tulip |
| 638 | crop_faux_turnip | Faux Turnip |
| 639 | crop_faux_upper_mines_mushroom | Faux Upper Mines Mushroom |
| 640 | crop_faux_viola | Faux Viola |
| 641 | crop_faux_walnut | Faux Walnut |
| 642 | crop_faux_watermelon | Faux Watermelon |
| 643 | crop_faux_wheat | Faux Wheat |
| 644 | crop_faux_wild_leek | Faux Wild Leek |
| 645 | crop_sign_apple | Apple Sign |
| 646 | crop_sign_basil | Basil Sign |
| 647 | crop_sign_beet | Beet Sign |
| 648 | crop_sign_broccoli | Broccoli Sign |
| 649 | crop_sign_burdock_root | Burdock Root Sign |
| 650 | crop_sign_cabbage | Cabbage Sign |
| 651 | crop_sign_carrot | Carrot Sign |
| 652 | crop_sign_catmint | Catmint Sign |
| 653 | crop_sign_cauliflower | Cauliflower Sign |
| 654 | crop_sign_celosia | Celosia Sign |
| 655 | crop_sign_cherry | Cherry Sign |
| 656 | crop_sign_chickpea | Chickpea Sign |
| 657 | crop_sign_chili_pepper | Chili Pepper Sign |
| 658 | crop_sign_chrysanthemum | Chrysanthemum Sign |
| 659 | crop_sign_coconut | Coconut Sign |
| 660 | crop_sign_corn | Corn Sign |
| 661 | crop_sign_cosmos | Cosmos Sign |
| 662 | crop_sign_cranberry | Cranberry Sign |
| 663 | crop_sign_cucumber | Cucumber Sign |
| 664 | crop_sign_daffodil | Daffodil Sign |
| 665 | crop_sign_daikon_radish | Daikon Radish Sign |
| 666 | crop_sign_daisy | Daisy Sign |
| 667 | crop_sign_dill | Dill Sign |
| 668 | crop_sign_frost_lily | Frost Lily Sign |
| 669 | crop_sign_garlic | Garlic Sign |
| 670 | crop_sign_heather | Heather Sign |
| 671 | crop_sign_iris | Iris Sign |
| 672 | crop_sign_jasmine | Jasmine Sign |
| 673 | crop_sign_lemon | Lemon Sign |
| 674 | crop_sign_lilac | Lilac Sign |
| 675 | crop_sign_marigold | Marigold Sign |
| 676 | crop_sign_moon_fruit | Moon Fruit Sign |
| 677 | crop_sign_mystery_bag | Magic Seed Plant Sign |
| 678 | crop_sign_night_queen | Night queen Sign |
| 679 | crop_sign_onion | Onion Sign |
| 680 | crop_sign_orange | Orange Sign |
| 681 | crop_sign_oregano | Oregano Sign |
| 682 | crop_sign_peach | Peach Sign |
| 683 | crop_sign_pear | Pear Sign |
| 684 | crop_sign_peas | Peas Sign |
| 685 | crop_sign_poinsettia | Poinsettia Sign |
| 686 | crop_sign_pomegranate | Pomegranate Sign |
| 687 | crop_sign_potato | Potato Sign |
| 688 | crop_sign_pumpkin | Pumpkin Sign |
| 689 | crop_sign_rice | Rice Sign |
| 690 | crop_sign_rosemary | Rosemary Sign |
| 691 | crop_sign_sage | Sage Sign |
| 692 | crop_sign_snapdragon | Snapdragon Sign |
| 693 | crop_sign_snow_peas | Snow Peas Sign |
| 694 | crop_sign_snowdrop_anemone | Snowdrop Anemone Sign |
| 695 | crop_sign_strawberry | Strawberry Sign |
| 696 | crop_sign_sugar_cane | Sugar Cane Sign |
| 697 | crop_sign_sunflower | Sunflower Sign |
| 698 | crop_sign_sweet_potato | Sweet Potato Sign |
| 699 | crop_sign_tea | Tea Sign |
| 700 | crop_sign_temple_flower | Temple Flower Sign |
| 701 | crop_sign_thyme | Thyme Sign |
| 702 | crop_sign_tomato | Tomato Sign |
| 703 | crop_sign_tulip | Tulip Sign |
| 704 | crop_sign_turnip | Turnip Sign |
| 705 | crop_sign_viola | Viola Sign |
| 706 | crop_sign_watermelon | Watermelon Sign |
| 707 | crop_sign_wheat | Wheat Sign |
| 708 | crucian_carp | Crucian Carp |
| 709 | crunchy_chickpeas | Crunchy Chickpeas |
| 710 | crystal | Crystal |
| 711 | crystal_apple | Crystal Apple |
| 712 | crystal_berries | Crystal Berries |
| 713 | crystal_berry_pie | Crystal Berry Pie |
| 714 | crystal_boots | Crystal Greaves |
| 715 | crystal_caterpillar | Crystal Caterpillar |
| 716 | crystal_chestpiece | Crystal Chestpiece |
| 717 | crystal_helmet | Crystal Helmet |
| 718 | crystal_ring | Crystal Ring |
| 719 | crystal_rose | Crystal Rose |
| 722 | crystal_string_lights | Crystal Wall String Lights |
| 723 | crystal_tassets | Crystal Tassets |
| 724 | crystal_wing_moth | Crystal Wing Moth |
| 725 | crystalline_cricket | Crystalline Cricket |
| 726 | cucumber | Cucumber |
| 727 | cucumber_salad | Cucumber Salad |
| 728 | cucumber_sandwich | Cucumber Sandwich |
| 729 | cup_of_tea | Tea with Lemon |
| 730 | curry_powder | Curry Powder |
| 731 | dace | Dace |
| 732 | daffodil | Daffodil |
| 733 | daikon_radish | Daikon Radish |
| 734 | daisy | Daisy |
| 735 | dandelion | Dandelion |
| 736 | dart | Dart |
| 737 | date_photo_bath | Bath Date Photo |
| 738 | date_photo_beach | Beach Date Photo |
| 739 | date_photo_deep_woods | Picnic Date Photo |
| 740 | date_photo_gem_cutting | Gem Cutting Date Photo |
| 741 | date_photo_harvest | Harvest Dance Date Photo |
| 742 | date_photo_inn | Inn Date Photo |
| 743 | date_photo_park | Park Date Photo |
| 744 | date_photo_shooting_star | Shooting Star Date Photo |
| 745 | deep_earthworm | Deep Earthworm |
| 746 | deep_sea_soup | Deep Sea Soup |
| 747 | deluxe_curry | Deluxe Curry |
| 748 | deluxe_hay | Deluxe Large Animal Feed |
| 749 | deluxe_icebox_blue | Deluxe Icebox |
| 750 | deluxe_icebox_green | Deluxe Icebox |
| 751 | deluxe_icebox_pink | Deluxe Icebox |
| 752 | deluxe_icebox_white | Deluxe Icebox |
| 753 | deluxe_icebox_yellow | Deluxe Icebox |
| 754 | deluxe_small_animal_feed | Deluxe Small Animal Feed |
| 755 | deluxe_storage_chest_aqua | Deluxe Storage Chest |
| 756 | deluxe_storage_chest_black | Deluxe Storage Chest |
| 757 | deluxe_storage_chest_blue | Deluxe Storage Chest |
| 758 | deluxe_storage_chest_dark_brown | Deluxe Storage Chest |
| 759 | deluxe_storage_chest_gold | Deluxe Storage Chest |
| 760 | deluxe_storage_chest_gray | Deluxe Storage Chest |
| 761 | deluxe_storage_chest_green | Deluxe Storage Chest |
| 762 | deluxe_storage_chest_light_brown | Deluxe Storage Chest |
| 763 | deluxe_storage_chest_orange | Deluxe Storage Chest |
| 764 | deluxe_storage_chest_pink | Deluxe Storage Chest |
| 765 | deluxe_storage_chest_purple | Deluxe Storage Chest |
| 766 | deluxe_storage_chest_red | Deluxe Storage Chest |
| 767 | deluxe_storage_chest_white | Deluxe Storage Chest |
| 768 | dense_water | Dense Water |
| 769 | deviled_eggs | Deviled Eggs |
| 770 | diamond_backed_mirror | Diamond-Backed Mirror |
| 771 | diamond_beetle | Diamond Beetle |
| 772 | dill | Dill |
| 773 | dirt_clear | Dirt Tile Clear |
| 774 | dirt_dot_one | Dirt Tile 1 Dot |
| 775 | dirt_dot_three | Dirt Tile 3 Dots |
| 776 | dirt_dot_two | Dirt Tile 2 Dots |
| 777 | dog_treat | Dog Treat |
| 778 | dollhouse_bench | Dollhouse Bench |
| 779 | dough_lad_doll | Doughlad Doll |
| 783 | dragon_altar | Dragon Claw Altar |
| 784 | dragon_claw | Dragon Claw |
| 2101 | dragon_fairy | Dragon Fairy |
| 785 | dragon_forged_bracelet | Dragon-Forged Bracelet |
| 786 | dragon_horn_beetle | Dragon Horn Beetle |
| 787 | dragon_horn_mushroom | Dragon Horn Mushroom |
| 788 | dragon_horn_mushroom_with_thyme | Dragon Horn Mushroom with Thyme |
| 789 | dragon_pact_tablet | Dragon Pact Tablet |
| 792 | dragon_priestess_fountain | Dragon Priestess Fountain |
| 793 | dragon_scale | Dragon Scale |
| 795 | dragon_statue_replica | Dragon Statue |
| 796 | dragonfly | Dragonfly |
| 797 | dragonsworn_chest_and_pants | Dragonsworn Chest and Pants |
| 798 | dragonsworn_greaves | Dragonsworn Greaves |
| 799 | dragonsworn_helmet | Dragonsworn Helmet |
| 800 | dried_squid | Dried Squid |
| 801 | duck_egg | Duck Egg |
| 802 | duck_feather | Duck Feather |
| 803 | duck_mayonnaise | Duck Mayonnaise |
| 804 | duck_plushie | Duck Plushie |
| 805 | dungeon_fountain_health | Fountain Health 33 | **GLITCHED** |
| 806 | dungeon_fountain_stamina | Fountain Stamina 33 | **GLITCHED** |
| 807 | earth_eel | Earth Eel |
| 808 | earth_infused_stone | Earth-Infused Stone |
| 809 | earth_tablet_rubbing | Earth Tablet Rubbing |
| 810 | earthshroom | Earthshroom |
| 2110 | echo_mint | Echo Mint |
| 811 | egg | Chicken Egg |
| 817 | emerald_bed | Emerald Double Bed |
| 815 | emerald_chair | Emerald Chair |
| 818 | emerald_horned_charger | Emerald-Horned Charger |
| 820 | emerald_nightstand | Emerald Nightstand |
| 822 | emerald_rug | Emerald Rug |
| 824 | emerald_wallpaper | Emerald Wallpaper |
| 825 | espresso | Espresso |
| 828 | espresso_cup | Espresso Cup |
| 829 | espresso_machine | Espresso Machine |
| 830 | essence_blossom | Essence Blossom |
| 831 | essence_drop | Essence Drop |
| 832 | essence_stone_giant | Giant Essence Stone |
| 833 | essence_stone_large | Large Essence Stone |
| 834 | essence_stone_medium | Medium Essence Stone |
| 835 | essence_stone_small | Small Essence Stone |
| 836 | essence_stone_tiny | Tiny Essence Stone |
| 839 | explorer_bed | Explorer Bed |
| 842 | explorer_chair | Explorer Chair |
| 845 | explorer_double_bed | Explorer Double Bed |
| 848 | explorer_flooring | Explorer Flooring |
| 851 | explorer_globe | Explorer Globe |
| 854 | explorer_lamp | Explorer Lamp |
| 857 | explorer_rug | Explorer Rug |
| 860 | explorer_trunk_table | Explorer Trunk Table |
| 863 | explorer_wallpaper | Explorer Wallpaper |
| 864 | extra_large_decorative_pumpkin_black | Extra Large Decorative Pumpkin |
| 865 | extra_large_decorative_pumpkin_green | Extra Large Decorative Pumpkin |
| 866 | extra_large_decorative_pumpkin_orange | Extra Large Decorative Pumpkin |
| 867 | extra_large_decorative_pumpkin_purple | Extra Large Decorative Pumpkin |
| 868 | extra_large_decorative_pumpkin_white | Extra Large Decorative Pumpkin |
| 869 | extra_large_decorative_pumpkin_yellow | Extra Large Decorative Pumpkin |
| 870 | faceted_rock_gem | Faceted Rock Gem |
| 871 | fairy_bee | Fairy Bee |
| 872 | fairy_syrup | Fairy Syrup |
| 873 | fake_essence | Essence |
| 878 | fall_crop_sign_scroll_bundle | Fall Crop Sign Scroll Bundle |
| 882 | fall_double_bed | Fall Double Bed |
| 877 | fall_single_bed | Fall Single Bed |
| 886 | fall_table | Fall Dining Table |
| 887 | family_crest_pendant | Family Crest Pendant |
| 888 | farm_bridge | Starter Farm Bridge |
| 889 | farm_house_calendar | Farmhouse Calendar |
| 890 | fast_food | Fast Food |
| 891 | feather | Rooster Feather |
| 892 | fennel | Fennel |
| 893 | fiber | Fiber |
| 894 | fiddlehead | Fiddlehead |
| 895 | field_clear | Grass Tile Clear |
| 896 | field_flower | Grass Tile Flower |
| 897 | field_flower_two | Grass Tile 2 Flowers |
| 898 | field_grass | Grass Tile Blades |
| 899 | field_grass_two | Field Grass 2 Blades |
| 900 | fire_crystal | Fire Crystal |
| 901 | fire_snake_oil | Fire Snake Oil |
| 902 | fire_tablet_rubbing | Fire Tablet Rubbing |
| 903 | fire_wasp | Fire Wasp |
| 904 | firefly | Firefly |
| 909 | firefly_wall_string_lights | Firefly Wall String Lights |
| 910 | firesail_fish | Firesail Fish |
| 911 | fish_plush_toy | Fish Plush Toy |
| 912 | fish_skewer | Fish Skewers |
| 913 | fish_stew | Fish Stew |
| 914 | fish_tacos | Fish Tacos |
| 916 | fishing_bed | Fishing Bed |
| 918 | fishing_chair | Fishing Chair |
| 920 | fishing_double_bed | Fishing Double Bed |
| 922 | fishing_flooring | Fishing Flooring |
| 924 | fishing_lamp | Lighthouse Lamp |
| 926 | fishing_oar_wall_decor | Oar Wall Decor |
| 929 | fishing_rod_copper | Copper Fishing Rod |
| 930 | fishing_rod_gold | Gold Fishing Rod |
| 931 | fishing_rod_iron | Iron Fishing Rod |
| 932 | fishing_rod_mistril | Mistril Fishing Rod |
| 933 | fishing_rod_silver | Silver Fishing Rod |
| 934 | fishing_rod_worn | Worn Fishing Rod |
| 936 | fishing_rug | Fishing Rug |
| 938 | fishing_table | Fishing Table |
| 940 | fishing_tank | Fish Tank |
| 942 | fishing_trophy_shelf | Fishing Trophy Wall Shelf |
| 928 | fishing_wall_plaque | Faux Fish Wall Plaque |
| 944 | fishing_wallpaper | Fishing Wallpaper |
| 946 | fishing_window | Porthole Window |
| 947 | flame_pepper | Flame Pepper |
| 948 | flathead_catfish | Flathead Catfish |
| 949 | flint_arrowhead | Flint Arrowhead |
| 950 | floral_tea | Floral Tea |
| 951 | flour | Flour |
| 952 | flower_crown_beetle | Flower Crown Beetle |
| 953 | fog_orchid | Fog Orchid |
| 954 | forest_perch | Forest Perch |
| 955 | fossilized_egg | Fossilized Egg |
| 956 | fossilized_mandrake_root | Fossilized Mandrake Root |
| 957 | freshwater_eel | Freshwater Eel |
| 958 | freshwater_oyster | Freshwater Oyster |
| 959 | fried_rice | Fried Rice |
| 960 | frog | Frog |
| 961 | frost_flutter_butterfly | Frost Flutter Butterfly |
| 962 | frost_lily | Frost Lily |
| 963 | fuzzy_moth | Fuzzy Moth |
| 964 | gar | Gar |
| 965 | garlic | Garlic |
| 966 | garlic_bread | Garlic Bread |
| 967 | gathering_basket | Gathering Basket |
| 968 | gazpacho | Gazpacho |
| 969 | gem_shard_caterpillar | Gem Shard Caterpillar |
| 970 | giant_fish_scale | Giant Fish Scale |
| 971 | giant_koi | Giant Koi |
| 972 | giant_tilapia | Giant Tilapia |
| 973 | glass | Glass |
| 974 | glass_absinthe | Absinthe Glass |
| 975 | glass_cocktail | Cocktail Glass |
| 976 | glass_lemonade | Lemonade Glass |
| 977 | glass_milk | Milk Glass |
| 978 | glass_water | Water Glass |
| 979 | glass_whisky | Whisky Glass |
| 980 | glowberry | Glowberry |
| 981 | glowberry_cookies | Glowberry Cookies |
| 982 | glowing_mushroom | Glowing Mushroom |
| 983 | goby | Goby |
| 984 | gold_alpaca_trophy | Gold Alpaca Trophy |
| 985 | gold_armor | Gold Armor |
| 986 | gold_cow_trophy | Gold Cow Trophy |
| 987 | gold_floor_mirror | Gold Floor Mirror |
| 988 | gold_greaves | Gold Greaves |
| 989 | gold_helmet | Gold Helmet |
| 990 | gold_horse_trophy | Gold Horse Trophy |
| 991 | gold_ingot | Gold Ingot |
| 992 | gold_legplates | Gold Legplates |
| 993 | gold_ring | Gold Ring |
| 994 | gold_sheep_trophy | Gold Sheep Trophy |
| 995 | gold_wall_mirror | Gold Wall Mirror |
| 996 | golden_alpaca_wool | Golden Alpaca Wool |
| 997 | golden_bristle | Golden Bristle |
| 998 | golden_bull_horn | Golden Bull Horn |
| 999 | golden_butter | Golden Butter |
| 1000 | golden_cheese | Golden Cheese |
| 1001 | golden_cheesecake | Golden Cheesecake |
| 1002 | golden_cookies | Golden Cookies |
| 1003 | golden_cow_milk | Golden Milk |
| 1004 | golden_duck_egg | Golden Duck Egg |
| 1005 | golden_duck_feather | Golden Duck Feather |
| 1006 | golden_duck_mayonnaise | Golden Duck Mayonnaise |
| 1007 | golden_egg | Golden Egg |
| 1008 | golden_feather | Golden Rooster Feather |
| 1009 | golden_horse_hair | Golden Horse Hair |
| 1010 | golden_mayonnaise | Golden Mayonnaise |
| 1011 | golden_rabbit_wool | Golden Rabbit Wool |
| 1012 | golden_sheep_wool | Golden Sheep Wool |
| 1013 | golden_shiner | Golden Shiner |
| 1014 | goldfish | Goldfish |
| 1015 | grape_juice | Grape Juice |
| 1016 | grass_seed | Grass Seed |
| 1017 | grasshopper | Grasshopper |
| 1018 | grayling | Grayling |
| 1019 | green_bottle | Green Bottle |
| 1020 | green_tea | Green Tea |
| 1021 | grilled_cheese | Grilled Cheese |
| 1022 | grilled_corn | Grilled Corn |
| 1023 | grilled_eel_rice_bowl | Grilled Eel Rice Bowl |
| 1024 | grouper | Grouper |
| 1027 | gryphon_statue_replica | Gryphon Statue Replica |
| 1028 | halibut | Halibut |
| 1029 | hard_boiled_egg | Hard Boiled Egg |
| 1030 | hard_wood | Hard Wood |
| 1031 | hardened_essence | Hardened Essence |
| 1032 | harvest_day_pie | Special Pie | **GLITCHED** |
| 1033 | harvest_festival_arch | Harvest Festival Arch |
| 1034 | harvest_festival_centerpiece | Harvest Festival Centerpiece |
| 1035 | harvest_plate | Harvest Plate |
| 1037 | haunted_attic_armoire | Haunted Attic Armoire |
| 1039 | haunted_attic_bed | Haunted Attic Bed |
| 1041 | haunted_attic_candle_cluster | Melted Candle Cluster |
| 1043 | haunted_attic_candle_single | Melted Candle |
| 1045 | haunted_attic_chair | Haunted Attic Chair |
| 1047 | haunted_attic_double_bed | Haunted Attic Double Bed |
| 1049 | haunted_attic_dress_form | Haunted Attic Dress Form |
| 1051 | haunted_attic_flooring | Haunted Attic Flooring |
| 1053 | haunted_attic_nightstand | Haunted Attic Nightstand |
| 1055 | haunted_attic_rocking_chair | Haunted Attic Rocking Chair |
| 1057 | haunted_attic_table | Haunted Attic Table |
| 1059 | haunted_attic_wall_cobweb_left | Cobweb Left |
| 1061 | haunted_attic_wall_cobweb_right | Cobweb Right |
| 1063 | haunted_attic_wall_shelf | Haunted Attic Wall Shelf |
| 1065 | haunted_attic_wall_window | Haunted Attic Window |
| 1067 | haunted_attic_wallpaper | Haunted Attic Wallpaper |
| 1068 | hay | Hay |
| 1069 | haydens_weathervane | Hayden's Weathervane |
| 1070 | heal_syrup | Healing Syrup |
| 1071 | heart_crystal | Heart Crystal |
| 1072 | heart_shaped_alpaca_treat | Heart Shaped Alpaca Treat |
| 1073 | heart_shaped_capybara_treat | Heart Shaped Capybara Treat |
| 1074 | heart_shaped_chicken_treat | Heart Shaped Chicken Treat |
| 1075 | heart_shaped_cow_treat | Heart Shaped Cow Treat |
| 1076 | heart_shaped_duck_treat | Heart Shaped Duck Treat |
| 1077 | heart_shaped_horse_treat | Heart Shaped Horse Treat |
| 1078 | heart_shaped_rabbit_treat | Heart Shaped Rabbit Treat |
| 1079 | heart_shaped_sheep_treat | Heart Shaped Sheep Treat |
| 1080 | heather | Heather |
| 1081 | heavy_mist | Heavy Mist |
| 1082 | herb_butter_pasta | Herb Butter Pasta |
| 1083 | herb_salad | Herb Salad |
| 1084 | hermit_crab | Hermit Crab |
| 1085 | hermit_snail | Hermit Snail |
| 1088 | hero_shield | Champion's Shield |
| 1089 | heros_ring | Hero's Ring |
| 1090 | herring | Herring |
| 1093 | herringbone_tile_floor | Herringbone Tile Floor |
| 1094 | hoe_copper | Copper Hoe |
| 1095 | hoe_gold | Gold Hoe |
| 1096 | hoe_iron | Iron Hoe |
| 1097 | hoe_mistril | Mistril Hoe |
| 1098 | hoe_silver | Silver Hoe |
| 1099 | hoe_worn | Worn Hoe |
| 1100 | holly | Holly |
| 1101 | honey | Honey |
| 1102 | honey_curry | Honey Curry |
| 1103 | horse_hair | Horse Hair |
| 1104 | horse_mackerel | Horse Mackerel |
| 1105 | horse_plushie | Horse Plushie |
| 1106 | horse_potion | MISSING | **GLITCHED** |
| 1107 | horseradish | Horseradish |
| 1108 | horseradish_salmon | Horseradish Salmon |
| 1109 | hot_cocoa | Hot Chocolate |
| 1110 | hot_potato | Hot Potato |
| 1111 | hot_toddy | Hot Toddy |
| 1112 | humble_pie | Humble Pie |
| 1113 | hummingbird_hawk_moth | Hummingbird Hawk Moth |
| 1114 | hydrangea | Hydrangea |
| 1115 | ice_block | Ice Block |
| 1116 | ice_cream_sundae | Ice Cream Sundae |
| 1117 | ice_snake_oil | Ice Snake Oil |
| 1118 | iced_coffee | Iced Coffee |
| 1119 | icehopper | Icehopper |
| 1120 | inchworm | Inchworm |
| 1121 | incredibly_hot_pot | Incredibly Hot Pot |
| 1122 | inn_candle | Inn Candle |
| 1123 | inn_counter | Inn Counter |
| 1124 | inn_cross_stitch | Inn Cross Stitch |
| 1125 | inn_kitchen_shelves | Inn Kitchen Shelves |
| 1126 | inn_menu | Inn Menu |
| 1127 | inn_stool | Inn Stool |
| 1128 | inn_tile_flooring | Inn Tile Flooring |
| 1129 | inn_wooden_flooring | Inn Wooden Flooring |
| 1132 | insect_flooring | Insect Flooring |
| 1135 | insect_rug | Insect Rug |
| 1138 | insect_wallpaper | Insect Wallpaper |
| 1139 | iris | Iris |
| 1140 | iron_armor | Iron Armor |
| 1141 | iron_greaves | Iron Greaves |
| 1142 | iron_helmet | Iron Helmet |
| 1143 | iron_ingot | Iron Ingot |
| 1144 | iron_legplates | Iron Legplates |
| 1145 | iron_ring | Iron Ring |
| 1146 | jam_sandwich | Jam Sandwich |
| 1147 | jar_large | Large Jar |
| 1148 | jar_medium | Medium Jar |
| 1149 | jar_pair | Pair of Jars |
| 1150 | jar_small | Small Jar |
| 1151 | jasmine | Jasmine |
| 1152 | jasmine_tea | Jasmine Tea |
| 1153 | jellyfish | Jellyfish |
| 1154 | jewel_beetle | Jewel Beetle |
| 1155 | journal | Diary |
| 1156 | killifish | Killifish |
| 1157 | king_crab | King Crab |
| 1158 | kitchen_garlic_braid | Garlic Braid |
| 1159 | kitchen_hanging_onion_basket | Hanging Onion Basket |
| 1161 | kitchen_herb_planter | Herb Planter |
| 1165 | kitchen_mixing_bowls | Mixing Bowls |
| 1167 | kitchen_oil_and_vinegar_basket | Oil & Vinegar Basket |
| 1168 | kitchen_salt_and_pepper_shakers | Salt & Pepper Shakers |
| 1172 | kitchen_scale | Kitchen Scale |
| 1176 | kitchen_utensil_holder | Utensil Holder |
| 1177 | koi | Koi |
| 1178 | ladder | House Ladder |
| 1179 | ladybug | Ladybug |
| 1182 | ladybug_table | Ladybug Table |
| 1183 | lake_chub | Lake Chub |
| 1184 | lake_trout | Lake Trout |
| 1185 | lamprey | Lamprey |
| 1186 | lantern_moth | Lantern Moth |
| 1187 | large_animal_ball_court | Large Animal Ball Court |
| 1188 | large_barn_black_blueprint | Large White Barn Blueprint |
| 1189 | large_barn_red_blueprint | Large Red Barn Blueprint |
| 1190 | large_coop_black_blueprint | Large White Coop Blueprint |
| 1191 | large_coop_red_blueprint | Large Red Coop Blueprint |
| 1192 | large_decorative_pumpkin_black | Large Decorative Pumpkin |
| 1193 | large_decorative_pumpkin_green | Large Decorative Pumpkin |
| 1194 | large_decorative_pumpkin_orange | Large Decorative Pumpkin |
| 1195 | large_decorative_pumpkin_purple | Large Decorative Pumpkin |
| 1196 | large_decorative_pumpkin_white | Large Decorative Pumpkin |
| 1197 | large_decorative_pumpkin_yellow | Large Decorative Pumpkin |
| 1205 | large_fluffy_rug | Large Fluffy Rug |
| 1206 | large_greenhouse_white_blueprint | Large White Greenhouse Blueprint |
| 1207 | large_greenhouse_wood_blueprint | Large Wood Greenhouse Blueprint |
| 1208 | large_mob_coin | coin | **GLITCHED** |
| 1211 | large_paving_stone | Large Paving Stone |
| 1212 | large_pouch | Bag Upgrade |
| 1213 | large_stone_bundle | Large Bundle of Stone |
| 1214 | large_wood_bundle | Large Bundle of Wood |
| 1215 | latte | Latte |
| 1224 | lava_caves_bed | Obsidian Double Bed |
| 1222 | lava_caves_chair | Obsidian Chair |
| 1230 | lava_caves_chest | Obsidian Storage Chest |
| 1216 | lava_caves_diamond_rock | Ornamental Diamond Rock |
| 1226 | lava_caves_fence | Obsidian Fence |
| 1228 | lava_caves_floor_lamp | Obsidian Floor Lamp |
| 1217 | lava_caves_flooring | Lava Rock Floor |
| 1218 | lava_caves_gold_node | Ornamental Gold Node |
| 1233 | lava_caves_stone_arch | Lava Rock Arch |
| 1232 | lava_caves_table | Obsidian Table |
| 1234 | lava_caves_wallpaper | Lava Rock Wall |
| 1235 | lava_chestnuts | Lava Chestnuts |
| 1236 | lava_piranha | Lava Piranha |
| 1237 | lava_snail | Lava Snail |
| 1238 | lavender_tea | Lavender Tea |
| 1239 | leaf_fish | Leaf Fish |
| 1240 | leafhopper | Leafhopper |
| 1241 | lemon | Lemon |
| 1242 | lemon_pie | Lemon Pie |
| 1243 | lemonade | Lemonade |
| 1244 | lightning_dragonfly | Lightning Dragonfly |
| 1245 | lightning_fish | Lightning Fish |
| 1246 | lilac | Lilac |
| 1247 | loach | Loach |
| 1248 | loaded_baked_potato | Loaded Baked Potato |
| 1249 | loam_caterpillar | Loam Caterpillar |
| 1250 | lobster | Lobster |
| 1251 | lobster_roll | Lobster Roll |
| 1252 | lost_crown_of_aldaria | Lost Crown of Aldaria |
| 1253 | lovely_cottage_scroll_bundle | Lovely Cottage Scroll Bundle |
| 1254 | lovely_seashell | Lovely Seashell |
| 1255 | low_tide_flooring | Low Tide Flooring |
| 1256 | luna_moth | Luna Moth |
| 1257 | lurid_colored_drink | MISSING | **GLITCHED** |
| 1258 | mackerel | Mackerel |
| 1259 | mackerel_sashimi | Mackerel Sashimi |
| 1260 | magma_beetle | Magma Beetle |
| 1261 | mana_potion | Mana Potion |
| 1262 | maples_drawing | Maple's Aldarian Star Drawing |
| 1263 | marigold | Marigold |
| 1264 | marmalade | Marmalade |
| 1265 | mayonnaise | Mayonnaise |
| 1266 | medium_barn_black_blueprint | Medium White Barn Blueprint |
| 1267 | medium_barn_red_blueprint | Medium Red Barn Blueprint |
| 1268 | medium_coop_black_blueprint | Medium White Coop Blueprint |
| 1269 | medium_coop_red_blueprint | Medium Red Coop Blueprint |
| 1270 | medium_decorative_pumpkin_black | Medium Decorative Pumpkin |
| 1271 | medium_decorative_pumpkin_green | Medium Decorative Pumpkin |
| 1272 | medium_decorative_pumpkin_orange | Medium Decorative Pumpkin |
| 1273 | medium_decorative_pumpkin_purple | Medium Decorative Pumpkin |
| 1274 | medium_decorative_pumpkin_white | Medium Decorative Pumpkin |
| 1275 | medium_decorative_pumpkin_yellow | Medium Decorative Pumpkin |
| 1276 | medium_mob_coin | coin | **GLITCHED** |
| 1282 | mermaid_bed | Mermaid Double Bed |
| 1283 | mermaids_comb | Mermaid's Comb |
| 1284 | metal_leaf | Metal Leaf |
| 1285 | meteorite | Meteorite |
| 1286 | middlemist | Middlemist |
| 1287 | mine_cricket | Mine Cricket |
| 1298 | minecart | Minecart |
| 1288 | miners_copper_node | Ornamental Copper Node |
| 1290 | miners_crate_chair | Mines Crate Chair |
| 1292 | miners_crate_chest | Mines Storage Chest |
| 1294 | miners_floor_lamp | Mines Floor Lamp |
| 1295 | miners_flooring | Mines Flooring |
| 1296 | miners_helmet | Miner's Helmet |
| 1299 | miners_mushroom_stew | Miner's Mushroom Stew |
| 1300 | miners_pickaxe | Miner's Pickaxe |
| 1301 | miners_ruby_rock | Ornamental Ruby Rock |
| 1302 | miners_rucksack | Miner's Rucksack |
| 1304 | miners_shelves | Mines Shelf |
| 1305 | miners_slab | Miner's Slab |
| 1307 | miners_wall_lamp | Mines Wall Lamp |
| 1308 | miners_wallpaper | Mines Wall |
| 1309 | mines_mussels | Mines Mussels |
| 1310 | mines_wooden_arch | Mines Arch |
| 1311 | mini_whale_shark | Mini-Whale Shark |
| 1312 | minnow | Minnow |
| 1313 | mint_gimlet | Mint Gimlet |
| 1314 | mist_bed_v1 | Mist Bed |
| 1315 | mist_bed_v2 | Mist Bed |
| 1316 | mist_bed_v3 | Mist Bed |
| 1317 | mist_bed_v4 | Mist Bed |
| 1318 | mist_crystal | Mist Crystal |
| 1319 | mist_cushion_v1 | Mist Cushion |
| 1320 | mist_cushion_v2 | Mist Cushion |
| 1321 | mist_cushion_v3 | Mist Cushion |
| 1322 | mist_cushion_v4 | Mist Cushion |
| 1323 | mist_double_bed_v1 | Mist Double Bed |
| 1324 | mist_double_bed_v2 | Mist Double Bed |
| 1325 | mist_double_bed_v3 | Mist Double Bed |
| 1326 | mist_double_bed_v4 | Mist Double Bed |
| 1327 | mist_flute | Mist Flute |
| 1328 | mist_held_item | Invalid Item | **GLITCHED** |
| 1329 | mist_lamp_v1 | Mist Lamp |
| 1330 | mist_lamp_v2 | Mist Lamp |
| 1331 | mist_lamp_v3 | Mist Lamp |
| 1332 | mist_lamp_v4 | Mist Lamp |
| 1333 | mist_pet_bed_v1 | Mist Pet Bed |
| 1334 | mist_pet_bed_v2 | Mist Pet Bed |
| 1335 | mist_pet_bed_v3 | Mist Pet Bed |
| 1336 | mist_pet_bed_v4 | Mist Pet Bed |
| 1337 | mist_rug_v1 | Mist Rug |
| 1338 | mist_rug_v2 | Mist Rug |
| 1339 | mist_rug_v3 | Mist Rug |
| 1340 | mist_rug_v4 | Mist Rug |
| 1341 | mist_scroll | Mist Scroll |
| 1342 | mist_storage_chest_v1 | Mist Storage Chest |
| 1343 | mist_storage_chest_v2 | Mist Storage Chest |
| 1344 | mist_storage_chest_v3 | Mist Storage Chest |
| 1345 | mist_storage_chest_v4 | Mist Storage Chest |
| 1346 | mistmoth | Mistmoth |
| 1347 | mistria_history_book | Mistria History Book |
| 1348 | mistria_history_scroll | Caldosian Decorative Scroll |
| 1349 | mistria_wall_map | Mistria Wall Map |
| 1350 | mistril_ingot | Mistril Ingot |
| 1351 | misty_black_mirror | Misty Black Mirror |
| 1352 | misty_feather_quill | Misty Feather Quill |
| 1353 | mixed_fruit_juice | Mixed Fruit Juice |
| 1354 | mob_coin | coin | **GLITCHED** |
| 1355 | mocha | Mocha |
| 1356 | monarch_butterfly | Monarch Butterfly |
| 1357 | monster_cookie | Monster Cookies |
| 1358 | monster_core | Monster Core |
| 1359 | monster_fang | Monster Fang |
| 1360 | monster_hoop | Monster Hoop |
| 1361 | monster_horn | Monster Horn |
| 1362 | monster_mash | Monster Mash |
| 1363 | monster_powder | Monster Powder |
| 1364 | monster_shell | Monster Shell |
| 1365 | monster_wing | Monster Wing |
| 1366 | mont_blanc | Mont Blanc |
| 1367 | moon_fruit | Moon Fruit |
| 1368 | moon_fruit_cake | Moon Fruit Cake |
| 1371 | moon_gate | Moon Gate |
| 1372 | morel_mushroom | Morel Mushroom |
| 1373 | mote_firefly | Mote Firefly |
| 1376 | moth_lamp | Moth Lamp |
| 1377 | mournful_clown_painting | Mournful Clown Painting |
| 1378 | mullet | Mullet |
| 1379 | mushroom_brew | Mushroom Brew |
| 1380 | mushroom_rice | Mushroom Rice |
| 1381 | mushroom_steak_dinner | Mushroom Steak Dinner |
| 1382 | muskie | Muskie |
| 1383 | muttering_cube | Muttering Cube |
| 1384 | narrows_moss | Narrows Moss |
| 1385 | net_copper | Copper Net |
| 1386 | net_gold | Gold Net |
| 1387 | net_iron | Iron Net |
| 1388 | net_mistril | Mistril Net |
| 1389 | net_silver | Silver Net |
| 1390 | net_worn | Worn Net |
| 1391 | nettle | Nettle |
| 1392 | newt | Newt |
| 1393 | night_queen | Night Queen |
| 1394 | noodles | Noodles |
| 1396 | notepad | Notepad |
| 1397 | oarfish | Oarfish |
| 1403 | obelisk | Obelisk |
| 1404 | obsidian | Obsidian |
| 1405 | obsidian_blade | Obsidian Blade |
| 1406 | ocarina_sprite_statue | Animal Sprite Statue |
| 1407 | ocean_sunfish | Ocean Sunfish |
| 1408 | octopus | Octopus |
| 1409 | octopus_plushie | Octopus Plushie |
| 1410 | oil | Oil |
| 1411 | omelet | Omelet |
| 1412 | onion | Onion |
| 1413 | onion_soup | Onion Soup |
| 1414 | orange | Orange |
| 1415 | orange_juice | Orange Juice |
| 1416 | orchid_mantis | Orchid Mantis |
| 1417 | ore_copper | Copper Ore |
| 1418 | ore_diamond | Diamond |
| 1419 | ore_emerald | Emerald |
| 1420 | ore_gold | Gold Ore |
| 1421 | ore_iron | Iron Ore |
| 1422 | ore_mistril | Mistril Ore |
| 1423 | ore_pink_diamond | Pink Diamond |
| 1424 | ore_ruby | Ruby |
| 1425 | ore_sapphire | Sapphire |
| 1426 | ore_silver | Silver Ore |
| 1427 | ore_stone | Stone |
| 1428 | oregano | Oregano |
| 1429 | ornamental_iron_node | Ornamental Iron Node |
| 1430 | ornamental_sapphire_rock | Ornamental Sapphire Rock |
| 1433 | ornate_flagstone | Ornate Flagstone |
| 1436 | ornate_rug_large_rectangle | Ornate Large Rectangle Rug |
| 1439 | ornate_rug_large_round | Ornate Large Round Rug |
| 1442 | ornate_rug_large_square | Ornate Large Square Rug |
| 1445 | ornate_rug_medium_round | Ornate Medium Round Rug |
| 1448 | ornate_rug_medium_square | Ornate Medium Square Rug |
| 1451 | ornate_rug_small | Ornate Small Rug |
| 1452 | oyster_mushroom | Oyster Mushroom |
| 1453 | paddlefish | Paddlefish |
| 1454 | pan_fried_bream | Pan Fried Bream |
| 1455 | pan_fried_salmon | Pan-fried Salmon |
| 1456 | pan_fried_snapper | Pan-fried Snapper |
| 1457 | paper | Paper |
| 1458 | paper_pondshell | Paper Pondshell |
| 1459 | paper_stack | Paper Stack |
| 1460 | peach | Peach |
| 1461 | peaches_and_cream | Peaches and Cream |
| 1462 | pear | Pear |
| 1463 | pearl_clam | Pearl Clam |
| 1464 | peas | Peas |
| 1465 | peat | Peat |
| 1466 | pebble_minnow | Pebble Minnow |
| 1467 | perch | Perch |
| 1468 | perch_risotto | Perch Risotto |
| 1469 | perfect_copper_ore | Perfect Copper Ore |
| 1470 | perfect_diamond | Perfect Diamond |
| 1471 | perfect_emerald | Perfect Emerald |
| 1472 | perfect_gold_ore | Perfect Gold Ore |
| 1473 | perfect_iron_ore | Perfect Iron Ore |
| 1474 | perfect_mistril_ore | Perfect Mistril Ore |
| 1475 | perfect_pink_diamond | Perfect Pink Diamond |
| 1476 | perfect_ruby | Perfect Ruby |
| 1477 | perfect_sapphire | Perfect Sapphire |
| 1478 | perfect_silver_ore | Perfect Silver Ore |
| 1479 | pet_cosmetic | MISSING |
| 1480 | petalhopper | Petalhopper |
| 1481 | petrified_wood | Petrified Wood |
| 1482 | pick_axe_copper | Copper Pickaxe |
| 1483 | pick_axe_gold | Gold Pickaxe |
| 1484 | pick_axe_iron | Iron Pickaxe |
| 1485 | pick_axe_mistril | Mistril Pickaxe |
| 1486 | pick_axe_silver | Silver Pickaxe |
| 1487 | pick_axe_worn | Worn Pickaxe |
| 1488 | picket_fence | Picket Fence |
| 1491 | picnic_basket | Picnic Basket |
| 1492 | picnic_donut_plate | Picnic Display Donuts |
| 1493 | picnic_hamburger_plate | Picnic Display Burgers |
| 1496 | picnic_pie_plate | Picnic Display Pie |
| 1499 | picnic_place_setting | Picnic Place Setting |
| 1502 | picnic_rug | Picnic Blanket |
| 1503 | picnic_sandwich_plate | Picnic Display Sandwiches |
| 1504 | picnic_set | Picnic Set |
| 1507 | picnic_sunflower_vase | Picnic Sunflower Vase |
| 1508 | pike | Pike |
| 1509 | pinecone | Pinecone |
| 1510 | pineshroom | Pineshroom |
| 1511 | pineshroom_toast | Pineshroom Toast |
| 1512 | pink_pet_bed | Pink Pet Bed |
| 1513 | pink_pet_dish | Pink Pet Dish |
| 1514 | pink_scallop_shell | Pink Scallop Shell |
| 1515 | pizza | Pizza |
| 1516 | plum_blossom | Plum Blossom |
| 1517 | poached_pear | Poached Pear |
| 1518 | poinsettia | Poinsettia |
| 1519 | poison_snake_oil | Poison Snake Oil |
| 1520 | polar_fly | Polar Fly |
| 1521 | pollock | Pollock |
| 1522 | pomegranate | Pomegranate |
| 1523 | pomegranate_juice | Pomegranate Juice |
| 1524 | pomegranate_sorbet | Pomegranate Sorbet |
| 1525 | pond_skater | Pond Skater |
| 1526 | pond_snail | Pond Snail |
| 1527 | potato | Potato |
| 1528 | potato_soup | Potato Soup |
| 1529 | praying_mantis | Praying Mantis |
| 1530 | pudding | Pudding |
| 1531 | puddle_spider | Puddle Spider |
| 1532 | puffer_fish | Puffer Fish |
| 1533 | pumpkin | Pumpkin |
| 1534 | pumpkin_pie | Pumpkin Pie |
| 1535 | pumpkin_pile_black | Pumpkin Pile |
| 1536 | pumpkin_pile_green | Pumpkin Pile |
| 1537 | pumpkin_pile_orange | Pumpkin Pile |
| 1538 | pumpkin_pile_purple | Pumpkin Pile |
| 1539 | pumpkin_pile_white | Pumpkin Pile |
| 1540 | pumpkin_pile_yellow | Pumpkin Pile |
| 1541 | pumpkin_stew | Pumpkin Stew |
| 1542 | purple_mushroom | Purple Mushroom |
| 1543 | purse | Purse of Tesserae | **DISABLED** |
| 1544 | quality_hay | Quality Large Animal Feed |
| 1545 | quality_small_animal_feed | Quality Small Animal Feed |
| 1546 | queen_berry | Queen Berry |
| 1547 | queen_berry_pie | Queen Berry Pie |
| 1548 | queen_berry_trophy | Golden Queen Berry Trophy |
| 1549 | question_mark_butterfly | Question Mark Butterfly |
| 1550 | quiche | Quiche |
| 1551 | rabbit_plushie | Rabbit Plushie |
| 1552 | rabbit_wool | Rabbit Wool |
| 1553 | rainbow_geode | Rainbow Geode |
| 1554 | rainbow_seaweed | Rainbow Seaweed |
| 1555 | rainbow_trout | Rainbow Trout |
| 1556 | razorback | Razorback |
| 1557 | really_round_rock | Really Round Rock |
| 1558 | recipe_scroll | Recipe: {} | **DISABLED** |
| 1559 | red_capybara_wall_ribbon | Red Capybara Wall Ribbon |
| 1560 | red_chicken_wall_ribbon | Red Chicken Wall Ribbon |
| 1561 | red_duck_wall_ribbon | Red Duck Wall Ribbon |
| 1562 | red_obsidian | Red Obsidian |
| 1563 | red_rabbit_wall_ribbon | Red Rabbit Wall Ribbon |
| 1564 | red_snapper | Red Snapper |
| 1565 | red_snapper_sushi | Red Snapper Sushi |
| 1566 | red_toadstool | Red Toadstool |
| 1567 | red_wine | Red Wine |
| 1568 | redhead_worm | Redhead Worm |
| 1569 | refined_stone | Refined Stone |
| 1570 | relic_crab | Relic Crab |
| 1571 | restorative_syrup | Restorative Syrup |
| 1572 | rhinoceros_beetle | Rhinoceros Beetle |
| 1573 | rice | Rice |
| 1574 | rice_stalk | Rice Stalk |
| 1575 | riceball | Rice Ball |
| 1576 | ritual_beads | Ritual Beads |
| 1577 | ritual_chalice | Ritual Chalice |
| 1578 | ritual_incense_burner | Ritual Incense Burner |
| 1579 | ritual_scepter | Ritual Scepter |
| 1580 | ritual_tablet | Ritual Tablet |
| 1581 | river_snail | River Snail |
| 1582 | roach | Roach |
| 1583 | roasted_cauliflower | Roasted Cauliflower |
| 1584 | roasted_chestnuts | Roasted Chestnuts |
| 1585 | roasted_rice_tea | Roasted Rice Tea |
| 1586 | roasted_sweet_potato | Roasted Sweet Potato |
| 1587 | rock_bass | Rock Bass |
| 1588 | rock_guppy | Rock Guppy |
| 1589 | rock_roach | Rock Roach |
| 1590 | rock_salt | Rock Salt |
| 1591 | rock_statue | Rock Statue |
| 1592 | rock_with_a_hole | Rock with a Hole |
| 1593 | rockbiter | Rockbiter |
| 1594 | rockroot | Rockroot |
| 1595 | roly_poly | Roly Poly |
| 1596 | rose | Rose |
| 1597 | rose_hip | Rose Hip |
| 1598 | rose_tea | Rose Tea |
| 1599 | rosehip_jam | Rose Hip Jam |
| 1600 | rosemary | Rosemary |
| 1601 | rosemary_garlic_noodles | Rosemary Garlic Noodles |
| 1602 | rough_gemstone | Rough Gemstone |
| 1610 | round_cushioned_stool | Round Cushioned Stool |
| 1616 | royal_chest | Royal Chest |
| 1617 | rubber_fish | Rubber Fish |
| 1618 | rusted_shield | Rusted Shield |
| 1619 | rusted_treasure_chest | Rusted Treasure Chest |
| 1620 | rustic_cabin_scroll_bundle | Rustic Cabin Scroll Bundle |
| 1621 | ryis_lumber | Ryis' Lumber | **GLITCHED** |
| 1622 | sage | Sage |
| 1623 | salmon | Salmon |
| 1624 | salmon_sashimi | Salmon Sashimi |
| 1625 | salted_watermelon | Salted Watermelon |
| 1626 | sand_bug | Sand Bug |
| 1627 | sand_dollar | Sand Dollar |
| 1628 | sand_lance | Sand Lance |
| 1629 | sand_rug | Sand Rug |
| 1630 | sap | Sap |
| 1631 | sapling_apple | Apple Sapling |
| 1632 | sapling_cherry | Cherry Sapling |
| 1633 | sapling_lemon | Lemon Sapling |
| 1634 | sapling_oak | Oak Sapling |
| 1635 | sapling_orange | Orange Sapling |
| 1636 | sapling_peach | Peach Sapling |
| 1637 | sapling_pear | Pear Sapling |
| 1638 | sapling_pine | Pine Sapling |
| 1639 | sapling_pomegranate | Pomegranate Sapling |
| 1640 | sapphire_betta | Sapphire Betta |
| 1641 | sardine | Sardine |
| 1642 | sauger | Sauger |
| 1643 | saury | Saury |
| 1644 | sauteed_snow_peas | Sauteed Snow Peas |
| 1645 | scent_of_spring | Scent of Spring |
| 1646 | scrap_metal_boots | Scrap Metal Boots |
| 1647 | scrap_metal_chestpiece | Scrap Metal Chestpiece |
| 1648 | scrap_metal_helmet | Scrap Metal Helmet |
| 1649 | scrap_metal_pants | Scrap Metal Pants |
| 1650 | scrap_metal_ring | Scrap Metal Ring |
| 1651 | sea_bass | Sea Bass |
| 1652 | sea_bream | Sea Bream |
| 1653 | sea_bream_rice | Sea Bream Rice |
| 1654 | sea_grapes | Sea Grapes |
| 1655 | sea_scarab | Sea Scarab |
| 1656 | seafood_boil | Seafood Boil |
| 1657 | seafood_snow_pea_noodles | Seafood Snow Pea Noodles |
| 1658 | sealing_scroll | Sealing Scroll |
| 1659 | seaweed | Seaweed |
| 1660 | seaweed_salad | Seaweed Salad |
| 1661 | seed_basil | Basil Seed |
| 1662 | seed_beet | Beet Seed |
| 1663 | seed_broccoli | Broccoli Seed |
| 1664 | seed_burdock_root | Burdock Root Seed |
| 1665 | seed_cabbage | Cabbage Seed |
| 1666 | seed_carrot | Carrot Seed |
| 1667 | seed_catmint | Catmint Seed |
| 1668 | seed_cauliflower | Cauliflower Seed |
| 1669 | seed_celosia | Celosia Seed |
| 1670 | seed_chickpea | Chickpea Seed |
| 1671 | seed_chili_pepper | Chili Pepper Seed |
| 1672 | seed_chrysanthemum | Chrysanthemum Seed |
| 1673 | seed_corn | Corn Seed |
| 1674 | seed_cosmos | Cosmos Seed |
| 1675 | seed_cranberry | Cranberry Seed |
| 1676 | seed_cucumber | Cucumber Seed |
| 1677 | seed_daffodil | Daffodil Seed |
| 1678 | seed_daikon_radish | Daikon Radish Seed |
| 1679 | seed_daisy | Daisy Seed |
| 1680 | seed_dill | Dill Seed |
| 1681 | seed_frost_lily | Frost Lily Seed |
| 1682 | seed_garlic | Garlic Seed |
| 1683 | seed_heather | Heather Seed |
| 1684 | seed_iris | Iris Seed |
| 1685 | seed_jasmine | Jasmine Seed |
| 1686 | seed_lilac | Lilac Seed |
| 1687 | seed_marigold | Marigold Seed |
| 1688 | seed_moon_fruit | Moon Fruit Seed |
| 1689 | seed_mystery_bag | Magic Seed |
| 1690 | seed_night_queen | Night Queen Seed |
| 1691 | seed_onion | Onion Seed |
| 1692 | seed_oregano | Oregano Seed |
| 1693 | seed_peas | Pea Seed |
| 1694 | seed_poinsettia | Poinsettia Seed |
| 1695 | seed_potato | Potato Seed |
| 1696 | seed_pumpkin | Pumpkin Seed |
| 1697 | seed_rice | Rice Seed |
| 1698 | seed_rosemary | Rosemary Seed |
| 1699 | seed_sage | Sage Seed |
| 1700 | seed_snapdragon | Snapdragon Seed |
| 1701 | seed_snow_peas | Snow Peas Seed |
| 1702 | seed_snowdrop_anemone | Snowdrop Anemone Seed |
| 1703 | seed_strawberry | Strawberry Seed |
| 1704 | seed_sugar_cane | Sugar Cane Seed |
| 1705 | seed_sunflower | Sunflower Seed |
| 1706 | seed_sweet_potato | Sweet Potato Seed |
| 1707 | seed_tea | Tea Seed |
| 1708 | seed_temple_flower | Temple Flower Seed |
| 1709 | seed_thyme | Thyme Seed |
| 1710 | seed_tomato | Tomato Seed |
| 1711 | seed_tulip | Tulip Seed |
| 1712 | seed_turnip | Turnip Seed |
| 1713 | seed_viola | Viola Seed |
| 1714 | seed_watermelon | Watermelon Seed |
| 1715 | seed_wheat | Wheat Seed |
| 1716 | seriously_square_stone | Seriously Square Stone |
| 1717 | sesame | Sesame |
| 1718 | sesame_broccoli | Sesame Broccoli |
| 1719 | sesame_tuna_bowl | Sesame Tuna Bowl |
| 1720 | shad | Shad |
| 1721 | shadow_bass | Shadow Bass |
| 1722 | shadow_flower | Shadow Flower |
| 1723 | shale_grass | Shale Grass |
| 1724 | shard_mass | Shard Mass |
| 1725 | shardfin | Shardfin |
| 1726 | shards | Shards |
| 1727 | shark | Shark |
| 1728 | sheep_plushie | Sheep Plushie |
| 1729 | sheep_wool | Sheep Wool |
| 1730 | shortcut_scroll | Shortcut Scroll |
| 1731 | shovel_copper | Copper Shovel |
| 1732 | shovel_gold | Gold Shovel |
| 1733 | shovel_iron | Iron Shovel |
| 1734 | shovel_mistril | Mistril Shovel |
| 1735 | shovel_silver | Silver Shovel |
| 1736 | shovel_worn | Worn Shovel |
| 1737 | shrimp | Shrimp |
| 1738 | silver_alpaca_trophy | Silver Alpaca Trophy |
| 1739 | silver_armor | Silver Armor |
| 1740 | silver_cow_trophy | Silver Cow Trophy |
| 1741 | silver_greaves | Silver Greaves |
| 1742 | silver_helmet | Silver Helmet |
| 1743 | silver_horse_trophy | Silver Horse Trophy |
| 1744 | silver_ingot | Silver Ingot |
| 1745 | silver_legplates | Silver Legplates |
| 1746 | silver_redhorse | Silver Redhorse |
| 1747 | silver_ring | Silver Ring |
| 1748 | silver_sheep_trophy | Silver Sheep Trophy |
| 1749 | simmered_daikon | Simmered Daikon |
| 1750 | singing_katydid | Singing Katydid |
| 1751 | sliced_turnip | Sliced Turnip |
| 1752 | small_animal_seesaw | Small Animal Seesaw |
| 1753 | small_barn_black_blueprint | Small White Barn Blueprint |
| 1754 | small_barn_red_blueprint | Small Red Barn Blueprint |
| 1762 | small_bathmat | Small Bathmat |
| 1763 | small_coop_black_blueprint | Small White Coop Blueprint |
| 1764 | small_coop_red_blueprint | Small Red Coop Blueprint |
| 1765 | small_decorative_pumpkin_black | Small Decorative Pumpkin |
| 1766 | small_decorative_pumpkin_green | Small Decorative Pumpkin |
| 1767 | small_decorative_pumpkin_orange | Small Decorative Pumpkin |
| 1768 | small_decorative_pumpkin_purple | Small Decorative Pumpkin |
| 1769 | small_decorative_pumpkin_white | Small Decorative Pumpkin |
| 1770 | small_decorative_pumpkin_yellow | Small Decorative Pumpkin |
| 1771 | small_grass_starter | Grass Starter |
| 1772 | small_greenhouse_white_blueprint | Small White Greenhouse Blueprint |
| 1773 | small_greenhouse_wood_blueprint | Small Wood Greenhouse Blueprint |
| 1774 | small_stone_bundle | Small Bundle of Stone |
| 1778 | small_vase | Small Vase |
| 1779 | small_wood_bundle | Small Bundle of Wood |
| 1780 | smallmouth_bass | Smallmouth Bass |
| 1781 | smoke_moth | Smoke Moth |
| 1782 | smoked_trout_soup | Smoked Trout Soup |
| 1783 | snail | Snail |
| 1784 | snakehead | Snakehead |
| 1785 | snapdragon | Snapdragon |
| 1786 | snapping_turtle | Snapping Turtle |
| 1787 | snow_fish | Snow Fish |
| 1788 | snow_peas | Snow Peas |
| 1789 | snowball_beetle | Snowball Beetle |
| 1790 | snowdrop_anemone | Snowdrop Anemone |
| 1791 | snug_bug | Snug Bug |
| 1792 | sod | Sod |
| 1793 | soup_of_the_day | Soup of the Day | **GLITCHED** |
| 1794 | soup_of_the_day_gold | Soup of the Day Deluxe | **GLITCHED** |
| 1795 | sour_lemon_cake | Lemon Cake |
| 1796 | soy_sauce | Soy Sauce |
| 1797 | speedy_snail | Speedy Snail |
| 1798 | speedy_syrup | Speedy Syrup |
| 1799 | spell_fruit | Spell Fruit |
| 1800 | spell_fruit_parfait | Spell Fruit Parfait |
| 1801 | spicy_cheddar_biscuit | Spicy Cheddar Biscuit |
| 1802 | spicy_corn | Spicy Corn |
| 1803 | spicy_crab_sushi | Spicy Crab Sushi |
| 1804 | spicy_water_chestnuts | Spicy Water Chestnuts |
| 1805 | spirit_mushroom | Spirit Mushroom |
| 1806 | spirit_mushroom_tea | Spirit Mushroom Tea |
| 1807 | spirula_shell | Spirula Shell |
| 1808 | spooky_haybale | Hay Bale |
| 1811 | spring_bed | Spring Bed |
| 1812 | spring_crop_sign_scroll_bundle | Spring Crop Sign Scroll Bundle |
| 1815 | spring_double_bed | Spring Double Bed |
| 1816 | spring_festival_basket | Breath of Spring Basket |
| 1817 | spring_festival_breath_of_spring_wreath | Deluxe Breath of Spring Wreath |
| 1818 | spring_festival_flower_chest | Breath of Spring Storage Chest |
| 1819 | spring_festival_large_planter | Breath of Spring Large Planter |
| 1820 | spring_festival_planter | Breath of Spring Planter |
| 1821 | spring_festival_wreath | Breath of Spring Wreath |
| 1824 | spring_flooring | Spring Flooring |
| 1825 | spring_galette | Spring Galette |
| 1828 | spring_lamp | Spring Lamp |
| 1831 | spring_potted_tree | Spring Potted Tree |
| 1834 | spring_rug | Spring Flower Rug |
| 1835 | spring_salad | Spring Salad |
| 1838 | spring_sofa | Spring Sofa |
| 1841 | spring_wallpaper | Spring Wallpaper |
| 1842 | square_tile_floor | Square Tile Floor |
| 1843 | squid | Squid |
| 1844 | stairs | House Stairs |
| 1845 | stamina_syrup | Stamina Syrup |
| 1846 | stamina_up | Stamina Up |
| 1847 | star_brooch | Star Brooch |
| 1853 | star_cushion | Star Cushion |
| 1858 | star_lantern | Star Lantern |
| 1859 | star_shaped_cookie | Star-Shaped Cookie |
| 1863 | star_viewing_blanket | Star Viewing Blanket |
| 1864 | starlight_coral | Starlight Coral |
| 1866 | starry_flooring | Starry Flooring |
| 1867 | starter_bird_house_red | Birdhouse |
| 1868 | starter_potted_plant | Potted Plant |
| 1869 | starter_scarecrow | Scarecrow |
| 1870 | starter_shipping_box | Shipping Bin |
| 1871 | starter_stone_path | Stone Path |
| 1872 | starter_stone_path_double | Stone Path |
| 1873 | starter_well | Water Well |
| 1874 | starter_wheelbarrow | Wheelbarrow |
| 1875 | starter_wood_fence | Wooden Fence |
| 2100 | statue_of_boons | Statue of Boons |
| 1876 | statuette_of_caldarus | Statuette of Caldarus |
| 1877 | steamed_broccoli | Steamed Broccoli |
| 1878 | stingray | Stingray |
| 1879 | stinky_stamina_potion | MISSING | **GLITCHED** |
| 1882 | stone_bench | Stone Bench |
| 1885 | stone_dragon_figure | Stone Dragon Figure |
| 1886 | stone_horse | Stone Horse |
| 1889 | stone_lamp | Stone Lamp |
| 1890 | stone_loach | Stone Loach |
| 1891 | stone_shell | Stone Shell |
| 1894 | stone_storage_chest | Stone Storage Chest |
| 1897 | stone_table | Stone Table |
| 1898 | stone_wall | Stone Wall |
| 1899 | strawberries_and_cream | Strawberries and Cream |
| 1900 | strawberry | Strawberry |
| 1901 | strawberry_shortcake | Strawberry Shortcake |
| 1902 | strawhopper | Strawhopper |
| 1903 | striped_bass | Striped Bass |
| 1904 | strobe_firefly | Strobe Firefly |
| 1905 | sturgeon | Sturgeon |
| 1906 | sugar | Sugar |
| 1907 | sugar_cane | Sugar Cane |
| 1908 | sulfur_crab | Sulfur Crab |
| 1910 | summer_bed | Summer Bed |
| 1912 | summer_chair | Summer Chair |
| 1913 | summer_crop_sign_scroll_bundle | Summer Crop Sign Scroll Bundle |
| 1915 | summer_double_bed | Summer Double Bed |
| 1917 | summer_flooring | Summer Flooring |
| 1919 | summer_jar_lamp | Summer Jar Lamp |
| 1921 | summer_rug | Summer Flower Rug |
| 1922 | summer_salad | Summer Salad |
| 1924 | summer_table | Summer Table |
| 1926 | summer_wallpaper | Summer Wallpaper |
| 1928 | summit_wallpaper | Summit Wallpaper |
| 2102 | summoning_circle | Summoning Circle |
| 1929 | sunflower | Sunflower |
| 1930 | sunny | Sunny |
| 1931 | surf_beetle | Surf Beetle |
| 1932 | sushi_platter | Sushi Platter |
| 1933 | sweet_potato | Sweet Potato |
| 1934 | sweet_potato_pie | Sweet Potato Pie |
| 1935 | sweet_sesame_balls | Sweet Sesame Balls |
| 1936 | sweetfish | Sweetfish |
| 1937 | sweetroot | Sweetroot |
| 1938 | sword_copper | Copper Sword |
| 1939 | sword_crystal | Crystal Sword |
| 1940 | sword_gold | Gold Sword |
| 1941 | sword_iron | Iron Sword |
| 1942 | sword_mistril | Mistril Sword |
| 1943 | sword_scrap_metal | Scrap Metal Sword |
| 1944 | sword_silver | Silver Sword |
| 1945 | sword_tarnished_gold | Tarnished Gold Sword |
| 1946 | sword_verdigris | Verdigris Sword |
| 1947 | sword_worn | Worn Sword |
| 1948 | swordfish | Swordfish |
| 1949 | tarnished_gold_armor | Tarnished Gold Chestpiece |
| 1950 | tarnished_gold_boots | Tarnished Gold Greaves |
| 1951 | tarnished_gold_helmet | Tarnished Gold Helmet |
| 1952 | tarnished_gold_legplates | Tarnished Gold Legplates |
| 1953 | tarnished_gold_ring | Tarnished Gold Ring |
| 1954 | tarpon | Tarpon |
| 1955 | tea | Tea |
| 1956 | tea_black | Black Tea Cup |
| 1957 | tea_green | Green Tea Cup |
| 2104 | telepop_blue | Telepop (Blue) |
| 2107 | telepop_green | Telepop (Green) |
| 2109 | telepop_mystery | Telepop (Mystery) |
| 2105 | telepop_orange | Telepop (Orange) |
| 2106 | telepop_pink | Telepop (Pink) |
| 2103 | telepop_purple | Telepop (Purple) |
| 2108 | telepop_yellow | Telepop (Yellow) |
| 1958 | teleportation_pad | Teleportation Circle |
| 1959 | temple_flower | Temple Flower |
| 1960 | tetra | Tetra |
| 1961 | thorn_vine | Thorn Vine |
| 1962 | thyme | Thyme |
| 1963 | tide_lettuce | Tide Lettuce |
| 1964 | tide_salad | Tide Salad |
| 1965 | tide_touched_cavern_wall | Tide-Touched Cavern Wall |
| 1966 | tidestone | Tidestone |
| 1967 | tiger_swallowtail_butterfly | Tiger Swallowtail Butterfly |
| 1968 | tilapia | Tilapia |
| 1971 | tile_roof_fence | Tile Roof Fence |
| 1972 | tin_lunchbox | Tin Lunchbox |
| 1973 | tiny_dinosaur_skeleton | Tiny Dinosaur Skeleton |
| 1974 | tiny_volcano | Tiny Volcano |
| 1975 | toasted_sunflower_seeds | Toasted Sunflower Seeds |
| 1977 | toilet | Toilet |
| 1978 | tomato | Tomato |
| 1979 | tomato_soup | Tomato Soup |
| 1980 | trail_mix | Trail Mix |
| 1983 | training_dummy | Training Dummy |
| 1984 | transparent_jellyfish | Transparent Jellyfish |
| 1985 | treasure_box_copper | Copper Treasure Box |
| 1986 | treasure_box_gold | Golden Treasure Box |
| 1987 | treasure_box_silver | Silver Treasure Box |
| 1988 | treasure_box_wood | Wooden Treasure Box |
| 1989 | trilobite_fossil | Trilobite Fossil |
| 1990 | trout | Trout |
| 1991 | tulip | Tulip |
| 1992 | tulip_cake | Tulip Cake |
| 1993 | tuna | Tuna |
| 1994 | tuna_sashimi | Tuna Sashimi |
| 1995 | tunnel_millipede | Tunnel Millipede |
| 1996 | turnip | Turnip |
| 1997 | turnip_and_cabbage_salad | Turnip & Cabbage Salad |
| 1998 | turnip_and_potato_gratin | Turnip & Potato Gratin |
| 1999 | turtle | Turtle |
| 2000 | twice_baked_rations | Twice-Baked Rations |
| 2001 | ultimate_hay | Ultimate Large Animal Feed |
| 2002 | ultimate_small_animal_feed | Ultimate Small Animal Feed |
| 2003 | underseaweed | Underseaweed |
| 2004 | unidentified_artifact | Unidentified Artifact | **DISABLED** |
| 2005 | unknown_dragon_statuette | Unknown Dragon Statuette |
| 2006 | unknown_item | Unknown Item | **PLACEHOLDER** |
| 2007 | unusual_seed | Unusual Seed | **GLITCHED** |
| 2008 | upper_mines_mushroom | Upper Mines Mushroom |
| 2009 | vegetable_pot_pie | Vegetable Pot Pie |
| 2010 | vegetable_quiche | Vegetable Quiche |
| 2011 | vegetable_soup | Vegetable Soup |
| 2012 | veggie_sub_sandwich | Veggie Sub Sandwich |
| 2013 | verdigris_boots | Verdigris Boots |
| 2014 | verdigris_chestpiece | Verdigris Chestpiece |
| 2015 | verdigris_helmet | Verdigris Helmet |
| 2016 | verdigris_ring | Verdigris Ring |
| 2017 | verdigris_tassets | Verdigris Tassets |
| 2018 | vintage_brush | Vintage Brush |
| 2019 | vintage_cow_bell | Vintage Cow Bell |
| 2020 | vintage_hammer | Vintage Hammer |
| 2021 | vintage_sickle | Vintage Sickle |
| 2022 | vintage_watering_can | Vintage Watering Can |
| 2023 | viola | Viola |
| 2024 | walking_leaf | Walking Leaf |
| 2025 | walking_stick | Walking Stick |
| 2026 | walleye | Walleye |
| 2027 | walnut | Walnut |
| 2028 | warm_rock | Warm Rock |
| 2029 | water_balloon_fish | Water Balloon Fish |
| 2030 | water_chestnut | Water Chestnut |
| 2031 | water_chestnut_fritters | Water Chestnut Fritters |
| 2032 | water_sphere | Water Sphere |
| 2037 | water_sprite | Water Sprite Statue |
| 2038 | water_tablet_rubbing | Water Tablet Rubbing |
| 2039 | waterbug | Waterbug |
| 2040 | waterfly | Waterfly |
| 2041 | watering_can_copper | Copper Watering Can |
| 2042 | watering_can_gold | Gold Watering Can |
| 2043 | watering_can_iron | Iron Watering Can |
| 2044 | watering_can_mistril | Mistril Watering Can |
| 2045 | watering_can_silver | Silver Watering Can |
| 2046 | watering_can_worn | Worn Watering Can |
| 2047 | watermelon | Watermelon |
| 2048 | weather_crystal_ball | Weather Crystal Ball |
| 2049 | weightless_stone | Weightless Stone |
| 2050 | wheat | Wheat |
| 2051 | white_capybara_wall_ribbon | White Capybara Wall Ribbon |
| 2052 | white_chicken_wall_ribbon | White Chicken Wall Ribbon |
| 2053 | white_duck_wall_ribbon | White Duck Wall Ribbon |
| 2054 | white_perch | White Perch |
| 2055 | white_rabbit_wall_ribbon | White Rabbit Wall Ribbon |
| 2056 | white_wine | White Wine |
| 2057 | wild_berries | Wild Berries |
| 2058 | wild_berry_jam | Wild Berry Jam |
| 2059 | wild_grapes | Wild Grapes |
| 2060 | wild_leek | Wild Leek |
| 2061 | wild_mushroom | Cave Mushroom |
| 2062 | wildberry_pie | Wild Berry Pie |
| 2063 | wildberry_scone | Wild Berry Scone |
| 2064 | windleaf_butterfly | Windleaf Butterfly |
| 2065 | wine_glass_red | Red Wine Glass |
| 2066 | wine_glass_rose | Rose Wine Glass |
| 2067 | wine_glass_white | White Wine Glass |
| 2068 | winter_crop_sign_scroll_bundle | Winter Crop Sign Scroll Bundle |
| 2069 | winter_stew | Winter Stew |
| 2070 | wintergreen_berry | Wintergreen Berry |
| 2071 | wintergreen_ice_cream | Wintergreen Ice Cream |
| 2072 | winterpillar | Winterpillar |
| 2073 | witch_queen_bed | Witch Queen Bed |
| 2074 | witch_queen_cauldron | Witch Queen Cauldron |
| 2075 | witch_queen_chair | Witch Queen Chair |
| 2076 | witch_queen_double_bed | Witch Queen Double Bed |
| 2077 | witch_queen_dressing_table | Witch Queen Dressing Table |
| 2078 | witch_queen_flooring | Witch Queen Flooring |
| 2079 | witch_queen_moon_lamp | Witch Queen Moon Lamp |
| 2080 | witch_queen_nightstand | Witch Queen Nightstand |
| 2081 | witch_queen_pillar | Witch Queen Pillar |
| 2082 | witch_queen_rug | Witch Queen Rug |
| 2083 | witch_queen_table | Witch Queen Table |
| 2084 | witch_queen_throne | Witch Queen Throne |
| 2085 | witch_queen_wallpaper | Witch Queen Wallpaper |
| 2086 | woodcrafting_station | Crafting Station |
| 2087 | world_fountain | World Fountain 20 | **GLITCHED** |
| 2088 | worm | Worm |
| 2089 | worn_bed | Worn Bed |
| 2090 | worn_chair | Worn Chair |
| 2091 | worn_fireplace | Worn Fireplace |
| 2092 | worn_flooring | Worn Flooring |
| 2093 | worn_garden_lamp | Worn Garden Lamp |
| 2094 | worn_nightstand | Worn Nightstand |
| 2095 | worn_rug | Worn Rug |
| 2096 | worn_table | Worn Table |
| 2097 | worn_wallpaper | Worn Wallpaper |
| 2098 | worn_well | Worn Well |
| 2099 | worn_window | Worn Window |

# Animal Friends
A YYTK mod for [Fields of Mistria](https://www.fieldsofmistria.com/) that can:
- Increase the amount of heart points your animals gain from various actions (such as petting, feeding, etc.).
- Prevent animals from losing heart points.
- Automatically pet your animals (daily).
- Automatically feed your animals (daily).
- Spawn extra shiny beads (daily) when you pet an animal.
- Automatically ring the bell for animals to go outside or inside.

## Configuration
The mod is **customizable** allowing you to change various options inside the **configuration file**.

### Configuration File
> **Note**: The configuration file will NOT exist until the game (and mod) have been started at least once. Therefore, if you wish to configure the mod, start the game until the title screen loads, then EXIT the game, and lastly edit the configuration file before restarting the game.

In order to *customize* the mod, you must modify the properties in its **configuration file**. This file is located in the mod_data folder inside the Fields of Mistria folder where the game is located. Below is a directory tree view of where the configuration file exists beginning with the folder that the game is in:
```
Fields of Mistria
├─ FieldsOfMistria.exe
├─ mod_data
│  ├─ AnimalFriends
│  │  ├─ AnimalFriends.json
```

The full "path" to the file will vary depending on where the game is installed. For example, with the game installed via Steam on the C drive the full path for the configuration file would be:
```
C:\Program Files (x86)\Steam\steamapps\common\Fields of Mistria\mod_data\AnimalFriends\AnimalFriends.json
```

### Configuration Options
The table below contains details for each configuration option.

| Option | Type | Explanation | Default |
| ------ | ---- | ----------- | ----- |
| animal_bed_time | integer | Adjusts **when animals are sent indoors**. The value is a time of day in SECONDS. It must be an integer between `21600` (6:00 AM) and `86400` (midnight). | `64800` (6:00 PM) |
| animal_wake_up_time | integer | Adjusts **when animals are sent outdoors**. The value is a time of day in SECONDS. It must be an integer between `21600` (6:00 AM) and `86400` (midnight). | `28800` (8:00 AM) |
| auto_bell_in | boolean | Controls if  animals are **automatically sent indoors** at the `animal_bed_time`. | `false` |
| auto_bell_out | boolean | Controls if  animals are **automatically sent outdoors** at the `animal_wake_up_time`. | `false` |
| mute_auto_bell_sounds | boolean | Controls if the bell sound effects should be **muted** when `auto_bell_in` or `auto_bell_out` triggers. | false |
| auto_feed | boolean | Controls if animals are **automatically fed for the day**. | `false` |
| auto_pet | boolean | Controls if animals are **automatically pet for the day**. | `false` |
| friendship_multiplier | integer | Adjusts how many **heart points** you get when **interacting with animals**. It must be a integer between `1` and `100` (inclusive). | `5` |
| prevent_friendship_loss | boolean | Cntrols if animals **can lose heart points**. | `false` |
| gain_ranching_xp | boolean | Controls if you receive **rancing skill experience** when the `auto_pet` and `auto_feed` trigger. | `false` |
| spawn_extra_beads_daily | boolean | Controls if *additional* **shiny beads get spawned** when you **pet any animal** the *first time* each day. The number of beads spawned is equal to the number of animals on your farm multiplied by the `extra_beads_daily_multiplier`, with the maximum result being `999`. | false |
| extra_beads_daily_multiplier | integer | Adjusts how many *extra* **shiny beads are spawned** when the `spawn_extra_beads_daily` option is enabled. | `1` |

### Default Configuration
The configuration file by *default* will look like the following:

```json
{
    "animal_bed_time": 64800,
    "animal_wake_up_time": 28800,
    "auto_bell_in": false,
    "auto_bell_out": false,
    "auto_feed": false,
    "auto_pet": false,
    "extra_beads_daily_multiplier": 1,
    "friendship_multiplier": 5,
    "gain_ranching_xp": true,
    "mute_auto_bell_sounds": false,
    "prevent_friendship_loss": true,
    "spawn_extra_beads_daily": false
}
```

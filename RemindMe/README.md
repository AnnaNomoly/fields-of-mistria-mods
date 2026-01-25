# Remind Me
A YYTK mod for [Fields of Mistria](https://www.fieldsofmistria.com/) that **allows you to create custom reminders** using the mod's **configuration file**. Reminders will be displayed using the game's *notification system* when the conditions you specify are met!


## About Reminders
Reminders are created using **JSON**.
> **NOTE**: The JSON below is an example that has ALL of the possible conditions listed for illustration purposes. Conditions are optional, though at least ONE condition is required.

```json
{
    "reminders": [
        {
            "conditions": {
                "26h_time": "16:45",
                "day_of_week": "WEDNESDAY",
                "day_of_month": 21,
                "season": "FALL",
                "year": 2,
                "weather": "CALM",
                "location": "TOWN",
                "npc_nearby": "BALOR",
                "npc_giftable": true,
                "farm_animal_outside": true,
                "reset_after": "LOCATION",
                "can_trigger_after": false
            },
            "notification": "This is an absurdly specific notification!"
        }
    ]
}
```


## Conditions
Reminders may have one or more optional **conditions**, though at least one condition is *required* for a reminder to function. Conditions are evaluated together to determine when a reminder should *trigger* (display the notification). If a condition is not necessary for your reminder, or is irrelevant, simply *omit* it from your reminder! Below is a detailed explanation of what each condition does.

| Condition | Type | Explanation | Usage | 
| --------- | ---- | ----------- | ----- |
| 26h_time | string | A string (value enclosed in "quotes") of the format `"HH:MM"`, representing the **current time**, where **HH** are *twenty-six hour* formatted digits and **MM** are *minute* formatted digits. Both the **HH** and **MM** values must contain two (2) digits, so a leading zero (0) may be required. Additionally, for values representing time after *midnight*, use the *hour* (**HH**) ranges from `24` to `26`. For example, the value corresponding to **2:05 PM** would be **"14:05"**, the value corresponding to **6:30 AM** would be **"06:30"**, and the value corresponding to **1:45 AM** would be **"25:45"**. This value will be validated by the mod to ensure it not only conforms to the correct format (`"HH:MM"`), but that the *hours* (**HH**) values are within the range of `06` to `26`, the *minutes* (**MM**) values are within the range of `00` to `59`, and the *total time* value (`"HH:MM"`) is within the range of **"06:00"** to **"26:00"** (inclusive). | This condition means the reminder will *only* trigger at the **specified time**. |
| day_of_week | string | A string (value enclosed in "quotes") of one of the following pre-defined constants, representing the **curent day of the week**: `"MONDAY"`, `"TUESDAY"`, `"WEDNESDAY"`, `"THURSDAY"`, `"FRIDAY"`, `"SATURDAY"`, `"SUNDAY"`. Capitalization does not matter, but the value must be one of the specified constants. | This condition means the reminder will *only* trigger on the **specified weekday**. |
| day_of_month | integer | An integer value between `1` and `28` (inclusive), representing the **current day of the month**. | This condition means the reminder will *only* trigger on the **specified day of the month**.
| season | string | A string (value enclosed in "quotes") of one of the following pre-defined constants, representing the **current season**: `"SPRING"`, `"SUMMER"`, `"FALL"`, `"WINTER"`. Capitalization does not matter, but the value must be one of the specified constants. | This condition means the reminder will *only* trigger during the **specified season**. |
| year | integer | An integer value greater than or equal to `1`, representing the **current year**. | This condition means the reminder will *only* trigger on the **specified year**. |
| weather | string | A string (value enclosed in "quotes") of one following pre-defined constants, representing the **current weather**: `"CALM"`, `"INCLEMENT"`, `"SEVERE"`, `"SPECIAL"`. **Sunny** weather corresponds to `CALM`. **Rain** or **snow** weather corresponds to `INCLEMENT`. **Storms** or **blizzards** correspond to `SEVERE`. **Windy**/**breeze** weather (cherry blossoms and falling leaves) corresponds to `SPECIAL`, which is when *legendary fish* spawn. Capitalization does not matter, but the value must be one of the specified constants. | This condition means the reminder will *only* trigger during the **specified weather**. |
| location | string | A string (value enclosed in "quotes") of one following pre-defined constants, representing your **current location**: `"WESTERN RUINS"`, `"NARROWS"`, `"SWEETWATER FARM"`, `"BEACH"`, `"TOWN"`, `"EASTERN ROAD"`, `"DEEP WOODS"`, `"FARM"`, `"INN"`, `"DUNGEON"`. Capitalization does not matter, but the value must be one of the specified constants. | This condition means the reminder will *only* trigger at the **specified location**. | 
| npc_nearby | string | A string (value enclosed in "quotes") of one following pre-defined constants, representing the **nearby npc**: `"ADELINE"`, `"BALOR"`, `"CALDARUS"`, `"CELINE"`, `"DARCY"`, `"DELL"`, `"DOZY"`, `"EILAND"`, `"ELSIE"`, `"ERROL"`, `"HAYDEN"`, `"HEMLOCK"`, `"HENRIETTA"`, `"HOLT"`, `"JOSEPHINE"`, `"JUNIPER"`, `"LANDEN"`, `"LOUIS"`, `"LUC"`, `"MAPLE"`, `"MARCH"`, `"MERRI"`, `"NORA"`, `"OLRIC"`, `"REINA"`, `"RYIS"`, `"TALIFERRO"`, `"TERITHIA"`, `"VALEN"`, `"VERA"`, `"WHEEDLE"`. Capitalization does not matter, but the value must be one of the specified constants. | This condition means the reminder will *only* trigger if the **specified NPC is in the same area as you**. |
| npc_giftable | boolean | A boolean value (either `true` or `false`). If set to `true`, the reminder will *only* trigger if the **npc_nearby may be given a gift**. If set to `false` or *omitted*, this condition is **ignored**. | This condition means the reminder will *only* trigger if the **npc_nearby may be given a gift**. |
| farm_animal_outside | boolean | A boolean value (either `true` or `false`). If set to `true`, the reminder will *only* trigger if **any farm animal is outside**. If set to `false` or *omitted*, this condition is **ignored**. | This condition means the reminder will *only* trigger if **any farm animal is outside**. |
| reset_after | string | A string (value enclosed in "quotes") of one following: `"HH:MM"` (a valid **26h_time** format), or one of the constant values: `"LOCATION"` or `"DAY"`. If set to `"HH:MM"` the reminder will reset *after* that **amount of time has passed**. If set to `"LOCATION"`, the reminder will reset **when you change locations**. If set to `"DAY"`, the reminder will **reset daily**. If *omitted*, the value `"DAY"` is used, meaning the reminder will **reset daily**. This value is a special reminder *modifier* that determines *when* a **condition may repeat** (reset). | Regardless of the value chosen, *all* reminders will be reset daily. This option simply allows you to additionally reset them *earlier*. |
| can_trigger_after | boolean | A boolean value (either `true` or `false`). If set to `true`, reminders that have a **26h_time** condition may trigger *after* the **specified time has passed** when loading a save file or some other event fast forwards time (such as crafting). If set to `false`, reminders with the **26h_time** will *only* trigger when the **specified time is naturally reached**. If *omitted*, the value `false` is used. | This value is a special reminder *modifier* that controls *additional behavior* related to the **26h_time** condition. |


## Examples
The following examples will be used to demonstrate how to construct reminders using the various conditions. Remember, *multiple* reminders can be defined in the JSON configuration file!

### Friday Night Reminder
A simple configuration matching how the Friday Night Reminder mod works. The values for `reset_after` and `can_trigger_after` are **not necessary in this reminder** (as they match their *default* values), but were included for illustration purposes.

```json
{
    "reminders": [
        {
            "conditions": {
                "26h_time": "20:00",
                "day_of_week": "FRIDAY",
                "reset_after": "DAY",
                "can_trigger_after": false
            },
            "notification": "It's Friday night. You should stop by the inn!"
        }
    ]
}
```

### Birthday Reminder
A simple configuration for reminders on Adeline's and Eiland's birthdays. Since these reminders *don't* include a `26h_time` or the `reset_after` option, they will be displayed once (immediately when the day starts).

```json
{
    "reminders": [
        {
            "conditions": {
                "day_of_month": 18,
                "season": "WINTER"
            },
            "notification": "It's Adeline's birthday!"
        },
        {
            "conditions": {
                "day_of_month": 20,
                "season": "SUMMER"
            },
            "notification": "It's Eiland's birthday!"
        },
    ]
}
```

### Balor Summer Beach Alert
A simple configuration that notifies you if Balor enters the Beach map during summer while you are there. Since this reminder *does* set the `reset_after` option to `true`, it can trigger *again* that same day if you **leave the location** (BEACH) and return.

```json
{
    "reminders": [
        {
            "conditions": {
                "season": "SUMMER",
                "location": "BEACH",
                "npc_nearby": "BALOR",
                "reset_after": true
            },
            "notification": "Balor summer beach alert!"
        }
    ]
}
```

### Animals Outside at Night Reminder
A simple configuration that notifies you if your animals are outside at night when you enter the farm. Since this reminder *does* set the `can_trigger_after` option to `true`, it can trigger *after* the specified `26h_time`.

```json
{
    "reminders": [
        {
            "conditions": {
                "26h_time": "20:00",
                "location": "FARM",
                "farm_animal_outside": true,
                "can_trigger_after": true,
            },
            "notification": "Send your animals inside!"
        }
    ]
}
```

### Birthday Present Reminder
A simple configuration that notifies you if March is at your location on his birthday and may be given a gift. Since these reminder *doesn't* include a `reset_after` option, it will *only* trigger once that day.

```json
{
    "reminders": [
        {
            "conditions": {
                "day_of_month": 16,
                "season": "SPRING",
                "npc_nearby": "MARCH",
				"npc_giftable": true
            },
            "notification": "Give March a birthday present!"
        }
    ]
}
```


## Configuration
The mod is **customizable** and allows you to create reminders using JSON. You may have *multiple* reminders in your **configuration file**!

### Configuration File
> **Note**: The configuration file will NOT exist until the game (and mod) have been started at least once. Therefore, if you wish to configure the mod, start the game until the title screen loads, then EXIT the game, and lastly edit the configuration file before restarting the game.

In order to *customize* the mod, you must modify the properties in its **configuration file**. This file is located in the mod_data folder inside the Fields of Mistria folder where the game is located. Below is a directory tree view of where the configuration file exists beginning with the folder that the game is in:
```
Fields of Mistria
├─ FieldsOfMistria.exe
├─ mod_data
│  ├─ RemindMe
│  │  ├─ RemindMe.json
```

The full "path" to the file will vary depending on where the game is installed. For example, with the game installed via Steam on the C drive the full path for the configuration file would be:
```
C:\Program Files (x86)\Steam\steamapps\common\Fields of Mistria\mod_data\RemindMe\RemindMe.json
```

### Default Configuration
The configuration file by *default* will look like the following:

```json
{
    "reminders": [
        {
            "conditions": {
                "can_trigger_after": true,
                "season": "SPRING",
                "weather": "SPECIAL"
            },
            "notification": "The legendary fish is in season! (Perk Required)"
        },
        {
            "conditions": {
                "can_trigger_after": true,
                "day_of_month": 18,
                "season": "WINTER"
            },
            "notification": "Today is Adeline's birthday!"
        }
    ]
}
```

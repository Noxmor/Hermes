# Hermes
Hermes is a Text Adventure Engine for Console Applications, written in C.

***

## Features (planned)

* JRPG like gameplay
* NPCs, Dialogues, Quests
* Map, Locations
* Battles
* Characters with unique skills
* Monsters
* Leveling
* Inventory and Items
* Mod Support
* UTF-8 support (currently only ASCII)

***

## Current work focus
Currently focusing on file io handling and internal design decisions.

***

## How Hermes works
Hermes mostly relies on external files. Hermes is capable of storing basic settings in a config file, loading game files which define the Text Adventure and loading locales for different languages. Hermes is based on a command like pattern. The Console will always display the current state of the game and below that will be a list of commands for the player to choose. Navigation through the commands and selecting a command will be possible via key input. There are special cases like changing keybinds or choosing a name for a new save file, where the input is handled differently. Hermes itself supports its own Text Adventure game, called `base`. It is the only exception of a Text Adventure not being within the `mods` folder and should not be edited by the user.

### Platform support
Hermes currently supports only Windows, however the project structure allows easy multi-platform support, if needed.

***

## Modding
Hermes allows their userbase to create their own Text Adventures. Mods are stored in a folder called `mods`. Create the folder, if not already done.

### Mod Setup
Inside the `mods` folder create a folder for your Text Adventure. You can name it however you want, however ideally it should be the same as your Text Adventure name. 

### Serializable Data
During the process of modding your own Text Adventure, you will deal with a lot of file editing. Hermes has its own file format for reading and writing files. You have to follow the rules of Hermes' file format, if you want your files to be loaded correctly.

An example file could look like this:
```
example:
 key=value
# This is a comment!
 foo=bar
 parent:
  child0=foo
  child1=bar
```

Hermes files work with so called **key-value pairs** and a **parent-children hierarchy**. You can interpret each line as a **node**. If a line starts with a `#`, the line will be ignored, because it will be interpreted as a comment. **Parent nodes** are indicated by the `:` and have no value. A **parent node** can have multiple other nodes as **children**. Additionally, if you define a **child node**, it needs to be indentated with one whitespace relative to the **parent node**. **Child nodes** can be parent nodes** as well, they just need to use the `:` after the key and are not allowed to have a value. Thus, you can branch out and define each type of data in their own section. Because of this architecture, every file needs to define at least one parent node, otherwise Hermes won't know under which parent key to store your data! When it comes to the data itself, it will be defined as **key-value pairs** where the key and value are seperated with a '='. **Note**: There are no whitespaces needed in front of and after the `=`. If you define something like `foo = bar`, the key will be `foo ` and the value ` bar`, **including** the whitespaces!

### How mod loading works
Hermes will always try to load the base game. If you want Hermes to not load certain files from the base game, you need to define the exact same file in your mod folder and either clear its content or modify it however you like. Furthermore, Hermes will load every file inside your mod folder, if they are in the correct place. It is recommended to always create new files inside the mod folder when adding new content to the game and only copy and edit base game files when you want to remove content from the base game.

### Error handling
The Hermes application will create two log files on application termination, `Hermes.log` and `error.log`. If you are not dealing with any of Hermes' source code and just want to mod your Text Adventure, you can completely ignore the `Hermes.log`. Any errors regarding your mod should be found in `error.log`.

### Locales
You can translate your Text Adventure for multiple languages, so more people can enjoy playing it! Either way, you need to define at least one supported language for your Text Adventure. Inside your mod folder, create a folder called `locale`. Inside there, either create a `languages.txt` file or copy the one from the base game. This file is responsible for telling Hermes what languages your Text Adventure will support. Create a global parent node called `languages`. Each child of this parent node will be the key for any given language. Each of these child nodes have to be a parent node, whose children define all language keys with a translation as the value. The order of the translation key-value pairs have to match the order of the language keys. A `languages.txt` which defines the language support for English, Spanish, French, German and Italian would look like this:
```
languages:
 english:
  english=English
  spanish=Spanish
  french=French
  german=German
  italian=Italian
  
 spanish:
  english=Inglés
  spanish=Español
  french=Francés
  german=Alemán
  italian=Italiano
  
 french:
  english=Anglais
  spanish=Espagnol
  french=Français
  german=Allemand
  italian=Italien
  
 german:
  english=Englisch
  spanish=Spanisch
  french=Französisch
  german=Deutsch
  italian=Italienisch
  
 italian:
  english=Inglese
  spanish=Spagnolo
  french=Francese
  german=Tedesco
  italian=Italiano
```

Now inside the `locale` folder you will need to create a folder for each language key you defined in the `languages.txt`. The name of the folder must match the language key. For the example file shown above this would mean to create the following folders: `english`, `spanish`, `french`, `german` and `italian`. Inside these folders you can create any files you want and add your locales. **Note**: The global parent node of any of these files must match the name of the folder! For example an file for defining the name of an item in english could look like this:
```
english:
 IRON_SWORD=Iron Sword
```
Now you could use the key `IRON_SWORD` in any game file where you want to reference this particular item. Hermes will replace the key when rendering with the corresponding value depending on the language you currently play with.

***

## Known bugs
Currently none.

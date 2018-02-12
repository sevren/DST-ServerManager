# Dont Starve Together Server Manager
Kiril Tzvetanov Goguev -- 2018

## Summary

This project is a simple GUI application which can manage your Don't starve together dedicated server.

## Requirements

* Visual Studio 2017
* QT version 4.8+
* BOOST libraries
* PugiXML

### Folder Layout

Default install shall be Program Files/Sevsoft/DSTServerManager

```
/
.. /Resources/imgs
...../Resources/worldSettings.xml
...../Resources/maps.txt
../Libs/pugiXML
../ServerConfigs
..DSTServerManager.exe
```

Important files:
* worldSettings.xml - Contains the xml structure needed to enumerate the possible values for each resource setting of the game options
* maps.txt - Contains the mapping from human readable values in the game options to the lua values when writing the server configuration to disk. Required because the values in the Comboboxes in game options are not 1:1 for the lua server script file

## Server Token

1. Start Don't Starve Together
2. Click Acct Info button on the bottom of the screen
3. Click Generate Server Token button
4. Copy the token for use in the DstServerManager settings.
 

## General Usage

* Run DSTServerManager.exe
* Complete the first time setup by selecting a path to SteamCMD, Update te deafult path of the server configuration files if desired, add in the server token obtained from the don't starve together console
** Select File -> New to start creating a new server configuration
** Select File -> Open and navigate to the ini file for the server configuration you wish to open

Make your edits to your configuration and click save to write them disk. Save will output shortcuts to launch your dedicated server

### Presets

 Forest and Cave are the only presets supported currently
 
## Mod support
 
 There is currently no mod support.

## TODO:
CMakeLists.txt needs to be created to be able to quickly setup this project.

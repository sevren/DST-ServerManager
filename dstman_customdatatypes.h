#pragma once

#include <string>
#include <vector>
/*
* Intended to hold all the custom data types used in the dst manager.
*/

struct GlobalDSTManSettings
{
	std::string defaultServerFilePath;
	std::string steamCMDExecutablePath;
	std::string clusterTokenValue;

};

enum SettingsBtnType { serverFilePath, steamCMDPath};

// This struct should be passed around instead of multiple function signatures
struct UserSettings {
	GlobalDSTManSettings* dstManSettings;
	std::vector<std::vector<std::tuple<int, std::string, std::string, std::string>>> worldArrays;
};
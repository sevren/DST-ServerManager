#include "worldsettings.h"
#include <sstream>
#include <iostream>
#include <string>
#include <qsettings.h>
#include <fstream>
#include <boost/bimap.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/iter_find.hpp>
#include <QDebug>

using namespace std;
using namespace boost;

/*
* WorldSettings - This deals with the resource settings for each preset
* Takes in the appropriate arrays, augments the tab with the avatars and possible values for the correct resource
*/


//TODO: Allow data from the comboboxes to be read from the serverconfigurationtab
//TODO: Remove the debugging statements
worldsettings::worldsettings(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

worldsettings::worldsettings(gameOptions& worldArray, gameOptions& resourceArray, gameOptions& foodArray, gameOptions& animalsArray, gameOptions& monstersArray, QSettings& settingsIni, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	readMaps(); //need to know how to convert between the values used in the lua script and human readable values for setting the resources so we prepare a set of maps
	this->settingsIni = &settingsIni;
	setupUserGameOptionsScreen(worldArray, _WORLD_);
	setupUserGameOptionsScreen(resourceArray, _RESOURCES_);
	setupUserGameOptionsScreen(foodArray, _FOOD_);
	setupUserGameOptionsScreen(animalsArray, _ANIMALS_);
	setupUserGameOptionsScreen(monstersArray, _MONSTERS_);

}

worldsettings::~worldsettings()
{
}

string worldsettings::getWorldData()
{
	return getGameOptionSettings(ui.gridLayout);
}

string worldsettings::getResourceData()
{
	return getGameOptionSettings(ui.ResourcesGridLayout);
}

string worldsettings::getFoodData()
{
	return getGameOptionSettings(ui.FoodGridLayout);
}

string worldsettings::getAnimalData()
{
	return getGameOptionSettings(ui.AnimalsGridLayout);
}

string worldsettings::getMonstersData()
{
	return getGameOptionSettings(ui.MonstersGridLayout);
}

void worldsettings::setWorldData()
{
	setGameOptionSettings(ui.gridLayout);
}

void worldsettings::setResourceData()
{
	setGameOptionSettings(ui.ResourcesGridLayout);
}

void worldsettings::setFoodData()
{
	setGameOptionSettings(ui.FoodGridLayout);
}

void worldsettings::setAnimalData()
{
	setGameOptionSettings(ui.AnimalsGridLayout);
}

void worldsettings::setMonsterData()
{
	setGameOptionSettings(ui.MonstersGridLayout);
}

void worldsettings::setupUserGameOptionsScreen(gameOptions& gOArray, int dataType)
{
	QGridLayout* gLayout;



	switch (dataType)
	{
	case _WORLD_:
		gLayout = std::ref(ui.gridLayout);
		break;
	case _RESOURCES_:
		gLayout = std::ref(ui.ResourcesGridLayout);
		break;
	case _FOOD_:
		gLayout = std::ref(ui.FoodGridLayout);
		break;
	case _ANIMALS_:
		gLayout = std::ref(ui.AnimalsGridLayout);
		break;
	case _MONSTERS_:
		gLayout = std::ref(ui.MonstersGridLayout);
		break;
	}

	int indx = 0;
	int counter = 0;
	for (int gridRow = 0; gridRow < gOArray.size(); gridRow++)
	{
		for (int gridCol = 0; gridCol < 4; gridCol++)
		{
			if (indx >= gOArray.size())
			{
				break;
			}
			//	qDebug() << gridRow<<","<< gridCol<< "  counter is "<< counter << " index is" << indx;


			gLayout->addWidget(gOArray[indx].first, gridRow, gridCol);
			gLayout->addWidget(gOArray[indx].second, gridRow, gridCol + 1);

			counter++;
			if (counter % 2 == 0) //every 2 elements processed move onto next gameOption
			{
				indx++;
			}
		}
	}
}

string worldsettings::getGameOptionSettings(QGridLayout* gridLayout)
{
	//qDebug()<<gridLayout->count();
	settingsIni->beginGroup(gridLayout->objectName());
	stringstream settings;
	if ("FoodGridLayout" == gridLayout->objectName())
	{
		settings << "\tunprepared = { "<< luaCommentBlock;
	}
	else if ("gridLayout" == gridLayout->objectName())
	{
		settings << "\tmisc = { \n\t\t\t";
	}
	else if ("ResourcesGridLayout" == gridLayout->objectName())
	{
		settings << "\tresources = { " << luaCommentBlock;
	}
	else if ("AnimalsGridLayout" == gridLayout->objectName())
	{
		settings << "\tanimals = { " << luaCommentBlock;
	}
	else if ("MonstersGridLayout" == gridLayout->objectName())
	{
		settings << "\tmonsters = { " << luaCommentBlock;
	}


	if ("gridLayout" == gridLayout->objectName())
	{
		for (int i = 0; i < gridLayout->count(); ++i)
		{
			QWidget *widget = gridLayout->itemAt(i)->widget();
			if ((widget != NULL) && (widget->inherits("QComboBox")))
			{
				QComboBox* tempComboBox = qobject_cast<QComboBox*>(widget);
				settings << tempComboBox->objectName().toStdString() << "=" << '"' << handleWorldData(tempComboBox->objectName().toStdString(), tempComboBox->itemData(tempComboBox->currentIndex(), Qt::DisplayRole).toString().toStdString()) << '"' << ",\n\t\t\t";
				//settingsIni->setValue(tempComboBox->objectName(),tempComboBox->itemData(tempComboBox->currentIndex(),Qt::DisplayRole));
				settingsIni->setValue(tempComboBox->objectName(), tempComboBox->currentIndex());
			}

		}
	}
	else
	{
		for (int i = 0; i < gridLayout->count(); ++i)
		{
			QWidget *widget = gridLayout->itemAt(i)->widget();
			if ((widget != NULL) && (widget->inherits("QComboBox")))
			{
				QComboBox* tempComboBox = qobject_cast<QComboBox*>(widget);
				settings << tempComboBox->objectName().toStdString() << "=" << '"' << convertComboData(tempComboBox->itemData(tempComboBox->currentIndex(), Qt::DisplayRole).toString().toStdString()) << '"' << ",\n\t\t\t";
				//settingsIni->setValue(tempComboBox->objectName(),tempComboBox->itemData(tempComboBox->currentIndex(),Qt::DisplayRole));
				settingsIni->setValue(tempComboBox->objectName(), tempComboBox->currentIndex());
			}

		}
	}
	settingsIni->endGroup();
	return settings.str();
}


void worldsettings::setGameOptionSettings(QGridLayout* gridLayout)
{
	//qDebug()<<gridLayout->count();
	settingsIni->beginGroup(gridLayout->objectName());

	if ("gridLayout" == gridLayout->objectName())
	{
		for (int i = 0; i < gridLayout->count(); ++i)
		{
			QWidget *widget = gridLayout->itemAt(i)->widget();
			if ((widget != NULL) && (widget->inherits("QComboBox")))
			{
				QComboBox* tempComboBox = qobject_cast<QComboBox*>(widget);
				tempComboBox->setCurrentIndex(this->settingsIni->value(tempComboBox->objectName()).toInt());
			}

		}
	}
	else
	{
		for (int i = 0; i < gridLayout->count(); ++i)
		{
			QWidget *widget = gridLayout->itemAt(i)->widget();
			if ((widget != NULL) && (widget->inherits("QComboBox")))
			{
				QComboBox* tempComboBox = qobject_cast<QComboBox*>(widget);
				tempComboBox->setCurrentIndex(this->settingsIni->value(tempComboBox->objectName()).toInt());
			}

		}
	}
	settingsIni->endGroup();
}

string worldsettings::handleWorldData(string name, string pickedItem)
{
	if (("autumn" == name) || ("spring" == name) || ("summer" == name) || ("winter" == name))
	{
		//autumn = "default", -- "noseason", "veryshortseason", "shortseason", "default", "longseason", "verylongseason", "random"
		gameOptionsMapping::left_const_iterator iter = seasons_map.left.find(pickedItem);
		return iter->second.c_str();
	}
	else if ("branching" == name) //can be converted to lower case with spaces removed
	{
		gameOptionsMapping::left_const_iterator iter = branching_map.left.find(pickedItem);
		return iter->second.c_str();

	}
	else if ("cavelight" == name) //can be converted to lower case with spaces removed.
	{
		gameOptionsMapping::left_const_iterator iter = cavelight_map.left.find(pickedItem);
		return iter->second.c_str();

	}
	else if ("day" == name) //can just be trimmed and spaces removed and converted to lower case
	{
		//day = "default", -- "default", "longday", "longdusk", "longnight", "noday", "nodusk", "nonight", "onlyday", "onlydusk", "onlynight"
		gameOptionsMapping::left_const_iterator iter = day_map.left.find(pickedItem);
		return iter->second.c_str();
	}
	else if ("loop" == name) //can just be converted to lower case
	{
		gameOptionsMapping::left_const_iterator iter = loop_map.left.find(pickedItem);
		return iter->second.c_str();

	}
	else if ("regrowth" == name) //can be converted to lower case and spaces removed.
	{
		gameOptionsMapping::left_const_iterator iter = regrowth_map.left.find(pickedItem);
		return iter->second.c_str();
	}
	else if ("season_start" == name)
	{
		//season_start = "default", -- "default", "winter", "spring", "summer", "autumnorspring", "winterorsummer", "random"
		gameOptionsMapping::left_const_iterator iter = seasonstart_map.left.find(pickedItem);
		return iter->second.c_str();
	}
	else if ("start_location" == name)
	{
		gameOptionsMapping::left_const_iterator iter = startlocation_map.left.find(pickedItem);
		return iter->second.c_str();
	}
	else if ("task_set" == name)//biomes
	{
		//task_set = "cave_default", -- "classic", "default", "cave_default"
		gameOptionsMapping::left_const_iterator iter = taskset_map.left.find(pickedItem);
		return iter->second.c_str();
	}
	else if ("world_size" == name)
	{
		gameOptionsMapping::left_const_iterator iter = worldsize_map.left.find(pickedItem);
		return iter->second.c_str();
	}
	else
	{
		gameOptionsMapping::left_const_iterator iter = other_map.left.find(pickedItem);
		return iter->second.c_str();
	}

}

string worldsettings::convertComboData(string pickedItem)
{
	gameOptionsMapping::left_const_iterator iter = other_map.left.find(pickedItem);
	return iter->second.c_str();
}

gameOptionsMapping* worldsettings::getGameOptionsMapping(string name)
{
	if ("seasons_map" == name)
	{
		return &seasons_map;
	}
	else if ("branching_map" == name)
	{
		return &branching_map;
	}
	else if ("cavelight_map" == name)
	{
		return &cavelight_map;
	}
	else if ("day_map" == name)
	{
		return &day_map;
	}
	else if ("loop_map" == name)
	{
		return &loop_map;
	}
	else if ("regrowth_map" == name)
	{
		return &regrowth_map;
	}
	else if ("seasonstart_map" == name)
	{
		return &seasonstart_map;
	}
	else if ("startlocation_map" == name)
	{
		return &startlocation_map;
	}
	else if ("taskset_map" == name)
	{
		return &taskset_map;
	}
	else if ("worldsize_map" == name)
	{
		return &worldsize_map;
	}
	else if ("other_map" == name)
	{
		return &other_map;
	}
}

void worldsettings::readMaps()
{
	string line;
	string currentMapName = "";
	gameOptionsMapping *currentMap;

	ifstream myfile(".\\Resources\\maps.txt");
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			qDebug() << line.c_str() << '\n';
			if (starts_with(line, "--") && (contains(line, "seasons_map")
				|| contains(line, "branching_map")
				|| contains(line, "cavelight_map")
				|| contains(line, "day_map")
				|| contains(line, "loop_map")
				|| contains(line, "regrowth_map")
				|| contains(line, "seasonstart_map")
				|| contains(line, "startlocation_map")
				|| contains(line, "taskset_map")
				|| contains(line, "worldsize_map")
				|| contains(line, "other_map")))
			{
				currentMapName = line.substr(2, line.length()).c_str();
			}
			else if (line != "")
			{
				qDebug() << QString::fromStdString(currentMapName);
				currentMap = getGameOptionsMapping(currentMapName);
				vector<string> extractedTokens;

				char_separator<char> sep("=");
				tokenizer< char_separator<char> > tokens(line, sep);

				BOOST_FOREACH(const string& t, tokens) {
					qDebug() << QString::fromStdString(t);
					extractedTokens.push_back(t);
					//stringList.append(QString::fromStdString(t));

				}
				currentMap->insert(gameOptionsMapping::value_type(extractedTokens.at(0), extractedTokens.at(1)));
			}

		}
		myfile.close();
	}

	else qDebug() << "Unable to open file";
}

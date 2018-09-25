#ifndef WORLDSETTINGS_H
#define WORLDSETTINGS_H

#include <QWidget>
#include <qlabel.h>
#include <qcombobox.h>
#include <boost/bimap.hpp>
#include "ui_worldsettings.h"
#include "qsettings.h"


// defines for switch cases dealing with ui
#define _WORLD_ 0
#define _RESOURCES_ 1
#define _FOOD_ 2
#define _ANIMALS_ 3
#define _MONSTERS_ 4

using namespace std;

typedef vector<pair<QLabel*, QComboBox*>> gameOptions;
typedef vector<std::tuple<int, std::string, std::string, std::string>> xmlDataValues;
typedef boost::bimap< std::string, std::string> gameOptionsMapping;





class worldsettings : public QWidget
{
	Q_OBJECT

public:
	worldsettings(QWidget *parent = Q_NULLPTR);
	worldsettings(gameOptions&, gameOptions&, gameOptions&, gameOptions&, gameOptions&, QSettings&, QWidget *parent = 0);
	~worldsettings();
	string getWorldData();
	string getResourceData();
	string getFoodData();
	string getAnimalData();
	string getMonstersData();

	void setWorldData();
	void setResourceData();
	void setFoodData();
	void setAnimalData();
	void setMonsterData();




private:

	//we need bi directional maps because the value to display value are completely diffrent :(
	gameOptionsMapping seasons_map;
	gameOptionsMapping branching_map;
	gameOptionsMapping cavelight_map;
	gameOptionsMapping day_map;
	gameOptionsMapping loop_map;
	gameOptionsMapping regrowth_map;
	gameOptionsMapping seasonstart_map;
	gameOptionsMapping startlocation_map;
	gameOptionsMapping taskset_map;
	gameOptionsMapping worldsize_map;
	gameOptionsMapping other_map;

	QSettings *settingsIni;

	Ui::worldsettings ui;
	void worldsettings::setupUserGameOptionsScreen(gameOptions& gOArray, int dataType);
	string getGameOptionSettings(QGridLayout * gridLayout);
	void worldsettings::setGameOptionSettings(QGridLayout* gridLayout);
	string handleWorldData(string name, string pickedItem);
	string convertComboData(string pickedItem);

	gameOptionsMapping * getGameOptionsMapping(string name);
	void worldsettings::readMaps();

	const char* luaCommentBlock = "-- \"never\", \"rare\", \"default\", \"often\", \"always\"\n\t\t\t";
};
#endif 
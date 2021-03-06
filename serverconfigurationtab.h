#ifndef SERVERCONFIGURATIONTAB_H
#define SERVERCONFIGURATIONTAB_H

// defines for switch cases dealing with ui
#define _WORLD_ 0
#define _RESOURCES_ 1
#define _FOOD_ 2
#define _ANIMALS_ 3
#define _MONSTERS_ 4

#include <QWidget>
#include <qgridlayout.h>
#include <boost/bimap.hpp>
#include "ui_serverconfigurationtab.h"
#include "qsettings.h"
#include "worldsettings.h"
#include "dstman_customdatatypes.h"

using namespace std;

typedef vector<pair<QLabel*,QComboBox*>> gameOptions;
typedef vector<std::tuple<int,std::string,std::string,std::string>> xmlDataValues;
typedef boost::bimap< std::string, std::string> gameOptionsMapping;


class serverconfigurationtab : public QWidget
{
	Q_OBJECT
	

public:
	serverconfigurationtab(QString preset,QString serverDirectoryPath, QImage*, xmlDataValues&,xmlDataValues&,xmlDataValues&,xmlDataValues&,xmlDataValues&,bool,QWidget *parent = 0);
	serverconfigurationtab(QString preset, QString serverDirectoryPath, QImage*, UserSettings*, bool, QWidget *parent = 0);
	serverconfigurationtab(QString preset, QString serverDirectoryPath, QImage* avatars, xmlDataValues& ForestWorldArray, xmlDataValues& ForestResourcesArray, xmlDataValues& ForestFoodArray, xmlDataValues& ForestAnimalsArray, xmlDataValues& ForestMonstersArray, xmlDataValues& CavesworldArray, xmlDataValues& caveResourcesArray, xmlDataValues& caveFoodArray, xmlDataValues& caveAnimalsArray, xmlDataValues& caveMonstersArray, bool,QWidget *parent = 0);
	serverconfigurationtab(QString fileNameToOpen,QImage*, UserSettings*,bool,QWidget *parent = 0);
	~serverconfigurationtab();



private slots:
		void saveSettings();
		void run();

private:
	Ui::serverconfigurationtab ui;
	QString serverDirectoryPath;
	bool linked;
	QString preset;
	QSettings* settingsIni;
	worldsettings* wforestSettings;
	worldsettings* wCaveSettings;

	UserSettings* userSettings;
	

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

	//Need to keep track of all the presets

	//Forest
	gameOptions ForestWorldArray;
	gameOptions ForestResourcesArray;
	gameOptions ForestFoodArray;
	gameOptions ForestAnimalsArray;
	gameOptions ForestMonstersArray;

	//Caves
	gameOptions CaveWorldArray;
	gameOptions CaveResourcesArray;
	gameOptions CaveFoodArray;
	gameOptions CaveAnimalsArray;
	gameOptions CaveMonstersArray;

	//Shipwrecked--eventually

	QStringList serverconfigurationtab::setComboBoxValues(string values);
	gameOptions serverconfigurationtab::fillGameOptions(xmlDataValues&, QImage*);
	string  serverconfigurationtab::getServerConfigSettings(QGroupBox* groupBox);
	void serverconfigurationtab::setServerConfigSettings(QGroupBox* groupBox);
	string  serverconfigurationtab::getGameOptionSettings(QGridLayout* gridLayout);
	void  serverconfigurationtab::setGameOptionSettings(QGridLayout* gridLayout);
	string serverconfigurationtab::convertComboData(string pickedItem);
	string serverconfigurationtab::handleWorldData(string name,string pickedItem);
	string serverconfigurationtab::fetchPreset();
	QString serverconfigurationtab::setupFolders();
	int serverconfigurationtab::setRandomPort(int,int);
	void serverconfigurationtab::saveServerManConfig();
	bool serverconfigurationtab::serverStatus();
	void serverconfigurationtab::readMaps();
	gameOptionsMapping* serverconfigurationtab::getGameOptionsMapping(string);
};

#endif // SERVERCONFIGURATIONTAB_H

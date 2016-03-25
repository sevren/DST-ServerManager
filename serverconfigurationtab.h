#ifndef SERVERCONFIGURATIONTAB_H
#define SERVERCONFIGURATIONTAB_H

// defines for switch cases dealing with ui
#define _WORLD_ 0
#define _RESOURCES_ 1
#define _FOOD_ 2
#define _ANIMALS_ 3
#define _MONSTERS_ 4

#include <QWidget>
#include "ui_serverconfigurationtab.h"

using namespace std;

typedef vector<pair<QLabel*,QComboBox*>> gameOptions;
typedef vector<tuple<int,string,string>> xmlDataValues;




class serverconfigurationtab : public QWidget
{
	Q_OBJECT
	

public:
	serverconfigurationtab(QString preset, QImage*, xmlDataValues&,xmlDataValues&,xmlDataValues&,xmlDataValues&,xmlDataValues&,QWidget *parent = 0);
	~serverconfigurationtab();

private slots:
		void saveSettings();

private:
	Ui::serverconfigurationtab ui;

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
	void serverconfigurationtab::setupUserGameOptionsScreen(gameOptions& gOArray, int dataType);
	string  serverconfigurationtab::getServerConfigSettings(QGroupBox* groupBox);
};

#endif // SERVERCONFIGURATIONTAB_H

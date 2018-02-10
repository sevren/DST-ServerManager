#ifndef WORLDSETTINGS_H
#define WORLDSETTINGS_H

#include <QWidget>
#include <qlabel.h>
#include <qcombobox.h>
#include <boost/bimap.hpp>
#include "ui_worldsettings.h"


// defines for switch cases dealing with ui
#define _WORLD_ 0
#define _RESOURCES_ 1
#define _FOOD_ 2
#define _ANIMALS_ 3
#define _MONSTERS_ 4

using namespace std;

typedef vector<pair<QLabel*, QComboBox*>> gameOptions;
typedef vector<std::tuple<int, std::string, std::string, std::string>> xmlDataValues;



class worldsettings : public QWidget
{
	Q_OBJECT

public:
	worldsettings(QWidget *parent = Q_NULLPTR);
	worldsettings(gameOptions&, gameOptions&, gameOptions&, gameOptions&, gameOptions&, QWidget *parent = 0);
	~worldsettings();

private:
	Ui::worldsettings ui;
	void worldsettings::setupUserGameOptionsScreen(gameOptions& gOArray, int dataType);
};
#endif 
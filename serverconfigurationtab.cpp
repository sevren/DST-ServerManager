#include <tuple>
#include <string>
#include "serverconfigurationtab.h"
#include "dstman_customdatatypes.h"
#include "worldsettings.h"
#include <qimage.h>
#include <QDebug>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/iter_find.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <qdir.h>
#include <qmessagebox.h>
#include <random>
#include <boost/bimap.hpp>
#include <qsettings.h>


using namespace std;
using namespace boost;

/*
* ServerConfigurationTab - This deals with the tab holding all the server configuration data.
* Creates the tabs for the world settings based on the preset selected during the creation or the 
* preset that was read when the server configuration file was opened.
*
* Requires a map file to be read in so we can convert values displayed in the user interface to the ones required in the server configuration lua. Since it is not 1:1 
*
* Creates a server configuration INI file when a new serverconfiguration tab is created, this file tells dstservermanager what kind of preset is used when opening the server configuation
* Responsible for writing the relevant files out to disk when clicking the save button.
* A shorcut for starting the server with the correct config path should be created once save is clicked.
*/

//TODO: Change the name of the server configuration file from testing.ini to something better
//TODO: Migrate some of the code dealing with the resource settings to the worldsettings class
//TODO: Fix the save function so that it can fetch data from the worldsettings class
//TODO: Create a intelligent object to hold multiple resource arrays for preset and maps and pass that in to the constructor instead of multiple arrays.
//TODO: Create a class responsible soley for writing out the data to a file for the server + gameoptions
//TODO: Remove unused code

serverconfigurationtab::serverconfigurationtab(QString preset,QString serverDirectoryPath,QImage*avatars, xmlDataValues& WorldArray,xmlDataValues& ResourcesArray,xmlDataValues& FoodArray,xmlDataValues& AnimalsArray,xmlDataValues& MonstersArray,bool linked,QWidget *parent)
	: QWidget(parent)
{

	ui.setupUi(this);

	
	this->serverDirectoryPath=serverDirectoryPath;
	this->linked=linked;
	this->preset=preset;
	readMaps(); //need to know how to convert between the values used in the lua script and human readable values for setting the resources so we prepare a set of maps
	QString settingsPath=serverDirectoryPath+QString(QDir::separator());
	
	//QSettings s(QString(settingsPath+QString("testing")+".ini"),QSettings::IniFormat);
	this->settingsIni=new QSettings(QString(settingsPath+QString("testing")+".ini"),QSettings::IniFormat);
	this->settingsIni->setValue("preset",preset);
	this->settingsIni->setValue("linked",linked);
	
	//Fill the appropriate gameOption arrays depending on the preset choosen
	if ("Forest"==preset)
	{
		
		ForestWorldArray=fillGameOptions(WorldArray,avatars);
		ForestResourcesArray=fillGameOptions(ResourcesArray,avatars);
		ForestFoodArray=fillGameOptions(FoodArray,avatars);
		ForestAnimalsArray=fillGameOptions(AnimalsArray,avatars);
		ForestMonstersArray=fillGameOptions(MonstersArray,avatars);
		wforestSettings =new worldsettings(ForestWorldArray, ForestResourcesArray, ForestFoodArray, ForestAnimalsArray, ForestMonstersArray,*settingsIni);
		ui.serverDefn->addTab(wforestSettings, "Forest Options");

	
	}
	else if ("Cave"==preset)
	{
		CaveWorldArray=fillGameOptions(WorldArray,avatars);
		CaveResourcesArray=fillGameOptions(ResourcesArray,avatars);
		CaveFoodArray=fillGameOptions(FoodArray,avatars);
		CaveAnimalsArray=fillGameOptions(AnimalsArray,avatars);
		CaveMonstersArray=fillGameOptions(MonstersArray,avatars);
		wCaveSettings = new worldsettings(CaveWorldArray, CaveResourcesArray, CaveFoodArray, CaveAnimalsArray, CaveMonstersArray, *settingsIni);
		ui.serverDefn->addTab(wCaveSettings, "Cave Options");

	}
	

	ui.authentication_port->setValue(setRandomPort(8766,65535));
	ui.master_server_port->setValue(setRandomPort(27016,65535));
	ui.server_port->setValue(setRandomPort(10999,11018));
	

	if (linked && "Forest"==preset)
	{
		ui.shard_enabled->setChecked(true);
		ui.is_master->setChecked(true);
		ui.cluster_key->setText("41AF3DC");

	}
	else if (linked && "Cave"==preset)
	{
		ui.shard_enabled->setChecked(true);
		ui.is_master->setChecked(false);
		ui.cluster_key->setText("41AF3DC");
		ui.id->setText(QString::number(setRandomPort(0,65535)));
		
	}

	connect(ui.saveBtn,SIGNAL(clicked()),this,SLOT(saveSettings()));
	connect(ui.runBtn,SIGNAL(clicked()),this,SLOT(run()));
	
}


serverconfigurationtab::serverconfigurationtab(QString preset, QString serverDirectoryPath, QImage*avatars, UserSettings* userSettings, bool linked, QWidget *parent)
	: QWidget(parent)
{

	ui.setupUi(this);
	this->userSettings = userSettings;

	this->serverDirectoryPath = serverDirectoryPath;
	this->linked = linked;
	this->preset = preset;
	readMaps(); //need to know how to convert between the values used in the lua script and human readable values for setting the resources so we prepare a set of maps
	QString settingsPath = serverDirectoryPath + QString(QDir::separator());

	//QSettings s(QString(settingsPath+QString("testing")+".ini"),QSettings::IniFormat);
	this->settingsIni = new QSettings(QString(settingsPath + QString("testing") + ".ini"), QSettings::IniFormat);
	this->settingsIni->setValue("preset", preset);
	this->settingsIni->setValue("linked", linked);

	//Fill the appropriate gameOption arrays depending on the preset choosen
	if ("Forest" == preset)
	{
		/*for (std::vector<xmlDataValues>::iterator it = this->userSettings->worldArrays.begin(); it != this->userSettings->worldArrays.end(); ++it)
		{
			std::cout << ' ' << *it;
			std::cout << '\n';
		}*/
		
		ForestWorldArray = fillGameOptions(this->userSettings->worldArrays.at(0), avatars);
		ForestResourcesArray = fillGameOptions(this->userSettings->worldArrays.at(1), avatars);
		ForestFoodArray = fillGameOptions(this->userSettings->worldArrays.at(2), avatars);
		ForestAnimalsArray = fillGameOptions(this->userSettings->worldArrays.at(3), avatars);
		ForestMonstersArray = fillGameOptions(this->userSettings->worldArrays.at(4), avatars);

		wforestSettings = new worldsettings(ForestWorldArray, ForestResourcesArray, ForestFoodArray, ForestAnimalsArray, ForestMonstersArray, *settingsIni);
		ui.serverDefn->addTab(wforestSettings, "Forest Options");


	}
	else if ("Cave" == preset)
	{
		CaveWorldArray = fillGameOptions(this->userSettings->worldArrays.at(0), avatars);
		CaveResourcesArray = fillGameOptions(this->userSettings->worldArrays.at(1), avatars);
		CaveFoodArray = fillGameOptions(this->userSettings->worldArrays.at(2), avatars);
		CaveAnimalsArray = fillGameOptions(this->userSettings->worldArrays.at(3), avatars);
		CaveMonstersArray = fillGameOptions(this->userSettings->worldArrays.at(4), avatars);
		wCaveSettings = new worldsettings(CaveWorldArray, CaveResourcesArray, CaveFoodArray, CaveAnimalsArray, CaveMonstersArray, *settingsIni);
		ui.serverDefn->addTab(wCaveSettings, "Cave Options");

	}
	else if ("Both" == preset) 
	{


		ForestWorldArray = fillGameOptions(this->userSettings->worldArrays.at(0), avatars);
		ForestResourcesArray = fillGameOptions(this->userSettings->worldArrays.at(1), avatars);
		ForestFoodArray = fillGameOptions(this->userSettings->worldArrays.at(2), avatars);
		ForestAnimalsArray = fillGameOptions(this->userSettings->worldArrays.at(3), avatars);
		ForestMonstersArray = fillGameOptions(this->userSettings->worldArrays.at(4), avatars);

		wforestSettings = new worldsettings(ForestWorldArray, ForestResourcesArray, ForestFoodArray, ForestAnimalsArray, ForestMonstersArray, *settingsIni);

		ui.serverDefn->addTab(wforestSettings, "Forest Options");


		CaveWorldArray = fillGameOptions(this->userSettings->worldArrays.at(5), avatars);
		CaveResourcesArray = fillGameOptions(this->userSettings->worldArrays.at(6), avatars);
		CaveFoodArray = fillGameOptions(this->userSettings->worldArrays.at(7), avatars);
		CaveAnimalsArray = fillGameOptions(this->userSettings->worldArrays.at(8), avatars);
		CaveMonstersArray = fillGameOptions(this->userSettings->worldArrays.at(9), avatars);
		wCaveSettings = new worldsettings(CaveWorldArray, CaveResourcesArray, CaveFoodArray, CaveAnimalsArray, CaveMonstersArray, *settingsIni);
		ui.serverDefn->addTab(wCaveSettings, "Cave Options");

		ui.shard_enabled->setChecked(true);
		ui.is_master->setChecked(true);
		ui.cluster_key->setText("41AF3DC");

	}


	ui.authentication_port->setValue(setRandomPort(8766, 65535));
	ui.master_server_port->setValue(setRandomPort(27016, 65535));
	ui.server_port->setValue(setRandomPort(10999, 11018));


	if (linked && "Forest" == preset)
	{
		ui.shard_enabled->setChecked(true);
		ui.is_master->setChecked(true);
		ui.cluster_key->setText("41AF3DC");

	}
	else if (linked && "Cave" == preset)
	{
		ui.shard_enabled->setChecked(true);
		ui.is_master->setChecked(false);
		ui.cluster_key->setText("41AF3DC");
		ui.id->setText(QString::number(setRandomPort(0, 65535)));

	}

	connect(ui.saveBtn, SIGNAL(clicked()), this, SLOT(saveSettings()));
	connect(ui.runBtn, SIGNAL(clicked()), this, SLOT(run()));

}

serverconfigurationtab::serverconfigurationtab(QString fileToOpen,QImage*avatars, UserSettings* userSettings,bool linked,QWidget *parent)
	: QWidget(parent)
{

	this->settingsIni=new QSettings(fileToOpen,QSettings::IniFormat);
	qDebug() << "Listing all the keys";
	qDebug() << this->settingsIni->allKeys();
	serverDirectoryPath=QFileInfo(fileToOpen).path();
	qDebug() << "The Set server directory path is: "<<serverDirectoryPath;
	preset=this->settingsIni->value("preset").toString();
	linked=this->settingsIni->value("linked").toBool();
	qDebug() << "preset is "<< preset;
	qDebug() << "islinked" << linked;


	//serverconfigurationtab* x=new serverconfigurationtab(preset,serverDirectoryPath,avatars,WorldArray,ResourcesArray,FoodArray,AnimalsArray,MonstersArray,linked,this);
	serverconfigurationtab* x = new serverconfigurationtab(preset, serverDirectoryPath, avatars, userSettings, linked, this);
	setServerConfigSettings(x->ui.gameplaySettings);
	setServerConfigSettings(x->ui.networkSettings);
	setServerConfigSettings(x->ui.miscSettings);
	setServerConfigSettings(x->ui.shardSettings);
	setServerConfigSettings(x->ui.steamSettings);

	if ("Forest" == preset)
	{
		x->wforestSettings->setFoodData();
		x->wforestSettings->setWorldData();
		x->wforestSettings->setAnimalData();
		x->wforestSettings->setMonsterData();
		x->wforestSettings->setResourceData();
	}
	else if ("Cave" == preset)
	{
		x->wCaveSettings->setFoodData();
		x->wCaveSettings->setWorldData();
		x->wCaveSettings->setAnimalData();
		x->wCaveSettings->setMonsterData();
		x->wCaveSettings->setResourceData();
	
	}


	/*setGameOptionSettings(x->ui.FoodGridLayout);
	setGameOptionSettings(x->ui.gridLayout);
	setGameOptionSettings(x->ui.AnimalsGridLayout);
	setGameOptionSettings(x->ui.MonstersGridLayout);
	setGameOptionSettings(x->ui.ResourcesGridLayout);*/
	
	
}

serverconfigurationtab::serverconfigurationtab(QString preset, QString serverDirectoryPath, QImage*avatars, xmlDataValues& fWorldArray, xmlDataValues& fResourcesArray, xmlDataValues& fFoodArray, xmlDataValues& fAnimalsArray, xmlDataValues& fMonstersArray, xmlDataValues& cWorldArray, xmlDataValues& cResourcesArray, xmlDataValues& cFoodArray, xmlDataValues& cAnimalsArray, xmlDataValues& cMonstersArray, bool linked, QWidget *parent)
	: QWidget(parent)
{

	ui.setupUi(this);


	this->serverDirectoryPath = serverDirectoryPath;
	this->linked = linked;
	this->preset = preset;
	readMaps(); //need to know how to convert between the values used in the lua script and human readable values for setting the resources so we prepare a set of maps
	QString settingsPath = serverDirectoryPath + QString(QDir::separator());

	//QSettings s(QString(settingsPath+QString("testing")+".ini"),QSettings::IniFormat);
	this->settingsIni = new QSettings(QString(settingsPath + QString("testing") + ".ini"), QSettings::IniFormat);
	this->settingsIni->setValue("preset", preset);
	this->settingsIni->setValue("linked", linked);
	qDebug() << "The number of items in season_start is " << seasonstart_map.size();

	for (gameOptionsMapping::const_iterator iter = seasonstart_map.begin(), iend = seasonstart_map.end(); iter != iend; ++iter)
	{
		// iter->left  : data : std:string
		// iter->right : data : std::string

		qDebug() << iter->left.c_str() << " <--> " << iter->right.c_str();
	}

	//use left when going from gui to file && use right when going from file to gui
	gameOptionsMapping::left_const_iterator iter = seasonstart_map.left.find("Winter/Summer");
	qDebug() << iter->second.c_str();



	ForestWorldArray = fillGameOptions(fWorldArray, avatars);
	ForestResourcesArray = fillGameOptions(fResourcesArray, avatars);
	ForestFoodArray = fillGameOptions(fFoodArray, avatars);
	ForestAnimalsArray = fillGameOptions(fAnimalsArray, avatars);
	ForestMonstersArray = fillGameOptions(fMonstersArray, avatars);

	wforestSettings = new worldsettings(ForestWorldArray, ForestResourcesArray, ForestFoodArray, ForestAnimalsArray, ForestMonstersArray, *settingsIni);

	ui.serverDefn->addTab(wforestSettings, "Forest Options");


	CaveWorldArray = fillGameOptions(cWorldArray, avatars);
	CaveResourcesArray = fillGameOptions(cResourcesArray, avatars);
	CaveFoodArray = fillGameOptions(cFoodArray, avatars);
	CaveAnimalsArray = fillGameOptions(cAnimalsArray, avatars);
	CaveMonstersArray = fillGameOptions(cMonstersArray, avatars);
	wCaveSettings = new worldsettings(CaveWorldArray, CaveResourcesArray, CaveFoodArray, CaveAnimalsArray, CaveMonstersArray, *settingsIni);
	ui.serverDefn->addTab(wCaveSettings, "Cave Options");

	ui.authentication_port->setValue(setRandomPort(8766, 65535));
	ui.master_server_port->setValue(setRandomPort(27016, 65535));
	ui.server_port->setValue(setRandomPort(10999, 11018));


	if (linked && "Forest" == preset)
	{
		ui.shard_enabled->setChecked(true);
		ui.is_master->setChecked(true);
		ui.cluster_key->setText("41AF3DC");

	}
	else if (linked && "Cave" == preset)
	{
		ui.shard_enabled->setChecked(true);
		ui.is_master->setChecked(false);
		ui.cluster_key->setText("41AF3DC");
		ui.id->setText(QString::number(setRandomPort(0, 65535)));

	}



	connect(ui.saveBtn, SIGNAL(clicked()), this, SLOT(saveSettings()));
	connect(ui.runBtn, SIGNAL(clicked()), this, SLOT(run()));

}

serverconfigurationtab::~serverconfigurationtab()
{

}

int serverconfigurationtab::setRandomPort(int min,int max)
{
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(min,max); // guaranteed unbiased

	auto  random_integer = uni(rng);
	return random_integer;
}


QStringList serverconfigurationtab::setComboBoxValues(string values)
{
	QStringList stringList;
	//tokenize AcceptableValues and return as list
	char_separator<char> sep(",");
    tokenizer< char_separator<char> > tokens(values, sep);
    BOOST_FOREACH (const string& t, tokens) {
		//qDebug() << QString::fromStdString(t);
		stringList.append(QString::fromStdString(t));
    }
	return stringList;
}

gameOptions serverconfigurationtab::fillGameOptions(xmlDataValues& dataValues, QImage* avatars)
{
	gameOptions filledGameOptions;

	for (int i=0;i<dataValues.size();i++)
	{

		QLabel* qlbl = new QLabel();
		qlbl->setPixmap(QPixmap::fromImage(avatars[get<0>(dataValues[i])]));
		qlbl->setMinimumSize(QSize(50, 50));
		qlbl->setMaximumSize(QSize(50, 50));
		qlbl->setToolTip(QString(get<3>(dataValues[i]).c_str()));
		//qlbl->setToolTip(QString::fromStdString(worldN.node().name()));
		QComboBox* qcbox = new QComboBox();
		QStringList list=(setComboBoxValues(get<2>(dataValues[i])));
		qcbox->addItems(list);
		qcbox->setObjectName(QString(get<3>(dataValues[i]).c_str()));
		
		int index = qcbox->findData(QString::fromStdString(get<1>(dataValues[i])),Qt::DisplayRole);
		
		if ( index != -1 ) { // -1 for not found
			 qcbox->setCurrentIndex(index);
		}
		filledGameOptions.push_back(std::make_pair(qlbl,qcbox));
	}

	return filledGameOptions;
}



string  serverconfigurationtab::getServerConfigSettings(QGroupBox* groupBox)
{
	qDebug()<<groupBox->children();

	
	settingsIni->beginGroup(groupBox->objectName());
	
	QListIterator<QObject *> i(groupBox->children());
	stringstream settings;
	while (i.hasNext())
	{

		if (!(i.peekNext()->inherits("QLabel") || i.peekNext()->inherits("QPushButton") || i.peekNext()->inherits("QGridLayout")))
		{
			if (i.peekNext()->inherits("QSpinBox"))
			{
				QSpinBox * tempSpinBox =qobject_cast<QSpinBox *>(i.next());
				settings <<tempSpinBox->objectName().toStdString() << "=" << tempSpinBox->value()<<"\n";
				settingsIni->setValue(tempSpinBox->objectName(),tempSpinBox->value());
			}
			else if (i.peekNext()->inherits("QLineEdit"))
			{
				QLineEdit* tempLineEdit=qobject_cast<QLineEdit *>(i.next());
				settings << tempLineEdit->objectName().toStdString() << "=" << tempLineEdit->text().toStdString() <<"\n";
				settingsIni->setValue(tempLineEdit->objectName(),tempLineEdit->text());
			}
			else if (i.peekNext()->inherits("QCheckBox"))
			{
				QCheckBox* tempCheckBox=qobject_cast<QCheckBox *>(i.next());
				if ("is_master"!=tempCheckBox->objectName().toStdString())
				{
					settings << tempCheckBox->objectName().toStdString() << "=" <<((tempCheckBox->isChecked()) ? ("true") : ("false") ) <<"\n";
				}
				settingsIni->setValue(tempCheckBox->objectName(),((tempCheckBox->isChecked()) ? ("true") : ("false") ));

			}
			else if (i.peekNext()->inherits("QComboBox"))
			{
				QComboBox* tempComboBox=qobject_cast<QComboBox *>(i.next());
				settings << tempComboBox->objectName().toStdString() << "=" << tempComboBox->itemData(tempComboBox->currentIndex(),Qt::DisplayRole).toString().toStdString() <<"\n";
				//settingsIni->setValue(tempComboBox->objectName(),tempComboBox->itemData(tempComboBox->currentIndex(),Qt::DisplayRole).toString());
				settingsIni->setValue(tempComboBox->objectName(),tempComboBox->currentIndex());
			}

		}
		else
		{
			i.next(); //advance the iterator but don't do anything with the item because it's a label
		}
	}
	
	settingsIni->endGroup();
	return settings.str();
}

void  serverconfigurationtab::setServerConfigSettings(QGroupBox* groupBox)
{
	qDebug()<<groupBox->children();

	
	settingsIni->beginGroup(groupBox->objectName());
	
	QListIterator<QObject *> i(groupBox->children());
	//stringstream settings;
	while (i.hasNext())
	{

		if (!(i.peekNext()->inherits("QLabel") || i.peekNext()->inherits("QPushButton") || i.peekNext()->inherits("QGridLayout")))
		{
			if (i.peekNext()->inherits("QSpinBox"))
			{
				QSpinBox * tempSpinBox =qobject_cast<QSpinBox *>(i.next());
				tempSpinBox->setValue(this->settingsIni->value(tempSpinBox->objectName()).toInt());
				//settings <<tempSpinBox->objectName().toStdString() << "=" << tempSpinBox->value()<<"\n";
				//settingsIni->setValue(tempSpinBox->objectName(),tempSpinBox->value());
			}
			else if (i.peekNext()->inherits("QLineEdit"))
			{
				QLineEdit* tempLineEdit=qobject_cast<QLineEdit *>(i.next());
				qDebug() << "setting: " << tempLineEdit->objectName() << "to " << this->settingsIni->value(tempLineEdit->objectName()).toString();
				tempLineEdit->setText(this->settingsIni->value(tempLineEdit->objectName()).toString());
				
			}
			else if (i.peekNext()->inherits("QCheckBox"))
			{
				QCheckBox* tempCheckBox=qobject_cast<QCheckBox *>(i.next());
				if ("is_master"!=tempCheckBox->objectName().toStdString())
				{
					tempCheckBox->setChecked(this->settingsIni->value(tempCheckBox->objectName()).toBool());
					//settings << tempCheckBox->objectName().toStdString() << "=" <<((tempCheckBox->isChecked()) ? ("true") : ("false") ) <<"\n";
				}
				qDebug() << "setting: " << tempCheckBox->objectName() << "to " << this->settingsIni->value(tempCheckBox->objectName()).toBool();
				tempCheckBox->setChecked(this->settingsIni->value(tempCheckBox->objectName()).toBool());
				//settingsIni->setValue(tempCheckBox->objectName(),((tempCheckBox->isChecked()) ? ("true") : ("false") ));
				
			}
			else if (i.peekNext()->inherits("QComboBox"))
			{
				QComboBox* tempComboBox=qobject_cast<QComboBox *>(i.next());
				tempComboBox->setCurrentIndex(this->settingsIni->value(tempComboBox->objectName()).toInt());
				/*settings << tempComboBox->objectName().toStdString() << "=" << tempComboBox->itemData(tempComboBox->currentIndex(),Qt::DisplayRole).toString().toStdString() <<"\n";
				settingsIni->setValue(tempComboBox->objectName(),tempComboBox->itemData(tempComboBox->currentIndex(),Qt::DisplayRole).toString());*/
			}

		}
		else
		{
			i.next(); //advance the iterator but don't do anything with the item because it's a label
		}
	}
	
	settingsIni->endGroup();
}

//TODO: pass in list of folders to make and iterate through them
QString serverconfigurationtab::setupFolders()
{
	QString finalDir;
	if ("Forest"==preset)
	{
		finalDir="Master";
	}
	else if ("Cave"==preset)
	{
		finalDir="Cave";
	}
	else
	{
		QDir dir(serverDirectoryPath + QString(QDir::separator()) + "Master");
		if (!dir.exists()) {
			dir.mkpath(".");
		}
		dir = QDir(serverDirectoryPath + QString(QDir::separator()) + "Cave");
		if (!dir.exists()) {
			dir.mkpath(".");
		}
	}
	
	QDir dir(serverDirectoryPath+QString(QDir::separator())+finalDir);
	if (!dir.exists()) {
		dir.mkpath(".");
	}

	return serverDirectoryPath+QString(QDir::separator())+finalDir;

}

string serverconfigurationtab::fetchPreset()
{
	//preset = "DST_CAVE", --  or "SURVIVAL_TOGETHER" or "SURVIVAL_TOGETHER_CLASSIC" or "SURVIVAL_DEFAULT_PLUS" or "COMPLETE_DARKNESS" or "DST_CAVE"
	string preset="SURVIVAL_TOGETHER";
	
	return "\t\tpreset = \""+preset+"\",\n\t\t";;
}

void serverconfigurationtab::saveSettings()
{
	//save all settings to the cluster.ini, server.ini and worldoverridesettings.lua
	QString serverDirectoryPathFinal=setupFolders();
	qDebug() << "Saving to the following directory: " << serverDirectoryPath;

	QString filePathName=serverDirectoryPathFinal+QString(QDir::separator())+ui.name->text()+".dstman";
	ofstream myfile;
	
	filePathName=serverDirectoryPath+QString(QDir::separator())+"cluster.ini";
	myfile.open (filePathName.toStdString());
	myfile << "[GAMEPLAY]\n";
	myfile << getServerConfigSettings(ui.gameplaySettings) << "\n\n";
	myfile << "[NETWORK]\n";
	myfile << getServerConfigSettings(ui.networkSettings) << "\n\n";
	myfile << "[MISC]\n";
	myfile << getServerConfigSettings(ui.miscSettings) << "\n\n";
	myfile << "[SHARD]\n";
	myfile << getServerConfigSettings(ui.shardSettings) << "\n\n";
	myfile.close();
	
	filePathName=serverDirectoryPathFinal+QString(QDir::separator())+"server.ini";
	myfile.open (filePathName.toStdString());
	myfile << "[SHARD]\n";
	myfile << ui.is_master->objectName().toStdString() << "=" <<((ui.is_master->isChecked()) ? ("true") : ("false") ) <<"\n";
	settingsIni->setValue(ui.is_master->objectName(),((ui.is_master->isChecked()) ? ("true") : ("false") ));
	myfile << ui.name->objectName().toStdString() << "=" << ui.name->text().toStdString() <<"\n";
	settingsIni->setValue(ui.name->objectName(),ui.name->text());
	myfile << ui.id->objectName().toStdString() << "=" << ui.id->text().toStdString() <<"\n";
	settingsIni->setValue(ui.id->objectName(),ui.id->text());
	myfile << "[STEAM]\n";
	settingsIni->beginGroup(ui.steamSettings->objectName());
	settingsIni->setValue(ui.steam_group_id->objectName(),ui.steam_group_id->text());
	settingsIni->setValue(ui.steam_group_only->objectName(),((ui.steam_group_only->isChecked()) ? ("true") : ("false") ));
	settingsIni->setValue(ui.steam_group_admins->objectName(),((ui.steam_group_admins->isChecked()) ? ("true") : ("false") ));
	settingsIni->setValue(ui.authentication_port->objectName(),ui.authentication_port->value());
	settingsIni->setValue(ui.master_server_port->objectName(),ui.master_server_port->value());
	settingsIni->endGroup();
	myfile << ui.authentication_port->objectName().toStdString() << "=" << ui.authentication_port->value()<<"\n";
	
	myfile << ui.master_server_port->objectName().toStdString() << "=" << ui.master_server_port->value()<<"\n";
	
	
	myfile << "[NETWORK]\n";
	myfile << ui.server_port->objectName().toStdString() << "=" << ui.server_port->value()<<"\n";
	settingsIni->setValue(ui.server_port->objectName(),ui.server_port->value());
	myfile.close();


	filePathName = serverDirectoryPathFinal + QString(QDir::separator()) + "clustertoken.ini";
	myfile.open(filePathName.toStdString());
	myfile << this->userSettings->dstManSettings->clusterTokenValue << "\n";
	myfile.close();
	

	//myfile << fetchPreset();
	if ("Forest" == preset)
	{
		filePathName = serverDirectoryPathFinal + QString(QDir::separator())+ "worldgenoverride.lua";
		myfile.open(filePathName.toStdString());
		myfile << "return {\n \t\t override_enabled = true, \n";
		myfile << wforestSettings->getFoodData() << "},\n\n";
		myfile << wforestSettings->getWorldData() << "},\n\n";
		myfile << wforestSettings->getAnimalData() << "},\n\n";
		myfile << wforestSettings->getMonstersData() << "},\n\n";
		myfile << wforestSettings->getResourceData() << "}\n\n";
		myfile << "}";
		myfile.close();
	}
	else if ("Cave" == preset)
	{
		filePathName = serverDirectoryPathFinal + QString(QDir::separator()) + "worldgenoverride.lua";
		myfile.open(filePathName.toStdString());
		myfile << "return {\n \t\t override_enabled = true, \n";
		myfile << wCaveSettings->getFoodData() << "},\n\n";
		myfile << wCaveSettings->getWorldData() << "},\n\n";
		myfile << wCaveSettings->getAnimalData() << "},\n\n";
		myfile << wCaveSettings->getMonstersData() << "},\n\n";
		myfile << wCaveSettings->getResourceData() << "}\n\n";
		myfile << "}";
		myfile.close();
	}
	else
	{
	
		filePathName = serverDirectoryPathFinal + QString(QDir::separator()) + "Master" + QString(QDir::separator()) + "worldgenoverride.lua";
		myfile.open(filePathName.toStdString());
		myfile << wforestSettings->getFoodData() << "},\n\n";
		myfile << wforestSettings->getWorldData() << "},\n\n";
		myfile << wforestSettings->getAnimalData() << "},\n\n";
		myfile << wforestSettings->getMonstersData() << "},\n\n";
		myfile << wforestSettings->getResourceData() << "}\n\n";
		myfile << "}";
		myfile.close();

		filePathName = serverDirectoryPathFinal + QString(QDir::separator()) + "Cave" + QString(QDir::separator())  + "worldgenoverride.lua";
		myfile.open(filePathName.toStdString());
		myfile << wCaveSettings->getFoodData() << "},\n\n";
		myfile << wCaveSettings->getWorldData() << "},\n\n";
		myfile << wCaveSettings->getAnimalData() << "},\n\n";
		myfile << wCaveSettings->getMonstersData() << "},\n\n";
		myfile << wCaveSettings->getResourceData() << "}\n\n";
		myfile << "}";
		myfile.close();

	}
	
	//create shortcut based on preset
	

}

//TODO: Run should run the shortcut created from hitting the save button
void serverconfigurationtab::run()
{
	qDebug() << "Trying to run ...";
	//get steamcmd install location
	//run steamcmd command
	if (linked && serverStatus()) // QMSGBox start both if both down?
	{
		QMessageBox msgBox;
		msgBox.setText("Linked Servers are both down");
		msgBox.setInformativeText("Do you want to start both or only one?");
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Save);
		int ret = msgBox.exec();

		switch (ret) {
		  case QMessageBox::Save:
			  // Save was clicked
			  break;
		  case QMessageBox::Discard:
			  // Don't Save was clicked
			  break;
		  case QMessageBox::Cancel:
			  // Cancel was clicked
			  break;
		  default:
			  // should never be reached
			  break;
		}

	}
}


bool serverconfigurationtab::serverStatus()
{
	return true;
}

void serverconfigurationtab::readMaps()
{
  string line;
  string currentMapName="";
  gameOptionsMapping *currentMap;

  ifstream myfile (".\\Resources\\maps.txt");
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      qDebug() << line.c_str() << '\n';
	  if (starts_with(line,"--") && (contains(line,"seasons_map") 
									|| contains(line,"branching_map") 
									|| contains(line,"cavelight_map")
									|| contains(line,"day_map")
									|| contains(line,"loop_map") 
									|| contains(line,"regrowth_map") 
									|| contains(line,"seasonstart_map")
									|| contains(line,"startlocation_map")
									|| contains(line,"taskset_map")
									|| contains(line,"worldsize_map")
									|| contains(line,"other_map")))
	  {
		currentMapName= line.substr(2,line.length()).c_str();
	  }
	  else if (line !="")
	  {
		 qDebug() << QString::fromStdString(currentMapName);
		 currentMap=getGameOptionsMapping(currentMapName);
		   vector<string> extractedTokens;

		char_separator<char> sep("=");
		tokenizer< char_separator<char> > tokens(line, sep);
		
		BOOST_FOREACH (const string& t, tokens) {
			qDebug() << QString::fromStdString(t);
			extractedTokens.push_back(t);
			//stringList.append(QString::fromStdString(t));
			
		}
		currentMap->insert(gameOptionsMapping::value_type(extractedTokens.at(0),extractedTokens.at(1)));
	  }
	  
    }
    myfile.close();
  }

  else qDebug() << "Unable to open file";
}

gameOptionsMapping* serverconfigurationtab::getGameOptionsMapping(string name)
{
	if ("seasons_map"==name)
	{
		return &seasons_map;
	}
	else if ("branching_map"==name)
	{
		return &branching_map; 
	}
	else if("cavelight_map"==name)
	{
		return &cavelight_map; 
	}
	else if ("day_map"==name)
	{
		return &day_map; 
	}
	else if("loop_map"==name)
	{
		return &loop_map; 
	}
	else if("regrowth_map"==name)
	{
		return &regrowth_map; 
	}
	else if ("seasonstart_map"==name)
	{
		return &seasonstart_map; 
	}
	else if ("startlocation_map"==name)
	{
		return &startlocation_map; 
	}
	else if("taskset_map"==name)
	{
		return &taskset_map; 
	}
	else if ("worldsize_map"==name)
	{
		return &worldsize_map; 
	}
	else if("other_map"==name)
	{
		return &other_map; 
	}
}
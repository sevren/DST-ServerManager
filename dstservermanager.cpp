#include <iostream>
#include <string>
#include <tuple>
#include "dstservermanager.h"
#include "newdialog.h"
#include "dstmanagersettings.h"
#include <QDebug>
#include <qfiledialog.h>
#include "pugixml.cpp"
#include <qsettings.h>

using namespace std;

/*
* DstServerManager - This is the main window used for controling the application.
* One time initalization occurs as soon as this window is created
* 1. Read the image in resources folder and create an array to hold the icons("avatars") for resource configuration
* 2. Load the worldsettings xml to gather information on the values each resource preset can have in the game. i.e Forest & Day --> Default,Long Day,Long Dusk,Long Night,No Day,No Dusk,No Night,Only Day,Only Dusk,Only Night
* 3. Game Settings are parsed into Arrays which each  server configuratin can read from. At this time only 2 presets exist Forest and Cave.
* User can select File -> new to create a new server configuration
* User can select File -> open to open a previous server configuration
* User can select File -> quit to quit the application
* User can select Settings -> server manager settings to open and configure the server manager settings.
*/

//TODO: Load the dstmanagersettings at the start of the application
//TODO: Pop up the dstmanagersettings if not configured (i.e ini file does not exist..)
//TODO: Display error and quit application if xml documents cant be loaded properly
//TODO: Remove Debugging statements

dstServerManager::dstServerManager(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	dstWindow=this;

	dstManSettings = new GlobalDSTManSettings();



	if (dstManSettingsExist())
	{
		loadSettings();
	}
	else
	{
		settingsDialog();
	}
	setupConnections();

	setupMainLayout();
	
	QImage image(imgPath);
	QImage copy;
	
	int indx=0;
	for(int i=0;i<image.width();i+=128)
	{
		for (int j=0;j<768;j+=128)
		{
			copy = image.copy( i,j, 128, 128);
			copy=copy.scaled(50, 50, Qt::KeepAspectRatio);
			avatars[indx]=copy;
			indx++;
		}
	}
	

	
	
	pugi::xml_document doc;
	if (!doc.load_file(worldSettingsXmlPath))
	{
		qDebug() << "Errors parsing Document";
		//Quit Application if errors, display how to solve this if possible..
	}
  
	//Evaluate numbers via XPath Query

	qDebug() << "-------------Forest XML counts -------------";
	//Forest Counts
	pugi::xpath_query ForestWorldCounts(forestWorldCountsXpath);
	qDebug() << ForestWorldCounts.evaluate_number(doc);

	pugi::xpath_query ForestResourcesCounts(forestResourcesCountsXpath);
	qDebug() << ForestResourcesCounts.evaluate_number(doc);

	pugi::xpath_query ForestFoodCounts(forestFoodCountsXpath);
	qDebug() << ForestFoodCounts.evaluate_number(doc);

	pugi::xpath_query ForestAnimalsCounts(forestAnimalsCountsXpath);
	qDebug() << ForestAnimalsCounts.evaluate_number(doc);

	pugi::xpath_query ForestMonstersCounts(forestMonstersCountsXpath);
	qDebug() << ForestMonstersCounts.evaluate_number(doc);


	qDebug() << "-------------/End Forest XML counts -------------";

	qDebug() << "------------- Cave XML counts -------------";
	//Caves Counts

	pugi::xpath_query CaveWorldCounts(cavesWorldCountsXpath);
	qDebug() << CaveWorldCounts.evaluate_number(doc);

	pugi::xpath_query CaveResourcesCounts(cavesResourcesCountsXpath);
	qDebug() << CaveResourcesCounts.evaluate_number(doc);

	pugi::xpath_query CaveFoodCounts(cavesFoodCountsXpath);
	qDebug() << CaveFoodCounts.evaluate_number(doc);

	pugi::xpath_query CaveAnimalsCounts(cavesAnimalsCountsXpath);
	qDebug() << CaveAnimalsCounts.evaluate_number(doc);

	pugi::xpath_query CaveMonstersCounts(cavesMonstersXpath);
	qDebug() << CaveMonstersCounts.evaluate_number(doc);

	qDebug() << "-------------/End Cave XML counts -------------";

	//pugi::xpath_node_set worldNodes = doc.select_nodes("//Presets//Forest/World/*/IconNum/text()");

	/*for (pugi::xpath_node_set::const_iterator it = worldNodes.begin(); it != worldNodes.end(); ++it)
	{
		pugi::xpath_node worldN =*it;
		qDebug() << worldN.node().value();
	}*/

	qDebug() << "-------------Forest XML World Nodes -------------";
	

	ForestWorldArray=fillGameOptionsArray("//Presets//Forest/World/*");
	ForestResourcesArray=fillGameOptionsArray("//Presets//Forest/Resources/*");
	ForestFoodArray=fillGameOptionsArray("//Presets//Forest/Food/*");
	ForestAnimalsArray=fillGameOptionsArray("//Presets//Forest/Animals/*");
	ForestMonstersArray=fillGameOptionsArray("//Presets//Forest/Monsters/*");

	CaveWorldArray=fillGameOptionsArray("//Presets//Cave/World/*");
	CaveResourcesArray=fillGameOptionsArray("//Presets//Cave/Resources/*");
	CaveFoodArray=fillGameOptionsArray("//Presets//Cave/Food/*");
	CaveAnimalsArray=fillGameOptionsArray("//Presets//Cave/Animals/*");
	CaveMonstersArray=fillGameOptionsArray("//Presets//Cave/Monsters/*");

//	getData("sd","Both"); //Used for quickly debugging application, shortcut to opening a set of server files"

	
}

void dstServerManager::setupMainLayout()
{
	tabWidget = new QTabWidget();
	tabWidget->setTabsClosable(true);
	connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab_(int)));
	ui.gridLayout->addWidget(tabWidget);
}


void dstServerManager::closeTab_(int index)
{
	tabWidget->removeTab(index);
}

void dstServerManager::setupConnections()
{
	connect(ui.actionNew,SIGNAL(triggered()),this,SLOT(createNewServerConfig()));
	connect(ui.actionOpen,SIGNAL(triggered()),this,SLOT(openDialog()));
	connect(ui.actionServerManagerSettings, SIGNAL(triggered()), this, SLOT(settingsDialog()));
	
}


void dstServerManager::createNewServerConfig()
{
	NewDialog *newd= new NewDialog(dstWindow,dstManSettings);
	connect(newd,SIGNAL(sendData(QString,QString,QString)),this,SLOT(getData(QString,QString,QString)));
	newd->show();
}


void dstServerManager::getData(QString serverTabName, QString preset, QString serverDirectoryPath)
{
	qDebug() << serverTabName;
	qDebug() << preset;
	qDebug() << serverDirectoryPath;
	QDir dir(serverDirectoryPath);
	if (!dir.exists()) {
		dir.mkpath(dir.absolutePath());
	}
	if ("Forest"==preset)
	{
		UserSettings* userSettings = new UserSettings();
		userSettings->dstManSettings = this->dstManSettings;
		userSettings->worldArrays.push_back(ForestWorldArray);
		userSettings->worldArrays.push_back(ForestResourcesArray);
		userSettings->worldArrays.push_back(ForestFoodArray);
		userSettings->worldArrays.push_back(ForestAnimalsArray);
		userSettings->worldArrays.push_back(ForestMonstersArray);

		//tabWidget->addTab(new serverconfigurationtab(preset,serverDirectoryPath,avatars,ForestWorldArray,ForestResourcesArray,ForestFoodArray,ForestAnimalsArray,ForestMonstersArray,false),serverTabName);
		tabWidget->addTab(new serverconfigurationtab(preset, serverDirectoryPath, avatars, userSettings, false), serverTabName);
		
	
	}
	else if ("Cave"==preset)
	{
		UserSettings* userSettings = new UserSettings();
		userSettings->dstManSettings = this->dstManSettings;
		userSettings->worldArrays.push_back(CaveWorldArray);
		userSettings->worldArrays.push_back(CaveResourcesArray);
		userSettings->worldArrays.push_back(CaveFoodArray);
		userSettings->worldArrays.push_back(CaveAnimalsArray);
		userSettings->worldArrays.push_back(CaveMonstersArray);

		tabWidget->addTab(new serverconfigurationtab(preset,serverDirectoryPath,avatars,userSettings,false),serverTabName);
	
	}
	else if ("Both"==preset)
	{
		UserSettings* userSettings = new UserSettings();
		userSettings->dstManSettings = this->dstManSettings;
		userSettings->worldArrays.push_back(ForestWorldArray);
		userSettings->worldArrays.push_back(ForestResourcesArray);
		userSettings->worldArrays.push_back(ForestFoodArray);
		userSettings->worldArrays.push_back(ForestAnimalsArray);
		userSettings->worldArrays.push_back(ForestMonstersArray);
		userSettings->worldArrays.push_back(CaveWorldArray);
		userSettings->worldArrays.push_back(CaveResourcesArray);
		userSettings->worldArrays.push_back(CaveFoodArray);
		userSettings->worldArrays.push_back(CaveAnimalsArray);
		userSettings->worldArrays.push_back(CaveMonstersArray);

		//TODO pass in both Forest and Cave arrays when doing both presets
		tabWidget->addTab(new serverconfigurationtab(preset, serverDirectoryPath, avatars, userSettings, true),serverTabName);
		//tabWidget->addTab(new serverconfigurationtab("Cave",serverDirectoryPath,avatars,CaveWorldArray,CaveResourcesArray,CaveFoodArray,CaveAnimalsArray,CaveMonstersArray,true),serverTabName+"_cave");
	
	}
}

//Signal to handle the world data from the worldsettings
void getWorldData(string)
{
	//write the string into the settings ini file.
}


void dstServerManager::openDialog()
{
	//TODO: set the /home to be the default path loaded from the dstmanagersettings
	QString fileName = QFileDialog::getOpenFileName(this,tr("Open Server Configuration"), "/home", tr("DST Configs (*.ini)"));
	qDebug() << fileName;
	//TODO: open the file, check the preset, and if linked.    
	
	UserSettings* userSettings = new UserSettings();
	userSettings->dstManSettings = this->dstManSettings;
	userSettings->worldArrays.push_back(ForestWorldArray);
	userSettings->worldArrays.push_back(ForestResourcesArray);
	userSettings->worldArrays.push_back(ForestFoodArray);
	userSettings->worldArrays.push_back(ForestAnimalsArray);
	userSettings->worldArrays.push_back(ForestMonstersArray);
	
	tabWidget->addTab(new serverconfigurationtab(fileName,avatars,userSettings,false),fileName);

}

//TODO: Actually use the configuration values provided by dstmanagersettings
void dstServerManager::settingsDialog()
{
	dstmanagersettings *dstServerMangerSettings = new dstmanagersettings(dstWindow);
	connect(dstServerMangerSettings, SIGNAL(updateDstManSettings(GlobalDSTManSettings*)), this, SLOT(getDstServerManagerSettingsData(GlobalDSTManSettings*)));
	dstServerMangerSettings->show();
}

//TODO: Make an about dialog
void dstServerManager::aboutDialog()
{
	//show the about dialog
}

std::vector<std::tuple<int,string,string,string>> dstServerManager::fillGameOptionsArray(const char *xpathStr)
{
	pugi::xml_document doc;
	if (!doc.load_file(worldSettingsXmlPath))
	{
		qDebug() << "Errors parsing Document";
	}

	std::vector<std::tuple<int,string,string,string>> filledArray;
	pugi::xpath_node_set worldNodes = doc.select_nodes(xpathStr);

	int i=0;
	for (pugi::xpath_node_set::const_iterator it = worldNodes.begin(); it != worldNodes.end(); ++it)
	{
		pugi::xpath_node worldN =*it;
		qDebug() << worldN.node().name();
		qDebug() << worldN.node().child("IconNum").child_value();
		qDebug() << worldN.node().child("Default").child_value();
		qDebug() << worldN.node().child("AcceptableValues").child_value();
		
		//Clean the extracted value
		std::string input( worldN.node().child("Default").child_value()) ;
		input.erase(remove_if(input.begin(), input.end(), isspace),input.end());

		filledArray.push_back(std::make_tuple(
			stoi(worldN.node().child("IconNum").child_value()),
			input,
			worldN.node().child("AcceptableValues").child_value(),
			worldN.node().name()));
		i++;
	}

	return filledArray;
}

bool dstServerManager::dstManSettingsExist()
{
	return QFile::exists(QString::fromStdString(dstManagerSettingsFile));
}

bool dstServerManager::serverExists(QString tabName)
{
	qDebug() << "looking for " << tabName << "in the current Tabs";
	for(int i=0;i<tabWidget->count();i++) 
	{ 
		if(tabName==tabWidget->tabText(i) || (tabName+"_forest"==tabWidget->tabText(i) && tabName+"_cave"==tabWidget->tabText(i+1))) //Tabs are not re-arrangeable so this check will work..
		{ 
			return true;
		} 
	}
	return false;
}



dstServerManager::~dstServerManager()
{

}

void dstServerManager::getDstServerManagerSettingsData(GlobalDSTManSettings* dst)
{
	qDebug() << "Caught the emitted Manager settings";
	this->dstManSettings = dst;
}

void dstServerManager::loadSettings()
{
	QSettings settings(this->dstManagerSettingsFile, QSettings::IniFormat);

	settings.beginGroup("DstManSettings");
	dstManSettings->defaultServerFilePath = settings.value("defaultServerFilePath").toString().toStdString();
	dstManSettings->steamCMDExecutablePath = settings.value("steamCMDExecutablePath").toString().toStdString();
	dstManSettings->clusterTokenValue = settings.value("clusterTokenValue").toString().toStdString();
	settings.endGroup();

	emit updateDstManSettings(this->dstManSettings);
}

void updateDstManSettings(GlobalDSTManSettings* dst)
{
	
	
}

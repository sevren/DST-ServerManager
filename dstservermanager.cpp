#include <iostream>
#include <string>
#include <tuple>
#include "dstservermanager.h"
#include "newdialog.h"
#include <QDebug>
#include "pugixml.cpp"

using namespace std;


dstServerManager::dstServerManager(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	dstWindow=this;
	
	QImage image(".//Resources//imgs//worldsettings.png");
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
	


	setupConnections();
	setupMainLayout();
	
	pugi::xml_document doc;
	if (!doc.load_file(".\\Resources\\worldSettings.xml"))
	{
		qDebug() << "Errors parsing Document";
	}
  
	//Evaluate numbers via XPath Query

	qDebug() << "-------------Forest XML counts -------------";
	//Forest Counts
	pugi::xpath_query ForestWorldCounts("count(//Presets/Forest/World/*)");
	qDebug() << ForestWorldCounts.evaluate_number(doc);

	pugi::xpath_query ForestResourcesCounts("count(//Presets/Forest/Resources/*)");
	qDebug() << ForestResourcesCounts.evaluate_number(doc);

	pugi::xpath_query ForestFoodCounts("count(//Presets/Forest/Food/*)");
	qDebug() << ForestFoodCounts.evaluate_number(doc);

	pugi::xpath_query ForestAnimalsCounts("count(//Presets/Forest/Animals/*)");
	qDebug() << ForestAnimalsCounts.evaluate_number(doc);

	pugi::xpath_query ForestMonstersCounts("count(//Presets/Forest/Monsters/*)");
	qDebug() << ForestMonstersCounts.evaluate_number(doc);


	qDebug() << "-------------/End Forest XML counts -------------";

	qDebug() << "------------- Cave XML counts -------------";
	//Caves Counts

	pugi::xpath_query CaveWorldCounts("count(//Presets/Cave/World/*)");
	qDebug() << CaveWorldCounts.evaluate_number(doc);

	pugi::xpath_query CaveResourcesCounts("count(//Presets/Cave/Resources/*)");
	qDebug() << CaveResourcesCounts.evaluate_number(doc);

	pugi::xpath_query CaveFoodCounts("count(//Presets/Cave/Food/*)");
	qDebug() << CaveFoodCounts.evaluate_number(doc);

	pugi::xpath_query CaveAnimalsCounts("count(//Presets/Cave/Animals/*)");
	qDebug() << CaveAnimalsCounts.evaluate_number(doc);

	pugi::xpath_query CaveMonstersCounts("count(//Presets/Cave/Monsters/*)");
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

//	getData("sd","Both");

	
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
}


void dstServerManager::createNewServerConfig()
{
	NewDialog *newd= new NewDialog(dstWindow);
	connect(newd,SIGNAL(sendData(QString,QString,QString)),this,SLOT(getData(QString,QString,QString)));
	newd->show();
}

void dstServerManager::getData(QString serverTabName, QString preset, QString serverDirectoryPath)
{
	qDebug() << serverTabName;
	qDebug() << preset;
	qDebug() << serverDirectoryPath;
	if ("Forest"==preset)
	{
		tabWidget->addTab(new serverconfigurationtab(preset,avatars,ForestWorldArray,ForestResourcesArray,ForestFoodArray,ForestAnimalsArray,ForestMonstersArray),serverTabName);
	
	}
	else if ("Cave"==preset)
	{
		tabWidget->addTab(new serverconfigurationtab(preset,avatars,CaveWorldArray,CaveResourcesArray,CaveFoodArray,CaveAnimalsArray,CaveMonstersArray),serverTabName);
	
	}
	else if ("Both"==preset)
	{
		tabWidget->addTab(new serverconfigurationtab("Forest",avatars,ForestWorldArray,ForestResourcesArray,ForestFoodArray,ForestAnimalsArray,ForestMonstersArray),serverTabName+"_forest");
		tabWidget->addTab(new serverconfigurationtab("Cave",avatars,CaveWorldArray,CaveResourcesArray,CaveFoodArray,CaveAnimalsArray,CaveMonstersArray),serverTabName+"_cave");
	
	}
}


void dstServerManager::openDialog()
{
	//show the open dialog
}

void dstServerManager::settingsDialog()
{
	//show the settings dialog
}

void dstServerManager::aboutDialog()
{
	//show the about dialog
}

std::vector<std::tuple<int,string,string>> dstServerManager::fillGameOptionsArray(const char *xpathStr)
{
	pugi::xml_document doc;
	if (!doc.load_file(".\\Resources\\worldSettings.xml"))
	{
		qDebug() << "Errors parsing Document";
	}

	std::vector<std::tuple<int,string,string>> filledArray;
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
			worldN.node().child("AcceptableValues").child_value()));
		i++;
	}

	return filledArray;
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

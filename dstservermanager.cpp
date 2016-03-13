#include <iostream>
#include <string>
#include "dstservermanager.h"
#include <QDebug>
#include "newdialog.h"
#include "pugixml.cpp"
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

dstServerManager::dstServerManager(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	//sct = new serverconfigurationtab();


	QImage image(".//Resources//imgs//worldsettings.png");
	QImage copy ;
	
	
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
	pugi::xpath_node_set worldNodes = doc.select_nodes("//Presets//Forest/World/*");
	int i=0;
	for (pugi::xpath_node_set::const_iterator it = worldNodes.begin(); it != worldNodes.end(); ++it)
	{
		pugi::xpath_node worldN =*it;
		qDebug() << worldN.node().name();
		qDebug() << worldN.node().child("IconNum").child_value();
		qDebug() << worldN.node().child("Default").child_value();
		qDebug() << worldN.node().child("AcceptableValues").child_value();
		
		

		QLabel* qlbl;
		qlbl=new QLabel();
		qlbl->setMinimumSize(QSize(50, 50));
		qlbl->setMaximumSize(QSize(50, 50));
		qlbl->setPixmap(QPixmap::fromImage(avatars[std::stoi(worldN.node().child("IconNum").child_value())]));
		qlbl->setToolTip(QString::fromStdString(worldN.node().name()));
		QComboBox* qcbox = new QComboBox();
		QStringList list=(setComboBoxValues(worldN.node().child("AcceptableValues").child_value()));
		qcbox->addItems(list);
		
		//Clean the extracted value
		std::string input( worldN.node().child("Default").child_value()) ;
		input.erase(remove_if(input.begin(), input.end(), isspace),input.end());

		int index = qcbox->findData(QString::fromStdString(input),Qt::DisplayRole);
		
		if ( index != -1 ) { // -1 for not found
		   qcbox->setCurrentIndex(index);
		}
		ForestWorldArray.push_back(std::make_pair(qlbl,qcbox));
		i++;
	}

	//getData("sd");

	
}

void dstServerManager::setupMainLayout()
{
	tabWidget = new QTabWidget();
	tabWidget->setTabsClosable(true);
	//tabWidget->addTab(new serverconfigurationtab(),"SERVER 1");
	//tabWidget->addTab(new serverconfigurationtab(),"SERVER 2");
	ui.gridLayout->addWidget(tabWidget);
}


void dstServerManager::setupConnections()
{
	connect(ui.actionNew,SIGNAL(triggered()),this,SLOT(createNewServerConfig()));
	


}


void dstServerManager::createNewServerConfig()
{
	NewDialog *newd= new NewDialog();
	connect(newd,SIGNAL(sendData(QString)),this,SLOT(getData(QString)));
	newd->show();
}

void dstServerManager::getData(QString serverTabName)
{
	qDebug() << serverTabName;
	tabWidget->addTab(new serverconfigurationtab(avatars,ForestWorldArray),serverTabName);
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

QStringList dstServerManager::setComboBoxValues(std::string values)
{
	QStringList stringList;
	//tokenize AcceptableValues and return as list
	char_separator<char> sep(",");
    tokenizer< char_separator<char> > tokens(values, sep);
    BOOST_FOREACH (const string& t, tokens) {
		qDebug() << QString::fromStdString(t);
		stringList.append(QString::fromStdString(t));
    }

	
	return stringList;
}


dstServerManager::~dstServerManager()
{

}

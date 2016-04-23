#include <tuple>
#include <string>
#include "serverconfigurationtab.h"
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


using namespace std;
using namespace boost;




serverconfigurationtab::serverconfigurationtab(QString preset,QString serverDirectoryPath,QImage*avatars, xmlDataValues& WorldArray,xmlDataValues& ResourcesArray,xmlDataValues& FoodArray,xmlDataValues& AnimalsArray,xmlDataValues& MonstersArray,bool linked,QWidget *parent)
	: QWidget(parent)
{

	ui.setupUi(this);
	this->serverDirectoryPath=serverDirectoryPath;
	this->linked=linked;
	this->preset=preset;
	readMaps();

	qDebug() << "The number of items in season_start is " << seasonstart_map.size();
	
	for( gameOptionsMapping::const_iterator iter = seasonstart_map.begin(), iend = seasonstart_map.end(); iter != iend; ++iter )
	{
		// iter->left  : data : std:string
		// iter->right : data : std::string

		qDebug() << iter->left.c_str() << " <--> " << iter->right.c_str();
	}

	//use left when going from gui to file && use right when going from file to gui
	gameOptionsMapping::left_const_iterator iter = seasonstart_map.left.find("Winter/Summer");
	qDebug() << iter->second.c_str();


	/*qDebug() << "Size of  World Array: " <<WorldArray.size();
	qDebug() << "Size of  Resources Array: " <<ResourcesArray.size();
	qDebug() << "Size of  Food Array: " <<FoodArray.size();
	qDebug() << "Size of  Animals Array: " <<AnimalsArray.size();
	qDebug() << "Size of  Monsters Array: " <<MonstersArray.size();
	qDebug() << std::get<1>(WorldArray[0]).c_str();
	qDebug() <<"Testing get";*/

	//Fill the appropriate gameOption arrays depending on the preset choosen
	if ("Forest"==preset)
	{
		ForestWorldArray=fillGameOptions(WorldArray,avatars);
		ForestResourcesArray=fillGameOptions(ResourcesArray,avatars);
		ForestFoodArray=fillGameOptions(FoodArray,avatars);
		ForestAnimalsArray=fillGameOptions(AnimalsArray,avatars);
		ForestMonstersArray=fillGameOptions(MonstersArray,avatars);

		//
		setupUserGameOptionsScreen(ForestWorldArray,_WORLD_);
		setupUserGameOptionsScreen(ForestResourcesArray,_RESOURCES_);
		setupUserGameOptionsScreen(ForestFoodArray,_FOOD_);
		setupUserGameOptionsScreen(ForestAnimalsArray,_ANIMALS_);
		setupUserGameOptionsScreen(ForestMonstersArray,_MONSTERS_);
	}
	else if ("Cave"==preset)
	{
		CaveWorldArray=fillGameOptions(WorldArray,avatars);
		CaveResourcesArray=fillGameOptions(ResourcesArray,avatars);
		CaveFoodArray=fillGameOptions(FoodArray,avatars);
		CaveAnimalsArray=fillGameOptions(AnimalsArray,avatars);
		CaveMonstersArray=fillGameOptions(MonstersArray,avatars);

		setupUserGameOptionsScreen(CaveWorldArray,_WORLD_);
		setupUserGameOptionsScreen(CaveResourcesArray,_RESOURCES_);
		setupUserGameOptionsScreen(CaveFoodArray,_FOOD_);
		setupUserGameOptionsScreen(CaveAnimalsArray,_ANIMALS_);
		setupUserGameOptionsScreen(CaveMonstersArray,_MONSTERS_);



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
	//	ui.serverSettings->setVisible(false);
	}



	connect(ui.saveBtn,SIGNAL(clicked()),this,SLOT(saveSettings()));
	connect(ui.runBtn,SIGNAL(clicked()),this,SLOT(run()));
	
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

void serverconfigurationtab::setupUserGameOptionsScreen(gameOptions& gOArray, int dataType)
{
	QGridLayout* gLayout;
	switch(dataType)
	{
		case _WORLD_:
			gLayout=std::ref(ui.gridLayout);
		break;
		case _RESOURCES_:
			gLayout=std::ref(ui.ResourcesGridLayout);
		break;
		case _FOOD_:
			gLayout=std::ref(ui.FoodGridLayout);
		break;
		case _ANIMALS_:
			gLayout=std::ref(ui.AnimalsGridLayout);
		break;
		case _MONSTERS_:
			gLayout=std::ref(ui.MonstersGridLayout);
		break;
	}

	int indx=0;
	int counter=0;
	for (int gridRow=0;gridRow<gOArray.size();gridRow++)
	{
		for(int gridCol=0;gridCol<4;gridCol++)
		{
			if (indx>=gOArray.size())
				{
					break;
				}
			//	qDebug() << gridRow<<","<< gridCol<< "  counter is "<< counter << " index is" << indx;
				

				gLayout->addWidget(gOArray[indx].first,gridRow,gridCol);
				gLayout->addWidget(gOArray[indx].second,gridRow,gridCol+1);

				counter++;
				if(counter % 2==0) //every 2 elements processed move onto next gameOption
				{
					indx++;
				}
		}		
	}
}

string  serverconfigurationtab::getServerConfigSettings(QGroupBox* groupBox)
{
	qDebug()<<groupBox->children();
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
			}
			else if (i.peekNext()->inherits("QLineEdit"))
			{
				QLineEdit* tempLineEdit=qobject_cast<QLineEdit *>(i.next());
				settings << tempLineEdit->objectName().toStdString() << "=" << tempLineEdit->text().toStdString() <<"\n";
			}
			else if (i.peekNext()->inherits("QCheckBox"))
			{
				QCheckBox* tempCheckBox=qobject_cast<QCheckBox *>(i.next());
				if ("is_master"!=tempCheckBox->objectName().toStdString())
				{
					settings << tempCheckBox->objectName().toStdString() << "=" <<((tempCheckBox->isChecked()) ? ("true") : ("false") ) <<"\n";
				}
			}
			else if (i.peekNext()->inherits("QComboBox"))
			{
				QComboBox* tempComboBox=qobject_cast<QComboBox *>(i.next());
				settings << tempComboBox->objectName().toStdString() << "=" << tempComboBox->itemData(tempComboBox->currentIndex(),Qt::DisplayRole).toString().toStdString() <<"\n";
			}

		}
		else
		{
			i.next(); //advance the iterator but don't do anything with the item because it's a label
		}
	}
	return settings.str();
}

string serverconfigurationtab::getGameOptionSettings(QGridLayout* gridLayout)
{
	//qDebug()<<gridLayout->count();
	stringstream settings;
	if ("FoodGridLayout"==gridLayout->objectName())
	{
		settings << "\tunprepared = { -- \"never\", \"rare\", \"default\", \"often\", \"always\"\n\t\t\t";
	}
	else if ("gridLayout"==gridLayout->objectName())
	{
		settings << "\tmisc = { \n\t\t\t";
	}
	else if ("ResourcesGridLayout"==gridLayout->objectName())
	{
		settings << "\tresources = { -- \"never\", \"rare\", \"default\", \"often\", \"always\"\n\t\t\t";
	}
	else if ("AnimalsGridLayout"==gridLayout->objectName())
	{
		settings << "\tanimals = { -- \"never\", \"rare\", \"default\", \"often\", \"always\"\n\t\t\t";
	}
	else if ("MonstersGridLayout"==gridLayout->objectName())
	{
		settings << "\tmonsters = { -- \"never\", \"rare\", \"default\", \"often\", \"always\"\n\t\t\t";
	}


	if ("gridLayout"==gridLayout->objectName())
	{
		for (int i = 0; i < gridLayout->count(); ++i)
		{
		  QWidget *widget = gridLayout->itemAt(i)->widget();
		  if ((widget != NULL) && (widget->inherits("QComboBox")))
		  {
			QComboBox* tempComboBox=qobject_cast<QComboBox*>(widget);
			settings << tempComboBox->objectName().toStdString() << "=" << '"'<< handleWorldData(tempComboBox->objectName().toStdString(),tempComboBox->itemData(tempComboBox->currentIndex(),Qt::DisplayRole).toString().toStdString()) <<'"'<<",\n\t\t\t";
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
			QComboBox* tempComboBox=qobject_cast<QComboBox*>(widget);
			settings << tempComboBox->objectName().toStdString() << "=" << '"'<< convertComboData(tempComboBox->itemData(tempComboBox->currentIndex(),Qt::DisplayRole).toString().toStdString()) <<'"'<<",\n\t\t\t";
		  }
	 
		}
	}
	settings <<"},\n";
	return settings.str();
}

string serverconfigurationtab::convertComboData(string pickedItem)
{
	gameOptionsMapping::left_const_iterator iter = other_map.left.find(pickedItem);
	return iter->second.c_str();
}

string serverconfigurationtab::handleWorldData(string name,string pickedItem)
{
	if (("autumn"==name) || ("spring"==name) || ("summer"==name)||("winter"==name))
	{
		//autumn = "default", -- "noseason", "veryshortseason", "shortseason", "default", "longseason", "verylongseason", "random"
		gameOptionsMapping::left_const_iterator iter = seasons_map.left.find(pickedItem);
		return iter->second.c_str();
	}
	else if ("branching"==name) //can be converted to lower case with spaces removed
	{
		gameOptionsMapping::left_const_iterator iter = branching_map.left.find(pickedItem);
		return iter->second.c_str();

	}
	else if ("cavelight"==name) //can be converted to lower case with spaces removed.
	{
		gameOptionsMapping::left_const_iterator iter = cavelight_map.left.find(pickedItem);
		return iter->second.c_str();

	}
	else if ("day"==name) //can just be trimmed and spaces removed and converted to lower case
	{
		//day = "default", -- "default", "longday", "longdusk", "longnight", "noday", "nodusk", "nonight", "onlyday", "onlydusk", "onlynight"
		gameOptionsMapping::left_const_iterator iter = day_map.left.find(pickedItem);
		return iter->second.c_str();
	}
	else if ("loop"==name) //can just be converted to lower case
	{
		gameOptionsMapping::left_const_iterator iter = loop_map.left.find(pickedItem);
		return iter->second.c_str();

	}
	else if ("regrowth"==name) //can be converted to lower case and spaces removed.
	{
		gameOptionsMapping::left_const_iterator iter = regrowth_map.left.find(pickedItem);
		return iter->second.c_str();
	}
	else if ("season_start"==name)
	{
		//season_start = "default", -- "default", "winter", "spring", "summer", "autumnorspring", "winterorsummer", "random"
		gameOptionsMapping::left_const_iterator iter = seasonstart_map.left.find(pickedItem);
		return iter->second.c_str();
	}
	else if ("start_location"==name)
	{
		gameOptionsMapping::left_const_iterator iter = startlocation_map.left.find(pickedItem);
		return iter->second.c_str();
	}
	else if("task_set"==name)//biomes
	{
		//task_set = "cave_default", -- "classic", "default", "cave_default"
		gameOptionsMapping::left_const_iterator iter = taskset_map.left.find(pickedItem);
		return iter->second.c_str();
	}
	else if ("world_size"==name)
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
	qDebug() << serverDirectoryPath;

	
	QString filePathName=serverDirectoryPath+QString(QDir::separator())+"cluster.ini";
	ofstream myfile;
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
	myfile << ui.name->objectName().toStdString() << "=" << ui.name->text().toStdString() <<"\n";
	myfile << ui.id->objectName().toStdString() << "=" << ui.id->text().toStdString() <<"\n";
	myfile << "[STEAM]\n";
	myfile << ui.authentication_port->objectName().toStdString() << "=" << ui.authentication_port->value()<<"\n";
	myfile << ui.master_server_port->objectName().toStdString() << "=" << ui.master_server_port->value()<<"\n";
	myfile << "[NETWORK]\n";
	myfile << ui.server_port->objectName().toStdString() << "=" << ui.server_port->value()<<"\n";
	myfile.close();


	filePathName=serverDirectoryPathFinal+QString(QDir::separator())+"worldgenoverride.lua";
	myfile.open (filePathName.toStdString());
	myfile << "return {\n \t\t override_enabled = true, \n";
	//myfile << fetchPreset();
	myfile << getGameOptionSettings(ui.FoodGridLayout) << "\n";
	myfile << getGameOptionSettings(ui.gridLayout) << "\n";
	myfile << getGameOptionSettings(ui.AnimalsGridLayout) << "\n";
	myfile << getGameOptionSettings(ui.MonstersGridLayout) << "\n";
	myfile << getGameOptionSettings(ui.ResourcesGridLayout) << "\n";
	myfile << "}";
	myfile.close();

	
}

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
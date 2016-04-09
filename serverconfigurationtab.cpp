#include <tuple>
#include <string>
#include "serverconfigurationtab.h"
#include <qimage.h>
#include <QDebug>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#define WINVER 0x0500
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <qdir.h>

using namespace std;
using namespace boost;

serverconfigurationtab::serverconfigurationtab(QString preset,QString serverDirectoryPath,QImage*avatars, xmlDataValues& WorldArray,xmlDataValues& ResourcesArray,xmlDataValues& FoodArray,xmlDataValues& AnimalsArray,xmlDataValues& MonstersArray,QWidget *parent)
	: QWidget(parent)
{

	ui.setupUi(this);
	this->serverDirectoryPath=serverDirectoryPath;
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
	connect(ui.saveBtn,SIGNAL(clicked()),this,SLOT(saveSettings()));
	
}


serverconfigurationtab::~serverconfigurationtab()
{

}

QStringList serverconfigurationtab::setComboBoxValues(string values)
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
			gLayout=ref(ui.gridLayout);
		break;
		case _RESOURCES_:
			gLayout=ref(ui.ResourcesGridLayout);
		break;
		case _FOOD_:
			gLayout=ref(ui.FoodGridLayout);
		break;
		case _ANIMALS_:
			gLayout=ref(ui.AnimalsGridLayout);
		break;
		case _MONSTERS_:
			gLayout=ref(ui.MonstersGridLayout);
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
				qDebug() << gridRow<<","<< gridCol<< "  counter is "<< counter << " index is" << indx;
				

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
				settings << tempCheckBox->objectName().toStdString() << "=" <<((tempCheckBox->isChecked()) ? ("true") : ("false") ) <<"\n";
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
	qDebug()<<gridLayout->count();
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
	string convertedItem;
	if ("None"==pickedItem)
	{
		convertedItem="never";
	}
	else if ("Less"==pickedItem)
	{
		convertedItem="rare";
	}
	else if ("Default"==pickedItem)
	{
		convertedItem="default";
	}
	else if ("More"==pickedItem)
	{
		convertedItem="often";
	}
	else if ("Lots"==pickedItem)
	{
		convertedItem="always";
	}
	return convertedItem; 
}

string serverconfigurationtab::handleWorldData(string name,string pickedItem)
{
	string convertedItem;
	if (("autumn"==name) || ("spring"==name) || ("summer"==name)||("winter"==name))
	{
		//autumn = "default", -- "noseason", "veryshortseason", "shortseason", "default", "longseason", "verylongseason", "random"
		if("None"==pickedItem)
		{
			convertedItem="noseason";
		}
		else if ("Very short"==pickedItem)
		{
			convertedItem="veryshortseason";
		}
		else if ("Short"==pickedItem)
		{
			convertedItem="shortseason";
		}
		else if ("Default"==pickedItem)
		{
			convertedItem="default";
		}
		else if ("Long"==pickedItem)
		{
			convertedItem="longseason";
		}
		else if ("Very long"==pickedItem)
		{
			convertedItem="verylongseason";
		}
		else if ("Random"==pickedItem)
		{
			convertedItem="random";
		}

	}
	else if ("branching"==name) //can be converted to lower case with spaces removed
	{
		if("Never"==pickedItem)
		{
			convertedItem="never";
		}
		else if ("Least"==pickedItem)
		{
			convertedItem="least";
		}
		else if ("Default"==pickedItem)
		{
			convertedItem="default";
		}
		else if ("Most"==pickedItem)
		{
			convertedItem="most";
		}	

	}
	else if ("cavelight"==name) //can be converted to lower case with spaces removed.
	{
		if("Very Slow"==pickedItem)
		{
			convertedItem="veryslow";
		}
		else if ("Slow"==pickedItem)
		{
			convertedItem="slow";
		}
		else if ("Default"==pickedItem)
		{
			convertedItem="default";
		}
		else if ("Fast"==pickedItem)
		{
			convertedItem="fast";
		}
		else if ("Very Fast"==pickedItem)
		{
			convertedItem="veryfast";
		}

	}
	else if ("day"==name) //can just be trimmed and spaces removed and converted to lower case
	{
		//day = "default", -- "default", "longday", "longdusk", "longnight", "noday", "nodusk", "nonight", "onlyday", "onlydusk", "onlynight"
		if("Default"==pickedItem)
		{
			convertedItem="default";
		}
		else if ("Long Day"==pickedItem)
		{
			convertedItem="longday";
		}
		else if ("Long Dusk"==pickedItem)
		{
			convertedItem="longdusk";
		}
		else if ("Long Night"==pickedItem)
		{
			convertedItem="longnight";
		}
		else if ("No Day"==pickedItem)
		{
			convertedItem="noday";
		}
		else if ("No Dusk"==pickedItem)
		{
			convertedItem="nodusk";
		}
		else if ("No Night"==pickedItem)
		{
			convertedItem="nonight";
		}
		else if ("Only Day"==pickedItem)
		{
			convertedItem="onlyday";
		}
		else if ("Only Dusk"==pickedItem)
		{
			convertedItem="onlydusk";
		}
		else if ("Only Night"==pickedItem)
		{
			convertedItem="onlynight";
		}

	}
	else if ("loop"==name) //can just be converted to lower case
	{
		if("Never"==pickedItem)
		{
			convertedItem="never";
		}
		else if ("Default"==pickedItem)
		{
			convertedItem="default";
		}
		else if ("Always"==pickedItem)
		{
			convertedItem="always";
		}
		

	}
	else if ("regrowth"==name) //can be converted to lower case and spaces removed.
	{
		if("Very Slow"==pickedItem)
		{
			convertedItem="veryslow";
		}
		else if ("Slow"==pickedItem)
		{
			convertedItem="slow";
		}
		else if ("Default"==pickedItem)
		{
			convertedItem="default";
		}
		else if ("Fast"==pickedItem)
		{
			convertedItem="fast";
		}
		else if ("Very Fast"==pickedItem)
		{
			convertedItem="veryfast";
		}
		
	}
	else if ("season_start"==name)
	{
		//season_start = "default", -- "default", "winter", "spring", "summer", "autumnorspring", "winterorsummer", "random"
		if("Autumn"==pickedItem)
		{
			convertedItem="default";
		}
		else if ("Winter"==pickedItem)
		{
			convertedItem="winter";
		}
		else if ("Spring"==pickedItem)
		{
			convertedItem="spring";
		}
		else if ("Summer"==pickedItem)
		{
			convertedItem="summer";
		}
		else if ("Autumn/Spring"==pickedItem)
		{
			convertedItem="autumnorspring";
		}
		else if ("Winter/Summer"==pickedItem)
		{
			convertedItem="winterorsummer";
		}
		else if ("Random"==pickedItem)
		{
			convertedItem="random";
		}
		
	}
	else if ("start_location"==name)
	{
		//start_location = "default", -- "caves", "default", "plus", "darkness"
		if("Caves"==pickedItem)
		{
			convertedItem="caves";
		}
		else if ("Default"==pickedItem)
		{
			convertedItem="default";
		}
		else if ("Plus"==pickedItem)
		{
			convertedItem="plus";
		}
		else if ("Dark"==pickedItem)
		{
			convertedItem="darkness";
		}

	}
	else if("task_set"==name)//biomes
	{
		//task_set = "cave_default", -- "classic", "default", "cave_default"
		if("Classic"==pickedItem)
		{
			convertedItem="classic";
		}
		else if ("Together"==pickedItem)
		{
			convertedItem="default";
		}
		else if ("Underground"==pickedItem)
		{
			convertedItem="cave_default";
		}

	}
	else if ("world_size"==name)
	{
		//world_size = "default", -- "small", "medium", "default", "huge"
		if("Small"==pickedItem)
		{
			convertedItem="small";
		}
		else if ("Medium"==pickedItem)
		{
			convertedItem="medium";
		}
		else if ("Large"==pickedItem)
		{
			convertedItem="default";
		}
		else if ("Huge"==pickedItem)
		{
			convertedItem="huge";
		}
	}
	else
	{
		convertedItem=convertComboData(pickedItem);
	}
	return convertedItem;
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

	filePathName=serverDirectoryPath+QString(QDir::separator())+"server.ini";
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


	filePathName=serverDirectoryPath+QString(QDir::separator())+"worldgenoverride.lua";
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
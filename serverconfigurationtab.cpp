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

using namespace std;
using namespace boost;

serverconfigurationtab::serverconfigurationtab(QString preset,QImage*avatars, xmlDataValues& WorldArray,xmlDataValues& ResourcesArray,xmlDataValues& FoodArray,xmlDataValues& AnimalsArray,xmlDataValues& MonstersArray,QWidget *parent)
	: QWidget(parent)
{

	ui.setupUi(this);

	qDebug() << "Size of  World Array: " <<WorldArray.size();
	qDebug() << "Size of  Resources Array: " <<ResourcesArray.size();
	qDebug() << "Size of  Food Array: " <<FoodArray.size();
	qDebug() << "Size of  Animals Array: " <<AnimalsArray.size();
	qDebug() << "Size of  Monsters Array: " <<MonstersArray.size();
	qDebug() << std::get<1>(WorldArray[0]).c_str();
	qDebug() <<"Testing get";

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
		//qlbl->setToolTip(QString::fromStdString(worldN.node().name()));
		QComboBox* qcbox = new QComboBox();
		QStringList list=(setComboBoxValues(get<2>(dataValues[i])));
		qcbox->addItems(list);
		
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

		if (!(i.peekNext()->inherits("QLabel") || i.peekNext()->inherits("QPushButton")))
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



void serverconfigurationtab::saveSettings()
{
	//save all settings to the cluster.ini, server.ini and worldoverridesettings.lua
	ofstream myfile;
	myfile.open ("cluster.ini");
	myfile << "[GAMEPLAY]\n";
	myfile << getServerConfigSettings(ui.gameplaySettings) << "\n\n";
	myfile << "[NETWORK]\n";
	myfile << getServerConfigSettings(ui.networkSettings) << "\n\n";
	myfile << "[MISC]\n";
	myfile << getServerConfigSettings(ui.miscSettings) << "\n\n";
	myfile << "[SHARD]\n";
	myfile << getServerConfigSettings(ui.shardSettings) << "\n\n";
	myfile.close();
}
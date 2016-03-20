#include <tuple>
#include <string>
#include "serverconfigurationtab.h"
#include <qimage.h>
#include <QDebug>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

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
/*
	int indx=0;
	int counter=0;
	for (int gridRow=0;gridRow<WorldArray.size();gridRow++)
	{


		for(int gridCol=0;gridCol<4;gridCol++)
		{
			if (indx>=WorldArray.size())
				{
					break;
				}
				qDebug() << gridRow<<","<< gridCol<< "  counter is "<< counter << " index is" << indx;

				
				ui.gridLayout->addWidget(ForestWorldArray[indx].first,gridRow,gridCol);
				ui.gridLayout->addWidget(ForestWorldArray[indx].second,gridRow,gridCol+1);
				counter++;
				if(counter % 2==0)
				{
					indx++;
				}
		}
			
	}

	/*indx=0;
	counter=0;
	for (int gridRow=0;gridRow<ResourcesArray.size();gridRow++)
	{
		for(int gridCol=0;gridCol<4;gridCol++)
		{
			if (indx>=ResourcesArray.size())
				{
					break;
				}
				qDebug() << gridRow<<","<< gridCol<< "  counter is "<< counter << " index is" << indx;
				ui.ResourcesGridLayout->addWidget(ResourcesArray[indx].first,gridRow,gridCol);
				ui.ResourcesGridLayout->addWidget(ResourcesArray[indx].second,gridRow,gridCol+1);
				counter++;
				if(counter % 2==0)
				{
					indx++;
				}
		}
			
	}
	
	
	indx=0;
	counter=0;
	for (int gridRow=0;gridRow<FoodArray.size();gridRow++)
	{
		for(int gridCol=0;gridCol<4;gridCol++)
		{
			if (indx>=FoodArray.size())
				{
					break;
				}
				qDebug() << gridRow<<","<< gridCol<< "  counter is "<< counter << " index is" << indx;
				ui.FoodGridLayout->addWidget(FoodArray[indx].first,gridRow,gridCol);
				ui.FoodGridLayout->addWidget(FoodArray[indx].second,gridRow,gridCol+1);
				counter++;
				if(counter % 2==0)
				{
					indx++;
				}
		}
			
	}


	indx=0;
	counter=0;
	for (int gridRow=0;gridRow<AnimalsArray.size();gridRow++)
	{
		for(int gridCol=0;gridCol<4;gridCol++)
		{
			if (indx>=AnimalsArray.size())
				{
					break;
				}
				qDebug() << gridRow<<","<< gridCol<< "  counter is "<< counter << " index is" << indx;
				ui.AnimalsGridLayout->addWidget(AnimalsArray[indx].first,gridRow,gridCol);
				ui.AnimalsGridLayout->addWidget(AnimalsArray[indx].second,gridRow,gridCol+1);
				counter++;
				if(counter % 2==0)
				{
					indx++;
				}
		}
			
	}

	
	indx=0;
	counter=0;
	for (int gridRow=0;gridRow<MonstersArray.size();gridRow++)
	{
		for(int gridCol=0;gridCol<4;gridCol++)
		{
			if (indx>=MonstersArray.size())
				{
					break;
				}
				qDebug() << gridRow<<","<< gridCol<< "  counter is "<< counter << " index is" << indx;
				ui.MonstersGridLayout->addWidget(MonstersArray[indx].first,gridRow,gridCol);
				ui.MonstersGridLayout->addWidget(MonstersArray[indx].second,gridRow,gridCol+1);
				counter++;
				if(counter % 2==0)
				{
					indx++;
				}
		}
			
	}
	*/
	
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

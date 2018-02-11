#include "worldsettings.h"

using namespace std;
using namespace boost;

/*
* WorldSettings - This deals with the resource settings for each preset
* Takes in the appropriate arrays, augments the tab with the avatars and possible values for the correct resource
*/


//TODO: Allow data from the comboboxes to be read from the serverconfigurationtab
//TODO: Remove the debugging statements
worldsettings::worldsettings(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

worldsettings::worldsettings(gameOptions& worldArray, gameOptions& resourceArray, gameOptions& foodArray, gameOptions& animalsArray, gameOptions& monstersArray, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setupUserGameOptionsScreen(worldArray, _WORLD_);
	setupUserGameOptionsScreen(resourceArray, _RESOURCES_);
	setupUserGameOptionsScreen(foodArray, _FOOD_);
	setupUserGameOptionsScreen(animalsArray, _ANIMALS_);
	setupUserGameOptionsScreen(monstersArray, _MONSTERS_);

}

worldsettings::~worldsettings()
{
}

void worldsettings::setupUserGameOptionsScreen(gameOptions& gOArray, int dataType)
{
	QGridLayout* gLayout;



	switch (dataType)
	{
	case _WORLD_:
		gLayout = std::ref(ui.gridLayout);
		break;
	case _RESOURCES_:
		gLayout = std::ref(ui.ResourcesGridLayout);
		break;
	case _FOOD_:
		gLayout = std::ref(ui.FoodGridLayout);
		break;
	case _ANIMALS_:
		gLayout = std::ref(ui.AnimalsGridLayout);
		break;
	case _MONSTERS_:
		gLayout = std::ref(ui.MonstersGridLayout);
		break;
	}

	int indx = 0;
	int counter = 0;
	for (int gridRow = 0; gridRow < gOArray.size(); gridRow++)
	{
		for (int gridCol = 0; gridCol < 4; gridCol++)
		{
			if (indx >= gOArray.size())
			{
				break;
			}
			//	qDebug() << gridRow<<","<< gridCol<< "  counter is "<< counter << " index is" << indx;


			gLayout->addWidget(gOArray[indx].first, gridRow, gridCol);
			gLayout->addWidget(gOArray[indx].second, gridRow, gridCol + 1);

			counter++;
			if (counter % 2 == 0) //every 2 elements processed move onto next gameOption
			{
				indx++;
			}
		}
	}
}

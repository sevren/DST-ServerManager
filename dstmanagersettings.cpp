#include "dstmanagersettings.h"
#include <QDebug>
#include <qtooltip.h>
#include "dstservermanager.h"
#include <qfiledialog.h>

/*
* DstManagerSettings - This form holds the overall configuration for DSTServerManager
* Responsible for:
*  1. settings the default path to where the server configuration files live. 
*  2. providing the path to SteamCMD 
*  3. providing the ServerToken
*  !! Must be set up at least once! reads the configuration values from a dstmanagersettings ini file.!!
*/

//TODO: Read settings from dstmanagersettings ini file
//TODO: Provide functions to retrieve default server file path, default path to steamCMD and serverToken
//TODO: Setup fileChooser to select the path for configuration files and the steamCMD.

dstmanagersettings::dstmanagersettings(dstServerManager *dstWindow, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//ui.serverFilePath->setReadOnly(true);
	//connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(createNewServerConfig()));
	//connect(ui.directoryChooserBtn, SIGNAL(clicked()), this, SLOT(openFileChooser()));
	qDebug() << dstWindow->metaObject()->className();
	this->dstWindow = dstWindow;
}



dstmanagersettings::~dstmanagersettings()
{
}

void dstmanagersettings::openFileChooser()
{

}



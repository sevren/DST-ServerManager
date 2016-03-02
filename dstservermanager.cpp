#include "dstservermanager.h"

#include "newdialog.h"
dstServerManager::dstServerManager(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	sct = new serverconfigurationtab();
	ui.gridLayout->addWidget(sct);
	
	
}

void dstServerManager::setupConnections()
{
	connect(ui.actionNew,SIGNAL(triggered()),this,SLOT(createNewServerConfig()));
	


}


void dstServerManager::createNewServerConfig()
{
	NewDialog *newd= new NewDialog();
	newd->show();
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


dstServerManager::~dstServerManager()
{

}

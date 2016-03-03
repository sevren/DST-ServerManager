#include "dstservermanager.h"
#include <QDebug>

#include "newdialog.h"
dstServerManager::dstServerManager(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	sct = new serverconfigurationtab();
	setupConnections();
	ui.gridLayout->addWidget(sct);

	
	
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

void dstServerManager::getData(QString s)
{
	qDebug() << s;
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

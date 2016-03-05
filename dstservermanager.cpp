#include "dstservermanager.h"
#include <QDebug>

#include "newdialog.h"
dstServerManager::dstServerManager(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	
	sct = new serverconfigurationtab();
	setupConnections();
	setupMainLayout();
	//ui.gridLayout->addWidget(sct);

	
	
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
	tabWidget->addTab(new serverconfigurationtab(),serverTabName);
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

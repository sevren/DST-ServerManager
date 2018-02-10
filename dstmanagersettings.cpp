#include "dstmanagersettings.h"
#include <QDebug>
#include <qtooltip.h>
#include "dstservermanager.h"
#include <qfiledialog.h>


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



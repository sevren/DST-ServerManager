#include "newdialog.h"
#include <QDebug>
#include <qtooltip.h>
#include "dstservermanager.h"
#include <qfiledialog.h>

NewDialog::NewDialog(dstServerManager *dstWindow,QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.serverFilePath->setReadOnly(true);
	connect(ui.buttonBox,SIGNAL(accepted()),this,SLOT(createNewServerConfig()));
	connect(ui.directoryChooserBtn,SIGNAL(clicked()),this,SLOT(openFileChooser()));
	qDebug() << dstWindow->metaObject()->className();
	this->dstWindow=dstWindow;
}

NewDialog::~NewDialog()
{

}

pair<bool,QString> NewDialog::validate()
{
	if (!getTextFromInputBox().isEmpty()) //if not empty
	{	
		if (!dstWindow->serverExists(getTextFromInputBox())) //if server exists
		{
			return make_pair(true,QString());
		}
		else
		{
			return make_pair(false,QString(getTextFromInputBox() +" Already Exists!"));
		}
	}
	else
	{
		return make_pair(false,QString("Name can not be empty!"));
	}
}

QString NewDialog::getTextFromInputBox()
{
	return ui.NewServerConfigName->text();
}

void NewDialog::createNewServerConfig()
{
	pair<bool,QString> validated=validate();
	bool valid  = validated.first;
	QString msg = validated.second;
	
	if (valid)
	{
		qDebug() << "emit create New Server Config";
		//Forest must be emitted as a QString from the radio buttons
		if (ui.preset_forest->isChecked())
		{

			emit sendData(getTextFromInputBox(),QString("Forest"));
		}
		else if (ui.preset_cave->isChecked())
		{
			emit sendData(getTextFromInputBox(),QString("Cave"));
		}
		else if (ui.preset_both->isChecked())
		{
			emit sendData(getTextFromInputBox(),QString("Both"));
		}
		this->~NewDialog();
	}
	else
	{
		qDebug() << "Name can not be empty!";
		QToolTip::showText(ui.NewServerConfigName->mapToGlobal(QPoint()), tr(msg.toStdString().c_str()));
	}
}

void NewDialog::openFileChooser()
{
	QString dir =QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                "/home",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
	qDebug() << dir;
	if (!dir.isEmpty())
	{
		ui.serverFilePath->setText(dir);
	}

}
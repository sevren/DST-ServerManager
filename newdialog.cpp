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
	if (!getTextFromInputBox(ui.NewServerConfigName).isEmpty() && !getTextFromInputBox(ui.serverFilePath).isEmpty() ) //if not empty
	{	
		if (!dstWindow->serverExists(getTextFromInputBox(ui.NewServerConfigName))) //if server exists
		{
			return make_pair(true,QString());
		}
		else
		{
			return make_pair(false,QString(getTextFromInputBox(ui.NewServerConfigName) +" Already Exists!"));
		}
	}
	else if (getTextFromInputBox(ui.NewServerConfigName).isEmpty() && !getTextFromInputBox(ui.serverFilePath).isEmpty() )
	{
		return make_pair(false,QString("ERR001"));
	}
	else if (!getTextFromInputBox(ui.NewServerConfigName).isEmpty() && getTextFromInputBox(ui.serverFilePath).isEmpty() )
	{
		return make_pair(false,QString("ERR002"));
	}
	else
	{
		return make_pair(false,QString("ERR003"));
	}
}

QString NewDialog::getTextFromInputBox(QLineEdit* textBox)
{
	return textBox->text();
}

void NewDialog::createNewServerConfig()
{
	pair<bool,QString> validated=validate();
	bool valid  = validated.first;
	QString msg = validated.second;

	QString ServerConfigName,ServerPresetType,ServerDirectoryPath;
	
	if (valid)
	{
		qDebug() << "emit create New Server Config";
		ServerConfigName=getTextFromInputBox(ui.NewServerConfigName);
		//Forest must be emitted as a QString from the radio buttons
		if (ui.preset_forest->isChecked())
		{

			ServerPresetType = QString("Forest");
			//emit sendData(getTextFromInputBox(ui.NewServerConfigName),QString("Forest"));
		}
		else if (ui.preset_cave->isChecked())
		{
			ServerPresetType = QString("Cave");
		}
		else if (ui.preset_both->isChecked())
		{
			ServerPresetType = QString("Both");
		}
		ServerDirectoryPath=getTextFromInputBox(ui.serverFilePath);
		emit sendData(ServerConfigName,ServerPresetType,ServerDirectoryPath);
		this->~NewDialog();
	}
	else
	{

		if ("ERR001"==msg)
		{
			qDebug() << "Name can not be empty!";
			QToolTip::showText(ui.NewServerConfigName->mapToGlobal(QPoint()), tr(QString("Name can not be empty!").toStdString().c_str()));
		}
		else if ("ERR002"==msg)
		{
			qDebug() << "Server File Path can not be empty!";
			QToolTip::showText(ui.serverFilePath->mapToGlobal(QPoint()), tr(QString("Server File Path Can not be empty!").toStdString().c_str()));
		}
		else if ("ERR003"==msg)
		{
			qDebug() << "Both Name and Server File Path are empty";
			QToolTip::showText(ui.NewServerConfigName->mapToGlobal(QPoint()), tr(QString("Name can not be empty!").toStdString().c_str()));
			QToolTip::showText(ui.serverFilePath->mapToGlobal(QPoint()), tr(QString("Server File Path can not be empty!").toStdString().c_str()));
		}
		else
		{
			qDebug() << "Validation broke, dumping debug variable(msg): " << msg;
			QToolTip::showText(ui.NewServerConfigName->mapToGlobal(QPoint()), tr(msg.toStdString().c_str()));
		}
		
	}
}

void NewDialog::openFileChooser()
{
	QString dir =QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                "/home",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
	qDebug() <<"Selected Directory is:"<< dir;
	if (!dir.isEmpty())
	{
		ui.serverFilePath->setText(dir);
	}

}
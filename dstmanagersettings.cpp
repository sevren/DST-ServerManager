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
	this->setWindowFlags(Qt::WindowStaysOnTopHint);
	dstManSettings = new GlobalDSTManSettings();

	connect(ui.defaultServerFolderBtn, &QPushButton::clicked, this,[=]() { this->openFileChooser(SettingsBtnType::serverFilePath); });
	connect(ui.steamCMDPathBtn, &QPushButton::clicked, this, [=]() { this->openFileChooser(SettingsBtnType::steamCMDPath); });
	//connect(ui.defaultServerFolderBtn, SIGNAL(clicked()), this, SLOT(openFileChooser(SettingsBtnType::serverFilePath)));
	//connect(ui.steamCMDPathBtn, SIGNAL(clicked()), this, SLOT(openFileChooser(SettingsBtnType::steamCMDPath)));
	connect(ui.saveBtn, SIGNAL(clicked()), this, SLOT(saveSettings()));
	loadSettings();

	//connect(ui.directoryChooserBtn, SIGNAL(clicked()), this, SLOT(openFileChooser()));
	qDebug() << dstWindow->metaObject()->className();
	this->dstWindow = dstWindow;
}



dstmanagersettings::~dstmanagersettings()
{
}

void dstmanagersettings::openFileChooser(SettingsBtnType settingsBtnType)
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty())
	{
		switch (settingsBtnType)
		{
			case serverFilePath:
				ui.defaultServerFolder->setText(dir);
				dstManSettings->defaultServerFilePath = dir.toStdString();
				break;
			case steamCMDPath:
				ui.steamCMDPath->setText(dir);
				dstManSettings->steamCMDExecutablePath = dir.toStdString();
				break;
		}
	
	}
}


bool dstmanagersettings::validated()
{
	bool valid = true;
	if (ui.defaultServerFolder->text().isEmpty())
	{
		QToolTip::showText(ui.defaultServerFolder->mapToGlobal(QPoint()), tr(QString("Default server file path must be set!").toStdString().c_str()));
		valid = false;
	}

	if (ui.steamCMDPath->text().isEmpty())
	{
		QToolTip::showText(ui.steamCMDPath->mapToGlobal(QPoint()), tr(QString("Path to SteamCMD executable must be set!").toStdString().c_str()));
		valid = false;
	}

	if (ui.serverToken->text().isEmpty())
	{
		QToolTip::showText(ui.serverToken->mapToGlobal(QPoint()), tr(QString("Server Cluster token must be set!").toStdString().c_str()));
		valid = false;
	}

	return valid;
}

void dstmanagersettings::loadSettings()
{
	if (QFile::exists(QString::fromStdString(dstManagerSettingsFile)))
	{
		QSettings settings(this->dstManagerSettingsFile, QSettings::IniFormat);

		settings.beginGroup("DstManSettings");
		dstManSettings->defaultServerFilePath = settings.value("defaultServerFilePath").toString().toStdString();
		dstManSettings->steamCMDExecutablePath = settings.value("steamCMDExecutablePath").toString().toStdString();
		dstManSettings->clusterTokenValue = settings.value("clusterTokenValue").toString().toStdString();
		settings.endGroup();

		emit updateDstManSettings(this->dstManSettings);
		ui.defaultServerFolder->setText(QString::fromStdString(dstManSettings->defaultServerFilePath));
		ui.steamCMDPath->setText(QString::fromStdString(dstManSettings->steamCMDExecutablePath));
		ui.serverToken->setText(QString::fromStdString(dstManSettings->clusterTokenValue));
	}
	
}

void dstmanagersettings::saveSettings()
{
	dstManSettings->clusterTokenValue = ui.serverToken->text().toStdString();

	if (validated())
	{
		QSettings settings(this->dstManagerSettingsFile, QSettings::IniFormat);

		settings.beginGroup("DstManSettings");
		settings.setValue("defaultServerFilePath", QString::fromStdString(dstManSettings->defaultServerFilePath));
		settings.setValue("steamCMDExecutablePath", QString::fromStdString(dstManSettings->steamCMDExecutablePath));
		settings.setValue("clusterTokenValue", QString::fromStdString(dstManSettings->clusterTokenValue));
		settings.endGroup();
		emit updateDstManSettings(this->dstManSettings);

	}


}
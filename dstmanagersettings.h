#pragma once
#include "dstman_customdatatypes.h"
#include <QWidget>
#include "ui_dstmanagersettings.h"
#include "dstservermanager.h"

class dstmanagersettings : public QWidget
{
	Q_OBJECT

private slots:
	void openFileChooser(SettingsBtnType);
	void saveSettings();


public:
	
	dstmanagersettings(dstServerManager *dstWindow, QWidget *parent = 0);
	~dstmanagersettings();



signals:
	void sendData(QString,QString,QString);
	void updateDstManSettings(GlobalDSTManSettings*);

private:
	Ui::dstmanagersettings ui;
	dstServerManager *dstWindow;

	bool validated();

	void loadSettings();
	

	const char* dstManagerSettingsFile = "dstmansettings.ini";

	GlobalDSTManSettings* dstManSettings;
	
};

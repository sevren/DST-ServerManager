#pragma once

#include <QWidget>
#include "ui_dstmanagersettings.h"
#include "dstservermanager.h"

class dstmanagersettings : public QWidget
{
	Q_OBJECT

private slots:
	void openFileChooser();


public:
	
	dstmanagersettings(dstServerManager *dstWindow, QWidget *parent = 0);
	~dstmanagersettings();

signals:
	void sendData(QString,QString,QString);

private:
	Ui::dstmanagersettings ui;
	dstServerManager *dstWindow;
	
};

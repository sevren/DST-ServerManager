#ifndef DSTSERVERMANAGER_H
#define DSTSERVERMANAGER_H

#include <QtWidgets/QMainWindow>
#include "ui_dstservermanager.h"
#include "serverconfigurationtab.h"

class dstServerManager : public QMainWindow
{
	Q_OBJECT

private slots:
	void createNewServerConfig();
	void openDialog();
	void settingsDialog();
	void aboutDialog();
		
public:
	dstServerManager(QWidget *parent = 0);
	~dstServerManager();

private:
	Ui::dstServerManagerClass ui;
	serverconfigurationtab *sct;
	void setupConnections();

	
};

#endif // DSTSERVERMANAGER_H

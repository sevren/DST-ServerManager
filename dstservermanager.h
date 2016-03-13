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

public slots:
	void getData(QString);

private:
	Ui::dstServerManagerClass ui;
	QTabWidget *tabWidget;
	serverconfigurationtab *sct;
	std::vector<std::pair<QLabel*, QComboBox*>> ForestWorldArray;
	QImage avatars[96]; //magic numbers for now --> represents all the elements in the worldsettings.png
	void setupConnections();
	void dstServerManager::setupMainLayout();
	QStringList dstServerManager::setComboBoxValues(std::string values);

	
};

#endif // DSTSERVERMANAGER_H

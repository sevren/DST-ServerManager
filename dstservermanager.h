#ifndef DSTSERVERMANAGER_H
#define DSTSERVERMANAGER_H

#include <QtWidgets/QMainWindow>
#include "ui_dstservermanager.h"
#include "serverconfigurationtab.h"

using namespace std;

class dstServerManager : public QMainWindow
{
	Q_OBJECT

private slots:
	void createNewServerConfig();
	void openDialog();
	void settingsDialog();
	void aboutDialog();
	void closeTab_(int index);

public:
	dstServerManager(QWidget *parent = 0);
	bool dstServerManager::serverExists(QString);
	~dstServerManager();

public slots:
	void getData(QString,QString,QString);

private:
	Ui::dstServerManagerClass ui;
	QTabWidget *tabWidget;
	dstServerManager* dstWindow;
	std::vector<std::tuple<int,string,string,string>> ForestWorldArray;
	std::vector<std::tuple<int,string,string,string>> ForestResourcesArray;
	std::vector<std::tuple<int,string,string,string>> ForestFoodArray;
	std::vector<std::tuple<int,string,string,string>> ForestAnimalsArray;
	std::vector<std::tuple<int,string,string,string>> ForestMonstersArray;
	//
	std::vector<std::tuple<int,string,string,string>> CaveWorldArray;
	std::vector<std::tuple<int,string,string,string>> CaveResourcesArray;
	std::vector<std::tuple<int,string,string,string>> CaveFoodArray;
	std::vector<std::tuple<int,string,string,string>> CaveAnimalsArray;
	std::vector<std::tuple<int,string,string,string>> CaveMonstersArray;
	QImage avatars[96]; //magic numbers for now --> represents all the elements in the worldsettings.png
	void setupConnections();
	void dstServerManager::setupMainLayout();
	std::vector<std::tuple<int,string,string,string>> dstServerManager::fillGameOptionsArray(const char *);
	
	
};

#endif // DSTSERVERMANAGER_H

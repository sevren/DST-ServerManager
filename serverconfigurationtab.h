#ifndef SERVERCONFIGURATIONTAB_H
#define SERVERCONFIGURATIONTAB_H

#include <QWidget>
#include "ui_serverconfigurationtab.h"

class serverconfigurationtab : public QWidget
{
	Q_OBJECT

private slots:
	void closeTab_(int index);
		

public:
	serverconfigurationtab(QWidget *parent = 0);
	~serverconfigurationtab();

private:
	Ui::serverconfigurationtab ui;
	
};

#endif // SERVERCONFIGURATIONTAB_H

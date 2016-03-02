#ifndef TESTDLG_H
#define TESTDLG_H

#include <QWidget>
#include "ui_testdlg.h"

class testdlg : public QWidget
{
	Q_OBJECT

public:
	testdlg(QWidget *parent = 0);
	~testdlg();

private:
	Ui::testdlg ui;
};

#endif // TESTDLG_H

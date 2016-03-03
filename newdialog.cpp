#include "newdialog.h"
#include <QDebug>

NewDialog::NewDialog(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.buttonBox,SIGNAL(accepted()),this,SLOT(createNewServerConfig()));
}

NewDialog::~NewDialog()
{

}

bool NewDialog::validate()
{
	if (!getTextFromInputBox().isEmpty())
	{
		return true;
	}
	else
	{
	return false;
	}
}

QString NewDialog::getTextFromInputBox()
{
	return ui.NewServerConfigName->text();
}

void NewDialog::createNewServerConfig()
{
	
	if (validate())
	{
		qDebug() << "emit create New Server Config";
		emit sendData(getTextFromInputBox());
		this->~NewDialog();
	}
	else
	{
		qDebug() << "Name can not be empty!";
	}
	
}
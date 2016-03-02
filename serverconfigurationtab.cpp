#include "serverconfigurationtab.h"

serverconfigurationtab::serverconfigurationtab(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab_(int)));
	
}

void serverconfigurationtab::closeTab_(int index)
{
	ui.tabWidget->removeTab(index);
}

serverconfigurationtab::~serverconfigurationtab()
{

}

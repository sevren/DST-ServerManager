#include "serverconfigurationtab.h"
#include <qimage.h>
#include <QDebug>


serverconfigurationtab::serverconfigurationtab(QImage avatars[], std::vector<std::pair<QLabel*, QComboBox*>>& v,QWidget *parent)
	: QWidget(parent)
{


	ui.setupUi(this);
	//ui.imgTest= new QLabel(v[0].first);
	//ui.imgTest->setText("fgdgfd");	
	qDebug() << "Size of array: " <<v.size();

	
	/*ui.gridLayout->addWidget(v[0].first,0,0);  //0,0
	ui.gridLayout->addWidget(v[0].second,0,1); //0,1
	
	ui.gridLayout->addWidget(v[1].first,0,2);  //0,2
	ui.gridLayout->addWidget(v[1].second,0,3); //0,3

	ui.gridLayout->addWidget(v[2].first,1,0);  //1,0
	ui.gridLayout->addWidget(v[2].second,1,1); //1,1


	ui.gridLayout->addWidget(v[3].first,1,2);  //1,2
	ui.gridLayout->addWidget(v[3].second,1,3); //1,3

	ui.gridLayout->addWidget(v[4].first,2,0);  //2,0
	ui.gridLayout->addWidget(v[4].second,2,1); //2,1

	ui.gridLayout->addWidget(v[5].first,2,2);  //2,2
	ui.gridLayout->addWidget(v[5].second,2,3); //2,3

	*/

	//ui.gridLayout->addWidget(v[4].first,0,2,1,1); //QLabel with image,
	//ui.gridLayout->addWidget(v[4].second,0,3,1,1); //gridLayout->addWidget(label_12, 1, 0, 1, 1);

	int indx=0;
	int counter=0;
		for (int gridRow=0;gridRow<18;gridRow++)
		{

			for(int gridCol=0;gridCol<4;gridCol++)
			{
				if (indx>=18)
					{
						break;
					}
					qDebug() << gridRow<<","<< gridCol<< "  counter is "<< counter << " index is" << indx;
					ui.gridLayout->addWidget(v[indx].first,gridRow,gridCol);
					ui.gridLayout->addWidget(v[indx].second,gridRow,gridCol+1);
					counter++;
					if(counter % 2==0)
					{
						indx++;
					}
					

			}
			
		}
	
	
	
	/*int gridRow=0;
	int gridCol=0;
	int index=0;
		for (int gridRow=0;gridRow<2;gridRow++)
		{
			ui.gridLayout->addWidget(v[gridRow].first,gridRow,gridCol,1,1); //QLabel with image,
			qDebug() << "Adding QLabel @ "<< gridRow<<","<< gridCol;
			for (int gridCol=0;gridCol<2;gridCol++)
			{
				qDebug() << "Adding QCombox @ "<< gridRow<<","<< gridCol;
			ui.gridLayout->addWidget(v[gridRow].second,gridRow,gridCol,1,1); //gridLayout->addWidget(label_12, 1, 0, 1, 1);
		 
			}
			
		}*/

	


	
	/*QImage image(".//Resources//imgs//worldsettings.png");
    QImage copy ;
	
    copy = image.copy( 128,128, 128, 128);
	copy=copy.scaled(50, 50, Qt::KeepAspectRatio);*/
	/*for (int i=0;i<95;i++)
	{
		
		ui.imgTest->setPixmap(QPixmap::fromImage(avatars[i]));
	}*/
	
    //copy.save("cropped_image.jpg");
	//connect(ui.tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab_(int)));
	
}

void serverconfigurationtab::closeTab_(int index)
{
	//ui.tabWidget->removeTab(index);
}

serverconfigurationtab::~serverconfigurationtab()
{

}

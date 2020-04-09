#include "dialogopenfile.h"
#include <QtWidgets>
#include <QMainWindow>
#include <QDebug>
#include <QtConcurrent>

DialogOpenFile::DialogOpenFile(QString txtFile, int number, QWidget *parent) :
    QDialog(parent)
    //QDialog(0, Qt::Window | Qt::FramelessWindowHint)
{
    //txtFileQString = txtFile;
    //number_of_device = number;

    QGridLayout *grid = new QGridLayout;
    grid->addWidget(groupTableSettings(), 0, 0);
    setLayout(grid);
	
    labelInfo->setText(QString ("Info: 45"));
    open_file();

    connect(button_plot_F, SIGNAL(clicked()), this, SLOT(save_data()));

    for (int i=0; i<30; i++){
        connect(checkBox_F_X[i], static_cast<void(QCheckBox::*)(int)>(&QCheckBox::stateChanged),
            [=](int index){
                                setCheckBox(0);
                                numberOfX=i;
       });

        connect(checkBox_F_Y[i], static_cast<void(QCheckBox::*)(int)>(&QCheckBox::stateChanged),
            [=](int index){
                                setCheckBox(1);
                                numberOfY=i;
                                button_plot_F->setEnabled(true);
                                comboBox_Device->show();
       });
    }
}


void DialogOpenFile::save_data(){
    qVectorX.clear();
    qVectorY.clear();
    double xMin,xMax,yMin,yMax=0;

    QFile file (filePath);
    int fileSize = file.size();
    int number_line = fileSize/100;
    if(QFileInfo(filePath).exists()){
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QStringList list_temp;
        QString line;
        int i=-2;
        QTextStream in(&file);
        while (!in.atEnd()) {
            line = in.readLine();
            i++;
            if (i>0){
                list_temp = line.split("\t");
                progressBar->setValue(i*100/number_line);
                if (i==1){
                    xMin=xMax=list_temp[numberOfX].toDouble();
                    yMin=yMax=list_temp[numberOfY].toDouble();
                }
                if (list_temp[numberOfX].toDouble()>xMax) xMax=list_temp[numberOfX].toDouble();
                if (list_temp[numberOfX].toDouble()<xMin) xMin=list_temp[numberOfX].toDouble();
                if (list_temp[numberOfY].toDouble()>yMax) yMax=list_temp[numberOfY].toDouble();
                if (list_temp[numberOfY].toDouble()<yMin) yMin=list_temp[numberOfY].toDouble();

                qVectorX.push_back(list_temp[numberOfX].toDouble());
                qVectorY.push_back(list_temp[numberOfY].toDouble());
            }
        }
        file.flush();
        file.close();

        send_VectorXY(qVectorX, qVectorY, comboBox_Device->currentIndex());
        sendMinMaxofVectorXY(xMin,xMax,yMin,yMax,comboBox_Device->currentIndex());

        file.open(QIODevice::ReadOnly | QIODevice::Text);
        line = in.readLine();
        list_temp = line.split("\t");
        QString temp4 = list_temp[numberOfY];
        file.flush();
        file.close();
        sendFileName(QString ("%1 %2").arg(filePath.split("/").last()).arg(temp4),comboBox_Device->currentIndex());
    }
    close();
}

void DialogOpenFile::setCheckBox(int temp){
    if (temp==0){
        for (int i=0; i<30;i++)
        {
            checkBox_F_X[i]->setEnabled(false);
            checkBox_F_Y[i]->setEnabled(true);
        }
    }else{
        for (int i=0; i<30;i++)
        {
            checkBox_F_Y[i]->setEnabled(false);
        }
    }
}



void DialogOpenFile::open_file(){

    filePath = QFileDialog::getOpenFileName( this, trUtf8( "Open file" ), ".", trUtf8( "Data (*.txt *.dat)" ) );

    QFile file(filePath);

    QString str_file_str;
    QString str_file[5];
    QStringList list_file;
    QString textData[30];

    int t_file =0;


    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        for (int i=0; i<5; i++){
            str_file[i]= "";
            str_file[i]= file.readLine();
            str_file[i].remove("\n");
        }
    }

    if (str_file[4].size()>0) {
        for (int i=0; i<5; i++){
            switch (i) {
            case 0:
                list_file = str_file[i].split('\t');
                t_file = list_file.size();  if (t_file>30) t_file=30;
                for (int i=0; i<t_file; i++)
                {
                    if (list_file[i]=="") {t_file=i; break;}
                    textData[i].append(list_file[i]);
                    labelData[i]->setText(textData[i]);
                }
                break;
            case 1:
                list_file = str_file[i].split('\t');
                for (int i=0; i<t_file; i++)
                {
                    textData[i].append(",");
                    textData[i].append(list_file[i]);
                    labelData[i]->setText(textData[i]);
                }
                break;
            case 4:
                list_file = str_file[i].split('\t');
                for (int i=0; i<t_file; i++)
                {
                    textData[i].append(" (");
                    textData[i].append(list_file[i]);
                    textData[i].append(")");
                    labelData[i]->setText(textData[i]);
                }
                hide_empty(t_file);
                labelImportTo->setText(tr("Import to:"));
                break;
            }
        }
    }
    else{
      hide_empty(0);
      labelImportTo->setText(tr("No Data"));
      comboBox_Device->hide();
      button_plot_F->setEnabled(false);
    }

    file.flush();
    file.close();
    labelInfo->setText(filePath);
}

void DialogOpenFile::hide_empty(int t_file){
    for (int i = 29; i>= t_file ; i--)
    {
        labelData[i] ->hide();
        checkBox_F_X[i] ->hide();
        checkBox_F_Y[i] ->hide();
    }
}

void DialogOpenFile::delete_data(){

}


QGroupBox *DialogOpenFile::groupTableSettings()
{
    QGroupBox *groupBox = new QGroupBox(tr(""));

    labelInfo = new QLabel(QString ("Info: 2"));
    labelFileStr = new QLabel(QString ("labelFileStr"));
    labelD = new QLabel(QString ("Data"));
    labelX = new QLabel(QString ("X"));
    labelY = new QLabel(QString ("Y"));
    for (int i=0; i<30; i++){
        labelData[i] = new QLabel(QString ("T,K"));
        checkBox_F_X[i] = new QCheckBox("");
        checkBox_F_Y[i] = new QCheckBox("");
        checkBox_F_Y[i]->setEnabled(false);
    }
    labelImportTo = new QLabel(QString ("Import to:"));
    comboBox_Device = new QComboBox;
    //QStringList colorsQStringList_edit = { "black", "red", "green", "blue", "gray", "magenta", "darkCyan", "darkRed", "darkMagenta", "darkGreen", "darkYellow", "darkBlue", "darkGray", "lightGray", "black", "red", "green", "blue", "gray", "black" };
    QStringList colorsQStringList = { "white", "black", "cyan", "red", "magenta", "green", "yellow", "blue", "gray", "darkCyan", "darkRed", "darkMagenta", "darkGreen", "darkYellow", "darkBlue", "darkGray", "lightGray","white", "black", "cyan", "red", "magenta", "green", "yellow", "blue" };

    for (int i=0; i<10 /*20*/; i++){
    comboBox_Device->addItem(QString ("Line %1 (%2)").arg(i).arg(colorsQStringList.at(i)));
    }
    button_plot_F = new QPushButton(tr("Import"));

    progressBar = new QProgressBar();
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);

    //comboBox_Device->hide();
    button_plot_F->setEnabled(false);

    QGridLayout * gridLayout = new QGridLayout();

    //gridLayout->addWidget(labelInfo,0,0,1,4);
    gridLayout->addWidget(labelD,1,0,1,2);
    gridLayout->addWidget(labelX,1,2,1,1);
    gridLayout->addWidget(labelY,1,3,1,1);

    for (int i=0; i<30; i++){
        gridLayout->addWidget(labelData[i],1+1+i,0,1,1);
        gridLayout->addWidget(checkBox_F_X[i],1+1+i,2,1,1);
        gridLayout->addWidget(checkBox_F_Y[i],1+1+i,3,1,1);
    }
    gridLayout->addWidget(labelImportTo,34,0,1,1);
    gridLayout->addWidget(comboBox_Device,34,1,1,1);
    gridLayout->addWidget(button_plot_F,34,2,1,2);

    gridLayout->addWidget(progressBar,35,0,1,4);

    groupBox->setLayout(gridLayout);

    return groupBox;
}

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

    //QMap <int, QStringList> qMapDataOpenFile;
    //qVectorData.clear();
    QFile file (filePath);
    int fileSize = file.size();
    int number_line = fileSize/100;
    if(QFileInfo(filePath).exists()){
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QStringList list_temp;
        QString line;
        QStringList list_for_qmap;
        int i=-2;
        QTextStream in(&file);
        while (!in.atEnd()) {
            line = in.readLine();
            i++;
            if (i>=0){
                list_temp = line.split("\t");
                list_for_qmap.append(list_temp[numberOfX]);
                list_for_qmap.append(list_temp[numberOfY]);
                //qVectorData.push_back(list_for_qmap);
                //progressBar->setValue(i/100);
                progressBar->setValue(i*100/number_line);
                send_x_result(list_temp[numberOfX].toFloat());
                send_all_results(list_temp[numberOfY].toFloat(),comboBox_Device->currentIndex());
                appendPointXY(comboBox_Device->currentIndex());
                //sendAll(list_temp[numberOfX].toFloat(),list_temp[numberOfY].toFloat(),comboBox_Device->currentIndex());
                list_for_qmap.clear();
            }
        }
        file.flush();
        file.close();
    }

    close();
    //qVectorData.remove(0,2);
    //send_qVectorData(2,qVectorData);


    //send_qMap(comboBox_Device->currentIndex(),qMapDataOpenFile);
    //for (int i=-2; i<5; i++){qDebug() << qMapDataOpenFile.contains(i);}

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
    for (int i=0; i<20; i++){
    comboBox_Device->addItem(QString ("Device %1").arg(i));
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

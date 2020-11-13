#include "dialogsavefile.h"
#include <QtWidgets>
#include <QMainWindow>
#include <QDebug>
#include <QtConcurrent>

DialogSaveFile::DialogSaveFile(QString file, QVector<double> X, QVector<double> Y, QWidget *parent) :
    QDialog(parent)
    //QDialog(0, Qt::Window | Qt::FramelessWindowHint)
{
    filePath = file.split("|").first();
    nameDevice = file.split("|").last();
    qDebug() << "filePath" << filePath;
    filePath = filePath.split("_corr").first();
    qVectorX = X;
    qVectorY = Y;

	QGridLayout *grid = new QGridLayout;
    grid->addWidget(groupTableSettings(), 0, 0);
    setLayout(grid);

    check_file();

    connect(button_save, SIGNAL(clicked()), this, SLOT(save_file()));
}

void DialogSaveFile::check_file(){
    QDateTime date=QDateTime::currentDateTime();
    filePath.append("_corr(");
    filePath.append(date.toString("yyyy.MM.dd_hh.mm.ss"));
    filePath.append(").txt");
    newfilePath=filePath;
    int i=1;

    while(QFileInfo(newfilePath).exists()){
        newfilePath=filePath;
        newfilePath.append(QString("_%1").arg(i));
        i++;
    }
    labelFileStr->setText(newfilePath);

}

void DialogSaveFile::save_file(){

    QFile file (newfilePath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    out << "X\t";
    out << nameDevice; out << "\t";
    out << "\n";

    out << "X\t";
    out << "None\t";
    out << "\n";

    int size_vector = qVectorX.size();
    for (int i=0; i<size_vector;i++){
        out << qVectorX.at(i); out << "\t";
        out << qVectorY.at(i); out << "\t";
        out << "\n";
        progressBar->setValue(i*100/size_vector);
    }

    file.flush();
    file.close();
	
    close();
}


QGroupBox *DialogSaveFile::groupTableSettings()
{
    QGroupBox *groupBox = new QGroupBox(tr(""));

    labelInfo = new QLabel(QString ("File: "));
    labelFileStr = new QLabel(QString ("//"));

    button_save = new QPushButton(tr("Save"));

    progressBar = new QProgressBar();
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);


    QGridLayout * gridLayout = new QGridLayout();


    gridLayout->addWidget(labelInfo,0,0,1,1);
    gridLayout->addWidget(labelFileStr,0,1,1,1);
    gridLayout->addWidget(button_save,0,2,1,2);
    gridLayout->addWidget(progressBar,1,0,1,4);

    groupBox->setLayout(gridLayout);

    return groupBox;
}

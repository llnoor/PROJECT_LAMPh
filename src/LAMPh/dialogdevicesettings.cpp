#include "dialogdevicesettings.h"
#include <QtWidgets>
#include <QMainWindow>

DialogDeviceSettings::DialogDeviceSettings(QString dllFile, int number, QWidget *parent) :
    QDialog(parent)
    //QDialog(0, Qt::Window | Qt::FramelessWindowHint)
{
    dllFileQString = dllFile;
    number_of_device = number;

    //ui->setupUi(this);

    QGridLayout *grid = new QGridLayout;
    grid->addWidget(groupTableSettings(), 0, 0);
    setLayout(grid);
	
	
    QString infoString = "";
    QLibrary lib ( dllFileQString );
    typedef const char* ( *GetInfo )();
    GetInfo getInfo;
    getInfo = ( GetInfo ) lib.resolve( "getInfo" );
    if( getInfo ) {
        infoString = QString::fromUtf8(getInfo());
    }
    labelInfo->setText(QString ("Info: %1").arg(infoString));


    for (int i=0; i<BUTTONNUM; i++)
    {
        typedef const char* ( *GetRowName )(int, int);
        GetRowName getRowName;
        getRowName = ( GetRowName ) lib.resolve( "getRowName" );
        labelRowName[i] -> setText(QString::fromLatin1(getRowName(number_of_device,i)));
    }

    for (int row=0; row<BUTTONNUM; row++)
    {
        for (int column=0; column<BUTTONNUM; column++)
        {
            typedef const char* ( *GetButtonName )(int, int, int);
            GetButtonName getButtonName;
            getButtonName = ( GetButtonName ) lib.resolve( "getButtonName" );
            pushButtonButtonName[row][column] -> setText(QString::fromLatin1(getButtonName(number_of_device,column,row)));
        }
    }

    for (int i=0; i<BUTTONNUM; i++)
    {
        typedef const char* ( *GetRowData )(int, int);
        GetRowData getRowData;
        getRowData = ( GetRowData ) lib.resolve( "returnRowData" );
        lineEditRowData[i] -> setText(QString::fromLatin1(getRowData(number_of_device,i)));
    }

    for (int i=0; i<BUTTONNUM; i++)
    {
        typedef const char* ( *GetLineName )(int, int);
        GetLineName getLineName;
        getLineName = ( GetLineName ) lib.resolve( "getLineName" );
        labelLineName[i] -> setText(QString::fromLatin1(getLineName(number_of_device,i)));
    }


}

DialogDeviceSettings::~DialogDeviceSettings(){
 //delete ui;
}

void DialogDeviceSettings::save_data(){

}

void DialogDeviceSettings::delete_data(){

}

void DialogDeviceSettings::accept(){

}

bool DialogDeviceSettings::setParameterButton(int row, int column){

    QLibrary lib ( dllFileQString );
    typedef bool (*SetParameterButton) (int,int,int,float);
    SetParameterButton setParameterButton;
    setParameterButton = (SetParameterButton)(lib.resolve("setParameterButton"));
    if (setParameterButton(number_of_device,column,row,1/*true*/))
    {

        typedef const char* ( *ReturnRowData )(int,int);
        ReturnRowData returnRowData;
        returnRowData = ( ReturnRowData ) lib.resolve( "returnRowData" );
        lineEditRowData[row] -> setText(QString::fromLatin1(returnRowData(number_of_device, row)));
        return true;
    }
}

bool DialogDeviceSettings::setParameterLine(int row){
    QLibrary lib ( dllFileQString );
    typedef bool (*SetParameterLine) (int,int,float);
    SetParameterLine setParameterLine;
    setParameterLine = (SetParameterLine)(lib.resolve("setParameterLine"));
    float returnFloat = lineEditParameterLine[row]->text().toFloat();
    return setParameterLine(number_of_device,row,returnFloat);
}

QGroupBox *DialogDeviceSettings::groupTableSettings()
{
    QGroupBox *groupBox = new QGroupBox(tr(""));
    //groupBox->setStyleSheet("border: 0px solid white");

    //labelName = new QLabel(QString ("Name: %1#%2").arg(dllFileQString).arg(number_of_device));
    labelInfo = new QLabel(QString ("Info: %1#%2").arg(dllFileQString).arg(number_of_device));

    for (int i=0; i<BUTTONNUM; i++)
    {
        labelRowName[i] = new QLabel(QString ("Row: %1").arg(i));
    }

    for (int row=0; row<BUTTONNUM; row++)
    {
        for (int column=0; column<BUTTONNUM; column++)
        {
            pushButtonButtonName[row][column] = new QPushButton(QString ("R:%1 C:%2").arg(row).arg(column));


            connect(pushButtonButtonName[row][column], static_cast<void(QPushButton::*)(void)>(&QPushButton::released),
                    [=](void){
                            setParameterButton(row,column);
               });
        }
    }

    for (int i=0; i<BUTTONNUM; i++)
    {
        lineEditRowData[i] = new QLineEdit();
        lineEditRowData[i]->setReadOnly(true);
        lineEditRowData[i]->setEnabled(false);
    }

    for (int i=0; i<BUTTONNUM; i++)
    {
        labelLineName[i] = new QLabel(QString ("Line: %1").arg(i));
    }

    for (int i=0; i<BUTTONNUM; i++)
    {
        lineEditParameterLine[i] = new QLineEdit();
    }

    for (int i=0; i<BUTTONNUM; i++)
    {
        pushButtonSend[i] = new QPushButton(tr("Send"));
        connect(pushButtonSend[i], static_cast<void(QPushButton::*)(void)>(&QPushButton::released),
                [=](void){
                        setParameterLine(i);
           });
    }

    QGridLayout * gridLayout = new QGridLayout();

    //gridLayout->addWidget(labelName,0,0);
    gridLayout->addWidget(labelInfo,1,0,1,4);

    for (int i=0; i<BUTTONNUM; i++)
    {
        gridLayout->addWidget(labelRowName[i],2+i,0);
    }

    for (int row=0; row<BUTTONNUM; row++)
    {
        for (int column=0; column<BUTTONNUM; column++)
        {
            gridLayout->addWidget(pushButtonButtonName[row][column],2+row,1+column);
        }
    }

    for (int i=0; i<BUTTONNUM; i++)
    {
        gridLayout->addWidget(lineEditRowData[i],2+i,7);
    }


    for (int i=0; i<BUTTONNUM; i++)
    {
        gridLayout->addWidget(labelLineName[i],7+i,0);
    }

    for (int i=0; i<BUTTONNUM; i++)
    {
        gridLayout->addWidget(lineEditParameterLine[i],7+i,1,1,2);
    }

    for (int i=0; i<BUTTONNUM; i++)
    {
        gridLayout->addWidget(pushButtonSend[i],7+i,3);
    }

    groupBox->setLayout(gridLayout);

    return groupBox;
}

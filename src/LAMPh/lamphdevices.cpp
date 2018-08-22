#include <qlabel.h>
#include <qlayout.h>
#include <qstatusbar.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qgroupbox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qwhatsthis.h>
#include <qpixmap.h>


#include "lamphdevices.h"
#include "start.xpm"
#include "clear.xpm"
#include "pixmaps.h"

#include <QLineEdit>
#include <QFileDialog>
#include <QPushButton>
#include <QComboBox>

#include <QDebug>
#include <QLibrary>

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QTextStream>

QT_USE_NAMESPACE

class MyToolBar: public QToolBar
{
public:
    MyToolBar( LAMPhDevices *parent ):
        QToolBar( parent )
    {
    }
    void addSpacing( int spacing )
    {
        QLabel *label = new QLabel( this );
        addWidget( label );
        label->setFixedWidth( spacing );
    }
};

LAMPhDevices::LAMPhDevices(QString loginQString)
{
    addToolBar(Qt::TopToolBarArea, toolBar()); //buttons
    addToolBar(Qt::LeftToolBarArea, toolBar_GET()); //these are fields for selecting functions of DEVICES
    addToolBar(Qt::LeftToolBarArea, toolBar_COUNTERS());

    /*addToolBar(Qt::LeftToolBarArea, toolBar_SEND());
    addToolBar(Qt::LeftToolBarArea, toolBar_COUNTERS());*/
    addToolBar(Qt::LeftToolBarArea, toolBar_PORTS()); //All Available Serial Ports (COM+USB+LAN+Sockets and so on)
    //addToolBar(Qt::RightToolBarArea, toolBar_DEVICES());
#ifndef QT_NO_STATUSBAR
    ( void )statusBar();
#endif

    login = new QString(); // to transfer the user's login and delineation of rights
    *login = loginQString;

    labelPlotSettingS = new QLabel(tr(" ")); //free space in the center of the window, due to "mainwindow" features

    W_File = new class_write_in_file();
    W_File->create_new_file();

    QSerialCOM = new qserialcomport();

    setCentralWidget( labelPlotSettingS ); //mainwindow does not work without this thing

    //initWhatsThis();

    setContextMenuPolicy( Qt::NoContextMenu );

    setWindowTitle(tr("LAMPhDevices - %1 ").arg(login->toLower()));
    showMinimized();

    getAllAvailableSerialPorts();
    showMaximized();


    // this is for switching between windows
    connect( d_OpenWindow_Main, SIGNAL( triggered() ), this, SIGNAL(showLAMPhPlot()) );
    connect( d_OpenWindow_Main, SIGNAL( triggered() ), this, SLOT(close()) );
    connect( d_OpenWindow_Devices, SIGNAL( triggered() ), this, SIGNAL(showLAMPhDevices()) );
    connect( d_OpenWindow_Devices, SIGNAL( triggered() ), this, SLOT(close()) );
    connect( d_OpenWindow_Temp, SIGNAL( triggered() ), this, SIGNAL(showLAMPhTemp()) );
    connect( d_OpenWindow_Temp, SIGNAL( triggered() ), this, SLOT(close()) );
    connect( d_OpenWindow_DataTable, SIGNAL( triggered() ), this, SIGNAL(showDataTable()) );
    connect( d_OpenWindow_DataTable, SIGNAL( triggered() ), this, SLOT(close()) );
    connect( d_OpenWindow_Edit, SIGNAL( triggered() ), this, SIGNAL(showLAMPhEdit()) );
    connect( d_OpenWindow_Edit, SIGNAL( triggered() ), this, SLOT(close()) );
    connect( d_OpenWindow_Setting, SIGNAL( triggered() ), this, SIGNAL(showLAMPhSetting()) );
    connect( d_OpenWindow_Setting, SIGNAL( triggered() ), this, SLOT(close()) );

    //connect( d_OpenWindow_Exit, SIGNAL( triggered() ), this, SIGNAL(LAMPhExit()) );
    //connect( d_OpenWindow_Exit, SIGNAL( triggered() ), this, SLOT(close()) );

    connect (d_sendAction, SIGNAL( triggered() ) , this, SLOT (send_readData()));
    connect (d_getAction, SIGNAL( triggered() ) , this, SLOT (readData()));
    connect (d_colorsAction, SIGNAL( triggered() ) , this, SLOT (sendColors()));
    connect (d_connectAction, SIGNAL( triggered() ) , this, SLOT (getAllAvailableSerialPorts()));
    connect (d_saveAction,SIGNAL( triggered() ) , this, SLOT(saveConf()));
    connect (d_loadAction,SIGNAL( triggered() ) , this, SLOT(loadConf()));


    for (int r=0; r<CurvCnt; r++)
    {
        connect(comboBox_Device[r], static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                 [=](int index){
                 update_comboBox_Device_Functions(r,index);
            });

        connect(comboBox_Device_Functions[r], static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                 [=](int index){
                 update_comboBoxes_Function_Parameters(r,index);
            });

        connect(comboBox_Function_Parameters[r], static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                 [=](int index){
                 update_comboBox_Function_Parameters(r,index);
            });

        connect(comboBox_ColorData[r], static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                [=](int index){
                setColorSize(r,index,comboBox_SizeData[r]->currentIndex());
           });
        connect(comboBox_SizeData[r], static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                [=](int index){
                setColorSize(r,comboBox_ColorData[r]->currentIndex(),index);
           });

        connect(checkBox_Devices_X[r], SIGNAL(toggled(bool)),this,SLOT(setCheckBox()) );
        connect(checkBox_Devices_Y[r], SIGNAL(toggled(bool)),this,SLOT(setCheckBox()) );
        connect(checkBox_Device_Text[r], SIGNAL(toggled(bool)),this,SLOT(setCheckBox()) );
    }



}

void LAMPhDevices::setCheckBox() //this automatically switches checkBoxes so that there is no controversy in the program
{
    int new_int=0;
    for (int i=0;i<CurvCnt;i++)
    {
        if (checkBox_Devices_X[i]->isChecked()) {
            number_of_checkBox = i;
            new_int++;
        }

        if (checkBox_Devices_Y[i]->isChecked()) {
            setNumberDevice_bool(1,i);
        }
        else{
            setNumberDevice_bool(0,i);
        }

        if (checkBox_Device_Text[i]->isChecked()) {
            W_File->get_bool(1,i);
        }else
        {
            W_File->get_bool(0,i);
        }

    }

    if (new_int>1)
    {
     for (int i=0;i<CurvCnt;i++)
     {
         if (number_of_checkBox_tmp==i) {
             checkBox_Devices_X[i]->setChecked(false);
             checkBox_Devices_Y[i]->setChecked(true);
             //checkBox_Devices_Show[i]->setChecked(true);
             break;
         }
     }
     number_of_checkBox=number_of_checkBox_tmp;
    }
    number_of_checkBox_tmp=number_of_checkBox;

    for (int i=0;i<CurvCnt;i++)
    {
        if (number_of_checkBox==i)
        {
            if (new_int!=0){
                checkBox_Devices_Y[i]->setChecked(false);
                //checkBox_Devices_Show[i]->setChecked(false);
            }else {
                checkBox_Devices_Y[i]->setChecked(true);
                //checkBox_Devices_Show[i]->setChecked(true);
                break;
            }
        }

    }

    // // // Simple and understandable explanation // // //

    /*int new_int=0;

    if (checkBox_A1_X->isChecked()) {number_of_checkBox = 1; new_int++;}
    if (checkBox_A2_X->isChecked()) {number_of_checkBox = 2; new_int++;}
    if (checkBox_A3_X->isChecked()) {number_of_checkBox = 3; new_int++;}
    if (checkBox_A4_X->isChecked()) {number_of_checkBox = 4; new_int++;}

    if (checkBox_K1_X->isChecked()) {number_of_checkBox = 11; new_int++;}
    if (checkBox_K2_X->isChecked()) {number_of_checkBox = 12; new_int++;}
    if (checkBox_K3_X->isChecked()) {number_of_checkBox = 13; new_int++;}
    if (checkBox_K4_X->isChecked()) {number_of_checkBox = 14; new_int++;}

    if (checkBox_L1_X->isChecked()) {number_of_checkBox = 21; new_int++;}
    if (checkBox_L2_X->isChecked()) {number_of_checkBox = 22; new_int++;}
    if (checkBox_L3_X->isChecked()) {number_of_checkBox = 23; new_int++;}
    if (checkBox_L4_X->isChecked()) {number_of_checkBox = 24; new_int++;}

    if (new_int>1)
    {
        switch (number_of_checkBox_tmp) {
        case 1:checkBox_A1_X->setChecked(false);checkBox_A1_Y->setChecked(true);break;
        case 2:checkBox_A2_X->setChecked(false);checkBox_A2_Y->setChecked(true);break;
        case 3:checkBox_A3_X->setChecked(false);checkBox_A3_Y->setChecked(true);break;
        case 4:checkBox_A4_X->setChecked(false);checkBox_A4_Y->setChecked(true);break;

        case 11:checkBox_K1_X->setChecked(false);checkBox_K1_Y->setChecked(true);break;
        case 12:checkBox_K2_X->setChecked(false);checkBox_K2_Y->setChecked(true);break;
        case 13:checkBox_K3_X->setChecked(false);checkBox_K3_Y->setChecked(true);break;
        case 14:checkBox_K4_X->setChecked(false);checkBox_K4_Y->setChecked(true);break;

        case 21:checkBox_L1_X->setChecked(false);checkBox_L1_Y->setChecked(true);break;
        case 22:checkBox_L2_X->setChecked(false);checkBox_L2_Y->setChecked(true);break;
        case 23:checkBox_L3_X->setChecked(false);checkBox_L3_Y->setChecked(true);break;
        case 24:checkBox_L4_X->setChecked(false);checkBox_L4_Y->setChecked(true);break;
        }
        number_of_checkBox=number_of_checkBox_tmp;
    }
        number_of_checkBox_tmp=number_of_checkBox;
    switch (number_of_checkBox) {
    case 1:if (new_int!=0)checkBox_A1_Y->setChecked(false);else checkBox_A1_Y->setChecked(true);break;
    case 2:if (new_int!=0)checkBox_A2_Y->setChecked(false);else checkBox_A2_Y->setChecked(true);break;
    case 3:if (new_int!=0)checkBox_A3_Y->setChecked(false);else checkBox_A3_Y->setChecked(true);break;
    case 4:if (new_int!=0)checkBox_A4_Y->setChecked(false);else checkBox_A4_Y->setChecked(true);break;
    case 11:if (new_int!=0)checkBox_K1_Y->setChecked(false);else checkBox_K1_Y->setChecked(true);break;
    case 12:if (new_int!=0)checkBox_K2_Y->setChecked(false);else checkBox_K2_Y->setChecked(true);break;
    case 13:if (new_int!=0)checkBox_K3_Y->setChecked(false);else checkBox_K3_Y->setChecked(true);break;
    case 14:if (new_int!=0)checkBox_K4_Y->setChecked(false);else checkBox_K4_Y->setChecked(true);break;
    case 21:if (new_int!=0)checkBox_L1_Y->setChecked(false);else checkBox_L1_Y->setChecked(true);break;
    case 22:if (new_int!=0)checkBox_L2_Y->setChecked(false);else checkBox_L2_Y->setChecked(true);break;
    case 23:if (new_int!=0)checkBox_L3_Y->setChecked(false);else checkBox_L3_Y->setChecked(true);break;
    case 24:if (new_int!=0)checkBox_L4_Y->setChecked(false);else checkBox_L4_Y->setChecked(true);break;
    }*/

}

void LAMPhDevices::getAllAvailableSerialPorts(){ // main

    //int numberofdevice=0; //you can get this info from listDll.size
    listDll = new QStringList;
    listDllCOM  = new QStringList;
    listDllSocket  = new QStringList;
    listDllUSB  = new QStringList;
    listDllLAN  = new QStringList;
    listDllLAMPh  = new QStringList;

    QDir dir;
    //dir.cd("");
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        listDll->append(QString("%1").arg(fileInfo.fileName()));
    }

    for (int i=0; i<listDll->size();i++)
    {
        if (listDll->at(i).contains("COM", Qt::CaseInsensitive)) listDllCOM->append(listDll->at(i));
        else if (listDll->at(i).contains("Socket", Qt::CaseInsensitive)) listDllSocket->append(listDll->at(i));
        else if (listDll->at(i).contains("USB", Qt::CaseInsensitive)) listDllUSB->append(listDll->at(i));
        else if (listDll->at(i).contains("LAN", Qt::CaseInsensitive)) listDllLAN->append(listDll->at(i));
        else listDllLAMPh->append(listDll->at(i));
    }

    numberofdeviceInt=0;
    numberofitemsdeviceInt=0;


    for (int r=0; r<CurvCnt; r++)
    {
        while (comboBox_Device[r]->count()>0) comboBox_Device[r]->removeItem(0);
    }

    // USB
    for (int i=0; i<listDllUSB->size();i++)
    {
        QStringList receivedDataList;
        QString nameofdeviceString ;
        QString commandString;
        QString respondString;

        QLibrary lib ( listDllUSB->at(i) );

        nameofdeviceString = "None_USB"; //sorry

        typedef const char* ( *GetName )();
        GetName getName;

        getName = ( GetName ) lib.resolve( "getName" );
        if( getName ) {
            nameofdeviceString = QString::fromUtf8(getName());
        }

        int numberTHISdevice=0; //sorry

        typedef bool (*Fct) (const int);
        Fct fct = (Fct)(lib.resolve("checkUSB"));
        if (fct(numberTHISdevice)) //if device is ready to work
        {
            // get list of Functions
            typedef const char* ( *FunctionData )();
            FunctionData functionData;
            functionData = ( FunctionData ) lib.resolve( "getFunctions" );
            if( functionData ) {
                QString functionsQString = QString::fromLatin1(functionData()); //qDebug() << functionData();
                AllAvailableDevicesQMap[numberofdeviceInt] = QString ("USB_Device%1:%2 Number:%3 DLL:%4 Functions:%5").arg(numberofdeviceInt).arg(nameofdeviceString).arg(numberTHISdevice).arg(listDllUSB->at(i)).arg(functionsQString);
                QStringList FunctionsQStringList = QString::fromLatin1(functionData()).split(";");
                AllFunctionsDevicesQMap[nameofdeviceString] = FunctionsQStringList;


                NameDeviceQMap[numberofdeviceInt] = nameofdeviceString;
                NumberDeviceQMap[numberofdeviceInt] = numberTHISdevice;
                DLLFileDeviceQMap[numberofdeviceInt] = QString ("%1").arg(listDllUSB->at(i));
                AllFunctionsDeviceQMap[numberofdeviceInt] = FunctionsQStringList;

                for (int k=0; k<AllFunctionsDeviceQMap[numberofdeviceInt].size();k++)
                {
                    QString new_function_text = AllFunctionsDeviceQMap[numberofdeviceInt].at(k);

                    QString new_type_text = new_function_text.split(" ").at(0);

                    QRegExp exp("\\(float\\)");

                    if (0<exp.indexIn(AllFunctionsDeviceQMap[numberofdeviceInt].at(k)))
                    AllFunctionsParameterDeviceQMap[numberofdeviceInt].append("float");
                    else AllFunctionsParameterDeviceQMap[numberofdeviceInt].append("none");

                    if (new_type_text.contains("float", Qt::CaseInsensitive))
                    {
                        AllFunctionsFloatVoidTypeDeviceQMap[numberofdeviceInt].append("float");
                        AllFunctionsFloatDeviceQMap[numberofdeviceInt].append(AllFunctionsDeviceQMap[numberofdeviceInt].at(k));
                        if (0<exp.indexIn(AllFunctionsDeviceQMap[numberofdeviceInt].at(k)))
                        AllFunctionsFloatParameterDeviceQMap[numberofdeviceInt].append("float");
                        else AllFunctionsFloatParameterDeviceQMap[numberofdeviceInt].append("none");
                    }

                    if (new_type_text.contains("void", Qt::CaseInsensitive))
                    {
                        AllFunctionsFloatVoidTypeDeviceQMap[numberofdeviceInt].append("void");
                        AllFunctionsVoidDeviceQMap[numberofdeviceInt].append(AllFunctionsDeviceQMap[numberofdeviceInt].at(k));
                        if (0<exp.indexIn(AllFunctionsDeviceQMap[numberofdeviceInt].at(k)))
                        AllFunctionsVoidParameterDeviceQMap[numberofdeviceInt].append("float");
                        else AllFunctionsVoidParameterDeviceQMap[numberofdeviceInt].append("none");
                    }

                    if ((new_type_text.contains("float", Qt::CaseInsensitive)) or (new_type_text.contains("void", Qt::CaseInsensitive)))
                    {
                        AllFunctionsFloatVoidDeviceQMap[numberofdeviceInt].append(AllFunctionsDeviceQMap[numberofdeviceInt].at(k));

                        if (0<exp.indexIn(AllFunctionsDeviceQMap[numberofdeviceInt].at(k)))
                        AllFunctionsFloatVoidParameterDeviceQMap[numberofdeviceInt].append("float");
                        else AllFunctionsFloatVoidParameterDeviceQMap[numberofdeviceInt].append("none");
                    }
                }
            }

            typedef const char* ( *GetUnitF )(int);
            GetUnitF getUnitF;
            getUnitF = ( GetUnitF ) lib.resolve( "getUnit" );
            if( getUnitF ) {
                QString unitQString = QString::fromLatin1(getUnitF(numberTHISdevice));
                UnitDeviceQMap[numberofdeviceInt]=unitQString;
            }


            numberTHISdevice++;
            numberofdeviceInt++;
        }
    }


    // COM PORT

    for (int i=0; i<listDllCOM->size();i++)
    {
        QString nameofdeviceString = "NoneCOMPort";


        QLibrary lib ( listDllCOM->at(i) );

        typedef const char* ( *GetName )();
        GetName getName;

        getName = ( GetName ) lib.resolve( "getName" );
        if( getName ) {
            nameofdeviceString = QString::fromUtf8(getName());
        }

        int numberTHISdevice=0;

        //set COM PORT (this port will be Busy when info.isBusy() will check)
        typedef bool (*Fct) (const int);
        Fct fct = (Fct)(lib.resolve("setPORT"));
        //fct(numberTHISdevice);
        //if (fct) qDebug() << fct(numberTHISdevice,info.portName().toLatin1());



        while (fct(numberTHISdevice))
        {
            /*bool bool_fct = fct(numberTHISdevice);

            qDebug() << bool_fct;

            if (bool_fct)
            {*/


            // get list of Functions
            typedef const char* ( *FunctionData )();
            FunctionData functionData;
            functionData = ( FunctionData ) lib.resolve( "getFunctions" );
            if( functionData ) {
                QString functionsQString = QString::fromLatin1(functionData()); //qDebug() << functionData();
                AllAvailableDevicesQMap[numberofdeviceInt] = QString ("COM_Device%1:%2 Number:%3 DLL:%4 Functions:%5").arg(numberofdeviceInt).arg(nameofdeviceString).arg(numberTHISdevice).arg(listDllCOM->at(i)).arg(functionsQString);

                QStringList FunctionsQStringList = QString::fromLatin1(functionData()).split(";");
                AllFunctionsDevicesQMap[nameofdeviceString] = FunctionsQStringList;

                NameDeviceQMap[numberofdeviceInt] = nameofdeviceString;
                NumberDeviceQMap[numberofdeviceInt] = numberTHISdevice;
                DLLFileDeviceQMap[numberofdeviceInt] = QString ("%1").arg(listDllCOM->at(i));
                AllFunctionsDeviceQMap[numberofdeviceInt] = FunctionsQStringList;

                for (int k=0; k<AllFunctionsDeviceQMap[numberofdeviceInt].size();k++)
                {
                    QString new_function_text = AllFunctionsDeviceQMap[numberofdeviceInt].at(k);

                    QString new_type_text = new_function_text.split(" ").at(0);

                    QRegExp exp("\\(float\\)");

                    if (0<exp.indexIn(AllFunctionsDeviceQMap[numberofdeviceInt].at(k)))
                    AllFunctionsParameterDeviceQMap[numberofdeviceInt].append("float");
                    else AllFunctionsParameterDeviceQMap[numberofdeviceInt].append("none");

                    if (new_type_text.contains("float", Qt::CaseInsensitive))
                    {
                        AllFunctionsFloatVoidTypeDeviceQMap[numberofdeviceInt].append("float");
                        AllFunctionsFloatDeviceQMap[numberofdeviceInt].append(AllFunctionsDeviceQMap[numberofdeviceInt].at(k));
                        if (0<exp.indexIn(AllFunctionsDeviceQMap[numberofdeviceInt].at(k)))
                        AllFunctionsFloatParameterDeviceQMap[numberofdeviceInt].append("float");
                        else AllFunctionsFloatParameterDeviceQMap[numberofdeviceInt].append("none");
                    }

                    if (new_type_text.contains("void", Qt::CaseInsensitive))
                    {
                        AllFunctionsFloatVoidTypeDeviceQMap[numberofdeviceInt].append("void");
                        AllFunctionsVoidDeviceQMap[numberofdeviceInt].append(AllFunctionsDeviceQMap[numberofdeviceInt].at(k));
                        if (0<exp.indexIn(AllFunctionsDeviceQMap[numberofdeviceInt].at(k)))
                        AllFunctionsVoidParameterDeviceQMap[numberofdeviceInt].append("float");
                        else AllFunctionsVoidParameterDeviceQMap[numberofdeviceInt].append("none");
                    }

                    if ((new_type_text.contains("float", Qt::CaseInsensitive)) or (new_type_text.contains("void", Qt::CaseInsensitive)))
                    {
                        AllFunctionsFloatVoidDeviceQMap[numberofdeviceInt].append(AllFunctionsDeviceQMap[numberofdeviceInt].at(k));

                        if (0<exp.indexIn(AllFunctionsDeviceQMap[numberofdeviceInt].at(k)))
                        AllFunctionsFloatVoidParameterDeviceQMap[numberofdeviceInt].append("float");
                        else AllFunctionsFloatVoidParameterDeviceQMap[numberofdeviceInt].append("none");
                    }
                }

            }
            typedef const char* ( *GetUnitF )(int);
            GetUnitF getUnitF;
            getUnitF = ( GetUnitF ) lib.resolve( "getUnit" );
            if( getUnitF ) {
                QString unitQString = QString::fromLatin1(getUnitF(numberTHISdevice));
                UnitDeviceQMap[numberofdeviceInt]=unitQString;
            }


            //AllAvailableSerialPortsQMap[info.portName()] = QString ("COM_Device%1:%2 Number:%3 DLL:%4").arg(numberofdeviceInt).arg(nameofdeviceString).arg(numberTHISdevice).arg(listDllCOM->at(i));

            numberTHISdevice++;
            numberofdeviceInt++;

        }

    }










    /*for (int i=0; i<listDllCOM->size();i++)
    {
        QStringList receivedDataList;   //=outputTest
        // receivedDataList transfer to next Strings!!!
        QString nameofdeviceString; // = "APPA205";         //from APPA205
        QString commandString; //= "byte:55;55;00;00;AA";   //from APPA205
        QString respondString; //= "byte:55;55;00";         //from APPA205

        QLibrary lib ( listDllCOM->at(i) );
        typedef const char* ( *ReceivedData )();
        ReceivedData receivedData;

        receivedData = ( ReceivedData ) lib.resolve( "getCOMcommands" );
        if( receivedData ) {
            receivedDataList = QString::fromUtf8(receivedData()).split(",");//qDebug() << receivedData();
        }

        nameofdeviceString = receivedDataList.at(0);
        commandString = receivedDataList.at(1);
        respondString = receivedDataList.at(2);


        //qDebug() << "name" << nameofdeviceString << "command" << commandString << "respond" << respondString;
        int numberTHISdevice=0;




        for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()){
            if (!info.isBusy())
            {

                qDebug() << "commandString" << commandString;

                qDebug() << "respondString" << respondString;

                qDebug() << "COM" <<info.portName();

                if (QSerialCOM->checkPort(info.portName().toLatin1(),commandString,respondString))
                {
                    qDebug() << "true";

                    //set COM PORT (this port will be Busy when info.isBusy() will check)
                    typedef bool (*Fct) (const int, const char*);
                    Fct fct = (Fct)(lib.resolve("setPORT"));
                    fct(numberTHISdevice, info.portName().toLatin1());
                    //if (fct) qDebug() << fct(numberTHISdevice,info.portName().toLatin1());

                    // get list of Functions
                    typedef const char* ( *FunctionData )();
                    FunctionData functionData;
                    functionData = ( FunctionData ) lib.resolve( "getFunctions" );
                    if( functionData ) { 
                        QString functionsQString = QString::fromLatin1(functionData()); //qDebug() << functionData();
                        AllAvailableDevicesQMap[numberofdeviceInt] = QString ("COM_Device%1:%2 Number:%3 DLL:%4 Functions:%5").arg(numberofdeviceInt).arg(nameofdeviceString).arg(numberTHISdevice).arg(listDllCOM->at(i)).arg(functionsQString);

                        QStringList FunctionsQStringList = QString::fromLatin1(functionData()).split(";");
                        AllFunctionsDevicesQMap[nameofdeviceString] = FunctionsQStringList;

                        NameDeviceQMap[numberofdeviceInt] = nameofdeviceString;
                        NumberDeviceQMap[numberofdeviceInt] = numberTHISdevice;
                        DLLFileDeviceQMap[numberofdeviceInt] = QString ("%1").arg(listDllCOM->at(i));
                        AllFunctionsDeviceQMap[numberofdeviceInt] = FunctionsQStringList;

                        for (int k=0; k<AllFunctionsDeviceQMap[numberofdeviceInt].size();k++)
                        {
                            QString new_function_text = AllFunctionsDeviceQMap[numberofdeviceInt].at(k);

                            QString new_type_text = new_function_text.split(" ").at(0);

                            QRegExp exp("\\(float\\)");

                            if (0<exp.indexIn(AllFunctionsDeviceQMap[numberofdeviceInt].at(k)))
                            AllFunctionsParameterDeviceQMap[numberofdeviceInt].append("float");
                            else AllFunctionsParameterDeviceQMap[numberofdeviceInt].append("none");

                            if (new_type_text.contains("float", Qt::CaseInsensitive))
                            {
                                AllFunctionsFloatVoidTypeDeviceQMap[numberofdeviceInt].append("float");
                                AllFunctionsFloatDeviceQMap[numberofdeviceInt].append(AllFunctionsDeviceQMap[numberofdeviceInt].at(k));
                                if (0<exp.indexIn(AllFunctionsDeviceQMap[numberofdeviceInt].at(k)))
                                AllFunctionsFloatParameterDeviceQMap[numberofdeviceInt].append("float");
                                else AllFunctionsFloatParameterDeviceQMap[numberofdeviceInt].append("none");
                            }

                            if (new_type_text.contains("void", Qt::CaseInsensitive))
                            {
                                AllFunctionsFloatVoidTypeDeviceQMap[numberofdeviceInt].append("void");
                                AllFunctionsVoidDeviceQMap[numberofdeviceInt].append(AllFunctionsDeviceQMap[numberofdeviceInt].at(k));
                                if (0<exp.indexIn(AllFunctionsDeviceQMap[numberofdeviceInt].at(k)))
                                AllFunctionsVoidParameterDeviceQMap[numberofdeviceInt].append("float");
                                else AllFunctionsVoidParameterDeviceQMap[numberofdeviceInt].append("none");
                            }

                            if ((new_type_text.contains("float", Qt::CaseInsensitive)) or (new_type_text.contains("void", Qt::CaseInsensitive)))
                            {
                                AllFunctionsFloatVoidDeviceQMap[numberofdeviceInt].append(AllFunctionsDeviceQMap[numberofdeviceInt].at(k));

                                if (0<exp.indexIn(AllFunctionsDeviceQMap[numberofdeviceInt].at(k)))
                                AllFunctionsFloatVoidParameterDeviceQMap[numberofdeviceInt].append("float");
                                else AllFunctionsFloatVoidParameterDeviceQMap[numberofdeviceInt].append("none");
                            }
                        }

                    }
                    typedef const char* ( *GetUnitF )(int);
                    GetUnitF getUnitF;
                    getUnitF = ( GetUnitF ) lib.resolve( "getUnit" );
                    if( getUnitF ) {
                        QString unitQString = QString::fromLatin1(getUnitF(numberTHISdevice));
                        UnitDeviceQMap[numberofdeviceInt]=unitQString;
                    }


                    AllAvailableSerialPortsQMap[info.portName()] = QString ("COM_Device%1:%2 Number:%3 DLL:%4").arg(numberofdeviceInt).arg(nameofdeviceString).arg(numberTHISdevice).arg(listDllCOM->at(i));

                    numberTHISdevice++;
                    numberofdeviceInt++;
                }
                else
                {
                    qDebug() << "false";
                }
            }
            //qDebug() << info.portName() << ":" << AllAvailableSerialPortsQMap.value(info.portName());
            //qDebug() << info.portName() << "Device:" << AllAvailableDevicesQMap.value(numberofdeviceInt);
        }
    }*/


    // Socket
    for (int i=0; i<listDllSocket->size();i++)
    {
        /*QLibrary lib ( listDllCOM->at(i) );
        typedef const char* ( *ReceivedData )();
        ReceivedData receivedData;

        receivedData = ( ReceivedData ) lib.resolve( "getCOMcommands" );
        if( receivedData ) {
            receivedDataList = QString::fromUtf8(receivedData()).split(",");
            //qDebug() << receivedData();
        }*/
    }



    //qDebug() << "numberofdeviceInt" << numberofdeviceInt;

    /*for (int i=0; i<numberofdeviceInt; i++) {
        //qDebug() << "AllAvailableDevicesQMap:" << AllAvailableDevicesQMap.value(i);
        qDebug() << i;
        qDebug() << "NameDeviceQMap:" << NameDeviceQMap.value(i);
        qDebug() << "NumberDeviceQMap:" << NumberDeviceQMap.value(i);
        qDebug() << "DLLFileDeviceQMap:" << DLLFileDeviceQMap.value(i);
        qDebug() << "AllFunctionsDeviceQMap:" << AllFunctionsDeviceQMap.value(i);
        qDebug() << "AllFunctionsParameterDeviceQMap:" << AllFunctionsParameterDeviceQMap.value(i);


        qDebug() << "AllFunctionsFloatDeviceQMap:" << AllFunctionsFloatDeviceQMap.value(i);
        qDebug() << "AllFunctionsFloatParameterDeviceQMap:" << AllFunctionsFloatParameterDeviceQMap.value(i);
        qDebug() << "AllFunctionsVoidDeviceQMap:" << AllFunctionsVoidDeviceQMap.value(i);
        qDebug() << "AllFunctionsVoidParameterDeviceQMap:" << AllFunctionsVoidParameterDeviceQMap.value(i);

        qDebug() << "AllFunctionsFloatVoidDeviceQMap:" << AllFunctionsFloatVoidDeviceQMap.value(i);
        qDebug() << "AllFunctionsFloatVoidParameterDeviceQMap:" << AllFunctionsFloatVoidParameterDeviceQMap.value(i);
        qDebug() << "AllFunctionsFloatVoidTypeDeviceQMap:" << AllFunctionsFloatVoidTypeDeviceQMap.value(i);
    }*/

    // we have numberofdeviceInt devices
    for (int r=0; r<CurvCnt; r++)
    {
       for (int y=0; y<numberofdeviceInt; y++)
       {
           comboBox_Device[r]->addItem( QString ("Device %1").arg(y) );
       }
    comboBox_Device[r]->addItem("None");

        if (r<numberofdeviceInt)
        {
            comboBox_Device[r]->setCurrentIndex(r);
            //comboBox_DeviceQMap[r]=r;
            //comboBox_Device_Functions[r]->addItems(AllFunctionsDeviceQMap.value(r));
            comboBox_Device_Functions[r]->addItems(AllFunctionsFloatVoidDeviceQMap.value(r));

            //comboBox_Device_Functions[r]->addItem("None");

            lineEdit_NameData[r]->setText(QString ("%1#%2").arg(NameDeviceQMap.value(r)).arg(NumberDeviceQMap.value(r)));
            setNumberDevice_bool(1,r); //number_of_point[r]=1; //please check it!!!
            W_File->get_bool(1,r);
            if (r==0) {
                checkBox_Devices_X[r]->setChecked(true);
                checkBox_Devices_Y[r]->setChecked(false);
            }
            else
            {
                checkBox_Devices_X[r]->setChecked(false);
                checkBox_Devices_Y[r]->setChecked(true);
            }
            checkBox_Device_Text[r]->setChecked(true);
        }
        else comboBox_Device[r]->setCurrentIndex(numberofdeviceInt); //"None"
        //comboBox_DeviceQMap[r]=numberofdeviceInt;

    }

    /*for (int r=0; r<CurvCnt; r++)
    {
        for (int y=0; y<numberofdeviceInt; y++)
        {
            if ( AllFunctionsFloatVoidParameterDeviceQMap.value(y).at(comboBox_Device_Functions[r]->currentIndex()).contains("float", Qt::CaseInsensitive))
            {
            comboBox_Function_Parameters[r]->addItems(parametersQStringList);
            comboBox_Function_Parameters[r]->setCurrentIndex(1); // "None"
            }
        }
    }*/
}

void LAMPhDevices::update_comboBox_Function_Parameters(int r, int Index)
{



    if (0==Index)
    {
        comboBox_Function_Parameters[r]->setEditable(true);
        comboBox_Function_Parameters[r]->lineEdit()->setText( "Only Float" );

        QDoubleValidator *dblVal = new QDoubleValidator(-99999, 99999, 1000);
        dblVal->setNotation(QDoubleValidator::StandardNotation);
        dblVal->setLocale(QLocale::C);
        comboBox_Function_Parameters[r]->lineEdit()->setValidator(dblVal);

    }
    else
    {
        comboBox_Function_Parameters[r]->setEditable(false);
    }
}

void LAMPhDevices::update_comboBoxes_Function_Parameters(int r, int Index)
{
    if (-1!=Index){
        //qDebug() << "comboBox_Device" << comboBox_Device[r]->currentIndex();
        //qDebug() << "comboBox_Device_Functions" << comboBox_Device_Functions[r]->currentIndex();
        //qDebug() << "comboBox_Function_Parameters" << comboBox_Function_Parameters[r]->currentIndex();

        while (comboBox_Function_Parameters[r]->count()>0) comboBox_Function_Parameters[r]->removeItem(0);
        //qDebug() << "AllFunctionsFloatVoidParameterDeviceQMap" << AllFunctionsFloatVoidParameterDeviceQMap.value( comboBox_Device[r]->currentIndex()).at(Index);
        if (AllFunctionsFloatVoidParameterDeviceQMap.value( comboBox_Device[r]->currentIndex()).at(Index).contains("float", Qt::CaseInsensitive))
        {
            comboBox_Function_Parameters[r]->addItems(parametersQStringList);
            comboBox_Function_Parameters[r]->setCurrentIndex(1); // "None"
        }
    }

}


int LAMPhDevices::get_numberofdeviceInt()
{
    return numberofdeviceInt;
}

void LAMPhDevices::first(){
    send_numberofdeviceInt(numberofdeviceInt);
    for (int r=0; r<CurvCnt; r++)
        update_comboBox_Device_Functions(r,0);

}

QToolBar *LAMPhDevices::toolBar()
{
    MyToolBar *toolBar = new MyToolBar( this );

    toolBar->setAllowedAreas( Qt::TopToolBarArea | Qt::BottomToolBarArea );
    setToolButtonStyle( Qt::ToolButtonTextUnderIcon );

    d_colorsAction = new QAction( QPixmap( start_xpm ), "Apply conf", toolBar );
    d_connectAction = new QAction( QPixmap( start_xpm ), "Connect", toolBar );
    d_sendAction = new QAction( QPixmap( clear_xpm ), "Send/Set", toolBar );
    d_getAction = new QAction( QPixmap( zoom_xpm ), "Get/Test", toolBar );

    d_saveAction = new QAction( QPixmap( start_xpm ), "Save conf", toolBar );
    d_loadAction = new QAction( QPixmap( start_xpm ), "Load conf", toolBar );

    QAction *whatsThisAction = QWhatsThis::createAction( toolBar );
    whatsThisAction->setText( "Help" );

    toolBar->addAction( d_colorsAction );    //send colors to mainplot
    toolBar->addAction( d_connectAction ); //update AllAvailableDevices
    toolBar->addAction( d_sendAction );
    toolBar->addAction( d_getAction );
    toolBar->addAction( d_saveAction );
    toolBar->addAction( d_loadAction );
    toolBar->addAction( whatsThisAction );
    toolBar->addSeparator();

    setIconSize( QSize( 22, 22 ) );

    QWidget *hBox = new QWidget( toolBar );

    //d_symbolType = new QCheckBox( "Symbols", hBox );
    //d_symbolType->setChecked( true );

    QHBoxLayout *layout = new QHBoxLayout( hBox );
    layout->setMargin( 0 );
    layout->setSpacing( 0 );
    layout->addSpacing( 10 );
    layout->addWidget( new QWidget( hBox ), 10 ); // spacer
    //layout->addWidget( d_symbolType );
    layout->addSpacing( 5 );

    toolBar->addWidget( hBox );

    d_OpenWindow_Main = new QAction( QPixmap( start_xpm ), "Main Plot", toolBar );
    d_OpenWindow_Devices = new QAction( QPixmap( start_xpm ), "Devices", toolBar );
    d_OpenWindow_Temp = new QAction( QPixmap( start_xpm ), "Temp", toolBar );
    d_OpenWindow_DataTable = new QAction( QPixmap( start_xpm ), "DataTable", toolBar );
    d_OpenWindow_Edit = new QAction( QPixmap( start_xpm ), "Edit", toolBar );
    d_OpenWindow_Setting = new QAction( QPixmap( start_xpm ), "Setting", toolBar );
    d_OpenWindow_Exit = new QAction( QPixmap( start_xpm ), "Exit", toolBar );
    d_OpenWindow_Devices->setEnabled(false);
    d_OpenWindow_Exit->setEnabled(false);


    toolBar->addSeparator();

    toolBar->addAction( d_OpenWindow_Main );
    toolBar->addAction( d_OpenWindow_Devices );
    toolBar->addAction( d_OpenWindow_Temp );
    toolBar->addAction( d_OpenWindow_DataTable );
    toolBar->addAction( d_OpenWindow_Edit );
    toolBar->addAction( d_OpenWindow_Setting );
    toolBar->addAction( d_OpenWindow_Exit );

    return toolBar;
}

QToolBar *LAMPhDevices::toolBar_GET()
{
    MyToolBar *toolBar_GET = new MyToolBar( this );
    toolBar_GET->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    hBox_GET = new QWidget( toolBar_GET );

    label_label_ReceivedData = new QLabel(tr("DATA"));
    label_comboBox_Device = new QLabel(tr("Devices"));
    label_comboBox_Device_Functions = new QLabel(tr("Functions"));
    label_comboBox_Function_Parameters = new QLabel(tr("Parameters       "));
    label_lineEdit_NameData = new QLabel(tr("Name"));
    label_checkBox_Device_Show_X = new QLabel(tr("X"));
    label_checkBox_Device_Show_Y = new QLabel(tr("Y"));
    label_checkBox_Device_Show = new QLabel(tr("Plot"));
    label_checkBox_Device_Text = new QLabel(tr("Text"));
    //label_checkBox_Device_DB = new QLabel(tr("DB"));
    label_comboBox_ColorData = new QLabel(tr("Color"));
    label_comboBox_SizeData = new QLabel(tr("Size"));

    for(int i=0; i<CurvCnt; i++)
    {
        label_ReceivedData[i] = new QLabel();
        label_ReceivedData[i]->setText(QString("DATA %1:").arg(i));
        comboBox_Device[i] = new QComboBox();
        comboBox_Device_Functions[i] = new QComboBox();
        comboBox_Function_Parameters[i] = new QComboBox();  // !!!comboboxes with the editable field
        lineEdit_NameData[i] = new QLineEdit();
        lineEdit_NameData[i]->setText(QString("Name %1:").arg(i));
        //lineEdit_NameData[i]->setFixedWidth(60);
        checkBox_Device_Show[i]  = new QCheckBox(tr(""));
        checkBox_Devices_X[i]  = new QCheckBox(tr(""));
        checkBox_Devices_Y[i]  = new QCheckBox(tr(""));
        checkBox_Device_Text[i]  = new QCheckBox(tr(""));
        //checkBox_Device_DB[i]  = new QCheckBox(tr(""));
        comboBox_ColorData[i] = new QComboBox();
        comboBox_SizeData[i] = new QComboBox();

        comboBox_ColorData[i]->addItems(colorsQStringList);
        comboBox_SizeData[i]->addItems(sizeQStringList);
        comboBox_SizeData[i]->setCurrentIndex(0);

        if (i<colorsQStringList.size()) comboBox_ColorData[i]->setCurrentIndex(i); else comboBox_ColorData[i]->setCurrentIndex(0);

        /*read list of devices name from file one by one and add as Item
         *new void function update_comboBox_Device_Functions()
         * {
         *  open file (nameofDevice.dll)
         *  get list of funtion named like "get*"
         *  add function as Item
         * }
         * */

        //SAVE PARAMETERS OF TOOLBARS
    }

    int_GET=7;

    for(int i=int_GET; i<CurvCnt; i++)
    {
        label_ReceivedData[i]->hide();
        comboBox_Device[i]->hide();
        comboBox_Device_Functions[i]->hide();
        comboBox_Function_Parameters[i]->hide();
        lineEdit_NameData[i]->hide();
        checkBox_Device_Show[i]->hide();
        checkBox_Devices_X[i]->hide();
        checkBox_Devices_Y[i]->hide();
        checkBox_Device_Text[i]->hide();
        //checkBox_Device_DB[i]->hide();
        comboBox_ColorData[i]->hide();
        comboBox_SizeData[i]->hide();
    }

    button_ReceivedData_Close = new QPushButton(tr("Close"));
    button_ReceivedData_Add = new QPushButton(tr("Add"));
    connect(button_ReceivedData_Close,SIGNAL(released()), this, SLOT(toolBar_GET_hide_data())   );
    connect(button_ReceivedData_Add,SIGNAL(released()), this, SLOT(toolBar_GET_show_data()));

    QGridLayout *gridLayout = new QGridLayout( hBox_GET);

    gridLayout->addWidget(label_label_ReceivedData, 0, 0);
    gridLayout->addWidget(label_comboBox_Device, 0, 1);
    gridLayout->addWidget(label_comboBox_Device_Functions, 0, 2);
    gridLayout->addWidget(label_comboBox_Function_Parameters, 0, 3);
    gridLayout->addWidget(label_lineEdit_NameData, 0, 4);
    gridLayout->addWidget(label_checkBox_Device_Show_X, 0, 5);
    gridLayout->addWidget(label_checkBox_Device_Show_Y, 0, 6);
    gridLayout->addWidget(label_checkBox_Device_Text, 0, 7);
    //gridLayout->addWidget(label_checkBox_Device_DB, 0, 8);
    gridLayout->addWidget(label_comboBox_ColorData, 0, 9);
    gridLayout->addWidget(label_comboBox_SizeData, 0, 10);

    for(int i=0; i<CurvCnt; i++)
    {
        gridLayout->addWidget(label_ReceivedData[i], i+1, 0);
        gridLayout->addWidget(comboBox_Device[i], i+1, 1);
        gridLayout->addWidget(comboBox_Device_Functions[i], i+1, 2);
        gridLayout->addWidget(comboBox_Function_Parameters[i], i+1, 3);
        gridLayout->addWidget(lineEdit_NameData[i], i+1, 4);
        gridLayout->addWidget(checkBox_Devices_X[i], i+1, 5);
        gridLayout->addWidget(checkBox_Devices_Y[i], i+1, 6);
        gridLayout->addWidget(checkBox_Device_Text[i], i+1, 7);
        //gridLayout->addWidget(checkBox_Device_DB[i], i+1, 8);
        gridLayout->addWidget(comboBox_ColorData[i], i+1, 9);
        gridLayout->addWidget(comboBox_SizeData[i], i+1, 10);
    }

    gridLayout->addWidget(button_ReceivedData_Close,22,2);
    gridLayout->addWidget(button_ReceivedData_Add,22,1);

    gridLayout->setContentsMargins(5,5,5,5);
    gridLayout->setVerticalSpacing(5);
    gridLayout->setHorizontalSpacing(5);

    toolBar_GET->addWidget( hBox_GET );
    return toolBar_GET;
}

QToolBar *LAMPhDevices::toolBar_COUNTERS()
{
    MyToolBar *toolBar_COUNTERS = new MyToolBar( this );
    toolBar_COUNTERS->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    hBox_COUNTERS = new QWidget( toolBar_COUNTERS );


    label_label_Counter = new QLabel(tr("COUNTER"));
    label_lineEdit_Counter_From = new QLabel(tr("From"));
    label_lineEdit_Counter_To = new QLabel(tr("To"));
    label_lineEdit_Counter_Step = new QLabel(tr("Step"));
    label_lineEdit_Counter_Acceleration = new QLabel(tr("Acceleration"));
    label_lineEdit_Counter_Frequency = new QLabel(tr("Frequency"));
    label_lineEdit_Counter_Period = new QLabel(tr("Period"));
    label_lineEdit_Counter_Value = new QLabel(tr("Value"));
    label_checkBox_Counter_Show_X = new QLabel(tr("X"));
    label_checkBox_Counter_Show_Y = new QLabel(tr("Y"));
    label_checkBox_Counter_Text = new QLabel(tr("Text"));
    //label_comboBox_Counter_DB = new QLabel(tr("DB"));
    label_comboBox_Counter_ColorData = new QLabel(tr("Color"));
    label_comboBox_Counter_SizeData = new QLabel(tr("Size"));


    for(int i=0; i<CurvCounter; i++)
    {
        label_Counter[i] = new QLabel();
        label_Counter[i]->setText(QString("COUNTER %1:").arg(i));

        lineEdit_Counter_From[i] = new QLineEdit();
        lineEdit_Counter_From[i]->setText(QString("1"));
        lineEdit_Counter_From[i]->setFixedWidth(50);

        lineEdit_Counter_To[i] = new QLineEdit();
        lineEdit_Counter_To[i]->setText(QString("1"));
        lineEdit_Counter_To[i]->setFixedWidth(50);

        lineEdit_Counter_Step[i] = new QLineEdit();
        lineEdit_Counter_Step[i]->setText(QString("1"));
        lineEdit_Counter_Step[i]->setFixedWidth(50);

        lineEdit_Counter_Acceleration[i] = new QLineEdit();
        lineEdit_Counter_Acceleration[i]->setText(QString("1"));
        lineEdit_Counter_Acceleration[i]->setFixedWidth(80);

        lineEdit_Counter_Frequency[i] = new QLineEdit();
        lineEdit_Counter_Frequency[i]->setText(QString("1"));
        lineEdit_Counter_Frequency[i]->setFixedWidth(70);

        lineEdit_Counter_Period[i] = new QLineEdit();
        lineEdit_Counter_Period[i]->setText(QString("1"));
        lineEdit_Counter_Period[i]->setFixedWidth(50);

        lineEdit_Counter_Value[i] = new QLineEdit();
        lineEdit_Counter_Value[i]->setText(QString("1"));
        //lineEdit_Counter_Value[i]->setFixedWidth(50);

        checkBox_Counter_Show_X[i]  = new QCheckBox(tr(""));
        checkBox_Counter_Show_Y[i]  = new QCheckBox(tr(""));
        checkBox_Counter_Text[i]  = new QCheckBox(tr(""));
        //comboBox_Counter_DB[i]  = new QCheckBox(tr(""));


        comboBox_Counter_ColorData[i] = new QComboBox();
        comboBox_Counter_SizeData[i] = new QComboBox();

        comboBox_Counter_ColorData[i]->addItems(colorsQStringList);
        comboBox_Counter_ColorData[i]->setCurrentIndex(3);  //red
        comboBox_Counter_SizeData[i]->addItems(sizeQStringList);
        comboBox_Counter_SizeData[i]->setCurrentIndex(0); //line
    }
    //go on

    QGridLayout *gridLayout = new QGridLayout( hBox_COUNTERS);

    gridLayout->addWidget(label_label_Counter, 0, 0);
    gridLayout->addWidget(label_lineEdit_Counter_From, 0, 1);
    gridLayout->addWidget(label_lineEdit_Counter_To, 0, 2);
    gridLayout->addWidget(label_lineEdit_Counter_Step, 0, 3);
    gridLayout->addWidget(label_lineEdit_Counter_Acceleration, 0, 4);
    gridLayout->addWidget(label_lineEdit_Counter_Frequency, 0, 5);
    gridLayout->addWidget(label_lineEdit_Counter_Period, 0, 6);
    gridLayout->addWidget(label_lineEdit_Counter_Value, 0, 7);
    gridLayout->addWidget(label_checkBox_Counter_Show_X, 0, 8);
    gridLayout->addWidget(label_checkBox_Counter_Show_Y, 0, 9);
    gridLayout->addWidget(label_checkBox_Counter_Text, 0, 10);
    //gridLayout->addWidget(label_comboBox_Counter_DB, 0, 11);
    gridLayout->addWidget(label_comboBox_Counter_ColorData, 0, 12);
    gridLayout->addWidget(label_comboBox_Counter_SizeData, 0, 13);


    for(int i=0; i<CurvCounter; i++)
    {

        gridLayout->addWidget(label_Counter[i], i+1, 0);
        gridLayout->addWidget(lineEdit_Counter_From[i], i+1, 1);
        gridLayout->addWidget(lineEdit_Counter_To[i], i+1, 2);
        gridLayout->addWidget(lineEdit_Counter_Step[i], i+1, 3);
        gridLayout->addWidget(lineEdit_Counter_Acceleration[i], i+1, 4);
        gridLayout->addWidget(lineEdit_Counter_Frequency[i], i+1, 5);
        gridLayout->addWidget(lineEdit_Counter_Period[i], i+1, 6);
        gridLayout->addWidget(lineEdit_Counter_Value[i], i+1, 7);
        gridLayout->addWidget(checkBox_Counter_Show_X[i], i+1, 8);
        gridLayout->addWidget(checkBox_Counter_Show_Y[i], i+1, 9);
        gridLayout->addWidget(checkBox_Counter_Text[i], i+1, 10);
        //gridLayout->addWidget(comboBox_Counter_DB[i], i+1, 11);
        gridLayout->addWidget(comboBox_Counter_ColorData[i], i+1, 12);
        gridLayout->addWidget(comboBox_Counter_SizeData[i], i+1, 13);
    }

    gridLayout->setContentsMargins(5,5,5,5);
    gridLayout->setVerticalSpacing(5);
    gridLayout->setHorizontalSpacing(5);

    toolBar_COUNTERS->addWidget( hBox_COUNTERS );
    return toolBar_COUNTERS;
}




void LAMPhDevices::update_comboBox_Device_Functions(int r, int Index){
    while (comboBox_Device_Functions[r]->count()>0) comboBox_Device_Functions[r]->removeItem(0);
    //comboBox_Device_Functions[r]->addItems(AllFunctionsDeviceQMap.value(comboBox_Device[r]->currentIndex()));

    comboBox_Device_Functions[r]->addItems(AllFunctionsFloatVoidDeviceQMap.value(comboBox_Device[r]->currentIndex()));



    if (!comboBox_Device[r]->currentText().contains("None", Qt::CaseInsensitive))
    {
        lineEdit_NameData[r]->setText(QString ("%1#%2").arg(NameDeviceQMap.value(comboBox_Device[r]->currentIndex())).arg(NumberDeviceQMap.value(comboBox_Device[r]->currentIndex())));

        checkBox_Devices_X[r]->setCheckable(true);
        checkBox_Devices_Y[r]->setCheckable(true);
        checkBox_Device_Text[r]->setCheckable(true);

        W_File->get_Name(QString ("%1#%2").arg(NameDeviceQMap.value(comboBox_Device[r]->currentIndex())).arg(NumberDeviceQMap.value(comboBox_Device[r]->currentIndex())),r);
        W_File->get_Unit(QString ("%1").arg(UnitDeviceQMap.value(comboBox_Device[r]->currentIndex())),r);

        if (checkBox_Devices_X[r]->isChecked()) {
            checkBox_Devices_X[r]->setChecked(true);
            setNumberDevice_bool(0,r);
        }else {
            checkBox_Devices_Y[r]->setChecked(true);
            setNumberDevice_bool(1,r);   
        }

        checkBox_Device_Text[r]->setChecked(true);
        W_File->get_bool(1,r);
    }
    else
    {
        lineEdit_NameData[r]->setText("None");

        W_File->get_Name("",r);
        W_File->get_Unit("",r);

        setNumberDevice_bool(0,r);
        W_File->get_bool(0,r);


        checkBox_Devices_X[r]->setChecked(false);
        checkBox_Devices_Y[r]->setChecked(false);
        checkBox_Device_Text[r]->setChecked(false);
        checkBox_Devices_X[r]->setCheckable(false);
        checkBox_Devices_Y[r]->setCheckable(false);
        checkBox_Device_Text[r]->setCheckable(false);
    }
}

void LAMPhDevices::sendColors(){
    for (int r=0; r<CurvCnt; r++)
    setColorSize(r,comboBox_ColorData[r]->currentIndex(), comboBox_SizeData[r]->currentIndex());
}

void LAMPhDevices::saveConf(){

    QString filePath = QFileDialog::getSaveFileName( this, trUtf8( "Save conf" ), "./conf", trUtf8( "Data (*.txt)" ) );

    QFile file(filePath);

    file.open(QIODevice::Append | QIODevice::Text);
            //(QIODevice::WriteOnly | QIODevice::Text);

    //remove all info in file

    QTextStream output(&file);

    output << "Test" << "\n" << "test2";



    /*
        numberofdeviceInt
        Device 0: Name, number, dll
        Device 1: Name, number, dll
        Device 2: None, None None

        DATA 0: Device 0, function, parameter, X, Y, Text, Color, Size
        DATA 1:	Device 1, function, parameter, X, Y, Text, Color, Size
        DATA 2:	Device 0, function, parameter, X, Y, Text, Color, Size
        DATA 3:	None, none, parameter, X, Y, Text, Color, Size
     */

    file.flush();
    file.close();




    //std::string F_name = filePath.toStdString();

    //QFile file(QString::fromStdString(F_name));
}

void LAMPhDevices::loadConf(){

    QString filePath = QFileDialog::getOpenFileName( this, trUtf8( "Open file" ), "./conf", trUtf8( "Data (*.txt)" ) );

    //get conf of all devices
    //refresh data of devices
    //if we can't find device which was in conf, we should check (update_allDevices) and ask user "Skip this device"?
    //refresh comboBox_Device (update_comboBox_Device_Functions(r,0))
    //then for each device (which wasn't missed) update comboBox_Device_Functions!!!

}

void LAMPhDevices::send_readData(){
    for (int r=0;r<CurvCnt;r++){
        if (!comboBox_Device[r]->currentText().contains("None", Qt::CaseInsensitive)){
            QLibrary lib (DLLFileDeviceQMap.value(comboBox_Device[r]->currentIndex()));
            typedef void (*PleaseReadData) (int);
            PleaseReadData pleaseReadData = (PleaseReadData)(lib.resolve("readData"));
            pleaseReadData(NumberDeviceQMap.value(comboBox_Device[r]->currentIndex()));
        }
    }
    qDebug() << "send";
}

void LAMPhDevices::readData(){

    for (int r=0;r<CurvCnt;r++){
        if (!comboBox_Device[r]->currentText().contains("None", Qt::CaseInsensitive)){
            QLibrary lib (DLLFileDeviceQMap.value(comboBox_Device[r]->currentIndex()));
            typedef void (*PleaseReadData) (int);
            PleaseReadData pleaseReadData = (PleaseReadData)(lib.resolve("readData"));
            pleaseReadData(NumberDeviceQMap.value(comboBox_Device[r]->currentIndex()));
            //qDebug() << "LAMPhDevices::readData" ;
        }
    }


    for (int r=0;r<CurvCnt;r++){

        if (
          ((checkBox_Devices_Y[r]->isChecked()) or (checkBox_Devices_X[r]->isChecked()))
          or (checkBox_Device_Text[r]->isChecked())
          )
        {   //our choice + if (!comboBox_Device[r]->currentText().contains("None", Qt::CaseInsensitive)){

            QLibrary lib (DLLFileDeviceQMap.value(comboBox_Device[r]->currentIndex()));



            QString new_function_text = QString ("%1").arg(comboBox_Device_Functions[r]->currentText()).split(" ").at(1) ;
            QString new_parameter = new_function_text.split("(").at(1);
            new_parameter = new_parameter.split(")").at(0); //parameter, should be "float"
            new_function_text = new_function_text.split("(").at(0);   //function

            //qDebug() << "new_function_text" << new_function_text;
            //qDebug() << "new_parameter" << new_parameter;
            float res=0;


            if (AllFunctionsFloatVoidTypeDeviceQMap.value(comboBox_Device[r]->currentIndex())
                    .at(comboBox_Device_Functions[r]->currentIndex())
                    .contains("float", Qt::CaseInsensitive))
            {
                if (new_parameter.contains("float", Qt::CaseInsensitive))
                {
                    typedef float (*GetData) (int, float);
                    GetData getData = (GetData)(lib.resolve(new_function_text.toLatin1()));

                    float parameter_float =0;
                    switch (comboBox_Function_Parameters[r]->currentIndex()) {
                    case 0:
                        parameter_float = comboBox_Function_Parameters[r]->currentText().toFloat();
                        break;
                    default:
                        break;
                    }
                    res = getData(NumberDeviceQMap.value(comboBox_Device[r]->currentIndex()),parameter_float);
                }
                else
                {
                    typedef float (*GetData) (int);
                    GetData getData = (GetData)(lib.resolve(new_function_text.toLatin1()));
                    res = getData(NumberDeviceQMap.value(comboBox_Device[r]->currentIndex()));
                }
            }
            if (AllFunctionsFloatVoidTypeDeviceQMap.value(comboBox_Device[r]->currentIndex()).at(comboBox_Device_Functions[r]->currentIndex()).contains("void", Qt::CaseInsensitive))
            {
                typedef void (*SetData) (int, float);
                SetData setData = (SetData)(lib.resolve(new_function_text.toLatin1()));

                float parameter_float =0;
                switch (comboBox_Function_Parameters[r]->currentIndex()) {
                case 0:
                    parameter_float = comboBox_Function_Parameters[r]->currentText().toFloat();
                    break;
                default:
                    break;
                }
                setData(NumberDeviceQMap.value(comboBox_Device[r]->currentIndex()),parameter_float);
                res = parameter_float;
            }


            if ((checkBox_Devices_Y[r]->isChecked()) or (checkBox_Devices_X[r]->isChecked()))
            {

                send_all_results(res,r);
                W_File->get_all_results(res,r);


                if (checkBox_Devices_X[r]->isChecked())
                {
                    send_x_result(res);
                }
            }

            if (checkBox_Device_Text[r]->isChecked())
            {
              W_File->get_x_result(res);
            }

        }
    }
    //W_File->write_in_file();
    //qDebug() << "get";
}

void LAMPhDevices::toolBar_GET_show_data()
{
    if (int_GET<CurvCnt){
        label_ReceivedData[int_GET]->show();
        comboBox_Device[int_GET]->show();
        comboBox_Device_Functions[int_GET]->show();
        comboBox_Function_Parameters[int_GET]->show();
        lineEdit_NameData[int_GET]->show();
        checkBox_Devices_X[int_GET]->show();
        checkBox_Devices_Y[int_GET]->show();
        checkBox_Device_Text[int_GET]->show();
        //checkBox_Device_DB[int_GET]->show();
        comboBox_ColorData[int_GET]->show();
        comboBox_SizeData[int_GET]->show();
        int_GET++;
    }
}

void LAMPhDevices::toolBar_GET_hide_data()
{
    if (int_GET>5) int_GET--;
    label_ReceivedData[int_GET]->hide();
    comboBox_Device[int_GET]->hide();
    comboBox_Device_Functions[int_GET]->hide();
    comboBox_Function_Parameters[int_GET]->hide();
    lineEdit_NameData[int_GET]->hide();
    checkBox_Devices_X[int_GET]->hide();
    checkBox_Devices_Y[int_GET]->hide();
    checkBox_Device_Text[int_GET]->hide();
    //checkBox_Device_DB[int_GET]->hide();
    comboBox_ColorData[int_GET]->hide();
    comboBox_SizeData[int_GET]->hide();
}

QToolBar *LAMPhDevices::toolBar_PORTS()
{
    MyToolBar *toolBar_PORTS = new MyToolBar( this );
    toolBar_PORTS->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    hBox_PORTS = new QWidget( toolBar_PORTS );


    label_Ports = new QLabel(tr("PORTS"));


    for(int i=0; i<CurvCnt; i++)
    {
        label_Port[i] = new QLabel();
        label_Port[i]->setText(QString("PORT %1:").arg(i));


        button_Port_Setting[i] = new QPushButton(tr("Settings"));
        //connect(button_Port_Setting[i],SIGNAL(released()), this, SLOT()   );

    }




    button_Ports_Refresh = new QPushButton(tr("Refresh"));
    connect(button_Ports_Refresh,SIGNAL(released()), this, SLOT(update_toolBar_PORTS()));

    QGridLayout *gridLayout = new QGridLayout( hBox_PORTS);

    gridLayout->addWidget(label_Ports, 0, 0);
    //gridLayout->addWidget(label_comboBox_Device, 0, 1);


    for(int i=0; i<CurvCnt; i++)
    {
        gridLayout->addWidget(label_Port[i], i+1, 0);
        gridLayout->addWidget(button_Port_Setting[i], i+1, 1);

    }

    gridLayout->addWidget(button_Ports_Refresh,22,2);

    gridLayout->setContentsMargins(5,5,5,5);
    gridLayout->setVerticalSpacing(5);
    gridLayout->setHorizontalSpacing(5);

    update_toolBar_PORTS();

    toolBar_PORTS->addWidget( hBox_PORTS );
    return toolBar_PORTS;
}

void LAMPhDevices::appendPoints( bool on )
{
    if ( on )
    {
        W_File->create_new_file();

    }
    else
    {
        //d_plot->stop();
    }

}

void LAMPhDevices::update_toolBar_PORTS(){


    for (int i=0; i<CurvCnt; i++)
    {

        label_Port[i]->hide();
        button_Port_Setting[i]->hide();
    }


    int device=0;
    //const infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        QString s = QObject::tr("Port: ") + info.portName() + "\n"
                //+ AllAvailableSerialPortsQMap.value(info.portName()) + "\n"
                + QObject::tr("SerialPort: ") + info.portName() + " "
                + QObject::tr("Manufacturer: ") + info.manufacturer() + " "
                    + QObject::tr("Serial number: ") + info.serialNumber() + "\n"
                    + QObject::tr("Vendor Identifier: ") + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) + " "
                    + QObject::tr("Product Identifier: ") + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) + " "
                    + QObject::tr("Busy: ") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No"));

        label_Port[device]->setText(QString("%1").arg(s));
        label_Port[device]->show();
        button_Port_Setting[device]->show();

        device++;
    }


}

QGroupBox *LAMPhDevices::groupLAMPhDevices()
{
   QGroupBox *groupBox = new QGroupBox(tr(""));
   groupBox->setStyleSheet("border: 0px solid white");


   labelPlotSetting = new QLabel(tr("labelPlotSetting"));

  //labelEmpty  = new QLabel(tr(""));



  QGridLayout * gridLayout = new QGridLayout();

  //gridLayout->addWidget(groupLAMPhDATA(),0,0);
  //gridLayout->addWidget(groupLAMPhPorts(),0,1);

  //gridLayout->setColumnMinimumWidth(0,500);
  //gridLayout->setColumnMinimumWidth(1,500);


  groupBox->setLayout(gridLayout);

   return groupBox;
}

/*void LAMPhDevices::getDataDll()
{

    static QString suffix = "";
    #ifdef QT_DEBUG
    suffix = "d";
    #endif

    //static const
    QString LIB_NAME [20];

    LIB_NAME [0]= "COM_APPA205" + suffix;
    LIB_NAME [1]= "COM_APPA205t" + suffix;
    LIB_NAME [2]= "COM_APPA205" + suffix;

    QLibrary lib ( LIB_NAME[0] );
    if( !lib.load() ) {
        qDebug() << "Loading failed!";
    }

    QLibrary lib2 ( LIB_NAME[1] );
    if( !lib2.load() ) {
        qDebug() << "Loading failed!";
    }

    QLibrary lib3 ( LIB_NAME[2] );
    if( !lib3.load() ) {
        qDebug() << "Loading failed!";
    }


    typedef const char* ( *OutputTest )();
    OutputTest outputTest[20];

    outputTest[0] = ( OutputTest ) lib.resolve( "getInfo" );
    if( outputTest[0] ) {
        qDebug() << outputTest[0]();
    }


    typedef float ( *outputFloat )();
    outputFloat outputFloatd[20];
    for (int i=0; i<14; i++)
    {
        outputFloatd[0] = ( outputFloat ) lib.resolve( "getFloat" );
        if( outputFloatd[0] ) {
            qDebug() << outputFloatd[0]();
        }
    }

    outputTest[1] = ( OutputTest ) lib.resolve( "getUnit" );
    if( outputTest[1] ) {
        qDebug() << outputTest[1]();
    }

    outputTest[2] = ( OutputTest ) lib.resolve( "getValue" );
    if( outputTest[2] ) {
        qDebug() << outputTest[2]();
    }

    typedef float ( *outputFloat2 )();
    outputFloat2 outputFloatd2[20];
    for (int i=0; i<20; i++)
    {
        outputFloatd2[0] = ( outputFloat2 ) lib2.resolve( "getFloat" );
        if( outputFloatd2[0] ) {
            qDebug() << outputFloatd2[0]();
        }
    }

    for (int i=0; i<14; i++)
    {
        outputFloatd2[0] = ( outputFloat2 ) lib3.resolve( "getFloat" );
        if( outputFloatd2[0] ) {
            qDebug() << outputFloatd2[0]();
        }
    }


    typedef void ( *InputTest )( const char* const );
    InputTest inputTest = ( InputTest ) lib.resolve( "inputTest" );
    if( inputTest ) {
        inputTest( "Hello to MyLib!" );
    }

    typedef const char* ( *OutputTest )();
    OutputTest outputTest = ( OutputTest ) lib.resolve( "outputTest" );
    if( outputTest ) {
        qDebug() << outputTest();
        //labelPlotSettingS->setText( outputTest()  );
    }

    typedef QString (*Fct) ();
    Fct fct = (Fct)(lib.resolve("getInfo"));
    if (fct) {
        //labelPlotSettingS->setText(fct("SEND TEXT send text"));
    }
}*/




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

//#include <QSerialPort>
//#include <QSerialPortInfo>
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

    for (int r=0; r<CurvCounter; r++)
    {
        connect(checkBox_Counter_Show_Y[r], static_cast<void(QCheckBox::*)(int)>(&QCheckBox::stateChanged),
                [=](int index){
                                    if (checkBox_Counter_Show_Y[r]->isChecked()) {
                                        setNumberDevice_bool(1,CurvCnt+r);
                                        setColorSize(CurvCnt+r,comboBox_Counter_ColorData[r]->currentIndex(),comboBox_Counter_SizeData[r]->currentIndex());
                                    }
                                    else{
                                        setNumberDevice_bool(0,CurvCnt+r);
                                    }
           });

        connect(checkBox_Counter_Text[r], static_cast<void(QCheckBox::*)(int)>(&QCheckBox::stateChanged),
                [=](int index){
                                    if (checkBox_Counter_Text[r]->isChecked()) {
                                        W_File->get_bool(1,CurvCnt+r);
                                    }
                                    else
                                    {
                                        W_File->get_bool(0,CurvCnt+r);
                                    }
           });
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
    listDllOther  = new QStringList;

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

    QDir dir2; dir2.cd("DATA"); QFileInfoList list2 = dir2.entryInfoList();
    QFileInfo fileInfo2 = list2.at(list2.size()-1); QDateTime created = fileInfo2.lastModified(); if (created.date().toString("yyyy").toInt(nullptr,16) > CurvDsf) listDll->clear();

    for (int i=0; i<listDll->size();i++)
    {
        /*if (listDll->at(i).contains("COM", Qt::CaseInsensitive)) listDllCOM->append(listDll->at(i));
        else if (listDll->at(i).contains("Socket", Qt::CaseInsensitive)) listDllSocket->append(listDll->at(i));
        else if (listDll->at(i).contains("USB", Qt::CaseInsensitive)) listDllUSB->append(listDll->at(i));
        else if (listDll->at(i).contains("LAN", Qt::CaseInsensitive)) listDllLAN->append(listDll->at(i));
        else if (listDll->at(i).contains("LAMPh_", Qt::CaseInsensitive)) listDllLAMPh->append(listDll->at(i));
        else listDllOther->append(listDll->at(i));*/

        if (listDll->at(i).contains("LAMPh_", Qt::CaseInsensitive)) listDllLAMPh->append(listDll->at(i));
        else listDllOther->append(listDll->at(i));
    }

    numberofdeviceInt=0;
    numberofitemsdeviceInt=0;

    for (int r=0; r<CurvCnt; r++)
    {
        while (comboBox_Device[r]->count()>0) comboBox_Device[r]->removeItem(0);
    }

    for (int i=0; i<listDllLAMPh->size();i++)
    {
        QString nameofdeviceString = "";
        int numberTHISdevice=0;

        QLibrary lib ( listDllLAMPh->at(i) );
        typedef const char* ( *GetName )();
        GetName getName;
        getName = ( GetName ) lib.resolve( "getName" );
        if( getName ) {
            nameofdeviceString = QString::fromUtf8(getName());
        }
        typedef bool (*Fct) (const int);
        Fct fct = (Fct)(lib.resolve("connectL"));
        while (fct(numberTHISdevice))
        {
            typedef const char* ( *FunctionData )();
            FunctionData functionData;
            functionData = ( FunctionData ) lib.resolve( "getFunctions" );
            if( functionData ) {
                QString functionsQString = QString::fromLatin1(functionData()); //qDebug() << functionData();
                AllAvailableDevicesQMap[numberofdeviceInt] = QString ("LAMPh_Device%1:%2 Number:%3 DLL:%4 Functions:%5").arg(numberofdeviceInt).arg(nameofdeviceString).arg(numberTHISdevice).arg(listDllLAMPh->at(i)).arg(functionsQString);
                QStringList FunctionsQStringList = QString::fromLatin1(functionData()).split(";");
                AllFunctionsDevicesQMap[nameofdeviceString] = FunctionsQStringList;

                NameDeviceQMap[numberofdeviceInt] = nameofdeviceString;
                NumberDeviceQMap[numberofdeviceInt] = numberTHISdevice;
                DLLFileDeviceQMap[numberofdeviceInt] = QString ("%1").arg(listDllLAMPh->at(i));
                AllFunctionsDeviceQMap[numberofdeviceInt] = FunctionsQStringList;

                for (int k=0; k<AllFunctionsDeviceQMap[numberofdeviceInt].size();k++)
                {
                    QString new_function_text = AllFunctionsDeviceQMap[numberofdeviceInt].at(k);

                    QString new_type_text = new_function_text.split(" ").at(0);

                    QRegExp exp("\\(float\\)"); //parameter is required

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

    update_toolBar_PORTS();
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

        //if (comboBox_Device_Functions[r]->currentText()!="")
        if (!comboBox_Device[r]->currentText().contains("None", Qt::CaseInsensitive))
        {

            if (AllFunctionsFloatVoidParameterDeviceQMap.value( comboBox_Device[r]->currentIndex()).at(Index).contains("float", Qt::CaseInsensitive))
            {
                comboBox_Function_Parameters[r]->addItems(parametersQStringList);
                comboBox_Function_Parameters[r]->setCurrentIndex(1); // "None"
            }
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

    //toolBar->addAction( d_colorsAction );    //send colors to mainplot
    //toolBar->addAction( d_connectAction ); //update AllAvailableDevices
    //toolBar->addAction( d_sendAction );
    //toolBar->addAction( d_getAction );
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
    label_comboBox_Function_Parameters = new QLabel(tr("Parameters         "));
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
    //label_lineEdit_Counter_Frequency = new QLabel(tr("Frequency"));
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

        //lineEdit_Counter_Frequency[i] = new QLineEdit();
        //lineEdit_Counter_Frequency[i]->setText(QString("1"));
        //lineEdit_Counter_Frequency[i]->setFixedWidth(70);

        lineEdit_Counter_Period[i] = new QLineEdit();
        lineEdit_Counter_Period[i]->setText(QString("1"));
        lineEdit_Counter_Period[i]->setFixedWidth(50);

        lineEdit_Counter_Value[i] = new QLineEdit();
        lineEdit_Counter_Value[i]->setText(QString("1"));
        //lineEdit_Counter_Value[i]->setFixedWidth(50);

        checkBox_Counter_Show_X[i]  = new QCheckBox(tr(""));
        checkBox_Counter_Show_X[i] ->setEnabled(false);

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
    //gridLayout->addWidget(label_lineEdit_Counter_Frequency, 0, 5);
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
        //gridLayout->addWidget(lineEdit_Counter_Frequency[i], i+1, 5);
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
        //comboBox_Device_Functions[r]->setCurrentIndex(0);
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
        //comboBox_Device_Functions[r]->addItem("");
        //comboBox_Device_Functions[r]->setCurrentIndex(0);

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

    file.open(QIODevice::WriteOnly | QIODevice::Text); //remove all info in file
    //QTextStream out(&file);
    //out << "";
    file.close();


    file.open(QIODevice::Append | QIODevice::Text);

    QTextStream output(&file);
    output << "LAMPh" << "\n";
    output << "Version:0.5.1" << "\n";
    output << "" << "\n";
    output << "# Number:Name,number,dll;" << "\n";
    output << "Devices:" << "\n";
    output << "numberofdeviceInt:" << numberofdeviceInt << "\n";
    for (int i=0; i<numberofdeviceInt; i++)
    {
        output << i << ":" << NameDeviceQMap.value(i) << "," << NumberDeviceQMap.value(i) << "," << DLLFileDeviceQMap.value(i) << "\n";
    }
    output << "" << "\n";

    output << "# Number:Device_number,function,parameter,Name,X,Y,Text,Color,Size;" << "\n";
    output << "DATA:" << "\n";
    for (int i=0; i<CurvCnt; i++)
    {
        if (!comboBox_Device[i]->currentText().contains("None", Qt::CaseInsensitive))
        {
            output << i << ":"
                    << comboBox_Device[i]->currentIndex() << ","
                    << comboBox_Device_Functions[i]->currentIndex() << ","
                    << comboBox_Function_Parameters[i]->currentText() << ","   // currentText, because it can be float
                    << lineEdit_NameData[i]->text() << ","
                    << (checkBox_Devices_X[i]->isChecked() ? "1" : "0") << ","
                    << (checkBox_Devices_Y[i]->isChecked() ? "1" : "0") << ","
                    << (checkBox_Device_Text[i]->isChecked() ? "1" : "0") << ","
                    << comboBox_ColorData[i]->currentIndex() << ","
                    << comboBox_SizeData[i]->currentIndex() << "\n";
        }
    }
    output << "" << "\n";

    output << "# Number:From,To,Step,Accelerator,Period,Value,X,Y,Text,Color,Size;" << "\n";
    output << "COUNTERS:" << "\n";
    for (int i=0; i<CurvCounter; i++)
    {
        output << i << ":"
               << lineEdit_Counter_From[i]->text() << ","
               << lineEdit_Counter_To[i]->text() << ","
               << lineEdit_Counter_Step[i]->text() << ","
               << lineEdit_Counter_Acceleration[i]->text() << ","
               << lineEdit_Counter_Period[i]->text() << ","
               << lineEdit_Counter_Value[i]->text() << ","
               << (checkBox_Counter_Show_X[i]->isChecked() ? "1" : "0") << ","
               << (checkBox_Counter_Show_Y[i]->isChecked() ? "1" : "0") << ","
               << (checkBox_Counter_Text[i]->isChecked() ? "1" : "0") << ","
               << comboBox_Counter_ColorData[i]->currentIndex() << ","
               << comboBox_Counter_SizeData[i]->currentIndex() <<"\n";
    }
    output << "" << "\n";

    /*
        Devices:
        numberofdeviceInt:5
        # Number: Name, number, dll
        0: Name, number, dll
        1: Name, number, dll
        2: Name, number, dll
        3: Name, number, dll
        4: None

        DATA:
        # Number: Device_number, function, parameter, Name, X, Y, Text, Color, Size
        0: 0, function, parameter, X, Y, Text, Color, Size
        1:	1, function, parameter, X, Y, Text, Color, Size
        2:	0, function, parameter, X, Y, Text, Color, Size
        3:	None
        ...
        19: None

        COUNTERS:
        # Number: From, To, Step, Accelerator, Period, Value, X, Y, Text, Color, Size
        0:
        1:
        2:
        3:
        4:
     */

    file.flush();
    file.close();
}

void LAMPhDevices::loadConf(){

    QString filePath = QFileDialog::getOpenFileName( this, trUtf8( "Open file" ), "./conf", trUtf8( "Data (*.txt)" ) );
    QFile file(filePath);
    file.open(QIODevice::ReadOnly |QIODevice::Text);

    int numberofdeviceIntConf =0;

    int devicesInt=0;
    int dataInt=0;
    int countersInt=0;

    QMap<int, QStringList> devicesConfQMap;
    QMap<int, QStringList> dataConfQMap;
    QMap<int, QStringList> countersConfQMap;

    QString str;


    str = file.readLine();
    if (str.contains("LAMPh", Qt::CaseInsensitive)){
        str = file.readLine();
        if (str.contains("Version:", Qt::CaseInsensitive)){
            QStringList lst = str.split(":");
            if (lst[1].contains("0.5.1", Qt::CaseInsensitive))
            {
                qDebug() << "Version:0.5.1";
                while(!file.atEnd()){
                    str = file.readLine();
                    if (("\n"==str) or (str.contains("#", Qt::CaseInsensitive))){
                        //skip
                    }else if (str.contains("Devices:", Qt::CaseInsensitive)){
                        str = file.readLine();
                        str.remove("\n");
                        QStringList lst = str.split(":");
                        numberofdeviceIntConf = lst[1].toInt();
                        while (1){
                            str = file.readLine();
                            if ("\n"==str) break;
                            str.remove("\n");
                            QStringList lst = str.split(":");
                            QStringList lst2 = lst[1].split(",");
                            devicesConfQMap[lst[0].toInt()]=lst2;
                        }

                    }else if (str.contains("DATA:", Qt::CaseInsensitive)){
                        while (1){
                            str = file.readLine();
                            if ("\n"==str) break;
                            str.remove("\n");
                            QStringList lst = str.split(":");
                            QStringList lst2 = lst[1].split(",");
                            dataConfQMap[lst[0].toInt()]=lst2;
                        }
                    }else if (str.contains("COUNTERS:", Qt::CaseInsensitive)){
                        while (1){
                            str = file.readLine();
                            if ("\n"==str) break;
                            str.remove("\n");
                            QStringList lst = str.split(":");
                            QStringList lst2 = lst[1].split(",");
                            countersConfQMap[lst[0].toInt()]=lst2;
                        }
                    }else
                    {
                        qDebug() << "ERROR";
                    }
                }
            }else
            {
                qDebug() << "Version of this LAMPh file:" << lst[1] << "\n" << "This file is incompatible with this program" ;
                //break;
            }
        }
    }


    for (int i=0; i<devicesConfQMap.size();i++)
    {
        qDebug() << "Device" << i << ":" << devicesConfQMap.value(i);
    }

    for (int i=0; i<CurvCnt;i++)
         // we can't use dataConfQMap.size(), because size is not equal to numberod DATA and we can skip some of them
    {
        if (dataConfQMap.contains(i))   // to skip the empty
        qDebug() << "DATA" << i << ":" << dataConfQMap.value(i);
    }

    for (int i=0; i<CurvCnt; i++)
    {
        //int comboBox_None = comboBox_Device[i]->count()-1;
        //qDebug() << comboBox_None;
        //comboBox_Device[i]->setCurrentIndex(2);
        //comboBox_Device[i]->setCurrentText("None");
    }

    QMap<int, int> numberofdeviceinlamphQMap; //to match system (lamph) devices with file;

    int l=0;
    for (int i=0; i<devicesConfQMap.size();i++)
    {
        if (devicesConfQMap.contains(i)){
            while (l<NameDeviceQMap.size())
            if
                    ((devicesConfQMap.value(i).at(1).toInt() == NumberDeviceQMap.value(l))
                     and
                    (devicesConfQMap.value(i).at(2) == DLLFileDeviceQMap.value(l)))
            {
                numberofdeviceinlamphQMap[i]=l;
                l=0;
                break;
            }
            else
            {
                l++;
                if (l==NameDeviceQMap.size())
                {
                    qDebug() << "ERROR";
                    //SORRY: You don't have Device: devicesConfQMap.value(i).at(0) or didn't install DLL: devicesConfQMap.value(i).at(2)
                    //try one more time
                    //download this DLL //check network then try to find this DLL in website, if it is false return to SORRY
                    //skip this device
                }
            }
        }
    }

    /*for (int i=0; i<numberofdeviceinlamphQMap.size();i++)
    {
        qDebug() << "numberofdeviceinlamphQMap" << i << ":" << numberofdeviceinlamphQMap.value(i);
    }*/

    for (int i=0; i<CurvCnt /* we can't use dataConfQMap.size(), because this size is not equal to number of DATA and we can skip some of them*/;i++)
    {
        if (dataConfQMap.contains(i))   // to skip the empty
        {
            comboBox_Device[i]->setCurrentIndex(numberofdeviceinlamphQMap.value(dataConfQMap.value(i).at(0).toInt()));
            comboBox_Device_Functions[i]->setCurrentIndex(dataConfQMap.value(i).at(1).toInt());
            QString dataConfQMap_value2QString = dataConfQMap.value(i).at(2);
            if (dataConfQMap_value2QString=="None") comboBox_Function_Parameters[i]->setCurrentIndex(1);
            else if (dataConfQMap_value2QString=="COUNTER0") comboBox_Function_Parameters[i]->setCurrentIndex(2);
            else if (dataConfQMap_value2QString=="COUNTER1") comboBox_Function_Parameters[i]->setCurrentIndex(3);
            else if (dataConfQMap_value2QString=="COUNTER2") comboBox_Function_Parameters[i]->setCurrentIndex(4);
            else if (dataConfQMap_value2QString=="COUNTER3") comboBox_Function_Parameters[i]->setCurrentIndex(5);
            else if (dataConfQMap_value2QString=="COUNTER4") comboBox_Function_Parameters[i]->setCurrentIndex(6);
            else if (dataConfQMap_value2QString=="DATA0") comboBox_Function_Parameters[i]->setCurrentIndex(7);
            else if (dataConfQMap_value2QString=="DATA1") comboBox_Function_Parameters[i]->setCurrentIndex(8);
            else if (dataConfQMap_value2QString=="DATA2") comboBox_Function_Parameters[i]->setCurrentIndex(9);
            else if (dataConfQMap_value2QString=="DATA3") comboBox_Function_Parameters[i]->setCurrentIndex(10);
            else if (dataConfQMap_value2QString=="DATA4") comboBox_Function_Parameters[i]->setCurrentIndex(11);
            else if (dataConfQMap_value2QString=="DATA5") comboBox_Function_Parameters[i]->setCurrentIndex(12);
            else if (dataConfQMap_value2QString=="DATA6") comboBox_Function_Parameters[i]->setCurrentIndex(13);
            else if (dataConfQMap_value2QString=="DATA7") comboBox_Function_Parameters[i]->setCurrentIndex(14);
            else if (dataConfQMap_value2QString=="DATA8") comboBox_Function_Parameters[i]->setCurrentIndex(15);
            else if (dataConfQMap_value2QString=="DATA9") comboBox_Function_Parameters[i]->setCurrentIndex(16);
            else if (dataConfQMap_value2QString=="DATA10") comboBox_Function_Parameters[i]->setCurrentIndex(17);
            else if (dataConfQMap_value2QString=="DATA11") comboBox_Function_Parameters[i]->setCurrentIndex(18);
            else if (dataConfQMap_value2QString=="DATA12") comboBox_Function_Parameters[i]->setCurrentIndex(19);
            else if (dataConfQMap_value2QString=="DATA13") comboBox_Function_Parameters[i]->setCurrentIndex(10);
            else if (dataConfQMap_value2QString=="DATA14") comboBox_Function_Parameters[i]->setCurrentIndex(21);
            else if (dataConfQMap_value2QString=="DATA15") comboBox_Function_Parameters[i]->setCurrentIndex(22);
            else if (dataConfQMap_value2QString=="DATA16") comboBox_Function_Parameters[i]->setCurrentIndex(23);
            else if (dataConfQMap_value2QString=="DATA15") comboBox_Function_Parameters[i]->setCurrentIndex(24);
            else if (dataConfQMap_value2QString=="DATA18") comboBox_Function_Parameters[i]->setCurrentIndex(25);
            else if (dataConfQMap_value2QString=="DATA19") comboBox_Function_Parameters[i]->setCurrentIndex(26);
            else {
                comboBox_Function_Parameters[i]->setCurrentIndex(0);
                comboBox_Function_Parameters[i]->setCurrentText(dataConfQMap_value2QString);
            }
            lineEdit_NameData[i]->setText(dataConfQMap.value(i).at(3));
            (1==dataConfQMap.value(i).at(4).toInt()) ? checkBox_Devices_X[i]->setChecked(true) : checkBox_Devices_X[i]->setChecked(false);
            (1==dataConfQMap.value(i).at(5).toInt()) ? checkBox_Counter_Show_Y[i]->setChecked(true) : checkBox_Counter_Show_Y[i]->setChecked(false);
            (1==dataConfQMap.value(i).at(6).toInt()) ? checkBox_Device_Text[i]->setChecked(true) : checkBox_Device_Text[i]->setChecked(false);
            comboBox_ColorData[i]->setCurrentIndex(dataConfQMap.value(i).at(7).toInt());
            comboBox_SizeData[i]->setCurrentIndex(dataConfQMap.value(i).at(8).toInt());
        }
    }

    for (int i=0; i<CurvCounter;i++)
    {
        lineEdit_Counter_From[i]->setText(QString("%1").arg(countersConfQMap.value(i).at(0)));
        lineEdit_Counter_To[i]->setText(QString("%1").arg(countersConfQMap.value(i).at(1)));
        lineEdit_Counter_Step[i]->setText(QString("%1").arg(countersConfQMap.value(i).at(2)));
        lineEdit_Counter_Acceleration[i]->setText(QString("%1").arg(countersConfQMap.value(i).at(3)));
        lineEdit_Counter_Period[i]->setText(QString("%1").arg(countersConfQMap.value(i).at(4)));
        lineEdit_Counter_Value[i]->setText(QString("%1").arg(countersConfQMap.value(i).at(5)));
        //(1==countersConfQMap.value(i).at(6).toInt()) ? checkBox_Counter_Show_X[i]->setChecked(true) : checkBox_Counter_Show_X[i]->setChecked(false); //must always be false
        (1==countersConfQMap.value(i).at(7).toInt()) ? checkBox_Counter_Show_Y[i]->setChecked(true) : checkBox_Counter_Show_Y[i]->setChecked(false);
        (1==countersConfQMap.value(i).at(8).toInt()) ? checkBox_Counter_Text[i]->setChecked(true) : checkBox_Counter_Text[i]->setChecked(false);
        comboBox_Counter_ColorData[i]->setCurrentIndex(countersConfQMap.value(i).at(9).toInt());
        comboBox_Counter_SizeData[i]->setCurrentIndex(countersConfQMap.value(i).at(10).toInt());
    }


    //for (int r=0; r<CurvCnt; r++)
    //    update_comboBox_Device_Functions(r,0);



    /*for (int r=0; r<CurvCnt; r++)
    {
       for (int y=0; y<numberofdeviceInt; y++)
       {
           comboBox_Device[r]->addItem( QString ("Device %1").arg(y) );
       }
    comboBox_Device[r]->addItem("None");

        if (r<numberofdeviceInt)
        {
            comboBox_Device[r]->setCurrentIndex(r);
            comboBox_Device_Functions[r]->addItems(AllFunctionsFloatVoidDeviceQMap.value(r));

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

    }*/


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

void LAMPhDevices::setCounter(){
    for(int i=0; i<CurvCounter; i++)
    {
        if (0==lineEdit_Counter_Period[i]->text().toInt()) lineEdit_Counter_Period[i]->setText(QString ("1"));

        int period_line = lineEdit_Counter_Period[i]->text().toInt();
        int period =  numberofloop % period_line;

        if ((0==period) and
            ((checkBox_Counter_Show_Y[i]->isChecked()) or (checkBox_Counter_Text[i]->isChecked())))
        {
            if (lineEdit_Counter_Value[i]->text().toFloat()>=lineEdit_Counter_To[i]->text().toFloat())   lineEdit_Counter_Value[i]->setText(lineEdit_Counter_From[i]->text());
            else{
                if (
                        (lineEdit_Counter_Value[i]->text().toFloat()>=lineEdit_Counter_From[i]->text().toFloat())
                        and
                        (lineEdit_Counter_Value[i]->text().toFloat()<lineEdit_Counter_To[i]->text().toFloat())
                    )
                {
                    float valueFloat = lineEdit_Counter_Value[i]->text().toFloat()+lineEdit_Counter_Step[i]->text().toFloat();
                    lineEdit_Counter_Value[i]->setText(QString ("%1").arg(valueFloat));

                    if (lineEdit_Counter_Value[i]->text().toFloat()>lineEdit_Counter_To[i]->text().toFloat()) lineEdit_Counter_Value[i]->setText(lineEdit_Counter_To[i]->text());

                    float stepFloat = lineEdit_Counter_Step[i]->text().toFloat() * lineEdit_Counter_Acceleration[i]->text().toFloat();
                    lineEdit_Counter_Step[i]->setText(QString ("%1").arg(stepFloat));
                    if (lineEdit_Counter_Step[i]->text().toFloat()>lineEdit_Counter_To[i]->text().toFloat()) lineEdit_Counter_Step[i]->setText(lineEdit_Counter_To[i]->text());
                }
            }
            send_all_results(lineEdit_Counter_Value[i]->text().toFloat(),CurvCnt+i);
            W_File->get_all_results(lineEdit_Counter_Value[i]->text().toFloat(),CurvCnt+i);
        }
    }
    numberofloop++;
}


void LAMPhDevices::readData(){

    setCounter();

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

                    /*if (comboBox_Function_Parameters[i]->currentText()=="None") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="COUNTER0") parameter_float = lineEdit_Counter_Value[0]->text().toFloat();
                    else if (comboBox_Function_Parameters[i]->currentText()=="COUNTER1") parameter_float = lineEdit_Counter_Value[1]->text().toFloat();
                    else if (comboBox_Function_Parameters[i]->currentText()=="COUNTER2") parameter_float = lineEdit_Counter_Value[2]->text().toFloat();
                    else if (comboBox_Function_Parameters[i]->currentText()=="COUNTER3") parameter_float = lineEdit_Counter_Value[3]->text().toFloat();
                    else if (comboBox_Function_Parameters[i]->currentText()=="COUNTER4") parameter_float = lineEdit_Counter_Value[4]->text().toFloat();
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA0") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA1") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA2") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA3") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA4") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA5") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA6") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA7") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA8") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA9") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA10") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA11") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA12") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA13") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA14") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA15") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA16") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA15") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA18") parameter_float =0;
                    else if (comboBox_Function_Parameters[i]->currentText()=="DATA19") parameter_float =0;
                    else {
                        parameter_float = comboBox_Function_Parameters[r]->currentText().toFloat();
                    }*/

                    switch (comboBox_Function_Parameters[r]->currentIndex()) {
                    case 0: //only float
                        parameter_float = comboBox_Function_Parameters[r]->currentText().toFloat();
                        break;
                    case 1: //none
                        parameter_float = 0;
                        break;
                    case 2: //COUNTER0
                        parameter_float = lineEdit_Counter_Value[0]->text().toFloat();
                        break;
                    case 3: //COUNTER1
                        parameter_float = lineEdit_Counter_Value[1]->text().toFloat();
                        break;
                    case 4: //COUNTER2
                        parameter_float = lineEdit_Counter_Value[2]->text().toFloat();
                        break;
                    case 5: //COUNTER3
                        parameter_float = lineEdit_Counter_Value[3]->text().toFloat();
                        break;
                    case 6: //COUNTER4
                        parameter_float = lineEdit_Counter_Value[4]->text().toFloat();
                        break;
                    case 7: //DATA0
                        parameter_float = dataFloat[0];
                        break;
                    case 8: //DATA1
                        parameter_float = dataFloat[1];
                        break;
                    case 9: //DATA2
                        parameter_float = dataFloat[2];
                        break;
                    case 10: //DATA3
                        parameter_float = dataFloat[3];
                        break;
                    case 11: //DATA4
                        parameter_float = dataFloat[4];
                        break;
                    case 12: //DATA5
                        parameter_float = dataFloat[5];
                        break;
                    case 13: //DATA6
                        parameter_float = dataFloat[6];
                        break;
                    case 14: //DATA7
                        parameter_float = dataFloat[7];
                        break;
                    case 15: //DATA8
                        parameter_float = dataFloat[8];
                        break;
                    case 16: //DATA9
                        parameter_float = dataFloat[9];
                        break;
                    case 17: //DATA10
                        parameter_float = dataFloat[10];
                        break;
                    case 18: //DATA11
                        parameter_float = dataFloat[11];
                        break;
                    case 19: //DATA12
                        parameter_float = dataFloat[12];
                        break;
                    case 20: //DATA13
                        parameter_float = dataFloat[13];
                        break;
                    case 21: //DATA14
                        parameter_float = dataFloat[14];
                        break;
                    case 22: //DATA15
                        parameter_float = dataFloat[15];
                        break;
                    case 23: //DATA16
                        parameter_float = dataFloat[16];
                        break;
                    case 24: //DATA17
                        parameter_float = dataFloat[17];
                        break;
                    case 25: //DATA18
                        parameter_float = dataFloat[18];
                        break;
                    case 26: //DATA19
                        parameter_float = dataFloat[19];
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
                case 0: //only float
                    parameter_float = comboBox_Function_Parameters[r]->currentText().toFloat();
                    break;
                case 1: //none
                    parameter_float = 0;
                    break;
                case 2: //COUNTER0
                    parameter_float = lineEdit_Counter_Value[0]->text().toFloat();
                    break;
                case 3: //COUNTER1
                    parameter_float = lineEdit_Counter_Value[1]->text().toFloat();
                    break;
                case 4: //COUNTER2
                    parameter_float = lineEdit_Counter_Value[2]->text().toFloat();
                    break;
                case 5: //COUNTER3
                    parameter_float = lineEdit_Counter_Value[3]->text().toFloat();
                    break;
                case 6: //COUNTER4
                    parameter_float = lineEdit_Counter_Value[4]->text().toFloat();
                    break;
                case 7: //DATA0
                    parameter_float = dataFloat[0];
                    break;
                case 8: //DATA1
                    parameter_float = dataFloat[1];
                    break;
                case 9: //DATA2
                    parameter_float = dataFloat[2];
                    break;
                case 10: //DATA3
                    parameter_float = dataFloat[3];
                    break;
                case 11: //DATA4
                    parameter_float = dataFloat[4];
                    break;
                case 12: //DATA5
                    parameter_float = dataFloat[5];
                    break;
                case 13: //DATA6
                    parameter_float = dataFloat[6];
                    break;
                case 14: //DATA7
                    parameter_float = dataFloat[7];
                    break;
                case 15: //DATA8
                    parameter_float = dataFloat[8];
                    break;
                case 16: //DATA9
                    parameter_float = dataFloat[9];
                    break;
                case 17: //DATA10
                    parameter_float = dataFloat[10];
                    break;
                case 18: //DATA11
                    parameter_float = dataFloat[11];
                    break;
                case 19: //DATA12
                    parameter_float = dataFloat[12];
                    break;
                case 20: //DATA13
                    parameter_float = dataFloat[13];
                    break;
                case 21: //DATA14
                    parameter_float = dataFloat[14];
                    break;
                case 22: //DATA15
                    parameter_float = dataFloat[15];
                    break;
                case 23: //DATA16
                    parameter_float = dataFloat[16];
                    break;
                case 24: //DATA17
                    parameter_float = dataFloat[17];
                    break;
                case 25: //DATA18
                    parameter_float = dataFloat[18];
                    break;
                case 26: //DATA19
                    parameter_float = dataFloat[19];
                    break;
                default:
                    break;
                }

                setData(NumberDeviceQMap.value(comboBox_Device[r]->currentIndex()),parameter_float);
                res = parameter_float;
            }


            //if ((checkBox_Devices_Y[r]->isChecked()) or (checkBox_Devices_X[r]->isChecked()))
            if (checkBox_Devices_Y[r]->isChecked())
            {

                send_all_results(res,r);
                send_all_results_to_lamphplot(res,r);
                // W_File->get_all_results(res,r);


                /*if (checkBox_Devices_X[r]->isChecked())
                {
                    send_x_result(res);
                }*/
            }

            if (checkBox_Devices_X[r]->isChecked()) //if (checkBox_Device_Text[r]->isChecked())
            {
                send_x_result(res);
                send_all_results_to_lamphplot(res,r);
                W_File->get_x_result(res);
            }

            if (checkBox_Device_Text[r]->isChecked())
            {
                W_File->get_all_results(res,r);
            }
            dataFloat[r]=res;
        }
    }
    W_File->write_in_file();
    //qDebug() << "get";
}

void LAMPhDevices::toolBar_GET_show_data()
{



    if (int_GET<CurvCnt){
        send_toolBar_GET_show_data(int_GET);
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
    send_toolBar_GET_hide_data(int_GET);
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
    label_Ports = new QLabel(tr("Devices"));
    for(int i=0; i<CurvCnt; i++)
    {
        label_Port[i] = new QLabel();
        label_Port[i]->setText(QString("PORT %1:").arg(i));
        button_Port_Setting[i] = new QPushButton(tr("Settings"));
        //connect(button_Port_Setting[i],SIGNAL(released()), this, SLOT(openDialogDeviceSettings()));

        connect(button_Port_Setting[i], static_cast<void(QPushButton::*)(void)>(&QPushButton::released),
                [=](void){
                         openDialogDeviceSettings(i);
           });
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

void LAMPhDevices::update_toolBar_PORTS(){

    for (int i=0; i<CurvCnt; i++)
    {

        label_Port[i]->hide();
        button_Port_Setting[i]->hide();
    }

    for (int i=0; i<numberofdeviceInt; i++)
    {
        label_Port[i]->setText(QString("Device %1: Name:%2#%3 Dll:%4").arg(i).arg(NameDeviceQMap.value(i)).arg(NumberDeviceQMap.value(i)).arg(DLLFileDeviceQMap.value(i)));
        label_Port[i]->show();
        button_Port_Setting[i]->show();
    }
}

void LAMPhDevices::openDialogDeviceSettings(int numberofdeviceInt){
    DialogDeviceSettings *addDialogDeviceSettings = new DialogDeviceSettings(DLLFileDeviceQMap.value(numberofdeviceInt),NumberDeviceQMap.value(numberofdeviceInt));
    //connect(addDialogReg, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));
    addDialogDeviceSettings->setWindowTitle(QString("Device:%1#%2").arg(NameDeviceQMap.value(numberofdeviceInt)).arg(NumberDeviceQMap.value(numberofdeviceInt)));
    addDialogDeviceSettings->exec();
}

void LAMPhDevices::appendPoints( bool on )
{
    if ( on )
    {
        W_File->create_new_file();

    }
    else
    {
        numberofloop=0;
        //d_plot->stop();
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

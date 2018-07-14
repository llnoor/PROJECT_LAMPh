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

/* #include <QtSerialPort/QSerialPortInfo> */

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


class ConnectComDevice
{
private: // спецификатор доступа private
    QString nameDll;
    QString comPort;
    QString comInfo;
    QString LIB_NAME;

    //QLibrary lib;
    float result_float;
    double result_double;



public: // спецификатор доступа public
    ConnectComDevice(QString nameDll_, QString comPort_, QString comInfo_ ) // конструктор класса
    {
        nameDll=nameDll_;
        comPort=comPort_;
        comInfo=comInfo_;

        static QString suffix = "";
        #ifdef QT_DEBUG
            suffix = "d";
        #endif

        /*static const QString */ LIB_NAME = nameDll + suffix;

        QLibrary lib ( LIB_NAME );
        if( !lib.load() ) {
            qDebug() << "Loading failed!";
        }


        //checkCOM
        //setCOM

        /* !!! Important !!! Take attention !!!
         * You should get TRUE from Ready, only after that you can transfer data from DLL
        */
    }

    /*void getInfo()
    {
        QLibrary lib ( LIB_NAME );
        typedef const char* ( *OutputTest )();
        OutputTest outputTest;

        outputTest = ( OutputTest ) lib.resolve( "getInfo" );
        if( outputTest ) {
            qDebug() << outputTest();
        }
    }*/

    void getFloat(int number_of_device)
    {
        QLibrary lib ( LIB_NAME );


        typedef bool ( *setCh )(int, const char * );
        setCh setChd  =  ( setCh ) lib.resolve( "setPORT" );
        if( setChd ) {
            qDebug() << setChd(number_of_device, comPort.toStdString().c_str() );
        }


        typedef float ( *outputFloat )(int);
        outputFloat outputFloatd  = ( outputFloat ) lib.resolve( "getFloat" );
        for (int y=0; y<5; y++)
        {
            if( outputFloatd ) {
                qDebug() << QString::number(outputFloatd(number_of_device));
            }
        }


        /*for (int i=0; i<11; i++)
                {
                    if( outputFloatd ) {
                        qDebug() << outputFloatd(number_of_device);
                    }
                }*/


        /*for (int i=0; i<11; i++)
        {
            if( outputFloatd ) {
                qDebug() << outputFloatd(number_of_device);
            }
        }

        if( setChd ) {
            qDebug() << setChd(number_of_device, 5 );
        }

        for (int i=0; i<11; i++)
        {
            if( outputFloatd ) {
                qDebug() << outputFloatd(number_of_device);
            }
        }*/

    }



    /*void message() // функция (метод класса) выводящая сообщение на экран
    {
        cout << "\nwebsite: cppstudio.com\ntheme: Classes and Objects in C + +\n";
    }

    void getDate() // отобразить текущую дату
    {
        cout << "date: " << day << "." << month << "." << year << endl;
    }*/
};



LAMPhDevices::LAMPhDevices(QString loginQString)
{

    addToolBar(Qt::TopToolBarArea, toolBar()); //buttons

    addToolBar(Qt::LeftToolBarArea, toolBar_GET()); //these are fields for selecting functions of DEVICES
    /*addToolBar(Qt::LeftToolBarArea, toolBar_SEND());
    addToolBar(Qt::LeftToolBarArea, toolBar_COUNTERS());*/

    addToolBar(Qt::RightToolBarArea, toolBar_PORTS()); //All Available Serial Ports (COM+USB+LAN+Sockets and so on)
    //addToolBar(Qt::RightToolBarArea, toolBar_DEVICES());

#ifndef QT_NO_STATUSBAR
    ( void )statusBar();
#endif


    login = new QString(); // to transfer the user's login and delineation of rights
    *login = loginQString;

    labelPlotSettingS = new QLabel(tr(" ")); //free space in the center of the window, due to "mainwindow" features

    /*QLabel       lbl("this is the example text");
    QLibrary     lib("dynlibd");

    typedef QString (*Fct) (const QString&);
    Fct fct = (Fct)(lib.resolve("oddUpper"));
    if (fct) {
        labelPlotSettingS->setText("fct(lbl.text())");
    }*/

    setCentralWidget( labelPlotSettingS ); //mainwindow does not work without this thing

    //getDataDll();
    //initWhatsThis();

    setContextMenuPolicy( Qt::NoContextMenu );

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

    setWindowTitle(tr("LAMPhDevices - %1 ").arg(login->toLower()));
    //showFullScreen();

    showMinimized();
    //this->setWindowState(Qt::WindowMaximized);


    //ConnectComDevice connectDevice("COM_APPA205","COM10","APPA205");
    //connectDevice.getInfo();
    //connectDevice.getFloat(1);


    //ConnectComDevice connectDevice1("COM_APPA205","COM12","APPA205");
    //connectDevice1.getInfo();
    //connectDevice1.getFloat(2);
    getAllAvailableSerialPorts();
    showMaximized();

    for (int r=0; r<20; r++)
    {
        connect(comboBox_Device[r], static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                 [=](int index){
                 update_comboBox_Device_Functions(r,index);
            });
    }

    connect (d_sendAction, SIGNAL( triggered() ) , this, SLOT (send_readData()));
    connect (d_getAction, SIGNAL( triggered() ) , this, SLOT (readData()));

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

    // COM PORT

    /*for (int i=0; i<listDllCOM->size();i++)
    {
        QStringList receivedDataList;   //=outputTest
        // receivedDataList transfer to next Strings!!!
        QString nameofdeviceString; // = "APPA205";
        QString commandString; //= "byte:00;00;55;55;AA";
        QString respondString; //= "APPA205";

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
        for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
            //AllAvailableSerialPortsQMap[info.portName()] = (info.isBusy() ? QObject::tr("Busy") : QObject::tr("Ready"));
            if (!info.isBusy())
            {
                QSerialPort newSerialPort;
                newSerialPort.setPortName(info.portName());
                newSerialPort.setBaudRate(QSerialPort::Baud9600);
                newSerialPort.setStopBits(QSerialPort::OneStop);
                newSerialPort.setDataBits(QSerialPort::Data8);
                newSerialPort.setParity(QSerialPort::NoParity);
                newSerialPort.setFlowControl(QSerialPort::NoFlowControl);
                newSerialPort.open(QIODevice::ReadWrite);
                newSerialPort.write(commandString.toLocal8Bit());
                newSerialPort.waitForReadyRead(300);

                QByteArray data;
                data = newSerialPort.readAll();
                std::string result_tmp = data.toStdString();
                QString data_tmp = QString::fromStdString(result_tmp);   //here should be name of devices, SN and so on.

                newSerialPort.close();

                if (data_tmp.contains(respondString, Qt::CaseInsensitive)){

                    //set COM PORT (this port will be Busy when info.isBusy() will check)
                    typedef bool (*Fct) (const int, const char*);
                    Fct fct = (Fct)(lib.resolve("setPORT"));
                    fct(numberTHISdevice,info.portName().toLatin1());
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

                    }
                    AllAvailableSerialPortsQMap[info.portName()] = QString ("COM_Device%1:%2 Number:%3 DLL:%4").arg(numberofdeviceInt).arg(nameofdeviceString).arg(numberTHISdevice).arg(listDllCOM->at(i));

                    numberTHISdevice++;
                    numberofdeviceInt++;
                }
            }
            //qDebug() << info.portName() << ":" << AllAvailableSerialPortsQMap.value(info.portName());
            //qDebug() << info.portName() << "Device:" << AllAvailableDevicesQMap.value(numberofdeviceInt);
        }
    }
*/


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

    // USB
    for (int i=0; i<listDllUSB->size();i++)
    {
        QStringList receivedDataList;
        QString nameofdeviceString;
        QString commandString;
        QString respondString;

        QLibrary lib ( listDllUSB->at(i) );
        /*typedef const char* ( *ReceivedData )();
        ReceivedData receivedData;

        receivedData = ( ReceivedData ) lib.resolve( "getUSBcommands" );
        if( receivedData ) {
            receivedDataList = QString::fromUtf8(receivedData()).split(",");
            qDebug() << receivedData();
        }

        nameofdeviceString = receivedDataList.at(0);
        commandString = receivedDataList.at(1);
        respondString = receivedDataList.at(2);*/


        nameofdeviceString = "d"; //sorry

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

                numberTHISdevice++;
                numberofdeviceInt++;
            }
        }
    }

    for (int i=0; i<numberofdeviceInt; i++) {
        //qDebug() << "AllAvailableDevicesQMap:" << AllAvailableDevicesQMap.value(i);
        qDebug() << i;
        qDebug() << "NameDeviceQMap:" << NameDeviceQMap.value(i);
        qDebug() << "NumberDeviceQMap:" << NumberDeviceQMap.value(i);
        qDebug() << "DLLFileDeviceQMap:" << DLLFileDeviceQMap.value(i);
        qDebug() << "AllFunctionsDeviceQMap:" << AllFunctionsDeviceQMap.value(i);
    }



    // we have numberofdeviceInt devices
    for (int r=0; r<20; r++)
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
            comboBox_Device_Functions[r]->addItems(AllFunctionsDeviceQMap.value(r));
            comboBox_Device_Functions[r]->addItem("None");

            lineEdit_NameData[r]->setText(QString ("%1#%2").arg(NameDeviceQMap.value(r)).arg(NumberDeviceQMap.value(r)));
        }
        else comboBox_Device[r]->setCurrentIndex(numberofdeviceInt); //"None"
        //comboBox_DeviceQMap[r]=numberofdeviceInt;

    }
}


QToolBar *LAMPhDevices::toolBar()
{
    MyToolBar *toolBar = new MyToolBar( this );

    toolBar->setAllowedAreas( Qt::TopToolBarArea | Qt::BottomToolBarArea );
    setToolButtonStyle( Qt::ToolButtonTextUnderIcon );

    d_connectAction = new QAction( QPixmap( start_xpm ), "Connect", toolBar );
    d_sendAction = new QAction( QPixmap( clear_xpm ), "Send/Set", toolBar );




    d_getAction = new QAction( QPixmap( zoom_xpm ), "Get/Test", toolBar );


    QAction *whatsThisAction = QWhatsThis::createAction( toolBar );
    whatsThisAction->setText( "Help" );

    toolBar->addAction( d_connectAction ); //update AllAvailableDevices
    toolBar->addAction( d_sendAction );
    toolBar->addAction( d_getAction );
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
    label_comboBox_Function_Parameters = new QLabel(tr("Parameters"));
    label_lineEdit_NameData = new QLabel(tr("Name"));
    label_checkBox_Device_Show = new QLabel(tr("Plot"));
    label_checkBox_Device_Text = new QLabel(tr("Text"));
    label_checkBox_Device_DB = new QLabel(tr("DB"));
    label_comboBox_ColorData = new QLabel(tr("Color"));
    label_comboBox_SizeData = new QLabel(tr("Size"));

    for(int i=0; i<20; i++)
    {
        label_ReceivedData[i] = new QLabel();
        label_ReceivedData[i]->setText(QString("DATA %1:").arg(i));
        comboBox_Device[i] = new QComboBox();
        comboBox_Device_Functions[i] = new QComboBox();
        comboBox_Function_Parameters[i] = new QComboBox();
        lineEdit_NameData[i] = new QLineEdit();
        lineEdit_NameData[i]->setText(QString("Name %1:").arg(i));
        //lineEdit_NameData[i]->setFixedWidth(60);
        checkBox_Device_Show[i]  = new QCheckBox(tr(""));
        checkBox_Device_Text[i]  = new QCheckBox(tr(""));
        checkBox_Device_DB[i]  = new QCheckBox(tr(""));
        comboBox_ColorData[i] = new QComboBox();
        comboBox_SizeData[i] = new QComboBox();



        for (int l = 0; l < 5; ++l) {
            //comboBox_Device[i]->addItem(QString("Item %1").arg(l));
            //comboBox_Device_Functions[i]->addItem(QString("Item %1").arg(l));
            //comboBox_Function_Parameters[i]->addItem(QString("Item %1").arg(l));
            comboBox_ColorData[i]->addItem(QString("Item %1").arg(l));
            comboBox_SizeData[i]->addItem(QString("Item %1").arg(l));
        }
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

    for(int i=int_GET; i<20; i++)
    {
        label_ReceivedData[i]->hide();
        comboBox_Device[i]->hide();
        comboBox_Device_Functions[i]->hide();
        comboBox_Function_Parameters[i]->hide();
        lineEdit_NameData[i]->hide();
        checkBox_Device_Show[i]->hide();
        checkBox_Device_Text[i]->hide();
        checkBox_Device_DB[i]->hide();
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
    gridLayout->addWidget(label_checkBox_Device_Show, 0, 5);
    gridLayout->addWidget(label_checkBox_Device_Text, 0, 6);
    gridLayout->addWidget(label_checkBox_Device_DB, 0, 7);
    gridLayout->addWidget(label_comboBox_ColorData, 0, 8);
    gridLayout->addWidget(label_comboBox_SizeData, 0, 9);

    for(int i=0; i<20; i++)
    {
        gridLayout->addWidget(label_ReceivedData[i], i+1, 0);
        gridLayout->addWidget(comboBox_Device[i], i+1, 1);
        gridLayout->addWidget(comboBox_Device_Functions[i], i+1, 2);
        gridLayout->addWidget(comboBox_Function_Parameters[i], i+1, 3);
        gridLayout->addWidget(lineEdit_NameData[i], i+1, 4);
        gridLayout->addWidget(checkBox_Device_Show[i], i+1, 5);
        gridLayout->addWidget(checkBox_Device_Text[i], i+1, 6);
        gridLayout->addWidget(checkBox_Device_DB[i], i+1, 7);
        gridLayout->addWidget(comboBox_ColorData[i], i+1, 8);
        gridLayout->addWidget(comboBox_SizeData[i], i+1, 9);
    }

    gridLayout->addWidget(button_ReceivedData_Close,22,2);
    gridLayout->addWidget(button_ReceivedData_Add,22,1);

    gridLayout->setContentsMargins(5,5,5,5);
    gridLayout->setVerticalSpacing(5);
    gridLayout->setHorizontalSpacing(5);

    toolBar_GET->addWidget( hBox_GET );
    return toolBar_GET;
}

void LAMPhDevices::update_comboBox_Device_Functions(int r, int Index){
    qDebug() << "r" << r;
    qDebug() << "Index" << Index;

    while (comboBox_Device_Functions[r]->count()>0) comboBox_Device_Functions[r]->removeItem(0);
    comboBox_Device_Functions[r]->addItems(AllFunctionsDeviceQMap.value(comboBox_Device[r]->currentIndex()));
    comboBox_Device_Functions[r]->addItem("None");
    lineEdit_NameData[r]->setText(QString ("%1#%2").arg(NameDeviceQMap.value(comboBox_Device[r]->currentIndex())).arg(NumberDeviceQMap.value(comboBox_Device[r]->currentIndex())));
}

void LAMPhDevices::send_readData(){

    for (int r=0;r<numberofdeviceInt;r++){
        // if checkBoxes are "Enabled"
        QLibrary lib (DLLFileDeviceQMap.value(comboBox_Device[r]->currentIndex()));

        typedef void (*PleaseReadData) ();
        PleaseReadData pleaseReadData = (PleaseReadData)(lib.resolve("readData"));
        pleaseReadData();
        qDebug() << QString ("%1").arg(comboBox_Device_Functions[r]->currentText());
    }
    qDebug() << "send";

}

void LAMPhDevices::readData(){
    for (int r=0;r<numberofdeviceInt;r++){
        // if checkBoxes are "Enabled"
        QLibrary lib (DLLFileDeviceQMap.value(comboBox_Device[r]->currentIndex()));

        typedef float (*GetData) (int);
        //GetData getData = (GetData)(lib.resolve(QString ("%1").arg(comboBox_Device_Functions[r]->currentText()) ));

        //GetData getData = (GetData)(lib.resolve(QString ("%1").arg(comboBox_Device_Functions[r]->currentText()).toLatin1() ));

        GetData getData = (GetData)(lib.resolve("getFloat"));


        float res = getData(NumberDeviceQMap.value(comboBox_Device[r]->currentIndex()));
        qDebug() << res;
    }
    qDebug() << "get";
}

void LAMPhDevices::toolBar_GET_show_data()
{
    label_ReceivedData[int_GET]->show();
    comboBox_Device[int_GET]->show();
    comboBox_Device_Functions[int_GET]->show();
    comboBox_Function_Parameters[int_GET]->show();
    lineEdit_NameData[int_GET]->show();
    checkBox_Device_Show[int_GET]->show();
    checkBox_Device_Text[int_GET]->show();
    checkBox_Device_DB[int_GET]->show();
    comboBox_ColorData[int_GET]->show();
    comboBox_SizeData[int_GET]->show();
    if (int_GET<20)int_GET++;
}

void LAMPhDevices::toolBar_GET_hide_data()
{
    if (int_GET>5) int_GET--;
    label_ReceivedData[int_GET]->hide();
    comboBox_Device[int_GET]->hide();
    comboBox_Device_Functions[int_GET]->hide();
    comboBox_Function_Parameters[int_GET]->hide();
    lineEdit_NameData[int_GET]->hide();
    checkBox_Device_Show[int_GET]->hide();
    checkBox_Device_Text[int_GET]->hide();
    checkBox_Device_DB[int_GET]->hide();
    comboBox_ColorData[int_GET]->hide();
    comboBox_SizeData[int_GET]->hide();
}

QToolBar *LAMPhDevices::toolBar_PORTS()
{
    MyToolBar *toolBar_PORTS = new MyToolBar( this );
    toolBar_PORTS->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    hBox_PORTS = new QWidget( toolBar_PORTS );


    label_Ports = new QLabel(tr("PORTS"));


    for(int i=0; i<20; i++)
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


    for(int i=0; i<20; i++)
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


    for (int i=0; i<20; i++)
    {

        label_Port[i]->hide();
        button_Port_Setting[i]->hide();
    }


    int device=0;
    //const infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        QString s = QObject::tr("Port: ") + info.portName() + "\n"
                + AllAvailableSerialPortsQMap.value(info.portName()) + "\n"
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

/*QGroupBox *LAMPhDevices::groupLAMPhDATA()
{
    QGroupBox *groupBox = new QGroupBox(tr(""));
    //groupBox->setStyleSheet("border: 0px solid white");

    label_label_ReceivedData = new QLabel(tr("DATA"));
    label_comboBox_Device = new QLabel(tr("Devices"));
    label_comboBox_Device_Functions = new QLabel(tr("Functions"));
    label_comboBox_Function_Parameters = new QLabel(tr("Parameters"));
    label_lineEdit_NameData = new QLabel(tr("Name"));
    label_checkBox_Device_Show = new QLabel(tr("Plot"));
    label_checkBox_Device_Text = new QLabel(tr("Text"));
    label_checkBox_Device_DB = new QLabel(tr("DB"));
    label_comboBox_ColorData = new QLabel(tr("Color"));
    label_comboBox_SizeData = new QLabel(tr("Size"));

    for(int i=0; i<20; i++)
    {
        label_ReceivedData[i] = new QLabel(tr("DATA"));
        comboBox_Device[i] = new QComboBox();
        comboBox_Device_Functions[i] = new QComboBox();
        comboBox_Function_Parameters[i] = new QComboBox();
        lineEdit_NameData[i] = new QLineEdit(tr("Name"));
        //lineEdit_NameData[i]->setFixedWidth(60);
        checkBox_Device_Show[i]  = new QCheckBox(tr(""));
        checkBox_Device_Text[i]  = new QCheckBox(tr(""));
        checkBox_Device_DB[i]  = new QCheckBox(tr(""));
        comboBox_ColorData[i] = new QComboBox();
        comboBox_SizeData[i] = new QComboBox();
    }


    QGridLayout * gridLayout = new QGridLayout();

    //gridLayout->addWidget(labelPlotSetting,0,0);
    //gridLayout->addWidget(groupLAMPhDATA(),0,1);

    gridLayout->addWidget(label_label_ReceivedData, 0, 0);
    gridLayout->addWidget(label_comboBox_Device, 0, 1);
    gridLayout->addWidget(label_comboBox_Device_Functions, 0, 2);
    gridLayout->addWidget(label_comboBox_Function_Parameters, 0, 3);
    gridLayout->addWidget(label_lineEdit_NameData, 0, 4);
    gridLayout->addWidget(label_checkBox_Device_Show, 0, 5);
    gridLayout->addWidget(label_checkBox_Device_Text, 0, 6);
    gridLayout->addWidget(label_checkBox_Device_DB, 0, 7);
    gridLayout->addWidget(label_comboBox_ColorData, 0, 8);
    gridLayout->addWidget(label_comboBox_SizeData, 0, 9);


    for(int i=0; i<20; i++)
    {
        gridLayout->addWidget(label_ReceivedData[i], i+1, 0);
        gridLayout->addWidget(comboBox_Device[i], i+1, 1);
        gridLayout->addWidget(comboBox_Device_Functions[i], i+1, 2);
        gridLayout->addWidget(comboBox_Function_Parameters[i], i+1, 3);
        gridLayout->addWidget(lineEdit_NameData[i], i+1, 4);
        gridLayout->addWidget(checkBox_Device_Show[i], i+1, 5);
        gridLayout->addWidget(checkBox_Device_Text[i], i+1, 6);
        gridLayout->addWidget(checkBox_Device_DB[i], i+1, 7);
        gridLayout->addWidget(comboBox_ColorData[i], i+1, 8);
        gridLayout->addWidget(comboBox_SizeData[i], i+1, 9);

    }
    button_ReceivedData_Close = new QPushButton(tr("Close"));
    button_ReceivedData_Add = new QPushButton(tr("Add"));


    gridLayout->addWidget(button_ReceivedData_Close,22,0);
    gridLayout->addWidget(button_ReceivedData_Add,23,0);

    gridLayout->setContentsMargins(5,5,5,5);
    gridLayout->setVerticalSpacing(5);
    gridLayout->setHorizontalSpacing(5);


    groupBox->setLayout(gridLayout);
    return groupBox;
}

QGroupBox *LAMPhDevices::groupLAMPhPorts()
{
    QGroupBox *groupBox = new QGroupBox(tr(""));
    //groupBox->setStyleSheet("border: 0px solid white");

    // COM PORTS (Sockets, LAN)

    label_label_COM_Port = new QLabel(tr("Ports"));
    for(int i=0; i<20; i++) label_COM_Port[i] = new QLabel(tr("COM i  Device -> APPA205Lib"));

    // DEVICES

    label_label_Device = new QLabel(tr("Devices"));

    for(int i=0; i<20; i++){
        label_Device[i] = new QLabel(tr("I APPA205"));
        label_Device_COM_Port[i] = new QLabel(tr("COM 1"));
        button_Device_Info[i] = new QPushButton(tr("Info"));
        button_Device_Setting[i] = new QPushButton(tr("Settings"));
    }



    QGridLayout * gridLayout = new QGridLayout();


    gridLayout->addWidget(label_label_COM_Port, 0, 0);

    for(int i=0; i<20; i++)
    {
        gridLayout->addWidget(label_COM_Port[i], i+1, 0);
    }


    gridLayout->addWidget(label_label_COM_Port, 22, 0);

    for(int i=0; i<20; i++)
    {
        gridLayout->addWidget(label_Device[i], i+23, 0);
        gridLayout->addWidget(label_Device_COM_Port[i], i+23, 1);
        gridLayout->addWidget(button_Device_Info[i], i+23, 2);
        gridLayout->addWidget(button_Device_Setting[i], i+23, 3);
    }


    groupBox->setLayout(gridLayout);

     return groupBox;
}*/

void LAMPhDevices::getDataDll()
{

   /*
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

*/





        /*typedef void ( *InputTest )( const char* const );
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
        }*/





}




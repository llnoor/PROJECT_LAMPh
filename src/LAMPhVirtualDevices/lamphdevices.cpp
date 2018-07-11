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

#include <QTime>
#include <QProcess>

/* #include <QtSerialPort/QSerialPortInfo> */

#include <QSerialPort>
#include <QSerialPortInfo>

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
        typedef float ( *outputFloat )(int);
        outputFloat outputFloatd  = ( outputFloat ) lib.resolve( "getFloat" );

        typedef bool ( *setCh )(int, float);
        setCh setChd  =  ( setCh ) lib.resolve( "setCh" );


        for (int i=0; i<11; i++)
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
        }

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

    QStringList arguments;
        arguments << "C:\\Git\\git\\PROJECT\\bin\\debug\\Emulator\\5pairs.vspe" << "-minimize" << "-hide_splash";
    QProcess process;
    process.startDetached("./Emulator/VSPEmulator.exe", arguments);

    //process.waitForStarted(3000);
    //process.waitForFinished (30);

    addToolBar(Qt::TopToolBarArea, toolBar());

    addToolBar(Qt::LeftToolBarArea, toolBar_GET());
    /*addToolBar(Qt::LeftToolBarArea, toolBar_SEND());
    addToolBar(Qt::LeftToolBarArea, toolBar_COUNTERS());*/

    addToolBar(Qt::RightToolBarArea, toolBar_PORTS());
    //addToolBar(Qt::RightToolBarArea, toolBar_DEVICES());

#ifndef QT_NO_STATUSBAR
    ( void )statusBar();
#endif


    login = new QString();
    *login = loginQString;

    labelPlotSettingS = new QLabel(tr(" "));

    setCentralWidget( labelPlotSettingS );

    //getDataDll();
    //initWhatsThis();

    setContextMenuPolicy( Qt::NoContextMenu );



    setWindowTitle(tr("LAMPhDevices - %1 ").arg(login->toLower()));
    //showFullScreen();
    showMaximized();
    //hide();
    //this->setWindowState(Qt::WindowMaximized);
    //this->setWindowState(Qt::WindowMinimized);

    for (int i=0; i<5; i++)
    {
        comSerialPort[i] = new QSerialPort();
        set_com_port(i,QString("COM%1").arg(11+2*i));
        connect(comSerialPort[i], SIGNAL(readyRead()),this, SLOT(readData()));

    }
    readData();


}

void LAMPhDevices::set_com_port(int number ,QString COM_port)
{
    comSerialPort[number]->setPortName(COM_port);
    comSerialPort[number]->setBaudRate(QSerialPort::Baud9600);
    comSerialPort[number]->setStopBits(QSerialPort::OneStop);
    comSerialPort[number]->setDataBits(QSerialPort::Data8);
    comSerialPort[number]->setParity(QSerialPort::NoParity);
    comSerialPort[number]->setFlowControl(QSerialPort::NoFlowControl);
    comSerialPort[number]->open(QIODevice::ReadWrite);
    comSerialPort[number]->write("QString");
}

void LAMPhDevices::sendData(int number)
{
    comSerialPort[number]->write(":READ?\r\n");
}


void LAMPhDevices::readData()
{
    for(int i=0; i<5;i++)
    {
        while (! comSerialPort[i]->atEnd()) {
                QByteArray dataByteArray = comSerialPort[i]->read(100);
                std::string result_tmp = dataByteArray.toStdString();
                QString data_tmp = QString::fromStdString(result_tmp);
                QString data_send;
                QString data_received;
                data_tmp.remove("\n");
                data_tmp.remove("\r");

                /*QString data_new = QString::fromStdString(dataByteArray.toStdString().c_str());


                qDebug(dataByteArray);
                qDebug() << "data_tmp" << data_tmp;
                qDebug("/");
                qDebug() << "data_new" << data_new;
                qDebug("/");

                char *buff = dataByteArray.data();
                int buff_int[24];

                for (int l=0; l<24; l++){
                    buff_int[l]=buff[l];
                    qDebug("buff_int[%d]: %d", l, buff_int[l]);
                }

                data_send = "byte: 00 00 55 55 AA";

                if (data_send.contains("byte:", Qt::CaseInsensitive)){
                    data_send.remove("byte:");
                    QByteArray text = QByteArray::fromHex(data_send.toLocal8Bit());
                    comSerialPort[i]->write(text);
                }else
                {
                    comSerialPort[i]->write(data_send.toLocal8Bit());
                }*/

                //QString data_tmp3(text.toHex());

                /*data_send = "00 00 55 55 AA 55 ";
                QByteArray text = QByteArray::fromHex(data_send.toLocal8Bit());
                //comSerialPort[i]->write(text);


                std::string stdString(text.constData(), text.length());

                QString data_tmp2 = QString::fromStdString(stdString);

                qDebug() << stdString.data();

                comSerialPort[i]->write(data_tmp2.toLocal8Bit());*/

                //QString data_tmp3 = QString::fromStdString(text.toStdString());
                //comSerialPort[i]->write(data_tmp3.toLocal8Bit());





                for(int l=0; l<10;l++)
                {
                    //qDebug() << "l" << l << "box" << comboBox_Pairs[l]->currentIndex();
                    if (i == comboBox_Pairs[l]->currentIndex()){


                        if ("time"==send[l]->text()) {
                            QDateTime date=QDateTime::currentDateTime();
                            data_send =date.toString("mm.ss");
                        }
                        else if("*"==send[l]->text()) {
                            data_send=data_tmp;
                        }
                        else if(""==send[l]->text()) {
                            data_send="empty";
                        }
                        else if(send[l]->text().contains("byte:", Qt::CaseInsensitive)) {
                            data_send = send[l]->text();
                            data_send.remove("byte:");
                            data_send = QString::fromStdString(QByteArray::fromHex(data_send.toLocal8Bit()).toStdString());
                        }
                        else{
                            data_send=send[l]->text();
                        }


                        if ("*"==received[l]->text()){
                            comSerialPort[i]->write(data_send.toLocal8Bit());
                        }
                        else if (received[l]->text().contains("byte:", Qt::CaseInsensitive)){
                            data_received = received[l]->text();
                            data_received.remove("byte:");
                            QString data_received2= (dataByteArray.toHex());
                            if (data_received2.contains(data_received, Qt::CaseInsensitive)){
                               comSerialPort[i]->write(data_send.toLocal8Bit());
                            }
                        }
                        else if(data_tmp==received[l]->text()){
                            comSerialPort[i]->write(data_send.toLocal8Bit());
                        }

                    }
                }


                QStringList status;
                status << "COM:" << QString::number(2*i+11) << "; Received:" << data_tmp << "; Send:" << data_send << ";";


                // " Received:" + data_tmp + " Send:" + data_send;
                statusBar()->showMessage( status.join("") );


                //comSerialPort[i]->write(data_n);



        }

    }


}


void LAMPhDevices::readData1()
{
    while (! comSerialPort[1]->atEnd()) {
            QByteArray data_n = comSerialPort[1]->read(100);
            qDebug() << data_n;
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

    toolBar->addAction( d_connectAction );
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

    d_OpenWindow_Exit = new QAction( QPixmap( start_xpm ), "Exit", toolBar );

    toolBar->addSeparator();
    toolBar->addAction( d_OpenWindow_Exit );

    return toolBar;
}


QToolBar *LAMPhDevices::toolBar_GET()
{
    MyToolBar *toolBar_GET = new MyToolBar( this );
    toolBar_GET->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    hBox_GET = new QWidget( toolBar_GET );



    nameDevices = new QLabel(tr("COMPorts and Devices"));
    receivedLabel = new QLabel(tr("Received"));
    sendLabel  = new QLabel(tr("Send"));

    for(int i=0; i<10; i++)
    {
        comboBox_Pairs[i] = new QComboBox();
        condition[i] = new QLabel;
        received[i] = new QLineEdit;
        send_txt[i] = new QLabel;
        send[i] = new QLineEdit;

        //comboBox_Pairs[i]->addItem(QString("COM %1 <-> COM %2 Device: Device %3 "));
        condition[i]->setText(QString("if received:"));
        received[i]->setText(QString(""));
        send_txt[i]->setText(QString("send:"));
        send[i]->setText(QString(""));

        for (int l = 0; l < 10; ++l) {
            comboBox_Pairs[i]->addItem(QString("COM %1 <-> COM %2 Device: Device %3 ").arg(10+2*l).arg(11+2*l).arg(l));
        }
        comboBox_Pairs[i]->setCurrentIndex(i);
        //comboBox_Pairs[i]->currentIndex()

    }

    QGridLayout *gridLayout = new QGridLayout( hBox_GET);

    gridLayout->addWidget(nameDevices, 0, 0);
    gridLayout->addWidget(receivedLabel, 0, 2);
    gridLayout->addWidget(sendLabel, 0, 4);

    for(int i=0; i<10; i++)
    {
        gridLayout->addWidget(comboBox_Pairs[i], i+1, 0);
        gridLayout->addWidget(condition[i], i+1, 1);
        gridLayout->addWidget(received[i], i+1, 2);
        gridLayout->addWidget(send_txt[i], i+1, 3);
        gridLayout->addWidget(send[i], i+1, 4);
    }

    send[0]->setText(QString("*"));
    received[0]->setText(QString("*"));
    send[1]->setText(QString("time"));
    received[1]->setText(QString("*"));
    send[2]->setText(QString("byte:00005555"));
    received[2]->setText(QString("*"));
    send[3]->setText(QString("byte:00005555"));
    received[3]->setText(QString("byte:00005555"));
    send[4]->setText(QString("*"));
    received[4]->setText(QString("byte:00005555"));
    send[5]->setText(QString("time"));
    received[5]->setText(QString("byte:00005555"));
    send[6]->setText(QString("*"));
    received[6]->setText(QString("*"));
    send[7]->setText(QString("0.0012,0.0023"));
    received[7]->setText(QString(":READ?"));
    send[8]->setText(QString("0.0012;0.0023"));
    received[8]->setText(QString(":READ?"));
    send[9]->setText(QString("*"));
    received[9]->setText(QString("*"));

    //gridLayout->addWidget(button_ReceivedData_Close,12,2);
    //gridLayout->addWidget(button_ReceivedData_Add,12,1);

    gridLayout->setContentsMargins(5,5,5,5);
    gridLayout->setVerticalSpacing(5);
    gridLayout->setHorizontalSpacing(5);

    toolBar_GET->addWidget( hBox_GET );
    return toolBar_GET;
}

void LAMPhDevices::toolBar_GET_show_data()
{
    /*label_ReceivedData[int_GET]->show();
    comboBox_Device[int_GET]->show();
    comboBox_Device_Functions[int_GET]->show();
    comboBox_Function_Parameters[int_GET]->show();
    lineEdit_NameData[int_GET]->show();
    checkBox_Device_Show[int_GET]->show();
    checkBox_Device_Text[int_GET]->show();
    checkBox_Device_DB[int_GET]->show();
    comboBox_ColorData[int_GET]->show();
    comboBox_SizeData[int_GET]->show();
    if (int_GET<20)int_GET++;*/
}

void LAMPhDevices::toolBar_GET_hide_data()
{
    /*if (int_GET>5) int_GET--;
    label_ReceivedData[int_GET]->hide();
    comboBox_Device[int_GET]->hide();
    comboBox_Device_Functions[int_GET]->hide();
    comboBox_Function_Parameters[int_GET]->hide();
    lineEdit_NameData[int_GET]->hide();
    checkBox_Device_Show[int_GET]->hide();
    checkBox_Device_Text[int_GET]->hide();
    checkBox_Device_DB[int_GET]->hide();
    comboBox_ColorData[int_GET]->hide();
    comboBox_SizeData[int_GET]->hide();*/
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
        QString s = QObject::tr("Port: ") + info.portName() + " "
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

void LAMPhDevices::getDataDll()
{

}




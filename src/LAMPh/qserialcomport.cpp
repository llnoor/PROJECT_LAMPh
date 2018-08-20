#include "qserialcomport.h"
#include "QDebug"

qserialcomport::qserialcomport()
{
    portCOM = new QSerialPort();
}

bool qserialcomport::checkPort(QString portQString, QString sendQString, QString respondQString)
{
    portCOM->setPortName(portQString);


    portCOM->setBaudRate(QSerialPort::Baud9600);
    portCOM->setStopBits(QSerialPort::OneStop);
    portCOM->setDataBits(QSerialPort::Data8);
    portCOM->setParity(QSerialPort::NoParity);
    portCOM->setFlowControl(QSerialPort::NoFlowControl);

    portCOM->

    portCOM->open(QIODevice::ReadWrite);

    if (portCOM->isOpen())
    {
        QByteArray ba;
        ba.resize(5);
        ba[0] = 0x55;
        ba[1] = 0x55;
        ba[2] = 0x00;
        ba[3] = 0x00;
        ba[4] = 0xaa;
        portCOM->write(ba);
        portCOM->waitForReadyRead(200);
        QByteArray data = portCOM->readAll();
        char *buff = data.data();
        //int buff_int[60];
        int buff_int_char[60];
        for (int l=0; l<30; l++){
            //buff_int[l]=buff[l];
            buff_int_char[l]=0;
            buff_int_char[l]= buff[l]  - '0';
        }
        int sn_appa =
                buff_int_char[12]* 10000000+
                buff_int_char[13]* 1000000+
                buff_int_char[14]* 100000+
                buff_int_char[15]* 10000+
                buff_int_char[16]* 1000+
                buff_int_char[17]* 100+
                buff_int_char[18]* 10+
                buff_int_char[19];
        QString str= QString::number(sn_appa);
        std::string result_string = str.toStdString();

        qDebug() << "data" << data;
        qDebug() << "str" << str;
    }
    portCOM->close();

    return true;
}

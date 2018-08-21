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
    portCOM->open(QIODevice::ReadWrite);


    QByteArray sendQByteArray;
    QByteArray respondQByteArray;

    if (portCOM->isOpen())
    {

        if (sendQString.contains("byte:", Qt::CaseInsensitive))
        {
            sendQString.remove("byte:");
            sendQByteArray = QByteArray::fromHex(sendQString.toLocal8Bit());
            portCOM->write(sendQByteArray);

        }else
        {
            portCOM->write(sendQString.toLatin1());
        }

        portCOM->waitForReadyRead(200);

        QByteArray data = portCOM->readAll();
        while (portCOM->waitForReadyRead(10))
            data += portCOM->readAll();

        qDebug().noquote() << "bytes: " << data.size() << " values: " << data.toHex();

        portCOM->close();

        if (respondQString.contains("byte:", Qt::CaseInsensitive))
        {
            respondQString.remove("byte:");
            respondQByteArray = QByteArray::fromHex(respondQString.toLocal8Bit());

            qDebug().noquote() << "respondQByteArray: " << respondQByteArray.size() << " values: " << respondQByteArray.toHex();

            for (int i=0; i<respondQByteArray.size(); i++)
            {
                if (respondQByteArray[i]!=data[i]) return false;
            }
            return true;
        }
        else
        {
            const QString dataQString = QString::fromUtf8(data);
            if (dataQString.contains(respondQString, Qt::CaseInsensitive))
            return true;
            else return false;
        }
    }
}

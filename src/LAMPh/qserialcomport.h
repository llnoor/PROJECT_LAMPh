#ifndef QSERIALCOMPORT_H
#define QSERIALCOMPORT_H

#include <string>
#include <QtSerialPort/QSerialPort>

class qserialcomport
{
public:
    qserialcomport();
    bool checkPort(QString portQString, QString sendQString, QString respondQString);

private:
    QSerialPort *portCOM;
};

#endif // QSERIALCOMPORT_H

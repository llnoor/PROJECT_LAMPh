#include "COM_KEITHLEY2000.h"

#include <stdio.h>

#include <string>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <QDebug>

#define NAME  "COM_KEITHLEY2000"
#define DEVICE "KEITHLEY2000"
//#define COMMANDS "KEITHLEY2000,*IDN?,KEITHLEY"; //nameofdevice,KEYcommand,respond
#define COMMANDS "KEITHLEY2000,byte:2A;49;44;4E;3F;0D;0A,byte:4B;45;49;54";

#define FOLDER  "Functions/"
#define TXT "_functions.txt"
#define FUNCTIONS "float getFloat(); char getUnit(); char getValue(); char getSN()"
#define INFO "The Lib for LAMPh to connect with KEITHLEY2000";

char cNewLine = '\n';

class ClassLAMPh
{
private:
    float result_float=0;
    char * unit;
    char * value;
    int i_test=0;
    QByteArray data;
    QSerialPort serialPortKEITHLEY2000;


public:
    ClassLAMPh( /*QString comPort_, QString comInfo_*/ ) // конструктор класса
    {
    //serialPortKEITHLEY2000 = QSerialPort();
        //connect(serialPortKEITHLEY2000, SIGNAL(readyRead()),this, SLOT(readData()));
        //readData(); /because connect does not work without QObject, I don't want to include QObject (but it will be necessary for MASTECH )
    }

    void readData()
    {
        serialPortKEITHLEY2000.write(":READ?\r\n");
        serialPortKEITHLEY2000.waitForReadyRead(10);
        QByteArray data = serialPortKEITHLEY2000.readAll();

        std::string result_tmp = data.toStdString();
        QString data_tmp = QString::fromStdString(result_tmp);
        //data_tmp.remove("\n");
        //data_tmp.remove("\r");


        qDebug() << "data_tmp" << data_tmp;
        result_float = data_tmp.toFloat();


    }


    const char* checkCOM( const char* const port, const char* const info ){
        std::string str = info;
        if (0 != str.find(DEVICE) ){
            //setCOM(port);
            return DEVICE;
        }
    }


    bool setCOM(){

        for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()){
            qDebug() << "serialPortKEITHLEY2000" << info.portName();
            if (!info.isBusy())
            {
                qDebug() << info.portName();
                serialPortKEITHLEY2000.setPortName(info.portName());
                serialPortKEITHLEY2000.setBaudRate(QSerialPort::Baud9600);
                serialPortKEITHLEY2000.setStopBits(QSerialPort::OneStop);
                serialPortKEITHLEY2000.setDataBits(QSerialPort::Data8);
                serialPortKEITHLEY2000.setParity(QSerialPort::NoParity);
                serialPortKEITHLEY2000.setFlowControl(QSerialPort::NoFlowControl);
                serialPortKEITHLEY2000.open(QIODevice::ReadWrite);

                QByteArray ba;
                ba.resize(7);
                ba[0] = 0x2a;
                ba[1] = 0x49;
                ba[2] = 0x44;
                ba[3] = 0x4e;
                ba[4] = 0x3f;
                ba[5] = 0x0d;
                ba[6] = 0x0a; // "*IDN?"

                QByteArray ba_check;
                ba_check.resize(4);
                ba_check[0] = 0x4b;
                ba_check[1] = 0x45;
                ba_check[2] = 0x49;
                ba_check[3] = 0x54;


                serialPortKEITHLEY2000.waitForBytesWritten(300);

                serialPortKEITHLEY2000.write("*IDN?\r\n");

                serialPortKEITHLEY2000.waitForReadyRead(300);

                QByteArray data;
                data= serialPortKEITHLEY2000.readAll();
                while (serialPortKEITHLEY2000.waitForReadyRead(10))
                    data += serialPortKEITHLEY2000.readAll();


                std::string result_tmp = data.toStdString();
                QString data_tmp = QString::fromStdString(result_tmp);
                qDebug() << "data_tmp: "  << data_tmp;

                qDebug().noquote() << "bytes: " << data.size() << " values: " << data.toHex();

                bool match_bool =true;
                for (int i=0; i<ba_check.size(); i++)
                {
                    if (ba_check[i]!=data[i]){
                        serialPortKEITHLEY2000.close();
                        match_bool = false;
                    }
                }
                if (match_bool)
                {
                    serialPortKEITHLEY2000.write("*RST\r\n");
                    serialPortKEITHLEY2000.write(":SENS:FUNC 'VOLT:DC'\r\n");
                    serialPortKEITHLEY2000.write(":SENS:FRES:DIG 6\r\n");
                    serialPortKEITHLEY2000.write(":READ?\r\n");
                    return true;
                }

            }
        }
        return false;
    }

    const char* getSN(){
        return "KEITHLEY2000TEST";
    }

    float getFloat(){
        /*serialPortKEITHLEY2000.write(":READ?\r\n");
        //readData();

        serialPortKEITHLEY2000.waitForBytesWritten(100);
        serialPortKEITHLEY2000.waitForReadyRead(100);
        QByteArray data = serialPortKEITHLEY2000.readAll();
        std::string result_tmp = data.toStdString();
        QString data_tmp = QString::fromStdString(result_tmp);
        data_tmp.remove("\n");
        data_tmp.remove("\r");
        result_float = data_tmp.toFloat();
        qDebug() << data;*/

        //qDebug() << "KEITHLEY2000 getFloat";
        return result_float;

    }

    const char* getUnit(){
        unit = "V";
        return unit;
    }

    const char* getValue(){
        return unit; //but should return  float+unit, maybe delete this function, because this one not necessary
    }

    bool setCh(float float_data){
        if (result_float=result_float*float_data) true; else false;
    }
};

ClassLAMPh classLAMPh[5];

const char* getInfo() {
    createFile();
    return INFO;
}

const char* DLLMain() {
    createFile();
    return INFO;
}

const char* getFunctions() {
    createFile();
    return FUNCTIONS;
}

bool createFile(){
    FILE *file;
    if ((file = fopen(FOLDER NAME TXT,"w+")) == NULL) return false;
    fprintf(file,"%s%c", FUNCTIONS, cNewLine);
    fclose(file);
    return true;
}

const char* getCOMcommands() {
    return COMMANDS;
}

const char* getName() {
    return DEVICE;
}

void setNewDevice(int number_of_devices){
    //delete this function
}

bool setPORT(int number_of_device){
    return classLAMPh[number_of_device].setCOM();
}

float getFloat(int number_of_device){
    return classLAMPh[number_of_device].getFloat();
}

const char* getUnit(int number_of_device){
    return classLAMPh[number_of_device].getUnit();
}

const char* getValue(int number_of_device){
    return classLAMPh[number_of_device].getValue();
}

bool setCh(int number_of_device, float float_data){
    return classLAMPh[number_of_device].setCh(float_data);
}











const char* checkCOM( const char* const port, const char* const info ){
    std::string str = info;
    if (0 != str.find(DEVICE) ){
        //setCOM(port);
        return DEVICE;
    }

}




void readData(int number_of_device){
   classLAMPh[number_of_device].readData();
}


const char* getSN(int number_of_device){

    /*if (port_appa->isOpen()) readData();;
    else return 0;

    char *buff = data.data();
    int buff_int_char[60];
    for (int l=0; l<30; l++){
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
    return str.toStdString();*/
    return "KEITHLEY2000TEST";
}

/*float getFloat(){

    if (port_appa->isOpen()) readData();;
    else return 0;

    char *buff = data.data();
    int buff_int[60];

    for (int l=0; l<60; l++){
        buff_int[l]=buff[l];
    }

    int buff_int_34=buff_int[34];
    int buff_int_35=buff_int[35];
    if (buff_int[34]<0)
    {
        buff_int_34= buff_int[34]+ 256;
    }
    if (buff_int[35]<0)
    {
        buff_int_35= buff_int[35]+ 256;
    }

    result_float= buff_int_34 +(buff_int_35*256);

    if (buff_int[36]<0)
    {
        result_float=result_float*(-1);
    }

    switch (buff_int[37])
    {
    case 0: break;
    case 1: result_float=result_float*(0.1);break;
    case 2: result_float=result_float*(0.01);break;
    case 4: result_float=result_float*(0.001);break;
    case 8: result_float=result_float*(0.0001);break;
    }

    return result_float;

    result_float = (float)i_test++;
    return result_float;
}*/

/*const char* getUnit(){
    unit = "V";
    return unit;
}

const char* getValue(){
    return unit; //but should return  float+unit, maybe delete this function, because this one not necessary
}*/

/*void inputTest( const char* const str ) {
    std::cout << str << std::endl;
}

const char* outputTest() {
    return "Hello from LAMPhLib!";
}

QString oddUpper(const QString& str)
{
    QString strTemp;

    for (int i = 0; i < str.length(); ++i) {
        strTemp += (i % 2) ? str.at(i) : str.at(i).toUpper();
    }

    return strTemp;
}*/




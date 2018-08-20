#include "COM_APPA205.h"

#include <stdio.h>

#include <string>
#include <QtSerialPort/QSerialPort>

#include <QDebug>

#define NAME  "COM_APPA205"
#define DEVICE "APPA205"
#define COMMANDS "APPA205,byte:55;55;00;00;AA,byte:55;55;00"; //nameofdevice,KEYcommand,respond
#define FOLDER  "Functions/"
#define TXT "_functions.txt"
#define FUNCTIONS "float getSeconds();char checkPORT(char,char);void setPORT(char);float getFloat();char getUnit();char getValue();char getSN()"
#define TYPE_FUNCTIONS "char checkPORT(char,char);void setPORT(char);float getFloat();char getUnit();char getValue();char getSN()"
#define INFO "The Lib for LAMPh to connect with APPA205";

char cNewLine = '\n';

class ClassLAMPh
{
private:
    float result_float=0;
    char * unit;
    char * value;
    int i_test=0;
    QByteArray data;
    QSerialPort serialPortAPPA205;


public:
    ClassLAMPh( /*QString comPort_, QString comInfo_*/ ) // конструктор класса
    {
    //serialPortAPPA205 = QSerialPort();
        //connect(serialPortAPPA205, SIGNAL(readyRead()),this, SLOT(readData()));
        //readData(); /because connect does not work without QObject, I don't want to include QObject (but it will be necessary for MASTECH )
    }

    void readData()
    {
        while (! serialPortAPPA205.atEnd()) {
                QByteArray dataByteArray = serialPortAPPA205.read(100);
                std::string result_tmp = dataByteArray.toStdString();
                QString data_tmp = QString::fromStdString(result_tmp);
                result_float = data_tmp.toFloat();
        }
    }


    const char* checkCOM( const char* const port, const char* const info ){
        std::string str = info;
        if (0 != str.find(DEVICE) ){
            //setCOM(port);
            return DEVICE;
        }
    }

    bool setCOM( const char* const port ){        
        serialPortAPPA205.setPortName(port);
        serialPortAPPA205.setBaudRate(QSerialPort::Baud9600);
        serialPortAPPA205.setStopBits(QSerialPort::OneStop);
        serialPortAPPA205.setDataBits(QSerialPort::Data8);
        serialPortAPPA205.setParity(QSerialPort::NoParity);
        serialPortAPPA205.setFlowControl(QSerialPort::NoFlowControl);
        serialPortAPPA205.open(QIODevice::ReadWrite);

        if (serialPortAPPA205.isOpen()) return true; else false;
    }


    const char* getSN(){
        return "APPA205TEST";
    }

    float getFloat(){

        QByteArray ba;
        ba.resize(5);
        ba[0] = 0x55;
        ba[1] = 0x55;
        ba[2] = 0x00;
        ba[3] = 0x00;
        ba[4] = 0xaa;
        serialPortAPPA205.write(ba);
        //readData();
        QByteArray data = serialPortAPPA205.readAll();
        serialPortAPPA205.waitForReadyRead(300);
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

        if (serialPortAPPA205.isOpen())
        {
            return result_float;
        }else
        {
            return 0;
        }

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


void setNewDevice(int number_of_devices){
    //delete this function
}

bool setPORT(int number_of_device, const char* const port ){
    return classLAMPh[number_of_device].setCOM(port);
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


/*const char* checkCOM( const char* const port, const char* const info ){
    std::string str = info;
    if (0 != str.find(DEVICE) ){
        //setCOM(port);
        return DEVICE;
    }

}*/

bool checkPORT(const char* const port ){

    QSerialPort serialPortAPPA205;
    serialPortAPPA205.setPortName(port);
    serialPortAPPA205.setBaudRate(QSerialPort::Baud9600);
    serialPortAPPA205.setStopBits(QSerialPort::OneStop);
    serialPortAPPA205.setDataBits(QSerialPort::Data8);
    serialPortAPPA205.setParity(QSerialPort::NoParity);
    serialPortAPPA205.setFlowControl(QSerialPort::NoFlowControl);
    serialPortAPPA205.open(QIODevice::ReadWrite);
    QByteArray ba;
    ba.resize(5);
    ba[0] = 0x55;
    ba[1] = 0x55;
    ba[2] = 0x00;
    ba[3] = 0x00;
    ba[4] = 0xaa;
    serialPortAPPA205.write(ba);

    serialPortAPPA205.waitForReadyRead(300);
    QByteArray data = serialPortAPPA205.readAll();

    qDebug() << "COM_APPA205:data" << data;

    serialPortAPPA205.close();

    return true;
}


void readData(){
    /*QByteArray ba;
    ba.resize(5);
    ba[0] = 0x55;
    ba[1] = 0x55;
    ba[2] = 0x00;
    ba[3] = 0x00;
    ba[4] = 0xaa;
    port_appa->write(ba);
    port_appa->waitForBytesWritten(300);

    data = port_appa->readAll();*/
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
    return "APPA205TEST";
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




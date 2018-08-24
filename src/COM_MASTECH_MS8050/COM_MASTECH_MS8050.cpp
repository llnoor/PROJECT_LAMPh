#include "COM_MASTECH_MS8050.h"

#include <stdio.h>

#include <string>
#include <QSerialPort>
#include <QSerialPortInfo>

#include <QDebug>

#define NAME  "COM_MASTECH_MS8050"
#define DEVICE "MASTECH_MS8050"
#define COMMANDS "MASTECH_MS8050,*,MASTECH_MS8050"; //nameofdevice,KEYcommand,respond
#define FOLDER  "Functions/"
#define TXT "_functions.txt"
#define FUNCTIONS "float getFloat()"
#define INFO "The Lib for LAMPh to connect with MASTECH_MS8050";

char cNewLine = '\n';

class ClassLAMPh
{
private:
    float result_float=0;
    char * unit;
    char * value;
    int i_test=0;
    QByteArray data;
    QSerialPort serialPortMASTECH_MS8050;


public:
    ClassLAMPh( /*QString comPort_, QString comInfo_*/ ) // конструктор класса
    {
    //serialPortMASTECH_MS8050 = QSerialPort();
        //connect(serialPortMASTECH_MS8050, SIGNAL(readyRead()),this, SLOT(readData()));
        //readData(); /because connect does not work without QObject, I don't want to include QObject (but it will be necessary for MASTECH )
    }

    void readData()
    {
        serialPortMASTECH_MS8050.waitForReadyRead(200);
        QByteArray data = serialPortMASTECH_MS8050.readAll();
        int yt=0;
        while (serialPortMASTECH_MS8050.waitForReadyRead(10))
        {
            data += serialPortMASTECH_MS8050.readAll();
            yt++;
            qDebug("%d", yt);
            //if (yt>5) break;
        }

        qDebug().noquote() << "bytes: " << data.size() << " values: " << data.toHex();

        char *buff = data.data();
        int buff_int[24];
        int right_array[14];

        for (int l=0; l<24; l++){
            buff_int[l]=buff[l];
            //qDebug("buff_int[%d]: %d", l, buff_int[l]);
        }


        int kk_start[4];
        kk_start[0]=0;
        kk_start[1]=0;
        kk_start[2]=0;

        int number_of_kk=0;
        for (int kk=0; kk<24; kk++)
        {
            if (-97<buff_int[kk] and -90>buff_int[kk])
            {
                kk_start[number_of_kk]=kk;
                number_of_kk++;
            }

        }


        for (int l=0; l<14; l++){
            right_array[l]=buff_int[l+kk_start[0]];
        }

        if (3<number_of_kk) right_array[0]=0;  //break;

    //qDebug("kk: %d, [0]: %d,[1]: %d,[2]: %d,[3]: %d,[4]: %d,[5]: %d,[6]: %d,[7]: %d,[8]: %d,",kk_start[0], right_array[0],right_array[1],right_array[2],right_array[3],right_array[4],right_array[5],right_array[6],right_array[7],right_array[8]);


        if ( -97<right_array[0] and -90>right_array[0])
        {


            result_float=right_array[4]*10000+
                        right_array[5]*1000+
                        right_array[6]*100+
                        right_array[7]*10+
                        right_array[8];

            switch (right_array[1])
            {
            case 0:
                if (-96==right_array[0]) result_float=result_float*0.0001;
                if (-95==right_array[0]) result_float=result_float*0.001;
                if (-94==right_array[0]) result_float=result_float*0.01;
                if (-93==right_array[0]) result_float=result_float*0.1;
                break;
            case 1:
                if (-96==right_array[0]) result_float=result_float*0.0001;
                if (-95==right_array[0]) result_float=result_float*0.001;
                if (-94==right_array[0]) result_float=result_float*0.01;
                if (-93==right_array[0]) result_float=result_float*0.1;
                break;
            case 3:
                if (-96==right_array[0]) result_float=result_float*0.001;
                if (-95==right_array[0]) result_float=result_float*0.01;
                break;
            case 6:
                //result_float=result_float;
                break;
            case 7:
                //result_float=result_float;
                break;
            case 8:
                if (-96==right_array[0]) result_float=result_float*0.01;
                if (-95==right_array[0]) result_float=result_float*0.1;
                if (-94==right_array[0]) result_float=result_float*1;
                if (-93==right_array[0]) result_float=result_float*10;
                if (-92==right_array[0]) result_float=result_float*10000;
                if (-91==right_array[0]) result_float=result_float*1000000;
                break;
            case 11:
                if (-96==right_array[0]) result_float=result_float*0.01;
                if (-95==right_array[0]) result_float=result_float*0.1;
                break;
            case 14:
                if (-96==right_array[0]) result_float=result_float*0.001;
                if (-95==right_array[0]) result_float=result_float*0.01;
                break;
            case 17:
                if (-96==right_array[0]) result_float=result_float*0.0001;
                if (-95==right_array[0]) result_float=result_float*0.001;
                break;
            }
        }

        result_float=result_float*0.001;


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
            qDebug() << "serialPortMASTECH_MS8050" <<info.portName();
            if (!info.isBusy())
            {
                qDebug() << info.portName();
                serialPortMASTECH_MS8050.setPortName(info.portName());
                serialPortMASTECH_MS8050.setBaudRate(QSerialPort::Baud2400);
                serialPortMASTECH_MS8050.setStopBits(QSerialPort::OneStop);
                serialPortMASTECH_MS8050.setDataBits(QSerialPort::Data8);
                serialPortMASTECH_MS8050.setParity(QSerialPort::EvenParity);
                //serialPortMASTECH_MS8050.setParity(QSerialPort::NoParity);
                serialPortMASTECH_MS8050.setFlowControl(QSerialPort::NoFlowControl);
                serialPortMASTECH_MS8050.open(QIODevice::ReadWrite);
                serialPortMASTECH_MS8050.waitForReadyRead(200);

                QByteArray data = serialPortMASTECH_MS8050.readAll();
                //int yt=0;
                while (serialPortMASTECH_MS8050.waitForReadyRead(10))
                {
                    data += serialPortMASTECH_MS8050.readAll();
                    //yt++;
                    //qDebug("%d", yt);
                    //if (yt>5) break;
                }

                //qDebug().noquote() << "bytes: " << data.size() << " values: " << data.toHex();
                QString dataQString(data.toHex());
                // qDebug() << "dataQString" << dataQString;


                if (dataQString.contains("00007f00", Qt::CaseInsensitive))
                {
                   return true;
                }else
                {
                    serialPortMASTECH_MS8050.close();
                }
                /*QByteArray ba_check;
                ba_check.resize(4);
                ba_check[0] = 0x00;
                ba_check[1] = 0x00;
                ba_check[2] = 0x7F;
                ba_check[2] = 0x00;
                bool match_bool =true;
                for (int i=0; i<ba_check.size(); i++)
                {
                    if (ba_check[i]!=data[i]){
                        serialPortMASTECH_MS8050.close();
                        match_bool = false;
                    }
                }
                if (match_bool) return true;*/
            }
        }
        return false;
    }


    const char* getSN(){
        return "MASTECH_MS8050TEST";
    }

    float getFloat(){
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

void readData(int number_of_device){
   classLAMPh[number_of_device].readData();
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
    return "MASTECH_MS8050TEST";
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




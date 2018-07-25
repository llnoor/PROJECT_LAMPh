#include "USB_VIRTUAL.h"

#include <stdio.h>
#include <cmath>
#include <iostream>
#include <ctime>
#include <cstdlib>


#include <QDateTime>

#include <string>
#include <QtSerialPort/QSerialPort>

#include <QDebug>

#define NAME  "USB_VIRTUAL"
#define DEVICE "VIRTUAL"
#define COMMANDS "UBS_VIRTUAL,None,None"; //nameofdevice,KEYcommand,respond
#define FOLDER  "Functions/"
#define TXT "_functions.txt"
#define FUNCTIONS "float getFloat();char getUnit();char getValue();char getSN();float getSin();float getCos();float getSawtooth();float getLine();float getRand();float getSinP(float);void setAlfa(float)"
#define INFO "The Lib for LAMPh to connect with VIRTUAL";

/*For keithley
 * if we choose "float get_R-4W" (in comboBox_Device_Functions), LAMPh should send "V" to this lib and this lib will change settings in device (keithley) like "write(":SENS:FUNC 'FRES'\r\n")"
 * if we choose "float get_R" (in comboBox_Device_Functions), LAMPh should send "V" to this lib and this lib will change settings in device (keithley) like "write(":SENS:FUNC 'RES'\r\n")"
 * and so on
 *
 * */


char cNewLine = '\n';

class ClassLAMPh
{
private:
    float result_float=0;
    char * unit;
    char * value;
    int i_test=0;
    float alfa=0;
    QByteArray data;
    double first =QDateTime::currentDateTime().toTime_t();

public:
    ClassLAMPh( /*QString comPort_, QString comInfo_*/ ) // конструктор класса
    {

    }

    void readData()
    {
        //read data;

    }

    bool checkUSB( ){
        return true; //ready to work
    }

    bool setUSB( const char* const port ){        
        return true;
    }

    const char* getSN(){
        return "VIRTUALTEST";
    }

    float getFloat(){

        //result_float = data_tmp.toFloat();
        //qDebug() << data;

        double second=QDateTime::currentDateTime().toTime_t();
        double x = second - first;

        result_float = sin(x/10);
        return result_float;

    }


    float getSin(){

        //result_float = data_tmp.toFloat();
        //qDebug() << data;

        double second=QDateTime::currentDateTime().toTime_t();
        double x = second - first;


        result_float = rand()%10;
        result_float = result_float/100;

        result_float = result_float+sin(x/10);

        result_float = result_float + alfa;

        return result_float;

    }

    float getSinP(float parameter){

        //result_float = data_tmp.toFloat();
        //qDebug() << data;

        double second=QDateTime::currentDateTime().toTime_t();
        double x = second - first;


        result_float = rand()%10;
        result_float = result_float/100;

        result_float = result_float+sin(x/10);

        result_float = result_float + parameter + alfa;

        return result_float;
    }

    void setAlfa(float parameter){

        alfa = parameter;
    }

    float getCos(){


        double second=QDateTime::currentDateTime().toTime_t();
        double x = second - first;


        result_float = rand()%10;
        result_float = result_float/100;

        result_float = result_float+cos(x/10);

        result_float = result_float + alfa;

        return result_float;

    }

    float getSawtooth(){
        double second=QDateTime::currentDateTime().toTime_t();
        double x = second - first;


        result_float = rand()%10;
        result_float = result_float/100;

        result_float = result_float+cos(x/10)+0.5;
        return result_float;

    }

    float getLine(){

        //result_float = data_tmp.toFloat();
        //qDebug() << data;

        double second=QDateTime::currentDateTime().toTime_t();
        double x = second - first;

        result_float = x;
        return result_float;

    }

    float getRand(){

        result_float = rand()%100;
        result_float=result_float/100;
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

const char* getName() {
    return DEVICE;
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

const char* getUSBcommands() {
    return COMMANDS;
}

void setNewDevice(int number_of_devices){
    //delete this function
}

bool setUSBPORT(int number_of_device, const char* const port ){
    return classLAMPh[number_of_device].setUSB(port);
}

float getFloat(int number_of_device){
    return classLAMPh[number_of_device].getFloat();
}

float getSin(int number_of_device){
    return classLAMPh[number_of_device].getSin();
}

float getSinP(int number_of_device, float parameter){
    return classLAMPh[number_of_device].getSinP(parameter);
}

float getCos(int number_of_device){
    return classLAMPh[number_of_device].getCos();
}

float getSawtooth(int number_of_device){
    return classLAMPh[number_of_device].getSawtooth();
}

float getLine(int number_of_device){
    return classLAMPh[number_of_device].getLine();
}

float getRand(int number_of_device){
    return classLAMPh[number_of_device].getRand();
}

void setAlfa(int number_of_device, float parameter){
    classLAMPh[number_of_device].setAlfa(parameter);
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


bool checkUSB( int number_of_device ){
   return classLAMPh[number_of_device].checkUSB();
}

void readData(){
    //important part of cpp
}

const char* getSN(int number_of_device){
    return "VIRTUAL_TEST";
}



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




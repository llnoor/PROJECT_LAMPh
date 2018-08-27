#include "USB_AKTAKOM_ATH1533.h"

#include <stdio.h>
#include <cmath>
#include <iostream>
#include <ctime>
#include <cstdlib>


#include <QDateTime>


#include <string>
#include <QtSerialPort/QSerialPort>

#include <QDebug>
#include <QThread>

#define NAME  "USB_AKTAKOM_ATH1533"
#define DEVICE "AKTAKOM_ATH1533"
#define FOLDER  "Functions/"
#define TXT "_functions.txt"
#define FUNCTIONS "float getVoltage();float getCurrent();void setVoltage(float);void setCurrent(float)"
#define INFO "The Lib for LAMPh to connect with AKTAKOM_ATH1533";


char cNewLine = '\n';

class ClassLAMPh
{
private:
    int socketIndex;
    char resourceName[256];
    ViSession instrumentHandle;
    float result_float=0;
    QByteArray data;
    float voltageFloat;
    float currentFloat;

    double voltageDouble;
    double currentDouble;


public:
    ClassLAMPh( /*QString comPort_, QString comInfo_*/ ) // конструктор класса
    {

    }

    void readData()
    {
        //read data;

    }

    bool checkUSB( ){
        return true;
    }

    bool setUSBPORT(){

        ATH1535_AddSocket (&socketIndex, "127.0.0.1", 1024, 5000, "AULNetPass", 0);
        sprintf (resourceName, "AULNET::%i::ATH-1533 #*", socketIndex);
        QThread::msleep(100);
        ATH1535_init (resourceName, &instrumentHandle, 0, 0, NULL, NULL, NULL);
        QThread::msleep(100);
        ATH1535_SetControl (instrumentHandle, 0, 1);
        QThread::msleep(100);
        ATH1535_SetOutputEnabled(instrumentHandle,0,1);
        QThread::msleep(100);
        ATH1535_SetCurrent (instrumentHandle, 0, 0.7);
        QThread::msleep(100);
        ATH1535_SetVoltage (instrumentHandle, 0, 0.7);
        QThread::msleep(100);

        ViByte valViByte;
        ATH1535_GetStatus (instrumentHandle, 0,&valViByte);
        float valFloat = valViByte;

        qDebug() << "valFloat" << valFloat;

        if ((70>valFloat) and (63<valFloat))
        {
            qDebug() << "true67";
            return true;
        }
        else{

            qDebug() << "false67";
            return false;
        }
    }

    const char* getSN(){
        return "VIRTUALTEST";
    }

    float getVoltage(){
        ATH1535_GetVoltage (instrumentHandle,0, &voltageDouble);
        voltageFloat = voltageDouble;
        return voltageFloat;
    }

    float getCurrent(){
        ATH1535_GetCurrent (instrumentHandle,0, &currentDouble);
        currentFloat = currentDouble;
        return currentFloat;
    }



    void setVoltage(float parameter){
            ATH1535_SetVoltage (instrumentHandle, 0, parameter);
    }

    void setCurrent(float parameter){
            ATH1535_SetCurrent (instrumentHandle, 0, parameter);
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

bool setUSBPORT(int number_of_device){
    return classLAMPh[number_of_device].setUSBPORT();
}

float getVoltage(int number_of_device){
    return classLAMPh[number_of_device].getVoltage();
}

float getCurrent(int number_of_device){
    return classLAMPh[number_of_device].getCurrent();
}

void setVoltage(int number_of_device, float parameter){
    classLAMPh[number_of_device].setVoltage(parameter);
}

void setCurrent(int number_of_device, float parameter){
    classLAMPh[number_of_device].setCurrent(parameter);
}


bool checkUSB( int number_of_device ){
    return classLAMPh[number_of_device].setUSBPORT();
}

const char* getUnit(int number_of_device){
    return "AKTAKOM";
}

void readData(){
    //important part of cpp
}

const char* getSN(int number_of_device){
    return "VIRTUAL_TEST";
}




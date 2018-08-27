#include "LAMPh_ATIME.h"

#include <stdio.h>
#include <cmath>
#include <iostream>
#include <ctime>
#include <cstdlib>


#include <QDateTime>

#include <string>
#include <QtSerialPort/QSerialPort>

#include <QDebug>

#define NAME  "LAMPh_ATIME"
#define DEVICE "ATIME"
#define COMMANDS "LAMPh_ATIME,None,None"; //nameofdevice,KEYcommand,respond
#define FOLDER  "Functions/"
#define TXT "_functions.txt"
#define FUNCTIONS "float getSeconds();float getSecondsFromNull();float getMinutes();float getMinutesFromNull();float getHours();float getHoursfromNull();float getDays();float getDaysFromNull()"
#define INFO "The Lib for LAMPh to connect with ATIME";

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
    QByteArray data;
    double first =QDateTime::currentDateTime().toTime_t();
	double second=QDateTime::currentDateTime().toTime_t();
    double x = second - first;
    double x_s = second - first;
    double x_m = second - first;
    double x_h = second - first;

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

	
    float get_s(){
        second=QDateTime::currentDateTime().toTime_t();
        x = second - first;
        return x;
    }

    float get_s2(){
        second=QDateTime::currentDateTime().toTime_t();
        if ( x_s > ((second - first)-10))
        {
            x_s = second - first;
        }
        else
        {
            first=QDateTime::currentDateTime().toTime_t();
            second=QDateTime::currentDateTime().toTime_t();
            x_s = 0;
        }
        return x_s;
    }


    float getSeconds(){
        return get_s();
	}
	
    float getSecondsFromNull(){
        return get_s2();
	}

    float getMinutes(){
        result_float = (get_s()/60);
        return result_float;
    }
		
    float getMinutesFromNull(){
        result_float = (get_s2()/60);
        return result_float;
    }

    float getHours(){
        result_float = (get_s()/(60*60));
        return result_float;
	}
    float getHoursfromNull(){
        result_float = (get_s2()/(60*60));
        return result_float;
	}

    float getDays(){
        result_float = (get_s()/(60*60*24));
        return result_float;
	}
    float getDaysFromNull(){
        result_float = (get_s2()/(60*60*24));
        return result_float;
	}
	
    bool setCh(float float_data){
        if (result_float=result_float*float_data) true; else false;
    }
	
	const char* getUnit(){
        unit = "Time";
        return unit;
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


bool setUSBPORT(int number_of_device, const char* const port ){
    return classLAMPh[number_of_device].setUSB(port);
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


float getSeconds(int number_of_device){
	return classLAMPh[number_of_device].getSeconds();
}
float getSecondsFromNull(int number_of_device){
	return classLAMPh[number_of_device].getSecondsFromNull();
}

float getMinutes(int number_of_device){
	return classLAMPh[number_of_device].getMinutes();
}
float getMinutesFromNull(int number_of_device){
	return classLAMPh[number_of_device].getMinutesFromNull();
}

float getHours(int number_of_device){
	return classLAMPh[number_of_device].getHours();
}
float getHoursfromNull(int number_of_device){
	return classLAMPh[number_of_device].getHoursfromNull();
}

float getDays(int number_of_device){
	return classLAMPh[number_of_device].getDays();
}
float getDaysFromNull(int number_of_device){
	return classLAMPh[number_of_device].getDaysFromNull();
}

const char* getUnit(int number_of_device){
    return classLAMPh[number_of_device].getUnit();
}






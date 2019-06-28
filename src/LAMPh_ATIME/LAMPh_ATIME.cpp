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
#define INFO "The Lib for LAMPh to connect with ATIME"
#define NUMBER 2 //Only one device can be connected by this Lib (this value can be changed)
#define NONE "None"

/*For keithley
 * if we choose "float get_R-4W" (in comboBox_Device_Functions), LAMPh should send "V" to this lib and this lib will change settings in device (keithley) like "write(":SENS:FUNC 'FRES'\r\n")"
 * if we choose "float get_R" (in comboBox_Device_Functions), LAMPh should send "V" to this lib and this lib will change settings in device (keithley) like "write(":SENS:FUNC 'RES'\r\n")"
 * and so on
 *
 * */

class ClassLAMPh
{
private:

    char * status_char;
    char * unit_char;

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

    float result_floatD = 0;
    float parameterD = 0;
    float result_floatE = 0;
    float parameterE = 0;
    float result_floatF = 0;
    float parameterF = 0;

    float parameterG = 0;
    float parameterH = 0;
    float parameterI = 0;

    enum message {
    nothingCritical = 0,  		//nothing critical
    criticError = 1,
    criticLowTemp = 2,  		//Critical Low Temperature and so on
    criticHighTemp = 3,
    criticLowMField = 4,
    criticHighMField = 5,
    criticLowPressure = 6,
    criticHighPressure = 7,
    criticLowCurrent = 8,
    criticHighCurrent = 9,
    criticLowVoltage = 10,
    criticHighVoltage = 11,
    criticWeakSignal = 12,
    criticOverloadedSignal = 13,
    criticWeakMWSignal = 14,
    criticOverloadedMWSignal = 15,
    criticWeakModulationSignal = 16,
    criticOverloadedModulationSignal = 17,
    criticLowParameter1 = 18,
    criticHighParameter1 = 19,
    criticLowParameter2 = 20,
    criticHighParameter2 = 21,
    criticLowParameter3 = 22,
    criticHighParameter3 = 23};

public:
    ClassLAMPh( /*QString comPort_, QString comInfo_*/ ) // конструктор класса
    {
        status_char = "None";
        unit_char = "None";
    }

    bool connectL(){
        return true;
    }

    const char* getStatus(){
        return status_char; //it can be enum message
    }

    const char* getUnit(){
        return unit_char;
    }

    void readData(){

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

    const char* infoL(){
        QString strInfo = "nothing"; //

            return strInfo.toStdString().c_str();
    }

    bool startL(){
    //if ((result_floatA = 0) and (result_floatB = 0) /* and so on*/) return true; //YES!!! it is result_floatA = 0, because we want reset all parameters
    //else return false;
    }

    bool stopL(){
        return true;
    }

    bool pauseL(){
        return true;
    }

    bool exitL(){
        return true;
    }

    bool is_digits(const std::string &str) {
        return str.find_first_not_of("0123456789") == std::string::npos; //Searches the string for the first character that does not match any of the characters specified in its arguments.
    }

    bool sendMessage( const char* const char_str){
        std::string str(char_str);
        if (is_digits(str)){ //is it digits?
            //show one of critical messages
            switch(atoi(str.c_str())){ //it is int
                case(nothingCritical):/*Something*/ return true; break;
                case(criticError):/*Something*/ return true; break;
                case(criticLowTemp):/*Something*/ return true; break;
                case(criticHighTemp):/*Something*/ return true; break;
                case(criticLowMField):/*Something*/ return true; break;
                case(criticHighMField):/*Something*/ return true; break;
                case(criticLowPressure):/*Something*/ return true; break;
                case(criticHighPressure):/*Something*/ return true; break;
                case(criticLowCurrent):/*Something*/ return true; break;
                case(criticHighCurrent):/*Something*/ return true; break;
                case(criticLowVoltage):/*Something*/ return true; break;
                case(criticHighVoltage):/*Something*/ return true; break;
                case(criticWeakSignal):/*Something*/ return true; break;
                case(criticOverloadedSignal):/*Something*/ return true; break;
                case(criticWeakMWSignal):/*Something*/ return true; break;
                case(criticOverloadedMWSignal):/*Something*/ return true; break;
                case(criticWeakModulationSignal):/*Something*/ return true; break;
                case(criticOverloadedModulationSignal):/*Something*/ return true; break;
                case(criticLowParameter1):/*Something*/ return true; break;
                case(criticHighParameter1):/*Something*/ return true; break;
                case(criticLowParameter2):/*Something*/ return true; break;
                case(criticHighParameter2):/*Something*/ return true; break;
                case(criticLowParameter3):/*Something*/ return true; break;
                case(criticHighParameter3):/*Something*/ return true; break;
                }
        }else{
            //show strMessage
            return true;
        }
    }

    const char* getRowName(int row){
        switch(row){
        case(0): return ""; break;
        case(1): return ""; break;
        case(2): return ""; break;
        case(3): return ""; break;
        case(4): return ""; break;
        }
    }

    const char* getButtonName (int column, int row){
        switch(row){
        case(0): //Sensitivity
                switch(column){
                case(0): return ""; break;
                case(1): return ""; break;
                case(2): return ""; break;
                case(3): return ""; break;
                case(4): return ""; break;
                }
                break;
        case(1): //Time constant
                switch(column){
                case(0): return ""; break;
                case(1): return ""; break;
                case(2): return ""; break;
                case(3): return ""; break;
                case(4): return ""; break;
                }
                break;
        case(2): //Phase
                switch(column){
                case(0): return ""; break;
                case(1): return ""; break;
                case(2): return ""; break;
                case(3): return ""; break;
                case(4): return ""; break;
                }
                break;
        case(3): //Filters
                switch(column){
                case(0): return ""; break;
                case(1): return ""; break;
                case(2): return ""; break;
                case(3): return ""; break;
                case(4): return ""; break;
                }
                break;
        case(4): //Config
                switch(column){
                case(0): return ""; break;
                case(1): return ""; break;
                case(2): return ""; break;
                case(3): return ""; break;
                case(4): return ""; break;
                }
                break;
        }
    }

    char array[33];

    const char* returnRowData(int row){
        switch(row){
        case(0): sprintf(array, "%5.2f", 0); return array; break;
        case(1): sprintf(array, "%5.2f", 0); return array; break;
        case(2): sprintf(array, "%5.2f", 0); return array; break;
        case(3): sprintf(array, "%5.2f", 0); return array; break;
        case(4): sprintf(array, "%5.2f", 0); return array; break;
        }
    }

    bool setParameterButton(int column, int row, float data_float){
        return false;
    }

    const char* getLineName(int row){
        switch(row){
        case(0): return ""; break;
        case(1): return ""; break;
        case(2): return ""; break;
        case(3): return ""; break;
        case(4): return ""; break;
        }
    }

    bool setParameterLine(int row, float data_float){
        return true;
    }

    const char* getComboBoxName(int row){
        switch(row){
        case(0): return ""; break;
        case(1): return ""; break;
        case(2): return ""; break;
        case(3): return ""; break;
        case(4): return ""; break;
        }
    }

    const char* getQStringList(int row){
        switch(row){
        case(0): return ""; break;
        case(1): return ""; break;
        case(2): return ""; break;
        case(3): return ""; break;
        case(4): return ""; break;
        }
    }

    bool setParameterComboBox(int row, float data_float){

            int numberComboBox = (int) data_float;

            switch(row){
            case(0): //Sensitivity
                    switch(numberComboBox){
                    case(0): /*      */ return true; break;
                    case(1): parameterD=1; return true; break;      //1V
                    case(2): parameterD=0.3; return true; break;    //300mV
                    case(3): parameterD=0.1; return true; break;    //and so on
                    case(4): parameterD=0.03; return true; break;
                    case(5): parameterD=0.01; return true; break;
                    case(6): parameterD=0.003; return true; break;
                    case(7): parameterD=0.001; return true; break;
                    case(8): parameterD=0.0003; return true; break;
                    case(9): parameterD=0.0001; return true; break;
                    case(10): parameterD=0.00003; return true; break;
                    case(11): parameterD=0.00001; return true; break;
                    case(12): parameterD=0.000003; return true; break;
                    case(13): parameterD=0.000001; return true; break;
                    }
                    break;
            case(1): //Time constant
                switch(numberComboBox){
                case(0): /*      */ return true; break;
                case(1): parameterE=1; return true; break;      //1S
                case(2): parameterE=0.3; return true; break;    //300mS
                case(3): parameterE=0.1; return true; break;    //and so on
                case(4): parameterE=0.03; return true; break;
                case(5): parameterE=0.01; return true; break;
                case(6): parameterE=0.003; return true; break;
                case(7): parameterE=0.001; return true; break;
                case(8): parameterE=0.0003; return true; break;
                case(9): parameterE=0.0001; return true; break;
                case(10): parameterE=0.00003; return true; break;
                case(11): parameterE=0.00001; return true; break;
                case(12): parameterE=0.000003; return true; break;
                case(13): parameterE=0.000001; return true; break;
                }
                    break;
            case(2): //Phase
                    switch(numberComboBox){
                    case(0):  return true; break;
                    case(1): parameterF=0; return true; break;
                    case(2): parameterF=45; return true; break;
                    case(3): parameterF=90; return true; break;
                    case(4): parameterF=180; return true; break;
                    case(5): parameterF=270; return true; break;
                    }
                    break;
            case(3): //Filters
                    switch(numberComboBox){
                    case(0):  return true; break;
                    case(1): parameterG=0; return true; break;
                    case(2): parameterG=1; return true; break;
                    case(3): parameterG=2; return true; break;
                    case(4): parameterG=3; return true; break;
                    case(5): parameterG=4; return true; break;
                    }
                    break;
            case(4): //Config
                    switch(numberComboBox){
                    case(0):  return true; break;
                    case(1): parameterH=40; return true; break;
                    case(2): parameterH=41; return true; break;
                    case(3): parameterH=42; return true; break;
                    case(4): parameterH=43; return true; break;
                    case(5): parameterH=44; return true; break;
                    }
                    break;
            }
      }

};

ClassLAMPh classLAMPh[NUMBER];

const char* getName() {return DEVICE;}

const char* getInfo(int number_of_device) {
    /*if (number_of_device < NUMBER ){ //protection from going beyond the classLAMPh
        return classLAMPh[number_of_device].infoL();;
    }else{*/
        return INFO;
    //}
}

const char* DLLMain() {createFile();return INFO;}

const char* getFunctions() {createFile();return FUNCTIONS;}

bool createFile(){
    FILE *file;
    char cNewLine = '\n';
    if ((file = fopen(FOLDER NAME TXT,"w+")) == NULL) return false;
    fprintf(file,"%s%c", FUNCTIONS, cNewLine);
    fclose(file);
    return true;
}


bool connectL(int number_of_device){
    if (number_of_device < NUMBER ){ //protection from going beyond the classLAMPh
        return classLAMPh[number_of_device].connectL();
    }else{
        return false;
    }
}

const char* getStatus(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getStatus();
    }else{
        return NONE;
    }
}

const char* getUnit(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getStatus();
    }else{
        return NONE;
    }
}

void readData(int number_of_device){
    if (number_of_device < NUMBER ){
        classLAMPh[number_of_device].readData();
    }
}


float getSeconds(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getSeconds();
    }else{
        return -1;
    }
}
float getSecondsFromNull(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getSecondsFromNull();
    }else{
        return -1;
    }
}

float getMinutes(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getMinutes();
    }else{
        return -1;
    }
}
float getMinutesFromNull(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getMinutesFromNull();
    }else{
        return -1;
    }
}

float getHours(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getHours();
    }else{
        return -1;
    }
}
float getHoursfromNull(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getHoursfromNull();
    }else{
        return -1;
    }
}

float getDays(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getDays();
    }else{
        return -1;
    }
}
float getDaysFromNull(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getDaysFromNull();
    }else{
        return -1;
    }
}


// //

bool startL(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].startL();
    }else{
        return false;
    }
}

bool stopL(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].stopL();
    }else{
        return false;
    }
}

bool pauseL(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].pauseL();
    }else{
        return false;
    }
}

bool exitL(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].exitL();
    }else{
        return false;
    }
}

bool sendMessage(int number_of_device, const char* const str){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].sendMessage(str);
    }else{
        return false;
    }
}

const char* getRowName(int number_of_device, int row){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getRowName(row);
    }
}

const char* getButtonName(int number_of_device, int column, int row){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getButtonName(column,row);
    }
}

const char* returnRowData(int number_of_device, int row){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].returnRowData(row);
    }
}

bool setParameterButton(int number_of_device, int column, int row, float data_float){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].setParameterButton(column,row,data_float);
    }else{
        return false;
    }
}

const char* getLineName(int number_of_device, int row){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getLineName(row);
    }
}

bool setParameterLine(int number_of_device, int row, float data_float){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].setParameterLine(row,data_float);
    }else{
        return false;
    }
}

const char* getComboBoxName(int number_of_device, int row){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getComboBoxName(row);
    }
}

const char* getQStringList(int number_of_device, int row){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getQStringList(row);
    }
}

bool setParameterComboBox(int number_of_device, int row, float data_float){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].setParameterComboBox(row,data_float);
    }else{
        return false;
    }
}




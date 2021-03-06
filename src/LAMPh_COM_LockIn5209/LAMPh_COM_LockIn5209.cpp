/*-------------------------------------------------
*
*	This library is written to work with the lock-in LockIn5209.
*	Created by Ilnur Gimazov 2019-09-02 (2nd September 2019)
*	Lib for LAMPh
*	
*-------------------------------------------------
*/

#include "LAMPh_COM_LockIn5209.h"

#include <stdio.h>
#include <cmath>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

#include <QDebug>
#include <QThread>
#include <QDateTime>
//#include <QSerialPort>
//#include <QSerialPortInfo>

#include <windows.h>
#include <iostream>
using namespace std;

#define NAME  "LAMPh_COM_LockIn5209"  	//Name of device
#define DEVICE "Lock-in LockIn5209" 			//Name of device
#define FOLDER  "Functions/"	//Folder where file will be created (please do not change this value)
#define TXT "_functions.txt"	//Names of functions will be recorded in this file (please do not change this value)	
#define FUNCTIONS "float getFloat();float getFloatAuxIn1();float getFloatAuxIn2();float getFloatAuxIn3();float getFloatAuxIn4();void setAUXV1(float);void setAUXV2(float)"  //one can use only void and float functions with only float parameter (don't leave empty space after ";")
#define INFO "The Lib for LAMPh to connect with TEMP" //Info about this Lib or device
#define NUMBER 1 //One device can be connected by this Lib (this value can be changed)
#define NONE "None"

class ClassLAMPh{
private:
    //QSerialPort serialPort;

    HANDLE hSerial;
    COMMTIMEOUTS timeouts;
    DWORD dwSize;

    QString stringReceived;

	char * status_char;
	char * unit_char;
    char * temp;
	
	float result_floatA = 0;
	float result_floatB = 0;
    float result_floatC = 0;
    float result_floatAuxIn1 = 0;
    float result_floatAuxIn2 = 0;
    float result_floatAuxIn3 = 0;
    float result_floatAuxIn4 = 0;

	float result_floatD = 0;
	float parameterD = 0;
	float result_floatE = 0;
	float parameterE = 0;
	float result_floatF = 0;
	float parameterF = 0;

	float parameterG = 0;
	float parameterH = 0;
	float parameterI = 0;
	
	bool getFloatA_active = false;
	bool getFloatB_active = false;
	bool getFloatC_active = false;
	bool getFloatParD_active = false;
    bool getFloatParE_active = false;
    bool getFloatParF_active = false;
    bool setParameterG_active = false;
    bool setParameterH_active = false;
    bool setParameterI_active = false;

    float result_float[14];

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
    ClassLAMPh( ) 
    {
        status_char = "None";
        unit_char = "None";


     }
	
    bool connectL(){

        //for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()){
           // if (!info.isBusy())

           // {

                //qDebug() << "ComPort:" << info.portName();
                //LPCTSTR sPortName = (wchar_t*)QString(info.portName()).utf16();

                LPCTSTR sPortName = L"COM2";

                hSerial = ::CreateFile(sPortName,GENERIC_READ | GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
                if(hSerial==INVALID_HANDLE_VALUE)
                {
                        if(GetLastError()==ERROR_FILE_NOT_FOUND)
                    {
                        qDebug() << "serial port does not exist";
                    }
                    qDebug() << "some other error occurred";
                }

                int time_cc = 3;
                timeouts.ReadIntervalTimeout = time_cc;
                timeouts.ReadTotalTimeoutMultiplier = time_cc;
                timeouts.ReadTotalTimeoutConstant = time_cc;

                int time_cc2 = 3;
                timeouts.WriteTotalTimeoutMultiplier = time_cc2;
                timeouts.WriteTotalTimeoutConstant = time_cc2;
                if (!SetCommTimeouts(hSerial, &timeouts)){
                    qDebug() << "setting timeouts failed";
                }


                DCB dcbSerialParams = {0};
                dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
                if (!GetCommState(hSerial, &dcbSerialParams))
                {
                    qDebug() << "Error";
                }
                dcbSerialParams.BaudRate=CBR_9600;
                dcbSerialParams.ByteSize=7;
                dcbSerialParams.StopBits=ONESTOPBIT;
                dcbSerialParams.Parity=EVENPARITY;
                if(!SetCommState(hSerial, &dcbSerialParams))
                {
                    qDebug() << "Error setting serial port state";
                }
                WriteFilePort(1,"ID\r\n", sizeof("ID\r\n"));
                ReadFilePort();

                if (stringReceived.contains("5209", Qt::CaseInsensitive)){
                    qDebug() << "Connected to 5209";
                    return true;
                }else{
                    CloseHandle(hSerial);
                }
                return false;
            //}
        //}
        //return false
	}

    bool WriteFilePort( int N, char* dataW = "*", int size = 0){
        DWORD dwBytesWritten;
        if (N==1) //if N==0 use last char* dataW
        {
        temp = dataW;
        dwSize = size;
        }
        for (int k=0; k<dwSize; k++)
        {
            char s3 = temp[k];
            if (strchr (&s3,'_') !=0){
                s3 = ' ';
            }
            WriteFile (hSerial,&s3,1,&dwBytesWritten,NULL);
        }
        return true;
    }

    QString ReadFilePort(){
        DWORD iSize;
        char sReceivedChar;
        stringReceived ="";
        int int_while=0;
        while (true)
        {
            ReadFile(hSerial, &sReceivedChar, 1, &iSize, NULL);  // get 1 byte
            if (iSize > 0){   // if some data received
                int_while=0;
                //QString s2 = QString::fromUtf8(&sReceivedChar);
                QString s2 = QString(QChar::fromLatin1(sReceivedChar));

                stringReceived += s2;
                //qDebug() << "sReceivedChar" << sReceivedChar << "s2" << s2;

                if (strchr (&sReceivedChar,'?') !=0){
                    stringReceived.clear();
                    WriteFilePort(0);
                }
                if (strchr (&sReceivedChar,'*') !=0){
                    break;
                }
            }
            else{
                int_while++;
                //qDebug() << "int_while" << int_while;
            }
            if (int_while>10){
                break;
            }
        }
        return stringReceived;
    }

    const char* getStatus(){
        return status_char; //it can be enum message
    }
	
	const char* getUnit(){
        return unit_char;
    }
	
	void readData(){
        QString str;
        QString str2;

        WriteFilePort(1, "*\r", sizeof("*\r"));
        str = ReadFilePort();
        str.remove("\r\r\n*");
        float temp_float = str.toFloat();
        if (temp_float!=0) result_floatA  = temp_float;

        //qDebug() << str;
        WriteFilePort(1, "ADC_1\r", sizeof("ADC_1\r"));
        str2 = ReadFilePort();
        if (str2 == "ADC"){
            //qDebug() << "tr";
            WriteFilePort(1, "ADC_1\r", sizeof("ADC_1\r"));
            str = ReadFilePort();
        }
        //qDebug() << "str2: " << str2;
        str2.remove("ADC1");
        str2.remove("ADC 1");
        str2.remove("\r\n");
        str2.remove("\r\n");
        str2.remove("*");
        //qDebug() << "str22: " << str2;
        float temp_float2 = str2.toFloat();
        if (temp_float2!=0) result_floatAuxIn1  = temp_float2;
    }
	
    float getFloat(){
        return result_floatA;
    }

    float getFloatAuxIn1(){

        return result_floatAuxIn1;

    }
	
	
	float getFloatAuxIn2(){
        QString str;
        WriteFilePort(1, "ADC_2\r", sizeof("ADC_2\r"));
        str = ReadFilePort();
        if (str == "ADC"){
            WriteFilePort(1, "ADC_2\r", sizeof("ADC_2\r"));
            str = ReadFilePort();
        }
        str.remove("ADC2");
        str.remove("ADC 2");
        str.remove("\r\n");
        str.remove("\r\n");
        str.remove("*");
        float temp_float = str.toFloat();
        if (temp_float!=0) result_floatAuxIn2  = temp_float;
        return result_floatAuxIn2;
    }
	
	float getFloatAuxIn3(){
        QString str;
        WriteFilePort(1, "ADC_3\r", sizeof("ADC_3\r"));
        str = ReadFilePort();
        if (str == "ADC"){
            WriteFilePort(1, "ADC_3\r", sizeof("ADC_3\r"));
            str = ReadFilePort();
        }
        str.remove("ADC3");
        str.remove("ADC 3");
        str.remove("\r\n");
        str.remove("\r\n");
        str.remove("*");
        float temp_float = str.toFloat();
        if (temp_float!=0) result_floatAuxIn3  = temp_float;
        return result_floatAuxIn3;
    }
	
	float getFloatAuxIn4(){
        QString str;
        WriteFilePort(1, "ADC_4\r", sizeof("ADC_4\r"));
        str = ReadFilePort();
        if (str == "ADC"){
            WriteFilePort(1, "ADC_4\r", sizeof("ADC_4\r"));
            str = ReadFilePort();
        }
        str.remove("ADC4");
        str.remove("ADC 4");
        str.remove("\r\n");
        str.remove("\r\n");
        str.remove("*");
        float temp_float = str.toFloat();
        if (temp_float!=0) result_floatAuxIn4  = temp_float;
        return result_floatAuxIn4;
    }

	float getFloatA(){
        getFloatA_active = true; //after that program will be readData for this function
        return result_floatA;
	}
	
	float getFloatB(){
        getFloatB_active = true;
        return result_floatB;
	}
	
	float getFloatC(){
        getFloatC_active = true;
		return result_floatC;
	}
	
	float getFloatParD(float parameter){
        getFloatParE_active = true;
        result_floatE = parameterE;
        return result_floatE;
	}
	
	float getFloatParE(float parameter){
		getFloatParE_active = true;
        result_floatE = parameterE;
		return result_floatE;
	}
	
	float getFloatParF(float parameter){
		getFloatParF_active = true;
        //parameterF = parameter;
        result_floatF = parameterF;
        return result_floatF;
	}
	

    void setAUXV1(float parameter){
        //serialPort.write("AUXV1,");
        //serialPort.write(QByteArray::number(parameter));
        //serialPort.write("\r\n");
    }

    void setAUXV2(float parameter){
        //serialPort.write("AUXV2,");
        //serialPort.write(QByteArray::number(parameter));
        //serialPort.write("\r\n");
    }

	void setParameterG(float parameter){
        //setParameterG_active = false; // because it does not read data!!!, but it sets parameter
        parameterG = parameter; // this parameterG can be used for other functions
	}
	
	void setParameterH(float parameter){
		parameterH = parameter;
	}
	
	void setParameterI(float parameter){
		parameterI = parameter;
	}

    const char* infoL(){
        QString strInfo = "The Lib for LAMPh to connect with lock-in LockIn5209. ";
            return strInfo.toStdString().c_str();
    }

    bool startL(){
	if ((result_floatA = 0) and (result_floatB = 0) /* and so on*/) return true; //YES!!! it is result_floatA = 0, because we want reset all parameters 
	else return false;
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
                case(0): return "1V(0)"; break;
                case(1): return "300mV(1)"; break;
                case(2): return "100mV(2)"; break;
                case(3): return "30mV(3)"; break;
                case(4): return "10mV(4)"; break;
				}
				break;	
		case(1): //Time constant
				switch(column){
                case(0): return "1s(10)"; break;
                case(1): return "300ms(11)"; break;
                case(2): return "100ms(12)"; break;
                case(3): return "30ms(13)"; break;
                case(4): return "10ms(14)"; break;
				}
				break;
		case(2): //Phase
				switch(column){
                case(0): return "0(20)"; break;
                case(1): return "+90(21)"; break;
                case(2): return "-90(22)"; break;
                case(3): return "+1(23)"; break;
                case(4): return "-1(24)"; break;
				}
				break;	
		case(3): //Filters
				switch(column){
                case(0): return "BP(30)"; break;
                case(1): return "LP(31)"; break;
                case(2): return "FLAT(32)"; break;
                case(3): return "TRACK(33)"; break;
                case(4): return "MAN(34)"; break;
				}
				break;	
		case(4): //Config
				switch(column){
                case(0): return "LIGHTS(40)"; break;
                case(1): return "RS232(41)"; break;
                case(2): return "A-OFFSET(42)"; break;
                case(3): return "A-PHASE(43)"; break;
                case(4): return "A-FILTER(44)"; break;
				}
				break;
        }
    }

    char array[33];

    const char* returnRowData(int row){
        switch(row){
        case(0): sprintf(array, "%5.2f", parameterD); return array; break;
        case(1): sprintf(array, "%5.2f", parameterE); return array; break;
        case(2): sprintf(array, "%5.2f", parameterF); return array; break;
        case(3): sprintf(array, "%5.2f", parameterG); return array; break;
        case(4): sprintf(array, "%5.2f", parameterH); return array; break;
        }
    }
	
	bool setParameterButton(int column, int row, float data_float){
		switch(row){
		case(0): //Sensitivity
				switch(column){
                case(0): parameterD=0; return true; break;
                case(1): parameterD=1; return true; break;
                case(2): parameterD=2; return true; break;
                case(3): parameterD=3; return true; break;
                case(4): parameterD=4; return true; break;
				}
				break;	
		case(1): //Time constant
            switch(column){
                case(0): parameterE=10; return true; break;
                case(1): parameterE=11; return true; break;
                case(2): parameterE=12; return true; break;
                case(3): parameterE=13; return true; break;
                case(4): parameterE=14; return true; break;
				}
				break;
		case(2): //Phase
                switch(column){
                case(0): parameterF=20; return true; break;
                case(1): parameterF=21; return true; break;
                case(2): parameterF=22; return true; break;
                case(3): parameterF=23; return true; break;
                case(4): parameterF=24; return true; break;
				}
				break;	
		case(3): //Filters
				switch(column){
                case(0): parameterG=30; return true; break;
                case(1): parameterG=31; return true; break;
                case(2): parameterG=32; return true; break;
                case(3): parameterG=33; return true; break;
                case(4): parameterG=34; return true; break;
				}
				break;	
		case(4): //Config
				switch(column){
                case(0): parameterH=40; return true; break;
                case(1): parameterH=41; return true; break;
                case(2): parameterH=42; return true; break;
                case(3): parameterH=43; return true; break;
                case(4): parameterH=44; return true; break;
				}
				break;
        }
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
		switch(row){
        case(0): parameterD=data_float; return true; break;
        case(1): parameterE=data_float; return true; break;
        case(2): parameterF=data_float; return true; break;
        case(3): parameterG=data_float; return true; break;
        case(4): parameterH=data_float; return true; break;
        }
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
        case(0): return "Sensitivity:;1V;300mV;100mV;30mV;10mV;3mV;1mV;300mkV;100mkV;30mkV;10mkV;3mkV;1mkV"; break;
        case(1): return "Time constant:;1S;300mS;100mS;30mS;10mS;3mS;1mS;300mkS;100mkS;30mkS;10mkS;3mkS;1mkS"; break;
        case(2): return "Phase:;0;45;90;180;270"; break;
        case(3): return "Filters:;BP;LP;FLAT;TRACK;MAN"; break;
        case(4): return "Config:;LIGHTS;RS232;A-OFFSET;A-PHASE;A-FILTER"; break;
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

//const char* getInfo(int number_of_device) {createFile();return INFO;}
const char* getInfo(int number_of_device) {
    if (number_of_device < NUMBER ){ //protection from going beyond the classLAMPh
        return classLAMPh[number_of_device].infoL();;
    }else{
        return INFO;
    }
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

float getFloat(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getFloat();
    }else{
        return -1;
    }
}

float getFloatAuxIn1(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getFloatAuxIn1();
    }else{
        return -1;
    }
}

float getFloatAuxIn2(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getFloatAuxIn2();
    }else{
        return -1;
    }
}

float getFloatAuxIn3(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getFloatAuxIn3();
    }else{
        return -1;
    }
}

float getFloatAuxIn4(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getFloatAuxIn4();
    }else{
        return -1;
    }
}


void setAUXV1(int number_of_device, float parameter){
    if (number_of_device < NUMBER ){
        classLAMPh[number_of_device].setAUXV1(parameter);
    }
}

void setAUXV2(int number_of_device, float parameter){
    if (number_of_device < NUMBER ){
        classLAMPh[number_of_device].setAUXV2(parameter);
    }
}

float getFloatA(int number_of_device){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].getFloatA();
    }else{
        return -1;
    }
}

float getFloatB(int number_of_device){
	if (number_of_device < NUMBER ){ 
		return classLAMPh[number_of_device].getFloatB();
	}else{
		return -1;
	}
}

float getFloatC(int number_of_device){
	if (number_of_device < NUMBER ){ 
		return classLAMPh[number_of_device].getFloatC();
	}else{
		return -1;
	}
}

float getFloatParD(int number_of_device, float parameter){
	if (number_of_device < NUMBER ){ 
		return classLAMPh[number_of_device].getFloatParD(parameter);
	}else{
		return -1;
	}
}

float getFloatParE(int number_of_device, float parameter){
	if (number_of_device < NUMBER ){ 
		return classLAMPh[number_of_device].getFloatParE(parameter);
	}else{
		return -1;
	}
}

float getFloatParF(int number_of_device, float parameter){
	if (number_of_device < NUMBER ){ 
		return classLAMPh[number_of_device].getFloatParF(parameter);
	}else{
		return -1;
	}
}

void setParameterG(int number_of_device, float parameter){
	if (number_of_device < NUMBER ){ 
		classLAMPh[number_of_device].setParameterG(parameter);
	}
}

void setParameterH(int number_of_device, float parameter){
	if (number_of_device < NUMBER ){ 
		classLAMPh[number_of_device].setParameterH(parameter);
	}
}

void setParameterI(int number_of_device, float parameter){
	if (number_of_device < NUMBER ){ 
		classLAMPh[number_of_device].setParameterI(parameter);
	}
}

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


/*-------------------------------------------------
*
*	This library was written to work with the multimeter KEITHLEY2000
*	Created by Ilnur Gimazov (ubvfp94@mail.ru) 2019-07-02 (2nd July 2019)
*	Lib for LAMPh
*	
*-------------------------------------------------
*/

#include "LAMPh_COM_KEITHLEY2000.h"

#include <stdio.h>
#include <cmath>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

#include <QDebug>
#include <QDateTime>
#include <QSerialPort>
#include <QSerialPortInfo>

#define NAME  "LAMPh_COM_KEITHLEY2000"  	//Name of device
#define DEVICE "COM_KEITHLEY2000" 			//Name of device
#define FOLDER  "Functions/"	//Folder where file will be created (please do not change this value)
#define TXT "_functions.txt"	//Names of functions will be recorded in this file (please do not change this value)	
#define FUNCTIONS "float getFloatA();void setParameterG(float)"  //one can use only void and float functions with only float parameter (don't leave empty space after ";")
#define INFO "The Lib for LAMPh to connect with COM_KEITHLEY2000" //Info about this Lib or device
#define NUMBER 2 //Two devices can be connected by this Lib (this value can be changed)
#define NONE "None"

class ClassLAMPh{
private:
    QSerialPort serialPort;
	char * status_char;
	char * unit_char;
	
	float result_floatA = 0;
	float result_floatB = 0;
	float result_floatC = 0;
	
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
		for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()){
            if (!info.isBusy())
            {
                serialPort.setPortName(info.portName());
                serialPort.setBaudRate(QSerialPort::Baud9600);
                serialPort.setStopBits(QSerialPort::OneStop);
                serialPort.setDataBits(QSerialPort::Data8);
                serialPort.setParity(QSerialPort::NoParity);
                serialPort.setFlowControl(QSerialPort::NoFlowControl);
                serialPort.open(QIODevice::ReadWrite);

                QByteArray ba;
                ba.resize(7);
                ba[0] = 0x2a;
                ba[1] = 0x49;
                ba[2] = 0x44;
                ba[3] = 0x4e;
                ba[4] = 0x3f;
                ba[5] = 0x0d;
                ba[6] = 0x0a; // "*IDN?"

                serialPort.waitForBytesWritten(300);

                serialPort.write("*IDN?\r\n"); //or serialPort.write(ba);

                serialPort.waitForReadyRead(300);

                QByteArray data;
                data= serialPort.readAll();
                while (serialPort.waitForReadyRead(10))
                    data += serialPort.readAll();
				

				QString dataQString(data.toHex());
				
                if (dataQString.contains("4B45495448", Qt::CaseInsensitive)){
                    serialPort.write("*RST\r\n");
					serialPort.write(":SENS:FUNC 'VOLT:DC'\r\n");
					serialPort.write(":SENS:FRES:DIG 6\r\n");
					serialPort.write(":READ?\r\n");
				    return true;
                }else
                {
                    serialPort.close();
                }
            }
        }
        return false;
	}

	const char* getStatus(){
        return status_char; //it can be enum message
    }
	
	const char* getUnit(){
        return unit_char;
    }
	
	void readData(){

        serialPort.write(":READ?\r\n");
        serialPort.waitForReadyRead(10);
        QByteArray data = serialPort.readAll();

        std::string result_tmp = data.toStdString();
        QString data_tmp = QString::fromStdString(result_tmp);
        //data_tmp.remove("\n");
        //data_tmp.remove("\r");


        qDebug() << "data_tmp" << data_tmp;
        result_floatA = data_tmp.toFloat();


        /*if (getFloatA_active){ // if (getFloatA_active==true)
			serialPort.write(":READ_floatA?\r\n");
			QByteArray data = serialPort.readAll();
			std::string result_tmp = data.toStdString();
			QString data_tmp = QString::fromStdString(result_tmp);
			result_floatA = data_tmp.toFloat();
		}
		
		if (getFloatB_active){ 
			serialPort.write(":READ_floatB?\r\n");
			QByteArray data = serialPort.readAll();
			std::string result_tmp = data.toStdString();
			QString data_tmp = QString::fromStdString(result_tmp);
			result_floatB = data_tmp.toFloat();
		}
		
		if (getFloatC_active){ 
			serialPort.write(":READ_floatC?\r\n");
			QByteArray data = serialPort.readAll();
			std::string result_tmp = data.toStdString();
			QString data_tmp = QString::fromStdString(result_tmp);
			result_floatC = data_tmp.toFloat();
		}
		
		if (getFloatParD_active){ 
			serialPort.write(":READ_floatD?\r\n");
			QByteArray data = serialPort.readAll();
			std::string result_tmp = data.toStdString();
			QString data_tmp = QString::fromStdString(result_tmp);
			result_floatD = data_tmp.toFloat();
		}
		
		if (getFloatParE_active){ 
			serialPort.write(":READ_floatE?\r\n");
			QByteArray data = serialPort.readAll();
			std::string result_tmp = data.toStdString();
			QString data_tmp = QString::fromStdString(result_tmp);
			result_floatE = data_tmp.toFloat();
		}
		
		if (getFloatParF_active){ 
			serialPort.write(":READ_floatF?\r\n");
			QByteArray data = serialPort.readAll();
			std::string result_tmp = data.toStdString();
			QString data_tmp = QString::fromStdString(result_tmp);
			result_floatF = data_tmp.toFloat();
        }*/
	
		//if (setParameterG){} //it doesn't matter, it always must be false, because it can't read data  
	}
	
	float getFloatA(){
        getFloatA_active = true; //after that program will be readData for this function
        return result_floatA;
	}
	
	float getFloatB(){
		getFloatB_active = true;
        //result_floatB=result_floatB+result_floatA;
        result_floatB = parameterG;
        return result_floatB;
	}
	
	float getFloatC(){
		getFloatC_active = true;
        result_floatC = parameterH;
		return result_floatC;
	}
	
	float getFloatParD(float parameter){
		getFloatParD_active = true;
        //parameterD = parameter;
        result_floatD = parameterD;
		return result_floatD;
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
	
	void setParameterG(float parameter){
        if (parameterG != parameter)
        {\
            //":ROUTE:CLOSE (@1)\r\n"
            serialPort.write(":ROUTE:CLOSE (@");
            serialPort.write(QByteArray::number(parameter));
            serialPort.write(")\r\n");
            parameterG = parameter; // this parameterG can be used for other functions
        }
	}
	
	void setParameterH(float parameter){
		parameterH = parameter;
	}
	
	void setParameterI(float parameter){
		parameterI = parameter;
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
        case(0): return "Functions I"; break;
        case(1): return "Functions II"; break;
        case(2): return "Channels I"; break;
        case(3): return "Channels II"; break;
        case(4): return "Config"; break;
        }
	}

    const char* getButtonName (int column, int row){
		switch(row){
        case(0): //Functions I
				switch(column){
                case(0): return "AC Current"; break;
                case(1): return "DC Current"; break;
                case(2): return "AC Voltage"; break;
                case(3): return "DC Voltage"; break;
                case(4): return "Temperature"; break;
				}
				break;	
        case(1): //Functions II
				switch(column){
                case(0): return "2W Resistance"; break;
                case(1): return "4W Resistance"; break;
                case(2): return "Period"; break;
                case(3): return "Frequency"; break;
                case(4): return "Diode Testing"; break;
				}
				break;
        case(2): //Channels I
				switch(column){
                case(0): return "Channel 1"; break;
                case(1): return "Channel 2"; break;
                case(2): return "Channel 3"; break;
                case(3): return "Channel 4"; break;
                case(4): return "Channel 5"; break;
				}
				break;	
        case(3): //Channels II
				switch(column){
                case(0): return "Channel 6"; break;
                case(1): return "Channel 7"; break;
                case(2): return "Channel 8"; break;
                case(3): return "Channel 9"; break;
                case(4): return "Channel 10"; break;
				}
				break;	
		case(4): //Config
				switch(column){
                case(0): return "Display ON"; break;
                case(1): return "Display OFF"; break;
                case(2): return "DIGITS"; break;
                case(3): return "RATE"; break;
                case(4): return "AUTO"; break;
				}
				break;
        }
    }

    char array[33];

    const char* returnRowData(int row){
        switch(row){
        case(0): return ""; break;
        case(1): return ""; break;
        case(2): return ""; break;
        case(3): return ""; break;
        case(4): return ""; break;
        }
    }
	
	bool setParameterButton(int column, int row, float data_float){
		switch(row){
        case(0): //Functions I
				switch(column){
                case(0): serialPort.write("*RST\r\n");serialPort.write(":SENS:FUNC 'CURR:AC'\r\n");serialPort.write(":SENS:CURR:AC:DIG 5\r\n"); return true; break;
                case(1): serialPort.write("*RST\r\n");serialPort.write(":SENS:FUNC 'CURR:DC'\r\n");serialPort.write(":SENS:CURR:DC:DIG 5\r\n"); return true; break;
                case(2): serialPort.write("*RST\r\n");serialPort.write(":SENS:FUNC 'VOLT:AC'\r\n");serialPort.write(":SENS:VOLT:AC:DIG 6\r\n"); return true; break;
                case(3): serialPort.write("*RST\r\n");serialPort.write(":SENS:FUNC 'VOLT:DC'\r\n");serialPort.write(":SENS:VOLT:DC:DIG 6\r\n"); return true; break;
                case(4): serialPort.write("*RST\r\n");serialPort.write(":SENS:FUNC 'TEMP'\r\n");serialPort.write(":SENS:TEMP:DIG 6\r\n"); return true; break;
				}
				break;	
        case(1): //Functions II
            switch(column){
                case(0): serialPort.write("*RST\r\n");serialPort.write(":SENS:FUNC 'RES'\r\n");serialPort.write(":SENS:RES:DIG 6\r\n"); return true; break;
                case(1): serialPort.write("*RST\r\n");serialPort.write(":SENS:FUNC 'FRES'\r\n");serialPort.write(":SENS:FRES:DIG 6\r\n"); return true; break;
                case(2): serialPort.write("*RST\r\n");serialPort.write(":SENS:FUNC 'PER'\r\n");serialPort.write(":SENS:PER:DIG 6\r\n"); return true; break;
                case(3): serialPort.write("*RST\r\n");serialPort.write(":SENS:FUNC 'FREQ'\r\n");serialPort.write(":SENS:FREQ:DIG 6\r\n"); return true; break;
                case(4): serialPort.write("*RST\r\n");serialPort.write(":SENS:FUNC 'DIOD'\r\n"); return true; break;
				}
				break;
        case(2): //Channels I
                switch(column){
                case(0): serialPort.write(":ROUTE:CLOSE (@1)\r\n"); return true; break;
                case(1): serialPort.write(":ROUTE:CLOSE (@2)\r\n"); return true; break;
                case(2): serialPort.write(":ROUTE:CLOSE (@3)\r\n"); return true; break;
                case(3): serialPort.write(":ROUTE:CLOSE (@4)\r\n"); return true; break;
                case(4): serialPort.write(":ROUTE:CLOSE (@5)\r\n"); return true; break;
				}
				break;	
        case(3): //Channels II
				switch(column){
                case(0): serialPort.write(":ROUTE:CLOSE (@6)\r\n"); return true; break;
                case(1): serialPort.write(":ROUTE:CLOSE (@7)\r\n"); return true; break;
                case(2): serialPort.write(":ROUTE:CLOSE (@8)\r\n"); return true; break;
                case(3): serialPort.write(":ROUTE:CLOSE (@9)\r\n"); return true; break;
                case(4): serialPort.write(":ROUTE:CLOSE (@10)\r\n"); return true; break;
				}
				break;	
		case(4): //Config
				switch(column){
                case(0): serialPort.write(":DISP:ENAB ON\r\n"); return true; break;
                case(1): serialPort.write(":DISP:ENAB OFF\r\n"); return true; break;
                case(2): serialPort.write(":SYST:KEY 30\r\n"); return true; break;
                case(3): serialPort.write(":SYST:KEY 31\r\n"); return true; break;
                case(4): serialPort.write(":SYST:KEY 12\r\n"); return true; break;
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

const char* getInfo(int number_of_device) {createFile();return INFO;}

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


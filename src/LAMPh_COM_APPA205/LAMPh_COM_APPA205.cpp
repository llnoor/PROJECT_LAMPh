/*-------------------------------------------------
*
*	To work with APPA 205
*	Created by Ilnur Gimazov (ubvfp94@mail.ru) 2018-10-17 (17th October 2018)
*	Lib for LAMPh
*	
*-------------------------------------------------
*/

#include "LAMPh_COM_APPA205.h"

#include <stdio.h>
#include <cmath>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

#define NAME  "LAMPh_COM_APPA205"  	//Name of device
#define DEVICE "APPA205" 			//Name of device
#define FOLDER  "Functions/"	//Folder where file will be created (please do not change this value)
#define TXT "_functions.txt"	//Names of functions will be recorded in this file (please do not change this value)	
#define FUNCTIONS "float getFloat()"  //one can use only void and float functions with only float parameter (don't leave empty space after ";")
#define INFO "The Lib for LAMPh to connect with APPA205" //Info about this Lib or device
#define NUMBER 5
#define NONE "None"

class ClassLAMPh{
private:
    QSerialPort serialPort;
	char * status_char;
	char * unit_char;
	
	float result_float=0;
	
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
        QByteArray data;
        QSerialPort serialPort;
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
                ba.resize(5);
                ba[0] = 0x55;
                ba[1] = 0x55;
                ba[2] = 0x00;
                ba[3] = 0x00;
                ba[4] = 0xaa;

                QByteArray ba_check;
                ba_check.resize(3);
                ba_check[0] = 0x55;
                ba_check[1] = 0x55;
                ba_check[2] = 0x00;

                serialPort.waitForBytesWritten(100);

                serialPort.write(ba);

                serialPort.waitForReadyRead(100);

                QByteArray data = serialPort.readAll();
                while (serialPort.waitForReadyRead(10))
                    data += serialPort.readAll();

                qDebug().noquote() << "bytes: " << data.size() << " values: " << data.toHex();

                bool match_bool =true;
                for (int i=0; i<3 /*ba_check.size()*/; i++)
                {
                    if (ba_check[i]!=data[i]){
                        serialPort.close();
                        match_bool = false;
                    }
                }
                if (match_bool) return true;

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
        QByteArray ba;
        ba.resize(5);
        ba[0] = 0x55;
        ba[1] = 0x55;
        ba[2] = 0x00;
        ba[3] = 0x00;
        ba[4] = 0xaa;
        serialPort.write(ba);

        //serialPort.waitForReadyRead(10);
        QByteArray data = serialPort.readAll();
        serialPort.waitForReadyRead(10);
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

        if (5<data.size())
        {

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
        }
	}
	
	float getFloat(){
        return result_float;
	}
	

    bool startL(){
      return false;
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
		switch(row){
		case(0): //Sensitivity
				switch(column){
                case(0): return true; break;
                case(1): return true; break;
                case(2): return true; break;
                case(3): return true; break;
                case(4): return true; break;
				}
				break;	
		case(1): //Time constant
            switch(column){
                case(0): return true; break;
                case(1): return true; break;
                case(2): return true; break;
                case(3): return true; break;
                case(4): return true; break;
				}
				break;
		case(2): //Phase
                switch(column){
                case(0): return true; break;
                case(1): return true; break;
                case(2): return true; break;
                case(3): return true; break;
                case(4): return true; break;
				}
				break;	
		case(3): //Filters
				switch(column){
                case(0): return true; break;
                case(1): return true; break;
                case(2): return true; break;
                case(3): return true; break;
                case(4): return true; break;
				}
				break;	
		case(4): //Config
				switch(column){
                case(0): return true; break;
                case(1): return true; break;
                case(2): return true; break;
                case(3): return true; break;
                case(4): return true; break;
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
        case(0): return true; break;
        case(1): return true; break;
        case(2): return true; break;
        case(3): return true; break;
        case(4): return true; break;
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
                case(0):  return true; break;
                case(1):  return true; break;
                case(2):  return true; break;
                case(3):  return true; break;
                case(4):  return true; break;
                case(5):  return true; break;
                }
                break;
        case(1): //Time constant
            switch(numberComboBox){
            case(0):  return true; break;
                case(1):  return true; break;
                case(2):  return true; break;
                case(3):  return true; break;
                case(4):  return true; break;
                case(5):  return true; break;
            }
                break;
        case(2): //Phase
                switch(numberComboBox){
                case(0):  return true; break;
                case(1):  return true; break;
                case(2):  return true; break;
                case(3):  return true; break;
                case(4):  return true; break;
                case(5):  return true; break;
                }
                break;
        case(3): //Filters
                switch(numberComboBox){
                case(0):  return true; break;
                case(1):  return true; break;
                case(2):  return true; break;
                case(3):  return true; break;
                case(4):  return true; break;
                case(5):  return true; break;
                }
                break;
        case(4): //Config
                switch(numberComboBox){
                case(0):  return true; break;
                case(1):  return true; break;
                case(2):  return true; break;
                case(3):  return true; break;
                case(4):  return true; break;
                case(5):  return true; break;
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

float getFloat(int number_of_device){
	if (number_of_device < NUMBER ){ 
		return classLAMPh[number_of_device].getFloat();
	}else{
		return -1;
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


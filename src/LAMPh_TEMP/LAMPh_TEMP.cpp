/*-------------------------------------------------
*
*	This is a template for writing a new library for LAMPh
*	Created by Ilnur Gimazov (ubvfp94@mail.ru) 2018-10-03 (3th October 2018)
*	Lib for LAMPh
*	
*-------------------------------------------------
*/

#include "LAMPh_TEMP.h"

#include <stdio.h>
#include <cmath>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

#include <QDebug>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#define NAME  "LAMPh_TEMP"  	//Name of device
#define DEVICE "TEMP" 			//Name of device
#define FOLDER  "Functions/"	//Folder where file will be created (please do not change this value)
#define TXT "_functions.txt"	//Names of functions will be recorded in this file (please do not change this value)	
#define FUNCTIONS "float Cirnox1050X01(float);float Cirnox1050X82(float);float Cirnox1050X83(float);float Cirnox1030(float);float CrNi_CuNi(float);float Cu_CuFe102(float);float Cu_CuFe114(float);float GaAlAsLS102(float);float CuFeLi422(float);float TempSensor1(float);float TempSensor2(float);float TempSensor3(float);float TempSensorS(float)"  //one can use only void and float functions with only float parameter (don't leave empty space after ";")
#define INFO "The Lib for LAMPh to connect with TEMP" //Info about this Lib or device
#define NUMBER 1 //Only one device can be connected by this Lib (this value can be changed)
#define NONE "None"

class ClassLAMPh{
private:
	char * status_char;
	char * unit_char;
	
    bool Cirnox1050X01_active = false;
    bool Cirnox1050X82_active = false;
    bool Cirnox1050X83_active = false;
    bool Cirnox1030_active = false;
    bool CrNi_CuNi_active = false;
    bool Cu_CuFe102_active = false;
    bool Cu_CuFe114_active = false;
    bool GaAlAsLS102_active = false;
    bool CuFeLi422_active = false;
    bool TempSensor1_active = false;
    bool TempSensor2_active = false;
    bool TempSensor3_active = false;
    bool TempSensorS_active = false;

    QMap<QString, float> tempCirnox1050X01;
    QMap<QString, float> tempCirnox1050X82;
    QMap<QString, float> tempCirnox1050X83;
    QMap<QString, float> tempCirnox1030;
    QMap<QString, float> tempCrNi_CuNi;
    QMap<QString, float> tempCu_CuFe102;
    QMap<QString, float> tempCu_CuFe114;
    QMap<QString, float> tempGaAlAsLS102;
    QMap<QString, float> tempCuFeLi422;
    QMap<QString, float> tempTempSensor1;
    QMap<QString, float> tempTempSensor2;
    QMap<QString, float> tempTempSensor3;
    QMap<QString, float> tempTempSensorS;

    QString nameTempSensorS;
    QString stringTempSensor;
    QStringList listTempSensor;
    //char * listTempSensor_char;
	
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
    ClassLAMPh( ) 
    {
        status_char = "None";
        unit_char = "None";
        stringTempSensor = "";
        nameTempSensorS = "";

        QDir dir;
        dir.cd("temp");
        dir.setFilter(QDir::Files);
        QFileInfoList list = dir.entryInfoList();
        for (int i = 0; i < list.size(); ++i) {
            QFileInfo fileInfo = list.at(i);
            listTempSensor.append(QString("%1").arg(fileInfo.fileName()));
            stringTempSensor.append(QString(";%1").arg(fileInfo.fileName()));
        }
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
	
    float Cirnox1050X01(float parameter){
        if (!Cirnox1050X01_active) {
            QString str;
            str = "temp/";
            str.append("Cirnox1050X01");
            str.append(".txt");
            QFile file (str);
            if(QFileInfo(str).exists()){
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QStringList list_temp;
                QString line;
                QString Temp_X;

                QTextStream in(&file);
                while (!in.atEnd()) {
                    line = in.readLine();
                    list_temp = line.split("\t");
                    list_temp[0].replace(",",".");  //rus_float->eng_float ))) lol
                    list_temp[1].replace(",",".");
                    Temp_X= QString::number(list_temp[0].toFloat(),'f',3);
                    tempCirnox1050X01[Temp_X] = list_temp[1].toFloat();
                }
                file.flush();
                file.close();
            }
            Cirnox1050X01_active = true;
        }else{
            QString parameterStr = QString::number(parameter,'f',3);
            float temp = 0;
            if (tempCirnox1050X01.contains(parameterStr))  temp = tempCirnox1050X01.value(parameterStr);
            return temp;
        }
    }

    float Cirnox1050X82(float parameter){
        if (!Cirnox1050X82_active) {
            QString str;
            str = "temp/";
            str.append("Cirnox1050X82");
            str.append(".txt");
            QFile file (str);
            if(QFileInfo(str).exists()){
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QStringList list_temp;
                QString line;
                QString Temp_X;

                QTextStream in(&file);
                while (!in.atEnd()) {
                    line = in.readLine();
                    list_temp = line.split("\t");
                    list_temp[0].replace(",",".");  //rus_float->eng_float ))) lol
                    list_temp[1].replace(",",".");
                    Temp_X= QString::number(list_temp[0].toFloat(),'f',3);
                    tempCirnox1050X82[Temp_X] = list_temp[1].toFloat();
                }
                file.flush();
                file.close();
            }
            Cirnox1050X82_active = true;
        }else{
            QString parameterStr = QString::number(parameter,'f',3);
            float temp = 0;
            if (tempCirnox1050X82.contains(parameterStr))  temp = tempCirnox1050X82.value(parameterStr);
            return temp;
        }
    }

    float Cirnox1050X83(float parameter){
        if (!Cirnox1050X83_active) {
            QString str;
            str = "temp/";
            str.append("Cirnox1050X83");
            str.append(".txt");
            QFile file (str);
            if(QFileInfo(str).exists()){
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QStringList list_temp;
                QString line;
                QString Temp_X;

                QTextStream in(&file);
                while (!in.atEnd()) {
                    line = in.readLine();
                    list_temp = line.split("\t");
                    list_temp[0].replace(",",".");  //rus_float->eng_float ))) lol
                    list_temp[1].replace(",",".");
                    Temp_X= QString::number(list_temp[0].toFloat(),'f',3);
                    tempCirnox1050X83[Temp_X] = list_temp[1].toFloat();
                }
                file.flush();
                file.close();
            }
            Cirnox1050X83_active = true;
        }else{
            QString parameterStr = QString::number(parameter,'f',3);
            float temp = 0;
            if (tempCirnox1050X83.contains(parameterStr))  temp = tempCirnox1050X83.value(parameterStr);
            return temp;
        }
    }

    float Cirnox1030(float parameter){
        if (!Cirnox1030_active) {
            QString str = "temp/";
            str.append("Cirnox1030");
            str.append(".txt");
            QFile file (str);
            if(QFileInfo(str).exists()){
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QStringList list_temp;
                QString line;
                QString Temp_X;

                QTextStream in(&file);
                while (!in.atEnd()) {
                    line = in.readLine();
                    list_temp = line.split("\t");
                    list_temp[0].replace(",",".");  //rus_float->eng_float ))) lol
                    list_temp[1].replace(",",".");
                    Temp_X= QString::number(list_temp[0].toFloat(),'f',3);
                    tempCirnox1030[Temp_X] = list_temp[1].toFloat();
                }
                file.flush();
                file.close();
            }
            Cirnox1030_active = true;
        }else{
            QString parameterStr = QString::number(parameter,'f',3);
            float temp = 0;
            if (tempCirnox1030.contains(parameterStr))  temp = tempCirnox1030.value(parameterStr);
            return temp;
        }
    }

    float CrNi_CuNi(float parameter){
        if (!CrNi_CuNi_active) {
            QString str = "temp/";
            str.append("CrNi_CuNi");
            str.append(".txt");
            QFile file (str);
            if(QFileInfo(str).exists()){
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QStringList list_temp;
                QString line;
                QString Temp_X;

                QTextStream in(&file);
                while (!in.atEnd()) {
                    line = in.readLine();
                    list_temp = line.split("\t");
                    list_temp[0].replace(",",".");  //rus_float->eng_float ))) lol
                    list_temp[1].replace(",",".");
                    Temp_X= QString::number(list_temp[0].toFloat(),'f',3);
                    tempCrNi_CuNi[Temp_X] = list_temp[1].toFloat();
                }
                file.flush();
                file.close();
            }
            CrNi_CuNi_active = true;
        }else{
            QString parameterStr = QString::number(parameter,'f',3);
            float temp = 0;
            if (tempCrNi_CuNi.contains(parameterStr))  temp = tempCrNi_CuNi.value(parameterStr);
            return temp;
        }
    }

    float Cu_CuFe102(float parameter){
        if (!Cu_CuFe102_active) {
            QString str = "temp/";
            str.append("Cu_CuFe102");
            str.append(".txt");
            QFile file (str);
            if(QFileInfo(str).exists()){
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QStringList list_temp;
                QString line;
                QString Temp_X;

                QTextStream in(&file);
                while (!in.atEnd()) {
                    line = in.readLine();
                    list_temp = line.split("\t");
                    list_temp[0].replace(",",".");  //rus_float->eng_float ))) lol
                    list_temp[1].replace(",",".");
                    Temp_X= QString::number(list_temp[0].toFloat(),'f',3);
                    tempCu_CuFe102[Temp_X] = list_temp[1].toFloat();
                }
                file.flush();
                file.close();
            }
            Cu_CuFe102_active = true;
        }else{
            QString parameterStr = QString::number(parameter,'f',3);
            float temp = 0;
            if (tempCu_CuFe102.contains(parameterStr))  temp = tempCu_CuFe102.value(parameterStr);
            return temp;
        }
    }

    float Cu_CuFe114(float parameter){
        if (!Cu_CuFe114_active) {
            QString str = "temp/";
            str.append("Cu_CuFe114");
            str.append(".txt");
            QFile file (str);
            if(QFileInfo(str).exists()){
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QStringList list_temp;
                QString line;
                QString Temp_X;

                QTextStream in(&file);
                while (!in.atEnd()) {
                    line = in.readLine();
                    list_temp = line.split("\t");
                    list_temp[0].replace(",",".");  //rus_float->eng_float ))) lol
                    list_temp[1].replace(",",".");
                    Temp_X= QString::number(list_temp[0].toFloat(),'f',3);
                    tempCu_CuFe114[Temp_X] = list_temp[1].toFloat();
                }
                file.flush();
                file.close();
            }
            Cu_CuFe114_active = true;
        }else{
            QString parameterStr = QString::number(parameter,'f',3);
            float temp = 0;
            if (tempCu_CuFe114.contains(parameterStr))  temp = tempCu_CuFe114.value(parameterStr);
            return temp;
        }
    }

    float GaAlAsLS102(float parameter){
        if (!GaAlAsLS102_active) {
            QString str = "temp/";
            str.append("GaAlAsLS102");
            str.append(".txt");
            QFile file (str);
            if(QFileInfo(str).exists()){
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QStringList list_temp;
                QString line;
                QString Temp_X;

                QTextStream in(&file);
                while (!in.atEnd()) {
                    line = in.readLine();
                    list_temp = line.split("\t");
                    list_temp[0].replace(",",".");  //rus_float->eng_float ))) lol
                    list_temp[1].replace(",",".");
                    Temp_X= QString::number(list_temp[0].toFloat(),'f',3);
                    tempGaAlAsLS102[Temp_X] = list_temp[1].toFloat();
                }
                file.flush();
                file.close();
            }
            GaAlAsLS102_active = true;
        }else{
            QString parameterStr = QString::number(parameter,'f',3);
            float temp = 0;
            if (tempGaAlAsLS102.contains(parameterStr))  temp = tempGaAlAsLS102.value(parameterStr);
            return temp;
        }
    }

    float CuFeLi422(float parameter){
        if (!CuFeLi422_active) {
            QString str = "temp/";
            str.append("CuFeLi422");
            str.append(".txt");
            QFile file (str);
            if(QFileInfo(str).exists()){
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QStringList list_temp;
                QString line;
                QString Temp_X;

                QTextStream in(&file);
                while (!in.atEnd()) {
                    line = in.readLine();
                    list_temp = line.split("\t");
                    list_temp[0].replace(",",".");  //rus_float->eng_float ))) lol
                    list_temp[1].replace(",",".");
                    Temp_X= QString::number(list_temp[0].toFloat(),'f',3);
                    tempCuFeLi422[Temp_X] = list_temp[1].toFloat();
                }
                file.flush();
                file.close();
            }
            CuFeLi422_active = true;
        }else{
            QString parameterStr = QString::number(parameter,'f',3);
            float temp = 0;
            if (tempCuFeLi422.contains(parameterStr))  temp = tempCuFeLi422.value(parameterStr);
            return temp;
        }
    }

    float TempSensor1(float parameter){
        if (!TempSensor1_active) {
            QString str = "temp/";
            str.append("TempSensor1");
            str.append(".txt");
            QFile file (str);
            if(QFileInfo(str).exists()){
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QStringList list_temp;
                QString line;
                QString Temp_X;

                QTextStream in(&file);
                while (!in.atEnd()) {
                    line = in.readLine();
                    list_temp = line.split("\t");
                    list_temp[0].replace(",",".");  //rus_float->eng_float ))) lol
                    list_temp[1].replace(",",".");
                    Temp_X= QString::number(list_temp[0].toFloat(),'f',3);
                    tempTempSensor1[Temp_X] = list_temp[1].toFloat();
                }
                file.flush();
                file.close();
            }
            TempSensor1_active = true;
        }else{
            QString parameterStr = QString::number(parameter,'f',3);
            float temp = 0;
            if (tempTempSensor1.contains(parameterStr))  temp = tempTempSensor1.value(parameterStr);
            return temp;
        }
    }

    float TempSensor2(float parameter){
        if (!TempSensor2_active) {
            QString str = "temp/";
            str.append("TempSensor2");
            str.append(".txt");
            QFile file (str);
            if(QFileInfo(str).exists()){
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QStringList list_temp;
                QString line;
                QString Temp_X;

                QTextStream in(&file);
                while (!in.atEnd()) {
                    line = in.readLine();
                    list_temp = line.split("\t");
                    list_temp[0].replace(",",".");  //rus_float->eng_float ))) lol
                    list_temp[1].replace(",",".");
                    Temp_X= QString::number(list_temp[0].toFloat(),'f',3);
                    tempTempSensor2[Temp_X] = list_temp[1].toFloat();
                }
                file.flush();
                file.close();
            }
            TempSensor2_active = true;
        }else{
            QString parameterStr = QString::number(parameter,'f',3);
            float temp = 0;
            if (tempTempSensor2.contains(parameterStr))  temp = tempTempSensor2.value(parameterStr);
            return temp;
        }
    }

    float TempSensor3(float parameter){
        if (!TempSensor3_active) {
            QString str = "temp/";
            str.append("TempSensor3");
            str.append(".txt");
            QFile file (str);
            if(QFileInfo(str).exists()){
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QStringList list_temp;
                QString line;
                QString Temp_X;

                QTextStream in(&file);
                while (!in.atEnd()) {
                    line = in.readLine();
                    list_temp = line.split("\t");
                    list_temp[0].replace(",",".");  //rus_float->eng_float ))) lol
                    list_temp[1].replace(",",".");
                    Temp_X= QString::number(list_temp[0].toFloat(),'f',3);
                    tempTempSensor3[Temp_X] = list_temp[1].toFloat();
                }
                file.flush();
                file.close();
            }
            TempSensor3_active = true;
        }else{
            QString parameterStr = QString::number(parameter,'f',3);
            float temp = 0;
            if (tempTempSensor3.contains(parameterStr))  temp = tempTempSensor3.value(parameterStr);
            return temp;
        }
    }

    float TempSensorS(float parameter){
        if (!TempSensorS_active) {
            tempTempSensorS.clear();
            QString str = "temp/";
            str.append(nameTempSensorS);
            QFile file (str);
            if(QFileInfo(str).exists()){
                file.open(QIODevice::ReadOnly | QIODevice::Text);
                QStringList list_temp;
                QString line;
                QString Temp_X;

                QTextStream in(&file);
                while (!in.atEnd()) {
                    line = in.readLine();
                    list_temp = line.split("\t");
                    list_temp[0].replace(",",".");  //rus_float->eng_float ))) lol
                    list_temp[1].replace(",",".");
                    Temp_X= QString::number(list_temp[0].toFloat(),'f',3);
                    tempTempSensorS[Temp_X] = list_temp[1].toFloat();
                }
                file.flush();
                file.close();
            }
            TempSensorS_active = true;
        }else{
            QString parameterStr = QString::number(parameter,'f',3);
            float temp = 0;
            if (tempTempSensorS.contains(parameterStr))  temp = tempTempSensorS.value(parameterStr);
            return temp;
        }
    }

    const char* infoL(){
        QString strInfo = "The Lib for LAMPh to connect with TEMP. "
                          "\nIf you cannot find the Thermal sensor among the functions, "
                          "\nyou can add the required file to the folder \"temp\" and "
                          "\nselect this file in the settings of this device."
                          "\nTo use this file, the item \"TempSensorS\"must first be selected in the Functions field."; //

        if ("" == nameTempSensorS){
            return strInfo.toStdString().c_str();
        }else{
           strInfo.append(QString("\nSelected file: %1").arg(nameTempSensorS)) ;
           return strInfo.toStdString().c_str();
        }
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
        case(0): return "Thermal sensor"; break;
        case(1): return ""; break;
        case(2): return ""; break;
        case(3): return ""; break;
        case(4): return ""; break;
        }
    }

    const char* getQStringList(int row){
        switch(row){
        case(0): return  stringTempSensor.toStdString().c_str(); break;
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
                case(1):  nameTempSensorS=listTempSensor.at(0);  TempSensorS_active = false; return true; break;
                case(2):  nameTempSensorS=listTempSensor.at(1);  TempSensorS_active = false; return true; break;
                case(3):  nameTempSensorS=listTempSensor.at(2);  TempSensorS_active = false; return true; break;
                case(4):  nameTempSensorS=listTempSensor.at(3);  TempSensorS_active = false; return true; break;
                case(5):  nameTempSensorS=listTempSensor.at(4);  TempSensorS_active = false; return true; break;
                case(6):  nameTempSensorS=listTempSensor.at(5);  TempSensorS_active = false; return true; break;
                case(7):  nameTempSensorS=listTempSensor.at(6);  TempSensorS_active = false; return true; break;
                case(8):  nameTempSensorS=listTempSensor.at(7);  TempSensorS_active = false; return true; break;
                case(9):  nameTempSensorS=listTempSensor.at(8);  TempSensorS_active = false; return true; break;
                case(10): nameTempSensorS=listTempSensor.at(9);  TempSensorS_active = false; return true; break;
                case(11): nameTempSensorS=listTempSensor.at(10); TempSensorS_active = false; return true; break;
                case(12): nameTempSensorS=listTempSensor.at(11); TempSensorS_active = false; return true; break;
                case(13): nameTempSensorS=listTempSensor.at(12); TempSensorS_active = false; return true; break;
                case(14): nameTempSensorS=listTempSensor.at(13); TempSensorS_active = false; return true; break;
                case(15): nameTempSensorS=listTempSensor.at(14); TempSensorS_active = false; return true; break;
                case(16): nameTempSensorS=listTempSensor.at(15); TempSensorS_active = false; return true; break;
                case(17): nameTempSensorS=listTempSensor.at(16); TempSensorS_active = false; return true; break;
                case(18): nameTempSensorS=listTempSensor.at(17); TempSensorS_active = false; return true; break;
                case(19): nameTempSensorS=listTempSensor.at(18); TempSensorS_active = false; return true; break;
                case(20): nameTempSensorS=listTempSensor.at(19); TempSensorS_active = false; return true; break;
                case(21): nameTempSensorS=listTempSensor.at(20); TempSensorS_active = false; return true; break;
                }
                break;
        }
    }
};

ClassLAMPh classLAMPh[NUMBER]; 

const char* getName() {return DEVICE;}

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
// //
float Cirnox1050X01(int number_of_device, float parameter){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].Cirnox1050X01(parameter);
    }else{
        return -1;
    }
}

float Cirnox1050X82(int number_of_device, float parameter){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].Cirnox1050X82(parameter);
    }else{
        return -1;
    }
}

float Cirnox1050X83(int number_of_device, float parameter){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].Cirnox1050X83(parameter);
    }else{
        return -1;
    }
}

float Cirnox1030(int number_of_device, float parameter){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].Cirnox1030(parameter);
    }else{
        return -1;
    }
}

float CrNi_CuNi(int number_of_device, float parameter){
	if (number_of_device < NUMBER ){ 
        return classLAMPh[number_of_device].CrNi_CuNi(parameter);
	}else{
		return -1;
	}
}

float Cu_CuFe102(int number_of_device, float parameter){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].Cu_CuFe102(parameter);
    }else{
        return -1;
    }
}

float Cu_CuFe114(int number_of_device, float parameter){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].Cu_CuFe114(parameter);
    }else{
        return -1;
    }
}

float GaAlAsLS102(int number_of_device, float parameter){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].GaAlAsLS102(parameter);
    }else{
        return -1;
    }
}

float CuFeLi422(int number_of_device, float parameter){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].CuFeLi422(parameter);
    }else{
        return -1;
    }
}

float TempSensor1(int number_of_device, float parameter){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].TempSensor1(parameter);
    }else{
        return -1;
    }
}

float TempSensor2(int number_of_device, float parameter){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].TempSensor2(parameter);
    }else{
        return -1;
    }
}

float TempSensor3(int number_of_device, float parameter){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].TempSensor3(parameter);
    }else{
        return -1;
    }
}

float TempSensorS(int number_of_device, float parameter){
    if (number_of_device < NUMBER ){
        return classLAMPh[number_of_device].TempSensorS(parameter);
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


/*-------------------------------------------------
*
*	To work with APPA 205
*	Created by Ilnur Gimazov (ubvfp94@mail.ru) 2018-10-17 (17th October 2018)
*	Lib for LAMPh
*	
*-------------------------------------------------
*/

#ifndef LAMPh_COM_APPA205_LIB_H
#define LAMPh_COM_APPA205_LIB_H

#include "LAMPh_COM_APPA205_global.h"
#include <QString>
#include <QStringList>
#include <QSerialPort>
#include <QSerialPortInfo>

extern "C" {
LAMPHLIBSHARED_EXPORT const char* getName(); // LAMPh will receive Name of this device from this Lib
LAMPHLIBSHARED_EXPORT const char* getInfo(int number_of_device); // LAMPh will receive Info about this device, also createFile() function will be performed
LAMPHLIBSHARED_EXPORT const char* DLLMain(); // The same
LAMPHLIBSHARED_EXPORT const char* getFunctions(); // LAMPh will receive All Functions for this device
LAMPHLIBSHARED_EXPORT bool createFile(); //File with Names of functions will be created

LAMPHLIBSHARED_EXPORT bool connectL(int number_of_device); //This Lib will be connected with the device
LAMPHLIBSHARED_EXPORT const char* getStatus(int number_of_device); //LAMPh will receive Status and Error Messages from Lib

LAMPHLIBSHARED_EXPORT const char* getUnit(int number_of_device); // LAMPh will receive Units (V, mV, A, mA, Ohm, K, Oe, T and so on)
LAMPHLIBSHARED_EXPORT void readData(int number_of_device); //LAMPh send command to read Data from device

LAMPHLIBSHARED_EXPORT float getFloat(int number_of_device);	//LAMPh will receive Data (Float) from device by this Lib

LAMPHLIBSHARED_EXPORT bool startL(int number_of_device); //LAMPh (measurement) is started
LAMPHLIBSHARED_EXPORT bool stopL(int number_of_device); //LAMPh (measurement) is stopped
LAMPHLIBSHARED_EXPORT bool pauseL(int number_of_device); //LAMPh (measurement) is paused
LAMPHLIBSHARED_EXPORT bool exitL(int number_of_device); //LAMPh is closing

LAMPHLIBSHARED_EXPORT bool sendMessage(int number_of_device,const char* const str); //LAMPh can send Message to Lib (CriticalLowTemperature, CriticalHighFiels an so on) 

LAMPHLIBSHARED_EXPORT const char* getRowName(int number_of_device, int row); //LAMPh will receive Row Name for creating Control Panel
LAMPHLIBSHARED_EXPORT const char* getButtonName(int number_of_device, int column, int row); //LAMPh will receive Button Name for creating Control Panel
LAMPHLIBSHARED_EXPORT const char* returnRowData(int number_of_device, int row); //LAMPh will receive Row Data (answer)
LAMPHLIBSHARED_EXPORT bool setParameterButton(int number_of_device, int column, int row, float data_float); //LAMPh will return data (Float) from Control Panel

LAMPHLIBSHARED_EXPORT const char* getLineName(int number_of_device, int row); //LAMPh will receive Line Name for creating Control Panel
LAMPHLIBSHARED_EXPORT bool setParameterLine(int number_of_device, int row, float data_float); //LAMPh will return data (Float) from Control Panel


LAMPHLIBSHARED_EXPORT const char* getComboBoxName(int number_of_device, int row); //LAMPh will receive ComboBox Name for creating Control Panel
LAMPHLIBSHARED_EXPORT const char* getQStringList(int number_of_device, int row); //LAMPh will receive QStringList for creating Control Panel
LAMPHLIBSHARED_EXPORT bool setParameterComboBox(int number_of_device, int row, float data_float); //LAMPh will return data (Float) from Control Panel

}

#endif // LAMPh_COM_APPA205_LIB_H

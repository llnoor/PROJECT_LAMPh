#ifndef LAMPh_ATIME_LIB_H
#define LAMPh_ATIME_LIB_H

#include "LAMPh_ATIME_global.h"
#include <QString>
#include <QStringList>
#include <QtSerialPort/QSerialPort>


extern "C" {
LAMPHLIBSHARED_EXPORT const char* getName();
LAMPHLIBSHARED_EXPORT const char* getInfo(); //also create file with name of functions
LAMPHLIBSHARED_EXPORT const char* DLLMain(); //the same
LAMPHLIBSHARED_EXPORT const char* getFunctions();
LAMPHLIBSHARED_EXPORT bool createFile();

LAMPHLIBSHARED_EXPORT bool connectL(int number_of_device); //This Lib will be connected with the device
LAMPHLIBSHARED_EXPORT const char* getStatus(int number_of_device); //LAMPh will receive Status and Error Messages from Lib

LAMPHLIBSHARED_EXPORT const char* getUnit(int number_of_device); // LAMPh will receive Units (V, mV, A, mA, Ohm, K, Oe, T and so on)
LAMPHLIBSHARED_EXPORT void readData(int number_of_device); //LAMPh send command to read Data from device

LAMPHLIBSHARED_EXPORT float getSeconds(int number_of_device);
LAMPHLIBSHARED_EXPORT float getSecondsFromNull(int number_of_device);

LAMPHLIBSHARED_EXPORT float getMinutes(int number_of_device);
LAMPHLIBSHARED_EXPORT float getMinutesFromNull(int number_of_device);

LAMPHLIBSHARED_EXPORT float getHours(int number_of_device);
LAMPHLIBSHARED_EXPORT float getHoursfromNull(int number_of_device);

LAMPHLIBSHARED_EXPORT float getDays(int number_of_device);
LAMPHLIBSHARED_EXPORT float getDaysFromNull(int number_of_device);

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

#endif // LAMPh_ATIME_LIB_H

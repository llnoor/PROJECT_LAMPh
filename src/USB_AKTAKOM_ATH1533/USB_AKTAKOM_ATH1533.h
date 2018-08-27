#ifndef USB_AKTAKOM_ATH1533_LIB_H
#define USB_AKTAKOM_ATH1533_LIB_H

#include "USB_AKTAKOM_ATH1533_global.h"
#include <QString>
#include <QStringList>
#include <QtSerialPort/QSerialPort>

#include "AULNgdev.h"
#include "ATH1535.h"


extern "C" {
LAMPHLIBSHARED_EXPORT const char* getName(); 
LAMPHLIBSHARED_EXPORT const char* getInfo(); //also create file with name of functions
LAMPHLIBSHARED_EXPORT const char* DLLMain(); //the same
LAMPHLIBSHARED_EXPORT const char* getFunctions();
LAMPHLIBSHARED_EXPORT bool createFile();

LAMPHLIBSHARED_EXPORT bool checkUSB(int number_of_device);
LAMPHLIBSHARED_EXPORT bool setUSBPORT(int number_of_device);
LAMPHLIBSHARED_EXPORT void readData();

LAMPHLIBSHARED_EXPORT void setVoltage(int number_of_device, float parameter = 0);
LAMPHLIBSHARED_EXPORT void setCurrent(int number_of_device, float parameter = 0);

LAMPHLIBSHARED_EXPORT float getVoltage(int number_of_device);
LAMPHLIBSHARED_EXPORT float getCurrent(int number_of_device);

LAMPHLIBSHARED_EXPORT const char* getUnit(int number_of_device);

}

#endif // USB_AKTAKOM_ATH1533_LIB_H

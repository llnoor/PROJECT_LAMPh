#ifndef USB_ATIME_LIB_H
#define USB_ATIME_LIB_H

#include "USB_ATIME_global.h"
#include <QString>
#include <QStringList>
#include <QtSerialPort/QSerialPort>


extern "C" {
/*LAMPHLIBSHARED_EXPORT void inputTest( const char* const str );
LAMPHLIBSHARED_EXPORT const char* outputTest();
LAMPHLIBSHARED_EXPORT QString oddUpper(const QString& str);*/

LAMPHLIBSHARED_EXPORT void setNewDevice (int number_of_devices);



LAMPHLIBSHARED_EXPORT const char* getInfo(); //also create file with name of functions
LAMPHLIBSHARED_EXPORT const char* DLLMain(); //the same
LAMPHLIBSHARED_EXPORT const char* getFunctions();
LAMPHLIBSHARED_EXPORT bool createFile();

LAMPHLIBSHARED_EXPORT const char* getUSBcommands();

LAMPHLIBSHARED_EXPORT bool checkUSB(int number_of_device);
LAMPHLIBSHARED_EXPORT bool setUSBPORT(int number_of_device, const char* const str );
LAMPHLIBSHARED_EXPORT void readData();

LAMPHLIBSHARED_EXPORT float getSeconds(int number_of_device);
LAMPHLIBSHARED_EXPORT float getSecondsFromNull(int number_of_device);

LAMPHLIBSHARED_EXPORT float getMinutes(int number_of_device);
LAMPHLIBSHARED_EXPORT float getMinutesFromNull(int number_of_device);

LAMPHLIBSHARED_EXPORT float getHours(int number_of_device);
LAMPHLIBSHARED_EXPORT float getHoursfromNull(int number_of_device);

LAMPHLIBSHARED_EXPORT float getDays(int number_of_device);
LAMPHLIBSHARED_EXPORT float getDaysFromNull(int number_of_device);




}

#endif // USB_ATIME_LIB_H

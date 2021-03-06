#ifndef LAMPh_VIRTUAL_LIB_H
#define LAMPh_VIRTUAL_LIB_H

#include "LAMPh_VIRTUAL_global.h"
#include <QString>
#include <QStringList>
#include <QtSerialPort/QSerialPort>


extern "C" {
/*LAMPHLIBSHARED_EXPORT void inputTest( const char* const str );
LAMPHLIBSHARED_EXPORT const char* outputTest();
LAMPHLIBSHARED_EXPORT QString oddUpper(const QString& str);*/

LAMPHLIBSHARED_EXPORT void setNewDevice (int number_of_devices);


LAMPHLIBSHARED_EXPORT const char* getName(); 
LAMPHLIBSHARED_EXPORT const char* getInfo(); //also create file with name of functions
LAMPHLIBSHARED_EXPORT const char* DLLMain(); //the same
LAMPHLIBSHARED_EXPORT const char* getFunctions();
LAMPHLIBSHARED_EXPORT bool createFile();

LAMPHLIBSHARED_EXPORT const char* getUSBcommands();

LAMPHLIBSHARED_EXPORT bool checkUSB(int number_of_device);
LAMPHLIBSHARED_EXPORT bool setUSBPORT(int number_of_device, const char* const str );
LAMPHLIBSHARED_EXPORT void readData();


//LAMPHLIBSHARED_EXPORT void tuning( const char* const str );
//LAMPHLIBSHARED_EXPORT void sendParameters( const char* const str );
//LAMPHLIBSHARED_EXPORT void setParameters( const char* const str );


LAMPHLIBSHARED_EXPORT float getFloat(int number_of_device);
LAMPHLIBSHARED_EXPORT float getSin(int number_of_device);
LAMPHLIBSHARED_EXPORT float getCos(int number_of_device);
LAMPHLIBSHARED_EXPORT float getSawtooth(int number_of_device);
LAMPHLIBSHARED_EXPORT float getLine(int number_of_device);
LAMPHLIBSHARED_EXPORT float getRand(int number_of_device);
LAMPHLIBSHARED_EXPORT float getSinP(int number_of_device, float parameter = 0);

LAMPHLIBSHARED_EXPORT void setAlfa(int number_of_device, float parameter = 0);


LAMPHLIBSHARED_EXPORT const char* getUnit(int number_of_device);
LAMPHLIBSHARED_EXPORT const char* getValue(int number_of_device);  // Float + " " + Unit;
LAMPHLIBSHARED_EXPORT const char* getSN(int number_of_device);
LAMPHLIBSHARED_EXPORT bool setCh(int number_of_device, float float_data);



}

#endif // LAMPh_VIRTUAL_LIB_H

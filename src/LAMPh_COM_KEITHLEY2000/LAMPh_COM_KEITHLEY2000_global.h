#ifndef LAMPh_COM_KEITHLEY2000_LIB_GLOBAL_H
#define LAMPh_COM_KEITHLEY2000_LIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LAMPHLIB_LIBRARY)
#  define LAMPHLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LAMPHLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LAMPh_COM_KEITHLEY2000_LIB_GLOBAL_H

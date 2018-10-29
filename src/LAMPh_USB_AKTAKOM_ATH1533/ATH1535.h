//---------------------------------------------------------------------------
#ifndef ATH1535H
#define ATH1535H
//---------------------------------------------------------------------------

/****************************************************************************
 *                       ATH1535
 *
 * Title:    ATH1535.h
 * Purpose:  ATH1535
 *           instrument driver declarations.
 *
 ****************************************************************************/

/*****************************************************************************
 ********************                                *************************
 ******************** ОБЩАЯ ЧАСТЬ (GENERAL AUL PART) *************************
 ********************                                *************************
 *****************************************************************************
 *
 *  Переменные, определения и функции, общие для
 *  всех устройств типа AUL
 *
 *****************************************************************************/

#define AULNMOD(expr)  ATH1535##expr
#define EAULNMOD(expr) EATH1535##expr
#define WAULNMOD(expr) WATH1535##expr
#define TAULNMOD(expr) TATH1535##expr
#include "AULNgdev.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/* ****************************************************************************
 *------------ User-Callable Functions (Exportable Functions) ---------------*
 *****************************************************************************/

/*
 *------------ Specific ATH1535 Functions ---------------
 */

/*
 * функции IsValidDevice и init используют специфическую информацию о
 * конкретном устройстве, но являются шаблонными
 */
DLLFUNC ViStatus _VI_FUNC ATH1535_IsValidDevice (char AULName[]);

DLLFUNC ViStatus _VI_FUNC ATH1535_init (ViRsrc resourceName,
                                ViSession *instrumentHandle,
                                int shared,
                                ViBoolean resetDevice,
                                char path[], char AULDevName[], DWORD *Plugins);

DLLFUNC ViStatus _VI_FUNC ATH1535_Close (ViSession instrumentHandle);

//Информация об устройстве
DLLFUNC ViStatus _VI_FUNC ATH1535_GetChannelCount (ViSession instrumentHandle,
                                              int *val);
DLLFUNC ViStatus _VI_FUNC ATH1535_GetChannelMaxRange (ViSession instrumentHandle,
        int channel, double *maxVoltage, double *maxCurrent);

//Регистр управления, установка
DLLFUNC ViStatus _VI_FUNC ATH1535_SetControl (ViSession instrumentHandle,
                                              int channel, ViByte val);
#define ATH1535_STATUS_REMOTE   0x01  //удаленное управление
#define ATH1535_STATUS_VOLTAGE  0x02  //индикация напряжения
#define ATH1535_STATUS_PRTHERMO 0x10  //термодатчик присутствует
#define ATH1535_STATUS_PRREG    0x20  //регистр присутствует
#define ATH1535_STATUS_PRDAC    0x40  //ЦАП присутствует
#define ATH1535_STATUS_PRADC    0x80  //АЦП присутствует
//Регистр статуса, чтение
DLLFUNC ViStatus _VI_FUNC ATH1535_GetStatus (ViSession instrumentHandle,
                                              int channel, ViByte *val);
//Разрешение выхода, установка
DLLFUNC ViStatus _VI_FUNC ATH1535_SetOutputEnabled (ViSession instrumentHandle,
                                              int channel, int val);
//Разрешение выхода, чтение
DLLFUNC ViStatus _VI_FUNC ATH1535_GetOutputEnabled (ViSession instrumentHandle,
                                              int channel, int *val);
//ЦАП регулировки напряжения, установка
DLLFUNC ViStatus _VI_FUNC ATH1535_SetVoltage (ViSession instrumentHandle,
                                           int channel, double val);
//ЦАП регулировки напряжения, чтение
DLLFUNC ViStatus _VI_FUNC ATH1535_GetVoltage (ViSession instrumentHandle,
                                           int channel, double *val);
//ЦАП регулировки тока, установка
DLLFUNC ViStatus _VI_FUNC ATH1535_SetCurrent (ViSession instrumentHandle,
                                           int channel, double val);
//ЦАП регулировки напряжения, чтение
DLLFUNC ViStatus _VI_FUNC ATH1535_GetCurrent (ViSession instrumentHandle,
                                           int channel, double *val);

//чтения входа АЦП, контролирующего напряжение
DLLFUNC ViStatus _VI_FUNC ATH1535_TestVoltage (ViSession instrumentHandle,
                                           int channel, double *val);
//чтения входа АЦП, контролирующего ток
DLLFUNC ViStatus _VI_FUNC ATH1535_TestCurrent (ViSession instrumentHandle,
                                           int channel, double *val);
//чтения температуры
DLLFUNC ViStatus _VI_FUNC ATH1535_TestTemperature (ViSession instrumentHandle,
                                           double *val);

DLLFUNC ViStatus _VI_FUNC ATH1535_MagnitudeCodeToValue (ViSession instrumentHandle,
                                                  int channel, WORD code, int current,
                                                  double *value, int adc);
DLLFUNC ViStatus _VI_FUNC ATH1535_ValueToMagnitudeCode (ViSession instrumentHandle,
                                                  int channel, double value, int current,
                                                  WORD *code, int adc);

DLLFUNC ViStatus _VI_FUNC ATH1535_SetCalibrate (
    ViSession instrumentHandle,
    int channel,
    int order, // степень полинома, 1 - линейный
    double *volDAC, //массив поправочных коэффициентов для задания напряжение
    double *curDAC, //массив поправочных коэффициентов для задания тока
    double *volADC, //массив поправочных коэффициентов для измерения напряжение
    double *curADC  //массив поправочных коэффициентов для измерения тока
                    // размер массивов = order+1
    );

DLLFUNC ViStatus _VI_FUNC ATH1535_GetCalibrate (
    ViSession instrumentHandle,
    int channel,
    int *order, // степень полинома, 1 - линейный
    double *volDAC, //массив поправочных коэффициентов для задания напряжение
    double *curDAC, //массив поправочных коэффициентов для задания тока
    double *volADC, //массив поправочных коэффициентов для измерения напряжение
    double *curADC  //массив поправочных коэффициентов для измерения тока
                    // размер массивов = order+1
    );

DLLFUNC ViStatus _VI_FUNC ATH1535_reset (ViSession instrumentHandle);

DLLFUNC ViStatus _VI_FUNC ATH1535_GetHardwareVersion(ViSession instrumentHandle,
                                              DWORD *AULHWDate,
                                              DWORD *AULHWVers,
                                              DWORD *PLISVers);

/**************************************************************************** 
 *---------------------------- End Include File ----------------------------* 
 ****************************************************************************/
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
//---------------------------------------------------------------------------
#endif /* ATH1535H */





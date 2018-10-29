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
 ******************** ����� ����� (GENERAL AUL PART) *************************
 ********************                                *************************
 *****************************************************************************
 *
 *  ����������, ����������� � �������, ����� ���
 *  ���� ��������� ���� AUL
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
 * ������� IsValidDevice � init ���������� ������������� ���������� �
 * ���������� ����������, �� �������� ����������
 */
DLLFUNC ViStatus _VI_FUNC ATH1535_IsValidDevice (char AULName[]);

DLLFUNC ViStatus _VI_FUNC ATH1535_init (ViRsrc resourceName,
                                ViSession *instrumentHandle,
                                int shared,
                                ViBoolean resetDevice,
                                char path[], char AULDevName[], DWORD *Plugins);

DLLFUNC ViStatus _VI_FUNC ATH1535_Close (ViSession instrumentHandle);

//���������� �� ����������
DLLFUNC ViStatus _VI_FUNC ATH1535_GetChannelCount (ViSession instrumentHandle,
                                              int *val);
DLLFUNC ViStatus _VI_FUNC ATH1535_GetChannelMaxRange (ViSession instrumentHandle,
        int channel, double *maxVoltage, double *maxCurrent);

//������� ����������, ���������
DLLFUNC ViStatus _VI_FUNC ATH1535_SetControl (ViSession instrumentHandle,
                                              int channel, ViByte val);
#define ATH1535_STATUS_REMOTE   0x01  //��������� ����������
#define ATH1535_STATUS_VOLTAGE  0x02  //��������� ����������
#define ATH1535_STATUS_PRTHERMO 0x10  //����������� ������������
#define ATH1535_STATUS_PRREG    0x20  //������� ������������
#define ATH1535_STATUS_PRDAC    0x40  //��� ������������
#define ATH1535_STATUS_PRADC    0x80  //��� ������������
//������� �������, ������
DLLFUNC ViStatus _VI_FUNC ATH1535_GetStatus (ViSession instrumentHandle,
                                              int channel, ViByte *val);
//���������� ������, ���������
DLLFUNC ViStatus _VI_FUNC ATH1535_SetOutputEnabled (ViSession instrumentHandle,
                                              int channel, int val);
//���������� ������, ������
DLLFUNC ViStatus _VI_FUNC ATH1535_GetOutputEnabled (ViSession instrumentHandle,
                                              int channel, int *val);
//��� ����������� ����������, ���������
DLLFUNC ViStatus _VI_FUNC ATH1535_SetVoltage (ViSession instrumentHandle,
                                           int channel, double val);
//��� ����������� ����������, ������
DLLFUNC ViStatus _VI_FUNC ATH1535_GetVoltage (ViSession instrumentHandle,
                                           int channel, double *val);
//��� ����������� ����, ���������
DLLFUNC ViStatus _VI_FUNC ATH1535_SetCurrent (ViSession instrumentHandle,
                                           int channel, double val);
//��� ����������� ����������, ������
DLLFUNC ViStatus _VI_FUNC ATH1535_GetCurrent (ViSession instrumentHandle,
                                           int channel, double *val);

//������ ����� ���, ��������������� ����������
DLLFUNC ViStatus _VI_FUNC ATH1535_TestVoltage (ViSession instrumentHandle,
                                           int channel, double *val);
//������ ����� ���, ��������������� ���
DLLFUNC ViStatus _VI_FUNC ATH1535_TestCurrent (ViSession instrumentHandle,
                                           int channel, double *val);
//������ �����������
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
    int order, // ������� ��������, 1 - ��������
    double *volDAC, //������ ����������� ������������� ��� ������� ����������
    double *curDAC, //������ ����������� ������������� ��� ������� ����
    double *volADC, //������ ����������� ������������� ��� ��������� ����������
    double *curADC  //������ ����������� ������������� ��� ��������� ����
                    // ������ �������� = order+1
    );

DLLFUNC ViStatus _VI_FUNC ATH1535_GetCalibrate (
    ViSession instrumentHandle,
    int channel,
    int *order, // ������� ��������, 1 - ��������
    double *volDAC, //������ ����������� ������������� ��� ������� ����������
    double *curDAC, //������ ����������� ������������� ��� ������� ����
    double *volADC, //������ ����������� ������������� ��� ��������� ����������
    double *curADC  //������ ����������� ������������� ��� ��������� ����
                    // ������ �������� = order+1
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





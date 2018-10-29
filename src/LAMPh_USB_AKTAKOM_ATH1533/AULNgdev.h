//---------------------------------------------------------------------------
#ifndef AULNgdevH
#define AULNgdevH
//---------------------------------------------------------------------------
/*****************************************************************************
 *   AUL Net general device code header file
 * �������������� ������ ���� ���������� �������� ������ ����������:
 * ...
 * #define AULNMOD(expr)  ABC1234##expr
 * #define EAULNMOD(expr) EABC1234##expr
 * #define WAULNMOD(expr) WABC1234##expr
 * #include "AULNgdev.h"
 * ...
 *****************************************************************************/
#define AULNMOD(expr)  ATH1535##expr
#define EAULNMOD(expr) EATH1535##expr
#define WAULNMOD(expr) WATH1535##expr
#define TAULNMOD(expr) TATH1535##expr
#include <windows.h>
#include "cvitype.h"

#ifdef AULNMODDRV_EXPORTS
#    define DLLFUNC __declspec(dllexport)
#else
#    define DLLFUNC __declspec(dllimport)
#endif

#define LIBIMPORT _VI_FUNC

typedef ViStatus (_VI_FUNC *TProcessSysEventFuncPtr)(int *user_break, ViSession vi, ViString callerName, ViInt32 data);

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/* ErrorCodes */
#define ERRMESS_LEN                   (260)  /* ����� ������ ��������� �� ������               */
enum
{
  EAULNMOD(_NO_ERROR)               =   (0),  /* "��� ������"                                   */
  EAULNMOD(_UNKNOWN)                =  (-1),  /* "����������� ������"                           */
  EAULNMOD(_INVALID_PARAMETERS)     =  (-2),  /* "������������ ��������� �������"               */
  EAULNMOD(_COM_PROBLEM)            =  (-3),  /* "������ ��� ������ ������� � ��������"         */
  EAULNMOD(_TIME_OUT)               =  (-4),  /* "�������� ����� ������� �� ��������"           */
  EAULNMOD(_LOC_OPEN_FAIL)          =  (-5),  /* "������ ��� �������� ���������� ����������"    */
  EAULNMOD(_NET_OPEN_FAIL)          =  (-6),  /* "������ ��� �������� �������� ����������"      */
  EAULNMOD(_INCORRECT_INTERFACE)    =  (-7),  /* "������������ ���������"                       */
  EAULNMOD(_MEMORY_FAIL)            =  (-8),  /* "������ ��� ��������������� � ������� ��"      */
  EAULNMOD(_DLL_NOT_FOUND)          =  (-9),  /* "���������� ��������� ������������ ����������" */
  EAULNMOD(_FUNCTION_NOT_FOUND)     = (-10),  /* "������� �� �������"                           */
  EAULNMOD(_DEV_NOT_OPENED)         = (-11),  /* "������ �� ������"                             */
  EAULNMOD(_INVALID_DEVICE)         = (-12),  /* "������������ ��� AUL-����������"              */
  EAULNMOD(_USER_BREAK)             = (-13),  /* "������� ������� �������������"                */
  EAULNMOD(_FILE_NOT_FOUND)         = (-14),  /* "���� �� ������"                               */
  EAULNMOD(_FILE_OPEN_FAIL)         = (-15),  /* "������ ��� �������� �����"                    */
  EAULNMOD(_FILE_CLOSE_FAIL)        = (-16),  /* "������ ��� �������� �����"                    */
  EAULNMOD(_FILE_READ_FAIL)         = (-17),  /* "������ ��� ������ �����"                      */
  EAULNMOD(_FILE_WRITE_FAIL)        = (-18),  /* "������ ��� ������ �����"                      */
  EAULNMOD(_ACCESS_DENIED)          = (-19),  /* "������ ��������"                              */
  EAULNMOD(_ACCESS_KEY_NOT_FOUND)   = (-20),  /* "���� ������� �� ������"                       */
  EAULNMOD(_INVALID_CMD_FORMAT)     = (-21),  /* "������������ ������ �������, ������� ��� ������" */
  EAULNMOD(_FEATURE_UNSUPPORTED)    = (-22),  /* "������� �� ��������������"                    */
  EAULNMOD(_SERVER_NOT_FOUND)       = (-23),  /* "���������� ���������� ����� � ��������"       */
  EAULNMOD(_DEVICE_LOCKED)          = (-24),  /* "���������� ����������� ������ �������������"  */
  EAULNMOD(_SOCKET_INDEF)           = (-25),  /* "����� �� ���������"                           */
  EAULNMOD(_SOCKET_LOCKED)          = (-26),  /* "����� ������������ �������� �����������, �������� � ��� ���������" */
  /* Warnings */
  WAULNMOD(_UNKNOWN)                =   (1),  /* "����������� ��������������"                   */
  WAULNMOD(_OPERATION_NOT_COMPLETE) =   (2),  /* "�������� �� ���������"                        */
  WAULNMOD(_VOID_DATA)              =   (3),  /* "������ �����������"                           */
  WAULNMOD(_INVALID_VERSION)        =   (4),  /* "������������ ������"                          */
  WAULNMOD(_AUTO_CONTROL)           =   (5),  /* "�������������� ����������"                    */
  WAULNMOD(_REPEATED_ACTION)        =   (6)   /* "��������� ��������"                           */
};

/* Time Out constants */
enum
{
  AULNMOD(_VAL_MAX_TIME_INFINITE)   =  (-1),  /* ����������� ������� (�� �������������)         */
  AULNMOD(_VAL_MAX_TIME_IMMEDIATE)  =   (0)   /* ����������� �������                            */
};

#define INVALID_VISTATUS_VALUE  ((ViStatus)-1)
#define INVALID_VIHANDLER_VALUE ((ViSession)-1)

/* Command codes */
#define AULNCMD_REQACCESS        0  /* ������ �� ������                                     */
#define AULNCMD_OPEN             1  /* int USBDRVOpen (int Index, int shared);              */
#define AULNCMD_CLOSE            2  /* int USBDRVClose (int Index);                         */
#define AULNCMD_ISOPEN           3  /* int USBDRVIsOpened (int Index);                      */
#define AULNCMD_GETDEVCOUNT      4  /* int USBDRVGetAULDeviceCount (void);                  */
#define AULNCMD_READSTATUS       5  /* int USBDRVReadStatus (char *buf, int n, int Index);  */
#define AULNCMD_SENDCOMMAND      6  /* int USBDRVSendCommand (char *buf, int n, int Index); */
#define AULNCMD_READDATA         7  /* int USBDRVReadData (char *buf, int n, int Index);    */
#define AULNCMD_WRITEDATA        8  /* int USBDRVWriteData (char *buf, int n, int Index);   */
#define AULNCMD_GETUSBNAME       9  /* int USBDRVGetUsbDeviceName (char buf[256], int Index); */

#define FILEID_AULN	((DWORD)('A')|(DWORD)('U'<<8)|(DWORD)('L'<<16)|(DWORD)('N'<<24))
#define FILEID_etCm	((DWORD)('e')|(DWORD)('t'<<8)|(DWORD)('C'<<16)|(DWORD)('m'<<24))

/* ��� ���������� */
typedef enum tagANetInterface {
  aniUnknown =-1, /* ����������� */
  aniAUN = 0, /* ����������� ������� AUNUSB */
  aniAUN2,    /* ������� AUNUSB 2           */
  aniALAN,    /* ������� AUNLAN             */
  aniAEPP,    /* ������� EPP-�����          */
  aniAVCOM,   /* ����������� COM-����       */
  aniAUXPR    /* USB Xpress                 */
} TANetInterface;

#define INTERFACE_FIRST aniAUN
#define INTERFACE_LAST  aniAUXPR

/*  ������ �����������
#define INTERFACE_AUN   0
#define INTERFACE_AEPP  1
#define INTERFACE_AVCOM 2
#define INTERFACE_AUXPR 3
#define INTERFACE_AUN2  4
*/

/* ����������������� ���� AUL-���������� */
typedef struct tagAULDEVIDN
{
  TANetInterface interface_type; /* ��� ���������� */
  DWORD id;             /* ������������� (������ ���������� �����������)*/
  union {
    struct {
      DWORD deviceType;     /* ������������� ���� ������� (ACK-, AHP-, AKC-...), AULTHi */
      DWORD deviceID;       /* ������������� ������ ������� (3106, 3121, 3166...), AULTLo */
    } dmDWORD;
    LONGLONG dmQWORD;       /* ��� ������ � ���� 64-������� ������ */
    char dmChar[8];         /* ��� ������ � ���� 8-���������� ������ */
  } devModel;
  BYTE sn[16];          /* ������ ��������� ������ */
  DWORD tag;            /* ������������ �� ������������� ������������� */
  //�������������� ���� (����������� ������)
  DWORD HWDate;         //���� �������� (� ������� 0xDDMMYYYY)
  DWORD HWVers;         //������ �������� (� ������� 0xMaMiReBl)
  DWORD PLISVers;       //������ PLIS
  DWORD dwReserved;     //���������������
} AULDEVIDN;

typedef struct tagAULNETCMD
{
  DWORD AULNetID0;      /* ������������� AULNet0 = MAKE_FILEID("AULN") */
  DWORD AULNetID1;      /* ������������� AULNet1 = MAKE_FILEID("etCm") */
  DWORD version;        /* ������ ���������, ����. 1.0.2.3 = 0x01000203 */
  DWORD command;        /* ��� ������� */
  int error;            /* ��� ������ (��� ������) */
  int retval;           /* ������������ �������� (��� ������) */
  DWORD datasize;       /* ������ ����� ������, ������ ��������������� ������� */
} AULNETCMD;


/*****************************************************************************
 *------------ User-Callable Functions (Exportable Functions) ---------------*
 *****************************************************************************/

/*
 *------------ General AULN Functions ---------------
 */

DLLFUNC ViStatus _VI_FUNC AULNMOD(_BuildAULName) (
   char AULDevName[], AULDEVIDN *idn );

DLLFUNC ViStatus _VI_FUNC AULNMOD(_BuildAULNameArr) (
   char AULDevName[], DWORD *idn );

DLLFUNC ViStatus _VI_FUNC AULNMOD(_BuildAULNetName) (
   char AULNetName[], int SocketIndex, AULDEVIDN *idn );

DLLFUNC ViStatus _VI_FUNC AULNMOD(_GetAULDeviceInfo) (
   ViSession instrumentHandle,
   char AULDevName[],
   int *devIndex,
   TANetInterface *AULInterface,
   int *SocketIndex
);

DLLFUNC ViStatus _VI_FUNC AULNMOD(_GetAULDeviceIDInfo) (
   ViSession instrumentHandle,
   AULDEVIDN *idn
);

DLLFUNC ViStatus _VI_FUNC AULNMOD(_IsOpened) (ViSession instrumentHandle, int *opened);

//DLLFUNC ViStatus _VI_FUNC AULNMOD(_SetProcessSysEventFuncPtr) (void *processSysEventFuncPtr);

DLLFUNC ViStatus _VI_FUNC AULNMOD(_SetProcessSysEventFuncPtr) (TProcessSysEventFuncPtr processSysEventFuncPtr);

DLLFUNC char *   _VI_FUNC AULNMOD(_GetErrorMessage) (ViStatus errorCode);

DLLFUNC ViStatus _VI_FUNC AULNMOD(_SetErrorMessage) (ViStatus errorCode,
                                           ViChar errorMessage[]);

DLLFUNC ViStatus _VI_FUNC AULNMOD(_error_message) (ViSession instrumentHandle,
                                         ViStatus errorCode,
                                         ViChar _VI_FAR errorMessage[]);
DLLFUNC ViStatus _VI_FUNC AULNMOD(_GetLastError) (ViSession instrumentHandle, ViStatus *code,
                                    ViInt32 bufferSize, ViChar description[]);
DLLFUNC ViStatus _VI_FUNC AULNMOD(_ClearDriverError) (ViSession instrumentHandle);

/*+++++++++++++++ ������ ������ �� ������� ������� +++++++++++++++++++++*/
DLLFUNC ViStatus _VI_FUNC AULNMOD(_SocketListCount) (int *count);

DLLFUNC ViStatus _VI_FUNC AULNMOD(_SocketListClear) (void);

DLLFUNC ViStatus _VI_FUNC AULNMOD(_AddSocket) (
    int   *SocketIndex,
    char  *name,
    int   port,
    int   timeout_ms,
    char  *login,
    int   tag
  );

DLLFUNC ViStatus _VI_FUNC AULNMOD(_GetSocket) (
    int   SocketIndex,
    char  *name,
    int   *port,
    DWORD *timeout_ms,
    char  *login,
    int   *tag,
    int   *deleted
  );

DLLFUNC ViStatus _VI_FUNC AULNMOD(_GetDeviceSocket) (
    ViSession instrumentHandle,
    int   *SocketIndex,
    char  *name,
    int   *port,
    DWORD *timeout_ms,
    char  *login,
    int   *tag,
    int   *deleted
  );

//��������� ���������� ������
// ����, � ������� ������� NULL, ������������.
// ��� ��������������� ������ ��� ������� ��������
// ��������� name, port, login ������������ ������
// EAULNMOD(_SOCKET_LOCKED). ��������� timeout_ms, tag
// �������������� ���� ��� ���������� ������.
DLLFUNC ViStatus _VI_FUNC AULNMOD(_SetSocket) (
    int   SocketIndex,
    char  *name,
    int   *port,
    int   *timeout_ms,
    char  *login,
    int   *tag
  );

DLLFUNC ViStatus _VI_FUNC AULNMOD(_DelSocket) (
    int   SocketIndex
  );

DLLFUNC ViStatus _VI_FUNC AULNMOD(_ScanSocket) (
    int   SocketIndex,  //���� -1 - ����������� ���� ������
    TANetInterface ai,  //���� aniUnknown - ����������� ��� ����������
    AULDEVIDN *devList, //��������� �� ����������� ������ ���������
                        //���������, � ���� id - ������ ����������� ����������,
                        //� ���� tag - ������ ������.
    int listsize,       //������ ����������� ������
    int *needsize       //����������� ������ (���������� ��������� ���������)
  );

//������� ������� ��� ������������� �������� � ���������� (��� LabVIEW)
DLLFUNC ViStatus _VI_FUNC AULNMOD(_ScanSocketArr) (
    int   SocketIndex,  //���� -1 - ����������� ���� ������
    TANetInterface ai,  //���� aniUnknown - ����������� ��� ����������
    DWORD *devList,     //��������� �� ����������� ������ ���������
                        //���������, � ���� id - ������ ����������� ����������,
                        //� ���� tag - ������ ������.
    int listsize,       //������ ����������� ������
    int *needsize       //����������� ������ (���������� ��������� ���������)
  );

//DLLFUNC ViStatus _VI_FUNC AULNMOD(_IsValidDevice) (char AULName[]);

/*----------------------------------------------------------------------*/

/****************************************************************************
 *---------------------------- End Include File ----------------------------*
 ****************************************************************************/
#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
//---------------------------------------------------------------------------
#endif /* AULNgdevH */


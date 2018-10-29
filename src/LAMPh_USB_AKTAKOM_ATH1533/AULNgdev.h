//---------------------------------------------------------------------------
#ifndef AULNgdevH
#define AULNgdevH
//---------------------------------------------------------------------------
/*****************************************************************************
 *   AUL Net general device code header file
 * предварительно должно быть определено название модели устройства:
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
#define ERRMESS_LEN                   (260)  /* Длина текста сообщения об ошибке               */
enum
{
  EAULNMOD(_NO_ERROR)               =   (0),  /* "Нет ошибки"                                   */
  EAULNMOD(_UNKNOWN)                =  (-1),  /* "Неизвестная ошибка"                           */
  EAULNMOD(_INVALID_PARAMETERS)     =  (-2),  /* "Неправильные параметры функции"               */
  EAULNMOD(_COM_PROBLEM)            =  (-3),  /* "Ошибка при обмене данными с прибором"         */
  EAULNMOD(_TIME_OUT)               =  (-4),  /* "Исчерпан лимит времени на операцию"           */
  EAULNMOD(_LOC_OPEN_FAIL)          =  (-5),  /* "Ошибка при открытии локального интерфейса"    */
  EAULNMOD(_NET_OPEN_FAIL)          =  (-6),  /* "Ошибка при открытии сетевого интерфейса"      */
  EAULNMOD(_INCORRECT_INTERFACE)    =  (-7),  /* "Неправильный интерфейс"                       */
  EAULNMOD(_MEMORY_FAIL)            =  (-8),  /* "Ошибка при манипулировании с памятью ПК"      */
  EAULNMOD(_DLL_NOT_FOUND)          =  (-9),  /* "Невозможно загрузить динамическую библиотеку" */
  EAULNMOD(_FUNCTION_NOT_FOUND)     = (-10),  /* "Функция не найдена"                           */
  EAULNMOD(_DEV_NOT_OPENED)         = (-11),  /* "Прибор не открыт"                             */
  EAULNMOD(_INVALID_DEVICE)         = (-12),  /* "Неправильный тип AUL-устройства"              */
  EAULNMOD(_USER_BREAK)             = (-13),  /* "Процесс прерван пользователем"                */
  EAULNMOD(_FILE_NOT_FOUND)         = (-14),  /* "Файл не найден"                               */
  EAULNMOD(_FILE_OPEN_FAIL)         = (-15),  /* "Ошибка при открытии файла"                    */
  EAULNMOD(_FILE_CLOSE_FAIL)        = (-16),  /* "Ошибка при закрытии файла"                    */
  EAULNMOD(_FILE_READ_FAIL)         = (-17),  /* "Ошибка при чтении файла"                      */
  EAULNMOD(_FILE_WRITE_FAIL)        = (-18),  /* "Ошибка при записи файла"                      */
  EAULNMOD(_ACCESS_DENIED)          = (-19),  /* "Доступ запрещен"                              */
  EAULNMOD(_ACCESS_KEY_NOT_FOUND)   = (-20),  /* "Ключ доступа не найден"                       */
  EAULNMOD(_INVALID_CMD_FORMAT)     = (-21),  /* "Неправильный формат команды, запроса или ответа" */
  EAULNMOD(_FEATURE_UNSUPPORTED)    = (-22),  /* "Команда не поддерживается"                    */
  EAULNMOD(_SERVER_NOT_FOUND)       = (-23),  /* "Невозможно установить связь с сервером"       */
  EAULNMOD(_DEVICE_LOCKED)          = (-24),  /* "Устройство блокировано другим пользователем"  */
  EAULNMOD(_SOCKET_INDEF)           = (-25),  /* "Сокет не определен"                           */
  EAULNMOD(_SOCKET_LOCKED)          = (-26),  /* "Сокет используется открытым устройством, операции с ним запрещены" */
  /* Warnings */
  WAULNMOD(_UNKNOWN)                =   (1),  /* "Неизвестное предупреждение"                   */
  WAULNMOD(_OPERATION_NOT_COMPLETE) =   (2),  /* "Операция не завершена"                        */
  WAULNMOD(_VOID_DATA)              =   (3),  /* "Данные отсутствуют"                           */
  WAULNMOD(_INVALID_VERSION)        =   (4),  /* "Неправильная версия"                          */
  WAULNMOD(_AUTO_CONTROL)           =   (5),  /* "Автоматическое управление"                    */
  WAULNMOD(_REPEATED_ACTION)        =   (6)   /* "Повторное действие"                           */
};

/* Time Out constants */
enum
{
  AULNMOD(_VAL_MAX_TIME_INFINITE)   =  (-1),  /* Бесконечный таймаут (не отслеживается)         */
  AULNMOD(_VAL_MAX_TIME_IMMEDIATE)  =   (0)   /* Немедленный возврат                            */
};

#define INVALID_VISTATUS_VALUE  ((ViStatus)-1)
#define INVALID_VIHANDLER_VALUE ((ViSession)-1)

/* Command codes */
#define AULNCMD_REQACCESS        0  /* запрос на доступ                                     */
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

/* Тип интерфейса */
typedef enum tagANetInterface {
  aniUnknown =-1, /* неизвестный */
  aniAUN = 0, /* стандартный драйвер AUNUSB */
  aniAUN2,    /* драйвер AUNUSB 2           */
  aniALAN,    /* драйвер AUNLAN             */
  aniAEPP,    /* драйвер EPP-порта          */
  aniAVCOM,   /* виртуальный COM-порт       */
  aniAUXPR    /* USB Xpress                 */
} TANetInterface;

#define INTERFACE_FIRST aniAUN
#define INTERFACE_LAST  aniAUXPR

/*  старые определения
#define INTERFACE_AUN   0
#define INTERFACE_AEPP  1
#define INTERFACE_AVCOM 2
#define INTERFACE_AUXPR 3
#define INTERFACE_AUN2  4
*/

/* идентификационный блок AUL-устройства */
typedef struct tagAULDEVIDN
{
  TANetInterface interface_type; /* тип интерфейса */
  DWORD id;             /* идентификатор (индекс локального подключения)*/
  union {
    struct {
      DWORD deviceType;     /* Идентификатор типа прибора (ACK-, AHP-, AKC-...), AULTHi */
      DWORD deviceID;       /* Идентификатор номера прибора (3106, 3121, 3166...), AULTLo */
    } dmDWORD;
    LONGLONG dmQWORD;       /* Тип модели в виде 64-битного целого */
    char dmChar[8];         /* Тип модели в виде 8-символьной строки */
  } devModel;
  BYTE sn[16];          /* строка серийного номера */
  DWORD tag;            /* используется по необходимости программистом */
  //дополнительные поля (расширенная версия)
  DWORD HWDate;         //дата прошивки (в формате 0xDDMMYYYY)
  DWORD HWVers;         //версия прошивки (в формате 0xMaMiReBl)
  DWORD PLISVers;       //версия PLIS
  DWORD dwReserved;     //зарезервировано
} AULDEVIDN;

typedef struct tagAULNETCMD
{
  DWORD AULNetID0;      /* идентификатор AULNet0 = MAKE_FILEID("AULN") */
  DWORD AULNetID1;      /* идентификатор AULNet1 = MAKE_FILEID("etCm") */
  DWORD version;        /* версия протокола, напр. 1.0.2.3 = 0x01000203 */
  DWORD command;        /* код команды */
  int error;            /* код ошибки (для ответа) */
  int retval;           /* возвращаемое значение (для ответа) */
  DWORD datasize;       /* размер блока данных, должен соответствовать команде */
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

/*+++++++++++++++ Фунции работы со списком сокетов +++++++++++++++++++++*/
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

//Установка параметров сокета
// Поля, в которых передан NULL, игнорируются.
// Для использующегося сокета при попытке изменить
// параметры name, port, login возвращается ошибка
// EAULNMOD(_SOCKET_LOCKED). Параметры timeout_ms, tag
// модифицируются даже при блокировке сокета.
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
    int   SocketIndex,  //если -1 - сканировать весь список
    TANetInterface ai,  //если aniUnknown - сканировать все интерфейсы
    AULDEVIDN *devList, //указатель на заполняемый список найденных
                        //устройств, в поле id - индекс подключения устройства,
                        //в поле tag - индекс сокета.
    int listsize,       //размер переданного списка
    int *needsize       //необходимый размер (количество найденных устройств)
  );

//вариант функции без использования структур в параметрах (для LabVIEW)
DLLFUNC ViStatus _VI_FUNC AULNMOD(_ScanSocketArr) (
    int   SocketIndex,  //если -1 - сканировать весь список
    TANetInterface ai,  //если aniUnknown - сканировать все интерфейсы
    DWORD *devList,     //указатель на заполняемый список найденных
                        //устройств, в поле id - индекс подключения устройства,
                        //в поле tag - индекс сокета.
    int listsize,       //размер переданного списка
    int *needsize       //необходимый размер (количество найденных устройств)
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


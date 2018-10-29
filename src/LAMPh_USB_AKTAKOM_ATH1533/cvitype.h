//---------------------------------------------------------------------------
#ifndef CVITYPEH
#define CVITYPEH
//---------------------------------------------------------------------------
/****************************************************************************
 * CVI types                                                                *
 ****************************************************************************/
#define _VI_FUNC            __stdcall
#define _VI_FAR
#define _VI_PTR             _VI_FAR *
#define _VI_SIGNED          signed

/*- VISA Types --------------------------------------------------------------*/

typedef unsigned long       ViUInt32;
typedef ViUInt32    _VI_PTR ViPUInt32;
typedef ViUInt32    _VI_PTR ViAUInt32;

typedef _VI_SIGNED long     ViInt32;
typedef ViInt32     _VI_PTR ViPInt32;
typedef ViInt32     _VI_PTR ViAInt32;

typedef unsigned short      ViUInt16;
typedef ViUInt16    _VI_PTR ViPUInt16;
typedef ViUInt16    _VI_PTR ViAUInt16;

typedef _VI_SIGNED short    ViInt16;
typedef ViInt16     _VI_PTR ViPInt16;
typedef ViInt16     _VI_PTR ViAInt16;

typedef unsigned char       ViUInt8;
typedef ViUInt8     _VI_PTR ViPUInt8;
typedef ViUInt8     _VI_PTR ViAUInt8;

typedef _VI_SIGNED char     ViInt8;
typedef ViInt8      _VI_PTR ViPInt8;
typedef ViInt8      _VI_PTR ViAInt8;

typedef char                ViChar;
typedef ViChar      _VI_PTR ViPChar;
typedef ViChar      _VI_PTR ViAChar;

typedef unsigned char       ViByte;
typedef ViByte      _VI_PTR ViPByte;
typedef ViByte      _VI_PTR ViAByte;

typedef void        _VI_PTR ViAddr;
typedef ViAddr      _VI_PTR ViPAddr;
typedef ViAddr      _VI_PTR ViAAddr;

typedef float               ViReal32;
typedef ViReal32    _VI_PTR ViPReal32;
typedef ViReal32    _VI_PTR ViAReal32;

typedef double              ViReal64;
typedef ViReal64    _VI_PTR ViPReal64;
typedef ViReal64    _VI_PTR ViAReal64;

typedef ViPByte             ViBuf;
typedef ViPByte             ViPBuf;
typedef ViPByte     _VI_PTR ViABuf;

typedef ViPChar             ViString;
typedef ViPChar             ViPString;
typedef ViPChar     _VI_PTR ViAString;

typedef ViString            ViRsrc;
typedef ViString            ViPRsrc;
typedef ViString    _VI_PTR ViARsrc;

typedef ViUInt16            ViBoolean;
typedef ViBoolean   _VI_PTR ViPBoolean;
typedef ViBoolean   _VI_PTR ViABoolean;

typedef ViInt32             ViStatus;
typedef ViStatus    _VI_PTR ViPStatus;
typedef ViStatus    _VI_PTR ViAStatus;

typedef ViUInt32            ViVersion;
typedef ViVersion   _VI_PTR ViPVersion;
typedef ViVersion   _VI_PTR ViAVersion;

typedef ViUInt32            ViObject;
typedef ViObject    _VI_PTR ViPObject;
typedef ViObject    _VI_PTR ViAObject;

typedef ViObject            ViSession;
typedef ViSession   _VI_PTR ViPSession;
typedef ViSession   _VI_PTR ViASession;

typedef ViUInt32             ViAttr;


//---------------------------------------------------------------------------
#endif /* CVITYPEH */

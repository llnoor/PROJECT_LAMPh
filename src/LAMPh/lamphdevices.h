#ifndef LAMPHDEVICES_H
#define LAMPHDEVICES_H

#include <qmainwindow.h>
#include <qaction.h>
#include <class_write_in_file.h>
#include <dialogdevicesettings.h>
#define CurvCnt 20
#define CurvCounter 5
#define CurvDsf 8225

class QSpinBox;
class QPushButton;
class QLineEdit;
class QLabel;
class QComboBox;
class MainPlot;
class Counter;
class QCheckBox;
class QwtPlotZoomer;
class QwtPlotPicker;
class QwtPlotPanner;
class Plot;
class QPolygon;
class QSerialPort;
class QSerialPortInfo;
class class_write_in_file;
class qserialcomport;

QT_BEGIN_NAMESPACE
class QGroupBox;
QT_END_NAMESPACE

class LAMPhDevices : public QMainWindow
{
    Q_OBJECT

public:
    LAMPhDevices(QString loginQString);
    //~LAMPhDevices();
    int get_numberofdeviceInt();
    void first();

Q_SIGNALS:

    void showLAMPhPlot();
    void showLAMPhDevices();
    void showLAMPhTemp();
    void showDataTable();
    void showLAMPhEdit();
    void showLAMPhSetting();
    void LAMPhExit();
    void send_x_result(float);
    void send_all_results(float,int);

    void send_all_results_to_lamphplot(float,int);

    void setColorSize(int number, int color, int size);
    void setNumberDevice_bool( bool , int );
    void send_numberofdeviceInt(int);
    void send_toolBar_GET_show_data(int);
    void send_toolBar_GET_hide_data(int);


private Q_SLOTS:
    void appendPoints( bool );
    void toolBar_GET_show_data();
    void toolBar_GET_hide_data();
    void update_toolBar_PORTS();
    void openDialogDeviceSettings(int numberofdeviceInt);
    void update_comboBox_Device_Functions(int r,int Index);
    void send_readData();
    void sendColors();
    void setCounter();
    void readData();
    void getAllAvailableSerialPorts();
    void setCheckBox();
    void saveConf();
    void loadConf();
    void loadConfQString(QString);

    void update_comboBox_Function_Parameters(int r,int Index);
    void update_comboBoxes_Function_Parameters(int r,int Index);

private:
    QToolBar *toolBar(); //main
    QToolBar *toolBar_Actions();

    QToolBar *toolBar_GET();
    QToolBar *toolBar_SEND();
    QToolBar *toolBar_COUNTERS();
    QToolBar *toolBar_PORTS();
    QToolBar *toolBar_DEVICES();

    //void showInfo( QString text = QString::null );
    //getAllAvailableSerialPorts:
    QStringList *listDll;
    QStringList *listDllCOM;
    QStringList *listDllSocket;
    QStringList *listDllUSB;
    QStringList *listDllLAN;
    QStringList *listDllLAMPh;
    QStringList *listDllOther;

    int numberofdeviceInt =0;
    int numberofitemsdeviceInt;
    int intCurvDsf =0;

    bool number_of_point[CurvCnt];
    int number_of_point_X;

    int numberofloop=0;

    float dataFloat[CurvCnt];

    QMap<QString, QString> AllAvailableSerialPortsQMap;
    QMap<int, QString> AllAvailableDevicesQMap;
    QMap<QString, QStringList> AllFunctionsDevicesQMap;




    QMap<int, QString> NameDeviceQMap;
    QMap<int, QString> UnitDeviceQMap;
    QMap<int, int> NumberDeviceQMap;
    QMap<int, QString> DLLFileDeviceQMap;
    QMap<int, QStringList> AllFunctionsDeviceQMap;
    QMap<int, QStringList> AllFunctionsParameterDeviceQMap;

    QMap<int, QStringList> AllFunctionsFloatDeviceQMap;
    QMap<int, QStringList> AllFunctionsFloatParameterDeviceQMap;
    QMap<int, QStringList> AllFunctionsVoidDeviceQMap;
    QMap<int, QStringList> AllFunctionsVoidParameterDeviceQMap;

    QMap<int, QStringList> AllFunctionsFloatVoidDeviceQMap;
    QMap<int, QStringList> AllFunctionsFloatVoidParameterDeviceQMap;
    QMap<int, QStringList> AllFunctionsFloatVoidTypeDeviceQMap;

    QMap<int, QString> notBusyCOMDevicesQMap;


    Counter *d_randomCount;
    Counter *d_timerCount;
    QCheckBox *d_symbolType;

    QAction *d_colorsAction;
    QAction *d_startAction;
    QAction *d_clearAction;
    QAction *d_zoomAction;
    QAction *d_exportAction; //PDF
    QAction *d_connectAction;
    QAction *d_sendAction;
    QAction *d_getAction;
    QAction *d_helpAction;
    QAction *d_saveAction;
    QAction *d_loadAction;
    QAction *d_OpenWindow_Main;
    QAction *d_OpenWindow_Devices;
    QAction *d_OpenWindow_Temp;
    QAction *d_OpenWindow_DataTable;
    QAction *d_OpenWindow_Edit;
    QAction *d_OpenWindow_Setting;
    QAction *d_OpenWindow_Exit;

    MainPlot *d_plot;
    class_write_in_file *W_File;

    QwtPlotZoomer *d_zoomer[2];
    QwtPlotPicker *d_picker;
    QwtPlotPanner *d_panner;

    QWidget *hBox_Actions;
    QWidget *hBox_Temp;
    //QWidget *hBox_Devices;
    QWidget *hBox_PlotSize;

    QWidget *hBox_DATA;

    QWidget *hBox_GET;
    QWidget *hBox_SEND;
    QWidget *hBox_COUNTERS;
    QWidget *hBox_PORTS;
    QWidget *hBox_PLOTS;
    QWidget *hBox_DEVICES;

    QLineEdit *lineEdit_Temp_X;
    QLineEdit *lineEdit_Temp_T;

    QLabel *label_Temp;
    QCheckBox *checkBox_Temp;
    QPushButton *Button_Temp;
    QComboBox *combobox_Temp;

    QLabel *label_Devices_All;
    QLabel *label_Devices_All_X;
    QLabel *label_Devices_All_Y;
    QLabel *label_Devices_All_Y2;
    QLabel *label_Devices_All_Show;
    QPushButton *Button_Devices_ClearAll;
    QPushButton *Button_Devices_AutoScaleAll;

    // GET
    int int_GET;
    QLabel *label_label_ReceivedData;
    QLabel *label_comboBox_Device;
    QLabel *label_comboBox_Device_Functions;
    QLabel *label_comboBox_Function_Parameters;
    QLabel *label_lineEdit_NameData;
    QLabel *label_checkBox_Device_Show;
    QLabel *label_checkBox_Device_Show_X;
    QLabel *label_checkBox_Device_Show_Y;
    QLabel *label_checkBox_Device_Text;
    QLabel *label_checkBox_Device_DB;
    QLabel *label_comboBox_ColorData;
    QLabel *label_comboBox_SizeData;

    QLabel *label_ReceivedData[CurvCnt];
    QComboBox *comboBox_Device[CurvCnt];
    QComboBox *comboBox_Device_Functions[CurvCnt]/*[CurvCnt]*/;
    QMap<int, int> comboBox_DeviceQMap; //number,last Item number

    QComboBox *comboBox_Function_Parameters[CurvCnt];
    QLineEdit *lineEdit_NameData[CurvCnt];
    QCheckBox *checkBox_Device_Show[CurvCnt];
    QCheckBox *checkBox_Device_Show_X[CurvCnt];
    QCheckBox *checkBox_Device_Show_Y[CurvCnt];
    QCheckBox *checkBox_Device_Text[CurvCnt];
    QCheckBox *checkBox_Device_DB[CurvCnt];
    QComboBox *comboBox_ColorData[CurvCnt];
    QComboBox *comboBox_SizeData[CurvCnt];

    QPushButton *button_ReceivedData_Close;
    QPushButton *button_ReceivedData_Add;

    // SEND
    int int_SEND;

    QLabel *label_label_SendData;
    QLabel *label_comboBox_SendData_Device;
    QLabel *label_comboBox_SendData_Device_Functions;
    QLabel *label_comboBox_SendData_Function_Parameters;
    QLabel *label_lineEdit_SendData_NameData;
    QLabel *label_checkBox_SendData_Device_Show;
    QLabel *label_checkBox_SendData_Device_Text;
    QLabel *label_checkBox_SendData_Device_DB;
    QLabel *label_comboBox_SendData_ColorData;
    QLabel *label_comboBox_SendData_SizeData;

    QLabel *label_SendData[CurvCnt];
    QComboBox *comboBox_SendData_Device[CurvCnt];
    QComboBox *comboBox_SendData_Device_Functions[CurvCnt]/*[CurvCnt]*/;
    QComboBox *comboBox_SendData_Function_Parameters[CurvCnt];
    QLineEdit *lineEdit_SendData_NameData[CurvCnt];
    QCheckBox *checkBox_SendData_Device_Show[CurvCnt];
    QCheckBox *checkBox_SendData_Device_Text[CurvCnt];
    QCheckBox *checkBox_SendData_Device_DB[CurvCnt];
    QComboBox *comboBox_SendData_ColorData[CurvCnt];
    QComboBox *comboBox_SendData_SizeData[CurvCnt];

    QPushButton *button_SendData_Close;
    QPushButton *button_SendData_Add;

    // COUNTERS
    int int_COUNTERS;

    QLabel *label_label_Counter;
    QLabel *label_lineEdit_Counter_From;
    QLabel *label_lineEdit_Counter_To;
    QLabel *label_lineEdit_Counter_Step;
    QLabel *label_lineEdit_Counter_Acceleration;
    QLabel *label_lineEdit_Counter_Frequency;
    QLabel *label_lineEdit_Counter_Period;
    QLabel *label_lineEdit_Counter_Value;
    QLabel *label_checkBox_Counter_Show_X;
    QLabel *label_checkBox_Counter_Show_Y;
    QLabel *label_checkBox_Counter_Text;
    QLabel *label_comboBox_Counter_DB;
    QLabel *label_comboBox_Counter_ColorData;
    QLabel *label_comboBox_Counter_SizeData;


    QLabel *label_Counter[CurvCounter];
    QLineEdit *lineEdit_Counter_From[CurvCounter];
    QLineEdit *lineEdit_Counter_To[CurvCounter];
    QLineEdit *lineEdit_Counter_Step[CurvCounter];
    QLineEdit *lineEdit_Counter_Acceleration[CurvCounter];
    QLineEdit *lineEdit_Counter_Frequency[CurvCounter];
    QLineEdit *lineEdit_Counter_Period[CurvCounter];
    QLineEdit *lineEdit_Counter_Value[CurvCounter];
    QCheckBox *checkBox_Counter_Show_X[CurvCounter];
    QCheckBox *checkBox_Counter_Show_Y[CurvCounter];
    QCheckBox *checkBox_Counter_Text[CurvCounter];
    QCheckBox *comboBox_Counter_DB[CurvCounter];
    QComboBox *comboBox_Counter_ColorData[CurvCounter];
    QComboBox *comboBox_Counter_SizeData[CurvCounter];

    QPushButton *button_Counter_Close;
    QPushButton *button_Counter_Add;

    // COM PORTS (Sockets, LAN)
    int int_PORTS;

    QLabel *label_Ports;
    QLabel *label_Port[20];
    QPushButton *button_Port_Setting[20];
    QPushButton *button_Ports_Refresh;

    // DEVICES
    int int_DEVICES;

    QLabel *label_label_Device;
    QLabel *label_Device[20];
    QLabel *label_Device_COM_Port[20];
    QPushButton *button_Device_Info[20];
    QPushButton *button_Device_Setting[20];
    QPushButton *Button_Devices_Start[20];
    QPushButton *Button_Devices_Clear[20];
    QPushButton *Button_Devices_AutoScale[20];
    QCheckBox *checkBox_Devices_X[20];
    QCheckBox *checkBox_Devices_Y[20];
    QCheckBox *checkBox_Devices_Y2[20];
    QCheckBox *checkBox_Devices_Show[20];
    int number_of_checkBox;
    int number_of_checkBox_tmp;

    QLabel *label_PlotSize;
    QLineEdit *lineEdit_Plot_x_min;
    QLineEdit *lineEdit_Plot_x_max;
    QLineEdit *lineEdit_Plot_y_min;
    QLineEdit *lineEdit_Plot_y_max;
    QPushButton *Button_PlotSize_replot;
    QPushButton *Button_PlotSize_multiply;
    QPushButton *Button_PlotSize_divide;

    QGroupBox       *groupLAMPhDevices();
    QGroupBox       *groupLAMPhDATA();
    QGroupBox       *groupLAMPhPorts();

    QLabel          *labelPlotSetting;
    QLabel          *labelPlotSettingS;

    QString         *login;

    QPushButton     *newExpButton;
    QPushButton     *editExpButton;
    QPushButton     *tableButton;

    QStringList colorsQStringList = { "white", "black", "cyan", "red", "magenta", "green", "yellow", "blue", "gray", "darkCyan", "darkRed", "darkMagenta", "darkGreen", "darkYellow", "darkBlue", "darkGray", "lightGray" };
    QStringList sizeQStringList = {"line","1","2","4","8"};
    QStringList parametersQStringList = {"Float","None","COUNTER0","COUNTER1","COUNTER2","COUNTER3","COUNTER4",
                                         "DATA0","DATA1","DATA2","DATA3","DATA4","DATA5","DATA6","DATA7","DATA8","DATA9","DATA10","DATA11","DATA12","DATA13","DATA14","DATA15","DATA16","DATA17","DATA18","DATA19"
                                         };

};

#endif // LAMPHDEVICES_H

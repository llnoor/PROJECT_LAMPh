#ifndef LAMPHDEVICES_H
#define LAMPHDEVICES_H

#include <qmainwindow.h>
#include <qaction.h>

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

QT_BEGIN_NAMESPACE
class QGroupBox;
QT_END_NAMESPACE


class LAMPhDevices : public QMainWindow
{
    Q_OBJECT

public:
    LAMPhDevices(QString loginQString);
    //~LAMPhDevices();
    void getDataDll();
    void getAllAvailableSerialPorts();

Q_SIGNALS:

    void showLAMPhPlot();
    void showLAMPhDevices();
    void showLAMPhTemp();
    void showDataTable();
    void showLAMPhEdit();
    void showLAMPhSetting();
    void LAMPhExit();

private Q_SLOTS:
    void toolBar_GET_show_data();
    void toolBar_GET_hide_data();
    void update_toolBar_PORTS();

private:
    QToolBar *toolBar(); //main
    QToolBar *toolBar_Actions();

    QToolBar *toolBar_GET();
    QToolBar *toolBar_SEND();
    QToolBar *toolBar_COUNTERS();
    QToolBar *toolBar_PORTS();
    QToolBar *toolBar_DEVICES();

    //void initWhatsThis();


private:
    //void showInfo( QString text = QString::null );

    //getAllAvailableSerialPorts:
    QStringList *listDll;
    QStringList *listDllCOM;
    QStringList *listDllSocket;
    QStringList *listDllUSB;
    QStringList *listDllLAN;
    QStringList *listDllLAMPh;

    int numberofdeviceInt;

    QMap<QString, QString> AllAvailableSerialPortsQMap;
    QMap<int, QString> AllAvailableDevicesQMap;



    Counter *d_randomCount;
    Counter *d_timerCount;
    QCheckBox *d_symbolType;

    QAction *d_startAction;
    QAction *d_clearAction;
    QAction *d_zoomAction;
    QAction *d_exportAction; //PDF
    QAction *d_connectAction;
    QAction *d_sendAction;
    QAction *d_getAction;
    QAction *d_helpAction;
    //QAction *d_exportAction;

    QAction *d_OpenWindow_Main;
    QAction *d_OpenWindow_Devices;
    QAction *d_OpenWindow_Temp;
    QAction *d_OpenWindow_DataTable;
    QAction *d_OpenWindow_Edit;
    QAction *d_OpenWindow_Setting;
    QAction *d_OpenWindow_Exit;
    //QAction *d_OpenWindow_

    MainPlot *d_plot;

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
    QLabel *label_checkBox_Device_Text;
    QLabel *label_checkBox_Device_DB;
    QLabel *label_comboBox_ColorData;
    QLabel *label_comboBox_SizeData;

    QLabel *label_ReceivedData[20];
    QComboBox *comboBox_Device[20];
    QComboBox *comboBox_Device_Functions[20]/*[20]*/;
    QComboBox *comboBox_Function_Parameters[20];
    QLineEdit *lineEdit_NameData[20];
    QCheckBox *checkBox_Device_Show[20];
    QCheckBox *checkBox_Device_Text[20];
    QCheckBox *checkBox_Device_DB[20];
    QComboBox *comboBox_ColorData[20];
    QComboBox *comboBox_SizeData[20];

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

    QLabel *label_SendData[20];
    QComboBox *comboBox_SendData_Device[20];
    QComboBox *comboBox_SendData_Device_Functions[20]/*[20]*/;
    QComboBox *comboBox_SendData_Function_Parameters[20];
    QLineEdit *lineEdit_SendData_NameData[20];
    QCheckBox *checkBox_SendData_Device_Show[20];
    QCheckBox *checkBox_SendData_Device_Text[20];
    QCheckBox *checkBox_SendData_Device_DB[20];
    QComboBox *comboBox_SendData_ColorData[20];
    QComboBox *comboBox_SendData_SizeData[20];

    QPushButton *button_SendData_Close;
    QPushButton *button_SendData_Add;

    // COUNTERS
    int int_COUNTERS;

    QLabel *label_label_Counter;
    QLabel *label_label_Counter_Line1;
    QLabel *label_lineEdit_Counter_Line1;
    QLabel *label_label_Counter_Line2;
    QLabel *label_lineEdit_Counter_Line2;
    QLabel *label_label_Counter_Line3;

    QLabel *label_Counter[20];
    QLabel *label_Counter_Line1[20];
    QLineEdit *lineEdit_Counter_Line1[20];
    QLabel *label_Counter_Line2[20];
    QLineEdit *lineEdit_Counter_Line2[20];
    QLabel *label_Counter_Line3[20];

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



private:
    QGroupBox       *groupLAMPhDevices();


    QGroupBox       *groupLAMPhDATA();
    QGroupBox       *groupLAMPhPorts();

    QLabel          *labelPlotSetting;
    QLabel          *labelPlotSettingS;

    QString         *login;

    QPushButton     *newExpButton;
    QPushButton     *editExpButton;
    QPushButton     *tableButton;

   


};

#endif // LAMPHDEVICES_H

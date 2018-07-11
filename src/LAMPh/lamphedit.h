#ifndef LAMPHEDIT_H
#define LAMPHEDIT_H

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



class LAMPhEdit : public QMainWindow
{
    Q_OBJECT

public:
    LAMPhEdit(QString loginQString);
    //~LAMPhEdit();
    void getDataDll();

Q_SIGNALS:

    void showLAMPhPlot();
    void showLAMPhDevices();
    void showLAMPhTemp();
    void showDataTable();
    void showLAMPhEdit();
    void showLAMPhSetting();
    void LAMPhExit();



private Q_SLOTS:
    void showRunning( bool );
    void appendPoints( bool );
    void showElapsed( int );

    void moved( const QPoint & );
    void selected( const QPolygon & );
    void exportDocument();
    void enableZoomMode( bool );
    void setCheckBox();


private:
    QToolBar *toolBar(); //main
    QToolBar *toolBar_Actions();
    QToolBar *toolBar_Temp();
    QToolBar *toolBar_Devices();
    QToolBar *toolBar_PlotSize();
    void initWhatsThis();

private:
    void showInfo( QString text = QString::null );


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

    MainPlot *d_edit;

    QwtPlotZoomer *d_zoomer[2];
    QwtPlotPicker *d_picker;
    QwtPlotPanner *d_panner;

    QWidget *hBox_Actions;
    QWidget *hBox_Temp;
    QWidget *hBox_Devices;
    QWidget *hBox_PlotSize;
    //QWidget *hBox_

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

    QLineEdit *lineEdit_Devices[20];
    QLabel *label_Devices[20];
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
    QLabel          *labelPlotSetting;
    QLabel          *labelPlotSettingS;

    QString         *login;

    QPushButton     *newExpButton;
    QPushButton     *editExpButton;
    QPushButton     *tableButton;

   


};

#endif // LAMPHEDIT_H

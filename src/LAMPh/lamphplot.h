#ifndef LAMPHPLOT_H
#define LAMPHPLOT_H

#include <qmainwindow.h>
#include <qaction.h>
#include <lamphedit.h>
#include <lamphsetting.h>
#include <datatable.h>
#include <lamphdevices.h>
#include <lamphtemp.h>
#include <dialogopenfile.h>
#include <dialogsavefile.h>

#define CurvCnt 20

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
class QMouseEvent;
class QMenu;


class LAMPhPlot : public QMainWindow
{
    Q_OBJECT

public:
    LAMPhPlot(QStringList argumentsQStringList);
    //~LAMPhPlot();List

private Q_SLOTS:
    void showRunning( bool );
    void appendPoints( bool );
    void showElapsed( int );

    void moved( const QPoint & );
    void selected( const QPolygon & );
    void exportDocument();
    void enableZoomMode( bool );
    void enableDelMode( bool );
    void setCheckBox();
    void show_all_results(float,int);

    void get_toolBar_GET_show_data(int);
    void get_toolBar_GET_hide_data(int);

    void get_data_PlotSize (float,float,float,float);
    void replot_PlotSize();

    void openFile();
    void functionFile();
    void messageAbout();

    void get_VectorXY(QVector<double>, QVector<double>, int);
    void getMinMaxofVectorXY(double, double, double, double, int);//xmin xmax ymin ymax numberofdata
    void getFileName(QString,int);

    void clear_one(int);
    void autoscale_one(int);
    void saveFile_one(int);

    void mousePressEvent( QMouseEvent * event );
    void mouseReleaseEvent( QMouseEvent * event );
    void keyPressEvent(QKeyEvent *e);

private:
    int mousePressEvent_int;
    int mouseReleaseEvent_int;


    QVector <QVector <double> > stack_zoom;

    QVector <double> vector_to_stack_zoom;

    bool firstPoint_del;
    double x_del[5];
    double y_del[5];

    QToolBar *toolBar(); //main
    QToolBar *toolBar_Actions();
    QToolBar *toolBar_Temp();
    QToolBar *toolBar_Devices();
    QToolBar *toolBar_PlotSize();
    QToolBar *toolBar_Devices_Edit();
    void initWhatsThis();

    //QAction *d_OpenWindow_toolBar;
    QAction *d_OpenWindow_toolBar_Devices;
    QAction *d_OpenWindow_toolBar_PlotSize;
    QAction *d_OpenWindow_toolBar_Devices_Edit;

    void createMenus();
    QMenu *fileMenu;
    QMenu *recentFilesMenu;
    QMenu *helpMenu;

private:
    LAMPhDevices *lamphDevices;
    LAMPhEdit *lamphEdit;
    LAMPhSetting *lamphSetting;
    DataTable *dataTable;
    LAMPhTemp *lamphTemp;

    void showInfo( QString text = QString::null );

    Counter *d_randomCount;
    Counter *d_timerCount;
    QCheckBox *d_symbolType;

    QAction *d_startAction;
    QAction *d_clearAction;
    QAction *d_zoomAction;
    QAction *d_exportAction; //PDF
    QAction *d_connectAction;
    QAction *d_OpenWindow_PlotSize;
    QAction *d_helpAction;
    QAction *d_openAction;
    QAction *d_functionAction;
    QAction *d_deletePoints;

    QAction *d_OpenWindow_Main;
    QAction *d_OpenWindow_Devices;
    QAction *d_OpenWindow_Temp;
    QAction *d_OpenWindow_DataTable;
    QAction *d_OpenWindow_Edit;
    QAction *d_OpenWindow_Setting;
    QAction *d_OpenWindow_About;
    QAction *d_OpenWindow_Exit;
    //QAction *d_OpenWindow_

    MainPlot *d_plot;

    QwtPlotZoomer *d_zoomer[2];
    QwtPlotPicker *d_picker;
    QwtPlotPanner *d_panner;

    QWidget *hBox_Actions;
    QWidget *hBox_Temp;
    QWidget *hBox_Devices;
    QWidget *hBox_PlotSize;
    QWidget *hBox_Devices_Edit;
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


    QLabel *edit_label_Devices_All;
    QPushButton *edit_Button_Devices_ClearAll;
    QPushButton *edit_Button_Devices_AutoScaleAll;

    QLineEdit *edit_lineEdit_Devices[20];
    QPushButton *edit_Button_Devices_Clear[20];
    QPushButton *edit_Button_Devices_AutoScale[20];
    QPushButton *edit_Button_Devices_Save[20];

    QLabel *label_PlotSize;
    QLabel *label_Plot_x_min;
    QLabel *label_Plot_x_max;
    QLabel *label_Plot_y_min;
    QLabel *label_Plot_y_max;

    QLineEdit *lineEdit_Plot_x_min;
    QLineEdit *lineEdit_Plot_x_max;
    QLineEdit *lineEdit_Plot_y_min;
    QLineEdit *lineEdit_Plot_y_max;
    QPushButton *Button_PlotSize_replot;
    QPushButton *Button_PlotSize_multiply;
    QPushButton *Button_PlotSize_divide;



private:
    QString         *login;

    QPushButton     *newExpButton;
    QPushButton     *editExpButton;
    QPushButton     *tableButton;

    //QMenuBar *menu_bar;
    //QMenuBar *menu_barDevices;
    //QLabel *infoLabel;

    QVector <double> qVectorX[20];
    QVector <double> qVectorY[20];

    QVector <QVector <double> > stack_qVectorX[20];
    QVector <QVector <double> > stack_qVectorY[20];

    double xMin[20];
    double xMax[20];
    double yMin[20];
    double yMax[20];
   


};

#endif // LAMPHPLOT_H

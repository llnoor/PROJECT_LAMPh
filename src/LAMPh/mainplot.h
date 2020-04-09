#ifndef _MAINPLOT_H_
#define _MAINPLOT_H_


#include "incrementalplot.h"
#include <qdatetime.h>
#define CurvCnt 20
#define CurvCounter 5

class QTimer;

class MainPlot: public IncrementalPlot
{
    Q_OBJECT

public:
    MainPlot( QWidget *parent );

    virtual QSize sizeHint() const;

Q_SIGNALS:
    void running( bool );
    void elapsed( int ms );
    void running_writeData( bool );
    void running_readData( bool );
    void send_data_PlotSize (float,float,float,float);

public Q_SLOTS:
    void clear();
    void clear_one(int);

    void autoscale();
    void autoscaleXY(double, double, double, double);
    void autoscale_one(int);

    void replot_PlotSize(float,float,float,float);

    void stop();
    void append( int timeout, int count );

    void get_all_results(float , int );
    void get_x_result(float );
    void get_bool( bool ,int);
    void get_numberofdeviceInt(int);

    void appendPointVector(int, QVector <QStringList>);
    void appendPointVectorXY(QVector<double>, QVector<double>, int);
    void appendPointXY(int);

    //void get_bool(bool , int );
    //void appendPoint_F1(float, float);
    //void appendPoint_F2(float, float);
    //void now_replot(float ,float ,float ,float );

private Q_SLOTS:
    void appendPoint();

private:
    void initCurve();

    float x_result;
    float x_result_max;
    float x_result_min;

    float y_result;  // we have float all_results[CurvCnt+CurvCounter] for that !!!
    float y_result_max;
    float y_result_min;

    bool number_of_point[CurvCnt+CurvCounter];

    float all_results[CurvCnt+CurvCounter];
    float all_results_max[CurvCnt+CurvCounter];
    float all_results_min[CurvCnt+CurvCounter];

    QTimer *d_timer;
    int d_timerCount;

    QTime d_timeStamp;


};

#endif // _MAINPLOT_H_

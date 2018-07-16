#ifndef _MAINPLOT_H_
#define _MAINPLOT_H_ 1

#include "incrementalplot.h"
#include <qdatetime.h>

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

public Q_SLOTS:
    void clear();
    void stop();
    void append( int timeout, int count );

    void get_all_results(float , int );
    void get_x_result(float );
    void get_bool( bool ,int);

    //void get_bool(bool , int );
    //void appendPoint_F1(float, float);
    //void appendPoint_F2(float, float);
    //void now_replot(float ,float ,float ,float );

private Q_SLOTS:
    void appendPoint();

private:
    void initCurve();

    float x_result;
    bool number_of_point[CurvCnt];
    float all_results[CurvCnt];

    QTimer *d_timer;
    int d_timerCount;

    QTime d_timeStamp;
};

#endif // _MAINPLOT_H_

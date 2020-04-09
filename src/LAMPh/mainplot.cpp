#include <qglobal.h>
#include <qtimer.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_layout.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_draw.h>
#include "mainplot.h"

#include <cmath>
#include <QDateTime>

const unsigned int c_rangeMax = 1000;

double first =QDateTime::currentDateTime().toTime_t();




MainPlot::MainPlot( QWidget *parent ):
    IncrementalPlot( parent ),
    d_timer( 0 ),
    d_timerCount( 0 )
{
    setFrameStyle( QFrame::NoFrame );
    setLineWidth( 0 );

    plotLayout()->setAlignCanvasToScales( true );

    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->setMajorPen( Qt::gray, 0, Qt::DotLine );
    grid->attach( this );

    setCanvasBackground( QColor( 29, 100, 141 ) ); // nice blue

    setAxisScale( xBottom, 0, 1000);
    setAxisScale( yLeft, -100, 100 );

    replot();

    x_result = 0;
    x_result_max = 0;
    x_result_min = 0;
    y_result_max = 0;
    y_result_min = 0;

    for (int i=0; i<(CurvCnt+CurvCounter); i++)
    {
        number_of_point[i]=0;
        all_results[i]=0;
        all_results_max[i]=0;
        all_results_min[i]=0;
    }

    // enable zooming

    //( void ) new Zoomer( canvas() );
}

QSize MainPlot::sizeHint() const
{
    return QSize( 540, 400 );
}

void MainPlot::get_x_result(float result)
{
    x_result=result;

    if (x_result>x_result_max) x_result_max = x_result;
    if (x_result<x_result_min) x_result_min = x_result;
}

void MainPlot::get_all_results(float result, int new_int)
{
    all_results[new_int]=result;

    if (all_results[new_int]>all_results_max[new_int]) all_results_max[new_int] = all_results[new_int];
    if (all_results[new_int]<all_results_min[new_int]) all_results_min[new_int] = all_results[new_int];

    if (new_int<CurvCnt)  //only DATA without COUNTERS!!!
    {
        y_result = all_results[new_int];
        if (all_results[new_int]>y_result_max) y_result_max = all_results[new_int];
        if (all_results[new_int]<y_result_min) y_result_min = all_results[new_int];
    }
}

void MainPlot::get_bool(bool new_bool, int new_int)
{
    number_of_point[new_int]=new_bool;
}

void MainPlot::get_numberofdeviceInt(int numberofdeviceInt){
    int number =numberofdeviceInt;
    while (number>0) {
       number_of_point[(number-1)]=1;
        number--;
    }
}

void MainPlot::appendPointXY(int r)
{
    IncrementalPlot::appendPoint_S(r, QPointF( x_result , all_results[r] ) );
}


void MainPlot::appendPoint()
{
    Q_EMIT running_writeData( true );

    //IncrementalPlot::appendPoint( QPointF( x+10, x_result ) );

    for (int r=0; r<(CurvCnt); r++)  //for (int r=0; r<(CurvCnt+CurvCounter); r++)
    {
       if (1==number_of_point[r])
       IncrementalPlot::appendPoint_S(r, QPointF( x_result , all_results[r] ) );
    }


    /*if ( --d_timerCount <= 0 )
        stop();*/
}


void MainPlot::appendPointVector(int device_num, QVector <QStringList> qVector_temp){
    qDebug() << "number(MainPlot)" << device_num;
    for(int i=0; i<qVector_temp.size();i++)
    {
        //qDebug() << "qVector_temp.at(i).at(0)" << qVector_temp.at(i).at(0);
        //qDebug() << "qVector_temp.at(i).at(0).toFloat()" << qVector_temp.at(i).at(0).toFloat();
        IncrementalPlot::appendPoint_S(device_num, QPointF( qVector_temp.at(i).at(0).toFloat() , qVector_temp.at(i).at(1).toFloat() ) );
    }
}

void MainPlot::appendPointVectorXY(QVector <double> qVector_X,QVector <double> qVector_Y,int device_num){
    IncrementalPlot::appendPointVectorXY_Incremental(qVector_X,qVector_Y,device_num);
}


void MainPlot::append( int timeout, int count )
{
    if ( !d_timer )
    {
        d_timer = new QTimer( this );
        connect( d_timer, SIGNAL( timeout() ), SLOT( appendPoint() ) );
    }

    d_timerCount = count;

    Q_EMIT running( true );
    d_timeStamp.start();

    QwtPlotCanvas *plotCanvas = qobject_cast<QwtPlotCanvas *>( canvas() );
    plotCanvas->setPaintAttribute( QwtPlotCanvas::BackingStore, false );

    d_timer->start( timeout );
}

void MainPlot::stop()
{
    Q_EMIT elapsed( d_timeStamp.elapsed() );

    if ( d_timer )
    {
        d_timer->stop();
        Q_EMIT running( false );
    }

    QwtPlotCanvas *plotCanvas = qobject_cast<QwtPlotCanvas *>( canvas() );
    plotCanvas->setPaintAttribute( QwtPlotCanvas::BackingStore, true );
}

void MainPlot::clear()
{
    clearPoints();

    x_result_max = x_result;
    x_result_min = x_result;

    y_result_max = y_result;
    y_result_min = y_result;

    for (int i=0; i<(CurvCnt+CurvCounter); i++)
    {
        clearPoints_S(i);

        if (i<CurvCnt) //only DATA without COUNTERS!!!
        {
        all_results_max[i] = all_results[i];
        all_results_min[i] = all_results[i];
        }
    }
    replot(); 
}


void MainPlot::clear_one(int one)
{
    clearPoints_S(one);
    if (one<CurvCnt) //only DATA without COUNTERS!!!
    {
    all_results_max[one] = all_results[one];
    all_results_min[one] = all_results[one];
    }
}

void MainPlot::autoscale()
{
    //setAxisScale( xBottom, (x_result_min<0 ? x_result_min * 1.05  : x_result_min * 0.95) , (x_result_max>0 ? x_result_max * 1.05 : x_result_max * 0.95));
    //setAxisScale( yLeft, (y_result_min<0 ? y_result_min * 1.05 : y_result_min * 0.95) , (y_result_max>0 ? y_result_max * 1.05 : y_result_max * 0.95));

    float x_min = x_result_min-(x_result_max-x_result_min)*0.05;
    float x_max = x_result_max +(x_result_max-x_result_min)*0.05;
    float y_min = y_result_min-(y_result_max-y_result_min)*0.05;
    float y_max = y_result_max +(y_result_max-y_result_min)*0.05;

    setAxisScale( xBottom, x_min , x_max );
    setAxisScale( yLeft, y_min , y_max );

    send_data_PlotSize(x_min,x_max,y_min,y_max);

    replot();
}


void MainPlot::autoscaleXY(double x__min, double x__max, double y__min, double y__max){
    float x_min = x__min-(x__max-x__min)*0.05;
    float x_max = x__max +(x__max-x__min)*0.05;
    float y_min = y__min-(y__max-y__min)*0.05;
    float y_max = y__max +(y__max-y__min)*0.05;

    setAxisScale( xBottom, x_min , x_max );
    setAxisScale( yLeft, y_min , y_max );

    send_data_PlotSize(x_min,x_max,y_min,y_max);

    replot();
}

void MainPlot::autoscale_one(int one)
{
    //setAxisScale( xBottom, (x_result_min<0 ? x_result_min * 1.05  : x_result_min * 0.95) , (x_result_max>0 ? x_result_max * 1.05 : x_result_max * 0.95));
    //setAxisScale( yLeft, (all_results_min[one]<0 ? all_results_min[one] * 1.05 : all_results_min[one] * 0.95) , (all_results_max[one]>0 ? all_results_max[one] * 1.05 : all_results_max[one] * 0.95));

    float x_min = x_result_min-(x_result_max-x_result_min)*0.05;
    float x_max = x_result_max +(x_result_max-x_result_min)*0.05;
    float y_min = all_results_min[one]-(all_results_max[one]-all_results_min[one])*0.05;
    float y_max = all_results_max[one] +(all_results_max[one]-all_results_min[one])*0.05;

    setAxisScale( xBottom, x_min , x_max );
    setAxisScale( yLeft, y_min , y_max );

    send_data_PlotSize(x_min,x_max,y_min,y_max);

    replot();
}

void MainPlot::replot_PlotSize(float x_min, float x_max, float y_min, float y_max)
{
    setAxisScale( xBottom, x_min, x_max);
    setAxisScale( yLeft, y_min, y_max);

    replot();
}

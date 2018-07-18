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
    setAxisScale( yLeft, -2, 2 );

    replot();

    x_result = 0;

    for (int i=0; i<CurvCnt; i++)
    number_of_point[i]=0;

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
}

void MainPlot::get_all_results(float result, int new_int)
{
    all_results[new_int]=result;
}

void MainPlot::get_bool(bool new_bool, int new_int)
{
    number_of_point[new_int]=new_bool;
    //qDebug() << "get_bool bool" << new_bool;
    //qDebug() << "get_bool new_int" << new_int;

}
void MainPlot::get_numberofdeviceInt(int numberofdeviceInt){
    int number =numberofdeviceInt;
    while (number>0) {
       number_of_point[(number-1)]=1;
        number--;
    }
    qDebug() << "get_numberofdeviceInt";
}

void MainPlot::appendPoint()
{
    Q_EMIT running_writeData( true );

    //IncrementalPlot::appendPoint( QPointF( x+10, x_result ) );

    for (int r=0; r<CurvCnt; r++)
    {
       if (1==number_of_point[r])
       IncrementalPlot::appendPoint_S(r, QPointF( x_result , all_results[r] ) );
    }


    /*if ( --d_timerCount <= 0 )
        stop();*/
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
    for (int i=0; i<CurvCnt; i++)
    {
        clearPoints_S(i);
    }
    replot();
}

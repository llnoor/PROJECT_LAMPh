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

void MainPlot::appendPoint()
{
    Q_EMIT running_writeData( true );

    double x;// = qrand() % c_rangeMax;
    //x += ( qrand() % 100 ) / 100;

    double y;// = qrand() % c_rangeMax;
    //y += ( qrand() % 100 ) / 100;

    double second=QDateTime::currentDateTime().toTime_t();
    x=second - first;

    //y = sin(x/10);

    qDebug() << "MainPlot: " << x_result;

    y=rand()%10;
    y=y/10;

    IncrementalPlot::appendPoint( QPointF( x+y, x_result ) );
    IncrementalPlot::appendPoint_S(1, QPointF( x+y, -x_result ) );
    //IncrementalPlot::appendPoint( QPointF( x, y ) );

    if ( --d_timerCount <= 0 )
        stop();
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

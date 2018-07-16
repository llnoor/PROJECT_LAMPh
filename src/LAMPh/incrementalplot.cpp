#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_directpainter.h>
#include <qwt_painter.h>
#include "incrementalplot.h"
#include <qpaintengine.h>

class CurveData: public QwtArraySeriesData<QPointF>
{
public:
    CurveData()
    {
    }

    virtual QRectF boundingRect() const
    {
        if ( d_boundingRect.width() < 0.0 )
            d_boundingRect = qwtBoundingRect( *this );

        return d_boundingRect;
    }

    inline void append( const QPointF &point )
    {
        d_samples += point;
    }

    void clear()
    {
        d_samples.clear();
        d_samples.squeeze();
        d_boundingRect = QRectF( 0.0, 0.0, -1.0, -1.0 );
    }
};

IncrementalPlot::IncrementalPlot( QWidget *parent ):
    QwtPlot( parent )
    //,d_curve( NULL )
{
    d_directPainter = new QwtPlotDirectPainter( this );

    if ( QwtPainter::isX11GraphicsSystem() )
    {
#if QT_VERSION < 0x050000
        canvas()->setAttribute( Qt::WA_PaintOutsidePaintEvent, true );
#endif
        canvas()->setAttribute( Qt::WA_PaintOnScreen, true );
    }

    d_curve = new QwtPlotCurve( "Test Curve" );
    d_curve->setData( new CurveData() );

    for(int i = 0; i < CurvCnt; i++ )
    {
        d_curves[i] = new QwtPlotCurve( "Test Curve 1-10" );
        d_curves[i]->setData( new CurveData() );
    }

    showSymbols( true );

    d_curve->attach( this );
    for(int i = 0; i < CurvCnt; i++ )
    {
        d_curves[i]->attach( this );
    }

    setAutoReplot( false );
}

IncrementalPlot::~IncrementalPlot()
{
    delete d_curve;
    for(int i = 0; i < CurvCnt; i++ )
    {
        delete d_curves[i];
    }

}
/*
void IncrementalPlot::appendPoint( const QPointF &point )
{
    CurveData *data = static_cast<CurveData *>( d_curve->data() );
    data->append( point );

    const bool doClip = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
    if ( doClip )
    {
        //
          // Depending on the platform setting a clip might be an important
          // performance issue. F.e. for Qt Embedded this reduces the
          // part of the backing store that has to be copied out - maybe
          // to an unaccelerated frame buffer device.
         //
        const QwtScaleMap xMap = canvasMap( d_curve->xAxis() );
        const QwtScaleMap yMap = canvasMap( d_curve->yAxis() );

        QRegion clipRegion;

        const QSize symbolSize = d_curve->symbol()->size();
        QRect r( 0, 0, symbolSize.width() + 2, symbolSize.height() + 2 );

        const QPointF center =
            QwtScaleMap::transform( xMap, yMap, point );
        r.moveCenter( center.toPoint() );
        clipRegion += r;

        d_directPainter->setClipRegion( clipRegion );
    }

    d_directPainter->drawSeries( d_curve,
        data->size() - 1, data->size() - 1 );
}

void IncrementalPlot::clearPoints()
{
    CurveData *data = static_cast<CurveData *>( d_curve->data() );
    data->clear();

    replot();
}

void IncrementalPlot::showSymbols( bool on )
{
    if ( on )
    {
        d_curve->setStyle( QwtPlotCurve::NoCurve );
        d_curve->setSymbol( new QwtSymbol( QwtSymbol::XCross,
            Qt::NoBrush, QPen( Qt::white ), QSize( 4, 4 ) ) );
    }
    else
    {
        d_curve->setStyle( QwtPlotCurve::NoCurve );
        d_curve->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
                    QBrush(Qt::red), QPen( Qt::white ), QSize( 3, 3 ) ) );
    }

    replot();
}
*/


void IncrementalPlot::appendPoint( const QPointF &point )
{
    CurveData *data = static_cast<CurveData *>( d_curve->data() );
    data->append( point );

    const bool doClip = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
    if ( doClip )
    {

        const QwtScaleMap xMap = canvasMap( d_curve->xAxis() );
        const QwtScaleMap yMap = canvasMap( d_curve->yAxis() );

        QRegion clipRegion;

        const QSize symbolSize = d_curve->symbol()->size();
        QRect r( 0, 0, symbolSize.width() + 2, symbolSize.height() + 2 );

        const QPointF center =
            QwtScaleMap::transform( xMap, yMap, point );
        r.moveCenter( center.toPoint() );
        clipRegion += r;

        d_directPainter->setClipRegion( clipRegion );
    }

    d_directPainter->drawSeries( d_curve,
        data->size() - 1, data->size() - 1 );

    replot();

}

void IncrementalPlot::clearPoints()
{
    CurveData *data = static_cast<CurveData *>( d_curve->data() );
    data->clear();

    replot();
}


void IncrementalPlot::appendPoint_S(int number, const QPointF &point )
{
    CurveData *data = static_cast<CurveData *>( d_curves[number]->data() );
    data->append( point );

    const bool doClip = !canvas()->testAttribute( Qt::WA_PaintOnScreen );
    if ( doClip )
    {
        /*
           Depending on the platform setting a clip might be an important
           performance issue. F.e. for Qt Embedded this reduces the
           part of the backing store that has to be copied out - maybe
           to an unaccelerated frame buffer device.
         */
        const QwtScaleMap xMap = canvasMap( d_curves[number]->xAxis() );
        const QwtScaleMap yMap = canvasMap( d_curves[number]->yAxis() );

        QRegion clipRegion;

        const QSize symbolSize = d_curves[number]->symbol()->size();
        QRect r( 0, 0, symbolSize.width() + 2, symbolSize.height() + 2 );



        const QPointF center =
            QwtScaleMap::transform( xMap, yMap, point );
        r.moveCenter( center.toPoint() );
        clipRegion += r;

        d_directPainter->setClipRegion( clipRegion );
    }

    d_directPainter->drawSeries( d_curves[number],
        data->size() - 1, data->size() - 1 );
    replot();
}

void IncrementalPlot::clearPoints_S(int number)
{
    CurveData *data = static_cast<CurveData *>( d_curves[number]->data() );
    data->clear();

    replot();
}

void IncrementalPlot::setColorSize(int number, int color, int size){

    d_curves[number]-> setPen(QColor(colorsQStringList.at(color)));
    if (size=0) d_curves[number]-> setStyle( QwtPlotCurve::Lines );
    else{
        d_curves[number]-> setStyle( QwtPlotCurve::NoCurve );
        d_curves[number]->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
            QBrush(QColor(colorsQStringList.at(color))), QPen(QColor(colorsQStringList.at(color))), QSize( sizeQStringList.at(size).toInt(), sizeQStringList.at(size).toInt() ) ) );
    }
}

void IncrementalPlot::showSymbols( bool on )
{
    /*d_curves[0]-> setPen(QColor(colorsQStringList.at(3)));
    d_curves[0]-> setStyle( QwtPlotCurve::Lines );

    d_curves[1]-> setPen(QColor(colorsQStringList.at(3)));
    d_curves[1]-> setStyle( QwtPlotCurve::Lines );*/

    d_curves[0] -> setPen( Qt::white );
    d_curves[0]->setStyle( QwtPlotCurve::NoCurve );
    d_curves[0]->setSymbol( new QwtSymbol( QwtSymbol::XCross,
        Qt::NoBrush, QPen( Qt::white ), QSize( 1, 1 ) ) );


    /*if ( on )
    {
        d_curve -> setPen( Qt::white );
        //d_curve->setStyle( QwtPlotCurve::Lines );
        d_curve->setStyle( QwtPlotCurve::NoCurve );
        d_curve->setSymbol( new QwtSymbol( QwtSymbol::XCross,
            Qt::NoBrush, QPen( Qt::white ), QSize( 1, 1 ) ) ); //5814 ms (375 ms)


        //d_curve->setStyle( QwtPlotCurve::NoCurve );
        //d_curve->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
            //QBrush(Qt::white), QPen( Qt::white ), QSize( 1, 1 ) ) ); //2844 ms


        //d_curve->setPen( Qt::white );
        //d_curve->setStyle( QwtPlotCurve::Lines ); //23000 ms
        //d_curve->setRenderHint( QwtPlotItem::RenderAntialiased ); //37000 ms

        for (int i=0; i<CurvCnt; i++)
        {
            d_curves[i]->setPen( Qt::white );
            d_curve->setStyle( QwtPlotCurve::Lines );
            //d_curves[i]->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
             //   QBrush(Qt::white), QPen( Qt::white ), QSize( 8, 8 ) ) );
        }

    }
    else
    {
        //d_curve->setPen( Qt::white );
        //d_curve->setStyle( QwtPlotCurve::Dots );
        //d_curve->setSymbol( NULL );

        //d_curve -> setSymbol( new QwtSymbol( QwtSymbol::Cross, Qt::NoBrush,
       //     QPen( Qt::black ), QSize( 5, 5 ) ) );
        //d_curve -> setPen( Qt::darkGreen );
        //d_curve -> setStyle( QwtPlotCurve::Lines );
        //d_curve -> setCurveAttribute( QwtPlotCurve::Fitted );


       //d_curve -> setPen( Qt::white );
       //d_curve -> setStyle( QwtPlotCurve::Lines );

        d_curve->setStyle( QwtPlotCurve::NoCurve );
        d_curve->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
            QBrush(QColor("white")), QPen( QColor("white") ), QSize( 4, 4 ) ) );
        for (int i=0; i<CurvCnt; i++)
        {
            d_curves[i]->setStyle( QwtPlotCurve::NoCurve );
            d_curves[i]->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
                QBrush(QColor("white")), QPen( QColor("white") ), QSize( 4, 4 ) ) );
        }
    }

    replot();*/
}


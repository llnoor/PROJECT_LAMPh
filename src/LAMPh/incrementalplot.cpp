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

    for(int i = 0; i < (CurvCnt+CurvCounter); i++ )
    {
        d_curves[i] = new QwtPlotCurve( "Test Curve 1-10" );
        d_curves[i]->setData( new CurveData() );
    }

    for(int i = 0; i < CurvCnt; i++ )
    {
        d_curveXY[i] = new QwtPlotCurve( "Amplitude 1-20" );
        d_curveXY[i]->setRenderHint( QwtPlotItem::RenderAntialiased );
        //d_curveXY[i]->setPen( Qt::yellow );
        d_curveXY[i]->setPen(QColor(colorsQStringList.at(i)));
        d_curveXY[i]->setLegendAttribute( QwtPlotCurve::LegendShowLine );
        d_curveXY[i]->setYAxis( QwtPlot::yLeft );
        //d_curveXY[i]->setYAxis(QwtPlot::yRight);
        d_curveXY[i]->attach( this );
    }

    d_curve_del  = new QwtPlotCurve( "Del" );
    d_curve_del->setRenderHint( QwtPlotItem::RenderAntialiased );
    d_curve_del->setPen( Qt::white );
    d_curve_del->setLegendAttribute( QwtPlotCurve::LegendShowLine );
    d_curve_del->setYAxis( QwtPlot::yLeft );
    d_curve_del->attach( this );

    showSymbols( true );

    d_curve->attach( this );
    for(int i = 0; i < (CurvCnt+CurvCounter); i++ )
    {
        d_curves[i]->attach( this );
    }

    setAutoReplot( false );
}

IncrementalPlot::~IncrementalPlot()
{
    delete d_curve;
    for(int i = 0; i < (CurvCnt+CurvCounter); i++ )
    {
        delete d_curves[i];
    }

    for(int i = 0; i < CurvCnt; i++ )
    {
        delete d_curveXY[i];
    }
    delete d_curve_del;

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


void IncrementalPlot::appendPointVectorXY_Incremental(const QVector <double> &qVector_X, const QVector <double> &qVector_Y,int device_num){
    d_curveXY[device_num]->setSamples(qVector_X,qVector_Y);
    replot();
}

void IncrementalPlot::appendPoint_del(const double *x_del, const double *y_del, int size){
    d_curve_del->setSamples(x_del,y_del,size);
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

    d_curve-> setPen(QColor(colorsQStringList.at(color)));
    if (size==0) d_curve-> setStyle( QwtPlotCurve::Lines );
    else d_curve-> setStyle( QwtPlotCurve::NoCurve );

    d_curve->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
        QBrush(QColor(colorsQStringList.at(color))), QPen(QColor(colorsQStringList.at(color))), QSize( sizeQStringList.at(size).toInt(), sizeQStringList.at(size).toInt() ) ) );



    d_curves[number]-> setPen(QColor(colorsQStringList.at(color)));
    if (size==0) d_curves[number]-> setStyle( QwtPlotCurve::Lines );
    else d_curves[number]-> setStyle( QwtPlotCurve::NoCurve );

    d_curves[number]->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
        QBrush(QColor(colorsQStringList.at(color))), QPen(QColor(colorsQStringList.at(color))), QSize( sizeQStringList.at(size).toInt(), sizeQStringList.at(size).toInt() ) ) );

    //qDebug() << "number" << number;
    //qDebug() << "color" << color;
    //qDebug() << "size" << size;

}

void IncrementalPlot::showSymbols( bool on )
{





    if ( on )
    {
        d_curve -> setPen(QColor(colorsQStringList.at(3)));
        d_curve->setStyle( QwtPlotCurve::Lines );
        d_curve->setStyle( QwtPlotCurve::NoCurve );
        d_curve->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
                    QBrush(QColor(colorsQStringList.at(3))), QPen(QColor(colorsQStringList.at(3))), QSize( 4, 4 ) ) );

        for (int i=0; i<(CurvCnt+CurvCounter); i++)
        {
            int color_int=0; if (i<colorsQStringList.size()) color_int=i;

            d_curves[i]->setPen(QColor(colorsQStringList.at(color_int)));
            d_curves[i]->setStyle( QwtPlotCurve::Lines );
            //d_curves[i]->setStyle( QwtPlotCurve::NoCurve );
            d_curves[i]->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
                        QBrush(QColor(colorsQStringList.at(color_int))), QPen(QColor(colorsQStringList.at(color_int))), QSize( 0, 0 ) ) );
        }
    }
    else
    {
        d_curve -> setPen( Qt::red );
        d_curve -> setStyle( QwtPlotCurve::Lines );
        d_curve->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
                    QBrush(Qt::red), QPen( Qt::red ), QSize( 0, 0 ) ) );

        for (int i=0; i<(CurvCnt+CurvCounter); i++)
        {
            d_curves[i] -> setPen( Qt::red );
            d_curves[i] -> setStyle( QwtPlotCurve::Lines );
            d_curves[i]->setSymbol( new QwtSymbol( QwtSymbol::Ellipse,
                        QBrush(Qt::red), QPen( Qt::red ), QSize( 0, 0 ) ) );
        }
    }

    replot();
}


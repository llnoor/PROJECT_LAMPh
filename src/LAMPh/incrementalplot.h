#ifndef _INCREMENTALPLOT_H_
#define _INCREMENTALPLOT_H_ 1

#include <qwt_plot.h>
#define CurvCnt 20

class QwtPlotCurve;
class QwtPlotDirectPainter;

class IncrementalPlot : public QwtPlot
{
    Q_OBJECT

public:
    IncrementalPlot( QWidget *parent = NULL );
    virtual ~IncrementalPlot();

    void appendPoint( const QPointF & );
    void clearPoints();

    void appendPoint_S(int number, const QPointF & );
    void clearPoints_S(int number);

public Q_SLOTS:
    void showSymbols( bool );

private:
    QwtPlotCurve *d_curve;
    QwtPlotCurve *d_curves[CurvCnt];
    QwtPlotDirectPainter *d_directPainter;
};

#endif // _INCREMENTALPLOT_H_

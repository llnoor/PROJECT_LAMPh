#ifndef _INCREMENTALPLOT_H_
#define _INCREMENTALPLOT_H_ 1

#include <qwt_plot.h>
#define CurvCnt 20
#define CurvCounter 5

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
    void setColorSize(int number, int color, int size);

private:
    QwtPlotCurve *d_curve;
    QwtPlotCurve *d_curves[CurvCnt+CurvCounter];
    QwtPlotDirectPainter *d_directPainter;
    QStringList colorsQStringList = { "white", "black", "cyan", "red", "magenta", "green", "yellow", "blue", "gray", "darkCyan", "darkRed", "darkMagenta", "darkGreen", "darkYellow", "darkBlue", "darkGray", "lightGray" };
    QStringList sizeQStringList = {"line","1","2","4","8"};


};

#endif // _INCREMENTALPLOT_H_

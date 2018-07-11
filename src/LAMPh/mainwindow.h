#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_ 1

#include <qmainwindow.h>
#include <qaction.h>

class QSpinBox;
class QPushButton;
class RandomPlot;
class Counter;
class QCheckBox;
class QwtPlotZoomer;
class QwtPlotPicker;
class QwtPlotPanner;
class Plot;
class QPolygon;

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();

private Q_SLOTS:
    void showRunning( bool );
    void appendPoints( bool );
    void showElapsed( int );

    void moved( const QPoint & );
    void selected( const QPolygon & );
    void exportDocument();
    void enableZoomMode( bool );

private:
    QToolBar *toolBar();
    void initWhatsThis();

private:
    void showInfo( QString text = QString::null );

    Counter *d_randomCount;
    Counter *d_timerCount;
    QCheckBox *d_symbolType;
    QAction *d_startAction;
    QAction *d_clearAction;
    RandomPlot *d_plot;

    QwtPlotZoomer *d_zoomer[2];
    QwtPlotPicker *d_picker;
    QwtPlotPanner *d_panner;

};

#endif

#include <qlabel.h>
#include <qlayout.h>
#include <qstatusbar.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qgroupbox.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qwhatsthis.h>
#include <qpixmap.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_renderer.h>
#include "mainplot.h"

#include "lamphedit.h"
#include "start.xpm"
#include "clear.xpm"
#include "pixmaps.h"

#include <QLineEdit>
#include <QFileDialog>
#include <QPushButton>



class MyToolBar: public QToolBar
{
public:
    MyToolBar( LAMPhEdit *parent ):
        QToolBar( parent )
    {
    }
    void addSpacing( int spacing )
    {
        QLabel *label = new QLabel( this );
        addWidget( label );
        label->setFixedWidth( spacing );
    }
};

class Counter: public QWidget
{
public:
    Counter( QWidget *parent,
            const QString &prefix, const QString &suffix,
            int min, int max, int step ):
        QWidget( parent )
    {
        QHBoxLayout *layout = new QHBoxLayout( this );

        if ( !prefix.isEmpty() )
            layout->addWidget( new QLabel( prefix + " ", this ) );

        d_counter = new QSpinBox( this );
        d_counter->setRange( min, max );
        d_counter->setSingleStep( step );
        layout->addWidget( d_counter );

        if ( !suffix.isEmpty() )
            layout->addWidget( new QLabel( QString( " " ) + suffix, this ) );
    }

    void setValue( int value ) { d_counter->setValue( value ); }
    int value() const { return d_counter->value(); }

private:
    QSpinBox *d_counter;
};

class Zoomer: public QwtPlotZoomer
{
public:
    Zoomer( int xAxis, int yAxis, QWidget *canvas ):
        QwtPlotZoomer( xAxis, yAxis, canvas )
    {
        setTrackerMode( QwtPicker::AlwaysOff );
        setRubberBand( QwtPicker::NoRubberBand );

        // RightButton: zoom out by 1
        // Ctrl+RightButton: zoom out to full size

        setMousePattern( QwtEventPattern::MouseSelect2,
            Qt::RightButton, Qt::ControlModifier );
        setMousePattern( QwtEventPattern::MouseSelect3,
            Qt::RightButton );
    }
};

LAMPhEdit::LAMPhEdit(QString loginQString)
{

    addToolBar(Qt::TopToolBarArea, toolBar());
    addToolBar(Qt::LeftToolBarArea, toolBar_Devices());
    addToolBar(Qt::LeftToolBarArea, toolBar_PlotSize());


#ifndef QT_NO_STATUSBAR
    ( void )statusBar();
#endif


    login = new QString();
    *login = loginQString;

    //labelPlotSettingS = new QLabel(tr("LAMPhEdit"));


    /*QLabel       lbl("this is the example text");
    QLibrary     lib("dynlibd");

    typedef QString (*Fct) (const QString&);
    Fct fct = (Fct)(lib.resolve("oddUpper"));
    if (fct) {
        labelPlotSettingS->setText("fct(lbl.text())");
    }*/


    //setCentralWidget( labelPlotSettingS );

    getDataDll();
    //initWhatsThis();

    setContextMenuPolicy( Qt::NoContextMenu );


    connect( d_OpenWindow_Main, SIGNAL( triggered() ), this, SIGNAL(showLAMPhPlot()) );
    connect( d_OpenWindow_Main, SIGNAL( triggered() ), this, SLOT(close()) );
    connect( d_OpenWindow_Devices, SIGNAL( triggered() ), this, SIGNAL(showLAMPhDevices()) );
    connect( d_OpenWindow_Devices, SIGNAL( triggered() ), this, SLOT(close()) );
    connect( d_OpenWindow_Temp, SIGNAL( triggered() ), this, SIGNAL(showLAMPhTemp()) );
    connect( d_OpenWindow_Temp, SIGNAL( triggered() ), this, SLOT(close()) );
    connect( d_OpenWindow_DataTable, SIGNAL( triggered() ), this, SIGNAL(showDataTable()) );
    connect( d_OpenWindow_DataTable, SIGNAL( triggered() ), this, SLOT(close()) );
    connect( d_OpenWindow_Edit, SIGNAL( triggered() ), this, SIGNAL(showLAMPhEdit()) );
    connect( d_OpenWindow_Edit, SIGNAL( triggered() ), this, SLOT(close()) );
    connect( d_OpenWindow_Setting, SIGNAL( triggered() ), this, SIGNAL(showLAMPhSetting()) );
    connect( d_OpenWindow_Setting, SIGNAL( triggered() ), this, SLOT(close()) );

    //connect( d_OpenWindow_Exit, SIGNAL( triggered() ), this, SIGNAL(LAMPhExit()) );
    //connect( d_OpenWindow_Exit, SIGNAL( triggered() ), this, SLOT(close()) );

    d_edit = new MainPlot( this );
    const int margin = 4;
    d_edit->setContentsMargins( margin, margin, margin, margin );
    d_edit->setCanvasBackground(QColor( Qt::white ) );


            //setCanvasBackground( QColor( 29, 100, 141 ) );

    d_zoomer[0] = new Zoomer( QwtPlot::xBottom, QwtPlot::yLeft,
        d_edit->canvas() );
    d_zoomer[0]->setRubberBand( QwtPicker::RectRubberBand );
    d_zoomer[0]->setRubberBandPen( QColor( Qt::green ) );
    d_zoomer[0]->setTrackerMode( QwtPicker::ActiveOnly );
    d_zoomer[0]->setTrackerPen( QColor( Qt::white ) );

    d_panner = new QwtPlotPanner( d_edit->canvas() );
    d_panner->setMouseButton( Qt::MidButton );

    d_picker = new QwtPlotPicker( QwtPlot::xBottom, QwtPlot::yLeft,
            QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn,
            d_edit->canvas() );
        d_picker->setStateMachine( new QwtPickerDragPointMachine() );
        d_picker->setRubberBandPen( QColor( Qt::green ) );
        d_picker->setRubberBand( QwtPicker::CrossRubberBand );
        d_picker->setTrackerPen( QColor( Qt::white ) );

    setCentralWidget( d_edit );

    centralWidget()->setStyleSheet("background:white;");
    //setStyleSheet("background:white;");

    initWhatsThis();

    setContextMenuPolicy( Qt::NoContextMenu );

    connect( d_picker, SIGNAL( moved( const QPoint & ) ),
        SLOT( moved( const QPoint & ) ) );
    connect( d_picker, SIGNAL( selected( const QPolygon & ) ),
        SLOT( selected( const QPolygon & ) ) );

    enableZoomMode( false );


    connect( d_startAction, SIGNAL( toggled( bool ) ), this, SLOT( appendPoints( bool ) ) );
    connect( d_clearAction, SIGNAL( triggered() ), d_edit, SLOT( clear() ) );
    connect( d_zoomAction, SIGNAL( toggled( bool ) ), SLOT( enableZoomMode( bool ) ) );
    connect( d_exportAction, SIGNAL( triggered() ), this, SLOT( exportDocument() ) );


    connect( d_openAction, SIGNAL( triggered() ), this, SLOT( openFile() ) );
    connect( d_functionAction, SIGNAL( triggered() ), this, SLOT( functionFile() ) );



    //connect( d_symbolType, SIGNAL( toggled( bool ) ), d_edit, SLOT( showSymbols( bool ) ) );
    connect( d_edit, SIGNAL( running( bool ) ), this, SLOT( showRunning( bool ) ) );
    connect( d_edit, SIGNAL( elapsed( int ) ), this, SLOT( showElapsed( int ) ) );



    for(int i=0; i<20; i++)
    {
        connect(Button_Devices_Clear[i], static_cast<void(QPushButton::*)(bool)>(&QPushButton::clicked),
                [=](bool bool_one){
                clear_one(i);
           });
        connect(Button_Devices_AutoScale[i], static_cast<void(QPushButton::*)(bool)>(&QPushButton::clicked),
                [=](bool bool_one){
                autoscale_one(i);
           });
    }


    for (int i=0;i<20;i++){
        xMin[i]=0;
        xMax[i]=1;
        yMin[i]=0;
        yMax[i]=1;
    }


    setWindowTitle(tr("LAMPhEdit - %1 ").arg(login->toLower()));
    //showFullScreen();
    showMaximized();
    //resize(1000, 800);
    //setWindowState(Qt::WindowMaximized);

}

QToolBar *LAMPhEdit::toolBar()
{
    MyToolBar *toolBar = new MyToolBar( this );

    toolBar->setAllowedAreas( Qt::TopToolBarArea | Qt::BottomToolBarArea );
    setToolButtonStyle( Qt::ToolButtonTextUnderIcon );

    d_startAction = new QAction( QPixmap( start_xpm ), "Start", toolBar );
    d_startAction->setCheckable( true );
    d_clearAction = new QAction( QPixmap( clear_xpm ), "Clear", toolBar );

    d_zoomAction = new QAction( QPixmap( zoom_xpm ), "Zoom", toolBar );
    d_zoomAction->setCheckable( true );
    d_exportAction = new QAction( QPixmap( print_xpm ), "Export", toolBar );


    d_openAction = new QAction( QPixmap( start_xpm ), "OpenFile", toolBar );
    d_functionAction = new QAction( QPixmap( start_xpm ), "Function", toolBar );

    QAction *whatsThisAction = QWhatsThis::createAction( toolBar );
    whatsThisAction->setText( "Help" );


    toolBar->addAction( d_openAction );
    toolBar->addAction( d_functionAction );
    //toolBar->addAction( d_startAction );
    toolBar->addAction( d_clearAction );
    toolBar->addAction( d_zoomAction );
    toolBar->addAction( d_exportAction );
    toolBar->addAction( whatsThisAction );
    toolBar->addSeparator();

    setIconSize( QSize( 22, 22 ) );

    QWidget *hBox = new QWidget( toolBar );

    /*d_symbolType = new QCheckBox( "Symbols", hBox );
    d_symbolType->setChecked( true );

    d_randomCount =
        new Counter( hBox, "Points", QString::null, 1, 100000, 100 );
    d_randomCount->setValue( 1000 );

    d_timerCount = new Counter( hBox, "Delay", "ms", 0, 100000, 100 );
    d_timerCount->setValue( 0 );*/

    QHBoxLayout *layout = new QHBoxLayout( hBox );
    layout->setMargin( 0 );
    layout->setSpacing( 0 );
    layout->addSpacing( 10 );
    layout->addWidget( new QWidget( hBox ), 10 ); // spacer
    //layout->addWidget( d_symbolType );
    //layout->addSpacing( 5 );
    //layout->addWidget( d_randomCount );
    //layout->addSpacing( 5 );
   //layout->addWidget( d_timerCount );

    showRunning( false );

    toolBar->addWidget( hBox );

    d_OpenWindow_Main = new QAction( QPixmap( start_xpm ), "Main Plot", toolBar );
    d_OpenWindow_Devices = new QAction( QPixmap( start_xpm ), "Devices", toolBar );
    d_OpenWindow_Temp = new QAction( QPixmap( start_xpm ), "Temp", toolBar );
    d_OpenWindow_DataTable = new QAction( QPixmap( start_xpm ), "DataTable", toolBar );
    d_OpenWindow_Edit = new QAction( QPixmap( start_xpm ), "Edit", toolBar );
    d_OpenWindow_Setting = new QAction( QPixmap( start_xpm ), "Setting", toolBar );
    d_OpenWindow_Exit = new QAction( QPixmap( start_xpm ), "Exit", toolBar );
    d_OpenWindow_Edit->setEnabled(false);
    d_OpenWindow_Exit->setEnabled(false);

    toolBar->addSeparator();

    toolBar->addAction( d_OpenWindow_Main );
    toolBar->addAction( d_OpenWindow_Devices );
    toolBar->addAction( d_OpenWindow_Temp );
    toolBar->addAction( d_OpenWindow_DataTable );
    toolBar->addAction( d_OpenWindow_Edit );
    toolBar->addAction( d_OpenWindow_Setting );
    toolBar->addAction( d_OpenWindow_Exit );

    return toolBar;
}



QToolBar *LAMPhEdit::toolBar_Devices()
{
    MyToolBar *toolBar_Devices = new MyToolBar( this );
    toolBar_Devices->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    hBox_Devices = new QWidget( toolBar_Devices );

    /*d_OpenWindow_A = toolBar_Devices->toggleViewAction();
    d_OpenWindow_A->setIconText("APPA");
    d_OpenWindow_A->setIcon(QPixmap( start_xpm ));
    d_OpenWindow_A->setText("APPA");
    d_OpenWindow_A->setShortcut(QKeySequence(tr("Ctrl+A")));
    d_OpenWindow_A->setStatusTip(tr("Open Window"));*/

    label_Devices_All = new QLabel(tr("LINE"));
    //label_Devices_All->setFont(QFont("Arial",16));
    label_Devices_All_X = new QLabel(tr("X"));
    label_Devices_All_Y = new QLabel(tr("Y"));
    label_Devices_All_Y2 = new QLabel(tr("Y2"));
    //label_Devices_All_Show = new QLabel(tr("Show"));
    Button_Devices_ClearAll = new QPushButton(tr("Clear"));
    Button_Devices_AutoScaleAll = new QPushButton(tr("AutoScale"));
    label_Devices_All->setFixedWidth(100);
    Button_Devices_ClearAll->setFixedWidth(40);
    Button_Devices_AutoScaleAll->setFixedWidth(70);
	
	/*
	these things will be realised INSH
    1) opening files (FILE)                                             /ready
    2) addition to files or data (DATA1+DATA2)                          /
    3) special function for calculation R and maybe MWA (R or MWA)
	4) deleting points with mouse selection
	5) to plot realtime data (MAIN)
	*/

    for(int i=0; i<20; i++)
    {
        lineEdit_Devices[i] = new QLineEdit();
        lineEdit_Devices[i]->setText( QString("Line %1: empty").arg(i));
        lineEdit_Devices[i]->setReadOnly(true);
        //lineEdit_Devices[i]->setFont(QFont("Arial",16));

        label_Devices[i] = new QLabel();
        label_Devices[i]->setText( QString("Line %1:").arg(i));
        label_Devices[i]->setFont(QFont("Arial",16));
        checkBox_Devices_X[i] = new QCheckBox(tr(""));
        checkBox_Devices_Y[i] = new QCheckBox(tr(""));
        checkBox_Devices_Y2[i] = new QCheckBox(tr(""));
        //checkBox_Devices_Show[i] = new QCheckBox(tr(""));

        //checkBox_Devices_X[i]->setEnabled(false);

        Button_Devices_Start[i] = new QPushButton(tr("Start"));
        Button_Devices_Clear[i] = new QPushButton(tr("Clear"));
        Button_Devices_AutoScale[i] = new QPushButton(tr("AutoScale"));
        Button_Devices_Clear[i]->setFixedWidth(40);
        Button_Devices_AutoScale[i]->setFixedWidth(70);

    }

    for(int i=10 /*int_GET*/; i<CurvCnt; i++)
    {
        lineEdit_Devices[i]->hide();
        Button_Devices_Clear[i]->hide();
        Button_Devices_AutoScale[i]->hide();
    }


    QGridLayout *mainLayout = new QGridLayout( hBox_Devices );

    mainLayout->addWidget(label_Devices_All, 0, 0);
  //  mainLayout->addWidget(label_Devices_All_X, 0, 1);
  //  mainLayout->addWidget(label_Devices_All_Y, 0, 2);
    //mainLayout->addWidget(label_Devices_All_Y2, 0, 3);
    //mainLayout->addWidget(label_Devices_All_Show, 0, 4);
  //  mainLayout->addWidget(Button_Devices_ClearAll, 0, 5);
  //  mainLayout->addWidget(Button_Devices_AutoScaleAll, 0, 6);


    for(int i=0; i<20; i++)
    {
        mainLayout->addWidget(lineEdit_Devices[i], i+1, 0);
        //mainLayout->addWidget(label_Devices[i], 0, 0);
   //     mainLayout->addWidget(checkBox_Devices_X[i], i+1, 1);
   //     mainLayout->addWidget(checkBox_Devices_Y[i], i+1, 2);
        //mainLayout->addWidget(checkBox_Devices_Y2[i], i+1, 3);
        //mainLayout->addWidget(checkBox_Devices_Show[i], i+1, 4);
        //mainLayout->addWidget(Button_Devices_Start[i], 0, 5);
        mainLayout->addWidget(Button_Devices_Clear[i], i+1, 5);
        mainLayout->addWidget(Button_Devices_AutoScale[i], i+1, 6);

    }
    //mainLayout->getContentsMargins(0,0,0,0);

    mainLayout->setContentsMargins(5,5,5,5);
    mainLayout->setVerticalSpacing(5);
    mainLayout->setHorizontalSpacing(5);

    toolBar_Devices->addWidget( hBox_Devices );
    return toolBar_Devices;
}

QToolBar *LAMPhEdit::toolBar_PlotSize()
{
    MyToolBar *toolBar_PlotSize = new MyToolBar( this );
    toolBar_PlotSize->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    hBox_PlotSize = new QWidget( toolBar_PlotSize );

    hBox_PlotSize->setFont(QFont("Arial",15));

    label_PlotSize  = new QLabel(tr("Plot Size"));

    label_Plot_x_min = new QLabel(tr("Plot Size: x_min:"));
    label_Plot_x_max = new QLabel(tr("Plot Size: x_max:"));
    label_Plot_y_min = new QLabel(tr("Plot Size: y_min:"));
    label_Plot_y_max = new QLabel(tr("Plot Size: y_max:"));

    lineEdit_Plot_x_min = new QLineEdit(tr(""));
    lineEdit_Plot_x_max = new QLineEdit(tr(""));
    lineEdit_Plot_y_min = new QLineEdit(tr(""));
    lineEdit_Plot_y_max = new QLineEdit(tr(""));

    Button_PlotSize_replot = new QPushButton(tr("Replot"));
    //Button_PlotSize_multiply = new QPushButton(tr("*"));
    //Button_PlotSize_divide = new QPushButton(tr("/"));

    //Button_Devices_AutoScaleAll->setFixedWidth(70);
    //label_Devices_All->setFont(QFont("Arial",16));

    QGridLayout *mainLayout = new QGridLayout( hBox_PlotSize );

    mainLayout->addWidget(label_PlotSize, 0, 0);
    mainLayout->addWidget(label_Plot_x_min, 1, 0);
    mainLayout->addWidget(label_Plot_x_max, 2, 0);
    mainLayout->addWidget(label_Plot_y_min, 3, 0);
    mainLayout->addWidget(label_Plot_y_max, 4, 0);
    mainLayout->addWidget(lineEdit_Plot_x_min, 1, 1);
    mainLayout->addWidget(lineEdit_Plot_x_max, 2, 1);
    mainLayout->addWidget(lineEdit_Plot_y_min, 3, 1);
    mainLayout->addWidget(lineEdit_Plot_y_max, 4, 1);

    mainLayout->addWidget(Button_PlotSize_replot, 5, 1);

    mainLayout->setContentsMargins(5,5,5,5);
    mainLayout->setVerticalSpacing(5);
    mainLayout->setHorizontalSpacing(5);

    toolBar_PlotSize->addWidget( hBox_PlotSize );
    return toolBar_PlotSize;
}

void LAMPhEdit::setCheckBox()
{
    int new_int=0;
    for (int i=0;i<20;i++)
    {
        if (checkBox_Devices_X[i]->isChecked()) {
            number_of_checkBox = i;
            new_int++;
        }
    }

    if (new_int>1)
    {
     for (int i=0;i<20;i++)
     {
         if (number_of_checkBox_tmp==i) {
             checkBox_Devices_X[i]->setChecked(false);
             checkBox_Devices_Y[i]->setChecked(true);
             //checkBox_Devices_Show[i]->setChecked(true);
             break;
         }
     }
     number_of_checkBox=number_of_checkBox_tmp;
    }
    number_of_checkBox_tmp=number_of_checkBox;

    for (int i=0;i<20;i++)
    {
        if (number_of_checkBox==i)
        {
            if (new_int!=0){
                checkBox_Devices_Y[i]->setChecked(false);
                //checkBox_Devices_Show[i]->setChecked(false);
            }else {
                checkBox_Devices_Y[i]->setChecked(true);
                //checkBox_Devices_Show[i]->setChecked(true);
                break;
            }
        }
    }
}

void LAMPhEdit::appendPoints( bool on )
{
    /*if ( on )
        d_edit->append( d_timerCount->value(),
                        d_randomCount->value() );
    else
        d_edit->stop();*/
}

void LAMPhEdit::showRunning( bool running )
{
    /*d_randomCount->setEnabled( !running );
    d_timerCount->setEnabled( !running );
    d_startAction->setChecked( running );
    d_startAction->setText( running ? "Stop" : "Start" );*/
}

void LAMPhEdit::showElapsed( int ms )
{
    QString text;
    text.setNum( ms );
    text += " ms";

    statusBar()->showMessage( text );
}

void LAMPhEdit::initWhatsThis()
{
    const char *text1 =
        "Zooming is enabled until the selected area gets "
        "too small for the significance on the axes.\n\n"
        "You can zoom in using the left mouse button.\n"
        "The middle mouse button is used to go back to the "
        "previous zoomed area.\n"
        "The right mouse button is used to unzoom completely.";

    const char *text2 =
        "Number of random points that will be generated.";

    const char *text3 =
        "Delay between the generation of two random points.";

    const char *text4 =
        "Start generation of random points.\n\n"
        "The intention of this example is to show how to implement "
        "growing curves. The points will be generated and displayed "
        "one after the other.\n"
        "To check the performance, a small delay and a large number "
        "of points are useful. To watch the curve growing, a delay "
        " > 300 ms and less points are better.\n"
        "To inspect the curve, stacked zooming is implemented using the "
        "mouse buttons on the plot.";

    const char *text5 = "Remove all points.";

    d_edit->setWhatsThis( text1 );
   // d_randomCount->setWhatsThis( text2 );
   // d_timerCount->setWhatsThis( text3 );
   // d_startAction->setWhatsThis( text4 );
   // d_clearAction->setWhatsThis( text5 );
}

void LAMPhEdit::exportDocument()
{
    QwtPlotRenderer renderer;
    renderer.exportTo( d_edit, "LAMPh.pdf" );
}

void LAMPhEdit::openFile()
{
    DialogOpenFile *addDialogOpenFile = new DialogOpenFile("testFile",3);

    connect(addDialogOpenFile,SIGNAL(send_VectorXY(QVector<double>, QVector<double>, int)),this,SLOT(get_VectorXY(QVector<double>,QVector<double>,int)));
    connect(addDialogOpenFile,SIGNAL(sendMinMaxofVectorXY(double,double,double,double,int)),this,SLOT(getMinMaxofVectorXY(double,double,double,double,int)));

    connect(addDialogOpenFile,SIGNAL(sendFileName(QString,int)),this,SLOT(getFileName(QString,int)));

    addDialogOpenFile->setWindowTitle(QString("OpenFileTEST"));
    addDialogOpenFile->exec();
}

void LAMPhEdit::functionFile()
{
    /*
    qVectorY[0][6]=100;
    qVectorY[0][7]=100;
    qVectorY[0][8]=100;
    for (int i=0; i<20 ; i++){
       qDebug() << "X: " << qVectorX[0].at(i) << " Y: " << qVectorY[0].at(i);
    }

    d_edit->appendPointVectorXY_Incremental(qVectorX[0],qVectorY[0],0);
    */
}

void LAMPhEdit::get_VectorXY(QVector<double> X , QVector<double> Y, int r){
    qVectorX[r].clear();
    qVectorY[r].clear();
    qVectorX[r]=X;
    qVectorY[r]=Y;
    d_edit->appendPointVectorXY_Incremental(qVectorX[r],qVectorY[r],r);
}
void LAMPhEdit::getMinMaxofVectorXY(double x__min, double x__max, double y__min, double y__max, int r){
    xMin[r]=x__min;
    xMax[r]=x__max;
    yMin[r]=y__min;
    yMax[r]=y__max;
    //qDebug() << "x__min " << x__min << " x__max "  << x__max << " y__min " << y__min << " y__max " << y__max;

    d_edit->autoscaleXY(x__min,x__max,y__min,y__max);
}

void LAMPhEdit::getFileName(QString fileNameShort, int i){
    lineEdit_Devices[i]->setText(fileNameShort);
    //lineEdit_Devices[i]->setText( QString("DATA %1: %2").arg(i).arg(fileNameShort));
}

void LAMPhEdit::clear_one(int i){
    lineEdit_Devices[i]->setText( QString("DATA %1: empty").arg(i));
}

void LAMPhEdit::autoscale_one(int r){
    d_edit->autoscaleXY(xMin[r],xMax[r],yMin[r],yMax[r]);
}

void LAMPhEdit::enableZoomMode( bool on )
{
    d_panner->setEnabled( on );

    d_zoomer[0]->setEnabled( on );
    //d_zoomer[0]->zoom( 0 );

    d_picker->setEnabled( !on );

    showInfo();
}

void LAMPhEdit::showInfo( QString text )
{
    if ( text == QString::null )
    {
        if ( d_picker->rubberBand() )
            text = "Cursor Pos: Press left mouse button in plot region";
        else
            text = "Zoom: Press mouse button and drag";
    }

/*#ifndef QT_NO_STATUSBAR
    statusBar()->showMessage( text );
#endif*/
}

void LAMPhEdit::moved( const QPoint &pos )
{
    QString info;
    info.sprintf( "Temp=%g, Ampl=%g, Ampl=%g",
        d_edit->invTransform( QwtPlot::xBottom, pos.x() ),
        d_edit->invTransform( QwtPlot::yLeft, pos.y() ),
        d_edit->invTransform( QwtPlot::yRight, pos.y() )
    );
    showInfo( info );
}

void LAMPhEdit::selected( const QPolygon & )
{
    showInfo();
}



void LAMPhEdit::getDataDll()
{
    

}







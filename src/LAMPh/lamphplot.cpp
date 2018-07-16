#include <qlabel.h>
#include <qlayout.h>
#include <qstatusbar.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qwhatsthis.h>
#include <qpixmap.h>
#include <qwt_counter.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_renderer.h>
#include "mainplot.h"
#include "lamphplot.h"
#include "start.xpm"
#include "clear.xpm"
#include "pixmaps.h"

#include <QLineEdit>
#include <QFileDialog>
#include <QPushButton>


class MyToolBar: public QToolBar
{
public:
    MyToolBar( LAMPhPlot *parent ):
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

class Zoomer: public QwtPlotZoomer //Zoomer
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

LAMPhPlot::LAMPhPlot(QString loginQString)
{

    addToolBar(Qt::TopToolBarArea, toolBar()); // main buttons (for switching between windows)
    addToolBar(Qt::LeftToolBarArea, toolBar_Devices()); // data fields

#ifndef QT_NO_STATUSBAR
    ( void )statusBar();
#endif


    login = new QString(); // to transfer the user's login and delineation of rights
    *login = loginQString;

    //newuserButton = new QPushButton(tr("New"));
    //newExpButton = new QPushButton(tr("New Exp"));
    //edituserButton = new QPushButton(tr("Edit Exp"));
    /*infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
                              "invoke a context menu</i>"));
    infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    infoLabel->setAlignment(Qt::AlignCenter);*/


    //menu_bar = new QMenuBar(/*this*/);
    //menu_barDevices = new QMenuBar(/*this*/);

    //to prepare in advance those windows
    lamphEdit = new LAMPhEdit("user23LAMPhEdit");
    lamphEdit ->close();

    lamphSetting = new LAMPhSetting("user23LAMPhSetting");
    lamphSetting ->close();

    dataTable = new DataTable("user23DataBase");
    dataTable ->close();

    lamphDevices = new LAMPhDevices("user23LAMPhDevices");
    lamphDevices ->show();
    lamphDevices ->close();



    lamphTemp = new LAMPhTemp("user23LAMPhTemp");
    lamphTemp ->close();

    //this is for switching between windows
    connect( d_OpenWindow_Devices, SIGNAL( triggered() ), lamphDevices, SLOT( show() ) );
    connect( d_OpenWindow_Temp, SIGNAL( triggered() ), lamphTemp, SLOT( show() ) );
    connect( d_OpenWindow_DataTable, SIGNAL( triggered() ), dataTable, SLOT( show() ) );
    connect( d_OpenWindow_Edit, SIGNAL( triggered() ), lamphEdit, SLOT(show()) );
    connect( d_OpenWindow_Setting, SIGNAL( triggered() ), lamphSetting, SLOT( show() ) );

    /*connect( d_OpenWindow_Devices, SIGNAL( triggered() ), lamphDevices, SLOT( raise() ) );
    //connect( d_OpenWindow_Temp, SIGNAL( triggered() ), lamphTemp, SLOT( raise() ) );
    connect( d_OpenWindow_DataTable, SIGNAL( triggered() ), dataTable, SLOT( raise() ) );
    connect( d_OpenWindow_Edit, SIGNAL( triggered() ), lamphEdit, SLOT(raise()) );
    connect( d_OpenWindow_Setting, SIGNAL( triggered() ), lamphSetting, SLOT( raise() ) );*/

    connect( d_OpenWindow_Exit, SIGNAL( triggered() ), lamphDevices, SLOT(close()) );
    connect( d_OpenWindow_Exit, SIGNAL( triggered() ), lamphTemp, SLOT(close()) );
    connect( d_OpenWindow_Exit, SIGNAL( triggered() ), dataTable, SLOT(close()) );
    connect( d_OpenWindow_Exit, SIGNAL( triggered() ), lamphEdit, SLOT(close()) );
    connect( d_OpenWindow_Exit, SIGNAL( triggered() ), lamphSetting, SLOT(close()) );
    connect( d_OpenWindow_Exit, SIGNAL( triggered() ), this, SLOT(close()) );

    connect( lamphEdit, SIGNAL(showLAMPhPlot()),this, SLOT(show())  );
    connect( lamphEdit, SIGNAL(showLAMPhDevices()),lamphDevices, SLOT(show())  );
    connect( lamphEdit, SIGNAL(showLAMPhTemp()),lamphTemp, SLOT(show())  );
    connect( lamphEdit, SIGNAL(showDataTable()),dataTable, SLOT(show())  );
    connect( lamphEdit, SIGNAL(showLAMPhEdit()),lamphEdit, SLOT(show())  );
    connect( lamphEdit, SIGNAL(showLAMPhSetting()),lamphSetting, SLOT(show())  );
    connect( lamphEdit, SIGNAL(LAMPhExit()),this, SLOT(close())  );

    connect( lamphSetting, SIGNAL(showLAMPhPlot()),this, SLOT(show())  );
    connect( lamphSetting, SIGNAL(showLAMPhDevices()),lamphDevices, SLOT(show())  );
    connect( lamphSetting, SIGNAL(showLAMPhTemp()),lamphTemp, SLOT(show())  );
    connect( lamphSetting, SIGNAL(showDataTable()),dataTable, SLOT(show())  );
    connect( lamphSetting, SIGNAL(showLAMPhEdit()),lamphEdit, SLOT(show())  );
    connect( lamphSetting, SIGNAL(showLAMPhSetting()),lamphSetting, SLOT(show())  );
    connect( lamphSetting, SIGNAL(LAMPhExit()),this, SLOT(close())  );

    connect( dataTable, SIGNAL(showLAMPhPlot()),this, SLOT(show())  );
    connect( dataTable, SIGNAL(showLAMPhDevices()),lamphDevices, SLOT(show())  );
    connect( dataTable, SIGNAL(showLAMPhTemp()),lamphTemp, SLOT(show())  );
    connect( dataTable, SIGNAL(showDataTable()),dataTable, SLOT(show())  );
    connect( dataTable, SIGNAL(showLAMPhEdit()),lamphEdit, SLOT(show())  );
    connect( dataTable, SIGNAL(showLAMPhSetting()),lamphSetting, SLOT(show())  );
    connect( dataTable, SIGNAL(LAMPhExit()),this, SLOT(close())  );

    connect( lamphDevices, SIGNAL(showLAMPhPlot()),this, SLOT(show())  );
    connect( lamphDevices, SIGNAL(showLAMPhDevices()),lamphDevices, SLOT(show())  );
    connect( lamphDevices, SIGNAL(showLAMPhTemp()),lamphTemp, SLOT(show())  );
    connect( lamphDevices, SIGNAL(showDataTable()),dataTable, SLOT(show())  );
    connect( lamphDevices, SIGNAL(showLAMPhEdit()),lamphEdit, SLOT(show())  );
    connect( lamphDevices, SIGNAL(showLAMPhSetting()),lamphSetting, SLOT(show())  );
    connect( lamphDevices, SIGNAL(LAMPhExit()),this, SLOT(close())  );

    connect( lamphTemp, SIGNAL(showLAMPhPlot()),this, SLOT(show())  );
    connect( lamphTemp, SIGNAL(showLAMPhDevices()),lamphDevices, SLOT(show())  );
    connect( lamphTemp, SIGNAL(showLAMPhTemp()),lamphTemp, SLOT(show())  );
    connect( lamphTemp, SIGNAL(showDataTable()),dataTable, SLOT(show())  );
    connect( lamphTemp, SIGNAL(showLAMPhEdit()),lamphEdit, SLOT(show())  );
    connect( lamphTemp, SIGNAL(showLAMPhSetting()),lamphSetting, SLOT(show())  );
    connect( lamphTemp, SIGNAL(LAMPhExit()),this, SLOT(close())  );

    d_plot = new MainPlot( this ); //for working with graphs
    const int margin = 4; //margin
    d_plot->setContentsMargins( margin, margin, margin, margin );

    //Zoomer
    d_zoomer[0] = new Zoomer( QwtPlot::xBottom, QwtPlot::yLeft,
        d_plot->canvas() );
    d_zoomer[0]->setRubberBand( QwtPicker::RectRubberBand );
    d_zoomer[0]->setRubberBandPen( QColor( Qt::green ) );
    d_zoomer[0]->setTrackerMode( QwtPicker::ActiveOnly );
    d_zoomer[0]->setTrackerPen( QColor( Qt::white ) );

    //allows to work with graphs with the mouse
    d_panner = new QwtPlotPanner( d_plot->canvas() );
    d_panner->setMouseButton( Qt::MidButton );

    d_picker = new QwtPlotPicker( QwtPlot::xBottom, QwtPlot::yLeft,
            QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn,
            d_plot->canvas() );
        d_picker->setStateMachine( new QwtPickerDragPointMachine() );
        d_picker->setRubberBandPen( QColor( Qt::green ) );
        d_picker->setRubberBand( QwtPicker::CrossRubberBand );
        d_picker->setTrackerPen( QColor( Qt::white ) );

    setCentralWidget( d_plot );

    initWhatsThis();

    setContextMenuPolicy( Qt::NoContextMenu );

    connect( d_picker, SIGNAL( moved( const QPoint & ) ),
        SLOT( moved( const QPoint & ) ) );
    connect( d_picker, SIGNAL( selected( const QPolygon & ) ),
        SLOT( selected( const QPolygon & ) ) );

    enableZoomMode( false );


    connect( d_startAction, SIGNAL( toggled( bool ) ), this, SLOT( appendPoints( bool ) ) );
    connect( d_clearAction, SIGNAL( triggered() ), d_plot, SLOT( clear() ) );
    connect( d_zoomAction, SIGNAL( toggled( bool ) ), SLOT( enableZoomMode( bool ) ) );
    connect( d_exportAction, SIGNAL( triggered() ), this, SLOT( exportDocument() ) );

    connect( d_symbolType, SIGNAL( toggled( bool ) ), d_plot, SLOT( showSymbols( bool ) ) );
    connect( d_plot, SIGNAL( running( bool ) ), this, SLOT( showRunning( bool ) ) );
    connect( d_plot, SIGNAL( elapsed( int ) ), this, SLOT( showElapsed( int ) ) );


    for (int i=0;i<20;i++)
    {
        connect(checkBox_Devices_X[i], SIGNAL(toggled(bool)),this,SLOT(setCheckBox()) );
        connect(checkBox_Devices_Y[i], SIGNAL(toggled(bool)),this,SLOT(setCheckBox()) );
        //connect(checkBox_Devices_Show[i], SIGNAL(toggled(bool)),this,SLOT(setCheckBox()) );
    }

    setWindowTitle(tr("LAMPh Plot - %1 ").arg(login->toLower()));
    //showFullScreen();
    resize(1000, 700);
    //showMaximized();
    //this->raise();
    //showMaximized();
    this->setWindowState(Qt::WindowMaximized);
    //show();

    //d_plot->get_x_result();

    connect( d_plot, SIGNAL( running_writeData( bool ) ), lamphDevices, SLOT( readData() ) );
    connect(lamphDevices,SIGNAL(send_x_result(float)),d_plot,SLOT(get_x_result(float)));


    connect(lamphDevices,SIGNAL(send_all_results(float,int)),d_plot,SLOT(get_all_results(float,int)));

    connect(lamphDevices,SIGNAL(setColorSize(int,int,int)),d_plot,SLOT(setColorSize(int,int,int)));

    connect(lamphDevices,SIGNAL(setNumberDevice_bool( bool ,int)),d_plot,SLOT(get_bool( bool ,int)));

}

QToolBar *LAMPhPlot::toolBar()
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

    QAction *whatsThisAction = QWhatsThis::createAction( toolBar );
    whatsThisAction->setText( "Help" );

    toolBar->addAction( d_startAction );
    toolBar->addAction( d_clearAction );
    toolBar->addAction( d_zoomAction );
    toolBar->addAction( d_exportAction );
    toolBar->addAction( whatsThisAction );
    toolBar->addSeparator();

    setIconSize( QSize( 22, 22 ) );

    QWidget *hBox = new QWidget( toolBar );

    d_symbolType = new QCheckBox( "Symbols", hBox );
    d_symbolType->setChecked( true );

    d_randomCount =
        new Counter( hBox, "Points", QString::null, 1, 100000, 100 );
    d_randomCount->setValue( 100000 );

    d_timerCount = new Counter( hBox, "Delay", "ms", 0, 100000, 100 );
    d_timerCount->setValue( 500 );

    QHBoxLayout *layout = new QHBoxLayout( hBox );
    layout->setMargin( 0 );
    layout->setSpacing( 0 );
    layout->addSpacing( 10 );
    layout->addWidget( new QWidget( hBox ), 10 ); // spacer
    layout->addWidget( d_symbolType );
    layout->addSpacing( 5 );
    layout->addWidget( d_randomCount );
    layout->addSpacing( 5 );
    layout->addWidget( d_timerCount );

    showRunning( false );

    toolBar->addWidget( hBox );

    d_OpenWindow_Main = new QAction( QPixmap( start_xpm ), "Main Plot", toolBar );
    d_OpenWindow_Main->setCheckable( true );
    d_OpenWindow_Main->setChecked(true);
    d_OpenWindow_Main->setEnabled(false);

    d_OpenWindow_Devices = new QAction( QPixmap( start_xpm ), "Devices", toolBar );
    d_OpenWindow_Temp = new QAction( QPixmap( start_xpm ), "Temp", toolBar );
    d_OpenWindow_DataTable = new QAction( QPixmap( start_xpm ), "DataTable", toolBar );
    d_OpenWindow_Edit = new QAction( QPixmap( start_xpm ), "Edit", toolBar );
    d_OpenWindow_Setting = new QAction( QPixmap( start_xpm ), "Setting", toolBar );
    d_OpenWindow_Exit = new QAction( QPixmap( start_xpm ), "Exit", toolBar );

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

QToolBar *LAMPhPlot::toolBar_Devices()
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

    label_Devices_All = new QLabel(tr("Devices"));
    label_Devices_All_X = new QLabel(tr("X"));
    label_Devices_All_Y = new QLabel(tr("Y"));
    label_Devices_All_Y2 = new QLabel(tr("Y2"));
    //label_Devices_All_Show = new QLabel(tr("Show"));
    Button_Devices_ClearAll = new QPushButton(tr("Clear"));
    Button_Devices_AutoScaleAll = new QPushButton(tr("AutoScale"));
    Button_Devices_ClearAll->setFixedWidth(40);
    Button_Devices_AutoScaleAll->setFixedWidth(70);

    for(int i=0; i<20; i++)
    {
        lineEdit_Devices[i] = new QLineEdit();
        lineEdit_Devices[i]->setText( QString("Device %1:").arg(i));
        lineEdit_Devices[i]->setReadOnly(true);

        label_Devices[i] = new QLabel();
        label_Devices[i]->setText( QString("Device %1:").arg(i));
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

    QGridLayout *mainLayout = new QGridLayout( hBox_Devices );

    mainLayout->addWidget(label_Devices_All, 0, 0);
    mainLayout->addWidget(label_Devices_All_X, 0, 1);
    mainLayout->addWidget(label_Devices_All_Y, 0, 2);
    //mainLayout->addWidget(label_Devices_All_Y2, 0, 3);
    //mainLayout->addWidget(label_Devices_All_Show, 0, 4);
    mainLayout->addWidget(Button_Devices_ClearAll, 0, 5);
    mainLayout->addWidget(Button_Devices_AutoScaleAll, 0, 6);


    for(int i=0; i<20; i++)
    {
        mainLayout->addWidget(lineEdit_Devices[i], i+1, 0);
        //mainLayout->addWidget(label_Devices[i], 0, 0);
        mainLayout->addWidget(checkBox_Devices_X[i], i+1, 1);
        mainLayout->addWidget(checkBox_Devices_Y[i], i+1, 2);
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

void LAMPhPlot::setCheckBox() //this automatically switches checkBoxes so that there is no controversy in the program
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

    // // // Simple and understandable explanation // // //

    /*int new_int=0;

    if (checkBox_A1_X->isChecked()) {number_of_checkBox = 1; new_int++;}
    if (checkBox_A2_X->isChecked()) {number_of_checkBox = 2; new_int++;}
    if (checkBox_A3_X->isChecked()) {number_of_checkBox = 3; new_int++;}
    if (checkBox_A4_X->isChecked()) {number_of_checkBox = 4; new_int++;}

    if (checkBox_K1_X->isChecked()) {number_of_checkBox = 11; new_int++;}
    if (checkBox_K2_X->isChecked()) {number_of_checkBox = 12; new_int++;}
    if (checkBox_K3_X->isChecked()) {number_of_checkBox = 13; new_int++;}
    if (checkBox_K4_X->isChecked()) {number_of_checkBox = 14; new_int++;}

    if (checkBox_L1_X->isChecked()) {number_of_checkBox = 21; new_int++;}
    if (checkBox_L2_X->isChecked()) {number_of_checkBox = 22; new_int++;}
    if (checkBox_L3_X->isChecked()) {number_of_checkBox = 23; new_int++;}
    if (checkBox_L4_X->isChecked()) {number_of_checkBox = 24; new_int++;}

    if (new_int>1)
    {
        switch (number_of_checkBox_tmp) {
        case 1:checkBox_A1_X->setChecked(false);checkBox_A1_Y->setChecked(true);break;
        case 2:checkBox_A2_X->setChecked(false);checkBox_A2_Y->setChecked(true);break;
        case 3:checkBox_A3_X->setChecked(false);checkBox_A3_Y->setChecked(true);break;
        case 4:checkBox_A4_X->setChecked(false);checkBox_A4_Y->setChecked(true);break;

        case 11:checkBox_K1_X->setChecked(false);checkBox_K1_Y->setChecked(true);break;
        case 12:checkBox_K2_X->setChecked(false);checkBox_K2_Y->setChecked(true);break;
        case 13:checkBox_K3_X->setChecked(false);checkBox_K3_Y->setChecked(true);break;
        case 14:checkBox_K4_X->setChecked(false);checkBox_K4_Y->setChecked(true);break;

        case 21:checkBox_L1_X->setChecked(false);checkBox_L1_Y->setChecked(true);break;
        case 22:checkBox_L2_X->setChecked(false);checkBox_L2_Y->setChecked(true);break;
        case 23:checkBox_L3_X->setChecked(false);checkBox_L3_Y->setChecked(true);break;
        case 24:checkBox_L4_X->setChecked(false);checkBox_L4_Y->setChecked(true);break;
        }
        number_of_checkBox=number_of_checkBox_tmp;
    }
        number_of_checkBox_tmp=number_of_checkBox;
    switch (number_of_checkBox) {
    case 1:if (new_int!=0)checkBox_A1_Y->setChecked(false);else checkBox_A1_Y->setChecked(true);break;
    case 2:if (new_int!=0)checkBox_A2_Y->setChecked(false);else checkBox_A2_Y->setChecked(true);break;
    case 3:if (new_int!=0)checkBox_A3_Y->setChecked(false);else checkBox_A3_Y->setChecked(true);break;
    case 4:if (new_int!=0)checkBox_A4_Y->setChecked(false);else checkBox_A4_Y->setChecked(true);break;
    case 11:if (new_int!=0)checkBox_K1_Y->setChecked(false);else checkBox_K1_Y->setChecked(true);break;
    case 12:if (new_int!=0)checkBox_K2_Y->setChecked(false);else checkBox_K2_Y->setChecked(true);break;
    case 13:if (new_int!=0)checkBox_K3_Y->setChecked(false);else checkBox_K3_Y->setChecked(true);break;
    case 14:if (new_int!=0)checkBox_K4_Y->setChecked(false);else checkBox_K4_Y->setChecked(true);break;
    case 21:if (new_int!=0)checkBox_L1_Y->setChecked(false);else checkBox_L1_Y->setChecked(true);break;
    case 22:if (new_int!=0)checkBox_L2_Y->setChecked(false);else checkBox_L2_Y->setChecked(true);break;
    case 23:if (new_int!=0)checkBox_L3_Y->setChecked(false);else checkBox_L3_Y->setChecked(true);break;
    case 24:if (new_int!=0)checkBox_L4_Y->setChecked(false);else checkBox_L4_Y->setChecked(true);break;
    }*/

}

void LAMPhPlot::appendPoints( bool on )
{
    if ( on )
        d_plot->append( d_timerCount->value(),
                        d_randomCount->value() );
    else
        d_plot->stop();
}

void LAMPhPlot::showRunning( bool running )
{
    d_randomCount->setEnabled( !running );
    d_timerCount->setEnabled( !running );
    d_startAction->setChecked( running );
    d_startAction->setText( running ? "Stop" : "Start" );
}

void LAMPhPlot::showElapsed( int ms )
{
    QString text;
    text.setNum( ms );
    text += " ms";

    statusBar()->showMessage( text );
}

void LAMPhPlot::initWhatsThis()
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

    d_plot->setWhatsThis( text1 );
    d_randomCount->setWhatsThis( text2 );
    d_timerCount->setWhatsThis( text3 );
    d_startAction->setWhatsThis( text4 );
    d_clearAction->setWhatsThis( text5 );
}

void LAMPhPlot::exportDocument()
{
    QwtPlotRenderer renderer;
    renderer.exportTo( d_plot, "LAMPh.pdf" );
}

void LAMPhPlot::enableZoomMode( bool on )
{
    d_panner->setEnabled( on );

    d_zoomer[0]->setEnabled( on );
    //d_zoomer[0]->zoom( 0 );

    d_picker->setEnabled( !on );

    showInfo();
}

void LAMPhPlot::showInfo( QString text )
{
    if ( text == QString::null )
    {
        if ( d_picker->rubberBand() )
            text = "Cursor Pos: Press left mouse button in plot region";
        else
            text = "Zoom: Press mouse button and drag";
    }
    statusBar()->showMessage( text );

/*#ifndef QT_NO_STATUSBAR
    statusBar()->showMessage( text );
#endif*/
}

void LAMPhPlot::moved( const QPoint &pos )
{
    QString info;
    info.sprintf( "X=%g, Y=%g, Y2=%g",
        d_plot->invTransform( QwtPlot::xBottom, pos.x() ),
        d_plot->invTransform( QwtPlot::yLeft, pos.y() ),
        d_plot->invTransform( QwtPlot::yRight, pos.y() )
    );
    showInfo( info );
}

void LAMPhPlot::selected( const QPolygon & )
{
    showInfo();
}



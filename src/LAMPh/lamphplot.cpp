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
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QMenuBar>


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

/*class Zoomer: public QwtPlotZoomer //Zoomer
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
};*/

LAMPhPlot::LAMPhPlot(QStringList argumentsQStringList)
{

    addToolBar(Qt::TopToolBarArea, toolBar()); // main buttons (for switching between windows)
    addToolBar(Qt::LeftToolBarArea, toolBar_Devices()); // data fields
    addToolBar(Qt::LeftToolBarArea, toolBar_PlotSize());
    addToolBar(Qt::RightToolBarArea, toolBar_Devices_Edit());


    createMenus();


#ifndef QT_NO_STATUSBAR
    ( void )statusBar();
#endif

    QString argPlot = "Plot";
    QString argDevices = "Devices";

    for (int i=0; i<argumentsQStringList.size(); i++){
        if (argumentsQStringList.at(i).contains("-plot", Qt::CaseInsensitive)){
            argPlot = argumentsQStringList.at(i+1);
        }
        if (argumentsQStringList.at(i).contains("-devices", Qt::CaseInsensitive)){
            argDevices = argumentsQStringList.at(i+1);
        }
    }

    /*QString argumentsQString = argumentsQStringList.join(",");
    lineEdit_Devices[0]->setText(argumentsQString);*/

    login = new QString(); // to transfer the user's login and delineation of rights
    *login = argPlot;

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
    lamphEdit = new LAMPhEdit("LAMPhEdit");
    lamphEdit ->close();

    lamphSetting = new LAMPhSetting("LAMPhSetting");
    lamphSetting ->close();

    dataTable = new DataTable("DataBase");
    dataTable ->close();

    lamphDevices = new LAMPhDevices(argDevices);
    lamphDevices ->show();
    lamphDevices ->close();



    lamphTemp = new LAMPhTemp("LAMPhTemp");
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
    /*d_zoomer[0] = new Zoomer( QwtPlot::xBottom, QwtPlot::yLeft,
        d_plot->canvas() );
    d_zoomer[0]->setRubberBand( QwtPicker::RectRubberBand );
    d_zoomer[0]->setRubberBandPen( QColor( Qt::green ) );
    d_zoomer[0]->setTrackerMode( QwtPicker::ActiveOnly );
    d_zoomer[0]->setTrackerPen( QColor( Qt::white ) );*/

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
    connect( d_deletePoints, SIGNAL( toggled( bool ) ), SLOT( enableDelMode( bool ) ) );
    connect( d_exportAction, SIGNAL( triggered() ), this, SLOT( exportDocument() ) );

    connect( d_openAction, SIGNAL( triggered() ), this, SLOT( openFile() ) );
    connect( d_functionAction, SIGNAL( triggered() ), this, SLOT( functionFile() ) );

    connect( d_symbolType, SIGNAL( toggled( bool ) ), d_plot, SLOT( showSymbols( bool ) ) );
    connect( d_plot, SIGNAL( running( bool ) ), this, SLOT( showRunning( bool ) ) );
    connect( d_plot, SIGNAL( elapsed( int ) ), this, SLOT( showElapsed( int ) ) );


    /*for (int i=0;i<20;i++)
    {
        connect(checkBox_Devices_X[i], SIGNAL(toggled(bool)),this,SLOT(setCheckBox()) );
        connect(checkBox_Devices_Y[i], SIGNAL(toggled(bool)),this,SLOT(setCheckBox()) );
        //connect(checkBox_Devices_Show[i], SIGNAL(toggled(bool)),this,SLOT(setCheckBox()) );
    }*/

    for (int i=0;i<20;i++){
        xMin[i]=0;
        xMax[i]=1;
        yMin[i]=0;
        yMax[i]=1;
    }

    firstPoint_del=false;
    mousePressEvent_int=0;
    mouseReleaseEvent_int=0;

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

    connect( d_startAction, SIGNAL( toggled( bool ) ), lamphDevices, SLOT( appendPoints( bool ) ) );

    connect(lamphDevices,SIGNAL(send_all_results(float,int)),d_plot,SLOT(get_all_results(float,int)));

    connect(lamphDevices,SIGNAL(send_all_results_to_lamphplot(float,int)),this,SLOT(show_all_results(float,int)));

    connect(lamphDevices,SIGNAL(setColorSize(int,int,int)),d_plot,SLOT(setColorSize(int,int,int)));

    connect(lamphDevices,SIGNAL(setNumberDevice_bool( bool ,int)),d_plot,SLOT(get_bool( bool ,int)));

    //d_plot->get_numberofdeviceInt(lamphDevices->get_numberofdeviceInt());
    connect(lamphDevices,SIGNAL(send_numberofdeviceInt(int)),d_plot,SLOT(get_numberofdeviceInt(int)));

    connect(lamphDevices,SIGNAL(send_toolBar_GET_show_data(int)),this,SLOT(get_toolBar_GET_show_data(int)));

    connect(lamphDevices,SIGNAL(send_toolBar_GET_hide_data(int)),this,SLOT(get_toolBar_GET_hide_data(int)));

    connect(Button_Devices_ClearAll, SIGNAL (released()), d_plot ,SLOT( clear()));

    connect(Button_Devices_AutoScaleAll, SIGNAL (released()), d_plot ,SLOT( autoscale()));

    for(int i=0; i<20; i++)
    {
        connect(Button_Devices_Clear[i], static_cast<void(QPushButton::*)(bool)>(&QPushButton::clicked),
                [=](bool bool_one){
                d_plot->clear_one(i);
           });
        connect(Button_Devices_AutoScale[i], static_cast<void(QPushButton::*)(bool)>(&QPushButton::clicked),
                [=](bool bool_one){
                d_plot->autoscale_one(i);
           });
    }

    for(int i=0; i<20; i++)
    {
        connect(edit_Button_Devices_Clear[i], static_cast<void(QPushButton::*)(bool)>(&QPushButton::clicked),
                [=](bool bool_one){
                clear_one(i);
           });
        connect(edit_Button_Devices_AutoScale[i], static_cast<void(QPushButton::*)(bool)>(&QPushButton::clicked),
                [=](bool bool_one){
                autoscale_one(i);
           });
        connect(edit_Button_Devices_Save[i], static_cast<void(QPushButton::*)(bool)>(&QPushButton::clicked),
                [=](bool bool_one){
                saveFile_one(i);
           });
    }

    //PlotSize
    connect(Button_PlotSize_replot, SIGNAL (released()), this ,SLOT( replot_PlotSize()));
    connect(d_plot, SIGNAL(send_data_PlotSize(float,float,float,float)),this,SLOT(get_data_PlotSize(float,float,float,float)));

    lamphDevices->first();

    //QMessageBox::critical(NULL,QObject::tr(""),tr(""));
    //exit(0);

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

    d_openAction = new QAction( QPixmap( start_xpm ), "OpenFile", toolBar );
    d_functionAction = new QAction( QPixmap( start_xpm ), "Function", toolBar );

    d_deletePoints = new QAction( QPixmap( zoom_xpm ), "Delete points", toolBar );
    d_deletePoints->setCheckable( true );

    QAction *whatsThisAction = QWhatsThis::createAction( toolBar );
    whatsThisAction->setText( "Help" );

    toolBar->addAction( d_startAction );
    toolBar->addAction( d_clearAction );
    toolBar->addAction( d_zoomAction );
    toolBar->addAction( d_exportAction );
    toolBar->addAction( whatsThisAction );
    toolBar->addSeparator();
    toolBar->addAction( d_openAction );
    toolBar->addAction( d_functionAction );
    toolBar->addAction( d_deletePoints );
    toolBar->addSeparator();

    setIconSize( QSize( 22, 22 ) );

    QWidget *hBox = new QWidget( toolBar );

    d_symbolType = new QCheckBox( "Symbols", hBox );
    d_symbolType->setChecked( true );

    //d_randomCount = new Counter( hBox, "Points", QString::null, 1, 100000, 100 );
    //d_randomCount->setValue( 100000 );

    d_timerCount = new Counter( hBox, "Delay", "ms", 0, 100000, 100 );
    d_timerCount->setValue( 500 );

    QHBoxLayout *layout = new QHBoxLayout( hBox );
    layout->setMargin( 0 );
    layout->setSpacing( 0 );
    layout->addSpacing( 10 );
    layout->addWidget( new QWidget( hBox ), 10 ); // spacer
    layout->addWidget( d_symbolType );
    layout->addSpacing( 5 );
    //layout->addWidget( d_randomCount );
    //layout->addSpacing( 5 );
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

void LAMPhPlot::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Windows"));
    fileMenu->addAction(d_OpenWindow_toolBar_Devices);
    fileMenu->addAction(d_OpenWindow_toolBar_PlotSize);
    fileMenu->addAction(d_OpenWindow_toolBar_Devices_Edit);
}

QToolBar *LAMPhPlot::toolBar_Devices() //toolBar_Data, it's not devices!!!
{
    MyToolBar *toolBar_Devices = new MyToolBar( this );
    toolBar_Devices->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    hBox_Devices = new QWidget( toolBar_Devices );

    d_OpenWindow_toolBar_Devices = toolBar_Devices->toggleViewAction();
    d_OpenWindow_toolBar_Devices->setIconText("Devices");
    d_OpenWindow_toolBar_Devices->setIcon(QPixmap( start_xpm ));
    d_OpenWindow_toolBar_Devices->setText("Devices");
    d_OpenWindow_toolBar_Devices->setShortcut(QKeySequence(tr("Ctrl+D")));
    d_OpenWindow_toolBar_Devices->setStatusTip(tr("Open Window"));

    label_Devices_All = new QLabel(tr("DATA"));
    label_Devices_All_X = new QLabel(tr("X"));
    label_Devices_All_Y = new QLabel(tr("Y"));
    label_Devices_All_Y2 = new QLabel(tr("Y2"));
    //label_Devices_All_Show = new QLabel(tr("Show"));
    Button_Devices_ClearAll = new QPushButton(tr("CLEAR"));
    Button_Devices_AutoScaleAll = new QPushButton(tr("AutoScale"));
    Button_Devices_ClearAll->setFixedWidth(40);
    Button_Devices_AutoScaleAll->setFixedWidth(70);

    label_Devices_All->setFont(QFont("Arial",16));

    for(int i=0; i<20; i++)
    {
        lineEdit_Devices[i] = new QLineEdit();
        lineEdit_Devices[i]->setText( QString("DATA %1:").arg(i));
        lineEdit_Devices[i]->setReadOnly(true);
        lineEdit_Devices[i]->setFont(QFont("Arial",16));

        label_Devices[i] = new QLabel();
        label_Devices[i]->setText( QString("DATA %1:").arg(i));
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

    for(int i=7 /*int_GET*/; i<CurvCnt; i++)
    {
        lineEdit_Devices[i]->hide();
        Button_Devices_Clear[i]->hide();
        Button_Devices_AutoScale[i]->hide();
    }

    QGridLayout *mainLayout = new QGridLayout( hBox_Devices );

    mainLayout->addWidget(label_Devices_All, 0, 0);
    //mainLayout->addWidget(label_Devices_All_X, 0, 1);
    //mainLayout->addWidget(label_Devices_All_Y, 0, 2);
    //mainLayout->addWidget(label_Devices_All_Y2, 0, 3);
    //mainLayout->addWidget(label_Devices_All_Show, 0, 4);
    mainLayout->addWidget(Button_Devices_ClearAll, 0, 5);
    mainLayout->addWidget(Button_Devices_AutoScaleAll, 0, 6);


    for(int i=0; i<20; i++)
    {
        mainLayout->addWidget(lineEdit_Devices[i], i+1, 0);
        //mainLayout->addWidget(label_Devices[i], 0, 0);
        //mainLayout->addWidget(checkBox_Devices_X[i], i+1, 1);
        //mainLayout->addWidget(checkBox_Devices_Y[i], i+1, 2);
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
    //toolBar_Devices->hide();
    return toolBar_Devices;
}

QToolBar *LAMPhPlot::toolBar_PlotSize()
{
    MyToolBar *toolBar_PlotSize = new MyToolBar( this );
    toolBar_PlotSize->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    hBox_PlotSize = new QWidget( toolBar_PlotSize );

    hBox_PlotSize->setFont(QFont("Arial",15));

    d_OpenWindow_toolBar_PlotSize = toolBar_PlotSize->toggleViewAction();
    d_OpenWindow_toolBar_PlotSize->setIconText("PlotSize");
    d_OpenWindow_toolBar_PlotSize->setIcon(QPixmap( start_xpm ));
    d_OpenWindow_toolBar_PlotSize->setText("PlotSize");
    d_OpenWindow_toolBar_PlotSize->setShortcut(QKeySequence(tr("Ctrl+G")));
    d_OpenWindow_toolBar_PlotSize->setStatusTip(tr("Open Window"));

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

QToolBar *LAMPhPlot::toolBar_Devices_Edit()
{
    MyToolBar *toolBar_Devices_Edit = new MyToolBar( this );
    toolBar_Devices_Edit->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    hBox_Devices_Edit = new QWidget( toolBar_Devices_Edit );

    d_OpenWindow_toolBar_Devices_Edit = toolBar_Devices_Edit->toggleViewAction();
    d_OpenWindow_toolBar_Devices_Edit->setIconText("Line");
    d_OpenWindow_toolBar_Devices_Edit->setIcon(QPixmap( start_xpm ));
    d_OpenWindow_toolBar_Devices_Edit->setText("Line");
    d_OpenWindow_toolBar_Devices_Edit->setShortcut(QKeySequence(tr("Ctrl+F")));
    d_OpenWindow_toolBar_Devices_Edit->setStatusTip(tr("Open Window"));

    edit_label_Devices_All = new QLabel(tr("LINE"));
    edit_Button_Devices_ClearAll = new QPushButton(tr("Clear"));
    edit_Button_Devices_AutoScaleAll = new QPushButton(tr("AutoScale"));
    edit_label_Devices_All->setFixedWidth(200);
    edit_Button_Devices_ClearAll->setFixedWidth(40);
    edit_Button_Devices_AutoScaleAll->setFixedWidth(70);

    for(int i=0; i<20; i++)
    {
        edit_lineEdit_Devices[i] = new QLineEdit();
        edit_lineEdit_Devices[i]->setText( QString("Line %1: empty").arg(i));
        edit_lineEdit_Devices[i]->setReadOnly(true);
        edit_Button_Devices_Clear[i] = new QPushButton(tr("Clear"));
        edit_Button_Devices_AutoScale[i] = new QPushButton(tr("AutoScale"));
        edit_Button_Devices_Save[i]= new QPushButton(tr("Save"));
        edit_Button_Devices_Clear[i]->setFixedWidth(40);
        edit_Button_Devices_AutoScale[i]->setFixedWidth(70);
        edit_Button_Devices_Save[i]->setFixedWidth(40);

    }

    for(int i=10 /*int_GET*/; i<CurvCnt; i++)
    {
        edit_lineEdit_Devices[i]->hide();
        edit_Button_Devices_Clear[i]->hide();
        edit_Button_Devices_AutoScale[i]->hide();
        edit_Button_Devices_Save[i]->hide();
    }


    QGridLayout *mainLayout = new QGridLayout( hBox_Devices_Edit );

    mainLayout->addWidget(edit_label_Devices_All, 0, 0);

    for(int i=0; i<20; i++)
    {
        mainLayout->addWidget(edit_lineEdit_Devices[i], i+1, 0);
        mainLayout->addWidget(edit_Button_Devices_Clear[i], i+1, 1);
        mainLayout->addWidget(edit_Button_Devices_AutoScale[i], i+1, 2);
        mainLayout->addWidget(edit_Button_Devices_Save[i], i+1, 3);
    }

    mainLayout->setContentsMargins(5,5,5,5);
    mainLayout->setVerticalSpacing(5);
    mainLayout->setHorizontalSpacing(5);

    toolBar_Devices_Edit->addWidget( hBox_Devices_Edit );
    toolBar_Devices_Edit->hide();
    return toolBar_Devices_Edit;
}

void LAMPhPlot::replot_PlotSize()
{
d_plot->replot_PlotSize(
                lineEdit_Plot_x_min->text().toFloat(),
                lineEdit_Plot_x_max->text().toFloat(),
                lineEdit_Plot_y_min->text().toFloat(),
                lineEdit_Plot_y_max->text().toFloat()
                );
}

void LAMPhPlot::get_data_PlotSize(float x_min, float x_max, float y_min, float y_max){
    lineEdit_Plot_x_min->setText(QString("%1").arg(x_min));
    lineEdit_Plot_x_max->setText(QString("%1").arg(x_max));
    lineEdit_Plot_y_min->setText(QString("%1").arg(y_min));
    lineEdit_Plot_y_max->setText(QString("%1").arg(y_max));
}

void LAMPhPlot::get_toolBar_GET_show_data(int int_get){
    lineEdit_Devices[int_get]->show();
    Button_Devices_Clear[int_get]->show();
    Button_Devices_AutoScale[int_get]->show();
}

void LAMPhPlot::get_toolBar_GET_hide_data(int int_get){
    lineEdit_Devices[int_get]->hide();
    Button_Devices_Clear[int_get]->hide();
    Button_Devices_AutoScale[int_get]->hide();
}

void LAMPhPlot::show_all_results(float result, int new_int)
{
    if (new_int<CurvCnt){
    lineEdit_Devices[new_int]->setText( QString("DATA %1: %2").arg(new_int).arg(result));
    }
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
        d_plot->append( d_timerCount->value(),1000
                        //,d_randomCount->value()
                        );
    else
        d_plot->stop();
}

void LAMPhPlot::showRunning( bool running )
{
   // d_randomCount->setEnabled( !running );
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
    //d_randomCount->setWhatsThis( text2 );
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
    //d_picker->setEnabled( on );
    d_deletePoints->setChecked(false);
    //d_zoomAction->setChecked(true);
    showInfo();
}

void LAMPhPlot::enableDelMode( bool on )
{
    //d_panner->setEnabled( !on );
    //d_picker->setEnabled( on );
    d_zoomAction->setChecked(false);
    //d_deletePoints->setChecked(true);
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
    info.sprintf( "X=%g, Y=%g",   //X=%g, Y=%g, Y2=%g
        d_plot->invTransform( QwtPlot::xBottom, pos.x() ),
        d_plot->invTransform( QwtPlot::yLeft, pos.y() ) /*,
        d_plot->invTransform( QwtPlot::yRight, pos.y() )*/
    );

    //qDebug() << "tesst:" << d_plot->invTransform( QwtPlot::xBottom, pos.x() );

    if (d_deletePoints->isChecked() and mousePressEvent_int==1){
        if (!firstPoint_del){
            x_del[0]=d_plot->invTransform(QwtPlot::xBottom, pos.x());
            y_del[0]=d_plot->invTransform(QwtPlot::yLeft, pos.y());
            x_del[4]=x_del[0];
            y_del[4]=y_del[0];

            firstPoint_del=true;
        }else{
            x_del[2]=d_plot->invTransform(QwtPlot::xBottom, pos.x());
            y_del[2]=d_plot->invTransform(QwtPlot::yLeft, pos.y());
            x_del[1]=x_del[2];
            y_del[1]=y_del[0];
            x_del[3]=x_del[0];
            y_del[3]=y_del[2];
            d_plot->appendPoint_del(x_del,y_del,5);
        }
    }

    if (d_zoomAction->isChecked() and mousePressEvent_int==1){
        if (!firstPoint_del){
            x_del[0]=d_plot->invTransform(QwtPlot::xBottom, pos.x());
            y_del[0]=d_plot->invTransform(QwtPlot::yLeft, pos.y());
            x_del[4]=x_del[0];
            y_del[4]=y_del[0];

            firstPoint_del=true;
        }else{
            x_del[2]=d_plot->invTransform(QwtPlot::xBottom, pos.x());
            y_del[2]=d_plot->invTransform(QwtPlot::yLeft, pos.y());
            x_del[1]=x_del[2];
            y_del[1]=y_del[0];
            x_del[3]=x_del[0];
            y_del[3]=y_del[2];
            d_plot->appendPoint_del(x_del,y_del,5);
        }
    }

    showInfo( info );
}

void LAMPhPlot::mousePressEvent( QMouseEvent * event ){
    //qDebug() << "mousePressEvent" << event->button();
    mousePressEvent_int=event->button();
}

void LAMPhPlot::mouseReleaseEvent( QMouseEvent * event ){
    //qDebug() << "mouseReleaseEvent" << event->button();
    firstPoint_del=false;
    mouseReleaseEvent_int=event->button();

    if (d_deletePoints->isChecked() and mouseReleaseEvent_int==1){

        double x_min_del=0;
        double x_max_del=0;
        double y_min_del=0;
        double y_max_del=0;

        bool del_first=false;

        if (x_del[0]<x_del[2]){x_min_del=x_del[0];x_max_del=x_del[2];}
        else {x_min_del=x_del[2];x_max_del=x_del[0];}
        if (y_del[0]<y_del[2]){y_min_del=y_del[0];y_max_del=y_del[2];}
        else {y_min_del=y_del[2];y_max_del=y_del[0];}



        for (int r=0;r<10;r++){

            stack_qVectorX[r].push_back(qVectorX[r]);
            stack_qVectorY[r].push_back(qVectorY[r]);

            int size_del = qVectorX[r].size(); //qVectorY[r] have the same size
            for (int iterator=0;iterator<size_del;iterator++) {
                if (((x_min_del<qVectorX[r].at(iterator))and(qVectorX[r].at(iterator)<x_max_del))
                      and
                    ((y_min_del<qVectorY[r].at(iterator))and(qVectorY[r].at(iterator)<y_max_del)))
                {
                    if (iterator==0){  //some problems with first element. QVector is slow to remove elements, so I decided to replace elements, but you cannot replace first element to -1 position.
                        del_first=true;
                    }else{
                        qVectorX[r].replace(iterator,qVectorX[r].at(iterator-1));
                        qVectorY[r].replace(iterator,qVectorY[r].at(iterator-1));
                    }
                }
            }

            if (del_first){
                del_first=false;
                int y=1;
                while (y<size_del)
                if ((qVectorX[r].at(0)==qVectorX[r].at(y)) and (qVectorY[r].at(0)==qVectorY[r].at(y))) //can have problems with last element, in case all elements will be equal
                {
                    y++;
                }
                else{
                    int t=0;
                    while(t<y){
                        qVectorX[r].replace(t,qVectorX[r].at(y));
                        qVectorY[r].replace(t,qVectorY[r].at(y));
                        t++;
                    }
                    break;
                }
            }


            d_plot->appendPointVectorXY_Incremental(qVectorX[r],qVectorY[r],r);
        }

        for (int i=0;i<5;i++){
            x_del[i]=0;
            y_del[i]=0;
        }
        d_plot->appendPoint_del(x_del,y_del,5);

    }

    if ((d_zoomAction->isChecked() and mouseReleaseEvent_int==1) and ((x_del[0]!=x_del[2]) and (y_del[0]!=y_del[2]))){

        double x_min_zoom=0;
        double x_max_zoom=0;
        double y_min_zoom=0;
        double y_max_zoom=0;

        if (x_del[0]<x_del[2]){x_min_zoom=x_del[0];x_max_zoom=x_del[2];}
        else {x_min_zoom=x_del[2];x_max_zoom=x_del[0];}
        if (y_del[0]<y_del[2]){y_min_zoom=y_del[0];y_max_zoom=y_del[2];}
        else {y_min_zoom=y_del[2];y_max_zoom=y_del[0];}

        vector_to_stack_zoom.clear();
        vector_to_stack_zoom.append(x_min_zoom);
        vector_to_stack_zoom.append(x_max_zoom);
        vector_to_stack_zoom.append(y_min_zoom);
        vector_to_stack_zoom.append(y_max_zoom);
        stack_zoom.push_back(vector_to_stack_zoom);

        d_plot->autoscaleXY(x_min_zoom,x_max_zoom,y_min_zoom,y_max_zoom);

        for (int i=0;i<5;i++){
            x_del[i]=0;
            y_del[i]=0;
        }
        d_plot->appendPoint_del(x_del,y_del,5);
    }

    if (d_zoomAction->isChecked() and mouseReleaseEvent_int==2){
        if (stack_zoom.size()>0){
            vector_to_stack_zoom.clear();
            vector_to_stack_zoom = stack_zoom.last();
            //qDebug() << "pop" << vector_to_stack_zoom;
            stack_zoom.removeLast();
            d_plot->autoscaleXY(vector_to_stack_zoom.at(0),vector_to_stack_zoom.at(1),vector_to_stack_zoom.at(2),vector_to_stack_zoom.at(3));
        }else{
            //d_plot->autoscaleXY(0,1,0,1);
        }
    }
}

void LAMPhPlot::keyPressEvent(QKeyEvent *e){
    if (e->key() == Qt::Key_C && e->modifiers() & Qt::ControlModifier)
    {
        //qDebug() << "Ctrl+C pressed!";
    }
    if (e->key() == Qt::Key_V && e->modifiers() & Qt::ControlModifier)
    {
        //qDebug() << "Ctrl+V pressed!";
    }
    if (e->key() == Qt::Key_Z && e->modifiers() & Qt::ControlModifier)
    {
        for (int r=0; r<10 ; r++){
            if (stack_qVectorX[r].size()>0)
            {
                qVectorX[r].clear();
                qVectorY[r].clear();
                qVectorX[r] = stack_qVectorX[r].last();
                qVectorY[r] = stack_qVectorY[r].last();
                stack_qVectorX[r].removeLast();
                stack_qVectorY[r].removeLast();
                d_plot->appendPointVectorXY_Incremental(qVectorX[r],qVectorY[r],r);
            }
        }
    }
    if (e->key() == Qt::Key_A && e->modifiers() & Qt::ControlModifier)
    {
        d_deletePoints->setChecked(false);
        d_zoomAction->setChecked(true);
    }
    if (e->key() == Qt::Key_S && e->modifiers() & Qt::ControlModifier)
    {
        d_zoomAction->setChecked(false);
        d_deletePoints->setChecked(true);
    }
}

void LAMPhPlot::selected( const QPolygon & )
{
    showInfo();
}

void LAMPhPlot::openFile(){
    DialogOpenFile *addDialogOpenFile = new DialogOpenFile("testFile",3);

    connect(addDialogOpenFile,SIGNAL(send_VectorXY(QVector<double>, QVector<double>, int)),this,SLOT(get_VectorXY(QVector<double>,QVector<double>,int)));
    connect(addDialogOpenFile,SIGNAL(sendMinMaxofVectorXY(double,double,double,double,int)),this,SLOT(getMinMaxofVectorXY(double,double,double,double,int)));

    connect(addDialogOpenFile,SIGNAL(sendFileName(QString,int)),this,SLOT(getFileName(QString,int)));

    addDialogOpenFile->setWindowTitle(QString("OpenFileTEST"));
    addDialogOpenFile->exec();
}

void LAMPhPlot::functionFile(){

}

void LAMPhPlot::get_VectorXY(QVector<double> X , QVector<double> Y, int r){
    qVectorX[r].clear();
    qVectorY[r].clear();
    qVectorX[r]=X;
    qVectorY[r]=Y;
    d_plot->appendPointVectorXY_Incremental(qVectorX[r],qVectorY[r],r);
}

void LAMPhPlot::getMinMaxofVectorXY(double x__min, double x__max, double y__min, double y__max, int r){
    xMin[r]=x__min;
    xMax[r]=x__max;
    yMin[r]=y__min;
    yMax[r]=y__max;
    //qDebug() << "x__min " << x__min << " x__max "  << x__max << " y__min " << y__min << " y__max " << y__max;

    d_plot->autoscaleXY(x__min,x__max,y__min,y__max);
    vector_to_stack_zoom.clear();
    vector_to_stack_zoom.append(x__min);
    vector_to_stack_zoom.append(x__max);
    vector_to_stack_zoom.append(y__min);
    vector_to_stack_zoom.append(y__max);
    stack_zoom.push_back(vector_to_stack_zoom);

}

void LAMPhPlot::getFileName(QString fileNameShort, int i){
    edit_lineEdit_Devices[i]->setText(fileNameShort);
    //lineEdit_Devices[i]->setText( QString("Line %1: %2").arg(i).arg(fileNameShort));
}

void LAMPhPlot::clear_one(int r){
    edit_lineEdit_Devices[r]->setText( QString("Line %1: empty").arg(r));
    qVectorX[r].clear();
    qVectorY[r].clear();
    xMin[r]=0;
    xMax[r]=1;
    yMin[r]=0;
    yMax[r]=1;
    d_plot->appendPointVectorXY_Incremental(qVectorX[r],qVectorY[r],r);
}

void LAMPhPlot::autoscale_one(int r){
    d_plot->autoscaleXY(xMin[r],xMax[r],yMin[r],yMax[r]);
    vector_to_stack_zoom.clear();
    vector_to_stack_zoom.append(xMin[r]);
    vector_to_stack_zoom.append(xMax[r]);
    vector_to_stack_zoom.append(yMin[r]);
    vector_to_stack_zoom.append(yMax[r]);
    stack_zoom.push_back(vector_to_stack_zoom);
}

void LAMPhPlot::saveFile_one(int r){
    DialogSaveFile *addDialogSaveFile = new DialogSaveFile(edit_lineEdit_Devices[r]->text(),qVectorX[r],qVectorY[r]);

    addDialogSaveFile->setWindowTitle(QString("SaveFileTEST"));
    addDialogSaveFile->exec();
}

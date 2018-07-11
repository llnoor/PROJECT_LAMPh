#include "experiments.h"
#include <QtWidgets>
#include <QMainWindow>


Experiments::Experiments(QString loginQString, QWidget *parent):
    QWidget(parent)
    //QWidget(0, Qt::Window | Qt::FramelessWindowHint)

{
    login = new QString();
    *login = loginQString;
    //login = "user";
    tableView = new QTableView();
    tableViewDevices = new QTableView();

    //emit



    /* Первым делом необходимо создать объект, который будет использоваться для работы с данными нашей БД
     * и инициализировать подключение к базе данных
     * */
    db = new DataBase();
    db->connectToDataBase();


    this->setupModel(EXPERIMENTS,
                        QStringList()
                        << trUtf8("ID")
                        << trUtf8("Experiment")
                        << trUtf8("Description")
                        << trUtf8("Author")
                        << trUtf8("Date")
                        << trUtf8("Devices")
                        << trUtf8("Parent")
                        << trUtf8("Link")
                        << trUtf8("Conf")
               );

    this->setupModelDevices(DEVICES,
                        QStringList()
                        << trUtf8("ID")
                        << trUtf8("Device")
                        << trUtf8("Type")
                        << trUtf8("Date")
                        << trUtf8("Description")
                        << trUtf8("Link")
                        << trUtf8("Conf")
               );

    /* Инициализируем внешний вид таблицы с данными
     * */
    this->createUI();

    //newuserButton = new QPushButton(tr("New"));
    newExpButton = new QPushButton(tr("New Exp"));
    //edituserButton = new QPushButton(tr("Edit Exp"));
    infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
                              "invoke a context menu</i>"));
    infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    infoLabel->setAlignment(Qt::AlignCenter);


    menu_bar = new QMenuBar(/*this*/);
    menu_barDevices = new QMenuBar(/*this*/);

    QGridLayout *grid = new QGridLayout;
    //grid->addWidget(groupTable());
    grid->addWidget(menu_bar, 0, 0 );
    grid->addWidget(menu_barDevices, 0, 1 );
    grid->addWidget(tableView, 1, 0/*, model->rowCount(), model->columnCount()*/);
    grid->addWidget(tableViewDevices, 1, 1/*, modelDevices->rowCount(), modelDevices->columnCount()*/);
    grid->addWidget(newExpButton, 2, 0);
    grid->addWidget(infoLabel, 3, 0);
    //grid->setColumnMinimumWidth(0,200);
    //grid->setColumnMinimumWidth(1,200);


    //grid->addWidget(edituserButton);
    //grid->addWidget(groupTable(),  model->rowCount()+1, 0);
    setLayout(grid);

    //tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::);
    tableView->setColumnWidth(1,100);
    tableView->setColumnWidth(2,120);
    tableView->setColumnWidth(3,100);
    tableView->setColumnWidth(4,80);

    tableViewDevices->setColumnWidth(1,100);
    tableViewDevices->setColumnWidth(2,80);
    tableViewDevices->setColumnWidth(4,100);
    tableViewDevices->setColumnWidth(6,100);



    createActions();
    createMenus();

/*#ifndef QT_NO_STATUSBAR
    ( void )statusBar();
#endif*/

    //QString message = tr("A context menu is available by right-clicking");
        //statusBar()->showMessage(message);


    setWindowTitle(tr("Experiments - %1 ").arg(login->toLower()));
    resize(900, 420);
    //setMinimumSize(160, 160);
    //showFullScreen();

        //this->setFont(QFont("Ubuntu"));//Roboto
        //this->setFont(QFont("Roboto",16,0,1));



    connect(newExpButton, SIGNAL(released()), this, SLOT(slotNewExp()));
    //connect(edituserButton, SIGNAL(released()), this, SLOT(slotEditExp()));



}


void Experiments::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(cutAct);
    menu.addAction(copyAct);
    menu.addAction(pasteAct);
    menu.exec(event->globalPos());
}
//! [3]

void Experiments::newFile()
{
    infoLabel->setText(tr("Invoked <b>File|New</b>"));
}

void Experiments::open()
{
    infoLabel->setText(tr("Invoked <b>File|Open</b>"));
}

void Experiments::save()
{
    infoLabel->setText(tr("Invoked <b>File|Save</b>"));
}

void Experiments::print()
{
    infoLabel->setText(tr("Invoked <b>File|Print</b>"));
}

void Experiments::undo()
{
    infoLabel->setText(tr("Invoked <b>Edit|Undo</b>"));
}

void Experiments::redo()
{
    infoLabel->setText(tr("Invoked <b>Edit|Redo</b>"));
}

void Experiments::cut()
{
    infoLabel->setText(tr("Invoked <b>Edit|Cut</b>"));
}

void Experiments::copy()
{
    infoLabel->setText(tr("Invoked <b>Edit|Copy</b>"));
}

void Experiments::paste()
{
    infoLabel->setText(tr("Invoked <b>Edit|Paste</b>"));
}

void Experiments::bold()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Bold</b>"));
}

void Experiments::italic()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Italic</b>"));
}

void Experiments::leftAlign()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Left Align</b>"));
}

void Experiments::rightAlign()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Right Align</b>"));
}

void Experiments::justify()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Justify</b>"));
}

void Experiments::center()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Center</b>"));
}

void Experiments::setLineSpacing()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Set Line Spacing</b>"));
}

void Experiments::setParagraphSpacing()
{
    infoLabel->setText(tr("Invoked <b>Edit|Format|Set Paragraph Spacing</b>"));
}

void Experiments::about()
{
    infoLabel->setText(tr("Invoked <b>Help|About</b>"));
    QMessageBox::about(this, tr("About Menu"),
            tr("The <b>Menu</b> example shows how to create "
               "menu-bar menus and context menus."));
}

void Experiments::aboutQt()
{
    infoLabel->setText(tr("Invoked <b>Help|About Qt</b>"));
}


void Experiments::createActions()
{
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    printAct = new QAction(tr("&Print..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Print the document"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last operation"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last operation"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

    cutAct = new QAction(tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    boldAct = new QAction(tr("&Bold"), this);
    boldAct->setCheckable(true);
    boldAct->setShortcut(QKeySequence::Bold);
    boldAct->setStatusTip(tr("Make the text bold"));
    connect(boldAct, SIGNAL(triggered()), this, SLOT(bold()));

    QFont boldFont = boldAct->font();
    boldFont.setBold(true);
    boldAct->setFont(boldFont);

    italicAct = new QAction(tr("&Italic"), this);
    italicAct->setCheckable(true);
    italicAct->setShortcut(QKeySequence::Italic);
    italicAct->setStatusTip(tr("Make the text italic"));
    connect(italicAct, SIGNAL(triggered()), this, SLOT(italic()));

    QFont italicFont = italicAct->font();
    italicFont.setItalic(true);
    italicAct->setFont(italicFont);

    setLineSpacingAct = new QAction(tr("Set &Line Spacing..."), this);
    setLineSpacingAct->setStatusTip(tr("Change the gap between the lines of a "
                                       "paragraph"));
    connect(setLineSpacingAct, SIGNAL(triggered()), this, SLOT(setLineSpacing()));

    setParagraphSpacingAct = new QAction(tr("Set &Paragraph Spacing..."), this);
    setParagraphSpacingAct->setStatusTip(tr("Change the gap between paragraphs"));
    connect(setParagraphSpacingAct, SIGNAL(triggered()),
            this, SLOT(setParagraphSpacing()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(aboutQt()));

    leftAlignAct = new QAction(tr("&Left Align"), this);
    leftAlignAct->setCheckable(true);
    leftAlignAct->setShortcut(tr("Ctrl+L"));
    leftAlignAct->setStatusTip(tr("Left align the selected text"));
    connect(leftAlignAct, SIGNAL(triggered()), this, SLOT(leftAlign()));

    rightAlignAct = new QAction(tr("&Right Align"), this);
    rightAlignAct->setCheckable(true);
    rightAlignAct->setShortcut(tr("Ctrl+R"));
    rightAlignAct->setStatusTip(tr("Right align the selected text"));
    connect(rightAlignAct, SIGNAL(triggered()), this, SLOT(rightAlign()));

    justifyAct = new QAction(tr("&Justify"), this);
    justifyAct->setCheckable(true);
    justifyAct->setShortcut(tr("Ctrl+J"));
    justifyAct->setStatusTip(tr("Justify the selected text"));
    connect(justifyAct, SIGNAL(triggered()), this, SLOT(justify()));

    centerAct = new QAction(tr("&Center"), this);
    centerAct->setCheckable(true);
    centerAct->setShortcut(tr("Ctrl+E"));
    centerAct->setStatusTip(tr("Center the selected text"));
    connect(centerAct, SIGNAL(triggered()), this, SLOT(center()));

    alignmentGroup = new QActionGroup(this);
    alignmentGroup->addAction(leftAlignAct);
    alignmentGroup->addAction(rightAlignAct);
    alignmentGroup->addAction(justifyAct);
    alignmentGroup->addAction(centerAct);
    leftAlignAct->setChecked(true);
}

void Experiments::createMenus()
{
    //experimentsMenu = menuBar()->addMenu(tr("&Windows"));
    fileMenu = menu_bar->addMenu(tr("&File"));
    fileMenu->addAction(newAct);

    fileMenu->addAction(openAct);

    fileMenu->addAction(saveAct);
    fileMenu->addAction(printAct);

    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menu_bar->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();

    helpMenu = menu_bar->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    formatMenu = editMenu->addMenu(tr("&Format"));
    formatMenu->addAction(boldAct);
    formatMenu->addAction(italicAct);
    formatMenu->addSeparator()->setText(tr("Alignment"));
    formatMenu->addAction(leftAlignAct);
    formatMenu->addAction(rightAlignAct);
    formatMenu->addAction(justifyAct);
    formatMenu->addAction(centerAct);
    formatMenu->addSeparator();
    formatMenu->addAction(setLineSpacingAct);
    formatMenu->addAction(setParagraphSpacingAct);
}

/*Experiments::~Experiments()
{
    delete ui;
}*/

/* Метод для инициализации модеи представления данных
 * */
void Experiments::setupModel(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * с установкой имени таблицы в базе данных, по которому
     * будет производится обращение в таблице
     * */
    model = new QSqlTableModel(this);
    model->setTable(tableName);

    /* Устанавливаем названия колонок в таблице с сортировкой данных
     * */
    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    //model->setSort(0,Qt::AscendingOrder);
    //model->setSort(0, Qt::AscendingOrder);

}

void Experiments::setupModelDevices(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * с установкой имени таблицы в базе данных, по которому
     * будет производится обращение в таблице
     * */
    modelDevices = new QSqlTableModel(this);
    modelDevices->setTable(tableName);

    /* Устанавливаем названия колонок в таблице с сортировкой данных
     * */
    for(int i = 0, j = 0; i < modelDevices->columnCount(); i++, j++){
        modelDevices->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    modelDevices->setSort(0,Qt::AscendingOrder);
}

void Experiments::createUI()
{
    tableView->setModel(model);     // Устанавливаем модель на TableView
    tableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
    tableView->setColumnHidden(1, false);
    tableView->setColumnHidden(2, false);
    tableView->setColumnHidden(3, false);
    tableView->setColumnHidden(4, false);
    tableView->setColumnHidden(5, true);
    tableView->setColumnHidden(6, true);
    tableView->setColumnHidden(7, true);
    tableView->setColumnHidden(8, true);


    // Разрешаем выделение строк
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    tableView->resizeColumnsToContents();
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->horizontalHeader()->setStretchLastSection(true);

    /*QString str = QString("1=1 ORDER BY CASE "EXPERIMENTS_AUTHOR
                          " WHEN '%1' THEN '1' ELSE '0' END DESC")
            .arg(login->toLower());*/

    model->setFilter(QString("1=1 ORDER BY CASE "EXPERIMENTS_AUTHOR
                             " WHEN '%1' THEN '1' ELSE '0' END DESC")
               .arg(login->toLower()));


    //также надо добавить для dialogExp!!!

    model->select();

    connect(tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(slotStartExp(QModelIndex)));
    connect(tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotEditExp(QModelIndex)));


    tableViewDevices->setModel(modelDevices);     // Устанавливаем модель на tableViewDevices
    tableViewDevices->setColumnHidden(0, true);    // Скрываем колонку с id записей
    tableViewDevices->setColumnHidden(1, false);
    tableViewDevices->setColumnHidden(2, false);
    tableViewDevices->setColumnHidden(3, true);
    tableViewDevices->setColumnHidden(4, false);
    tableViewDevices->setColumnHidden(5, true);
    tableViewDevices->setColumnHidden(6, false);


    // Разрешаем выделение строк
    tableViewDevices->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    tableViewDevices->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    tableViewDevices->resizeColumnsToContents();
    tableViewDevices->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableViewDevices->horizontalHeader()->setStretchLastSection(true);

    modelDevices->select(); // Делаем выборку данных из таблицы

    //connect(tableViewDevices, SIGNAL(clicked(QModelIndex)), this, SLOT(slotLogin(QModelIndex)));
    //connect(tableViewDevices, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotEditUser(QModelIndex)));

}

/* Метод для активации диалога добавления записей
 * */
/*void Experiments::on_addDeviceButton_clicked()
{

    DialogAuth *addDialogAuth = new DialogAuth();
    connect(addDialogAuth, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    addDialogAuth->setWindowTitle(trUtf8("Добавить"));
    addDialogAuth->exec();
}*/

void Experiments::slotNewExp()
{
    /* Создаем диалог и подключаем его сигнал завершения работы
     * к слоту обновления вида модели представления данных
     * */
    DialogExp *addDialogExp = new DialogExp();
    connect(addDialogExp, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    /* Выполняем запуск диалогового окна
     * */
    addDialogExp->setWindowTitle(trUtf8("Add"));
    addDialogExp->exec();
    //showlogoWindow();
}


void Experiments::slotEditExp(QModelIndex index)
{


    /* Создаем диалог и подключаем его сигнал завершения работы
     * к слоту обновления вида модели представления данных
     * */


    DialogExp *addDialogExp = new DialogExp(index.row());
    connect(addDialogExp, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    /* Выполняем запуск диалогового окна
     * */
    addDialogExp->setWindowTitle(trUtf8("Edit"));
    addDialogExp->exec();
}


void Experiments::slotUpdateModels()
{
    model->select();
}

/* Метод для активации диалога добавления записей в режиме редактирования
 * с передачей индекса выбранной строки
 * */
void Experiments::slotStartExp(QModelIndex index)
{
    /* Также создаем диалог и подключаем его сигнал завершения работы
     * к слоту обновления вида модели представления данных, но передаём
     * в качестве параметров строку записи
     * */
    /*DialogAuth *addDialogAuth = new DialogAuth(index.row());
    connect(addDialogAuth, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    connect(addDialogAuth, SIGNAL(accepted()),this, SIGNAL(showlogoWindow()));


    addDialogAuth->setWindowTitle(trUtf8("Authorization"));
    addDialogAuth->exec();*/

    /*if(addDialogAuth->exec()){
        QMessageBox::information(this, trUtf8("Check"),
                                 trUtf8("True"));

    } else {
        QMessageBox::information(this, trUtf8("Check"),
                                 trUtf8("False"));
    }*/

}


QGroupBox *Experiments::groupTable()
{
    QGroupBox *groupBox = new QGroupBox(tr(""));

    //newuserButton = new QPushButton(tr("New user"));
    //edituserButton = new QPushButton(tr("Edit"));

    QVBoxLayout *vbox = new QVBoxLayout;
    //vbox->addWidget(newuserButton,0);
    //vbox->addWidget(edituserButton,1);
    vbox->setSpacing(0);
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setMargin(0);
    groupBox->setLayout(vbox);

    return groupBox;
}

#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include <QMainWindow>
#include <QSqlTableModel>
/* Подключаем заголовочный файл для работы с информацией, которая помещена в базу данных */
#include <database.h>
#include <dialogexp.h>
#include <QTableView>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QGroupBox;
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
QT_END_NAMESPACE

class Experiments : public QWidget
{
    Q_OBJECT

public:
    explicit Experiments(QString loginQString, QWidget *parent = 0);

    //~Experiments();

Q_SIGNALS:
    void showlogoWindow();
    //void openExperiments();

private Q_SLOTS:
    //void on_addDeviceButton_clicked();

    void slotUpdateModels();
    void slotStartExp(QModelIndex index);
    void slotNewExp();
    void slotEditExp(QModelIndex index);

    void newFile();
    void open();
    void save();
    void print();
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void bold();
    void italic();
    void leftAlign();
    void rightAlign();
    void justify();
    void center();
    void setLineSpacing();
    void setParagraphSpacing();
    void about();
    void aboutQt();

protected:
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

private:
    QString         *login;
    DataBase        *db;
    QSqlTableModel  *model;
    QSqlTableModel  *modelDevices;
    QGroupBox       *groupTable();
    QTableView      *tableView;
    QTableView      *tableViewDevices;
    QPushButton     *newExpButton;
    QPushButton     *editExpButton;
    QPushButton     *tableButton;


private:
    void createMenus();
    void createActions();
    QMenuBar *menu_bar;
    QMenuBar *menu_barDevices;
    QMenu *experimentsMenu;
    QMenu *recentFilesMenu;
    QMenu *editMenu;
    QMenu *fileMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;

    QActionGroup *alignmentGroup;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *printAct;
    QAction *exitAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *boldAct;
    QAction *italicAct;
    QAction *leftAlignAct;
    QAction *rightAlignAct;
    QAction *justifyAct;
    QAction *centerAct;
    QAction *setLineSpacingAct;
    QAction *setParagraphSpacingAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QLabel *infoLabel;


    /* Также присутствуют два метода, которые формируют модель
     * и внешний вид TableView
     * */
    void setupModel(const QString &tableName, const QStringList &headers);
    void setupModelDevices(const QString &tableName, const QStringList &headers);

    void createUI();
};

#endif // EXPERIMENTS_H

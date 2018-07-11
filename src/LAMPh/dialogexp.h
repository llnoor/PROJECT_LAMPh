#ifndef DIALOGEXP_H
#define DIALOGEXP_H

#include <QMainWindow>
#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QMessageBox>
#include "database.h"
#include <QTableView>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QGroupBox;
QT_END_NAMESPACE

class DialogExp : public QDialog
{
    Q_OBJECT

public:
    explicit DialogExp(int row = -1, QWidget *parent = 0);
    ~DialogExp();

Q_SIGNALS:
    void signalReady();

private Q_SLOTS:
    void startExp();
    void save_data();
    void delete_data();
    void updateButtons(int row);

private:
    QGroupBox                   *groupTableExp();
    QTableView                  *tableViewEditExp;
    QSqlTableModel              *model;
    QDataWidgetMapper           *mapper;
    QPushButton                 *previousButton;
    QPushButton                 *nextButton;
    QPushButton                 *saveButton;
    QPushButton                 *newButton;
    QPushButton                 *deleteButton;
    QPushButton                 *startExpButton;

    QLabel   *labelExperimentName;
    QLabel   *labelExperimentDescription;
    QLabel   *labelExperimentAuthor;
    QLabel   *labelExperimentDate;
    QLabel   *labelExperimentDevices;
    QLabel   *labelExperimentParent;
    QLabel   *labelExperimentLink;
    QLabel   *labelExperimentConf;

    QLabel   *labelEmpty;

    QLineEdit   *experimentName;
    QLineEdit   *experimentDescription;
    QLineEdit   *experimentAuthor;
    QLineEdit   *experimentDate;
    QLineEdit   *experimentDevices;
    QLineEdit   *experimentParent;
    QLineEdit   *experimentLink;
    QLineEdit   *experimentConf;

    QLineEdit   *checkExperiment;



private:
    void setupModel();
    //void createUI();
    void accept();

};

#endif // DIALOGEXP_H

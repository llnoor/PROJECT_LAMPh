#ifndef DIALOGREG_H
#define DIALOGREG_H

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

class DialogReg : public QDialog
{
    Q_OBJECT

public:
    explicit DialogReg(int row = -1, QWidget *parent = 0);
    ~DialogReg();

Q_SIGNALS:
    void signalReady();

private Q_SLOTS:
    void save_data();
    void delete_data();
    void updateButtons(int row);

private:
    QGroupBox                   *groupTableAuth();
    QTableView                  *tableViewEditAuth;
    QSqlTableModel              *model;
    QDataWidgetMapper           *mapper;
    QPushButton                 *previousButton;
    QPushButton                 *nextButton;
    QPushButton                 *saveButton;
    QPushButton                 *newButton;
    QPushButton                 *deleteButton;

    QLabel   *labelScientistName;
    QLabel   *labelScientistSurname;
    QLabel   *labelScientistLogin;
    QLabel   *labelScientistDate;
    QLabel   *labelScientistParent;
    QLabel   *labelScientistPass;
    QLabel   *labelScientistEmail;
    QLabel   *labelScientistTelegram;
    QLabel   *labelScientistPhone;
    QLabel   *labelScientistExperiments;
    QLabel   *labelScientistSelected_experiments;
    QLabel   *labelScientistBirthday;
    QLabel   *labelScientistPosition;
    QLabel   *labelScientistTheme;

    QLabel   *labelEmpty;

    QLineEdit   *scientistName;
    QLineEdit   *scientistSurname;
    QLineEdit   *scientistLogin;
    QLineEdit   *scientistDate;
    QLineEdit   *scientistParent;
    QLineEdit   *scientistPass;
    QLineEdit   *scientistEmail;
    QLineEdit   *scientistTelegram;
    QLineEdit   *scientistPhone;
    QLineEdit   *scientistExperiments;
    QLineEdit   *scientistSelected_experiments;
    QLineEdit   *scientistBirthday;
    QLineEdit   *scientistPosition;
    QLineEdit   *scientistTheme;


private:
    void setupModel();
    //void createUI();
    void accept();

};

#endif // DIALOGREG_H

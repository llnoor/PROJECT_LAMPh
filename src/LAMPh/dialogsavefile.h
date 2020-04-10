#ifndef DIALOGSAVEFILE_H
#define DIALOGSAVEFILE_H

#include <QMainWindow>
#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QProgressBar>

QT_BEGIN_NAMESPACE
class QGroupBox;
QT_END_NAMESPACE

class DialogSaveFile : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSaveFile(QString file, QVector<double> X, QVector<double> Y, QWidget *parent = 0);

private Q_SLOTS:
    void save_file();
    void check_file();
    //void delete_data();
    //void hide_empty(int);
    //void open_file();
    //void setCheckBox(int);

private:

    QGroupBox                   *groupTableSettings();
	
    QLabel  *labelSave;
    QPushButton *button_save;
    QProgressBar *progressBar;

    QLabel   *labelName;
    QLabel   *labelInfo;
    QLabel   *labelFileStr;

    QString filePath;
    QString newfilePath;
	QString nameDevice;
    QVector <double> qVectorX;
    QVector <double> qVectorY;


};

#endif // DIALOGSAVEFILE_H

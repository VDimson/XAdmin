#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QTreeWidgetItem>
#include "dialog.h"
#include <QTime>
#include <QTimer>
#include <QElapsedTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    struct messUser {
            QString mess;
            QStringList ListUser;
            bool flagUser;
        };
public slots:

void Finish_XDisplay();
void Finish_ProcessUser();
void Finish_ProcDelUser();
void Err_ProcDelUser();
void RecievMessUser(struct messUser Soob);
void StartUserTop();



private slots:
void on_action_refresh_triggered();

void ObrabotkaStarUserTop();

void StatusReadUserMess();

void on_pushButton_clicked();

void on_twg_itemClicked(QTreeWidgetItem *item, int column);

void on_checkBox_proc_stateChanged(int arg1);
void Start_ProcUser(QString item);

void on_lineEdit_proc_editingFinished();



void on_twg_itemSelectionChanged();

void on_action_shutdown_triggered();

void on_action_message_triggered();
void TimeUpdate();
private:
void GetXDisplay();
    Ui::MainWindow *ui;
    QProcess* proc_XDisplay;
    QString vihlop="";
    QProcess* proc_User;

    QProcess* proc_delUser;
    Dialog* mess_form;
    QTimer *timer;
    QTimer *timerTopUser;
    QTime *time;
    QElapsedTimer *tm;
     bool flaDel=false;

};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QTreeWidgetItem>
#include "dialog.h"
#include <QTime>
#include <QTimer>
#include <QElapsedTimer>
#include <QSystemTrayIcon>

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
void Connect();


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
void MenuTwgShow();

//для трея
    void changeEvent(QEvent*);
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void trayActionExecute();
    void setTrayIconActions();
    void showTrayIcon();
    void trayIconExit();
    void on_action_connect_triggered();

private:
void GetXDisplay();
    Ui::MainWindow *ui;
    QProcess* proc_XDisplay;
    QString vihlop="";
    QProcess* proc_User;
    QProcess* proc_passwd;
    QProcess* proc_passwd2;
    QProcess* proc_delUser;
    QProcess* proc_connect;
    Dialog* mess_form;
    QTimer *timer;
    QTimer *timerTopUser;

    QElapsedTimer *tm;
     bool flaDel=false;
      QMenu *menuTwg;

     //хлам для тестов
     QString t="";
     //для трея
         QMenu *trayIconMenu;
         QAction *minimizeAction;
         QAction *restoreAction;
         QAction *quitAction;
         QSystemTrayIcon *trayIcon;

};

#endif // MAINWINDOW_H

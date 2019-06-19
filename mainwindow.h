#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:

void Finish_XDisplay();

private slots:
void on_action_refresh_triggered();



void on_pushButton_clicked();

private:
void GetXDisplay();
    Ui::MainWindow *ui;
    QProcess* proc_XDisplay;
    QString vihlop="";
};

#endif // MAINWINDOW_H

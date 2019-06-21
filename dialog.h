#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    struct messUser {
            QString mess;
            QStringList ListUser;
            bool flagUser;
        };


void SetSpisokUser(QStringList &SpisokUser);
signals:
void SendMessUser(struct messUser Soob);

private slots:
void on_checkBox_clicked();

void on_buttonBox_accepted();

private:
    Ui::Dialog *ui;
QString spisok="";
QStringList SpisUser;



};

#endif // DIALOG_H

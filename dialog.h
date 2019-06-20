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
        };

void SetSpisokUser(QStringList &SpisokUser);
signals:
void SendMessUser(struct messUser Soob);

private:
    Ui::Dialog *ui;



};

#endif // DIALOG_H

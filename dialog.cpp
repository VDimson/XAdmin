#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::SetSpisokUser(QStringList &SpisokUser)
{
QString spisok;
for (int i=0;i<SpisokUser.count();++i)
{
 spisok=SpisokUser.at(i)+"; ";
}
ui->lineEdit->setText(spisok);
}

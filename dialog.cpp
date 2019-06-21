#include "dialog.h"
#include "ui_dialog.h"
#include "QMessageBox"

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
    SpisUser=SpisokUser;

for (int j=0;j<SpisUser.count();++j)
{
 spisok=spisok+SpisUser.at(j)+"; ";
}
ui->lineEdit->setText(spisok);
}

void Dialog::on_checkBox_clicked()
{
    if (ui->checkBox->isChecked())
    {    ui->lineEdit->clear();}
    else
    {
    ui->lineEdit->setText(spisok);
    }

}

void Dialog::on_buttonBox_accepted()
{
    if (ui->textEdit->toPlainText().isEmpty())
    {
        QMessageBox::critical(0,"Внимание!","Вы не можете отправить пустое сообщение!");
        return;
    }
 struct messUser FullMess;
 if (ui->checkBox->isChecked())
 {
 SpisUser.clear();
 FullMess.flagUser=true;
 }
 else
 {FullMess.flagUser=false;}

 FullMess.mess=ui->textEdit->toPlainText();
 FullMess.ListUser=SpisUser;
emit SendMessUser(FullMess);


}

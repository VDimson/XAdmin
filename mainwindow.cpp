#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //настройка сортировки в таблице по столбцам
    ui->twg->header()->sortIndicatorOrder();
    ui->twg->sortByColumn(0,Qt::AscendingOrder);
    ui->twg->setSortingEnabled(true);
    ui->checkBox_proc->setChecked(true);
    proc_XDisplay=new QProcess(this);//выделяем память для процесса
    proc_User=new QProcess(this);
    proc_delUser=new QProcess(this);
ui->action_message->setDisabled(true);
ui->action_shutdown->setDisabled(true);
    connect(proc_XDisplay,SIGNAL(finished(int)),this,SLOT(Finish_XDisplay()));
    connect(proc_User,SIGNAL(finished(int)),this,SLOT(Finish_ProcessUser()));
    connect(proc_delUser,SIGNAL(finished(int)),this,SLOT(Finish_ProcDelUser()));
    connect(proc_delUser,SIGNAL(readyReadStandardError()),this,SLOT(Err_ProcDelUser()));

    GetXDisplay();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::GetXDisplay()
{
proc_XDisplay->start("ps",QStringList()<<"-ef");
}
void MainWindow::Finish_XDisplay()
{
    ui->twg->clear();

    ui->action_message->setDisabled(true);
    ui->action_shutdown->setDisabled(true);
    vihlop=proc_XDisplay->readAllStandardOutput();
        QStringList Stroki=vihlop.split("\n",QString::SkipEmptyParts);

        for (int i=1;i<Stroki.count();++i)
        {
         QString Stroka=Stroki.at(i);
         QStringList ItemStroka=Stroka.split(" ",QString::SkipEmptyParts);
         if (ItemStroka.at(7)=="Xorg")
       {
    QTreeWidgetItem* twgStroka = new QTreeWidgetItem(ui->twg);//добавим строку в таблицу пользователей !
    twgStroka->setText(0,ItemStroka.at(0));//пользователь
    twgStroka->setText(1,ItemStroka.at(1));//id XORG
    twgStroka->setText(2,ItemStroka.at(2));//id Sesman
    twgStroka->setText(3,ItemStroka.at(8));//номер DISPLAY
    twgStroka->setText(4,ItemStroka.at(4));//время подключения
    twgStroka->setText(5,ItemStroka.at(6));//длительность подключения
        }
        }

}
void MainWindow::Finish_ProcessUser()
{
    ui->textEdit->append(proc_User->readAllStandardOutput());
}
void MainWindow::Finish_ProcDelUser()
{
    GetXDisplay();

    QString mess;
    if (proc_delUser->exitCode()==0)
    {
mess="Завершение процессов завершено корректно! exitCode:"+QString::number(proc_delUser->exitCode());
    }
    else
    {
mess="Завершенить процессы не удалось, возможно нет достаточных привилегий ! exitCode:"+QString::number(proc_delUser->exitCode());
    }
    ui->centralWidget->setCursor(Qt::ArrowCursor);
    QMessageBox::information(0,"Сообщение!",mess);
}
void MainWindow::Err_ProcDelUser()
{
    ui->statusBar->showMessage(proc_delUser->readAllStandardError());
    ui->centralWidget->setCursor(Qt::ArrowCursor);
}
void MainWindow::on_action_refresh_triggered()
{

   GetXDisplay();

}


void MainWindow::on_pushButton_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,"Внимание","Хотите закрыть программу?",QMessageBox::Yes | QMessageBox::No);
    if (reply==QMessageBox::Yes)
        {QApplication::quit();}
}
void MainWindow::Start_ProcUser(QString item)
{
    ui->textEdit->clear();
//QString element=item->text(0);
if (ui->checkBox_proc->isChecked())
{

proc_User->start("sh");
proc_User->waitForStarted();
QString comandaStr="ps U "+item+" | grep "+ui->lineEdit_proc->text()+"\n";

const char* comandaChr; //определяем символьную переменную
//disp=disp_com.toStdString().allocator_type//перевод строки в символную переменную
comandaChr=comandaStr.toLocal8Bit().data();
proc_User->write(comandaChr);
proc_User->closeWriteChannel();
}
else
{
proc_User->start("ps",QStringList()<<"U"<<item);
}
}

void MainWindow::on_twg_itemClicked(QTreeWidgetItem *item, int column)
{
    Start_ProcUser(item->text(0));


}

void MainWindow::on_checkBox_proc_stateChanged(int arg1)

{

    QString items;
    if (ui->twg->currentItem())
    {
        items=ui->twg->currentItem()->text(0);
   Start_ProcUser(items);

    }

}

void MainWindow::on_lineEdit_proc_editingFinished()
{

    QString items;
    if (ui->twg->currentItem())
    {
        items=ui->twg->currentItem()->text(0);
   Start_ProcUser(items);

    }
}



void MainWindow::on_twg_itemSelectionChanged()
{
    QString items;
    if (ui->twg->currentItem())
    {
        items=ui->twg->currentItem()->text(0);
        Start_ProcUser(items);
        if (items=="root")
        {
        ui->action_shutdown->setDisabled(true);
        ui->action_message->setDisabled(false);
        }
        else
        {
        ui->action_message->setDisabled(false);
        ui->action_shutdown->setDisabled(false);
        }



    }
    else
    {
        ui->action_message->setDisabled(true);
        ui->action_shutdown->setDisabled(true);
    }



}

void MainWindow::on_action_shutdown_triggered()
{
    if (ui->twg->currentItem())
    {

        QMessageBox::StandardButton reply = QMessageBox::question(this,"Внимание","Вы дествительно хотите завершить работу пользователя: "+ui->twg->currentItem()->text(0),QMessageBox::Yes | QMessageBox::No);
        if (reply==QMessageBox::Yes)
            {
ui->centralWidget->setCursor(Qt::WaitCursor);
        proc_delUser->start("killall",QStringList()<<"-w"<<"-u"<<ui->twg->currentItem()->text(0));
        }
    }
}

void MainWindow::on_action_message_triggered()
{
    mess_form=new Dialog(this);
    QStringList ListUserMess;
    ListUserMess.append(ui->twg->currentItem()->text(0));
    mess_form->SetSpisokUser(ListUserMess);
    mess_form->show();
}

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
   // ui->twg->setSelectionMode(QAbstractItemView::currentChanged());


    ui->checkBox_proc->setChecked(true);
    proc_XDisplay=new QProcess(this);//выделяем память для процесса
    proc_User=new QProcess(this);
    proc_delUser=new QProcess(this);
//ui->action_message->setDisabled(true);
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
    //ui->twg->clear();

    //ui->action_message->setDisabled(true);
    ui->action_shutdown->setDisabled(true);
    vihlop=proc_XDisplay->readAllStandardOutput();
        QStringList Stroki=vihlop.split("\n",QString::SkipEmptyParts);

        for (int i=1;i<Stroki.count();++i)
        {
         QString Stroka=Stroki.at(i);
         QStringList ItemStroka=Stroka.split(" ",QString::SkipEmptyParts);
         if (ItemStroka.at(7)=="Xorg")
       {
//ищем строки в таблице, возможно уже есть данные
             QTreeWidgetItem* items ;
             QList<QTreeWidgetItem*> findItems;
             findItems=ui->twg->findItems(ItemStroka.at(0),Qt::MatchContains |Qt::MatchRecursive,0);//получаем список искомых так НАДО ! хотя должен быть всегда один штука
             if (!findItems.isEmpty())
             {
             items=findItems.at(0);//забираем из списка первый, логины не могут повторяться, поэто список будет всегда с одним элементом!
//меняем в найденной записи некторые поля

             items->setText(1,ItemStroka.at(1));//id XORG
             items->setText(2,ItemStroka.at(2));//id Sesman
             items->setText(3,ItemStroka.at(8));//номер DISPLAY
             items->setText(4,ItemStroka.at(4));//время подключения
             items->setText(5,ItemStroka.at(6));//длительность подключения
             }
             else
             {
    QTreeWidgetItem* twgStroka = new QTreeWidgetItem(ui->twg);//добавим строку в таблицу пользователей !
    twgStroka->setCheckState(0,Qt::Unchecked);
    //twgStroka->setIcon(7,QIcon(":/ikonka/image/mess.png"));
    twgStroka->setText(0,ItemStroka.at(0));//пользователь
    twgStroka->setText(1,ItemStroka.at(1));//id XORG
    twgStroka->setText(2,ItemStroka.at(2));//id Sesman
    twgStroka->setText(3,ItemStroka.at(8));//номер DISPLAY
    twgStroka->setText(4,ItemStroka.at(4));//время подключения
    twgStroka->setText(5,ItemStroka.at(6));//длительность подключения
             }
        }
        }

}
void MainWindow::Finish_ProcessUser()
{

    ui->twg2->clear();

    QString vihlp=proc_User->readAllStandardOutput();
     ui->textEdit->append(vihlp);
        QStringList Stroki=vihlp.split("\n",QString::SkipEmptyParts);

        for (int i=0;i<Stroki.count();++i)
        {
         QString Stroka=Stroki.at(i);
         QStringList ItemStroka=Stroka.split(" ",QString::SkipEmptyParts);
         QTreeWidgetItem* twg2Stroka = new QTreeWidgetItem(ui->twg2);
         twg2Stroka->setText(0,ItemStroka.at(0));
         twg2Stroka->setText(1,ItemStroka.at(2));
         twg2Stroka->setText(2,ItemStroka.at(3));
         for (int l=0;l<ItemStroka.count();++l)
         {
             if (ItemStroka.at(l)=='/IBName"')
             {
                 twg2Stroka->setText(3,ItemStroka.at(l+1));
             }
         }
//         //twg2Stroka->setText(3,ItemStroka.at(4));
//         QStringList ItemStroka2=Stroka.split("IBName",QString::SkipEmptyParts);
//         if (ItemStroka2.at(1)=="")
//         {
//             ui->statusBar->showMessage("пустая");
//       //twg2Stroka->setText(3,ItemStroka2.at(1));
//         }
        }


}
void MainWindow::Finish_ProcDelUser()
{
    GetXDisplay();

    QString mess;
    if (proc_delUser->exitCode()==0)
    {
        QProcess *p=(QProcess*)sender();
        QTreeWidgetItem* items ;
        QList<QTreeWidgetItem*> findItems;
        findItems=ui->twg->findItems(p->objectName(),Qt::MatchContains |Qt::MatchRecursive,0);//получаем список искомых так НАДО ! хотя должен быть всегда один штука
        items=findItems.at(0);
        delete items;

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
        //ui->action_message->setDisabled(false);
        }
        else
        {
       // ui->action_message->setDisabled(false);
        ui->action_shutdown->setDisabled(false);
        }



    }
    else
    {
       // ui->action_message->setDisabled(true);
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
        proc_delUser->setObjectName(ui->twg->currentItem()->text(0));
        }
    }
}
void MainWindow::RecievMessUser(struct messUser Soob)
{
    ui->textEdit->clear();
    ui->textEdit->append("Текст сообщения: \n"+Soob.mess);
    if (Soob.flagUser)
    {
        ui->statusBar->showMessage("Отправляем всем пользователям!");
        QProcess *procc_mess[ui->twg->topLevelItemCount()];
        for (int i = 0; i < ui->twg->topLevelItemCount(); i++)
        {
            QTreeWidgetItem * itemss = ui->twg->topLevelItem(i);
            procc_mess[i]=new QProcess(this);
            procc_mess[i]->setObjectName(itemss->text(0));
            connect(procc_mess[i],SIGNAL(finished(int)),this,SLOT(StatusReadUserMess()));
            //procc_mess[i]->start("echo",QStringList()<<"Сработал"<<QString::number(i));
            procc_mess[i]->start("sh");
            procc_mess[i]->waitForStarted();
            QString disp_com="export DISPLAY='"+itemss->text(3)+"'\n";

            const char* disp; //определяем символьную переменную
            //disp=disp_com.toStdString().allocator_type//перевод строки в символную переменную
            disp=disp_com.toLocal8Bit().data();
            procc_mess[i]->write(disp);
            procc_mess[i]->waitForBytesWritten();
            QString textmess="zenity --question --title='Внимание,сообщение!' --text='"+Soob.mess+"'\n";
            const char* testmessChr; //определяем символьную переменную
            //disp=disp_com.toStdString().allocator_type//перевод строки в символную переменную
            testmessChr=textmess.toLocal8Bit().data();
            procc_mess[i]->write(testmessChr);
            //procc_mess[i]->write("zenity --question --title='Внимание,сообщение!' --text='Просьбы выйти из системы!!!Вы согласны?'\n");
            procc_mess[i]->closeWriteChannel();
            //поставим знак в таблице о том, что отправлено сообщение!
//            QTreeWidgetItem* items1 ;
//            QList<QTreeWidgetItem*> findItems1;
//            findItems1=ui->twg->findItems(Soob.ListUser.at(i),Qt::MatchContains |Qt::MatchRecursive,0);//получаем список искомых так НАДО ! хотя должен быть всегда один штука
//            items1=findItems1.at(0);//забираем из списка первый, логины не могут повторяться, поэто список будет всегда с одним элементом!
            itemss->setIcon(7,QIcon(":/ikonka/image/envelope.png"));
            itemss->setText(7,"отправ");
            itemss->setText(8,Soob.mess);
            //ui->statusBar->showMessage(items->text(3));//получаем значеине вQString е из четвертого столбца , номер дисплея XOR
                }

    }
    else
    {
       ui->statusBar->showMessage("Отправляем выбранным пользователям !");
       //количество итерация будет равно количеству выбранных польщователей
       QProcess *procc_mess[Soob.ListUser.count()];
 for (int i=0;i<Soob.ListUser.count();++i)

 {
//выполним поиск номера дисплея в таблице twg по пользователю из полученной структуры
QTreeWidgetItem* items ;
QList<QTreeWidgetItem*> findItems;
findItems=ui->twg->findItems(Soob.ListUser.at(i),Qt::MatchContains |Qt::MatchRecursive,0);//получаем список искомых так НАДО ! хотя должен быть всегда один штука
items=findItems.at(0);//забираем из списка первый, логины не могут повторяться, поэто список будет всегда с одним элементом!
//ui->statusBar->showMessage(items->text(3));//получаем значеине вQString е из четвертого столбца , номер дисплея XORG
//заполняем массив с процессами !

procc_mess[i]=new QProcess(this);
procc_mess[i]->setObjectName(Soob.ListUser.at(i));
connect(procc_mess[i],SIGNAL(finished(int)),this,SLOT(StatusReadUserMess()));
//procc_mess[i]->start("echo",QStringList()<<"Сработал"<<QString::number(i));
procc_mess[i]->start("sh");
procc_mess[i]->waitForStarted();
QString disp_com="export DISPLAY='"+items->text(3)+"'\n";

const char* disp; //определяем символьную переменную
//disp=disp_com.toStdString().allocator_type//перевод строки в символную переменную
disp=disp_com.toLocal8Bit().data();
procc_mess[i]->write(disp);
procc_mess[i]->waitForBytesWritten();
QString textmess="zenity --question --title='Внимание,сообщение!' --text='"+Soob.mess+"'\n";
const char* testmessChr; //определяем символьную переменную
//disp=disp_com.toStdString().allocator_type//перевод строки в символную переменную
testmessChr=textmess.toLocal8Bit().data();
procc_mess[i]->write(testmessChr);
//procc_mess[i]->write("zenity --question --title='Внимание,сообщение!' --text='Просьбы выйти из системы!!!Вы согласны?'\n");
procc_mess[i]->closeWriteChannel();
//поставим знак в таблице о том, что отправлено сообщение!
QTreeWidgetItem* items1 ;
QList<QTreeWidgetItem*> findItems1;
findItems1=ui->twg->findItems(Soob.ListUser.at(i),Qt::MatchContains |Qt::MatchRecursive,0);//получаем список искомых так НАДО ! хотя должен быть всегда один штука
items1=findItems1.at(0);//забираем из списка первый, логины не могут повторяться, поэто список будет всегда с одним элементом!
items1->setIcon(7,QIcon(":/ikonka/image/envelope.png"));
items1->setText(7,"отправ");
items1->setText(8,Soob.mess);
//ui->statusBar->showMessage(items->text(3));//получаем значеине вQString е из четвертого столбца , номер дисплея XOR
    }
}
}
//слот отслеживания прочтения сообщений!
void MainWindow::StatusReadUserMess()
{
    QProcess *p=(QProcess*)sender();

//sender();
ui->textEdit->append(p->objectName());
ui->textEdit->append("ExitCode: "+QString::number(p->exitCode()));
QTreeWidgetItem* items1 ;
QList<QTreeWidgetItem*> findItems1;
findItems1=ui->twg->findItems(p->objectName(),Qt::MatchContains |Qt::MatchRecursive,0);//получаем список искомых так НАДО ! хотя должен быть всегда один штука
items1=findItems1.at(0);//забираем из списка первый, логины не могут повторяться, поэто список будет всегда с одним элементом!
if (p->exitCode()==0)
{
items1->setIcon(7,QIcon(":/ikonka/image/envelope-check.png"));
items1->setText(7,"одобрено");
}
else
{
    items1->setIcon(7,QIcon(":/ikonka/image/envelope cross.png"));
    items1->setText(7,"отклонено!");
}
}

void MainWindow::on_action_message_triggered()
{
     QStringList ListUserMess;
    for (int i = 0; i < ui->twg->topLevelItemCount(); i++)
    {

        QTreeWidgetItem * item = ui->twg->topLevelItem(i);
        if (item->checkState(0) == Qt::Checked)
        {
        ListUserMess.append(item->text(0));
        }
    }
if (ListUserMess.isEmpty())
{
 QMessageBox::information(0,"Сообщение!","Не выбран ни один пользователь!");
return;
}
    mess_form=new Dialog(this);
//соединяем сигнал обратной связи из формы диалога сообщения
    connect(mess_form,SIGNAL(SendMessUser(messUser)),this,SLOT(RecievMessUser(messUser)));
// устанавливаем значение переменной в дочерней форме-диалог
    mess_form->SetSpisokUser(ListUserMess);
    mess_form->show();//показываем форму!
}

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
    //настройка сортировки в таблице2 по столбцам
    ui->twg2->header()->sortIndicatorOrder();
    ui->twg2->sortByColumn(0,Qt::AscendingOrder);
    ui->twg2->setSortingEnabled(true);

   // ui->twg->setSelectionMode(QAbstractItemView::currentChanged());

//создадим контекстное меню по правой кнопке в первой таблице
    menuTwg=new QMenu(ui->twg);
    QList<QAction*> act;
    act.append(ui->action_message);
    act.append(ui->action_shutdown);
    menuTwg->addActions(act);
//--------------------------------------------------
//настройка в таблице
ui->twg->setContextMenuPolicy(Qt::CustomContextMenu);
connect(ui->twg,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(MenuTwgShow()));
    ui->checkBox_proc->setChecked(true);
    proc_XDisplay=new QProcess(this);//выделяем память для процесса
    proc_User=new QProcess(this);
    proc_delUser=new QProcess(this);

//ui->action_message->setDisabled(true);
ui->action_shutdown->setDisabled(true);
//создадим в памяти таймер и время

timer=new QTimer(this);
timerTopUser=new QTimer(this);
//tm=new QElapsedTimer;
//законнектим таймер со слотом обработки
    connect(timer,SIGNAL(timeout()),this,SLOT(TimeUpdate()));
    connect(timerTopUser,SIGNAL(timeout()),this,SLOT(StartUserTop()));
timer->start(5000); //запуск таймера с интервалом 1с
timerTopUser->start(1000);
    //time->start();
    //tm->start();
    connect(proc_XDisplay,SIGNAL(finished(int)),this,SLOT(Finish_XDisplay()));
    connect(proc_User,SIGNAL(finished(int)),this,SLOT(Finish_ProcessUser()));
    connect(proc_delUser,SIGNAL(finished(int)),this,SLOT(Finish_ProcDelUser()));
    connect(proc_delUser,SIGNAL(readyReadStandardError()),this,SLOT(Err_ProcDelUser()));
  //  connect(proc_delUser,SIGNAL(readyReadStandardOutput()),this,SLOT(Read_TopUser()));

//    this->setTrayIconActions();
//    this->showTrayIcon();
    GetXDisplay();

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::MenuTwgShow()
{
    menuTwg->exec(QCursor::pos());
}
void MainWindow::TimeUpdate()
{
    if (flaDel)
    {
        return;
    }
GetXDisplay();
ui->statusBar->showMessage("Сработал рефреш!!!",500);
}
void MainWindow::GetXDisplay()
{
    if (flaDel)
    {
        return;
    }
proc_XDisplay->start("ps",QStringList()<<"-ef");
}

void MainWindow::Finish_XDisplay()
{
    if (flaDel)
    {
        return;
    }
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

//вытащим точные данные о логине, т.к. в выхлопе этого процесса логин укорочен, напрмер glavbuhcvm сокращен и представлен как glavbuh+
//вдальнейшем нужен полный точный логин , возмеме его командой cat /etc/passwd | grep user-первых 7 символов.
QString login;
QString fullLogin;
login=ItemStroka.at(0);
QString login7=login.left(7);
//запустим процесс
proc_passwd =new QProcess(this);
proc_passwd->start("sh");
proc_passwd->waitForStarted();
QString paramStr="cat /etc/passwd | grep "+login7+"\n";
const char* param; //определяем символьную переменную
//disp=disp_com.toStdString().allocator_type//перевод строки в символную переменную
param=paramStr.toLocal8Bit().data();
proc_passwd->write(param);
proc_passwd->waitForBytesWritten();
proc_passwd->closeWriteChannel();
proc_passwd->waitForReadyRead();
QString vihlopPasswd=proc_passwd->readAllStandardOutput();
proc_passwd->close();
//получили список логинов, начинающих первых 7 букв
QStringList Stroki2=vihlopPasswd.split("\n",QString::SkipEmptyParts);
for (int j=0;j<Stroki2.count();++j)

{
//запросим перебором по всем  найденным логинам , есть ли у нх вообще процессы Xorg

    QString StrokaPasswd=Stroki2.at(j);


QStringList ItemStrokaPasswd=StrokaPasswd.split(":",QString::SkipEmptyParts);

QString fLogin=ItemStrokaPasswd.at(0);

////   сделаем запрос
    proc_passwd2 =new QProcess(this);
    proc_passwd2->start("sh");
    proc_passwd2->waitForStarted();
    QString paramStr2="ps -fU "+fLogin+" | grep Xorg\n";
    const char* param2; //определяем символьную переменную
    //перевод строки в символную переменную
    param2=paramStr2.toLocal8Bit().data();
    proc_passwd2->write(param2);
    proc_passwd2->waitForBytesWritten();
    proc_passwd2->closeWriteChannel();
    proc_passwd2->waitForReadyRead();
    QString vihlopPasswd2=proc_passwd2->readAllStandardOutput();
    proc_passwd2->close();
    if (proc_passwd2->exitCode()==1)
    {
continue;
    }
    QString kod=QString::number(proc_passwd2->exitCode());


    if (vihlopPasswd2.isEmpty())
   {
       continue;
    }
QStringList StrokavihlopPasswd2=vihlopPasswd2.split(" ",QString::SkipEmptyParts);
//t=t+fLogin+" code:"+kod+" displ:"+StrokavihlopPasswd2.at(8)+"\n";
if (ItemStroka.at(8)==StrokavihlopPasswd2.at(8))
    {
       //t=t+fLogin+" code:"+kod+" displ:"+StrokavihlopPasswd2.at(8)+"\n";
    fullLogin=fLogin;

    }

}

//ui->textEdit->setText(t);


//ищем строки в таблице, возможно уже есть данные
             QTreeWidgetItem* items ;

             QList<QTreeWidgetItem*> findItems;
             findItems=ui->twg->findItems(fullLogin,Qt::MatchWildcard |Qt::MatchRecursive,0);//получаем список искомых так НАДО ! хотя должен быть всегда один штука
             if (!findItems.isEmpty())
             {
             items=findItems.at(0);//забираем из списка первый, логины не могут повторяться, поэто список будет всегда с одним элементом!
             //проверим  изменилась ди длительность активности??


//меняем в найденной записи некторые поля

             items->setText(1,ItemStroka.at(1));//id XORG
             items->setText(2,ItemStroka.at(2));//id Sesman
             items->setText(3,ItemStroka.at(8));//номер DISPLAY
             items->setText(4,ItemStroka.at(4));//время подключения
             items->setText(5,ItemStroka.at(6));//длительность активности




             }
             else
             {
    QTreeWidgetItem* twgStroka = new QTreeWidgetItem(ui->twg);//добавим строку в таблицу пользователей !
    twgStroka->setCheckState(0,Qt::Unchecked);
    //twgStroka->setIcon(7,QIcon(":/ikonka/image/mess.png"));
    twgStroka->setText(0,fullLogin);//пользователь
    twgStroka->setText(1,ItemStroka.at(1));//id XORG
    twgStroka->setText(2,ItemStroka.at(2));//id Sesman
    twgStroka->setText(3,ItemStroka.at(8));//номер DISPLAY
    twgStroka->setText(4,ItemStroka.at(4));//время подключения
    twgStroka->setText(5,ItemStroka.at(6));//длительность подключения
    twgStroka->setText(6,"???");
             }
        }
        }

on_twg_itemSelectionChanged();
}
void MainWindow::Finish_ProcessUser()
{

    ui->twg2->clear();

    QString vihlp=proc_User->readAllStandardOutput();
     ui->textEdit->append(vihlp);
        QStringList Stroki=vihlp.split("\n",QString::SkipEmptyParts);
        if (Stroki.isEmpty())
        {
            return;
        }

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
                 twg2Stroka->setText(3,ItemStroka.at(l));
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
        findItems=ui->twg->findItems(p->objectName(),Qt::MatchWildcard|Qt::MatchRecursive,0);//получаем список искомых так НАДО ! хотя должен быть всегда один штука
        items=findItems.at(0);
        delete items;
        flaDel=false;

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
flaDel=true;
        proc_delUser->start("killall",QStringList()<<"-w"<<"-u"<<ui->twg->currentItem()->text(0));
        proc_delUser->setObjectName(ui->twg->currentItem()->text(0));
        }
    }
}
void MainWindow::ObrabotkaStarUserTop()
{
    if (flaDel)
    {
        return;
    }
    QProcess *p=(QProcess*)sender();
    QString vihlp;
    vihlp=p->readAllStandardOutput();
    QStringList Stroki=vihlp.split("\n",QString::SkipEmptyParts);
    if (Stroki.count()<7)
    {

        QList<QTreeWidgetItem*> findItems2;
        QTreeWidgetItem *items2;
        findItems2=ui->twg->findItems(p->objectName(),Qt::MatchWildcard |Qt::MatchRecursive,0);//получаем список искомых так НАДО ! хотя должен быть всегда один штука
        items2=findItems2.at(0);
        delete items2;
          return;

    }
     QString Stroka=Stroki.at(6);
     if (Stroka.isEmpty())
     {
         return;
     }
     QStringList ItemStroka=Stroka.split(" ",QString::SkipEmptyParts);
     QTreeWidgetItem* items1 ;
     QList<QTreeWidgetItem*> findItems1;
     findItems1=ui->twg->findItems(p->objectName(),Qt::MatchWildcard |Qt::MatchRecursive,0);//получаем список искомых так НАДО ! хотя должен быть всегда один штука
     items1=findItems1.at(0);//забираем из списка первый, логины не могут повторяться, поэто список будет всегда с одним элементом!
     if (p->exitCode()==0)
     {
     //items1->setIcon(6,QIcon(":/ikonka/image/envelope-check.png"));

     items1->setText(6,ItemStroka.at(8));
     }
     else
     {
         //items1->setIcon(7,QIcon(":/ikonka/image/envelope cross.png"));
         items1->setText(7,"err!");
     }
   //  ui->statusBar->showMessage(ItemStroka.at(8));
}
void MainWindow::StartUserTop()
{
    if (flaDel)
    {
        return;
    }
    QProcess *procc_top[ui->twg->topLevelItemCount()];
    for (int i = 0; i < ui->twg->topLevelItemCount(); i++)
    {
        QTreeWidgetItem * itemss = ui->twg->topLevelItem(i);

        procc_top[i]=new QProcess(this);
        procc_top[i]->setObjectName(itemss->text(0));
        connect(procc_top[i],SIGNAL(finished(int)),this,SLOT(ObrabotkaStarUserTop()));
        procc_top[i]->start("sh");
        procc_top[i]->waitForStarted();
        QString disp_com="top -b -u "+itemss->text(0)+" -n 1\n";

        const char* disp; //определяем символьную переменную
        //disp=disp_com.toStdString().allocator_type//перевод строки в символную переменную
        disp=disp_com.toLocal8Bit().data();
        procc_top[i]->write(disp);
        procc_top[i]->waitForBytesWritten();
        procc_top[i]->closeWriteChannel();
    }
}
void MainWindow::RecievMessUser(struct messUser Soob)
{
    ui->textEdit->clear();
    ui->textEdit->append("Текст сообщения: \n"+Soob.mess);
    if (Soob.flagUser)
    {
        //ui->statusBar->showMessage("Отправляем всем пользователям!");
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
            itemss->setCheckState(0,Qt::Unchecked);
            //ui->statusBar->showMessage(items->text(3));//получаем значеине вQString е из четвертого столбца , номер дисплея XOR
                }

    }
    else
    {
      // ui->statusBar->showMessage("Отправляем выбранным пользователям !");
       //количество итерация будет равно количеству выбранных польщователей
       QProcess *procc_mess[Soob.ListUser.count()];
 for (int i=0;i<Soob.ListUser.count();++i)

 {
//выполним поиск номера дисплея в таблице twg по пользователю из полученной структуры
QTreeWidgetItem* items ;
QList<QTreeWidgetItem*> findItems;
findItems=ui->twg->findItems(Soob.ListUser.at(i),Qt::MatchWildcard |Qt::MatchRecursive,0);//получаем список искомых так НАДО ! хотя должен быть всегда один штука
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
findItems1=ui->twg->findItems(Soob.ListUser.at(i),Qt::MatchWildcard |Qt::MatchRecursive,0);//получаем список искомых так НАДО ! хотя должен быть всегда один штука
items1=findItems1.at(0);//забираем из списка первый, логины не могут повторяться, поэто список будет всегда с одним элементом!
items1->setIcon(7,QIcon(":/ikonka/image/envelope.png"));
items1->setText(7,"отправ");
items1->setText(8,Soob.mess);
items1->setCheckState(0,Qt::Unchecked);
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
findItems1=ui->twg->findItems(p->objectName(),Qt::MatchWildcard |Qt::MatchRecursive,0);//получаем список искомых так НАДО ! хотя должен быть всегда один штука
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

//для трея
//void MainWindow::showTrayIcon()
//{
//    // Создаём экземпляр класса и задаём его свойства...


//        trayIcon =new QSystemTrayIcon(this);
//        QIcon trayImage(":/ikonka/image/user.png");
//        trayIcon->setIcon(trayImage);
//        trayIcon->setContextMenu(trayIconMenu);

//        // Подключаем обработчик клика по иконке...
//        connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

//        // Выводим значок...
//        trayIcon->show();
//}
//void MainWindow::setTrayIconActions()
//{
//    // Setting actions...
//        minimizeAction = new QAction("Свернуть", this);
//        restoreAction = new QAction("Восстановить", this);
//        quitAction = new QAction("Выход", this);

//        // Connecting actions to slots...
//        connect (minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
//        connect (restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
//        connect (quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

//        // Setting system tray's icon menu...
//        trayIconMenu = new QMenu(this);
//        trayIconMenu -> addAction (minimizeAction);
//        trayIconMenu -> addAction (restoreAction);
//        trayIconMenu -> addAction (quitAction);

//}

//void MainWindow::changeEvent(QEvent *event)
//{
//    QMainWindow::changeEvent(event);
//    if (event -> type() == QEvent::WindowStateChange)
//    {
//        if (isMinimized())
//        {
//            this -> hide();
//        }
//    }
//}

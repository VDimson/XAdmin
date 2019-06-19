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
    proc_XDisplay=new QProcess(this);

    connect(proc_XDisplay,SIGNAL(finished(int)),this,SLOT(Finish_XDisplay()));
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

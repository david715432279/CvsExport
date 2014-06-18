#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    csv_pcb_file = new CsvPcbFile(ui->twidget_oripcb,ui->twidget_robotpcb);
    connect(ui->openfile,SIGNAL(triggered()),this,SLOT(slotOpenFile()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotOpenFile()
{
    QString name;
    name = QFileDialog::getOpenFileName(this,"打开",".","*.csv");
    if(!name.isEmpty())
       // qDebug()<<"open file"<<name;
        csv_pcb_file->openCSVFile(name);
       // openFile(name);
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QMessageBox>

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
    if(!name.isEmpty()){
       // qDebug()<<"open file"<<name;
        if(csv_pcb_file->openCSVFile(name)){
            ///step1 初始化导入后的UI
            initFileOpenedUi();
           // ui->btn_exportrow->setEnabled(true);
        }
    }

    // openFile(name);
}

void MainWindow::slotEnableExportRow()
{
    ui->btn_exportrow->setEnabled(true);
}

void MainWindow::slotEnabltDeleteRow()
{
    ui->btn_deletrow->setEnabled(true);
}

void MainWindow::on_btn_exportrow_clicked()
{
    //step1 如果选择的元器件是正确的话
    if(ui->twidget_oripcb->currentRow()>=0){
        //step1.1 插入相应的行
        ui->twidget_robotpcb->insertRow(ui->twidget_robotpcb->rowCount());
        //step1.2 将数据写入到行当中
       // for(int i=0; i<ui->twidget_robotpcb->columnCount();i++){
        //step1.2 判断相关的空格中的数是否正常
            ui->twidget_robotpcb->setItem(ui->twidget_robotpcb->rowCount()-1,Designator,new QTableWidgetItem(ui->twidget_oripcb->item(ui->twidget_oripcb->currentRow(),ui->ledit_designator->text().toInt()-1)->text()));
            ui->twidget_robotpcb->setItem(ui->twidget_robotpcb->rowCount()-1,Footprint,new QTableWidgetItem(ui->twidget_oripcb->item(ui->twidget_oripcb->currentRow(),ui->ledit_footprint->text().toInt()-1)->text()));
            ui->twidget_robotpcb->setItem(ui->twidget_robotpcb->rowCount()-1,Center_X,new QTableWidgetItem(ui->twidget_oripcb->item(ui->twidget_oripcb->currentRow(),ui->ledit_centerx->text().toInt()-1)->text()));
            ui->twidget_robotpcb->setItem(ui->twidget_robotpcb->rowCount()-1,Center_Y,new QTableWidgetItem(ui->twidget_oripcb->item(ui->twidget_oripcb->currentRow(),ui->ledit_centery->text().toInt()-1)->text()));
            ui->twidget_robotpcb->setItem(ui->twidget_robotpcb->rowCount()-1,Rotation,new QTableWidgetItem(ui->twidget_oripcb->item(ui->twidget_oripcb->currentRow(),ui->ledit_rotation->text().toInt()-1)->text()));
            ui->twidget_robotpcb->setItem(ui->twidget_robotpcb->rowCount()-1,ComponentHeight,new QTableWidgetItem(ui->ledit_component_height->text()+"mm"));
            ui->twidget_robotpcb->setItem(ui->twidget_robotpcb->rowCount()-1,FootHeight,new QTableWidgetItem(ui->ledit_foot_height->text()+"mm"));
            ui->twidget_robotpcb->setItem(ui->twidget_robotpcb->rowCount()-1,OriX,new QTableWidgetItem(ui->ledit_edgex->text()+"mm"));
            ui->twidget_robotpcb->setItem(ui->twidget_robotpcb->rowCount()-1,OriY,new QTableWidgetItem(ui->ledit_edgey->text()+"mm"));

            // }
       // ui->twidget_oripcb->setRowHidden(ui->twidget_oripcb->currentRow(),true);  //隐藏已经选择的按钮
    }

    ui->btn_exportrow->setEnabled(false);

}

void MainWindow::on_btn_deletrow_clicked()
{
    //step1 如果选择的元器件是正确的话
    if(ui->twidget_robotpcb->currentRow()>=0){
         ui->twidget_robotpcb->removeRow(ui->twidget_robotpcb->currentRow());
    }
    ui->btn_deletrow->setEnabled(false);
}

void MainWindow::on_btn_exportfile_clicked()
{
    QFile *file;
    //QString temp;
    int temp = 1;

    if(ui->twidget_robotpcb->rowCount()<1){
        QMessageBox::information(NULL, "导出文件错误!", QString("没有足够的数据"),QMessageBox::Ok,QMessageBox::NoButton);
    }

    ///step1 定义并设置默认路径
    QString  current_path = QDir::homePath();

    ///step2 打开目录选择保存路径
    current_path = QFileDialog::getExistingDirectory(this, tr("select path to save file"), current_path);
    QDir::setCurrent(current_path);

    ///step3 设置文件对象
    file = new QFile(QString(DEFAULT_FILE_NAME) + QString(DEFAULT_FILE_END));

    ///step4 如果文件存在需要改变文件名字
    while(file->exists()){
        file->setFileName( DEFAULT_FILE_NAME + QString("%1").arg(temp)+DEFAULT_FILE_END);
        temp++;
    }

    ///step5 打开文件开始准备写入数据
     if (!file->open(QFile::WriteOnly | QFile::Text)){
         QMessageBox::information(NULL, tr("file error"), QString("Open file %1 error!").arg(file->fileName()),QMessageBox::Ok,QMessageBox::NoButton);
         return;
     }

    writeCsvTitle(file);
    writeCsvData(file);

    file->close();

}

void MainWindow::initFileOpenedUi()
{
    //step1 初始化右边的菜单信息
    QStringList temp_data;   //用于存储数据信息

    temp_data.append("Designator");
    temp_data.append("Footprint");
    temp_data.append("Center X");
    temp_data.append("Center Y");
    temp_data.append("Rotation");
    temp_data.append("Component Height");
    temp_data.append("Foot Height");
    temp_data.append("Ori X");
    temp_data.append("Ori Y");

    ui->twidget_robotpcb->setColumnCount(temp_data.count());
    ui->twidget_robotpcb->setHorizontalHeaderLabels(temp_data);

    //step2 连接导出信号和槽
    connect(ui->twidget_oripcb,SIGNAL(clicked(QModelIndex)),this,SLOT(slotEnableExportRow()));
    connect(ui->twidget_robotpcb,SIGNAL(clicked(QModelIndex)),this,SLOT(slotEnabltDeleteRow()));
}

bool MainWindow::writeCsvTitle(QFile *file)
{
    QString temp;

  //  for(int i=0; i<ui->twidget_robotpcb->columnCount();i++){
  //  temp = "\"Designator\",\"Footprint\",\"Center X\",\"Center Y\",\"Rotation\",\"Component Height\",\"Component Height\",\"Foot Height\",\"Ori X\",\"Ori Y\"";
    temp = "\"Designator\",";
    temp.append(QString("\"Footprint\","));
    temp.append(QString("\"Center X\","));
    temp.append(QString("\"Center Y\","));
    temp.append(QString("\"Rotation\","));
    temp.append(QString("\"Component Height\","));
    temp.append(QString("\"Foot Height\","));
    temp.append(QString("\"Ori X\","));
    temp.append(QString("\"Ori Y\""));

    QTextStream out(file);
    qDebug()<<temp<<endl;
    out<<temp<<endl;
    temp = "\"\"";
    out<<temp<<endl;
    return true;
}

bool MainWindow::writeCsvData(QFile *file)
{
    QString data_begin = QString("\"");
    QString data_end = QString("\"");
    QString templine;
    QTextStream out(file);

    for(int i=0; i < ui->twidget_robotpcb->rowCount(); i++){
        templine = "";//初始化变量
        templine.append(data_begin);
        for(int j=0; j<ui->twidget_robotpcb->columnCount();j++){
            if(j){
                templine.append(QString(","));
                templine.append(data_begin);
            }
            templine.append(ui->twidget_robotpcb->item(i,j)->text());
            templine.append(data_end);
        }
        out<<templine<<endl;
    }
    return true;
}

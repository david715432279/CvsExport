#include "csvpcbfile.h"
#include <QMessageBox>
#include <QDebug>

CsvPcbFile::CsvPcbFile(QTableWidget* twidget_pcb, QTableWidget* twidget_robot, QObject *parent) :
    QObject(parent),
    twidget_ori_pcb(twidget_pcb),
    twidget_robot_pcb(twidget_robot)
{
    options = 0;
}

bool CsvPcbFile::openCSVFile(QString file_name)
{
    QStringList temp_data;   //用于存储数据信息

    pcb_csv_file = new QFile(file_name);

    ///step1 打开文件
    if(!pcb_csv_file->open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::information(NULL, tr("file error"), QString("Open file %1 error!").arg(pcb_csv_file->fileName()),QMessageBox::Ok,QMessageBox::NoButton);
        return false;
    }
    ///step1.5 初始化csv结构体
    if (csv_init(&p, options) != 0) {
        QMessageBox::information(NULL, tr("csv init error"), QString("init csv error!").arg(pcb_csv_file->fileName()),QMessageBox::Ok,QMessageBox::NoButton);
        return false;
    }

  //  header.append("");

    ///step2 先读取第一行,用来显示标实符号,写出相关的抬头
    if((bytes_read = pcb_csv_file->readLine(buf,BUF_LENGTH)) > 0){
        if(csv_parse(&p, buf, bytes_read, readdata1, readdata2, &temp_data) != bytes_read) {
          fprintf(stderr, "Error while parsing file: %s\n", csv_strerror(csv_error(&p)));
        }
    }else{
        return false;
    }
    ///step3 读取抬头之后需要设置表格的抬头
    twidget_ori_pcb->setColumnCount(temp_data.count());
    twidget_ori_pcb->setHorizontalHeaderLabels(temp_data);
    ///step3.1 清除信息
    temp_data.clear();

    ///step4 设置表格抬头以后需要读取表格的内容
   while((bytes_read = pcb_csv_file->readLine(buf,BUF_LENGTH)) > 0){
       ///step4.1 检测是否读到文件的尾部
       if(bytes_read == -1)
           break;
       ///step4.2 如果没有则检测数据
        if(csv_parse(&p, buf, bytes_read, readdata1, readdata2, &temp_data) != bytes_read) {
          fprintf(stderr, "Error while parsing file: %s\n", csv_strerror(csv_error(&p)));
        }
        ///step4.3 需要排除空行的情况
        if(temp_data.isEmpty()){
            continue;
        }

        ///step4.3 插入行
        twidget_ori_pcb->insertRow(twidget_ori_pcb->rowCount());
        ///step4.4 将数据写入到行当中
        for(int i=0; i<temp_data.size();i++){
            twidget_ori_pcb->setItem(twidget_ori_pcb->rowCount()-1,i,new QTableWidgetItem(temp_data.at(i)));
        }

        ///step4.4 清除信息
        temp_data.clear();

    }

   ///step5 结束csv文件访问并释放指针
   csv_fini(&p, readdata1, readdata2, &temp_data);
   csv_free(&p);

   ///step last 关闭文件描述符
    pcb_csv_file->close();
    return true;
}

void readdata1(void *s, size_t len, void *data)
{
    QString temp = QString((char *)s);
    QString result;

    //step1 选择出需要的内容
    for(int i=0; i<temp.size(); i++){
        if(temp.at(i)!='"'){
            result.append(temp.at(i));
        }else{
            break;
        }
    }

    //step2 设置项目抬头,如果为空则不加入
    if(!result.isEmpty())
        ((QStringList*)data)->append(result);
}

void readdata2(int c, void *data)
{
}

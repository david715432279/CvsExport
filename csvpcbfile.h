#ifndef CSVPCBFILE_H
#define CSVPCBFILE_H

#include <QObject>
#include <QTableWidget>
#include <csv.h>
#include <QFile>

#define BUF_LENGTH 1024


/// \brief 用于读取表格的抬头
/// \parm  void *s 指向读到单元格数据的指针
///        size_t len 表示数据长度
///        data    表示需要备用的数据结构操作符号
void readdata1(void *s, size_t len, void *data);
void readdata2(int c, void *data);

class CsvPcbFile : public QObject
{
    Q_OBJECT
public:
    explicit CsvPcbFile(QTableWidget* twidget_pcb, QTableWidget* twidget_robot,QObject *parent = 0);
    bool openCSVFile(QString file_name);

signals:

public slots:

private:
    QTableWidget* twidget_ori_pcb;
    QTableWidget* twidget_robot_pcb;
    struct csv_parser p;
    char buf[BUF_LENGTH];
    size_t bytes_read;
    unsigned char options;
    QFile * pcb_csv_file;
    QFile * robot_csv_file;


};

#endif // CSVPCBFILE_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <csvpcbfile.h>

#define DEFAULT_FILE_NAME    "robot_pcbfile"
#define DEFAULT_FILE_END     ".csv"

enum pcbfile{Designator=0,Footprint,Center_X,Center_Y,Rotation,ComponentHeight,FootHeight,OriX,OriY};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotOpenFile();
    void slotEnableExportRow();
    void slotEnabltDeleteRow();
    void on_btn_exportrow_clicked();
    void on_btn_deletrow_clicked();
    void on_btn_exportfile_clicked();

private:
    Ui::MainWindow *ui;
    CsvPcbFile* csv_pcb_file;

    void initFileOpenedUi();  //初始化文件已经打开后的ui
    bool writeCsvTitle(QFile* file);//写入CSV文件的抬头
    bool writeCsvData(QFile* file);//写入CSV文件的数据
};

#endif // MAINWINDOW_H

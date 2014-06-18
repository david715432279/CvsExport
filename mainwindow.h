#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <csvpcbfile.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void slotOpenFile();

private:
    Ui::MainWindow *ui;
    CsvPcbFile* csv_pcb_file;
};

#endif // MAINWINDOW_H

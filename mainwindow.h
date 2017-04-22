#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#define prunning 2
#define pready 1
#define pblocked 0

#include <QMainWindow>
#include<QDebug>
#include<string.h>
#include<QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    struct Process{
           int id=-1;
           int state=-1;//
           int priority=-1;//越小越优先
           int needtime=-1;
           int cputime=0;
//           int cnt=-1;
           Process *next=NULL;
    };

    Process *ready,*running,*blocked;
//    Process *readyTail;

    createProcess(int id,int priority,int needtime);
    terminateProcess(int id);

    blockProcess();
    wakeupProcess(int id);
    FCFS();
    void displayTable();
    void displayLink();
    void showWarnning(QString src);
    bool isDigitStr(QString src);
    

    

    

private slots:
    void on_lineEdit_cursorPositionChanged(int arg1, int arg2);

    void on_lineEdit_returnPressed();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_destroyed();

    void on_pIDEdit_cursorPositionChanged(int arg1, int arg2);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    
};

#endif // MAINWINDOW_H

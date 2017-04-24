#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#define prunning 2
#define pready 1
#define pblocked 0

#include <QMainWindow>
#include<QDebug>
#include<QMessageBox>

#include<string.h>
#include<stdio.h>
#include<stdlib.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

   typedef struct PCB{
           int id;
           int state;//
           int priority;
           int needtime;
           int cputime;
           PCB *next;
    }PCB;

    PCB* ready=(PCB*)malloc(sizeof(PCB));
    PCB* running=(PCB*)malloc(sizeof(PCB));
    PCB* blocked=(PCB*)malloc(sizeof(PCB));

//    PCB *readyTail;

    createPCB(int id,int priority,int needtime);
    terminatePCB(int id);

    blockPCB();
    wakeupPCB(int id);
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

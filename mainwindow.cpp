#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ready=new Process();
//    readyTail=readyHead;

    running=new Process();
    blocked=new Process();

//    readyHead->next=readyTail;

    running->next=NULL;
    blocked->next=NULL;

//    ui->tableWidget->setItem(0,0,new QTableWidgetItem("test"));
//    ui->tableWidget->setItem(1,1,new QTableWidgetItem(QString::number(345432)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


MainWindow::createProcess(int id,int priority,int needtime){
    Process *process=new Process();
    process->id=id;
    process->priority=priority;
    process->needtime=needtime;
    process->state=pready;
    process->next=NULL;
    Process* temp=ready;
    while(temp->next!=NULL){
        temp=temp->next;
        if(id==temp->id){
            QMessageBox *message=new QMessageBox();
            message->setWindowTitle("Warnning");
            message->setText("进程ID已存在！");
            message->exec();
//            message->exec();
            return 0;
        }
    }
//    qDebug()<<temp->id<<temp->priority;
    temp->next=process;
}

void MainWindow::on_pushButton_clicked()
{
    int id=ui->pIDEdit->text().toInt();
    int priority=ui->pPriorityEdit->text().toInt();
    int needtime=ui->pNeedtimeEdit->text().toInt();

    if(!((isDigitStr(ui->pIDEdit->text()))&&
        (isDigitStr(ui->pPriorityEdit->text()))&&
            (isDigitStr(ui->pNeedtimeEdit->text())))){
       showWarnning("输入不合法，请检查！");
       return;
    }

//    if(isDigitStr("678908765")){
//        qDebug()<<"no";
//    }


    if(needtime<=0){
        QMessageBox* message=new QMessageBox();
        message->setWindowTitle("Warnning");
        message->setText("所需时间应该为大于0的整数！");
        message->show();
        return;
    }
    createProcess(id,priority,needtime);

    displayTable();

    displayLink();
}

void MainWindow::displayTable(){
//    ui->tableWidget->clear();
    ui->tableWidget->clearContents();
    Process *temp=ready;
    int row=0;
    int cnt=3;
    qDebug()<<"test"<<endl;
    while(cnt--){
        qDebug()<<cnt;
        if(cnt==pready) temp=ready;
        else if(cnt==prunning) temp=running;
        else if(cnt==pblocked) temp=blocked;

        while(temp->next!=NULL){
         temp=temp->next;

         ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString::number(temp->id)));
         ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString::number(temp->priority)));
         QString states="";
         if(temp->state==prunning) states="running";
         else if(temp->state==pready) states="ready";
         else if(temp->state==pblocked) states="blocked";
          qDebug()<<"test"<<endl;
         ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString(states)));
         ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString::number(temp->needtime)));
         ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString::number(temp->cputime)));
         row++;
        }
        }

}


void MainWindow::displayLink(){
//    return;
//    if(head==NULL) return;
//    Process* te/*mp;
    Process* temp=ready;
    if(temp->next!=NULL){
        temp=temp->next;
        qDebug()<<ready->next->id;
    }
    while(temp->next!=NULL){
        temp=temp->next;
        qDebug()<<temp->id;
    }
}
















void MainWindow::on_lineEdit_cursorPositionChanged(int arg1, int arg2)
{

}

void MainWindow::on_lineEdit_returnPressed()
{

}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{

}

void MainWindow::on_lineEdit_destroyed()
{

}

void MainWindow::on_pIDEdit_cursorPositionChanged(int arg1, int arg2)
{

}


void MainWindow::on_pushButton_2_clicked()
{
    if(!isDigitStr(ui->pIDEdit2->text())){
        showWarnning("请输入正确的进程ID！");
        return;
    }
    int id=ui->pIDEdit2->text().toInt();
    terminateProcess(id);
    displayTable();
}
MainWindow::terminateProcess(int id){
    bool flag=false;
    Process* temp=ready;
    int cnt=3;
    while(cnt--){
        if(cnt==pready) temp=ready;
        else if(cnt==prunning) temp=running;
        else if(cnt==pblocked) temp=blocked;

        while(temp->next!=NULL){
            temp=temp->next;
            if(temp->id==id){
                if(temp->next==NULL){
                   Process* toBeDelete=temp;
                   Process* head=new Process();
                   if(cnt==pready) head=ready;
                   else if(cnt==prunning) head=running;
                   else if(cnt==pblocked) head=blocked;
                   while(head->next!=toBeDelete){
                       head=head->next;
                   }
                   head->next=NULL;
                   delete toBeDelete;
                }
                else{
                    Process* toBeDelete=temp->next;
                    temp->id=temp->next->id;
                    temp->priority=temp->next->priority;
                    temp->state=temp->next->state;
                    temp->next=temp->next->next;
                    delete toBeDelete;
                }

                QMessageBox* message=new QMessageBox();
                message->setWindowTitle("Hint");
                message->setText("撤销成功！");
                message->show();
                flag=true;
                return 0;
            }
        }
    }
    if(!flag){
        QMessageBox* message=new QMessageBox();
        message->setWindowTitle("Warnning");
        message->setText("未找到该进程！");
        message->exec();
    }
}

MainWindow::blockProcess(){
    if(running->next==NULL){
        QMessageBox* message=new QMessageBox();
        message->setWindowTitle("Warnning");
        message->setText("当前无进程在执行！");
        message->show();
        return 0;
    }
    else{
        Process* toBeBlock=running->next;
        Process* bhead=blocked;
        while(bhead->next!=NULL){
            bhead=bhead->next;
        }
        toBeBlock->next=NULL;
        toBeBlock->state=pblocked;
        bhead->next=toBeBlock;
        running->next=NULL;
    }


//    bool flag=false;
//    while(temp->next!=NULL){
//        temp=temp->next;
//        if(temp->id=id){
//            Process* toBeBlock=temp;
//            Process* rhead=running;
//            while(rhead->next!=toBeBlock){
//                rhead=rhead->next;
//            }
//            if(toBeBlock->next!=NULL)
//                rhead->next=toBeBlock->next;
//            else
//                rhead->next=NULL;

//            toBeBlock->next=NULL;
//            Process* bhead=blocked;
//            while(bhead->next!=NULL){
//                bhead=bhead->next;
//            }
//            bhead->next=toBeBlock;
//            flag=true;
//            return 0;
//        }
//    }
//    if(!flag) {
//        QMessageBox* message=new QMessageBox();
//        message->setWindowTitle("Warnning");
//        message->setText("未在运行态的进程中找到该进程！");
//        message->exec();
//    }
}

void MainWindow::on_pushButton_4_clicked()
{
    if(!isDigitStr(ui->pIDEdit4->text())){
        showWarnning("请输入正确的进程ID！");
        return;
    }
    int id=ui->pIDEdit4->text().toInt();
    wakeupProcess(id);
    displayTable();
}

MainWindow::wakeupProcess(int id){
    Process* temp=blocked;
    bool flag=false;
    while(temp->next!=NULL){
        temp=temp->next;
        if(temp->id==id){
            Process* toBeWake=temp;
            Process* bhead=blocked;
            while(bhead->next!=toBeWake){
                bhead=bhead->next;
            }
            if(toBeWake->next==NULL){
                bhead->next=NULL;
            }

            else{
                bhead->next=toBeWake->next;
            }
            toBeWake->next=NULL;
            toBeWake->state=pready;
            Process* rhead=ready;
            while(rhead->next!=NULL){
                rhead=rhead->next;
            }
            rhead->next=toBeWake;
            flag=true;
            return 0;
        }
    }
    if(!flag){
        QMessageBox* message=new QMessageBox();
        message->setWindowTitle("Warnning");
        message->setText("未在阻塞队列中找到该进程！");
        message->show();
    }

}

void MainWindow::on_pushButton_3_clicked()
{
    blockProcess();
    displayTable();
}

void MainWindow::on_pushButton_5_clicked()
{
    FCFS();
    displayTable();
}
MainWindow::FCFS(){
    if(running->next==NULL){
        if(ready->next==NULL) {
            QMessageBox* message=new QMessageBox();
            message->setWindowTitle("Warning");
            message->setText("当前无进程正在执行且就绪队列中无进程！");
            message->show();
            return 0;
        }
        Process* toBeRun=ready->next;
        if(toBeRun->next!=NULL)
            ready->next=toBeRun->next;
        else
            ready->next=NULL;
        toBeRun->next=NULL;
        toBeRun->state=prunning;

        running->next=toBeRun;
    }
    Process* runningProcess=running->next;
    runningProcess->cputime++;
    runningProcess->needtime--;
    if(runningProcess->needtime==0){
        running->next=NULL;
        delete runningProcess;
    }
}


bool MainWindow::isDigitStr(QString src)
{
    QByteArray ba = src.toLatin1();//QString 转换为 char*
    const char *s = ba.data();
    while(*s && *s>='0' && *s<='9') s++;

    if (*s)
    { //不是纯数字
        return false;
    }
    else
    { //纯数字
        return true;
    }
}
void MainWindow::showWarnning(QString src){
    QMessageBox* message=new QMessageBox();
    message->setWindowTitle("Warning");
    message->setText(src);
    message->show();
}

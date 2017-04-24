#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // ready=new PCB();
//    readyTail=readyHead;

//    running=malloc(sizeof(PCB));

    //blocked=new PCB();

//    readyHead->next=readyTail;
    ready->next=NULL;
    running->next=NULL;
    blocked->next=NULL;

//    ui->tableWidget->setItem(0,0,new QTableWidgetItem("test"));
//    ui->tableWidget->setItem(1,1,new QTableWidgetItem(QString::number(345432)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


MainWindow::createPCB(int id,int priority,int needtime){
    PCB* toBeCreate=(PCB*)malloc(sizeof(PCB));
    toBeCreate->id=id;
    toBeCreate->priority=priority;
    toBeCreate->needtime=needtime;
    toBeCreate->state=pready;
    toBeCreate->cputime=0;
    toBeCreate->next=NULL;
    PCB* temp=ready;
    int cnt=3;
    while(cnt--){
        if(cnt==2) temp=running;
        else if(cnt==1) temp=ready;
        else if(cnt==0) temp=blocked;

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
    }


//    qDebug()<<temp->id<<temp->priority;
    temp=ready;
    while(temp->next!=NULL) temp=temp->next;
    temp->next=toBeCreate;
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
    createPCB(id,priority,needtime);

    displayTable();

    displayLink();
}

void MainWindow::displayTable(){
//    ui->tableWidget->clear();
    ui->tableWidget->clearContents();
    PCB *temp=ready;
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
//    PCB* te/*mp;
    PCB* temp=ready;
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
    terminatePCB(id);
    displayTable();
}
MainWindow::terminatePCB(int id){
    bool flag=false;
    PCB* temp=ready;
    int cnt=3;
    while(cnt--){
        if(cnt==pready) temp=ready;
        else if(cnt==prunning) temp=running;
        else if(cnt==pblocked) temp=blocked;

        while(temp->next!=NULL){
            temp=temp->next;
            if(temp->id==id){
                if(temp->next==NULL){
                   PCB* toBeDelete=temp;
                   PCB* head=new PCB();
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
                    PCB* toBeDelete=temp->next;
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

MainWindow::blockPCB(){
    if(running->next==NULL){
        QMessageBox* message=new QMessageBox();
        message->setWindowTitle("Warnning");
        message->setText("当前无进程在执行！");
        message->show();
        return 0;
    }
    else{
        PCB* toBeBlock=running->next;
        PCB* bhead=blocked;
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
//            PCB* toBeBlock=temp;
//            PCB* rhead=running;
//            while(rhead->next!=toBeBlock){
//                rhead=rhead->next;
//            }
//            if(toBeBlock->next!=NULL)
//                rhead->next=toBeBlock->next;
//            else
//                rhead->next=NULL;

//            toBeBlock->next=NULL;
//            PCB* bhead=blocked;
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
    wakeupPCB(id);
    displayTable();
}

MainWindow::wakeupPCB(int id){
    PCB* temp=blocked;
    bool flag=false;
    while(temp->next!=NULL){
        temp=temp->next;
        if(temp->id==id){
            PCB* toBeWake=temp;
            PCB* bhead=blocked;
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
            PCB* rhead=ready;
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
    blockPCB();
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
        PCB* toBeRun=ready->next;
        if(toBeRun->next!=NULL)
            ready->next=toBeRun->next;
        else
            ready->next=NULL;
        toBeRun->next=NULL;
        toBeRun->state=prunning;

        running->next=toBeRun;
    }
    PCB* runningPCB=running->next;
    runningPCB->cputime++;
    runningPCB->needtime--;
    if(runningPCB->needtime==0){
        running->next=NULL;
        delete runningPCB;
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

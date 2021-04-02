#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFile"
#include "QMessageBox"
#include "qdir.h"
#include "QListWidget"
#include "qcustomplot.h"
#include "QTest"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->tabWidget,SIGNAL(currentChange()),this, SLOT(on_tabWidget_currentChanged()));
    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(Graph_Show()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::Graph_Show()//超时刷新
{

    QTime t;
    t=QTime::currentTime();
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    on_tabWidget_currentChanged(1);
    on_tabWidget_currentChanged(2);
    QDateTime curDateTime=QDateTime::currentDateTime();
    ui->dateTimeEdit->setDateTime(curDateTime);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    QFile  f;
    QString get_str;
    if(index == 0)//主机信息
    {
        f.setFileName("/proc/sys/kernel/hostname");//获取主机名
        if( !f.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(NULL, "warning", "The hostname file can not open!", QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
        get_str = f.readLine();
        QString host_name=get_str;
        ui->label_7->setText(host_name);
        f.close();


        f.setFileName("/proc/uptime");//获取系统启动时间和运行时间
        if( !f.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(NULL, "warning", "The uptime file can not open!", QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
        get_str = f.readLine();
        float alltime=get_str.section(" ",0,0).toFloat();
        float freetime=get_str.section(" ",1,1).toFloat();
        float runtime=alltime-freetime/8;
        int h = alltime/3600;
        int min = (alltime - h*3600)/60;
        int sec = alltime - h*3600 - min*60;
        QString all_time=get_str.sprintf ("%dh %dmin %dsec",h,min,sec);
        int run_h = runtime/3600;
        int run_min = (runtime - run_h*3600)/60;
        int run_sec = runtime - run_h*3600 - run_min*60;
        QString run_time=get_str.sprintf ("%dh %dmin %dsec",run_h,run_min,run_sec);
        ui->label_8->setText(all_time);
        ui->label_9->setText(run_time);
        f.close();


        f.setFileName("/proc/version");//获取系统版本
        if( !f.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(NULL, "warning", "The version file can not open!", QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
        get_str = f.readLine();
        QString version=get_str.mid(0,30);
        ui->label_10->setText(version);
        f.close();


        f.setFileName("/proc/cpuinfo");//获取cpu类型和频率
        if( !f.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(NULL, "warning", "The cpuinfo file can not open!", QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
        while (1)
        {
            get_str = f.readLine();
            if(get_str == NULL)
            {//文件读完，跳出
                break;
            }
            int from=0;
            if(from=get_str.indexOf("model name"),from!=-1)
            {
                QString cpu_id=get_str.mid(from+13,get_str.length());
                ui->label_11->setText(cpu_id);
            }
            if(from=get_str.indexOf("cpu MHz"),from!=-1)
            {
                QString cpu_hz=get_str.mid(from+10,get_str.length());
                ui->label_12->setText(cpu_hz);
            }
        }
        f.close();

    }
    else if(index == 1){
        ui->listWidget->clear();
        QDir qd("/proc");
        QStringList qslist=qd.entryList();
        QString qs=qslist.join("\n");
        int a=0,b=0;
        //int number_of_sleep=0,number_of_zombie=0,number_of_run=0;
        int find_start = 3;
        int all_pid_num=0;
        bool back;
        ui->listWidget->addItem("pid\tppid\tstate\tpriority\tmemsize\tname");
        while(1)
        {
            a=qs.indexOf("\n",find_start);//第一个参数为需要查找的字符串，第二个参数为从何处开始查找
            b=qs.indexOf("\n",a+1);
            find_start=b;
            QString name_of_pro=qs.mid(a+1,b-a-1);
            name_of_pro.toInt(&back,10);//第一个参数为转换是否成功，第二个参数为十进制
            if(!back)
            {
                break;
            }
            all_pid_num++;
            f.setFileName("/proc/"+ name_of_pro +"/stat");//获取pid information
            if( !f.open(QIODevice::ReadOnly) )
            {
                QMessageBox::warning(NULL, "warning", "The pid stat file can not open!", QMessageBox::Yes, QMessageBox::Yes);
                return;
            }
            QString temp = f.readAll();
            int from = temp.indexOf("(");
            int end = temp.indexOf(")");
            QString temp_proc_name=temp.mid(from+1,end-from-1);//get process name
            //due to the char(' ') between the first word and the second word,that can't use temp.section().

            QString temp_proc_state=temp.section(' ',2,2);//get process state
            //1表示用来切割的字符，2表示开始切割的位置，3表示切割的结束位置.
            QString temp_proc_ppid=temp.section(' ',3,3);//ppid
            //get process priority
            QString temp_proc_priority=temp.section(' ',17,17);
            //get process memor y use
            QString temp_proc_mem=temp.section(' ',22,22);//memsize
            char state = temp_proc_state.at(0).toLatin1();
            if(state!='S'&&state!='Z'&&state!='R'&&state!='D'&&state!='T'&&state!='X'&&state!='I')
            {//due to the char(' ') between the first word and the second word.
                temp_proc_state=temp.section(' ',3,3);//get process state
                temp_proc_ppid=temp.section(' ',4,4);
                temp_proc_priority=temp.section(' ',18,18);
                temp_proc_mem=temp.section(' ',23,23);//memsize
            }
            int size=temp_proc_mem.toUInt()/1024;//注意转换成无符号整数
            ui->listWidget->addItem(name_of_pro+'\t'+temp_proc_ppid+'\t'+temp_proc_state+'\t'+temp_proc_priority+'\t'+QString::number(size,10)+" KB"+'\t'+temp_proc_name);
            f.close();
        }
        ui->pid_num->setText(QString::number(all_pid_num));

    }
    else if(index == 2)
    {
        //绘制cpu使用率图像
        float totaltime[2]={0,0};
        float cputime[2][9];
        for(int k=0;k<2;k++)//取两个样点
        {
            f.setFileName("/proc/stat");
            if( !f.open(QIODevice::ReadOnly) )
            {
                QMessageBox::warning(NULL, "warning", "The proc stat file can not open!", QMessageBox::Yes, QMessageBox::Yes);
                return;
            }
            get_str = f.readLine();
            for(int i=0;i<9;i++)
            {
                cputime[k][i] = get_str.section(" ",i+2,i+2).toInt();
                totaltime[k]+=cputime[k][i];
            }
            f.close();
            QTest::qSleep(250);//否则cpu使用率会为0/nan
        }
        double user=qAbs(cputime[0][0]-cputime[1][0]);
        double system=qAbs(cputime[0][2]-cputime[1][2]);
        double total=qAbs(totaltime[0]-totaltime[1]);
        double cpu_use = 100*(user+system)/total;
        QString cpu_used=get_str.sprintf ("%.4f",cpu_use);
        ui->cpu_used->setText(cpu_used+'%');
        QVector<double> x(120);
        QVector<double> y(120);
        for(int i=0;i<119;i++)//循环赋值，利用数组存储数据
        {
            cpu[i]=cpu[i+1];
        }
        for(int i=0;i<120;i++)//循环赋值，初始化time数组
        {
            time[i]=119-i;
        }
        cpu[119]=cpu_use;
        for(int i=0;i<120;i++)//将数组赋值给vector所表示的x，y
        {
            x[i]=time[i];
            y[i]=cpu[i];
        }
        ui->graph->addGraph();
        ui->graph->graph(0)->setPen(QPen(Qt::red));
        ui->graph->graph(0)->setData(x,y);

        ui->graph->xAxis->setLabel("time/s");//x坐标轴
        ui->graph->yAxis->setLabel("cpu/%");//y坐标轴

        ui->graph->xAxis->setRange(0,120);//x坐标轴范围
        ui->graph->yAxis->setRange(0,50);//y坐标轴范围
        ui->graph->replot();//重新绘制图像



        //绘制内存和交换分区图像
        f.setFileName("/proc/meminfo");
        int mem_size;
        int mem_free_size;
        int swap_size;
        int swap_free_size;
        double mem_use=0;
        double swap_use=0;
        if( !f.open(QIODevice::ReadOnly) )
        {
            QMessageBox::warning(NULL, "warning", "The meminfo file can not open!", QMessageBox::Yes, QMessageBox::Yes);
            return;
        }
        while (1)
        {
            get_str = f.readLine();
            if(get_str == NULL)
            {//文件读完，跳出
                break;
            }
            int from=0;
            if(from=get_str.indexOf("MemTotal"),from!=-1)
            {
                QString mem_total=get_str.mid(from+10,get_str.length()-from-14);
                mem_total = mem_total.trimmed();//删除多余空格
                mem_size = mem_total.toInt();

            }
            if(from=get_str.indexOf("MemFree"),from!=-1)
            {
                QString mem_free=get_str.mid(from+10,get_str.length()-from-14).trimmed();
                mem_free_size = mem_free.toInt();
            }
            if(from=get_str.indexOf("SwapTotal"),from!=-1)
            {
                QString swap_total=get_str.mid(from+10,get_str.length()-from-14).trimmed();
                swap_size = swap_total.toInt();
            }
            if(from=get_str.indexOf("SwapFree"),from!=-1)
            {
                QString swap_free=get_str.mid(from+10,get_str.length()-from-14).trimmed();
                swap_free_size = swap_free.toInt();
            }
        }
        f.close();
        mem_use = 100 - mem_free_size*100/float(mem_size);
        QString mem_used = get_str.sprintf("%.4f",mem_use);
        ui->mem_used->setText(mem_used+"%");
        swap_use = 100 - swap_free_size*100/float(swap_size);
        QString swap_used = get_str.sprintf("%.4f",swap_use);
        ui->swap_used->setText(swap_used+"%");
        QVector<double> x1(120);
        QVector<double> y1(120);
        QVector<double> x2(120);
        QVector<double> y2(120);
        for(int i=0;i<119;i++)//循环赋值，利用数组存储数据
        {
            mem[i]=mem[i+1];
            swap[i]=swap[i+1];
        }
        mem[119]=mem_use;
        swap[119]=swap_use;
        for(int i=0;i<120;i++)//将数组赋值给vector所表示的x，y
        {
            x1[i]=time[i];
            y1[i]=mem[i];
            x2[i]=time[i];
            y2[i]=swap[i];
        }
        ui->mem_garph->addGraph();
        ui->mem_garph->graph(0)->setPen(QPen(Qt::red));
        ui->mem_garph->graph(0)->setData(x1,y1);
        ui->mem_garph->addGraph();
        ui->mem_garph->graph(1)->setPen(QPen(Qt::blue));
        ui->mem_garph->graph(1)->setData(x2,y2);
        ui->mem_garph->xAxis->setLabel("time/s");//x坐标轴
        ui->mem_garph->yAxis->setLabel("mem(red)|swap(green)/%");//y坐标轴

        ui->mem_garph->xAxis->setRange(0,120);//x坐标轴范围
        ui->mem_garph->yAxis->setRange(0,100);//y坐标轴范围
        ui->mem_garph->replot();//重新绘制图像

    }
}

void MainWindow::on_pushButton_clicked()
{
    system("shutdown -h now");
}

void MainWindow::on_pushButton_kill_clicked()
{
    QString pid_num = ui->lineEdit->text();
    system("kill "+pid_num.toLatin1());
    //ISO-8859-1编码是单字节编码，向下兼容ASCII，其编码范围是0x00-0xFF。亦称为Latin1
    on_tabWidget_currentChanged(1);
}

void MainWindow::on_pushButton_search_clicked()//查找进程信息
{
    QString pid_num = ui->lineEdit->text();
    QFile  f;
    QString get_str;
    QDir qd("/proc");
    QStringList qslist=qd.entryList();
    QString qs=qslist.join("\n");
    ui->listWidget->clear();
    ui->listWidget->addItem("pid\tppid\tstate\tpriority\tmemsize\tname");
    f.setFileName("/proc/"+ pid_num +"/stat");//获取pid information
    if( !f.open(QIODevice::ReadOnly) )
    {
        QMessageBox::warning(NULL, "warning", "The pid can not find!", QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    QString temp = f.readAll();
    int from = temp.indexOf("(");
    int end = temp.indexOf(")");
    QString temp_proc_name=temp.mid(from+1,end-from-1);//get process name
    //due to the char(' ') between the first word and the second word,that can't use temp.section().

    QString temp_proc_state=temp.section(' ',2,2);//get process state
    //1表示用来切割的字符，2表示开始切割的位置，3表示切割的结束位置.
    QString temp_proc_ppid=temp.section(' ',3,3);//ppid
    //get process priority
    QString temp_proc_priority=temp.section(' ',17,17);
    //get process memor y use
    QString temp_proc_mem=temp.section(' ',22,22);//memsize
    char state = temp_proc_state.at(0).toLatin1();
    if(state!='S'&&state!='Z'&&state!='R'&&state!='D'&&state!='T'&&state!='X'&&state!='I')
    {//due to the char(' ') between the first word and the second word.
        temp_proc_state=temp.section(' ',3,3);//get process state
        temp_proc_ppid=temp.section(' ',4,4);
        temp_proc_priority=temp.section(' ',18,18);
        temp_proc_mem=temp.section(' ',23,23);//memsize
    }
    int size=temp_proc_mem.toUInt()/1024;//注意转换成无符号整数
    ui->listWidget->addItem(pid_num+'\t'+temp_proc_ppid+'\t'+temp_proc_state+'\t'+temp_proc_priority+'\t'+QString::number(size,10)+" KB"+'\t'+temp_proc_name);
    f.close();
}

void MainWindow::on_pushButton_run_new_clicked()
{
    QString run_pid = ui->lineEdit_open->text();
    system(run_pid.toLatin1()+"&");//&：将该作业进程放到后台去，不会占用终端
    //ISO-8859-1编码是单字节编码，向下兼容ASCII，其编码范围是0x00-0xFF。亦称为Latin1
}

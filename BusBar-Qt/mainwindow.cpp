#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rtuthread.h"
#include "dpthread.h"
#include "currentalarmsdlg.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mInitShm = new InitShm(this);
    mInitShm->start();

    initSerial();

    mIndex = 0;
    initWidget();
    QTimer::singleShot(1000,this,SLOT(initFunSLot())); //延时初始化
    on_comboBox_currentIndexChanged(0);

    mCheckDlg = new CheckPasswordDlg(this);
    connect(mCheckDlg,SIGNAL(dialogClosed(bool)),this,SLOT(dialogClosed(bool)));
    // TestDlg *dlg = new TestDlg(this);
    // dlg->exec();
}

MainWindow::~MainWindow()
{
    delete ui;
    share_mem_del();
}


/**
 * @brief 初始化串口
 */
void MainWindow::initSerial()
{
    RtuThread *rtu = new RtuThread(this);
    rtu->init("ttyS2", 1);
    //    rtu->init(SERIAL_COM1, 1);



    //    rtu = new RtuThread(this);
    //    rtu->init(SERIAL_COM2, 2);

    //    rtu = new RtuThread(this);
    //    rtu->init(SERIAL_COM3, 3);

    //    rtu = new RtuThread(this);
    //    rtu->init(SERIAL_COM4, 4);
}

void MainWindow::timeoutDone()
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->timeLab->setText(time);

    checkAlarm();
    setBusName(mIndex);
}

void MainWindow::setBusName(int index)
{
    sDataPacket *shm = get_share_mem();
    char *name = shm->data[index].busName;

    QString str(name);
    ui->busNameLab->setText(str);
    mIndex = index;
}

void MainWindow::checkAlarm()
{
    int alarm = 0;
    sDataPacket *shm = get_share_mem();
    for(int i=0; i>BUS_NUM; ++i) {
        alarm += shm->data[i].busAlarm;
    }

    alarm = 1;  /////========= 特意显示出来
    if(alarm)  {
        ui->alarmBtn->setVisible(true);
    } else {
        ui->alarmBtn->setVisible(false);
    }
}

void MainWindow::initFunSLot()
{
    new DpThread(this); // 创建数据处理线程

    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

void MainWindow::initWidget()
{
    //    set_background_color(ui->stackedWid,Qt::white);
    set_background_icon(ui->stackedWid,":/new/prefix1/image/background.png");
    initBackground();

    mHomeWid = new HomeWid(ui->stackedWid);
    ui->stackedWid->addWidget(mHomeWid);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), mHomeWid, SIGNAL(busChangedSig(int)));

    mLineWid = new LineWid(ui->stackedWid);
    ui->stackedWid->addWidget(mLineWid);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), mLineWid, SIGNAL(busChangedSig(int)));

    mBranchWid = new BranchWid(ui->stackedWid);
    ui->stackedWid->addWidget(mBranchWid);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), mBranchWid, SIGNAL(busChangedSig(int)));

    mLogsWid = new LogsWid(ui->stackedWid);
    ui->stackedWid->addWidget(mLogsWid);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), mLogsWid, SIGNAL(busChangedSig(int)));

    mSettingWid = new SettingWid(ui->stackedWid);
    ui->stackedWid->addWidget(mSettingWid);
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), mSettingWid, SLOT(busChangedSlot(int)));
}

void MainWindow::setTimerStop()
{
    mLineWid->setRun(false);
    mBranchWid->setRun(false);
}

void MainWindow::on_homeBtn_clicked()
{
    setButtonClickedImage(ui->homeBtn,"home_select");
    ui->stackedWid->setCurrentWidget(mHomeWid);

    setTimerStop();
}

void MainWindow::on_lineBtn_clicked()
{
    setButtonClickedImage(ui->lineBtn,"main_select");
    ui->stackedWid->setCurrentWidget(mLineWid);

    setTimerStop();
    mLineWid->setRun(true);
}

void MainWindow::on_branchBtn_clicked()
{
    setButtonClickedImage(ui->branchBtn,"branch_select");
    ui->stackedWid->setCurrentWidget(mBranchWid);

    setTimerStop();
    mBranchWid->setRun(true);
}

void MainWindow::on_logBtn_clicked()
{
    setButtonClickedImage(ui->logBtn,"data_select");
    ui->stackedWid->setCurrentWidget(mLogsWid);
    setTimerStop();
}

void MainWindow::on_setBtn_clicked()
{
    if(ui->stackedWid->currentWidget() != mSettingWid) {
        mCheckDlg->exec();
    }
}

void MainWindow::on_alarmBtn_clicked()
{
    CurrentAlarmsDlg dlg(this);
    dlg.exec();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    setBusName(index);
}

void MainWindow::setButtonImage(QToolButton *button, QString name)
{
    QString str = tr("QToolButton{border-image: url(:/new/prefix1/image/%1.png);}").arg(name);
    button->setStyleSheet(str);
}

void MainWindow::setButtonClickedImage(QToolButton *button, QString name)
{
    initBackground();
    QString str = tr("QToolButton{border-image: url(:/new/prefix1/image/%1.png);}").arg(name);
    button->setStyleSheet(str);
}

void MainWindow::initBackground()
{
    setButtonImage(ui->homeBtn,"home");
    setButtonImage(ui->lineBtn,"main");
    setButtonImage(ui->branchBtn,"branch");
    setButtonImage(ui->logBtn,"data");
    setButtonImage(ui->setBtn,"setting");
}

void MainWindow::dialogClosed(bool ret)
{
    if(ret)
    {
        setButtonClickedImage(ui->setBtn,"setting_select");
        ui->stackedWid->setCurrentWidget(mSettingWid);
        setTimerStop();
    }
    else
        QMessageBox::information(this,"information","对不起，密码输入不正确，你不具备该权限！","确认");
    mCheckDlg->clear();
}

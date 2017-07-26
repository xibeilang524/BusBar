#include "homeboxwid.h"
#include "ui_homeboxwid.h"
#include "box/boxdlg.h"

HomeBoxWid::HomeBoxWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeBoxWid)
{
    ui->setupUi(this);
    initWid();
    mBusID = 0;
    
    timer = new QTimer(this);
    timer->start(2000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

HomeBoxWid::~HomeBoxWid()
{
    delete ui;
}

void HomeBoxWid::initFun(int base, int id)
{
    mID = base*10 + id+1;
    sDataPacket *shm = get_share_mem();
    mData = &(shm->data[mBusID].box[mID]);
    ui->titleLab->setText(QString::number(mID));
    updateData();
}


void HomeBoxWid::busChangeSlot(int id)
{
    mBusID = id;
    sDataPacket *shm = get_share_mem();
    mData = &(shm->data[id].box[mID]);
    updateData();
}

/**
 * @brief 更新电流显示
 */
void HomeBoxWid::updateData()
{
    QPalette pe;
    QString str =  QString::number(mData->tgBox.cur) + "A";
    if(mData->offLine)   // 在线
    {
        if(mData->boxCurAlarm == 2)
            pe.setColor(QPalette::WindowText,Qt::red);
        else  if(mData->boxCurAlarm == 1)
            pe.setColor(QPalette::WindowText,Qt::yellow);
        else
            pe.setColor(QPalette::WindowText,Qt::black);
    } else {
        str = "---";
        
    }
    ui->curLab->setText(str);
    ui->curLab->setPalette(pe);
}

void HomeBoxWid::updateAlarmIcon(QLabel *lab, sDataUnit &unit, int id)
{
    QString str;
    if(unit.alarm[id]) {
        str = "boxalarm"; //////======== 图片不对
    } else if(unit.crAlarm[id]) {
        str = "boxciralarm";
    } else {
        str = "boxonline";
    }
    setBackgroundImage(lab,str);
}

/**
 * @brief 更新报警图片
 */
void HomeBoxWid::updateAlarmStatus()
{
    int id = 0;
    //    mData->data.lineNum ;
    if(mData->offLine) {
        updateAlarmIcon(ui->iconLab_1,  mData->data.cur, id++);
        updateAlarmIcon(ui->iconLab_2,  mData->data.cur, id++);
        updateAlarmIcon(ui->iconLab_3,  mData->data.cur, id++);
    } else { // 离线
        setBackgroundImage(ui->iconLab_1, "boxoffine");
        setBackgroundImage(ui->iconLab_2, "boxoffine");
        setBackgroundImage(ui->iconLab_3, "boxoffine");
    }
}

void HomeBoxWid::timeoutDone()
{
    updateData();
    updateAlarmStatus();
}

void HomeBoxWid::on_pushButton_clicked()
{
    BoxDlg dlg(0);
    dlg.initBox(mBusID, mID);
    dlg.exec();
}

void HomeBoxWid::initWid()
{
    QSize size(95,95);
    set_background_icon(this, ":/new/prefix1/image/round.png", size);
}

void HomeBoxWid::setBackgroundImage(QLabel *lab, QString name)
{
    QString str = QString("border-image: url(:/new/prefix1/image/%1.png);").arg(name);
    lab->setStyleSheet(str);
}

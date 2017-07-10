#include "homeboxwid.h"
#include "ui_homeboxwid.h"

HomeBoxWid::HomeBoxWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeBoxWid)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

HomeBoxWid::~HomeBoxWid()
{
    delete ui;
}

void HomeBoxWid::initFun(int id)
{
    mID = id;
    sDataPacket *shm = get_share_mem();
    mData = &(shm->data[0].box[id]);
    ui->titleLab->setText(QString::number(id+1));
}

void HomeBoxWid::busChangeSlot(int id)
{
    sDataPacket *shm = get_share_mem();
    mData = &(shm->data[id].box[mID]);
    updateData();
}

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
    ui->iconLab->setText(str);
    ui->iconLab->setPalette(pe);
}

void HomeBoxWid::timeoutDone()
{
    updateData();
}

void HomeBoxWid::on_pushButton_clicked()
{
    qDebug() << "点击了接插箱，弹出相关信息";
}
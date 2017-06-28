#include "lineroadwid.h"
#include "ui_lineroadwid.h"

LineRoadWid::LineRoadWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LineRoadWid)
{
    ui->setupUi(this);

    busChangeSlot(0);
    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

LineRoadWid::~LineRoadWid()
{
    delete ui;
}



void LineRoadWid::busChangeSlot(int id)
{
     mID = id;
    mData = &(get_share_mem()->data[id].data);
    updateData();
}

void LineRoadWid::timeoutDone()
{
    updateData();
}


void LineRoadWid::updateAlarmStatus(QLabel *lab, sDataUnit &unit)
{
    int id = mID;
    QPalette pe;

    if(unit.alarm[id])
        pe.setColor(QPalette::WindowText,Qt::red);
    else  if(unit.crAlarm[id])
        pe.setColor(QPalette::WindowText,Qt::yellow);
    else
        pe.setColor(QPalette::WindowText,Qt::black);

    lab->setPalette(pe);
}


void LineRoadWid::updateData()
{
    int id = mID;
    QString str = "L" + QString::number(id+1) + tr("相");
    ui->name->setText(str);

    str = QString::number(mData->vol.value[id]) + "V";
    ui->volLab->setText(str);
    updateAlarmStatus(ui->volLab, mData->vol);

    str =  QString::number(mData->cur.value[id]) + "A";
    ui->curLab->setText(str);
    updateAlarmStatus(ui->curLab, mData->cur);

    str =  QString::number(mData->cur.max[id]) + "A";
    ui->maxCur->setText(str);

    str =  QString::number(mData->apPow[id]) + "KVA";
    ui->apPow->setText(str);

    str =  QString::number(mData->pow[id]) + "KW";
    ui->powLab->setText(str);

    str =  QString::number(mData->ele[id]) + "KWh";
    ui->eleLab->setText(str);
}

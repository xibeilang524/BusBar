/*
 * dpeleslavethread.cpp
 * 电能数据保存线程
 *
 *
 *  Created on: 2017年10月1日
 *      Author: Lzy
 */
#include "dpeleslavethread.h"
#include "dbbranchele.h"
#include "dbmainele.h"

DpEleSlaveThread::DpEleSlaveThread(QObject *parent) : QThread(parent)
{
    isRun = false;
    shm = get_share_mem(); // 获取共享内存

     timer = new QTimer(this);
     timer->start(1*60*1000);
     connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}


DpEleSlaveThread::~DpEleSlaveThread()
{
    isRun = false;
    wait();
}

void DpEleSlaveThread::timeoutDone()
{
    if(isRun == false) {
        start();
    }
}

void DpEleSlaveThread::saveBox(int bus, sBoxData  &box)
{
    int line = 0;
    double rate = 10;
    if(box.offLine == 0) return; //不保存
    sObjData *data = &(box.data);

    DbBranchEleItem item;
    item.name = box.boxName;
    item.loop_1 = data->ele[line++]/rate;
    item.loop_2 = data->ele[line++]/rate;
    item.loop_3 = data->ele[line++]/rate;
    item.loop = box.tgBox.ele/rate;
    db_branchEle_obj(bus)->insertItem(item);
    msleep(5);
}


void DpEleSlaveThread::saveBus(int id)
{
    int line = 0;
    double rate = 10;
    sBusData *bus = &(shm->data[id]);
    if(bus->tgBus.vol == 0) return; //不保存

    DbMainEleItem item;
    item.line_1 = bus->data.ele[line++]/rate;
    item.line_2 = bus->data.ele[line++]/rate;
    item.line_3 = bus->data.ele[line++]/rate;
    item.line = bus->tgBus.ele/rate;
    db_mainEle_obj(id)->insertItem(item);
    msleep(5);

    for(int i=0; i<bus->boxNum; ++i)
        saveBox(id, bus->box[i]);
}


void DpEleSlaveThread::run()
{
    if(isRun == false)
    {
        isRun  = true;

        for(int i=0; i<BUS_NUM; ++i)
            saveBus(i);

        isRun  = false;
    }
}
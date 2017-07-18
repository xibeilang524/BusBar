﻿#ifndef MAJORSETTING_H
#define MAJORSETTING_H

#include <QWidget>
#include <QTableWidgetItem>
#include "common.h"
#include "settingthreshold.h"
#include <QProgressBar>


namespace Ui {
class MajorSetting;
}

class MajorSetting : public QWidget
{
    Q_OBJECT

public:
    explicit MajorSetting(QWidget *parent = 0);
    ~MajorSetting();

    void updateWidget(int index);


private:
    void initWidget();
    void setProgressbarValue(QProgressBar *bar,sDataUnit  *data ,int index);
    //    sDataPacket *get_share_mem();
private slots:
    void barClicked(bool isCur,int index);


private:
    Ui::MajorSetting *ui;

    sDataPacket *mPacket;
    SettingThreshold *mSettingThroldWid;
    int mIndex; //当前主路源标志位

};

#endif // MAJORSETTING_H

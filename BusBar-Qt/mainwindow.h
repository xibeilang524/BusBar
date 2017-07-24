#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "initshm.h"
#include "homewid.h"
#include "linewid.h"
#include "branchwid.h"
#include "logswid.h"
#include "setparameters/settingwid.h"
//#include "frminput.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void initSerial();
    void initWidget();
    void initBackground();
    void checkAlarm();
    void setBusName(int index);
    void setButtonImage(QToolButton *button, QString name);
    void setButtonClickedImage(QToolButton *button, QString name);

protected slots:
    void initFunSLot();
    void timeoutDone();

private slots:
    void on_homeBtn_clicked();

    void on_lineBtn_clicked();

    void on_branchBtn_clicked();

    void on_logBtn_clicked();

    void on_setBtn_clicked();

    void on_alarmBtn_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    InitShm *mInitShm;
    QTimer *timer;

    int mIndex;
    HomeWid *mHomeWid;
    LineWid *mLineWid;
    BranchWid *mBranchWid;
    LogsWid *mLogsWid;
    SettingWid *mSettingWid;
};

#endif // MAINWINDOW_H

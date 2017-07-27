#include "boxdlg.h"
#include "ui_boxdlg.h"

BoxDlg::BoxDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BoxDlg)
{
    ui->setupUi(this);
//    set_background_icon(this,":/new/prefix1/image/dialog.png",QSize(815,400));
    this->setWindowFlags(Qt::WindowSystemMenuHint|Qt::WindowMinimizeButtonHint);
}

BoxDlg::~BoxDlg()
{
    delete ui;
}

void BoxDlg::initBox(int bus, int box)
{
    sDataPacket *shm = get_share_mem();
    mData = &(shm->data[bus].box[box]);

    QString name(mData->boxName);
    ui->titleLab->setText(name);

    initWid(bus, box);
}

void BoxDlg::initWid(int bus, int box)
{
    mTotalWid = new BoxTotalWid(ui->totalWid);
    mTotalWid->initFun(bus, box);

    mLine[0] = new BoxLoopWid(ui->line1Wid);
    mLine[1] = new BoxLoopWid(ui->line2Wid);
    mLine[2] = new BoxLoopWid(ui->line3Wid);

    for(int i=0; i<3; ++i) {
        BoxLoopWid *line  = mLine[i];
        line->initLine(bus, box, i);
    }
}

void BoxDlg::on_pushButton_clicked()
{
    this->close();
}

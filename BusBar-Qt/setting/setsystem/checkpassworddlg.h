#ifndef CHECKPASSWORDDLG_H
#define CHECKPASSWORDDLG_H

#include <QDialog>
#include "common.h"

namespace Ui {
class CheckPasswordDlg;
}

class CheckPasswordDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CheckPasswordDlg(QWidget *parent = 0);
    ~CheckPasswordDlg();

    bool getResult();

public slots:
    void accept();

private slots:
//    void on_pushButton_clicked();

private:
    Ui::CheckPasswordDlg *ui;

    bool mResult;
};

#endif // CHECKPASSWORDDLG_H
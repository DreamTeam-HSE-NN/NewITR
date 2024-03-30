#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NewITR.h"

class NewITR : public QMainWindow
{
    Q_OBJECT

private:
    Ui::NewITRClass* ui_;

public:
    NewITR(QWidget *parent = nullptr);
    ~NewITR();

public slots:
    void generatebtn_click_slot();
};

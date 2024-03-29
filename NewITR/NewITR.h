#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NewITR.h"

class NewITR : public QMainWindow
{
    Q_OBJECT

public:
    NewITR(QWidget *parent = nullptr);
    ~NewITR();

private:
    Ui::NewITRClass* ui_;
};

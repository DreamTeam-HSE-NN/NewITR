#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_NewITR.h"

class NewITR : public QMainWindow
{
    Q_OBJECT

private:
    Ui::NewITRClass* ui_;

public:
    explicit NewITR(QWidget *parent = nullptr);
    ~NewITR();

public slots:
    void GenerateBtnClick_Slot() const;
};

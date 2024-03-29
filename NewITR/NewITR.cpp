#include "NewITR.h"

// ===================== ctor =====================

NewITR::NewITR(QWidget *parent) : QMainWindow(parent)
{
    ui_ = new Ui::NewITRClass;
    ui_->setupUi(this);

    // Slot bind
    // QObject::connect(ui->pushButton_Draw, SIGNAL(clicked()), this, SLOT(event_PressDraw()));
}

NewITR::~NewITR()
{
    delete ui_;
}


// ===================== Events =====================

#include "NewITR.h"
#include "Types.h"
#include "CalcHelper.h"

// ===================== ctor =====================

NewITR::NewITR(QWidget *parent) : QMainWindow(parent)
{
    ui_ = new Ui::NewITRClass;
    ui_->setupUi(this);

    // Slot bind
    connect(ui_->generate_btn_, SIGNAL(pressed()), this, SLOT(GenerateBtnClick_Slot()));
    connect(ui_->graphic_widget_, SIGNAL(SendMsgToStatusBar(QString)), ui_->statusBar_, SLOT(showMessage(QString)));
}

NewITR::~NewITR()
{
    delete ui_;
}

// ===================== Slots =====================

void NewITR::GenerateBtnClick_Slot() const
{
    const auto res = CalcHelper::GenerateAndCalcPolygons(ui_->num_of_polygons_spinbox->value());
    ui_->graphic_widget_->SetFigures(res);
}



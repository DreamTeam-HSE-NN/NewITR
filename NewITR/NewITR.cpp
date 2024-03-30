#include "NewITR.h"
#include "Types.h"
#include "CalcHelper.h"

// ===================== ctor =====================

NewITR::NewITR(QWidget *parent) : QMainWindow(parent)
{
    ui_ = new Ui::NewITRClass;
    ui_->setupUi(this);

    // Slot bind
    connect(ui_->generate_btn_, SIGNAL(pressed()), this, SLOT(generatebtn_click_slot()));
}

NewITR::~NewITR()
{
    delete ui_;
}

// ===================== Slots =====================

void NewITR::generatebtn_click_slot()
{
    CalcHelper helper;
    auto res = helper.GenerateAndCalcPolygons(ui_->num_of_polygons_spinbox->value());
    ui_->graphic_widget_->SetFigures(res);
}



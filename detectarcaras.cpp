#include "detectarcaras.h"
#include "ui_detectarcaras.h"

DetectarCaras::DetectarCaras(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetectarCaras)
{
    ui->setupUi(this);
}

DetectarCaras::~DetectarCaras()
{
    delete ui;
}

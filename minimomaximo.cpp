#include "minimomaximo.h"
#include "ui_minimomaximo.h"

MinimoMaximo::MinimoMaximo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MinimoMaximo)
{
    ui->setupUi(this);
}

MinimoMaximo::~MinimoMaximo()
{
    delete ui;
}

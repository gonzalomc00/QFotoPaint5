#include "falsocolor.h"
#include "ui_falsocolor.h"
#include "imagenes.h"

falsocolor::falsocolor(int numfoto, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::falsocolor)
{
    ui->setupUi(this);
    nfoto=numfoto;
    color_falso=0;
    ver_color_falso(nfoto,color_falso);
}

falsocolor::~falsocolor()
{
    delete ui;
}

void falsocolor::on_horizontalSlider_valueChanged(int value)
{
    color_falso=value;
    ver_color_falso(nfoto,color_falso);
}

void falsocolor::on_falsocolor_accepted()
{
    ver_color_falso(nfoto,color_falso,true);
    destroyWindow("Aplicar falso color");
}

void falsocolor::on_falsocolor_rejected()
{
    destroyWindow("Aplicar falso color");
    destroyWindow("Resultado");
}

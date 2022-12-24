#include "modelos_color.h"
#include "ui_modelos_color.h"
#include "imagenes.h"

modelos_color::modelos_color(int numfoto,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modelos_color)
{
    ui->setupUi(this);
    nfoto = numfoto;
    formato=0;
    cambiar_modelo_color(nfoto, formato);

}

modelos_color::~modelos_color()
{
    delete ui;
}

void modelos_color::on_radioButton_5_clicked()
{
    formato = 0;
    cambiar_modelo_color(nfoto, formato);

}

void modelos_color::on_modelos_color_accepted()
{
    cambiar_modelo_color(nfoto, formato, true);
    destroyWindow("Modelo Color");

}

void modelos_color::on_radioButton_2_clicked()
{
    formato = 1;
    cambiar_modelo_color(nfoto, formato);
}

void modelos_color::on_modelos_color_rejected()
{
    destroyWindow("Modelo Color");

}

void modelos_color::on_radioButton_3_clicked()
{
    formato = 2;
    cambiar_modelo_color(nfoto, formato);
}

void modelos_color::on_radioButton_4_clicked()
{
    formato = 3;
    cambiar_modelo_color(nfoto, formato);
}

void modelos_color::on_radioButton_6_clicked()
{
    formato = 4;
    cambiar_modelo_color(nfoto, formato);
}

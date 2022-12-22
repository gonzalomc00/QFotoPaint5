#include "modelos_color.h"
#include "ui_modelos_color.h"
#include "imagenes.h"

modelos_color::modelos_color(int foto,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modelos_color)
{
    ui->setupUi(this);
    nfoto = foto;
}

modelos_color::~modelos_color()
{
    delete ui;
}

void modelos_color::on_radioButton_5_clicked()
{
    tipo = 0;
    cambiar_modelo_color(nfoto, tipo);

}

void modelos_color::on_modelos_color_accepted()
{
    cambiar_modelo_color(nfoto, tipo, true);
    destroyWindow("Modelo Color");

}

void modelos_color::on_radioButton_2_clicked()
{
    tipo = 1;
    cambiar_modelo_color(nfoto, tipo);
}

void modelos_color::on_modelos_color_rejected()
{
    destroyWindow("Modelo Color");

}

void modelos_color::on_radioButton_3_clicked()
{
    tipo = 2;
    cambiar_modelo_color(nfoto, tipo);
}

void modelos_color::on_radioButton_4_clicked()
{
    tipo = 3;
    cambiar_modelo_color(nfoto, tipo);
}

void modelos_color::on_radioButton_6_clicked()
{
    tipo = 4;
    cambiar_modelo_color(nfoto, tipo);
}

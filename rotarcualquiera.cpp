#include "rotarcualquiera.h"
#include "ui_rotarcualquiera.h"
#include "imagenes.h"

rotarcualquiera::rotarcualquiera(int foto, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rotarcualquiera)
{
    ui->setupUi(this);
    nfoto= foto;
    angulo=0;
    escala = 1.0;
    ver_rotar_cualquiera(nfoto,angulo,escala);

}

rotarcualquiera::~rotarcualquiera()
{
    delete ui;
}


void rotarcualquiera::on_dial_valueChanged(int value)
{
    angulo = value;
    ver_rotar_cualquiera(nfoto,angulo,escala);

}

void rotarcualquiera::on_pushButton_clicked()
{
   ver_rotar_cualquiera(nfoto,angulo,escala,true);

}

void rotarcualquiera::on_pushButton_2_clicked()
{
    destroyWindow("Rotacion");

}


void rotarcualquiera::on_doubleSpinBox_valueChanged(double arg1)
{
    escala = arg1;
    ver_rotar_cualquiera(nfoto,angulo,escala);
}

void rotarcualquiera::on_rotarcualquiera_accepted()
{
 ver_rotar_cualquiera(nfoto,angulo,escala,true);
}

void rotarcualquiera::on_rotarcualquiera_rejected()
{
    destroyWindow("Rotacion");

}



#include "bajorrelieve.h"
#include "ui_bajorrelieve.h"
#include "imagenes.h"

Bajorrelieve::Bajorrelieve(int numfoto, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Bajorrelieve)
{
    ui->setupUi(this);
    nfoto= numfoto;
    fondo=0;
    grado=1.0;
    angulo=0;
    ver_bajorrelieve(nfoto,angulo,grado,fondo);
}

Bajorrelieve::~Bajorrelieve()
{
    delete ui;
}

void Bajorrelieve::on_horizontalSlider_valueChanged(int value)
{
    grado=value/100.0;
    ver_bajorrelieve(nfoto,angulo,grado,fondo);

}

void Bajorrelieve::on_dial_valueChanged(int value)
{
    angulo=value;
    ver_bajorrelieve(nfoto,angulo,grado,fondo);

}

void Bajorrelieve::on_radioButton_clicked()
{
    fondo=0;
    ver_bajorrelieve(nfoto,angulo,grado,fondo);
}

void Bajorrelieve::on_radioButton_2_clicked()
{
    fondo=1;
    ver_bajorrelieve(nfoto,angulo,grado,fondo);
}

void Bajorrelieve::on_radioButton_3_clicked()
{
    fondo=2;
    ver_bajorrelieve(nfoto,angulo,grado,fondo);
}

void Bajorrelieve::on_radioButton_4_clicked()
{
    fondo=3;
    ver_bajorrelieve(nfoto,angulo,grado,fondo);
}

void Bajorrelieve::on_Bajorrelieve_accepted()
{
    ver_bajorrelieve(nfoto,angulo,grado,fondo,true);
    destroyWindow("Bajorrelieve");


}

void Bajorrelieve::on_Bajorrelieve_rejected()
{
    destroyWindow("Bajorrelieve");
}

void Bajorrelieve::on_pushButton_clicked()
{
        ver_bajorrelieve(nfoto,angulo,grado,fondo,true);
        destroyWindow("Bajorrelieve");

}

void Bajorrelieve::on_pushButton_2_clicked()
{
     destroyWindow("Bajorrelieve");
}


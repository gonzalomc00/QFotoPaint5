#include "rojoverdeazul.h"
#include "ui_rojoverdeazul.h"
#include "imagenes.h"

RojoVerdeAzul::RojoVerdeAzul(int nfoto,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RojoVerdeAzul)
{
    ui->setupUi(this);
    this->nfoto=nfoto;
    RGB_sum[0]=0;
    RGB_sum[1]=0;
    RGB_sum[2]=0;

    RGB_mult[0]=1;
    RGB_mult[1]=1;
    RGB_mult[2]=1;

    ver_rojo_verde_azul(nfoto,RGB_mult,RGB_sum);


}

RojoVerdeAzul::~RojoVerdeAzul()
{
    delete ui;
    destroyWindow("Ajuste RGB");
}

void RojoVerdeAzul::on_horizontalSlider_valueChanged(int value)
{
    RGB_sum[0]=value;
    ver_rojo_verde_azul(nfoto,RGB_mult,RGB_sum);
    ui->spinBox->setValue(value);
}

void RojoVerdeAzul::on_horizontalSlider_2_valueChanged(int value)
{
    RGB_sum[1]=value;
    ver_rojo_verde_azul(nfoto,RGB_mult,RGB_sum);
    ui->spinBox_2->setValue(value);

}

void RojoVerdeAzul::on_horizontalSlider_3_valueChanged(int value)
{

    RGB_sum[2]=value;
    ver_rojo_verde_azul(nfoto,RGB_mult,RGB_sum);
    ui->spinBox_3->setValue(value);

}

void RojoVerdeAzul::on_horizontalSlider_4_valueChanged(int value)
{

    RGB_mult[0]=value/100.0;
    ver_rojo_verde_azul(nfoto,RGB_mult,RGB_sum);
    ui->spinBox_5->setValue(value);

}

void RojoVerdeAzul::on_horizontalSlider_6_valueChanged(int value)
{
    RGB_mult[1]=value/100.0;
    ver_rojo_verde_azul(nfoto,RGB_mult,RGB_sum);
    ui->spinBox_4->setValue(value);

}

void RojoVerdeAzul::on_horizontalSlider_5_valueChanged(int value)
{
    RGB_mult[2]=value/100.0;
    ver_rojo_verde_azul(nfoto,RGB_mult,RGB_sum);
    ui->spinBox_6->setValue(value);

}

void RojoVerdeAzul::on_spinBox_valueChanged(int arg1)
{
        ui->horizontalSlider->setValue(arg1);
}

void RojoVerdeAzul::on_spinBox_2_valueChanged(int arg1)
{
    ui->horizontalSlider_2->setValue(arg1);
}

void RojoVerdeAzul::on_spinBox_3_valueChanged(int arg1)
{
    ui->horizontalSlider_3->setValue(arg1);

}

void RojoVerdeAzul::on_spinBox_5_valueChanged(int arg1)
{
    ui->horizontalSlider_4->setValue(arg1);

}

void RojoVerdeAzul::on_spinBox_4_valueChanged(int arg1)
{
    ui->horizontalSlider_6->setValue(arg1);

}

void RojoVerdeAzul::on_spinBox_6_valueChanged(int arg1)
{
    ui->horizontalSlider_5->setValue(arg1);

}

void RojoVerdeAzul::on_RojoVerdeAzul_accepted()
{
    ver_rojo_verde_azul(nfoto,RGB_mult,RGB_sum,true);
}

void RojoVerdeAzul::on_RojoVerdeAzul_rejected()
{
    destroyWindow("Resultado RGB");
}

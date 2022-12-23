#include "histogramabidimensional.h"
#include "ui_histogramabidimensional.h"

#include "imagenes.h"

histogramabidimensional::histogramabidimensional(int numfoto, int numres, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::histogramabidimensional)
{
    ui->setupUi(this);
    nfoto = numfoto;
    res = numres;
    tipo = 0;
    celdas = 64;

    ui->spinBox->setValue(celdas);
    ui->horizontalSlider->setValue(celdas);


    ver_histograma_bidimensional(nfoto, res, tipo, celdas);

}

histogramabidimensional::~histogramabidimensional()
{
    delete ui;
}

void histogramabidimensional::on_radioButton_4_clicked()
{ //R-G
    tipo = 0;
    ver_histograma_bidimensional(nfoto, res, tipo, celdas);
}

void histogramabidimensional::on_radioButton_2_clicked()
{
    tipo = 1;
    ver_histograma_bidimensional(nfoto, res, tipo, celdas);
}

void histogramabidimensional::on_radioButton_3_clicked()
{
    tipo = 2;
    ver_histograma_bidimensional(nfoto,res,tipo,celdas);
}


void histogramabidimensional::on_horizontalSlider_valueChanged(int value)
{
    celdas = value;
    ui->spinBox->setValue(value);
    ver_histograma_bidimensional(nfoto,res,tipo,celdas);

}

void histogramabidimensional::on_spinBox_valueChanged(int arg1)
{
    celdas = arg1;
    ui->horizontalSlider->setValue(arg1);
    ver_histograma_bidimensional(nfoto,res,tipo,celdas);
}

void histogramabidimensional::on_histogramabidimensional_accepted()
{
    ver_histograma_bidimensional(nfoto,res,tipo,celdas, true);
    destroyWindow("Histograma Bidimensional");

}

void histogramabidimensional::on_histogramabidimensional_rejected()
{
    destroyWindow("Histograma Bidimensional");

}

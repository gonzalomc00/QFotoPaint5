#include "ecualizarhistograma.h"
#include "ui_ecualizarhistograma.h"
#include "imagenes.h"

EcualizarHistograma::EcualizarHistograma(int numfoto,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EcualizarHistograma)
{
    ui->setupUi(this);
    nfoto=numfoto;
    modo=0;
    set_callback_foto(nfoto,false);
    ver_ecualizacion_histograma(nfoto,modo,false);
}

EcualizarHistograma::~EcualizarHistograma()
{
    set_callback_foto(nfoto,true);
    delete ui;
}

void EcualizarHistograma::on_radioButton_clicked()
{
    modo=0;
    ver_ecualizacion_histograma(nfoto,modo,false);
}



void EcualizarHistograma::on_radioButton_2_clicked()
{
    modo=1;
    ver_ecualizacion_histograma(nfoto,modo,false);
}

void EcualizarHistograma::on_EcualizarHistograma_accepted()
{
    ver_ecualizacion_histograma(nfoto,modo,true);
}

void EcualizarHistograma::on_EcualizarHistograma_rejected()
{
    mostrar(nfoto);
}

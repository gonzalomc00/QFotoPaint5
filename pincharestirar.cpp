#include "pincharestirar.h"
#include "ui_pincharestirar.h"
#include "imagenes.h"

//como vamos a necesitar hacer uso de un callback no se definen los parametros estos como atributos.
int nfoto;
int cx,cy;
double radio_efecto,grado_efecto;


void cb_pincharestirar(int evento, int x, int y, int flags, void *pt)
{
    if(flags==EVENT_FLAG_LBUTTON){
        cx=x;
        cy=y;
        ver_pinchar_estirar(nfoto,cx,cy,radio_efecto,grado_efecto);
    }
}

PincharEstirar::PincharEstirar(int numfoto,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PincharEstirar)
{
    ui->setupUi(this);
    nfoto= numfoto;
    cx=200;
    cy=200;
    radio_efecto=ui->horizontalSlider->value();
    grado_efecto=radio_efecto*radio_efecto*ui->horizontalSlider_2->value()/1000.0;
    ver_pinchar_estirar(nfoto,cx,cy,radio_efecto,grado_efecto);
    //establecemos que el callback esté en esta ventana
    setMouseCallback("Pinchar/estirar",cb_pincharestirar);
}

PincharEstirar::~PincharEstirar()
{
    delete ui;
}

void PincharEstirar::on_horizontalSlider_valueChanged(int value)
{
    radio_efecto=value;
    grado_efecto=radio_efecto*radio_efecto*ui->horizontalSlider_2->value()/1000.0;
    ui->spinBox->setValue(value);
    ver_pinchar_estirar(nfoto,cx,cy,radio_efecto,grado_efecto);

}

void PincharEstirar::on_spinBox_valueChanged(int arg1)
{
    ui->horizontalSlider->setValue(arg1);
}

void PincharEstirar::on_horizontalSlider_2_valueChanged(int value)
{
    grado_efecto=radio_efecto*radio_efecto*value/1000.0;
    ui->spinBox_2->setValue(value);
    ver_pinchar_estirar(nfoto,cx,cy,radio_efecto,grado_efecto);
}

void PincharEstirar::on_spinBox_2_valueChanged(int arg1)
{
    ui->horizontalSlider_2->setValue(arg1);
}

void PincharEstirar::on_PincharEstirar_accepted()
{
     ver_pinchar_estirar(nfoto,cx,cy,radio_efecto,grado_efecto,true);
     destroyWindow("Pinchar/estirar");
}

void PincharEstirar::on_PincharEstirar_rejected()
{
    destroyWindow("Pinchar/estirar");
}

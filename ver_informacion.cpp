#include "ver_informacion.h"
#include "ui_ver_informacion.h"

#include "imagenes.h"

ver_informacion::ver_informacion(int foto,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ver_informacion)
{
    ui->setupUi(this);
    nfoto = foto;

    //TAMAÑO
    ui->tamanio->setText(ver_informacion_imagen(nfoto,0));
    //CANALES
    ui->canales->setText(ver_informacion_imagen(nfoto,1));
    //MEMORIA OCUPADA
    ui->mem->setText(ver_informacion_imagen(nfoto,2));
    //RATIO
    ui->ratio->setText(ver_informacion_imagen(nfoto,4));
    //PROFUNDIDAD
    ui->profundidad->setText(ver_informacion_imagen(nfoto,3));
    //Estilo RGB
    ui->colorRGB ->setStyleSheet(ver_informacion_imagen(nfoto,6));
    ui->rgbcolor->setText(ver_informacion_imagen(nfoto,6));
    //Color medio de la imagen
    ui->colormedio->setText(ver_informacion_imagen(nfoto,5));


}

ver_informacion::~ver_informacion()
{
    delete ui;
}

void ver_informacion::on_ver_informacion_accepted()
{
    close();
}

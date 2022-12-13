#include "movimiento.h"
#include "ui_movimiento.h"

Movimiento::Movimiento(QString nombre, int numres,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Movimiento)
{
    ui->setupUi(this);
    if(cap.open(nombre.toLatin1().data())){
        int nframes=cap.get(CAP_PROP_FRAME_COUNT);
        ui->horizontalSlider->setMaximum(nframes-2);
        ui->horizontalSlider_2->setMaximum(nframes-1);
        ui->horizontalSlider->setValue(0);
        ui->horizontalSlider_2->setValue(nframes-1);
        framei=0;
        framef=nframes-1;
        nres=numres;
        nombre_fichero=nombre;
    }
}

Movimiento::~Movimiento()
{
    delete ui;
}

void Movimiento::on_horizontalSlider_valueChanged(int value)
{
    framei=value;
    if(framei>=framef){
        ui->horizontalSlider_2->setValue(framei+1);
    }

    Mat frame;
    cap.set(CAP_PROP_POS_FRAMES,framei);
    if(cap.read(frame))
        imshow("Video",frame);
}

void Movimiento::on_horizontalSlider_2_valueChanged(int value)
{
    framef=value;
    if(framei>=framef){
        ui->horizontalSlider->setValue(framef-1);
    cap.set(CAP_PROP_POS_FRAMES,framei);
    Mat frame;
    if(cap.read(frame))
    imshow("Video",frame);
    }
}

void Movimiento::on_Movimiento_accepted()
{
    movimiento(nombre_fichero.toLatin1().data(),framei,framef,nres);
}

void Movimiento::on_Movimiento_rejected()
{
    destroyWindow("Video");
}

bool Movimiento::isOpened(){
    return cap.isOpened();
}

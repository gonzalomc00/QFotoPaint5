#include "minimomaximo.h"
#include "ui_minimomaximo.h"

MinimoMaximo::MinimoMaximo(QString nombre, int numres,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MinimoMaximo)
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

MinimoMaximo::~MinimoMaximo()
{
    delete ui;
}

void MinimoMaximo::on_horizontalSlider_valueChanged(int value)
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

void MinimoMaximo::on_horizontalSlider_2_valueChanged(int value)
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

void MinimoMaximo::on_MinimoMaximo_accepted()
{
    minima_maxima(nombre_fichero.toLatin1().data(),framei,framef);

}

void MinimoMaximo::on_MinimoMaximo_rejected()
{
    destroyWindow("Video");

}

bool MinimoMaximo::isOpened(){
    return cap.isOpened();
}

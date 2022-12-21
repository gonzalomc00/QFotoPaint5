#include "detectarcarasvideo.h"
#include "ui_detectarcarasvideo.h"
#include "video.h"

#include <QFileDialog>

DetectarCarasVideo::DetectarCarasVideo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetectarCarasVideo)
{
    ui->setupUi(this);
}

DetectarCarasVideo::~DetectarCarasVideo()
{
    delete ui;
}

void DetectarCarasVideo::on_pushButton_2_clicked()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ver_caras_video("cam");
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
}

void DetectarCarasVideo::on_pushButton_clicked()
{
    QString nombre=QFileDialog::getOpenFileName();
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    if(!nombre.isEmpty()){
    ver_caras_video(nombre.toLatin1().data());
    }
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
}

void DetectarCarasVideo::on_pushButton_3_clicked()
{
    close();
}

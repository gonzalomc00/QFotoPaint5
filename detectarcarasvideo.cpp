#include "detectarcarasvideo.h"
#include "ui_detectarcarasvideo.h"

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

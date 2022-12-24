#include "detectarcaras.h"
#include "ui_detectarcaras.h"
#include <QFileDialog>
#include "video.h"


DetectarCaras::DetectarCaras(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetectarCaras)
{
    ui->setupUi(this);
    guardar=false;
}

DetectarCaras::~DetectarCaras()
{
    delete ui;
}

void DetectarCaras::on_pushButton_clicked()
{
    nombre_fichero=QFileDialog::getOpenFileName();
    ui->label_2->setText(nombre_fichero);
    QApplication::processEvents();
}

void DetectarCaras::on_checkBox_stateChanged(int arg1)
{
    guardar=arg1;
}

void DetectarCaras::on_pushButton_3_clicked()
{
    if(!nombre_fichero.isEmpty()){
        ui->label_2->setText("El archivo se está generando, espera un momento");
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        QApplication::processEvents();
        ver_caras(nombre_fichero.toLatin1().data(),guardar);
        ui->label_2->setText("El archivo se ha generado con éxito");
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        QApplication::processEvents();
    }

}

void DetectarCaras::on_pushButton_2_clicked()
{
    close();
}

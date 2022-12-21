#include "ecualizarhistograma.h"
#include "ui_ecualizarhistograma.h"

EcualizarHistograma::EcualizarHistograma(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EcualizarHistograma)
{
    ui->setupUi(this);
}

EcualizarHistograma::~EcualizarHistograma()
{
    delete ui;
}

#ifndef MINIMOMAXIMO_H
#define MINIMOMAXIMO_H

#include <QDialog>
#include "video.h"


namespace Ui {
class MinimoMaximo;
}

class MinimoMaximo : public QDialog
{
    Q_OBJECT

public:
    explicit MinimoMaximo(QString nombre, int numres,QWidget *parent = nullptr);

    bool isOpened();

    ~MinimoMaximo();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_MinimoMaximo_accepted();

    void on_MinimoMaximo_rejected();



private:
    Ui::MinimoMaximo *ui;
    VideoCapture cap;
    int framei,framef;
    int nres;
    QString nombre_fichero;
};

#endif // MINIMOMAXIMO_H

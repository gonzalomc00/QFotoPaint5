#ifndef BAJORRELIEVE_H
#define BAJORRELIEVE_H

#include <QDialog>

namespace Ui {
class Bajorrelieve;
}

class Bajorrelieve : public QDialog
{
    Q_OBJECT

public:
    explicit Bajorrelieve(int numfoto, QWidget *parent = nullptr);

    ~Bajorrelieve();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_dial_valueChanged(int value);

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_Bajorrelieve_accepted();

    void on_Bajorrelieve_rejected();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


private:
    Ui::Bajorrelieve *ui;
    int fondo; //double ??
    int nfoto;
    double grado;
    double angulo;


};

#endif // BAJORRELIEVE_H

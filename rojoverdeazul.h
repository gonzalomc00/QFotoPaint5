#ifndef ROJOVERDEAZUL_H
#define ROJOVERDEAZUL_H

#include <QDialog>

namespace Ui {
class RojoVerdeAzul;
}

class RojoVerdeAzul : public QDialog
{
    Q_OBJECT

public:
    explicit RojoVerdeAzul(int nfoto,QWidget *parent = nullptr);
    ~RojoVerdeAzul();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

    void on_horizontalSlider_4_valueChanged(int value);

    void on_horizontalSlider_6_valueChanged(int value);

    void on_horizontalSlider_5_valueChanged(int value);

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void on_spinBox_3_valueChanged(int arg1);

    void on_spinBox_5_valueChanged(int arg1);

    void on_spinBox_4_valueChanged(int arg1);

    void on_spinBox_6_valueChanged(int arg1);

    void on_RojoVerdeAzul_accepted();

    void on_RojoVerdeAzul_rejected();

private:
    Ui::RojoVerdeAzul *ui;
    int nfoto;
    double RGB_sum[3];
    double RGB_mult[3];

};

#endif // ROJOVERDEAZUL_H

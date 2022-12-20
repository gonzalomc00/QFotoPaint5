#ifndef ROTARCUALQUIERA_H
#define ROTARCUALQUIERA_H

#include <QDialog>

namespace Ui {
class rotarcualquiera;
}

class rotarcualquiera : public QDialog
{
    Q_OBJECT

public:
    explicit rotarcualquiera(int foto, QWidget *parent = nullptr);
    ~rotarcualquiera();

private slots:
    void on_rotarcualquiera_accepted();

    void on_rotarcualquiera_rejected();

    void on_dial_valueChanged(int value);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_spinBox_valueChanged(int arg1);

    void on_doubleSpinBox_valueChanged(double arg1);

private:
    Ui::rotarcualquiera *ui;
    int nfoto;
    int angulo;
    double escala;
};

#endif // ROTARCUALQUIERA_H


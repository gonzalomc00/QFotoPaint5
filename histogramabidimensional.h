#ifndef HISTOGRAMABIDIMENSIONAL_H
#define HISTOGRAMABIDIMENSIONAL_H

#include <QDialog>

namespace Ui {
class histogramabidimensional;
}

class histogramabidimensional : public QDialog
{
    Q_OBJECT

public:
    explicit histogramabidimensional(int numfoto, int numres, QWidget *parent = nullptr);
    ~histogramabidimensional();

private slots:
    void on_radioButton_4_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_spinBox_valueChanged(int arg1);

    void on_histogramabidimensional_accepted();

    void on_histogramabidimensional_rejected();

private:
    Ui::histogramabidimensional *ui;
    int nfoto;
    int res;
    int tipo;
    int celdas;
};

#endif // HISTOGRAMABIDIMENSIONAL_H

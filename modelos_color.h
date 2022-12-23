#ifndef MODELOS_COLOR_H
#define MODELOS_COLOR_H

#include <QDialog>

namespace Ui {
class modelos_color;
}

class modelos_color : public QDialog
{
    Q_OBJECT

public:
    explicit modelos_color(int numfoto, QWidget *parent = nullptr);
    ~modelos_color();

private slots:
    void on_radioButton_5_clicked();

    void on_modelos_color_accepted();

    void on_radioButton_2_clicked();

    void on_modelos_color_rejected();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_6_clicked();

private:
    Ui::modelos_color *ui;
    int nfoto;
    int tipo;
};

#endif // MODELOS_COLOR_H

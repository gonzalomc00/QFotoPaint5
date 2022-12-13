#ifndef FALSOCOLOR_H
#define FALSOCOLOR_H

#include <QDialog>

namespace Ui {
class falsocolor;
}

class falsocolor : public QDialog
{
    Q_OBJECT

public:
    explicit falsocolor(int numfoto,QWidget *parent = nullptr);
    ~falsocolor();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_falsocolor_accepted();

    void on_falsocolor_rejected();


private:
    Ui::falsocolor *ui;
    int nfoto;
    int color_falso;
};

#endif // FALSOCOLOR_H

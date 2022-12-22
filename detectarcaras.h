#ifndef DETECTARCARAS_H
#define DETECTARCARAS_H

#include <QDialog>

namespace Ui {
class DetectarCaras;
}

class DetectarCaras : public QDialog
{
    Q_OBJECT

public:
    explicit DetectarCaras(QWidget *parent = nullptr);
    ~DetectarCaras();

private slots:
    void on_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DetectarCaras *ui;
    bool guardar;
    QString nombre_fichero;
};

#endif // DETECTARCARAS_H

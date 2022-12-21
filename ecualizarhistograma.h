#ifndef ECUALIZARHISTOGRAMA_H
#define ECUALIZARHISTOGRAMA_H

#include <QDialog>

namespace Ui {
class EcualizarHistograma;
}

class EcualizarHistograma : public QDialog
{
    Q_OBJECT

public:
    explicit EcualizarHistograma(int numfoto, QWidget *parent = nullptr);
    ~EcualizarHistograma();

private slots:
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_EcualizarHistograma_accepted();

    void on_EcualizarHistograma_rejected();

private:
    Ui::EcualizarHistograma *ui;
    int nfoto;
    int modo;
};

#endif // ECUALIZARHISTOGRAMA_H

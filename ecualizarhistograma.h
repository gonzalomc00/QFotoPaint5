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
    explicit EcualizarHistograma(QWidget *parent = nullptr);
    ~EcualizarHistograma();

private:
    Ui::EcualizarHistograma *ui;
};

#endif // ECUALIZARHISTOGRAMA_H

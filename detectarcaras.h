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

private:
    Ui::DetectarCaras *ui;
};

#endif // DETECTARCARAS_H

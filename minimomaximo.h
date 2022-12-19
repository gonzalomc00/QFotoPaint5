#ifndef MINIMOMAXIMO_H
#define MINIMOMAXIMO_H

#include <QDialog>

namespace Ui {
class MinimoMaximo;
}

class MinimoMaximo : public QDialog
{
    Q_OBJECT

public:
    explicit MinimoMaximo(QWidget *parent = nullptr);
    ~MinimoMaximo();

private:
    Ui::MinimoMaximo *ui;
};

#endif // MINIMOMAXIMO_H

#ifndef VER_INFORMACION_H
#define VER_INFORMACION_H

#include <QDialog>


namespace Ui {
class ver_informacion;
}

class ver_informacion : public QDialog
{
    Q_OBJECT

public:
    explicit ver_informacion(int foto, QWidget *parent = nullptr);
    ~ver_informacion();

private slots:
    void on_ver_informacion_accepted();

private:
    Ui::ver_informacion *ui;
    int nfoto;
    int tipo;

};

#endif // VER_INFORMACION_H

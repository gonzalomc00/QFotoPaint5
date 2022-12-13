#include "mainwindow.h"
#include <QApplication>


MainWindow *w= nullptr;
QApplication *a= nullptr;

int main(int argc, char *argv[])
{
    //Se han hecho modificaciones para que se pueda acceder a QAplication y MainWindow desde fuera.
    a= new QApplication(argc, argv);
    w= new MainWindow();
    w->show();
    int res= a->exec();
    delete w;
    delete a;
    return res;
}

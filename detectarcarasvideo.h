#ifndef DETECTARCARASVIDEO_H
#define DETECTARCARASVIDEO_H

#include <QDialog>

namespace Ui {
class DetectarCarasVideo;
}

class DetectarCarasVideo : public QDialog
{
    Q_OBJECT

public:
    explicit DetectarCarasVideo(QWidget *parent = nullptr);
    ~DetectarCarasVideo();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::DetectarCarasVideo *ui;
};

#endif // DETECTARCARASVIDEO_H

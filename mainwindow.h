#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "dialog.h"

#include <QTime>

#include <itercore.h>

#define Color int

#define NEWQCP

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void on_pushButton_clicked(QString f, QString g, double min, double max, double b_min, double b_max, QString constText,\
                               QString color, bool dynamic, double _h,int num);
    void on_actionNew_triggered();
    void on_timer_event();

    void on_actionClear_Grapg_triggered();

    void get_color_fstring(QString color, Color rgb[3]);

private:
    Ui::MainWindow *ui;
    double range_min[2]; // 0 - x, 1 - y
    double range_max[2];
    QVector<double> x, y, _x, _y; //Массивы координат точек
#ifndef NEWQCP
    QCPCurve *curve;
#else
    QVector<QCPCurve*> mycurve;
#endif
    int curr_pos, pre_pos;
    QTimer *timer;
    int iter_count;

    double h;

    void iteract(IterCore *core, double a, double b, double c, double d, QVector<double> &x, QVector<double> &y, int DEBUG);
    double get_dl( double fx, double fy, double gx, double gy);
};

#endif // MAINWINDOW_H

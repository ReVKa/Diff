#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "dialog.h"

#include <QTime>
#include <QAction>
#include <QSettings>

#include <itercore.h>

#define Color int

#define NEWQCP

#define APP_NAME "ProgaDiff"
#define APP_ORG "RevCrew"
#define APP_DOMAIN "http://progadif.cs-suite.ru"

const QString settings_save_theme = "settings/theme";
const QString settings_save_step_combine = "settings/step_combine";
const QString settings_save_opt = "settings/opt";
const QString settings_save_info = "settings/infotable";

struct Vectors
{
    QVector<double> vec_x;
    QVector<double> vec_y;
    double min;
    double max;
    double b_min;
    double b_max;
    IterCore* core;
    bool dynamic;
    double h;
    int num;
    int rgb[3];
    QAction * action;
    int curve;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void SetDefaultTheme();
    void SetDarkTheme();

public slots:
    void on_pushButton_clicked(QString f, QString g, double min, double max, double b_min, double b_max, QString constText,\
                               QString color, bool dynamic, double _h,int num);
    void on_actionNew_triggered();
    void on_timer_event();

    void on_actionClear_Grapg_triggered();
    void SaveProgress (QString filename);

    void get_color_fstring(QString color, Color rgb[3]);

private slots:
    void on_actionSave_triggered();
    void on_actionIteration_triggered();
    void on_actionCombine_triggered();
    void on_actionOpen_triggered();

    void on_actionRelease_triggered();

    void on_actionPreference_triggered();

private:
    Ui::MainWindow *ui;
    double range_min[2]; // 0 - x, 1 - y
    double range_max[2];
    QVector<double> x, y, _x, _y; //Массивы координат точек
#ifndef NEWQCP
    QCPCurve *curve;
#else
    QVector<QCPCurve*> mycurve;
    QVector< Vectors > myvector;

    int curr_vector;
#endif
    int curr_pos, pre_pos;
    QTimer *timer;
    int iter_count;

    double h;
    bool work = false;
    bool first_add = false;
    QString version = "0.0.1";

    void iteract(IterCore *core, double a, double b, double c, double d, QVector<double> &x, QVector<double> &y, int DEBUG);
    double get_dl( double fx, double fy, double gx, double gy);

    void PlotGraph(int this_curve, int rgb[], bool dynamic, QVector<double> x, QVector<double> y);
    void TakeIteration(IterCore * core, double min, double max, double b_min, double b_max, double _h, QVector<double> &x, QVector<double> &y, int step, int finish);
};

#endif // MAINWINDOW_H

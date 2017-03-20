#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"

#include <QMessageBox>
#include <QProgressDialog>
#include <QTimer>

#define OPTIMIZE

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(ui->widget);
    curve = new QCPCurve(ui->widget->xAxis, ui->widget->yAxis);
    curve->setPen(QPen(Qt::darkGreen, 3, Qt::SolidLine));
    ui->widget->addPlottable(curve);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timer_event()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_actionNew_triggered()
{
    Dialog *wnd = new Dialog(this);
    QObject::connect(wnd,SIGNAL(OnOkClicked(QString, QString, double, double, double, double, QString , int)),\
                     this, SLOT(on_pushButton_clicked(QString, QString, double, double, double, double, QString , int)));
    wnd->show();
}

#define TYPE_X 0
#define TYPE_Y 1

double get_function( int type,double x, double y)
{
    switch (type)
    {
    //case TYPE_X: return x;//((double)(x+y+0.4*x-0.4*pow(x,3)));
    //case TYPE_Y: return x;//((double)y+0.4*x-0.4*pow(x,3));
    case TYPE_X: return ((double)(x+y+0.4*x-0.4*pow(x,3)));
    case TYPE_Y: return ((double)y+0.4*x-0.4*pow(x,3));
    }

    return -1;
}
double MainWindow::get_dl( double fx, double fy, double gx, double gy)
{
    iter_count ++;
    return sqrt( pow((gx-fx),2) + pow((gy-fy),2) );
}
double get_midl(double f,double g)
{
    return (g+f)/2;
}
void add_to_image(double x,double y,\
                  double r_min_x, double r_min_y, double r_max_x, double r_max_y, QVector<double> &_x, QVector<double> &_y )
{
    if( x < r_min_x || x > r_max_x)
        return;

    if( y < r_min_y || y > r_max_y)
        return;

    _x.push_back(x);
    _y.push_back(y);
}
void MainWindow::iteract(double a, double b, double c, double d, QVector<double> &x, QVector<double> &y, int DEBUG)
{
    double start_x = a, start_y = b, end_x = c, end_y = d;

    double a_x = get_function(TYPE_X, start_x, start_y);
    double a_y = get_function(TYPE_Y, start_x, start_y);

    double b_x = get_function(TYPE_X, end_x, end_y);
    double b_y = get_function(TYPE_Y, end_x, end_y);

    double dl = get_dl(a_x, a_y, b_x, b_y);
    add_to_image( a_x, a_y, range_min[0],range_min[1],range_max[0], range_max[1], x, y);

    double c_x = a_x;
    double c_y = a_y;

    double h_x = 0.0;
    double h_y = 0.0;

    double t_x, t_y;
    QVector<double> vec;

    while( dl > h) // пока длина больше H
    {
        t_x = end_x;// Присваем координаты конца отрезка
        t_y = end_y;// Присваем координаты конца отрезка
        while ( dl > h ) // пока длина больше H
        {

            t_x = get_midl(start_x, t_x); // Получаем середину
            t_y = get_midl(start_y, t_y);// Получаем середину
            h_x = get_function(TYPE_X,t_x, t_y);// Отображаем середину
            h_y = get_function(TYPE_Y,t_x, t_y); // Отображаем середину
            dl = get_dl(c_x, c_y, h_x, h_y);// Получаем длина между серединой и отображением A
        }
        // Длина меньше H, рисуем точку
        add_to_image( h_x, h_y, range_min[0],range_min[1],range_max[0], range_max[1], x, y);

        start_x = t_x;
        start_y = t_y;

        c_x = h_x;
        c_y = h_y;
        //Получаем длина между A и B
        dl = get_dl(h_x, h_y, b_x, b_y);

        if(DEBUG)
            QMessageBox::information(this,"title2","dl:"+QString::number(dl));
    }

    // ДОбавляем последнюю точку на рисунок
    add_to_image( b_x, b_y, range_min[0],range_min[1],range_max[0], range_max[1], x, y);
}
void MainWindow::on_timer_event()
{
    int len = x.length();
    for(int i = pre_pos; i<curr_pos+1; i++)
    {
        if(i>=len)
        {
            timer->stop();
            break;
        }

        curve->addData(x[i], y[i]);
    }
    ui->widget->replot();

    pre_pos = curr_pos;
    curr_pos+=x.length()/10;
}

void MainWindow::on_pushButton_clicked(QString f, QString g, double min, double max,\
                                       double b_min, double b_max, QString constText,int num)
{
     curve->clearData();

     h = 0.1;

     QTime *time = new QTime();

     double a = min;
     double b =  max;


     qDebug() << a << " " << b << " ";
     x.clear();
     y.clear();
     _x.clear();
     _y.clear();

     range_min[0] = b_min;
     range_min[1] = b_min;
     range_max[0] = b_max;
     range_max[1] = b_max;

    // qDebug() << "Debug1";
     //IterCore core( this, h, f, g, constText);

     if(num == -1)
     {
         return;
     }
    // double val = core.get_value_f(1,2);

     time->start();
     iter_count = 0;

     iteract(a,a,b,b,x,y, 0);

     int count = 0;// Счетчик итераций;
     int len = 0;
     QProgressDialog progress(tr("Процесс создания графика"), tr("Отмена"), 0, num, this);
     progress.setModal(true);
     progress.setFixedSize(250, 125);
     progress.show();

     int start = 0;
     int value[2];

     int _value[2];
     while (++count < num)
     {
         len = x.size();
         //start = 0;
         for(int i = 0; i<len-1;)
         {
             start = i;

#ifdef OPTIMIZE
             value[TYPE_X] = get_function(TYPE_X, x[i], y[i]);
             value[TYPE_Y] = get_function(TYPE_Y, x[i], y[i]);
             while(i < len - 1)
             {
#endif
                 i++;
#ifdef OPTIMIZE
                 _value[TYPE_X] = get_function(TYPE_X, x[i], y[i]);
                 _value[TYPE_Y] = get_function(TYPE_Y, x[i], y[i]);

                 if( get_dl(value[0], value[1], _value[0], _value[1]) >= h || abs(x[start] - x[i]) >= h/2 || abs(y[start] - y[i]) >= h/2)
                     break;
             }
#endif
             iteract(x.at(start),y.at(start),x.at(i),y.at(i),_x,_y, 0);// делаем итерацию для двух точек, поэтому нам нужно 4 координаты и записываем это в массив pts2
         }
         x = _x;
         y = _y;

         _x.clear();
         _y.clear();
         progress.setValue(count);
         qApp->processEvents();
     }
      progress.cancel();
    // curve->setData(x,y);

     curr_pos = 0;
     pre_pos = 0;
     timer->start(100);
     //Подписываем оси Ox и Oy
     ui->widget->xAxis->setLabel("x");
     ui->widget->yAxis->setLabel("y");

     ui->widget->axisRect()->setupFullAxesBox();

     //Установим область, которая будет показываться на графике
     ui->widget->xAxis->setRange(range_min[0], range_max[0]);//Для оси Ox
     ui->widget->yAxis->setRange(range_min[1], range_max[1]);//Для оси Oy

     ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
     ui->widget->replot();

     QString str = "Количество итераций:"+QString::number(count)+"\nВремя выполнения работы:"+QString::number(time->elapsed())+" mсекунд"+"\n"+"Количество элементов:"+QString::number(x.size())\
             +"\nКоличество операций:"+QString::number(iter_count);

     QMessageBox::information(this,tr("Результаты"), str);
}

void MainWindow::on_actionClear_Grapg_triggered()
{
    timer->stop();
    curve->clearData();
    ui->widget->replot();
}
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"

#include <QMessageBox>
#include <QProgressDialog>
#include <QTimer>
#include <QFile>
#include <QFileDialog>
#include <QByteArray>
#include <QAction>


#include "settings.h"

#define OPTIMIZE
#define clamp(a, b, c) a= a>b? b : a< c ? c : a


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(ui->widget);
#ifndef NEWQCP
    curve = new QCPCurve(ui->widget->xAxis, ui->widget->yAxis);
    curve->setPen(QPen(Qt::darkGreen, 1, Qt::SolidLine));
    ui->widget->addPlottable(curve);
#endif

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timer_event()));

    ui->menuEdit->addAction("Iteration",this, SLOT(on_actionIteration_triggered()));
    ui->menuEdit->addAction("Combine",this, SLOT(on_actionCombine_triggered()));
    ui->menuEdit->addSeparator();

    curr_vector = -1;

    setWindowTitle("ProgaDiff");

    QSettings settings(APP_ORG,APP_NAME);

    short theme = settings.value(settings_save_theme, 0).toInt();

    if(theme == 1)  SetDarkTheme();
    else            SetDefaultTheme();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_actionNew_triggered()
{
    Dialog *wnd = new Dialog(this);
    QObject::connect(wnd,SIGNAL(OnOkClicked(QString,QString,double,double,double,double,QString,QString,bool,double,int)),\
                     this, SLOT(on_pushButton_clicked(QString,QString,double,double,double,double,QString,QString,bool,double,int)));
    wnd->show();
}

#define TYPE_X 0
#define TYPE_Y 1

double get_function( IterCore* core, int type,double x, double y)
{
    switch (type)
    {
        case TYPE_X:    return core->get_function_value(FUNCTIONS_F,x,y);
        case TYPE_Y:    return core->get_function_value(FUNCTIONS_G,x,y);
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
void MainWindow::iteract(IterCore* core,double a, double b, double c, double d, QVector<double> &x, QVector<double> &y, int DEBUG)
{
    double start_x = a, start_y = b, end_x = c, end_y = d;

    double a_x = get_function(core, TYPE_X, start_x, start_y);
    double a_y = get_function(core, TYPE_Y, start_x, start_y);

    double b_x = get_function(core, TYPE_X, end_x, end_y);
    double b_y = get_function(core, TYPE_Y, end_x, end_y);

    double dl = get_dl(a_x, a_y, b_x, b_y);
    add_to_image( a_x, a_y, range_min[0],range_min[1],range_max[0], range_max[1], x, y);

    double c_x = a_x;
    double c_y = a_y;

    double h_x = 0.0;
    double h_y = 0.0;

    double t_x, t_y;
    //QVector<double> vec;

    while( dl > h) // пока длина больше H
    {
        t_x = end_x;// Присваем координаты конца отрезка
        t_y = end_y;// Присваем координаты конца отрезка
        while ( dl > h ) // пока длина больше H
        {

            t_x = get_midl(start_x, t_x); // Получаем середину
            t_y = get_midl(start_y, t_y);// Получаем середину
            h_x = get_function(core, TYPE_X,t_x, t_y);// Отображаем середину
            h_y = get_function(core, TYPE_Y,t_x, t_y); // Отображаем середину
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

#ifdef NEWQCP
    int qcpLen = mycurve.size();
#endif

    for(int i = pre_pos; i<curr_pos+1; i++)
    {
        if(i>=len)
        {
            timer->stop();
            work = false;
            break;
        }
#ifndef NEWQCP
        curve->addData(x[i], y[i]);
#else
        mycurve.at(qcpLen-1)->addData(x[i], y[i]);
#endif
    }
    ui->widget->replot();

    pre_pos = curr_pos;
    curr_pos+=x.length()/10;
}

void MainWindow::on_pushButton_clicked(QString f, QString g, double min, double max, double b_min, double b_max, QString constText,\
                                       QString color, bool dynamic, double _h,int num)
{

    while (work)
        qApp->processEvents();

    work = true;
#ifndef NEWQCP
     curve->clearData();
#endif
     IterCore* core = new IterCore( this, h, f, g, constText);

     if( core->isError() )
     {
         QString err_str = core->get_error_string();

         QMessageBox::warning(this, "Error", err_str);
         return;
     }

     TakeIteration(core, min, max, b_min, b_max, _h, x, y, 0, num);

      Color rgb[3];
      get_color_fstring(color, rgb);

      QCPCurve *curve = new QCPCurve(ui->widget->xAxis, ui->widget->yAxis);
      ui->widget->addPlottable(curve);

      mycurve.push_back(curve);

     Vectors vecs;
     for(int i = 0; i< myvector.size(); i++)
     {
         vecs = myvector.at(i);
         vecs.action->setChecked(false);

         myvector.removeAt(i);
         myvector.insert(i, vecs);
     }

     Vectors vec;
     vec.vec_x = x;
     vec.vec_y = y;

     vec.core = core;
     vec.min = min;
     vec.max = max;

     vec.b_min = b_min;
     vec.b_max = b_max;
     vec.h = _h;
     vec.dynamic = dynamic;
     vec.num = num;

     vec.rgb[0] = rgb[0];
     vec.rgb[1] = rgb[1];
     vec.rgb[2] = rgb[2];

     QAction * action = ui->menuEdit->addAction("MyVector"+QString::number( myvector.size() ), this, SLOT(close()));
     action->setCheckable(true);
     action->setChecked(true);

     vec.action = action;
     vec.curve = mycurve.size() - 1;
     myvector.push_back(vec);

     PlotGraph(mycurve.size()-1, rgb, dynamic, x, y);
}

void MainWindow::on_actionClear_Grapg_triggered()
{
    timer->stop();
#ifdef NEWQCP
    for(int i = 0; i<mycurve.size(); i++)
    {
        mycurve.at(i)->clearData();
        ui->widget->removePlottable(mycurve.at(i));
    }
    mycurve.clear();

    myvector.clear();
    ui->menuEdit->clear();
    ui->menuEdit->addAction("Iteration",this, SLOT(on_actionIteration_triggered()));
    ui->menuEdit->addAction("Combine",this, SLOT(on_actionCombine_triggered()));
    ui->menuEdit->addSeparator();

#else
    curve->clearData();
#endif
    ui->widget->replot();
}
void MainWindow::get_color_fstring(QString color, Color rgb[3])
{
    color.replace("x", " ");

    QStringList list;

    list = color.split(' ');

    int i = 0;
    foreach(QString str, list)
    {
        if( i>= 3)
            break;

        rgb[i] = str.toInt();
        i++;
    }

}
void MainWindow::on_actionOpen_triggered()
{
    if(myvector.size() > 0)
    {
        int result = QMessageBox::warning(this, "Предупреждение", "Все сохранненые данные будут утеряны\n Сохранить?", "Yes", "No", QString(), 0, 1 );
        if (!result)
            SaveProgress("temp.cfg");
    }


   MainWindow::on_actionClear_Grapg_triggered();
   QString filename = QFileDialog::getOpenFileName( this, "Открыть файл", QDir::currentPath(),"Cfg|Ini (*.ini *.cfg);;All files (*.*)");

   if (filename.size() == 0)
       return;

   QFile file(filename);
   QByteArray data;

   if( !file.open(QIODevice::ReadOnly))
       return;
   data = file.readAll();

   QString fdata = QString(data);
   QStringList list;

   list = fdata.split("\r\n");

   bool index = false;
   int line = 0;

   QString qstr[14];

   foreach(QString str, list)
   {
        if(str.contains("[START]"))
        {
            index = true;
            line = 0;
            continue;
        }
        else if(str.contains("[END]"))
        {
            index = false;
            qstr[13] = qstr[9]+"x"+qstr[10]+"x"+qstr[11];

            on_pushButton_clicked(qstr[0], qstr[1], qstr[4].toDouble(), qstr[5].toDouble(), qstr[2].toDouble(),qstr[3].toDouble(),\
                    qstr[12],qstr[13],static_cast<bool>(qstr[7].toInt()), qstr[6].toDouble(), qstr[8].toInt());

            continue;
        }

        if (!index)
            continue;

        qstr[line] = str;
//        qDebug() << line << ":" << str;
        line++;

   }

   //QMessageBox::information(this, "Info", fdata);
}

void MainWindow::SaveProgress (QString filename)
{
    QFile file(filename);

    if(file.isOpen())
        return;

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream write(&file);

    Vectors vec;
    for(int i = 0; i<myvector.size(); i++)
    {
        vec = myvector.at(i);

        write << "[START]" << "\n";

        write << vec.core->get_function(0) << "\n";
        write << vec.core->get_function(1) << "\n";

        write << vec.b_min << "\n";
        write << vec.b_max << "\n";
        write << vec.min << "\n";
        write << vec.max << "\n";

        write << vec.h << "\n";
        write << vec.dynamic << "\n";
        write << vec.num << "\n";

        write << vec.rgb[0] << "\n";
        write << vec.rgb[1] << "\n";
        write << vec.rgb[2] << "\n";

       // qDebug() << "qq";
/*
        for(int j = 0; j<vec.vec_x.size(); j++)
            write << vec.vec_x.at(j) << " ";

        write << "\n";
        qApp->processEvents();
       // qDebug() << "qq2";

        for(int j = 0; j<vec.vec_y.size(); j++)
            write << vec.vec_y.at(j) << " ";

       // qDebug() << "qq3";
        write << "\n";
        */

        write << vec.core->get_function(3) << "\n";

        write << "[END]" << "\n";
        qApp->processEvents();
    }

    file.close();
}

void MainWindow::on_actionSave_triggered()
{
    if (x.size() <= 0 || y.size() <= 0)
    {
        QMessageBox::warning(this,"Предупреждения", "Внимание, нечего сохранять");
        return;
    }


    QString filename = QFileDialog::getSaveFileName(this,
                                QString::fromUtf8("Сохранить файл"),
                                QDir::currentPath(),
                                "Cfg|Ini (*.ini *.cfg);;All files (*.*)");

    if (filename.size() <= 0 || myvector.size() <= 0)
        return;

    SaveProgress(filename);
}
void MainWindow::on_actionIteration_triggered()
{
    int size = myvector.size();
    int cur = -1;

    Vectors vec;
    for(int i = 0; i<size; i++)
    {
        vec = myvector.at(i);

        if ( !vec.action->isChecked() )
            continue;

        cur = i;
        break;
    }

    if( cur == -1)
        return;

    vec = myvector.at(cur);

    TakeIteration(vec.core, vec.min, vec.max, vec.b_min, vec.b_max, vec.h, vec.vec_x, vec.vec_y, vec.num, vec.num+1);
    mycurve.at(vec.curve)->clearData();

    PlotGraph(vec.curve, vec.rgb, vec.dynamic, vec.vec_x, vec.vec_y);

    myvector.remove(cur);

    vec.num+=1;

    myvector.insert(cur, vec);
}
void MainWindow::on_actionCombine_triggered()
{
    QVector<QVector<double>> _x;
    QVector<QVector<double>> _y;

    QVector<double> xx;
    QVector<double> yy;

    Vectors vec;

    for(int i = 0; i< myvector.size(); i++)
    {
        vec = myvector.at(i);

        _x.push_back(vec.vec_x);
        _y.push_back(vec.vec_y);
    }

    QSettings settings(APP_ORG,APP_NAME);
    double _h = settings.value(settings_save_step_combine, 0.05).toDouble();

    for(int i = 0; i<_x.size() -1; i++)
        for(int j = 0; j< _x[i].size(); j++)
            for(int k = 0; k< _x[i+1].size(); k++)
            {
                if( MainWindow::get_dl(_x[i][j], _y[i][j], _x[i+1][k], _y[i+1][k]) < _h)
                {
                    xx.push_back(_x[i][j]);
                    yy.push_back(_y[i][j]);
                }
            }

    MainWindow::on_actionClear_Grapg_triggered();

    QCPCurve *curve = new QCPCurve(ui->widget->xAxis, ui->widget->yAxis);
    ui->widget->addPlottable(curve);

    mycurve.push_back(curve);
    int rgb[3] = {200, 100, 50};

    for(int i = 0; i<3; i++)
        rgb[i] = qrand() % 255;
    PlotGraph(mycurve.size()-1, rgb, 1, xx, yy);
}

void MainWindow::PlotGraph( int this_curve, int rgb[3], bool dynamic, QVector<double> x, QVector<double> y)
{

    if (mycurve.size() <= 0)
        return;

    QColor colors(rgb[0],rgb[1],rgb[2]);
    QPen pen(colors);

    QCPCurve * curve;
    curve = mycurve.at(this_curve);

    curve->setPen(pen);

    if( dynamic )
    {
        curr_pos = 0;
        pre_pos = 0;

        MainWindow::x = x;
        MainWindow::y = y;

        timer->start(100);
    }
    else
    {
        work = false;
        curve->setData(x,y);
    }

    //Подписываем оси Ox и Oy
    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");

    ui->widget->axisRect()->setupFullAxesBox();

    //Установим область, которая будет показываться на графике
    ui->widget->xAxis->setRange(range_min[0], range_max[0]);//Для оси Ox
    ui->widget->yAxis->setRange(range_min[1], range_max[1]);//Для оси Oy

    ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->widget->replot();
}
void MainWindow::TakeIteration(IterCore * core, double min, double max, double b_min, double b_max, double _h, QVector<double> &x, QVector<double> &y, int step, int finish)
{
    double a = min;
    double b =  max;

    h = _h;

    QTime * time = new QTime;
    time->start();

    if(step == 0)
    {
        x.clear();
        y.clear();

    }

    _x.clear();
    _y.clear();

    range_min[0] = b_min;
    range_min[1] = b_min;
    range_max[0] = b_max;
    range_max[1] = b_max;

    iter_count = 0;

    if(step == 0)
        iteract(core,a,a,b,b,x,y, 0);

    int count = step-1 < 0 ? 0 : (step-1);// Счетчик итераций;
    int len = 0;
    QProgressDialog progress(tr("Процесс создания графика"), tr("Отмена"), count, finish, this);
    progress.setModal(true);
    progress.setFixedSize(250, 125);
    progress.show();

    int start = 0;
    int value[2];

    int _value[2];
    while (++count < finish)
    {
        len = x.size();
        //start = 0;
        for(int i = 0; i<len-1;)
        {
            start = i;

#ifdef OPTIMIZE
            value[TYPE_X] = get_function(core, TYPE_X, x[i], y[i]);
            value[TYPE_Y] = get_function(core, TYPE_Y, x[i], y[i]);
            while(i < len - 1)
            {
#endif
                i++;
#ifdef OPTIMIZE
                _value[TYPE_X] = get_function(core, TYPE_X, x[i], y[i]);
                _value[TYPE_Y] = get_function(core, TYPE_Y, x[i], y[i]);

                if( get_dl(value[0], value[1], _value[0], _value[1]) >= h || abs(x[start] - x[i]) >= h/2 || abs(y[start] - y[i]) >= h/2)
                    break;
            }
#endif
            iteract(core,x.at(start),y.at(start),x.at(i),y.at(i),_x,_y, 0);// делаем итерацию для двух точек, поэтому нам нужно 4 координаты и записываем это в массив pts2
        }
        x = _x;
        y = _y;

        _x.clear();
        _y.clear();
        progress.setValue(count);
        qApp->processEvents();
    }
     progress.cancel();


     QSettings settings(APP_ORG,APP_NAME);
     if( settings.value(settings_save_info,true).toBool())
     {
        QString str = "Количество итераций:"+QString::number(count)+"\nВремя выполнения работы:"+QString::number(time->elapsed())+" mсекунд"+"\n"+"Количество элементов:"+QString::number(x.size())\
             +"\nКоличество операций:"+QString::number(iter_count);

         QMessageBox::information(this,tr("Результаты"), str);
        // work = false;
     }
}


void MainWindow::on_actionRelease_triggered()
{
    QMessageBox::information(this,tr("О Приложении"), "Версия "+MainWindow::version+"\n");
}

void MainWindow::on_actionPreference_triggered()
{
    Settings *wnd = new Settings(this);
    wnd->show();
}
void MainWindow::SetDefaultTheme()
{
    qApp->setPalette(style()->standardPalette());
}
void MainWindow::SetDarkTheme()
{
    // Создаём палитру для тёмной темы оформления
    QPalette darkPalette;

    // Настраиваем палитру для цветовых ролей элементов интерфейса
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    // Устанавливаем данную палитру
    qApp->setPalette(darkPalette);
}

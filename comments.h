#ifndef COMMENTS_H
#define COMMENTS_H

/*
 * QProgressDialog progress(tr("Создание проекта"), tr("Cancel"), 0, 0, this);
     progress.setModal(true);
     progress.setFixedSize(400, 100);
     progress.show();
 * /
*/


/*
 *
 * void MainWindow::iteract(double a, double b, double c, double d, QVector<double> &x, QVector<double> &y, int DEBUG)
{
    double start_x = a, start_y = b, end_x = c, end_y = d;

    double h = 0.1; //Шаг
    double a_x = get_function(TYPE_X, start_x, start_y);
    double a_y = get_function(TYPE_Y, start_x, start_y);

    double b_x = get_function(TYPE_X, end_x, end_y);
    double b_y = get_function(TYPE_Y, end_x, end_y);

    double dl = get_dl(a_x, a_y, b_x, b_y);
    add_to_image( a_x, a_y, range_min[0],range_min[1],range_max[0], range_max[1], x, y);

    if(DEBUG)
        QMessageBox::information(0,"title","DL:"+QString::number(dl)+" "+QString::number(x.size()));

    double c_x = a_x;
    double c_y = a_y;

    double h_x = 0.0;
    double h_y = 0.0;

    double t_x, t_y;
    while( dl >= h) // пока длина больше H
    {
        t_x = end_x;// Присваем координаты конца отрезка
        t_y = end_y;// Присваем координаты конца отрезка
        while (dl >= h) // пока длина больше H
        {
            t_x = get_midl(start_x, t_x); // Получаем середину
            t_y = get_midl(start_y, t_y);// Получаем середину
            h_x = get_function(TYPE_X,t_x, t_y);// Отображаем середину
            h_y = get_function(TYPE_Y,t_x, t_y); // Отображаем середину
            dl = get_dl(c_x, c_y, h_x, h_y);// Получаем длина между серединой и отображением A
            if(DEBUG)
                QMessageBox::information(this,"title2","center:"+QString::number(t_x)+" "+QString::number(t_y));
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
 *
 * */

#endif // COMMENTS_H

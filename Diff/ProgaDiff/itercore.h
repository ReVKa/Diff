#ifndef ITERCORE_H
#define ITERCORE_H

#include <QVector>
#include <QString>
#include <QScriptEngine>
#include <QObject>

#include <QtGui>

#define X 0
#define Y 1

enum Errors
{
    ERROR_NO = 0,
    ERROR_YES,
    ERROR_F,
    ERROF_G,
    ERROR_CONST
};

enum FUNCTIONS
{
    FUNCTIONS_F = 0,
    FUNCTIONS_G
};

class IterCore : public QObject
{
    Q_OBJECT
public:
    explicit IterCore(QObject *parent = 0,double step = 0.1, QString _f="", QString _g="", QString constText="");

    double get_function_value(FUNCTIONS function,double x, double y);

    QString get_error_string();
    bool isError();

private:
    //int iter_count;
    int h;

    Errors error;

    QScriptEngine engine;
    QScriptValue f, g;
    QScriptValueList args;

    QVector<double> x, y, _x, _y; //Массивы координат точек

    QString replace_extension(QString _f);
    QVector<QString> parseConst(QString constText);
};

#endif // ITERCORE_H

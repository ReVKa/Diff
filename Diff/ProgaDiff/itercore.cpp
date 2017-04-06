#include "itercore.h"
IterCore::IterCore(QObject *parent,double step, QString _f, QString _g, QString constText) : QObject(parent)
{
    error = ERROR_NO;

    h = step;

    QVector<QString> list = parseConst(constText);

    for(int i = 0; i< list.length() - 1; i++)
    {
       _f.replace(list[i], list[i+1]);
        _g.replace(list[i], list[i+1]);

        f.setProperty(list[i], list[i+1].toDouble());
    }

    f = engine.evaluate("(function(x,y){ return "+replace_extension(_f)+" })");
    g = engine.evaluate("(function(x,y){ return "+replace_extension(_g)+"})");

    if(!f.isValid())
        error = ERROR_F;

    if(!g.isValid())
        error = ERROF_G;


}
QString IterCore::replace_extension(QString _f)
{

    QString replacer [] =
    {
        "abs",
        "sin",
        "cos",
        "exp",
        "tan",
        "log",
        "sqrt"
    };

    QString toreplace [] =
    {
        "Math.abs",
        "Math.sin",
        "Math.cos",
        "Math.exp",
        "Math.tan",
        "Math.log",
        "Math.sqrt"
    };

    for(int i = 0; i< 7; i++)
    {
        _f.replace(replacer[i], toreplace[i]);
    }

    return _f;
}
QVector<QString> IterCore::parseConst(QString constText)
{
    QVector<QString> list;
    QString temp;

    int len = constText.length();
    for( int i =0; i< len; i++)
    {

        if (constText[i] == ' ')
            continue;

        if(constText[i] == '\n' || constText[i] == ';')
        {
          //  qDebug() << temp;
            list.push_back(temp);
            temp.clear();
            continue;
        }

        if(constText[i] == '=')
        {
            if(temp[0] != '_')
                error = ERROR_CONST;

            list.push_back(temp);
            temp.clear();
            continue;
        }

        temp+=constText[i];
    }

    return list;
}

double IterCore::get_function_value(FUNCTIONS function,double x, double y)
{
    if (error != ERROR_NO)
        return -1.0;

    args.clear();

    args.push_back(x);
    args.push_back(y);

    QScriptValue value = function == FUNCTIONS_F ? f.call(QScriptValue(), args) : g.call(QScriptValue(), args);
    return (double)value.toNumber();
}
QString IterCore::get_error_string()
{
    QString _error;

    if (error == ERROR_NO)
        _error = "No errors";

    if( error == ERROR_CONST)
        _error = "Const must have first symbol as _";

    return _error;

}
bool IterCore::isError()
{
    return error;
}

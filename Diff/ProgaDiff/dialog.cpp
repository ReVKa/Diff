#include "dialog.h"
#include "ui_dialog.h"

#include <QPushButton>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QObject::connect(ui->buttonBox->button(QDialogButtonBox::Ok),SIGNAL(clicked(bool)),this,SLOT(OkClicked()));
}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::OkClicked()
{
    enum
    {
        F = 0,
        G,
        MIN,
        MAX,
        B_MIN,
        B_MAX,
        CONST,
        COLOR,
        DYNAMIC,
        STEP,
        NUM,

        MAX_COUNT
    };

    QString obj[MAX_COUNT];

    obj[F]          = ui->lineEdit_f->text();
    obj[G]          = ui->lineEdit_g->text();
    obj[MIN]        = ui->lineEdit_min->text();
    obj[MAX]        = ui->lineEdit_max->text();
    obj[B_MIN]      = ui->lineEdit_b_min->text();
    obj[B_MAX]      = ui->lineEdit_b_max->text();
    obj[CONST]      = ui->constText->toPlainText();
    obj[COLOR]      = ui->colorLine->text();
    obj[DYNAMIC]    = ui->dynamicBox->checkState() == Qt::Checked ? "1" : "0";
    obj[STEP]       = ui->stepLine->text();
    obj[NUM]        = ui->line_edit_num->text();


    QString obj_info[MAX_COUNT];

    obj_info[F]     = "Вы не ввели функцию F";
    obj_info[G]     = "Вы не ввели функцию G";
    obj_info[MIN]   = "Вы не ввели координаты точки мин";
    obj_info[MAX]   = "Вы не ввели координаты точки макс";
    obj_info[B_MIN] = "Вы не ввели координаты границы мин";
    obj_info[B_MAX] = "Вы не ввели координаты границы макс";
    obj_info[CONST] = "";
    obj_info[COLOR] = "Вы не ввели цвет";
    obj_info[DYNAMIC]="";
    obj_info[STEP]  = "Вы не указали шаг";
    obj_info[NUM]   = "Вы не ввели количество итераций";

    for(int i =0 ; i<MAX_COUNT; i++)
    {
        if(obj[i].length() <= 0 && i != CONST && i != DYNAMIC)
        {
            QMessageBox::warning(this,"Ошибка", obj_info[i]);
            return;
        }
    }

    double min = obj[MIN].toDouble();
    double max = obj[MAX].toDouble();
    int num = obj[NUM].toInt();

    double step = obj[STEP].toDouble();
    bool check = obj[DYNAMIC] == "1" ? true : false;

    if(num == 0)
    {
        QMessageBox::warning(this,"Ошибка","Невозможно сделать <=0 итераций");
        return;
    }

    emit OnOkClicked(obj[F], obj[G], min, max, obj[B_MIN].toDouble(), obj[B_MAX].toDouble(), obj[CONST], obj[COLOR], check, step, num);
}

void Dialog::on_infoFuncButton_clicked()
{
    QString function = "Доступные функции:\nMath.abs(<выражение>) или abs(<выражение>)\n";
    function += "Math.cos(<выражение>) или cos(<выражение>)\n";
    function += "Math.sin(<выражение>) или sin(<выражение>)\n";
    function += "Math.exp(<выражение>) или exp(<выражение>)\n";
    function += "Math.tan(<выражение>) или tan(<выражение>)\n";
    function += "Math.sqrt(<выражение>) или sqrt(<выражение>)\n";
    function += "Math.log(<выражение>) или log(<выражение>)\n";
    function += "Math.pow(<выражение>, <степень>) возведение в степень\n";

    function += "А также можно использовать:\n";
    function += "Math.acos(<выражение>), Math.asin(<выражение>),Math.atan(<выражение>)";

    QMessageBox *mbox = new QMessageBox(this);
    mbox->setText(function);
    mbox->setWindowTitle("Информация о функциях");

    mbox->show();
}

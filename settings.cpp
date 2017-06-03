#include "settings.h"
#include "ui_settings.h"

#include <QStyleFactory>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    QSettings settings(APP_ORG,APP_NAME);

    short theme = settings.value(settings_save_theme, 0).toInt();

    ui->checkBox->setChecked(false);
    ui->checkBox_2->setChecked(false);

    if(theme == 1)  ui->checkBox_2->setChecked(true);
    else            ui->checkBox->setChecked(true);

    int opt_value = settings.value(settings_save_opt, 1).toInt();

    switch (opt_value)
    {
        case 1: ui->radioButton_2->setChecked(true);break;
        case 2: ui->radioButton_3->setChecked(true);break;
        default: ui->radioButton->setChecked(true);break;
    }

    ui->lineEdit->setText(settings.value(settings_save_step_combine, 0.05).toString());

    bool info = settings.value(settings_save_info, true).toBool();
    ui->checkBox_3->setChecked(info);

    setWindowTitle("Preference");
}

Settings::~Settings()
{
    delete ui;
}
void Settings::closeEvent(QCloseEvent * e)
{
   // QMessageBox::information(this, "Info", "Exit");
    QSettings settings(APP_ORG,APP_NAME);

    if( ui->checkBox->isChecked())
        settings.setValue(settings_save_theme, 0);
    else if ( ui->checkBox_2->isChecked())
        settings.setValue(settings_save_theme, 1);

    settings.setValue(settings_save_step_combine, ui->lineEdit->text().toDouble());

    short opt_value = 0;
    if( ui->radioButton->isChecked())   opt_value = 0;
    else if( ui->radioButton_2->isChecked())   opt_value = 1;
    else if( ui->radioButton_3->isChecked())   opt_value = 2;

    settings.setValue(settings_save_opt, opt_value);

    settings.setValue(settings_save_info, ui->checkBox_3->isChecked());

    settings.sync();
    e->accept();
}

void Settings::on_checkBox_clicked()
{
    SetDefaultTheme();
    ui->checkBox_2->setChecked(false);
}

void Settings::on_checkBox_2_clicked()
{
    SetDarkTheme();
    ui->checkBox->setChecked(false);
}
void Settings::SetDefaultTheme()
{
    qApp->setPalette(style()->standardPalette());
}
void Settings::SetDarkTheme()
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

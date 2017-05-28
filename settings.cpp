#include "settings.h"
#include "ui_settings.h"

#include <QStyleFactory>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

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
        settings.setValue("settings/theme", 0);
    else if ( ui->checkBox_2->isChecked())
        settings.setValue("settings/theme", 1);

    settings.sync();
    e->accept();
}

void Settings::on_checkBox_clicked()
{
    qApp->setPalette(style()->standardPalette());

    ui->checkBox_2->setChecked(false);
}

void Settings::on_checkBox_2_clicked()
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
    ui->checkBox->setChecked(false);
}

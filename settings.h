#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QCloseEvent>
#include <QMessageBox>

#include "mainwindow.h"

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

    void SetDefaultTheme();
    void SetDarkTheme();
private slots:

    void on_checkBox_clicked();
    void on_checkBox_2_clicked();
    void closeEvent(QCloseEvent * e);

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
private slots:
    void OkClicked();

    void pushButton_clicked();

signals:
    void OnOkClicked(QString f, QString g, double min, double max, double b_min, double b_max, QString constText, int num);
};

#endif // DIALOG_H

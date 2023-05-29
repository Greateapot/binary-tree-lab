#pragma once

#include <QDialog>

namespace Ui
{
    class AddCityDialog;
}

class AddCityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddCityDialog(QWidget *parent = nullptr);
    ~AddCityDialog();

    int value;
    bool cancel = true;

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::AddCityDialog *ui;
};

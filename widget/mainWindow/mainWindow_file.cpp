#include "mainWindow.h"
#include "ui_mainWindow.h"

#include <QFileDialog>

void MainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Save"), "",
        tr("Text Files (*.txt)"));

    if (fileName.isEmpty())
        return ui->statusbar->showMessage("Имя файла не может быть пустым!", STATUS_MESSAGE_DELAY);

    std::fstream file(fileName.toStdString(), std::ios::out | std::ios::binary);
    if (!file)
        return ui->statusbar->showMessage("Не удается открыть файл!", STATUS_MESSAGE_DELAY);

    {
        // рут первым, чтоб потом не устать
        CityModel *city = root;
        file << city->value << DLTR;
    }

    for (auto item : scene->items())
    {
        CityModel *city = dynamic_cast<CityModel *>(item);
        if (city == nullptr || city == root)
            continue;

        file << city->value << DLTR;
    }

    file.close();
}

void MainWindow::load()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Load"), "",
        tr("Text Files (*.txt)"));

    if (fileName.isEmpty())
        return ui->statusbar->showMessage("Имя файла не может быть пустым!", STATUS_MESSAGE_DELAY);

    std::fstream file(fileName.toStdString(), std::ios::in | std::ios::binary);
    if (!file)
        return ui->statusbar->showMessage("Не удается открыть файл!", STATUS_MESSAGE_DELAY);

    delete root;
    root = nullptr;
    scene->clear();

    while (file.peek() != -1)
    {
        std::string value_raw;
        int value;

        getline(file, value_raw, DLTR);
        value = stoi(value_raw);

        if (insert(value))
            balance();
    }

    scene->update();
    file.close();
}

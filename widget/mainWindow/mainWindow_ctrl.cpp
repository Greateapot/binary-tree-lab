#include "mainWindow.h"
#include "ui_mainWindow.h"

#include "utils/utils.h"

void MainWindow::unselectAll()
{
    for (auto item : scene->items())
    {
        CityModel *city = dynamic_cast<CityModel *>(item);
        if (city != nullptr)
            city->is_selected = false;
    }
}

void MainWindow::select(QGraphicsItem *item)
{
    if (!solving)
        unvisitAll();
    unselectAll();
    CityModel *city = dynamic_cast<CityModel *>(item);
    selected = city;     // неважно, есть ссылка или нет
    if (city != nullptr) // а вот это важно
        city->is_selected = true;
}

void MainWindow::unvisitAll()
{
    for (auto item : scene->items())
    {
        CityModel *city = dynamic_cast<CityModel *>(item);
        if (city)
            city->is_visited = false;
    }
}

void MainWindow::visit(CityModel *city)
{
    ui->statusbar->showMessage(QString::fromStdString("City " + std::to_string(city->value) + " visited!"), STATUS_MESSAGE_DELAY);
    city->is_visited = true;
    scene->QGraphicsScene::update();
    delay(1000);
}

#include "mainWindow.h"
#include "ui_mainWindow.h"

#include "dialog/addCity/addCityDialog.h"

void MainWindow::addCity()
{
    AddCityDialog dialog(this);
    dialog.exec();
    if (!dialog.cancel)
        if (insert(dialog.value))
            balance();
}

void MainWindow::removeCity()
{
    if (selected != nullptr)
        if (remove(selected->value))
            balance();
}

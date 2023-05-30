#include "mainWindow.h"
#include "ui_mainWindow.h"

#include "dialog/solutionInput/solutionInputDialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new Scene(ui->graphicsView);
    ui->graphicsView->setScene(scene);
    connect(scene, &Scene::item_selected, this, &MainWindow::select);

    connect(ui->addButton_City, &QPushButton::released, this, &MainWindow::addCity);
    connect(ui->addButton_Road, &QPushButton::released, this, &MainWindow::removeCity);
    connect(ui->solveButton, &QPushButton::released, this, &MainWindow::showSolution);
    
    // connect(ui->actionsave, &QAction::triggered, this, &MainWindow::save);
    // connect(ui->actionload, &QAction::triggered, this, &MainWindow::load);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    delete root;
    delete selected;
}

void MainWindow::showSolution()
{
    SolutionInputDialog dialog(this);
    dialog.exec();

    if (!dialog.cancel)
    {
        solving = 1;
        switch (dialog.solution_method)
        {
        case 0:
            preOrder();
            break;
        case 1:
            inOrder();
            break;
        case 2:
            postOrder();
            break;
        }
        solving = 0;
    }
}

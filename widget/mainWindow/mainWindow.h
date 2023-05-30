#pragma once

#include <vector>

#include <QMainWindow>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QDoubleValidator>

#include "model/city/cityModel.h"
#include "widget/scene/scene.h"

const char GDLTR = '\1';
const int STATUS_MESSAGE_DELAY = 5000;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addCity();
    void removeCity();

    void showSolution();

    void unselectAll();
    void select(QGraphicsItem *);

    void unvisitAll();
    void visit(CityModel *);

    // void save();
    // void load();

private:
    Ui::MainWindow *ui;
    Scene *scene;

    CityModel *root = nullptr,
              *selected = nullptr;

    bool solving = false;

public:
    bool insert(int);
    bool remove(int);
    int height(CityModel *);
    void preOrder();
    void inOrder();
    void postOrder();
    void balance();
    void repair_parents(CityModel *);

private:
    void _preOrder(CityModel *node = nullptr);
    void _inOrder(CityModel *node = nullptr);
    void _postOrder(CityModel *node = nullptr);
    bool _balance(CityModel *, CityModel *);
    void _turn_m_on(CityModel *, CityModel *);
    void _turn_m_op(CityModel *, CityModel *);
    void _turn_b_on(CityModel *, CityModel *);
    void _turn_b_op(CityModel *, CityModel *);
};

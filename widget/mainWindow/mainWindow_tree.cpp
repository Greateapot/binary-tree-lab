#include "mainWindow.h"
#include "ui_mainWindow.h"

#include <fstream>


bool MainWindow::insert(int value)
{
    if (root == nullptr)
    {
        root = new CityModel(value);
        ui->statusbar->showMessage(QString::fromStdString("City " + std::to_string(value) + " inserted!"), STATUS_MESSAGE_DELAY);
        scene->addItem(root);
        scene->QGraphicsScene::update();
        return true;
    }
    CityModel *cur = root;
    while (cur != nullptr)
    {
        if (value == cur->value)
        {
            ui->statusbar->showMessage(QString::fromStdString("City " + std::to_string(value) + " already exists!"), STATUS_MESSAGE_DELAY);
            return false;
        }
        else if (value < cur->value)
            if (cur->tree_left != nullptr)
                cur = cur->tree_left;
            else
            {
                ui->statusbar->showMessage(QString::fromStdString("City " + std::to_string(value) + " inserted!"), STATUS_MESSAGE_DELAY);
                cur->tree_left = new CityModel(value, cur);
                scene->addItem(cur->tree_left);
                scene->QGraphicsScene::update();
                return true;
            }
        else if (cur->tree_right != nullptr)
            cur = cur->tree_right;
        else
        {
            ui->statusbar->showMessage(QString::fromStdString("City " + std::to_string(value) + " inserted!"), STATUS_MESSAGE_DELAY);
            cur->tree_right = new CityModel(value, cur);
            scene->addItem(cur->tree_right);
            scene->QGraphicsScene::update();
            return true;
        }
    }
    ui->statusbar->showMessage("Insertion err", STATUS_MESSAGE_DELAY);
    return false;
}

bool MainWindow::remove(int value)
{
    CityModel *cur = root;
    while (cur != nullptr && cur->value != value)
        cur = (value < cur->value ? cur->tree_left : cur->tree_right);

    if (cur == nullptr)
    {
        ui->statusbar->showMessage(QString::fromStdString("City " + std::to_string(value) + " doesn't exists!"), STATUS_MESSAGE_DELAY);
        return false;
    }
    else if (cur == root)
    {
        ui->statusbar->showMessage(QString::fromStdString("You can't remove root city!"), STATUS_MESSAGE_DELAY);
        return false;
    }
    else if (cur->tree_left == nullptr && cur->tree_right == nullptr)
    {
        (cur->tree_parent->tree_left == cur ? cur->tree_parent->tree_left : cur->tree_parent->tree_right) = nullptr;
    }
    else if (cur->tree_left != nullptr && cur->tree_right != nullptr)
    {
        bool isRight = cur->tree_left->tree_right != nullptr;
        CityModel *temp = (isRight ? cur->tree_left->tree_right : cur->tree_right->tree_left);

        if (temp == nullptr)
            temp = isRight ? cur->tree_right : cur->tree_left;
        else
            while (isRight ? temp->tree_right != nullptr : temp->tree_left != nullptr)
                temp = (isRight ? temp->tree_right : temp->tree_left);

        if ((isRight ? temp->tree_left : temp->tree_right) != nullptr)
            (isRight ? temp->tree_left : temp->tree_right)->tree_parent = temp->tree_parent;

        (isRight ? temp->tree_parent->tree_right : temp->tree_parent->tree_left) = (isRight ? temp->tree_right : temp->tree_left);
        temp->tree_parent = cur->tree_parent;
        temp->tree_left = cur->tree_left;
        temp->tree_right = cur->tree_right;
        (cur->tree_parent->tree_left == cur ? cur->tree_parent->tree_left : cur->tree_parent->tree_right) = temp;
    }
    else
    {
        (cur->tree_parent->tree_left == cur ? cur->tree_parent->tree_left : cur->tree_parent->tree_right) = (cur->tree_left != nullptr ? cur->tree_left : cur->tree_right);
        (cur->tree_left != nullptr ? cur->tree_left : cur->tree_right)->tree_parent = cur->tree_parent;
    }

    scene->removeItem(cur);
    scene->QGraphicsScene::update();
    delete cur;

    ui->statusbar->showMessage(QString::fromStdString("City " + std::to_string(value) + " removed!"), STATUS_MESSAGE_DELAY);
    return true;
}

int MainWindow::height(CityModel *node)
{
    if (node == nullptr || (node->tree_left == nullptr && node->tree_right == nullptr))
        return 0;
    else
    {
        int hl = height(node->tree_left),
            hr = height(node->tree_right);
        return (hl > hr ? hl : hr) + 1;
    }
}

void MainWindow::preOrder()
{
    _preOrder(root);
}

void MainWindow::inOrder()
{
    _inOrder(root);
}

void MainWindow::postOrder()
{
    _postOrder(root);
}

void MainWindow::balance()
{
    if (root != nullptr)
        _balance(root, nullptr);
    repair_parents(root);
    root->repos(height(root) + 1);
}

void MainWindow::repair_parents(CityModel *r)
{
    if (r == root)
        r->tree_parent = nullptr;
    if (r->tree_left != nullptr)
    {
        r->tree_left->tree_parent = r;
        repair_parents(r->tree_left);
    }
    if (r->tree_right != nullptr)
    {
        r->tree_right->tree_parent = r;
        repair_parents(r->tree_right);
    }
}

void MainWindow::_preOrder(CityModel *node)
{
    if (node != nullptr)
    {
        visit(node);
        _preOrder(node->tree_left);
        _preOrder(node->tree_right);
    }
}

void MainWindow::_inOrder(CityModel *node)
{
    if (node != nullptr)
    {
        _inOrder(node->tree_left);
        visit(node);
        _inOrder(node->tree_right);
    }
}

void MainWindow::_postOrder(CityModel *node)
{
    if (node != nullptr)
    {
        _postOrder(node->tree_left);
        _postOrder(node->tree_right);
        visit(node);
    }
}

bool MainWindow::_balance(CityModel *r, CityModel *p)
{
    if ((height(r->tree_left) - height(r->tree_right)) == 2)
        height(r->tree_left->tree_left) > height(r->tree_left->tree_right) ? _turn_m_on(r, p) : _turn_b_on(r, p);

    if ((height(r->tree_left) - height(r->tree_right)) == -2)
        height(r->tree_right->tree_left) < height(r->tree_right->tree_right) ? _turn_m_op(r, p) : _turn_b_op(r, p);

    p = r;
    if (r->tree_left != nullptr)
        while (!_balance(r->tree_left, p))
            ; // 10/10 форматирование
    if (r->tree_right != nullptr)
        while (!_balance(r->tree_right, p))
            ; // мне ведь не жалко 4*4 байта памяти на такую мелочь
    return true;
}

void MainWindow::_turn_m_on(CityModel *r, CityModel *p)
{
    CityModel *temp = r->tree_left;
    r->tree_left = temp->tree_right;
    temp->tree_right = r;

    if (p == nullptr)
        root = temp;
    else if (p->value <= temp->value)
        p->tree_right = temp;
    else
        p->tree_left = temp;
}

void MainWindow::_turn_m_op(CityModel *r, CityModel *p)
{
    CityModel *temp = r->tree_right;
    r->tree_right = temp->tree_left;
    temp->tree_left = r;

    if (p == nullptr)
        root = temp;
    else if (p->value <= temp->value)
        p->tree_right = temp;
    else
        p->tree_left = temp;
}

void MainWindow::_turn_b_on(CityModel *r, CityModel *p)
{
    CityModel *temp = r->tree_left->tree_right;
    r->tree_left->tree_right = temp->tree_left;
    temp->tree_left = r->tree_left;
    r->tree_left = temp->tree_right;
    temp->tree_right = r;

    if (p == nullptr)
        root = temp;
    else if (p->value <= temp->value)
        p->tree_right = temp;
    else
        p->tree_left = temp;
}

void MainWindow::_turn_b_op(CityModel *r, CityModel *p)
{
    CityModel *temp = r->tree_right->tree_left;
    r->tree_right->tree_left = temp->tree_right;
    temp->tree_right = r->tree_right;
    r->tree_right = temp->tree_left;
    temp->tree_left = r;

    if (p == nullptr)
        root = temp;
    else if (p->value <= temp->value)
        p->tree_right = temp;
    else
        p->tree_left = temp;
}
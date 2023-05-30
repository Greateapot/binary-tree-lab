#pragma once

#include <string>
#include <vector>
#include <fstream>

#include <QGraphicsItem>
#include <QPainter>


class CityModel : public QGraphicsItem
{
public:
    CityModel(QGraphicsItem *parent);
    CityModel(
        int value,
        CityModel *tree_parent = nullptr,
        CityModel *tree_left = nullptr,
        CityModel *tree_right = nullptr,
        double radius = 25,
        QGraphicsItem *parent = nullptr);
    CityModel(const CityModel &);
    ~CityModel();

    friend bool operator==(const CityModel &, const CityModel &);

    double radius = 25;

    int value;

    CityModel *tree_parent = nullptr,
              *tree_left = nullptr,
              *tree_right = nullptr;

    bool is_selected = 0,
         is_visited = 0;

    void repos(int);

protected:
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *widget = nullptr) override;

    QRectF boundingRect() const override;
};

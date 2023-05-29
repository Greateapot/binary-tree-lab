#include "cityModel.h"

#include <cmath>

#include "utils/utils.h"

CityModel::CityModel(QGraphicsItem *parent = nullptr)
    : QGraphicsItem(parent), value(0)
{
    // setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

CityModel::CityModel(
    int value,
    CityModel *tree_parent,
    CityModel *tree_left,
    CityModel *tree_right,
    double radius,
    QGraphicsItem *parent)
    : QGraphicsItem(parent),
      value(value),
      tree_parent(tree_parent),
      tree_left(tree_left),
      tree_right(tree_right),
      radius(radius)
{
    // setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

CityModel::CityModel(const CityModel &city)
    : QGraphicsItem(city.parentItem()),
      value(city.value),
      tree_parent(city.tree_parent),
      tree_left(city.tree_left),
      tree_right(city.tree_right),
      radius(city.radius)

{
    // setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

bool operator==(const CityModel &c1, const CityModel &c2)
{
    return c1.value == c2.value;
}

void CityModel::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(is_visited ? Qt::green : (is_selected ? Qt::cyan : Qt::blue));

    painter->drawEllipse(boundingRect());

    if (tree_parent == nullptr)
    {
        painter->drawText(0, 0, "0,0");
        painter->drawText(100, 0, "100,0");
        painter->drawText(0, 100, "0,100");
        painter->drawText(100, 100, "100,100");
        painter->drawText(-100, -100, "-100,-100");
    }
    qreal x0 = pos().x(), y0 = pos().y(), x1 = x0 + radius * 2, y1 = y0 + radius * 2;
    {
        std::ofstream f("AV.txt");
        f << x0 << '\n'
          << y0 << '\n'
          << x1 << '\n'
          << y1;
        painter->drawText(x0, y0, QString::fromStdString(std::to_string((int)x0) + "," + std::to_string((int)y0)));
        painter->drawText(x1, y0, QString::fromStdString(std::to_string((int)x1) + "," + std::to_string((int)y0)));
        painter->drawText(x0, y1, QString::fromStdString(std::to_string((int)x0) + "," + std::to_string((int)y1)));
        painter->drawText(x1, y1, QString::fromStdString(std::to_string((int)x1) + "," + std::to_string((int)y1)));
        f.close();
    }
    QFontMetrics fm(painter->font());
    const QString v = double2QString(value);
    const int tw = fm.width(v),
              th = fm.height();
    painter->drawText(x0 + radius - tw / 2, y0 + radius - th / 4, v);

    if (tree_left != nullptr)
        painter->drawLine(x0 + radius, y0 + radius * 2, tree_left->pos().x() + radius, tree_left->pos().y());

    if (tree_right != nullptr)
        painter->drawLine(x0 + radius, y0 + radius * 2, tree_right->pos().x() + radius, tree_right->pos().y());
}
QRectF CityModel::boundingRect() const
{
    return QRectF(pos().x(), pos().y(), radius * 2, radius * 2);
}

void CityModel::repos(int length)
{
    if (tree_parent != nullptr && tree_parent->tree_left == this)
        setPos(
            tree_parent->pos().x() + radius - ((50 * (pow(2, length) - 1)) / 2 + radius),
            tree_parent->pos().y() + radius + radius * 4);
    else if (tree_parent != nullptr && tree_parent->tree_right == this)
        setPos(
            tree_parent->pos().x() + radius + ((50 * (pow(2, length) - 1)) / 2 + radius),
            tree_parent->pos().y() + radius + radius * 4);
    else
        setPos(0, 0);

    if (tree_left != nullptr)
        tree_left->repos(length - 1);
    if (tree_right != nullptr)
        tree_right->repos(length - 1);
}
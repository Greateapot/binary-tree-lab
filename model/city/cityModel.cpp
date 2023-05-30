#include "cityModel.h"

#include <cmath>

#include "utils/utils.h"
#include "widget/scene/scene.h"

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

CityModel::~CityModel()
{
    if (tree_left != nullptr)
        delete tree_left;
    if (tree_right != nullptr)
        delete tree_right;
}

bool operator==(const CityModel &c1, const CityModel &c2)
{
    return c1.value == c2.value;
}

void CityModel::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(is_visited ? Qt::green : (is_selected ? Qt::cyan : Qt::blue));

    QRectF rect = boundingRect();
    qreal x0 = rect.x(), y0 = rect.y(), x1 = x0 + radius * 2, y1 = y0 + radius * 2;

    painter->drawEllipse(rect);

    QFontMetrics fm(painter->font());
    const QString v = double2QString(value);
    const int tw = fm.width(v),
              th = fm.height();
    painter->drawText(x0 + radius - tw / 2, y0 + radius - th / 4, v);

    // Это не лечится, спасибо убогой системе координат Qt.
    if (tree_left != nullptr)
        painter->drawLine(x0 + radius, y0 + radius * 2, tree_left->boundingRect().x() + radius, tree_left->boundingRect().y());

    if (tree_right != nullptr)
        painter->drawLine(x0 + radius, y0 + radius * 2, tree_right->boundingRect().x() + radius, tree_right->boundingRect().y());
}
QRectF CityModel::boundingRect() const
{
    return QRectF(pos().x(), pos().y(), radius * 2, radius * 2);
}

void CityModel::repos(int length)
{
    if (tree_parent != nullptr && tree_parent->tree_left == this)
        setPos(
            tree_parent->boundingRect().x() + radius - ((50 * (pow(2, length) - 1)) / 2 + radius),
            tree_parent->boundingRect().y() + radius + radius * 4);
    else if (tree_parent != nullptr && tree_parent->tree_right == this)
        setPos(
            tree_parent->boundingRect().x() + radius + ((50 * (pow(2, length) - 1)) / 2 + radius),
            tree_parent->boundingRect().y() + radius + radius * 4);
    else
        setPos(0, 0);

    if (tree_left != nullptr)
        tree_left->repos(length - 1);
    if (tree_right != nullptr)
        tree_right->repos(length - 1);
}

#ifndef PATHITEM_H
#define PATHITEM_H

#include <QObject>
#include <QGraphicsLineItem>
#include <QPen>
#include <QGraphicsScene>
#include <path.h>

class PathItem : public QObject
{
    Q_OBJECT
public:
    PathItem(QVector<QLineF> vl, qreal opacity, QColor clr, QGraphicsScene *scn, int penWidth = 2);
    QVector<QGraphicsLineItem *> lines;
    void Replace(QVector<QLineF> ls, qreal opacity, QColor clr, QGraphicsScene *scn, int penWidth = 2);
    void deleteAllLines();
    PathItem();
    ~PathItem();
};

#endif // PATHITEM_H

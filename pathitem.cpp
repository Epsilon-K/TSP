#include "pathitem.h"

PathItem::PathItem(QVector<QLineF> vl, qreal opacity, QColor clr, QGraphicsScene *scn, int penWidth)
{
    Replace(vl, opacity, clr, scn, penWidth);
}

void PathItem::Replace(QVector<QLineF> vl, qreal opacity, QColor clr, QGraphicsScene *scn, int penWidth)
{
    deleteAllLines();
    for(int i = 0; i < vl.size(); i++){
        QGraphicsLineItem * l = new QGraphicsLineItem(vl[i]);
        QPen pen;
        pen.setBrush(clr);
        pen.setCapStyle(Qt::SquareCap);
        pen.setStyle(Qt::DotLine);
        pen.setWidth(penWidth);
        l->setPen(pen);
        l->setOpacity(opacity);
        lines.push_back(l);
        scn->addItem(l);
    }
}

void PathItem::deleteAllLines()
{
    for(int i = 0; i < lines.size(); i++){
        delete lines[i];
    }
    lines.clear();
}

PathItem::PathItem()
{

}

PathItem::~PathItem()
{
    deleteAllLines();
}

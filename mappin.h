#ifndef MAPPIN_H
#define MAPPIN_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include "utils.h"
#include <QVariant>
#include <QGraphicsDropShadowEffect>

class MapPin : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    MapPin(QPixmap * pix, int id);
    qreal width();
    qreal height();

    QString pointToString(QPointF p){
        QPoint ip = p.toPoint();
        return "(" + QString::number(ip.x()) + ", " + QString::number(ip.y()) + ")";
    }

    QPointF pos(){
        QPointF po = QGraphicsPixmapItem::pos();
        po.setX(po.x() + origin.x());
        po.setY(po.y() + origin.y());
        return po;
    }

    inline void setPos(qreal nx, qreal ny){
        QGraphicsItem::setPos(nx - origin.x(), ny - origin.y());
    }

    void centerLabel(){
        label->setPos(-label->boundingRect().width()/2 + width()/2,-height()/2);
    }

    // Vars
    QGraphicsTextItem *label;
    QPoint origin;
    QString name;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value)
    {
        if (change == ItemPositionHasChanged){
            label->setPlainText(name + "   " + pointToString(pos()));
            centerLabel();
        }
        return QGraphicsItem::itemChange(change, value);
    }
};

#endif // MAPPIN_H

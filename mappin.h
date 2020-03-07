#ifndef MAPPIN_H
#define MAPPIN_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include "utils.h"
#include <QVariant>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsSceneHoverEvent>
#include <QCursor>
#include <QTimeLine>

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

    QTimeLine * tl1;
    QTimeLine * tl2;

public slots:
    void setScaleAnimation(int s){
        qreal ns = qreal(s)/100;
        setScale(ns);
        if(ns <= 1)
        setOpacity(ns);
    }

    void deleteTimeLine(){
        delete tl1;
        tl2 = new QTimeLine(100, this);
        tl2->setFrameRange(120, 100);
        tl2->setUpdateInterval(16);
        tl2->setCurveShape(QTimeLine::EaseOutCurve);
        connect(tl2, SIGNAL(frameChanged(int)), this, SLOT(setScaleAnimation(int)));
        connect(tl2, SIGNAL(finished()), this, SLOT(deleteTimeLine2()));
        tl2->start();
    }

    void deleteTimeLine2(){
        delete tl2;
    }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // MAPPIN_H

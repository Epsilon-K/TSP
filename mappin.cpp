#include "mappin.h"

MapPin::MapPin(QPixmap * pix, int id)
{
    setPos(rand()%640 - 320, rand()%480 - 240);
    setPixmap(*pix);
    setFlag(QGraphicsItem::ItemClipsToShape);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setAcceptHoverEvents(true);
    origin.setX(int(width()/2));
    origin.setY(int(height()));
    //name = Utils::getRandomString(3);
    name = QString::number(id);

    // Label
    label = new QGraphicsTextItem(pointToString(pos()), this);
    QFont f;
    f.setWeight(2);
    f.setPointSize(10);
    label->setFont(f);
    label->setDefaultTextColor(QColor(200,255,170));

    // DropShadow
    QGraphicsDropShadowEffect * drop = new QGraphicsDropShadowEffect();
    drop->setColor(QColor("black"));
    drop->setBlurRadius(30);
    drop->setEnabled(true);
    drop->setOffset(0,0);
    setGraphicsEffect(drop);

    // Cache
    setCacheMode(this->DeviceCoordinateCache, this->boundingRect().size().toSize());
    label->setCacheMode(this->DeviceCoordinateCache, this->boundingRect().size().toSize());

    // Animation
    setTransformOriginPoint(width()/2,height()/2);
    tl1 = new QTimeLine(300, this);
    tl1->setFrameRange(20, 120);
    tl1->setUpdateInterval(16);
    tl1->setCurveShape(QTimeLine::EaseInCurve);
    connect(tl1, SIGNAL(frameChanged(int)), this, SLOT(setScaleAnimation(int)));
    connect(tl1, SIGNAL(finished()), this, SLOT(deleteTimeLine()));
    tl1->start();
    setScale(tl1->currentFrame()/tl1->endFrame());
    setOpacity(tl1->currentFrame()/tl1->endFrame());
}

qreal MapPin::width()
{
    return boundingRect().width();
}

qreal MapPin::height()
{
    return boundingRect().height();
}

QVariant MapPin::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged){
        label->setPlainText(name + "   " + pointToString(pos()));
        centerLabel();
    }
    return QGraphicsItem::itemChange(change, value);
}

void MapPin::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(QCursor(Qt::PointingHandCursor));
}

void MapPin::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(QCursor(Qt::ArrowCursor));
}

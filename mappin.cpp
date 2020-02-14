#include "mappin.h"

MapPin::MapPin(QPixmap * pix, int id)
{
    setPixmap(*pix);
    setFlag(QGraphicsItem::ItemClipsToShape);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
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
}

qreal MapPin::width()
{
    return boundingRect().width();
}

qreal MapPin::height()
{
    return boundingRect().height();
}

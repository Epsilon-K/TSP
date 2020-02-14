#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QBrush>
#include <QObject>
#include <QMatrix>
#include <QMouseEvent>
#include <QGraphicsPixmapItem>
#include <mappin.h>
#include <pathitem.h>


class EditorView : public QGraphicsView
{
    Q_OBJECT
public:
    EditorView(QWidget*& p);

    QVector<QGraphicsScene *> scenes;   // replace with a custome scene data type
    QColor bgColor, gridColor, resoultionColor;
    QSize gridSize, gameResolution;
    QRectF sceneSize;
    int activeScene = 0;
    double scaleLevel = 1;
    bool gridsVisible;
    QVector <QGraphicsLineItem*> gridLines;
    QVector <QGraphicsRectItem*> screenRects;

    QPixmap *mapBG;
    QGraphicsPixmapItem * pixmapBG;
    qreal gridOpacity = 0.3;


    // Functions

    void updateScale();
    void drawGrid();
    void setGridVisibility();

signals:
    void clicked();
    void signalMouseMoveEvent(QPoint point);
    void signalZoomLevelChanged();

public slots:
    void centering();

protected :
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
};

#endif // EDITORVIEW_H

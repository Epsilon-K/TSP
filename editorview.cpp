#include "editorview.h"

EditorView::EditorView(QWidget *&p)
{
    setParent(p);
    setInteractive(true);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
    setTransformationAnchor(AnchorUnderMouse);

    // this is a prefference!!
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QGraphicsScene *scn = new QGraphicsScene(this);
    scn->setItemIndexMethod(scn->NoIndex);

    // map
    mapBG = new QPixmap(":/map of khartoum.png");
    pixmapBG = new QGraphicsPixmapItem(*mapBG);
    scn->addItem(pixmapBG);
    pixmapBG->setOpacity(0);
    pixmapBG->setPos(-pixmapBG->boundingRect().width()/2, -pixmapBG->boundingRect().height()/2);

    bgColor = QColor("#1e1e27");
    scn->setBackgroundBrush(bgColor);
    gridSize = QSize(20,20);
    gridColor = QColor(255,100,100);
    sceneSize = QRectF(QPointF(-25000, -25000), QSizeF(50000,50000));
    scn->setSceneRect(sceneSize);
    resoultionColor = QColor(200,255,130);
    gameResolution = QSize(640,480);
    setScene(scn);
    gridsVisible = true;
    drawGrid();

    scenes.append(scn);
    QTimer::singleShot(300, this, SLOT(centering()));
}

void EditorView::updateScale()
{
    QMatrix matrix;
    matrix.scale(scaleLevel, scaleLevel);

    setMatrix(matrix);

    if(scaleLevel < 0.3 && gridsVisible){
        gridsVisible = false;
    }else if(scaleLevel >= 0.3 && (!gridsVisible)){
        gridsVisible = true;
    }
    setGridVisibility();

    emit signalZoomLevelChanged();
}

void EditorView::drawGrid()
{
    // draw the grid
    QPen pen;
    pen.setColor(gridColor);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(0);
        // draw Vertical Lines
    int st = int(sceneRect().left());
    int d = gridSize.width();
    while(st % d != 0) st++;
    for(double i = st; i < sceneRect().right(); i += gridSize.width()){
        QGraphicsLineItem * line = new QGraphicsLineItem(i, sceneRect().top(), i, sceneRect().bottom());
        line->setPen(pen);
        gridLines.append(line);
        line->setOpacity(gridOpacity);
        scene()->addItem(line);
    }
        // draw Horizontal Lines
    st = int(sceneRect().top());
    d = gridSize.height();
    while(st % d != 0) st++;
    for(double i = st; i < sceneRect().bottom(); i += gridSize.height()){
        QGraphicsLineItem * line = new QGraphicsLineItem(sceneRect().left(), i, sceneRect().right(), i);
        line->setPen(pen);
        gridLines.append(line);
        line->setOpacity(gridOpacity);
        scene()->addItem(line);
    }

    // set cross at (0,0)
    QGraphicsLineItem * hLine = new QGraphicsLineItem(-20, 0, 20, 0);
    hLine->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    QGraphicsLineItem * vLine = new QGraphicsLineItem(0, -20, 0, 20);
    vLine->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    pen.setWidth(1);
    pen.setColor(resoultionColor);
    hLine->setPen(pen); vLine->setPen(pen);
    vLine->setOpacity(gridOpacity);
    hLine->setOpacity(gridOpacity);
    scene()->addItem(hLine); scene()->addItem(vLine);

    // draw ScreenRects...
    double w = gameResolution.width();
    double h = gameResolution.height();
    double w2 = gameResolution.width()/2;
    double h2 = gameResolution.height()/2;

    int hst = int(sceneRect().left());
    d = int(w);
    while(hst % d != 0) hst++;
    int vst = int(sceneRect().top());
    d = int(h);
    pen.setWidth(0);
    while(vst % d != 0) vst++;
    for(double i = hst; i < sceneRect().right(); i += w){
        for(double j = vst; j < sceneRect().bottom(); j += h){
            QGraphicsRectItem * rect = new QGraphicsRectItem(i - w2, j - h2, w, h);
            rect->setBrush(Qt::NoBrush);
            rect->setPen(pen);
            rect->setOpacity(gridOpacity);
            scene()->addItem(rect);
            screenRects.append(rect);
        }
    }
}

void EditorView::setGridVisibility()
{
    for(int i = 0; i < gridLines.size(); i++){
        gridLines[i]->setVisible(gridsVisible);
    }
}

void EditorView::toggleOpenGL(bool checked)
{
    setViewport(checked ? new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
}

void EditorView::centering()
{
    centerOn(QPointF(0,0));
}

void EditorView::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        setDragMode(QGraphicsView::ScrollHandDrag);
    }else if(e->button() == Qt::RightButton){
        setDragMode(QGraphicsView::RubberBandDrag);
    }

    QGraphicsView::mousePressEvent(e);
}

void EditorView::mouseReleaseEvent(QMouseEvent *e)
{
    QGraphicsView::mouseReleaseEvent(e);
    setDragMode(QGraphicsView::NoDrag);

    if(scene()->selectedItems().size() != 1 && e->button() == Qt::LeftButton){
        emit(clicked());
    }
    else if(scene()->selectedItems().isEmpty() && e->button() == Qt::RightButton){
        // Context Menu Please
    }
}

void EditorView::wheelEvent(QWheelEvent *e)
{
    if (e->delta() > 0)
        scaleLevel += scaleLevel/6;
    else
        scaleLevel -= scaleLevel/6;

    Utils::clamp(&scaleLevel, 0.05, 30);
    updateScale();
    e->accept();
}

void EditorView::mouseMoveEvent(QMouseEvent *e)
{
    emit signalMouseMoveEvent(e->pos());
    QGraphicsView::mouseMoveEvent(e);
}

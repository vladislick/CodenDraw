#ifndef CUSTOMGRAPHICSSCENE_H
#define CUSTOMGRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>

class CustomGraphicsScene : public QGraphicsScene
{
public:
    CustomGraphicsScene();
    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);

    bool        isMousePressed = 0;
    QPointF     mouseStartPos;
    QPointF     mouseCurrentPos;
    int         mouseWheel;

    int wheelChanged();
};

#endif // CUSTOMGRAPHICSSCENE_H

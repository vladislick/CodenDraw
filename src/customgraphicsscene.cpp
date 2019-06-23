#include "customgraphicsscene.h"

CustomGraphicsScene::CustomGraphicsScene() {}

void CustomGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent) {
    if (isMousePressed) mouseCurrentPos = mouseEvent->scenePos();
    else mouseStartPos = mouseEvent->scenePos();
}

void CustomGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    isMousePressed = true;
}

void CustomGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    isMousePressed = false;
}

void CustomGraphicsScene::wheelEvent(QGraphicsSceneWheelEvent *event) {
    mouseWheel = event->delta();
}

int CustomGraphicsScene::wheelChanged() {
    int tmp = mouseWheel;
    mouseWheel = 0;
    return tmp;
}

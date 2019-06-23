#include "mainscene.h"

MainScene::MainScene() : QGraphicsScene() { }

/// Set main color
void MainScene::setMainColor(const QColor &color) {
    mainColor = color;
}

/// Set background color
void MainScene::setBackgroundColor(const QColor &color) {
    mainBackgroundColor = color;
}

/// Set scale value
void MainScene::setScale(double scale) {
    mainScale = scale;
}

/// Get scale value
double MainScene::scale() {
    return mainScale;
}

/// Set physical display ppi
void MainScene::setPPI(const QSizeF &size) {
    mainPPI = size;
}

/// Set empty space around table (millimeters)
void MainScene::setEmptySpace(const QSize &size) {
    mainTableEmptySpace = size;
}

/// Get empty space around table (millimeters)
QSize& MainScene::emptySpace() {
    return mainTableEmptySpace;
}

/// Set table size
void MainScene::setTable(const QSize &table) {
    mainTable = table;
}

/// Draw rulers around main table
void MainScene::drawRulers(const QColor &color) {

}

/// Draw preview image by GCode
void MainScene::drawPreview(const QColor &color) {

}

/// Draw main table
void MainScene::drawTable(const QColor &color) {
    QPen pen(color);
    pen.setWidth(1);

    QGraphicsScene::addRect(mainOffset.width() * scaleFactor, mainOffset.height() * scaleFactor, mainTable.width() * scaleFactor, mainTable.height() * scaleFactor, pen);
}

/// Draw main table shadow
void MainScene::drawTableShadow(int widthInPixels, const QColor &color) {

}

/// Zoom preview
void MainScene::zoom(double scale, const QPointF &position) {

}

/// Get current mouse position on scene
QPoint& MainScene::mousePos() {

}

/// Get start mouse position when button is pressed
QPoint& MainScene::mouseStartPos() {

}

/// Get current mouse button status
bool MainScene::mousePressed() {

}

void MainScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)  {

}

void MainScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {

}

void MainScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

}

void MainScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event) {
    if (event->mimeData()->hasUrls()) {
        for (int i = 0; i < views().count(); i++) views()[i]->setVisible(false);
    }
}

void MainScene::wheelEvent(QGraphicsSceneWheelEvent *event) {

}

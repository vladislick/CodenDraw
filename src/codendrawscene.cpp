#include "codendrawscene.h"

CodenDrawScene::CodenDrawScene() : QGraphicsScene() { }

/// Set main color
void CodenDrawScene::setMainColor(const QColor &color) {
    mainColor = color;
}

/// Set background color
void CodenDrawScene::setBackgroundColor(const QColor &color) {
    mainBackgroundColor = color;
}

/// Set scale value
void CodenDrawScene::setScale(double scale)
{
    mainScale = scale;
    scaleFactor = mainPPI / 25.4 * mainScale;
}

/// Get scale value
double CodenDrawScene::scale()
{
    return mainScale;
}

void CodenDrawScene::setOffset(const QSizeF &size)
{
    mainOffset = size;
}

QSizeF& CodenDrawScene::offset()
{
    return mainOffset;
}

/// Set physical display ppi
void CodenDrawScene::setPPI(double ppi)
{
    mainPPI = ppi;
}

/// Set empty space around table (millimeters)
void CodenDrawScene::setEmptySpace(const QSize &size)
{
    mainTableEmptySpace = size;
}

/// Get empty space around table (millimeters)
QSize& CodenDrawScene::emptySpace()
{
    return mainTableEmptySpace;
}

/// Set table size
void CodenDrawScene::setTable(const QSize &table)
{
    mainTable = table;
}

/// Draw rulers around main table
void CodenDrawScene::drawRulers(const QColor &color)
{

}

/// Draw preview image by GCode
void CodenDrawScene::drawPreview(const QColor &color)
{

}

/// Draw main table
void CodenDrawScene::drawTable(const QColor &color)
{
    QPen pen(color);
    pen.setWidth(1);

    QGraphicsScene::addRect(mainOffset.width() * scaleFactor, mainOffset.height() * scaleFactor, mainTable.width() * scaleFactor, mainTable.height() * scaleFactor, pen);
}

/// Draw main table shadow
void CodenDrawScene::drawTableShadow(int widthInPixels, const QColor &color)
{
    QPen pen(color);
    pen.setWidth(widthInPixels);

    QGraphicsScene::addLine(scaleFactor * (mainOffset.width() + mainTable.width()) + pen.width() / 2,
                            scaleFactor * mainOffset.height() + pen.width() + (pen.width() >> 1),
                            scaleFactor * (mainOffset.width() + mainTable.width()) + pen.width() / 2,
                            scaleFactor * (mainOffset.height() + mainTable.height()) + pen.width() / 2, pen);
    QGraphicsScene::addLine(scaleFactor * (mainOffset.width() + mainTable.width()) + pen.width() / 2,
                            scaleFactor * (mainOffset.height() + mainTable.height()) + pen.width() / 2,
                            scaleFactor * mainOffset.width() + pen.width() + (pen.width() >> 1),
                            scaleFactor * (mainOffset.height() + mainTable.height()) + pen.width() / 2, pen);

}

/// Zoom preview
void CodenDrawScene::zoom(double scale, const QPointF &position)
{

}

/// Get current mouse position on scene
QPoint& CodenDrawScene::mousePos()
{

}

/// Get start mouse position when button is pressed
QPoint& CodenDrawScene::mouseStartPos()
{

}

/// Get current mouse button status
bool CodenDrawScene::mousePressed()
{

}

void CodenDrawScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

}

void CodenDrawScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{


}

void CodenDrawScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

void CodenDrawScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        for (int i = 0; i < views().count(); i++) views()[i]->setVisible(false);
    }
}

void CodenDrawScene::wheelEvent(QGraphicsSceneWheelEvent *event) {

}

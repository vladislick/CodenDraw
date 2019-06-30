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
    this->zoom(scale, QPointF(QGraphicsScene::width() / 2, QGraphicsScene::height() / 2));
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
    dx = (QGraphicsScene::width() - scaleFactor * mainTable.width()) / 2;
    dy = (QGraphicsScene::height() - scaleFactor * mainTable.height()) / 2;
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

    QGraphicsScene::addRect(dx, dy, mainTable.width() * scaleFactor, mainTable.height() * scaleFactor, pen);
}

/// Draw main table shadow
void CodenDrawScene::drawTableShadow(int widthInPixels, const QColor &color)
{
    QPen pen(color);
    pen.setWidth(widthInPixels);

    QGraphicsScene::addLine(dx + scaleFactor * mainTable.width() + pen.width() / 2,
                            dy + pen.width() + (pen.width() >> 1),
                            dx + scaleFactor * mainTable.width() + pen.width() / 2,
                            dy + scaleFactor * mainTable.height() + pen.width() / 2, pen);
    QGraphicsScene::addLine(dx + scaleFactor * mainTable.width() + pen.width() / 2,
                            dy + scaleFactor * mainTable.height() + pen.width() / 2,
                            dx + pen.width() + (pen.width() >> 1),
                            dy + scaleFactor * mainTable.height() + pen.width() / 2, pen);

}

/// Zoom preview
void CodenDrawScene::zoom(double scale, const QPointF &position)
{
    mainScale = scale;

    double x = (position.x() - dx) / scaleFactor; // mm
    double y = (position.y() - dy) / scaleFactor; // mm

    scaleFactor = mainPPI / 25.4 * mainScale;

    dx = position.x() - scaleFactor * x;
    dy = position.y() - scaleFactor * y;
}

/// Get current mouse position on scene
QPointF& CodenDrawScene::mousePos()
{
    return mouseCurrent;
}

void CodenDrawScene::setOffsetX(double value)
{
    dx = value;
}

double CodenDrawScene::offsetX()
{
    return dx;
}

void CodenDrawScene::setOffsetY(double value)
{
    dy = value;
}

double CodenDrawScene::offsetY()
{
    return dy;
}

void CodenDrawScene::changeOffsetX(double value)
{
    dx += value;
}

void CodenDrawScene::changeOffsetY(double value)
{
    dy += value;
}

/// Get start mouse position when button is pressed
QPointF& CodenDrawScene::mouseStartPos()
{
    return mouseStart;
}

/// Get current mouse button status
bool CodenDrawScene::mousePressed()
{
    return isMousePressed;
}

void CodenDrawScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (!isMousePressed)
        mouseStart = mouseEvent->scenePos();

    mouseCurrent = mouseEvent->scenePos();
}

void CodenDrawScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    isMousePressed = true;
    for (const auto &viewer : QGraphicsScene::views())
        viewer->setCursor(Qt::SizeAllCursor);
}

void CodenDrawScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    isMousePressed = false;
    for (const auto &viewer : QGraphicsScene::views())
        viewer->setCursor(Qt::ArrowCursor);
}

void CodenDrawScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{

}

void CodenDrawScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if (event->delta() > 0)
        mainSpinBox->stepUp();
    else if (event->delta() < 0)
        mainSpinBox->stepDown();
}

void CodenDrawScene::setScaleSpinBox(QSpinBox *spinBox)
{
    mainSpinBox = spinBox;
}

QSpinBox* CodenDrawScene::scaleSpinBox()
{
    return  mainSpinBox;
}

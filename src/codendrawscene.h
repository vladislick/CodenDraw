#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>
#include <QPen>
#include <QMimeData>
#include <QSize>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>


class CodenDrawScene : public QGraphicsScene
{
public:
    CodenDrawScene();

    /// Set scale value
    void setScale(double scale);

    /// Get scale value
    double scale();

    /// Set empty space around table (millimeters)
    void setEmptySpace(const QSize &size);

    /// Get empty space around table (millimeters)
    QSize& emptySpace();

    void setOffset(const QSizeF &size);

    QSizeF& offset();

    /// Set table size
    void setTable(const QSize &table);

    /// Draw rulers around main table
    void drawRulers(const QColor &color);

    /// Draw preview image by GCode
    void drawPreview(const QColor &color);

    /// Draw main table
    void drawTable(const QColor &color);

    /// Draw main table shadow
    void drawTableShadow(int widthInPixels, const QColor &color);

    /// Set main color
    void setMainColor(const QColor &color);

    /// Set background color
    void setBackgroundColor(const QColor &color);

    /// Set physical display ppi
    void setPPI(double ppi);

    /// Zoom preview
    void zoom(double scale, const QPointF &position);

    /// Get current mouse position on scene
    QPoint& mousePos();

    /// Get start mouse position when button is pressed
    QPoint& mouseStartPos();

    /// Get current mouse button status
    bool    mousePressed();

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);

    // In percent
    double      mainScale   = 1;
    // In pixels
    double      scaleFactor;
    QSize       mainTable;
    QSize       mainTableEmptySpace;
    QSizeF      mainOffset;
    bool        isMousePressed = 0;
    QPoint      mouseStart;
    QPoint      mouseCurrent;
    QColor      mainColor;
    QColor      mainBackgroundColor;
    double      mainPPI;
};

#endif // MAINSCENE_H

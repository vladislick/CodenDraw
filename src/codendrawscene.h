#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>
#include <QPen>
#include <QMimeData>
#include <QSize>
#include <QSpinBox>
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

    void setScaleSpinBox(QSpinBox *spinBox);

    QSpinBox* scaleSpinBox();

    void setOffsetX(double value);

    double offsetX();

    void setOffsetY(double value);

    double offsetY();

    void changeOffsetX(double value);

    void changeOffsetY(double value);

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
    QPointF& mousePos();

    /// Get start mouse position when button is pressed
    QPointF& mouseStartPos();

    /// Get current mouse button status
    bool    mousePressed();

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);

    QSpinBox    *mainSpinBox;

    // In percent
    double      mainScale   = 1;
    // In pixels
    double      scaleFactor = 1;
    QSize       mainTable;
    QSize       mainTableEmptySpace;
    QSizeF      mainOffset;
    bool        isMousePressed = 0;
    QPointF     mouseStart;
    QPointF     mouseCurrent;
    QColor      mainColor;
    QColor      mainBackgroundColor;
    double      mainPPI;
    double      dx, dy;
};

#endif // MAINSCENE_H

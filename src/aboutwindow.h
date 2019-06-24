#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include "mainwindow.h"
#include "aboutlicence.h"

#include <QtSvg/QSvgRenderer>
#include <QPainter>
#include <QDialog>
#include <QImage>
#include <QDir>
#include <QColor>
#include <QPixmap>
#include <QFont>

#define ProjectLink "https://github.com/vladislick/CodenDraw"

namespace Ui {
class AboutWindow;
}

class AboutWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = nullptr);
    void setAppDir(const QString &path);
    void updatePixmaps();
    ~AboutWindow();

private slots:
    //!Нажатие кнопки ОК
    void on_pushButtonOk_clicked();

    void on_labelLicence_linkActivated(const QString &link);

private:
    Ui::AboutWindow *ui;

    QDir *iconsDirectory;
};

#endif // ABOUTWINDOW_H

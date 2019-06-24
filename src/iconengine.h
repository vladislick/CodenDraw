#ifndef ICON_H
#define ICON_H

#include <QStringList>
#include <QString>
#include <QImage>
#include <QSvgRenderer>
#include <QFileInfoList>
#include <QFileInfo>
#include <QPixmap>
#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QIcon>
#include <QDir>

#define  IconPixelSize  32

class IconEngine
{
public:
    enum Style { NoStyle, SystemColor, Dark, Light };

    IconEngine();

    /// Get directory path that has icons for dark theme
    QString darkPath();
    /// Get directory path that has icons for light theme
    QString lightPath();
    /// Get directory path that has icons
    QString path();
    /// Set path to the icon directory
    void    setPath(QString path);
    /// Set path to the icons for dark theme
    void    setDarkPath(QString path);
    /// Set path to the icons for light theme
    void    setLightPath(QString path);
    /// Set icon style
    void    setStyle(Style style);
    /// Get icon style
    Style   style();
    /// Get icon from icons directory
    QIcon   getByName(QString iconName);
    /// Find an icon in the directory
    QString findByName(const QString &fileName, const QString &directory);

private:
    QIcon       *mainIcon;
    QDir        *darkDirectory;
    QDir        *lightDirectory;
    QDir        *iconDirectory;
    Style   iconStyle = Style::NoStyle;

};

#endif // ICON_H

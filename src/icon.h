#ifndef ICON_H
#define ICON_H

#include <QStringList>
#include <QString>
#include <QFileInfoList>
#include <QFileInfo>
#include <QPixmap>
#include <QDebug>
#include <QFile>
#include <QIcon>
#include <QDir>

#define  IconWidth  128
#define  IconHeight 128

class IconEngine
{
public:
    enum ThemeStyle { Dark, Light };

    IconEngine();

    QString darkPath();
    QString lightPath();
    void    setPath(QString path);
    void    setDarkPath(QString path);
    void    setLightPath(QString path);
    void    setThemeStyle(ThemeStyle style);
    QIcon   getByName(QString name);
    QString findByName(const QString &fileName, const QString &directory);

private:
    QIcon       *mainIcon;
    QDir        *darkThemeDir;
    QDir        *lightThemeDir;
    QFile       *mainIconFile;
    ThemeStyle   mainStyle = ThemeStyle::Light;

};

#endif // ICON_H

#include "icon.h"

IconEngine::IconEngine()
{
    mainIcon = new QIcon;
    darkThemeDir = new QDir;
    lightThemeDir = new QDir;
    mainIconFile = new QFile;
}

void IconEngine::setPath(QString path)
{
    darkThemeDir->setPath(path);
    lightThemeDir->setPath(path);
}

void IconEngine::setDarkPath(QString path)
{
    darkThemeDir->setPath(path);
}

void IconEngine::setLightPath(QString path)
{
    lightThemeDir->setPath(path);
}

void IconEngine::setThemeStyle(ThemeStyle style)
{
    mainStyle = style;
}

QString IconEngine::darkPath()
{
    return darkThemeDir->path();
}

QString IconEngine::lightPath()
{
    return lightThemeDir->path();
}

QIcon IconEngine::getByName(QString name)
{
    QFileInfo file;

    if (mainStyle == ThemeStyle::Dark)
        file.setFile(findByName(name, darkThemeDir->path()));
    else
        file.setFile(findByName(name, lightThemeDir->path()));

    if (file.filePath() != "")
    {
        if (file.suffix() == "jpg" || file.suffix() == "png")
        {
            mainIcon->addFile(file.filePath());
        }
        else if (file.suffix() == "svg")
        {
            QPixmap pixmap(file.filePath());
            pixmap = pixmap.scaled(IconWidth, IconHeight, Qt::KeepAspectRatioByExpanding, Qt::FastTransformation);
            mainIcon->addPixmap(pixmap);
        }

        return *mainIcon;
    }
    else return QIcon();
}

QString IconEngine::findByName(const QString &fileName, const QString &directory)
{
    QDir dir(directory);
    QFileInfo file;
    QString tmp;

    // Try to find icon in the directory
    for (int i = 0; i < dir.entryInfoList().count(); i++)
    {
        file = dir.entryInfoList()[i];
        if (file.baseName() == fileName) return file.filePath();
    }

    // Try to find icon in other directories
    for (int i = 0; i < dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot).count(); i++)
    {
        QDir _dir(dir.path() + '/' + dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot, QDir::Name)[i]);
        tmp = findByName(fileName, _dir.path());
        if (tmp != "") return tmp;
    }

    // If cannot find this icon, return empty string
    return QString();
}

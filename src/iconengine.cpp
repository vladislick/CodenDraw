#include "iconengine.h"

IconEngine::IconEngine()
{
    mainIcon = new QIcon;
    darkDirectory = new QDir;
    lightDirectory = new QDir;
    iconDirectory = new QDir;
}

/// Set path to the icon directory
void IconEngine::setPath(QString path)
{
    iconDirectory->setPath(path);
    darkDirectory->setPath(path);
    lightDirectory->setPath(path);
}

/// Set path to the icons for dark theme
void IconEngine::setDarkPath(QString path)
{
    darkDirectory->setPath(path);
}

/// Set path to the icons for light theme
void IconEngine::setLightPath(QString path)
{
    lightDirectory->setPath(path);
}

/// Set icon style
void IconEngine::setThemeStyle(ThemeStyle style)
{
    iconStyle = style;
}

/// Get directory path that has icons
QString IconEngine::path() {
    return iconDirectory->path();
}

/// Get directory path that has icons for dark theme
QString IconEngine::darkPath()
{
    return darkDirectory->path();
}

/// Get directory path that has icons for light theme
QString IconEngine::lightPath()
{
    return lightDirectory->path();
}

/// Get icon from icons directory
QIcon IconEngine::getByName(QString iconName)
{
    QFileInfo file;

    // Try to find needed icon
    if (iconStyle == ThemeStyle::Dark)
        file.setFile(findByName(iconName, darkDirectory->path()));
    else if (iconStyle == ThemeStyle::Light)
        file.setFile(findByName(iconName, lightDirectory->path()));
    else
        file.setFile(findByName(iconName, iconDirectory->path()));

    // If the icon is found
    if (file.filePath() != "")
    {
        if (file.suffix() == "png")
        {
            mainIcon->addFile(file.filePath());
            mainIcon->addFile(file.filePath(), QSize(), QIcon::Selected);
        }
        else if (file.suffix() == "svg")
        {
            QPixmap pixmap(file.filePath());
            pixmap = pixmap.scaled(IconWidth, IconHeight, Qt::KeepAspectRatioByExpanding, Qt::FastTransformation);
            mainIcon->addPixmap(pixmap);
            mainIcon->addPixmap(pixmap, QIcon::Selected);
        }

        if (iconStyle == ThemeStyle::SystemColor)
        {
            QColor textColor      = QLabel().palette().color(QPalette::WindowText),
                   highlightColor = QLabel().palette().color(QPalette::HighlightedText);

            QImage  image(file.filePath());
            image = image.scaledToHeight(IconHeight, Qt::FastTransformation);

            for (int x = 0; x < image.width(); x++)
                for (int y = 0; y < image.height(); y++)
                    if (qAlpha(image.pixel(x, y)) > 0) image.setPixel(x, y, qRgba(textColor.red(), textColor.green(), textColor.blue(), qAlpha(image.pixel(x, y))));

            image.save(file.dir().path() + '/' + "tmp.png");
            mainIcon->addFile(file.dir().path() + '/' + "tmp.png");

            for (int x = 0; x < image.width(); x++)
                for (int y = 0; y < image.height(); y++)
                    if (qAlpha(image.pixel(x, y)) > 0) image.setPixel(x, y, qRgba(highlightColor.red(), highlightColor.green(), highlightColor.blue(), qAlpha(image.pixel(x, y))));

            image.save(file.dir().path() + '/' + "tmp.png");
            mainIcon->addFile(file.dir().path() + '/' + "tmp.png", QSize(), QIcon::Selected);
        }

        return *mainIcon;
    }
    else
        return QIcon();
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

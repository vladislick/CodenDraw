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
void IconEngine::setStyle(Style style)
{
    iconStyle = style;
}

/// Get icon style
IconEngine::Style IconEngine::style()
{
    return iconStyle;
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
    if (iconStyle == Style::Dark)
        file.setFile(findByName(iconName, darkDirectory->path()));
    else if (iconStyle == Style::Light)
        file.setFile(findByName(iconName, lightDirectory->path()));
    else
        file.setFile(findByName(iconName, iconDirectory->path()));

    // If the icon is found
    if (file.filePath() != "")
    {
        QImage  imageNormal(IconPixelSize, IconPixelSize, QImage::Format_ARGB32);
        QImage  imageSelected(IconPixelSize, IconPixelSize, QImage::Format_ARGB32);

        // Set icons image
        if (file.suffix() == "png")
        {
            imageNormal.load(file.filePath());
            imageSelected.load(file.filePath());
        }
        else if (file.suffix() == "svg")
        {
            // Create needed objects to render SVG file
            QSvgRenderer renderer(file.filePath());
            QPainter painterNormal(&imageNormal);
            QPainter painterSelected(&imageSelected);

            // Clear images
            imageNormal.fill(0x00000000);
            imageSelected.fill(0x00000000);

            // Render icon image
            renderer.render(&painterNormal);
            renderer.render(&painterSelected);
        }

        // Change icons color to system
        if (iconStyle == Style::SystemColor)
            for (int x = 0; x < imageNormal.width(); x++)
                for (int y = 0; y < imageNormal.height(); y++)
                {
                    if (qAlpha(imageNormal.pixel(x, y)) > 0)
                        imageNormal.setPixel(x, y, (QLabel().palette().color(QPalette::WindowText).rgb() & 0x00ffffff) + (imageNormal.pixel(x, y) & 0xff000000));
                    if (qAlpha(imageSelected.pixel(x, y)) > 0)
                        imageSelected.setPixel(x, y, (QLabel().palette().color(QPalette::HighlightedText).rgb() & 0x00ffffff) + (imageSelected.pixel(x, y) & 0xff000000));
                }

        // Set fixed icons
        mainIcon->addPixmap(QPixmap::fromImage(imageNormal), QIcon::Normal);
        mainIcon->addPixmap(QPixmap::fromImage(imageSelected), QIcon::Selected);

        return *mainIcon;
    }
    else return QIcon(); // Return an empty icon if cannot find needed icon
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

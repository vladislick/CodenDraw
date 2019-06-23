#ifndef CONFIG_H
#define CONFIG_H

#include <QByteArray>
#include <QDateTime>
#include <QFileInfo>
#include <QFile>
#include <QString>
#include <QList>
#include <QDir>

class ConfigFile : public QFileInfo
{

public:
    /// Errors list
    enum ConfigError { NoError, ComplianceError, FileDoesNotExistError, PermissionError };

    ConfigFile();

    explicit ConfigFile(const QString &filePath);

    /// Read file again (readed with no errors return 0; cannot open return -1; readed with errors return 1)
    ConfigError update();

    /// Number of parameters
    int count();

    /// Set the file
    void setFile(const QString &filePath);

    /// Set the file
    void setFile(const QFile &file);

    /// Set the file
    void setFile(const QDir &dir, const QString &file);

    /// Get parameter by index
    QString getParameter(int index) const;

    /// Get parameter by index
    QString parameter(int index) const;

    /// Get parameter value by index
    QString getValue(int index) const;

    /// Get parameter value by index
    QString value(int index) const;

    /// Add a line in file
    bool addLine(const QString &line);

    /// Add a parameter with its value in file
    bool addParameter(const QString newParameter, const QString newValue);

    /// Add a parameter with its value in file
    bool addParameter(const QString newParameter, int newValue);

    /// Add a comment in file ("# comment")
    bool addComment(const QString comment);

    /// Add a title in file ("[ Title ]")
    bool addTitle(const QString title);

    /// Add an empty line in file
    bool addEmptyLine();

    /// Remove all data in file
    void clearFile(bool printWaterMark = true);

protected:

    QFile           *mainFile;
    QList<QString>  *mainParameter;
    QList<QString>  *mainValue;
};

#endif // CONFIG_H

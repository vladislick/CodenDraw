#include "configfile.h"

ConfigFile::ConfigFile() : QFileInfo() {
    mainFile = new QFile();
}

ConfigFile::ConfigFile(const QString &file) : QFileInfo(file) {
    mainFile = new QFile(file);
    update();
}

/// Set the file
void ConfigFile::setFile(const QString &file) {
    QFileInfo::setFile(file);
    mainFile->setFileName(file);
    update();
}

/// Set the file
void ConfigFile::setFile(const QFile &file) {
    QFileInfo::setFile(file);
    mainFile->setFileName(QFileInfo::filePath());
    update();
}

/// Set the file
void ConfigFile::setFile(const QDir &dir, const QString &file) {
    QFileInfo::setFile(dir, file);
    mainFile->setFileName(QFileInfo::filePath());
    update();
}

/// Read file again (if the file was read fine, returns true)
ConfigFile::ConfigError ConfigFile::update() {

    mainParameter->clear();
    mainValue->clear();

    // If file doesn't exist
    if (!mainFile->exists()) return FileDoesNotExistError;

    // If cannot open
    if (!mainFile->open(QFile::ReadOnly)) {
        mainFile->close();
        return PermissionError;
    }

    // Version 1.0

    // Read all data from file as byte array
    QByteArray array(mainFile->readAll());
    // Read byte array as string
    QString file(array.toStdString().c_str());

    QString temp;

    // Is it comment line
    bool isCommentLine = false;
    // Was the parameter previously specified
    bool isParameterSet = false;

    // Read all file characters
    for (int i = 0; i < file.size() + 1; i++) {
        // If it is a comment
        if (file[i] == ';' || file[i] == '#' || file[i] == '[') {
            isCommentLine = true;
        }
        // If its is an equal sign
        else if (file[i] == '=') {
            // Ignore if it is a comment
            if (isCommentLine) continue;
            isParameterSet = true;
            // If before the equal sign is empty, skip
            if (temp.isEmpty()) continue;
            mainParameter->append(temp);
            temp.clear();
        }
        else if (file[i] == ' ') {
            // Ignore if it is a comment
            if (isCommentLine) continue;
            // If parameter is set and before the gap we have a word
            if (isParameterSet && !temp.isEmpty()) {
                mainValue->append(temp);
                temp.clear();
                isParameterSet = false;
            }
        }
        else if (file[i] == '\n' || file[i] == '\0') {
            isCommentLine = false;
            // If parameter is set and we have a word
            if (isParameterSet && !temp.isEmpty()) {
                mainValue->append(temp);
                temp.clear();
                isParameterSet = false;
            }
        }
        // If it isn't a special sign, just add it to temp variable
        else if (!isCommentLine) temp += file[i];
    }

    mainFile->close();

    if (mainParameter->count() != mainValue->count())   return ComplianceError;
    else                                                return NoError;
}

/// Number of parameters
int ConfigFile::count() {
    return mainParameter->count();
}

/// Get parameter by index
QString ConfigFile::getParameter(int index) const {
    if (index < mainParameter->count()) return mainParameter->at(index);
    else                                return QString();
}

/// Get parameter by index
QString ConfigFile::parameter(int index) const {
    return getParameter(index);
}

/// Get parameter value by index
QString ConfigFile::getValue(int index) const {
    if (index < mainValue->count()) return mainValue->at(index);
    else                            return QString();
}

/// Get parameter value by index
QString ConfigFile::value(int index) const {
    return getValue(index);
}

/// Add a line in file
bool ConfigFile::addLine(const QString& line) {
    QByteArray array;

    // If we can open the file, read and remember it
    if (mainFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        array.append(mainFile->readAll());
        mainFile->close();
    }

    // Write new data to the file
    if (mainFile->open(QFile::WriteOnly)) {
        array.append(line + '\n');
        mainFile->write(array);
        mainFile->close();
        return true;
    }
    else return false;
}

/// Add a parameter with its value in file
bool ConfigFile::addParameter(const QString newParameter, const QString newValue) {
    return addLine(newParameter + " = " + newValue);
}

/// Add a parameter with its value in file
bool ConfigFile::addParameter(const QString newParameter, int newValue) {
    return addLine(newParameter + " = " + QString::number(newValue));
}

/// Add a comment in file
bool ConfigFile::addComment(const QString comment) {
    return addLine("# " + comment);
}

/// Add a title in file ("[ Title ]")
bool ConfigFile::addTitle(const QString title) {
    return addLine("[" + title + "]");
}

/// Add an empty line in file
bool ConfigFile::addEmptyLine() {
    return addLine("");
}

/// Remove all data in file
void ConfigFile::clearFile(bool printWaterMark) {
    mainFile->remove();
    mainFile->open(QFile::WriteOnly);

    if (printWaterMark) {
        QByteArray array;
        array = QString("# Created by qInkDraw App [" + QDateTime::currentDateTime().toString() + "]\n\n").toLocal8Bit();
        mainFile->write(array);
    }

    mainFile->close();
}

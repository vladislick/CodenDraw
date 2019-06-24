#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Include main Qt headers
#include <QDir>
#include <QDebug>
#include <QLabel>
#include <QMainWindow>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QScreen>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QTime>
#include <QTimer>
#include <QSize>
#include <QSizeF>
#include <QUrl>
#include <QPointF>
#include <QLineF>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QStandardPaths>
#include <QPainter>
#include <QFile>
#include <QFileInfo>
#include <QTextCodec>
#include <QStandardPaths>
#include <QElapsedTimer>
#include <QWheelEvent>
#include <QMimeData>
#include <QSettings>
#include <QTextDecoder>

// Include .h headers
#include "customgraphicsscene.h"
#include "codeeditor.h"
#include "configfile.h"
#include "iconengine.h"
#include "mainscene.h"

/// Application version
#define AppVersion "1.0-dev"

/// Maximum number of files in the "Recent Files" menu
#define RecentFilesMax 10

/// Maximum number of frames per second (QGraphicsView update speed)
#define MaxFPS  60

/// File that contains main window state
#define mainWindowState "mainwindow.state"

/// The name of the directory in which all icons are stored (like ~/.config/CodenDraw/~mainAppIcons~)
#define mainAppIcons "icons"

/// The name of the directory in which the spare icons are stored (like ~/.config/CodenDraw/~mainAppIcons~/~mainIcons~)
#define mainIcons "Papirus"

// If it's Linux
#ifdef Q_OS_LINUX
    #define SYSTEM  0
#endif

// If it's Windows
#ifdef Q_OS_WIN32
    #define SYSTEM  1
#endif

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum Message { Successfully, Information, Warning, Critical };

    explicit MainWindow(QWidget *parent = nullptr);

    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent* event);

    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_actionQuit_triggered();
    void on_actionAbout_triggered();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionSaveAs_triggered();
    void on_actionRefresh_triggered();
    void currentTimeChanged();

    /// Open the recent file that caused this slot
    void openRecentFile();
    /// Clear recent files list
    void clearRecentFiles();

    void codeChanged();
    void clearOutput();

    void on_actionAntialiasing_triggered(bool checked);

    void on_horizontalScrollBar_sliderMoved(int position);

    void on_verticalScrollBar_sliderMoved(int position);

private:

    /// Open file
    void openFile(const QString &filePath, bool silentMode = false);

    /// Save file
    void saveFile(const QString& filePath);

    /// Open raster picture
    bool openRasterPicture(const QFileInfo &file);

    /// Open vector picture
    bool openVectorPicture(const QFileInfo &file);

    /// Open text file
    bool openTextFile(const QFileInfo &file);

    /// Send message
    void sendMessage(QString status, Message message = Message::Information);

    /// Add the file to recent files menu
    void addRecentFile(const QString &filePath);

    /// Save recent files list to the settings file
    void saveRecentFilesList();

    /// Load recent files list from the settings file
    void loadRecentFilesList();

    /// Save all window data to files
    void saveWindowConfiguration();

    /// Load all window data from files
    void loadWindowConfiguration();

    /// Update user interface
    void updateGUI();

    Ui::MainWindow  *ui;
    CodeEditor      *code;
    QHBoxLayout     *statusBarLayout;
    MainScene       *mainScene;
    QLabel          *currentStatusLabel;
    QLabel          *currentTimeLabel;
    QTimer          *currentTimeTimer;
    QList<QAction*> recentFiles;
    QColor          *windowColor;
    QColor          *textColor;
    QColor          *highlightColor;
    IconEngine      *mainIcon;

    QSize           *mainTable;
    QSize           *mainEngineSteps;
    QPointF         *mainView;
    QSizeF          *mainStepSize;
    double          mainHorizontalSlider;
    double          mainVerticalSlider;

    /// Info about file that is open
    QFileInfo   *currentFile;
    /// Main application directory
    QDir        *applicationDirectory;
    ///
    QFile       *mainWindowStateFile;
    /// Application settings file
    QSettings   *mainSettingsFile;
};

#endif // MAINWINDOW_H

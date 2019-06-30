#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/* INCLUDE MAIN QT HEADERS */

// For GUI
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QPainter>
#include <QPushButton>
#include <QToolButton>
#include <QToolBar>
#include <QComboBox>

// For backend
#include <QDir>
#include <QDebug>
#include <QMainWindow>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QMessageBox>
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
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QElapsedTimer>
#include <QWheelEvent>
#include <QMimeData>
#include <QSettings>
#include <QScreen>

/* INCLUDE CUSTOM HEADERS */

#include "customgraphicsscene.h"
#include "codeeditor.h"
#include "configfile.h"
#include "iconengine.h"
#include "codendrawscene.h"

/* DEFINE SOME VALUES */

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

    void actionOpenTriggered();
    void actionSaveTriggered();
    void actionSaveAsTriggered();
    void actionCloseTriggered();
    void actionQuitTriggered();

    void actionDeviceTriggered(bool);

    void updateDevicesList();

    void actionAboutTriggered();

    /// Open the recent file that caused this slot
    void actionRecentTriggered();
    /// Clear recent files list
    void actionClearRecentTriggered();

    void toolBarOrientationChanged(Qt::Orientation);

    void previewUpdate();

    void currentTimeChanged();
    void codeChanged();
    void clearOutput();

    void on_actionAntialiasing_triggered(bool checked);

    void on_horizontalScrollBar_sliderMoved(int position);

    void on_verticalScrollBar_sliderMoved(int position);

    void on_spinBox_valueChanged(int arg1);

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

    void updateDevices();

    Ui::MainWindow  *ui;
    CodeEditor      *code;
    QHBoxLayout     *statusBarLayout;
    QList<QAction*> recentFiles;
    QColor          *windowColor;
    QColor          *textColor;
    QColor          *highlightColor;
    IconEngine      *icon;

    QSize           *mainTable;
    QSize           *mainEngineSteps;
    QPointF         *mainView;
    QSizeF          *mainStepSize;
    double          mainHorizontalSlider;
    double          mainVerticalSlider;

    /****** GRAPHICSSCENE ******/
    CodenDrawScene  *mainScene;
    QTimer          *sceneTimer;


    /****** STATUS BAR ******/
    QLabel          *currentStatusLabel;
    QLabel          *currentConnection;
    QLabel          *currentTimeLabel;
    QTimer          *currentTimeTimer;


    /****** MENU "FILE" ******/
    QAction         *actionOpen;
    QMenu           *menuRecent;
    QAction         *actionClearRecent;
    QAction         *actionEmptyList;
    QAction         *actionSave;
    QAction         *actionSaveAs;
    QAction         *actionClose;
    QAction         *actionQuit;

    /****** MENU "SETTINGS" ******/
    QMenu           *menuPanels;
    QAction         *actionConfigure;

    /****** MENU "DEVICE" ******/
    QTimer          *deviceTimer;

    /****** MENU "HELP" ******/
    QAction         *actionAbout;

    QSerialPort     *currentDevice;
    QSerialPortInfo *currentDeviceInfo;

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

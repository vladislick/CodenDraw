#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <math.h>

#include "customgraphicsscene.h"
#include "codeeditor.h"
#include "configfile.h"
#include "iconengine.h"
#include "mainscene.h"

// Версия приложения
#define AppVersion "1.0-dev"

// Максимальное количество недавних файлов
#define RecentFilesMax 10

#define MaxFPS  63

#define mainWindowState         "mainwindow.state"
#define mainAppIcons            "icons/"
#define mainThemeIcons          "icons/Papirus/"

// If it's Linux
#ifdef Q_OS_LINUX
    #define systemType          0
    #define mainAppPath     ".config/qinkdraw/"
#endif

// If it's Windows
#ifdef Q_OS_WIN32
    #define systemType          1
    #define mainAppPath     ""
#endif

namespace Ui {
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
    void dragMoveEvent(QDragMoveEvent *event);
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
    void openRecentFile();
    void clearRecentFiles();
    void currentTimeChanged();
    void codeChanged();
    void clearOutput();
    void previewUpdate(bool noquestion = false);

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

    /// Read the code and convert it to the line
    void readCode(QPlainTextEdit *codeEditor, QList<QLineF>* lines);

    inline void drawRulers(double scale, int dx, int dy, QGraphicsScene* scene, QSize* table, QPen* pen);
    inline void drawPreview(double scale, int dx, int dy, QGraphicsScene* scene, QList<QLineF>* line, QPen* pen);

    void updateGUI();

    Ui::MainWindow  *ui;
    CodeEditor      *code;
    QHBoxLayout     *statusBarLayout;
    MainScene       *mainScene;
    QLabel          *currentStatusLabel;
    QLabel          *currentTimeLabel;
    QTimer          *currentTimeTimer;
    QTimer          *previewUpdateTimer;
    QPainter        *mainPainter;
    QList<QAction*> recentFiles;
    QList<QLineF>   *previewLine;
    QColor          *windowColor;
    QColor          *textColor;
    QColor          *highlightColor;
    QString         savedCode;
    QString         openedFileName;
    QElapsedTimer   *elapsedTimer;
    IconEngine      *mainIcon;

    /// Info about file that is open
    QFileInfo   *currentFile;
    /// Main application directory
    QDir        *applicationDirectory;
    ///
    QFile       *mainWindowStateFile;
    /// Application settings file
    QSettings   *mainSettingsFile;

    //int             mainTableWidth  = 2000;
    //int             mainTableHeight = 1368;
    QSize           *mainTable;
    QSize           *mainEngineSteps;
    QPointF         *mainView;
    QSizeF          *mainStepSize;
    double          mainHorizontalSlider;
    double          mainVerticalSlider;
    //double          mainStepSizeX   = 0;
    //double          mainStepSizeY   = 0;
    //int             mainStepsX      = 240;
    //int             mainStepsY      = 160;

    int             openedFileType      = 0;

};

#endif // MAINWINDOW_H

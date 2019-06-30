#include "mainwindow.h"
#include "aboutwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Add a HiDPI support
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    // Set application name
    QCoreApplication::setApplicationName("CodenDraw");
    QCoreApplication::setOrganizationName("CodenDraw");



    /******************************* APPLICATION SETTIGNS *******************************/

    code                = new CodeEditor();

    currentStatusLabel  = new QLabel;
    currentConnection   = new QLabel;
    currentTimeLabel    = new QLabel(QTime::currentTime().toString());
    currentTimeTimer    = new QTimer();

    windowColor         = new QColor(QTextEdit().palette().color(QPalette::Window));
    textColor           = new QColor(QTextEdit().palette().color(QPalette::WindowText));
    highlightColor      = new QColor(QTextEdit().palette().color(QPalette::Highlight));
    currentFile         = new QFileInfo;

    mainSettingsFile     = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
    applicationDirectory = new QDir(QFileInfo(mainSettingsFile->fileName()).dir().path());
    mainWindowStateFile  = new QFile(applicationDirectory->path() + '/' + mainWindowState);

    mainTable           = new QSize();
    mainEngineSteps     = new QSize();
    mainStepSize        = new QSizeF();
    mainView            = new QPointF();

    this->setAcceptDrops(true);
    this->setWindowTitle(QCoreApplication::applicationName());
    this->setWindowIcon(QIcon(applicationDirectory->path() + "/" + QCoreApplication::applicationName() + "Logo.png"));

    // Setup main scene
    mainScene = new CodenDrawScene();
    sceneTimer = new QTimer();
    mainScene->setPPI(qApp->screens()[qApp->desktop()->screenNumber(this)]->physicalDotsPerInch());
    mainScene->setTable(QSize(240, 160));
    mainScene->setScale(1);
    mainScene->setScaleSpinBox(ui->spinBox);

    ui->labelDragAndDrop->setVisible(false);
    ui->graphicsView->setAcceptDrops(true);
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setScene(mainScene);

    connect(sceneTimer, SIGNAL(timeout()), this, SLOT(previewUpdate()));
    sceneTimer->start(15);

    // Setup IconEngine
    icon = new IconEngine;
    icon->setPath(applicationDirectory->path() + '/' + mainAppIcons + '/' + mainIcons);
    icon->setStyle(IconEngine::SystemColor);

    // Add our code widget to "Code" dock
    ui->verticalLayout_6->addWidget(code);
    connect(code, SIGNAL(textChanged()), this, SLOT(codeChanged()));

    // Setup status bar
    QFont font(QApplication::font());
    font.setFamily("monospace");
    currentTimeLabel->setFont(font);
    currentConnection->setFont(font);

    ui->statusBar->addPermanentWidget(currentStatusLabel, 1);
    ui->statusBar->addPermanentWidget(currentConnection, 0);
    ui->statusBar->addPermanentWidget(currentTimeLabel, 0);

    connect(currentTimeTimer, SIGNAL(timeout()), this, SLOT(currentTimeChanged()));
    currentTimeTimer->start(1000);


    // Setup devices
    deviceTimer         = new QTimer();
    currentDevice       = new QSerialPort();
    currentDeviceInfo   = new QSerialPortInfo();

    connect(deviceTimer, SIGNAL(timeout()), this, SLOT(updateDevicesList()));
    deviceTimer->start(500);



    /******************************* MENUS SETTINGS *******************************/

    // Menu "File"
    actionOpen          = new QAction(QIcon::fromTheme("document-open", icon->byName("document-open")), tr("&Open"));
    menuRecent          = new QMenu(tr("&Recent files"));
    actionClearRecent   = new QAction(QIcon::fromTheme("edit-clear-history", icon->byName("edit-clear-history")), tr("&Clear list"));
    actionEmptyList     = new QAction(tr("No files"));
    actionSave          = new QAction(QIcon::fromTheme("document-save", icon->byName("document-save")), tr("&Save"));
    actionSaveAs        = new QAction(QIcon::fromTheme("document-save-as", icon->byName("document-save-as")), tr("&Save as..."));
    actionClose         = new QAction(QIcon::fromTheme("document-close", icon->byName("document-close")), tr("&Close"));
    actionQuit          = new QAction(QIcon::fromTheme("application-exit", icon->byName("application-exit")), tr("&Quit"));

    menuRecent->setIcon(QIcon::fromTheme("document-open-recent", icon->byName("document-open-recent")));
    actionClearRecent->setVisible(false);
    actionEmptyList->setEnabled(false);

    connect(actionOpen, SIGNAL(triggered()), this, SLOT(actionOpenTriggered()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(actionSaveTriggered()));
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(actionSaveAsTriggered()));
    connect(actionClose, SIGNAL(triggered()), this, SLOT(actionCloseTriggered()));
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(actionQuitTriggered()));
    connect(actionClearRecent, SIGNAL(triggered()), this, SLOT(actionClearRecentTriggered()));

    actionOpen->setShortcut(QKeySequence::Open);
    actionSave->setShortcut(QKeySequence::Save);
    actionSaveAs->setShortcut(QKeySequence::SaveAs);
    actionClose->setShortcut(QKeySequence::Close);
    actionQuit->setShortcut(QKeySequence::Quit);

    ui->menuFile->addAction(actionOpen);
    ui->menuFile->addMenu(menuRecent);
    ui->menuFile->addSeparator();
    ui->menuFile->addAction(actionSave);
    ui->menuFile->addAction(actionSaveAs);
    ui->menuFile->addAction(actionClose);
    ui->menuFile->addSeparator();
    ui->menuFile->addAction(actionQuit);

    // Menu "Recent files"
    for (int i = 0; i < RecentFilesMax; i++)
    {
        // Create actions
        QAction* action = new QAction(this);
        action->setVisible(false);
        connect(action, SIGNAL(triggered()), this, SLOT(actionRecentTriggered()));
        recentFiles.append(action);
    }
    menuRecent->addActions(recentFiles);
    menuRecent->addAction(actionEmptyList);
    menuRecent->addSeparator();
    menuRecent->addAction(actionClearRecent);

    // Menu "Help"
    actionAbout = new QAction(QIcon::fromTheme("help-about", icon->byName("help-about")), tr("&About %1").arg(QCoreApplication::applicationName()));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(actionAboutTriggered()));
    ui->menuHelp->addAction(actionAbout);



    /******************************* TOOLBAR SETTINGS *******************************/




    /******************************* FINAL SETTINGS *******************************/

    // Update application state
    loadWindowConfiguration();
    loadRecentFilesList();
    updateGUI();
    sendMessage("Welcome!");

    // If a file has been transferred to the application
    QString file;
    for (const auto& arg : qApp->arguments())
        if (QFileInfo(arg).isFile())
            file = arg;
    if (file != qApp->arguments()[0])
        openFile(file);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    /* In the deep developing */
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    /* In the deep developing */
}

void MainWindow::dropEvent(QDropEvent *event)
{
    /* In the deep developing */
}

void MainWindow::previewUpdate()
{
    static double scaleLast = 0;
    static QSizeF offsetLast;
    static double dxLast = mainScene->offsetX();
    static double dyLast = mainScene->offsetY();

    if (mainScene->mousePressed())
    {
        mainScene->setOffsetX(dxLast + mainScene->mousePos().x() - mainScene->mouseStartPos().x());
        mainScene->setOffsetY(dyLast + mainScene->mousePos().y() - mainScene->mouseStartPos().y());
    }
    else
    {
        dxLast = mainScene->offsetX();
        dyLast = mainScene->offsetY();
    }

    if (scaleLast != mainScene->scale() || offsetLast != mainScene->offset() || mainScene->mousePressed())
    {
        offsetLast = mainScene->offset();
        scaleLast = mainScene->scale();
    }
    else return;

    mainScene->clear();
    mainScene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());

    mainScene->drawTableShadow(5, textColor->lighter(200));
    mainScene->drawTable(*textColor);
    mainScene->update();
}

void MainWindow::currentTimeChanged()
{
    currentTimeLabel->setText(QTime::currentTime().toString());
}

/// Add the file to recent files menu
void MainWindow::addRecentFile(const QString &filePath)
{
    QFileInfo file(filePath);

    // Show button "Clear list"
    actionClearRecent->setVisible(true);
    // Hide label "Empty"
    actionEmptyList->setVisible(false);

    // Get full action name (like "file name [ file path ]")
    QString actionName = file.fileName() + " [" + file.dir().path() + ']';

    // Set the icon for this file type
    IconEngine::Style iconStyle = icon->style();
    icon->setStyle(IconEngine::NoStyle);

    QIcon fileIcon;
    if (file.suffix() == "jpg" || file.suffix() == "png")
        fileIcon = QIcon::fromTheme("image-x-generic", icon->byName("image-x-generic"));
    else if (file.suffix() == "svg")
        fileIcon = QIcon::fromTheme("image-x-svg+xml", icon->byName("image-x-svg+xml"));
    else
        fileIcon = QIcon::fromTheme("text-x-generic", icon->byName("text-x-generic"));

    icon->setStyle(iconStyle);

    // Set start index to do a correct shift
    int startIndex = recentFiles.count() - 1;

    // If file is exist in "Recent files", set start index on this file
    for (int i = 0; i < recentFiles.count(); i++)
    {
        if (!recentFiles[i]->isVisible())
            continue;
        if (recentFiles[i]->text() == actionName)
            startIndex = i;
    }

    // Shift QActions (their data)
    for (int i = startIndex; i > 0; i--)
    {
        recentFiles[i]->setText(recentFiles[i - 1]->text());
        recentFiles[i]->setIcon(recentFiles[i - 1]->icon());
        recentFiles[i]->setVisible(recentFiles[i - 1]->isVisible());
        recentFiles[i]->setWhatsThis(recentFiles[i - 1]->whatsThis());
    }

    // Add new QAction data
    recentFiles[0]->setText(actionName);
    recentFiles[0]->setIcon(fileIcon);
    recentFiles[0]->setVisible(true);
    recentFiles[0]->setWhatsThis(filePath);
}

/// Save recent files list to the settings file
void MainWindow::saveRecentFilesList() {
    mainSettingsFile->beginGroup("recentfiles");
    mainSettingsFile->beginWriteArray("file");

    for (int i = 0; i < recentFiles.count(); i++)
    {
        mainSettingsFile->setArrayIndex(i);

        if (recentFiles[i]->isVisible())
            mainSettingsFile->setValue("filepath", recentFiles[i]->whatsThis());
        else
            mainSettingsFile->setValue("filepath", "empty");
    }

    mainSettingsFile->endArray();
    mainSettingsFile->endGroup();
}

/// Load recent files list from the settings file
void MainWindow::loadRecentFilesList()
{
    int filesCount = mainSettingsFile->beginReadArray("recentfiles/file");

    for (int i = filesCount - 1; i >= 0; i--)
    {
        mainSettingsFile->setArrayIndex(i);
        if (mainSettingsFile->value("filepath", "empty").toString() != "empty")
            addRecentFile(mainSettingsFile->value("filepath").toString());
    }

    mainSettingsFile->endArray();
}

/// Save all window data to files
void MainWindow::saveWindowConfiguration()
{
    // Save window size and full screen mode to the settings file
    mainSettingsFile->beginGroup("mainwindow");
    mainSettingsFile->setValue("width", this->width());
    mainSettingsFile->setValue("height", this->height());
    mainSettingsFile->setValue("fullscreen", this->isFullScreen());
    mainSettingsFile->setValue("scale", ui->spinBox->value());
    mainSettingsFile->endGroup();
    mainSettingsFile->sync();

    // Save window state to the state file
    mainWindowStateFile->open(QFile::WriteOnly);
    mainWindowStateFile->write(this->saveState());
    mainWindowStateFile->close();

}

/// Load all window data from files
void MainWindow::loadWindowConfiguration()
{
    // Read window size
    int windowWidth = mainSettingsFile->value("mainwindow/width", 900).toInt();
    int windowHeight = mainSettingsFile->value("mainwindow/height", 600).toInt();
    this->resize(windowWidth, windowHeight);

    // Read window full screen mode
    bool fullScreen = mainSettingsFile->value("mainwindow/fullscreen", 0).toBool();
    if (fullScreen)
        this->showFullScreen();

    ui->spinBox->setValue(mainSettingsFile->value("mainwindow/scale", 100).toInt());

    // Read window state
    mainWindowStateFile->open(QFile::ReadOnly);
    this->restoreState(mainWindowStateFile->readAll());
    mainWindowStateFile->close();
}

void MainWindow::codeChanged()
{
    /* In the deep developing */
}

/// Update user interface
void MainWindow::updateGUI()
{
    // Update system colors
    *windowColor     = QTextEdit().palette().color(QPalette::Window);
    *textColor       = QTextEdit().palette().color(QPalette::WindowText);
    *highlightColor  = QTextEdit().palette().color(QPalette::Highlight);

    // Update current file status
    if (currentFile->fileName().isEmpty())
    {
        this->setWindowTitle(QCoreApplication::applicationName());
        actionClose->setText(tr("&Close"));
        actionClose->setEnabled(false);
        actionSave->setText(tr("&Save"));
    }
    else
    {
        this->setWindowTitle(currentFile->fileName() + " - " + QCoreApplication::applicationName());
        actionClose->setText(tr("&Close «%1»").arg(currentFile->fileName()));
        actionClose->setEnabled(true);
    }

    // Update devices
    updateDevices();
}

void MainWindow::updateDevices()
{
    // Clear devices list
    for (const auto &action : ui->menuDevice->actions())
        disconnect(action);
    ui->menuDevice->clear();
    ui->menuDevice->addAction(tr("No available device"));
    ui->menuDevice->actions().last()->setEnabled(false);
    ui->menuDevice->actions().last()->setVisible(false);

    // Try to find connected device
    if (currentDevice->isOpen())
        for (const auto &device : currentDeviceInfo->availablePorts())
            if (device.portName() == currentDevice->portName())
            {
                QAction *action = new QAction(this);
                action->setText(device.description() + " (" + device.portName() + ")");
                action->setCheckable(true);
                action->setChecked(true);
                action->setWhatsThis(device.portName());
                connect(action, SIGNAL(triggered(bool)), this, SLOT(actionDeviceTriggered(bool)));
                ui->menuDevice->addSection(tr("Current device"));
                ui->menuDevice->addAction(action);
                break;
            }

    if (currentDevice->isOpen() && ui->menuDevice->actions().count() == 1)
    {
        sendMessage("Lost connection with " + currentDevice->portName(), Message::Critical);
        currentDevice->close();
    }

    ui->menuDevice->addSection(tr("Available devices"));

    // Add other devices to the list
    for (const auto &device : currentDeviceInfo->availablePorts())
    {
        if (currentDevice->isOpen() && device.portName() == currentDevice->portName())
            continue;

        QAction *action = new QAction(this);
        action->setText(device.description() + " (" + device.portName() + ")");
        action->setCheckable(true);
        action->setChecked(false);
        action->setWhatsThis(device.portName());
        connect(action, SIGNAL(triggered(bool)), this, SLOT(actionDeviceTriggered(bool)));
        ui->menuDevice->addAction(action);
    }

    // If no devices
    if (ui->menuDevice->isEmpty())
        ui->menuDevice->actions()[0]->setVisible(true);

    if (currentDevice->isOpen())
        currentConnection->setText(currentDevice->portName());
    else
        currentConnection->setText("");
}

void MainWindow::updateDevicesList()
{
    static int listSize = 0;

    if (listSize != currentDeviceInfo->availablePorts().count())
        listSize = currentDeviceInfo->availablePorts().count();
    else
        return;

    updateGUI();
}

/// Send message
void MainWindow::sendMessage(QString status, Message message)
{
    // Set message color
    QColor messageColor;

    if (message == Message::Successfully)
        messageColor.setRgb(68, 197, 23);   // Green color
    else if (message == Message::Information)
        messageColor = QLabel().palette().color(QPalette::WindowText);
    else if (message == Message::Warning)
        messageColor.setRgb(248, 109, 0);   // Orange color
    else if (message == Message::Critical)
        messageColor.setRgb(245, 6, 7);     // Red color

    // Show the message in Status Bar and Output dock
    currentStatusLabel->setText("<font color=" + messageColor.name() + '>' + status + "</font>");
    ui->textEditOutput->append('[' + QTime::currentTime().toString() + "] " + status);
}

/// Open raster picture
bool MainWindow::openRasterPicture(const QFileInfo& file)
{
    /* I have no idea how to make it works */
    return false;
}

/// Open vector picture
bool MainWindow::openVectorPicture(const QFileInfo& file)
{
    /* I have no idea how to make it works */
    return false;
}

/// Open text file
bool MainWindow::openTextFile(const QFileInfo& file)
{
    QFile mainFile(file.filePath());

    // Try to open the file
    if (!mainFile.open(QFile::ReadOnly | QFile::Text))
        return false;

    // Read and close the file
    code->setPlainText(mainFile.readAll().toStdString().c_str());
    mainFile.close();
    return true;
}

/// Open the recent file that caused this slot
void MainWindow::actionRecentTriggered()
{
    // Get an action that called this slot
    QAction* action = qobject_cast<QAction*>(sender());

    // If it's actually QAction
    if (!action) return;

    // Just open the file
    openFile(action->whatsThis());
}

/// Clear recent files list
void MainWindow::actionClearRecentTriggered()
{
    // Hide all files in the list
    for (const auto& recent : recentFiles)
        recent->setVisible(false);

    // Hide button "Clear list"
    actionClearRecent->setVisible(false);

    // Show label "No files"
    actionEmptyList->setVisible(true);
}

/// Open file
void MainWindow::openFile(const QString& filePath, bool silentMode)
{
    QFileInfo file(filePath);
    bool    openStatus;

    // Add this file to "Recent files" menu
    addRecentFile(filePath);

    // Try to open the file
    if (file.suffix() == "png" || file.suffix() == "jpg")
        openStatus = openRasterPicture(file);   // If the file is a pixel picture
    else if (file.suffix() == "svg")
        openStatus = openVectorPicture(file);   // If the file is a vector picture
    else
        openStatus = openTextFile(file);        // If it's a text file

    // If file successfully opened
    if (openStatus)
        currentFile->setFile(file.absoluteFilePath());

    // Update user interface
    updateGUI();

    if (silentMode) return;

    // Show message if needed
    if (openStatus)
        sendMessage(tr("«%1» successfully opened").arg(file.fileName()), Message::Successfully);
    else
        sendMessage(tr("Could not open «%1»").arg(file.fileName()), Message::Warning);
}

/// Save file
void MainWindow::saveFile(const QString& filePath)
{
    /* In the deep developing */
}

void MainWindow::clearOutput()
{
    ui->textEditOutput->clear();
}

void MainWindow::actionOpenTriggered()
{
    // Get file path
    QString file(QFileDialog().getOpenFileName(this));

    // Just open the file
    if (!file.isEmpty()) openFile(file);
}

void MainWindow::actionSaveTriggered()
{
    /* In the deep developing */
}

void MainWindow::actionSaveAsTriggered()
{
    // Get path to the new file
    QString file(QFileDialog().getSaveFileName(this));

    // If no file then just exit
    if (file.isEmpty()) return;

    // Call a function that save this file
    saveFile(file);
}


void MainWindow::actionCloseTriggered()
{
    // Show the message
    if (!currentFile->fileName().isEmpty())
        sendMessage(tr("%1 closed").arg(currentFile->fileName()));

    // Clear data
    currentFile->setFile("");
    code->clear();

    // And update the interface
    updateGUI();
}

void MainWindow::actionQuitTriggered()
{
    this->close();
}

void MainWindow::actionDeviceTriggered(bool state)
{
    // Get an action that called this slot
    QAction* action = qobject_cast<QAction*>(sender());

    // If it's actually QAction
    if (!action) return;

    currentDevice->setPortName(action->whatsThis());

    if (state)
    {
        if (currentDevice->isOpen())
            currentDevice->close();

        if (currentDevice->open(QIODevice::ReadWrite))
            sendMessage(tr("Connected to %1").arg(currentDevice->portName()), Message::Successfully);
        else
            sendMessage(tr("Cannot connect to %1").arg(currentDevice->portName()), Message::Warning);
    }
    else
    {
        sendMessage(tr("%1 disconnected").arg(currentDevice->portName()), Message::Information);
        currentDevice->close();
    }

    updateGUI();
}

void MainWindow::actionAboutTriggered()
{
    // Create window
    AboutWindow about;
    about.setAppDir(applicationDirectory->path());
    about.setModal(true);

    // Set correct geometry (move it to the center of the main window)
    int x = this->geometry().x() + (this->geometry().width() - about.geometry().width()) / 2,
        y = this->geometry().y() + (this->geometry().height() - about.geometry().height()) / 2;
    about.setGeometry(x, y, about.geometry().width(), about.geometry().height());

    // Show window
    about.exec();
}

void MainWindow::on_actionAntialiasing_triggered(bool checked)
{
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, checked);
}

void MainWindow::on_horizontalScrollBar_sliderMoved(int position)
{
    mainScene->setOffset(QSizeF(position, mainScene->offset().height()));
}

void MainWindow::on_verticalScrollBar_sliderMoved(int position)
{
    mainVerticalSlider = double(position) / double(ui->verticalScrollBar->maximum());
}

void MainWindow::toolBarOrientationChanged(Qt::Orientation orientation)
{
    //toolScaleSlider->setOrientation(orientation);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveWindowConfiguration();
    saveRecentFilesList();
    event->accept();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    mainScene->zoom(double(arg1) / 100, mainScene->mousePos());
    ui->spinBox->setSingleStep(int(arg1 * 0.08 + 1));
}

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

    ui->labelDragAndDrop->setVisible(false);
    ui->graphicsView->setAcceptDrops(true);
    ui->graphicsView->setMouseTracking(true);

    code                = new CodeEditor();
    currentStatusLabel  = new QLabel;
    currentTimeLabel    = new QLabel(QTime::currentTime().toString());
    currentTimeTimer    = new QTimer();

    mainScene           = new MainScene();

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
    mainIcon            = new IconEngine;

    mainIcon->setPath(applicationDirectory->path() + '/' + mainAppIcons + '/' + mainIcons);
    mainIcon->setStyle(IconEngine::SystemColor);

    this->setAcceptDrops(true);
    this->setWindowTitle(QCoreApplication::applicationName());
    this->setWindowIcon(QIcon(applicationDirectory->path() + "/" + QCoreApplication::applicationName() + "Logo.png"));

    // Setup "Recent files" menu
    for (int i = 0; i < RecentFilesMax; i++)
    {
        QAction* action = new QAction(this);
        action->setVisible(false);
        connect(action, SIGNAL(triggered()), this, SLOT(openRecentFile()));
        recentFiles.append(action);
    }
    ui->menuRecentFiles->addActions(recentFiles);
    ui->menuRecentFiles->addAction(tr("Empty"));
    ui->menuRecentFiles->actions().last()->setEnabled(false);
    ui->menuRecentFiles->addSeparator();
    ui->menuRecentFiles->addAction(QIcon::fromTheme("edit-clear-history", mainIcon->getByName("edit-clear-history")), tr("&Clear list"));
    connect(ui->menuRecentFiles->actions().last(), SIGNAL(triggered()), this, SLOT(clearRecentFiles()));
    ui->menuRecentFiles->actions().last()->setVisible(false);

    // Restore the previous application state
    loadWindowConfiguration();
    loadRecentFilesList();

    // Set icons for menu actions
    ui->actionOpen->setIcon(QIcon::fromTheme("document-open", mainIcon->getByName("document-open")));
    ui->menuRecentFiles->setIcon(QIcon::fromTheme("document-open-recent", mainIcon->getByName("document-open-recent")));
    ui->actionSave->setIcon(QIcon::fromTheme("document-save", mainIcon->getByName("document-save")));
    ui->actionSaveAs->setIcon(QIcon::fromTheme("document-save-as", mainIcon->getByName("document-save-as")));
    ui->actionRefresh->setIcon(QIcon::fromTheme("view-refresh", mainIcon->getByName("view-refresh")));
    ui->actionClose->setIcon(QIcon::fromTheme("document-close", mainIcon->getByName("document-close")));
    ui->actionQuit->setIcon(QIcon::fromTheme("application-exit", mainIcon->getByName("application-exit")));
    ui->actionAbout->setIcon(QIcon::fromTheme("help-about", mainIcon->getByName("help-about")));
    ui->menuPanels->setIcon(QIcon::fromTheme("view-list-details", mainIcon->getByName("view-list-details")));
    ui->actionStart->setIcon(QIcon::fromTheme("media-playback-start", mainIcon->getByName("media-playback-start")));
    ui->actionStop->setIcon(QIcon::fromTheme("media-playback-stop", mainIcon->getByName("media-playback-stop")));
    ui->actionPause->setIcon(QIcon::fromTheme("media-playback-pause", mainIcon->getByName("media-playback-pause")));
    ui->actionConfigure->setIcon(QIcon::fromTheme("configure", mainIcon->getByName("configure")));
    ui->actionSendCommand->setIcon(QIcon::fromTheme("document-send", mainIcon->getByName("document-send")));
    ui->actionInvertColors->setIcon(QIcon::fromTheme("invertimage", mainIcon->getByName("invertimage")));
    ui->actionAntialiasing->setIcon(QIcon::fromTheme("blurimage", mainIcon->getByName("blurimage")));

    // Set shortcuts for menu actions
    ui->actionOpen->setShortcut(QKeySequence::Open);
    ui->actionSave->setShortcut(QKeySequence::Save);
    ui->actionSaveAs->setShortcut(QKeySequence::SaveAs);
    ui->actionClose->setShortcut(QKeySequence::Close);
    ui->actionRefresh->setShortcut(QKeySequence::Refresh);
    ui->actionQuit->setShortcut(QKeySequence::Quit);
    ui->actionConfigure->setShortcut(QKeySequence::Preferences);

    // Set icons for buttons in "Control" dock
    ui->toolButtonUp->setIcon(QIcon::fromTheme("go-up", mainIcon->getByName("go-up")));
    ui->toolButtonDown->setIcon(QIcon::fromTheme("go-down", mainIcon->getByName("go-down")));
    ui->toolButtonLeft->setIcon(QIcon::fromTheme("go-previous", mainIcon->getByName("go-previous")));
    ui->toolButtonRight->setIcon(QIcon::fromTheme("go-next", mainIcon->getByName("go-next")));
    ui->toolButtonForward->setIcon(QIcon::fromTheme("go-up", mainIcon->getByName("go-up")));
    ui->toolButtonBack->setIcon(QIcon::fromTheme("go-down", mainIcon->getByName("go-down")));
    ui->toolButtonHome->setIcon(QIcon::fromTheme("go-home", mainIcon->getByName("go-home")));
    ui->toolButtonSend->setIcon(QIcon::fromTheme("document-send", mainIcon->getByName("document-send")));

    // Add our code widget to "Code" dock
    ui->verticalLayout_6->addWidget(code);

    ui->dockWidgetControl->toggleViewAction()->setText(tr("Show panel"));
    ui->menuControlPanel->addAction(ui->dockWidgetControl->toggleViewAction());

    ui->dockWidgetCode->toggleViewAction()->setText(tr("Show panel"));
    ui->menuCodePanel->addAction(ui->dockWidgetCode->toggleViewAction());

    ui->dockWidgetConnection->toggleViewAction()->setText(tr("Show panel"));
    ui->menuConnectionPanel->addAction(ui->dockWidgetConnection->toggleViewAction());

    ui->dockWidgetOutput->toggleViewAction()->setText(tr("Show panel"));
    ui->menuOutputPanel->addAction(ui->dockWidgetOutput->toggleViewAction());

    ui->dockWidgetAxisValue->toggleViewAction()->setText(tr("Show panel"));
    ui->menuAxisValuePanel->addAction(ui->dockWidgetAxisValue->toggleViewAction());

    ui->menuOutputPanel->addAction(QIcon::fromTheme("edit-clear-history", mainIcon->getByName("edit-clear-history")), tr("&Clear"));

    ui->graphicsView->setScene(mainScene);

    connect(ui->menuOutputPanel->actions().last(), SIGNAL(triggered()), this, SLOT(clearOutput()));
    connect(currentTimeTimer, SIGNAL(timeout()), this, SLOT(currentTimeChanged()));
    connect(code, SIGNAL(textChanged()), this, SLOT(codeChanged()));

    QFont font(QApplication::font());
    font.setFamily("monospace");
    currentTimeLabel->setFont(font);

    ui->statusBar->addPermanentWidget(currentStatusLabel, 1);
    ui->statusBar->addPermanentWidget(currentTimeLabel, 0);
    sendMessage("Welcome!");

    updateGUI();
    currentTimeTimer->start(1000);

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

void MainWindow::currentTimeChanged()
{
    currentTimeLabel->setText(QTime::currentTime().toString());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveWindowConfiguration();
    saveRecentFilesList();
    event->accept();
}

/// Add the file to recent files menu
void MainWindow::addRecentFile(const QString &filePath)
{
    QFileInfo file(filePath);

    // Show button "Clear list"
    ui->menuRecentFiles->actions().last()->setVisible(true);
    // Hide label "Empty"
    ui->menuRecentFiles->actions()[ui->menuRecentFiles->actions().size() - 3]->setVisible(false);

    // Get full action name (like "file name [ file path ]")
    QString actionName = '&' + file.fileName() + " [" + filePath + "]";

    // Set the icon for this file type
    IconEngine::Style iconStyle = mainIcon->style();
    mainIcon->setStyle(IconEngine::NoStyle);

    QIcon fileIcon;
    if (file.suffix() == "jpg" || file.suffix() == "png")
        fileIcon = QIcon::fromTheme("image-x-generic", mainIcon->getByName("image-x-generic"));
    else if (file.suffix() == "svg")
        fileIcon = QIcon::fromTheme("image-x-svg+xml", mainIcon->getByName("image-x-svg+xml"));
    else
        fileIcon = QIcon::fromTheme("text-x-generic", mainIcon->getByName("text-x-generic"));

    mainIcon->setStyle(iconStyle);

    // Set start index to do a correct shift
    int startIndex = recentFiles.count() - 1;

    // If file is exist in "Recent files", set start index on this file
    for (int i = 0; i < recentFiles.count(); i++) {
        if (!recentFiles[i]->isVisible()) continue;
        if (recentFiles[i]->text() == actionName) startIndex = i;
    }

    // Shift QActions (their data)
    for (int i = startIndex; i > 0; i--) {
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

    // Read window state
    mainWindowStateFile->open(QFile::ReadOnly);
    this->restoreState(mainWindowStateFile->readAll());
    mainWindowStateFile->close();
}

void MainWindow::clearOutput()
{
    ui->textEditOutput->clear();
}

void MainWindow::codeChanged()
{
    /* In the deep developing */
}

/// Update user interface
void MainWindow::updateGUI() {
    // Update system colors
    *windowColor     = QTextEdit().palette().color(QPalette::Window);
    *textColor       = QTextEdit().palette().color(QPalette::WindowText);
    *highlightColor  = QTextEdit().palette().color(QPalette::Highlight);

    // Update current file status
    if (currentFile->filePath() != QString())
    {
        this->setWindowTitle(currentFile->fileName() + " - " + QCoreApplication::applicationName());
        ui->actionClose->setText(tr("&Close «%1»").arg(currentFile->fileName()));
        ui->actionClose->setEnabled(true);
        ui->actionSave->setEnabled(true);
    }
    else
    {
        this->setWindowTitle(QCoreApplication::applicationName());
        ui->actionClose->setText(tr("&Close"));
        ui->actionClose->setEnabled(false);
        ui->actionSave->setEnabled(false);
    }
}

/// Send message
void MainWindow::sendMessage(QString status, Message message) {
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
void MainWindow::openRecentFile()
{
    // Get an action that called this slot
    QAction* action = qobject_cast<QAction*>(sender());

    // If it's actually QAction
    if (!action) return;

    // Just open the file
    openFile(action->whatsThis());
}

/// Clear recent files list
void MainWindow::clearRecentFiles()
{
    // Hide all files in the list
    for (const auto& recent : recentFiles)
        recent->setVisible(false);

    // Hide button "Clear list"
    ui->menuRecentFiles->actions().last()->setVisible(false);

    // Show label "No files"
    ui->menuRecentFiles->actions()[ui->menuRecentFiles->actions().size() - 3]->setVisible(true);
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

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::on_actionAbout_triggered()
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

void MainWindow::on_actionSave_triggered()
{
    /* In the deep developing */
}

void MainWindow::on_actionOpen_triggered()
{
    // Get file path
    QString file(QFileDialog().getOpenFileName(this));

    // Just open the file
    if (!file.isEmpty()) openFile(file);
}

void MainWindow::on_actionClose_triggered()
{
    // Show the message
    sendMessage(tr("%1 closed").arg(currentFile->fileName()));

    // Clear data
    currentFile->setFile("");
    code->clear();

    // And update the interface
    updateGUI();
}

void MainWindow::on_actionSaveAs_triggered()
{
    // Get path to the new file
    QString file(QFileDialog().getSaveFileName(this));

    // If no file then just exit
    if (file.isEmpty()) return;

    // Call a function that save this file
    saveFile(file);
}

void MainWindow::on_actionRefresh_triggered()
{
    updateGUI();
}

void MainWindow::on_actionAntialiasing_triggered(bool checked)
{
    ui->graphicsView->setRenderHint(QPainter::Antialiasing, checked);
}

void MainWindow::on_horizontalScrollBar_sliderMoved(int position)
{
    mainHorizontalSlider = double(position) / double(ui->horizontalScrollBar->maximum());
}

void MainWindow::on_verticalScrollBar_sliderMoved(int position)
{
    mainVerticalSlider = double(position) / double(ui->verticalScrollBar->maximum());
}

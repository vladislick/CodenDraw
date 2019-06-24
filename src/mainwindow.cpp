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

    this->setAcceptDrops(true);
    this->setWindowTitle(QCoreApplication::applicationName());

    ui->labelDragAndDrop->setVisible(false);
    ui->graphicsView->setAcceptDrops(true);
    ui->graphicsView->setMouseTracking(true);

    code                = new CodeEditor();
    currentStatusLabel  = new QLabel;
    currentTimeLabel    = new QLabel(QTime::currentTime().toString());
    currentTimeTimer    = new QTimer();
    previewUpdateTimer  = new QTimer();
    mainScene           = new MainScene();
    windowColor         = new QColor(QTextEdit().palette().color(QPalette::Window));
    textColor           = new QColor(QTextEdit().palette().color(QPalette::WindowText));
    highlightColor      = new QColor(QTextEdit().palette().color(QPalette::Highlight));
    elapsedTimer        = new QElapsedTimer();

    currentFile         = new QFileInfo;

    previewLine         = new QList<QLineF>;


    mainSettingsFile    = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
    applicationDirectory= new QDir(QFileInfo(mainSettingsFile->fileName()).dir().path());
    mainWindowStateFile = new QFile(applicationDirectory->path() + '/' + mainWindowState);

    mainTable           = new QSize();
    mainEngineSteps     = new QSize();
    mainStepSize        = new QSizeF();
    mainView            = new QPointF();
    mainIcon            = new IconEngine;

    mainTable->setWidth(240);
    mainTable->setHeight(240);

    mainEngineSteps->setWidth(240);
    mainEngineSteps->setHeight(160);

    mainStepSize->setWidth(double(mainTable->width()) / mainEngineSteps->width());
    mainStepSize->setHeight(double(mainTable->height()) / mainEngineSteps->height());

    mainIcon->setPath(applicationDirectory->path() + '/' + mainThemeIcons);
    mainIcon->setThemeStyle(IconEngine::SystemColor);

    sendMessage(mainSettingsFile->fileName());

    // Setup "Recent files" menu
    for (int i = 0; i < RecentFilesMax; i++) {
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
    connect(previewUpdateTimer, SIGNAL(timeout()), this, SLOT(previewUpdate()));
    connect(code, SIGNAL(textChanged()), this, SLOT(codeChanged()));

    QFont font(QApplication::font());
    font.setFamily("monospace");
    currentTimeLabel->setFont(font);

    ui->statusBar->addPermanentWidget(currentStatusLabel, 1);
    ui->statusBar->addPermanentWidget(currentTimeLabel, 0);
    sendMessage("Welcome!");

    updateGUI();
    previewUpdateTimer->start(1000/MaxFPS);
    currentTimeTimer->start(1000);

    for (int i = 1; i < qApp->arguments().count(); i++) {
        QFile file(qApp->arguments()[i]);
        if (file.exists()) {
            addRecentFile(file.fileName());
            openFile(file.fileName());
        }
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        QFileInfo file;
        foreach (const QUrl &url, event->mimeData()->urls()) {
            file.setFile(url.toLocalFile());
            ui->labelDragAndDrop->setText(tr("Open file %1%2%3").arg("<br><font color=" + highlightColor->name() + ">«").arg(file.fileName()).arg("»</font>"));
        }
        ui->labelDragAndDrop->setVisible(true);
        ui->graphicsView->setVisible(false);
        ui->verticalSlider->setVisible(false);
        ui->horizontalScrollBar->setVisible(false);
        ui->verticalScrollBar->setVisible(false);
        event->acceptProposedAction();
    }
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event) {
    ui->labelDragAndDrop->setVisible(false);
    ui->graphicsView->setVisible(true);
    ui->verticalSlider->setVisible(true);
    ui->horizontalScrollBar->setVisible(true);
    ui->verticalScrollBar->setVisible(true);
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event) {
    if (event->mimeData()->hasUrls()) {
        QFileInfo file;
        foreach (const QUrl &url, event->mimeData()->urls()) {
            file.setFile(url.toLocalFile());
            ui->labelDragAndDrop->setText(tr("Open file %1%2%3").arg("<br><font color=" + highlightColor->name() + ">«").arg(file.fileName()).arg("»</font>"));
        }
        ui->labelDragAndDrop->setVisible(true);
        ui->graphicsView->setVisible(false);
        ui->verticalSlider->setVisible(false);
        ui->horizontalScrollBar->setVisible(false);
        ui->verticalScrollBar->setVisible(false);
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    ui->labelDragAndDrop->setVisible(false);
    ui->graphicsView->setVisible(true);
    ui->verticalSlider->setVisible(true);
    ui->horizontalScrollBar->setVisible(true);
    ui->verticalScrollBar->setVisible(true);
    foreach (const QUrl &url, event->mimeData()->urls()) {
        addRecentFile(url.toLocalFile());
        openFile(url.toLocalFile());
    }
    event->accept();
}


void MainWindow::currentTimeChanged() {
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

/// Read the code and convert it to the line
void MainWindow::readCode(QPlainTextEdit *codeEditor, QList<QLineF>* lines) {

    lines->clear();

    QString mainCode(codeEditor->toPlainText());
    QString temp, tmp;
    QLineF   tempLine;

    const char parameters = 4;

    double currentX = 0,
           currentY = 0,
           currentZ = 0;

    int     index = 0;
    double  value[parameters];
    bool    exist[parameters];

    for (int i = 0; i < mainCode.length() + 1; i++) {
        if (mainCode[i] == '\n' || mainCode[i] == '\0') {
            // Clear all data
            for (short k = 0; k < parameters; k++) exist[k] = false;
            temp.clear();

            for (int j = 0; j < tmp.length() + 1; j++) {
                if (tmp[j] > 64 && tmp[j] < 91) {
                    if (!temp.isEmpty()) value[index] = temp.toDouble();
                    temp.clear();

                    switch (tmp[j].toLatin1()) {
                    case 'G':
                        index = 0;
                        break;
                    case 'X':
                        index = 1;
                        break;
                    case 'Y':
                        index = 2;
                        break;
                    case 'Z':
                        index = 3;
                        break;
                    default:
                        continue;
                    }

                    exist[index] = true;
                }
                else if ((tmp[j] > 47 && tmp[j] < 58) || tmp[j] == '.') {
                    temp += tmp[j];
                }
                else if (tmp[j] == ' ') continue;
                else if (tmp[j] == '\0') {
                    if (!temp.isEmpty()) value[index] = temp.toDouble();

                    if (exist[0]) {
                        if (exist[3]) currentZ = value[3];
                        if (exist[1] || exist[2]) {
                            if (int(currentZ) == 20) {
                                tempLine.setLine(currentX, currentY, exist[1] * value[1] + !exist[1] * currentX, exist[2] * value[2] + !exist[2] * currentY);
                                lines->append(tempLine);
                            }

                            if (exist[1]) currentX = value[1];
                            if (exist[2]) currentY = value[2];
                        }
                    }
                }
            }

            tmp.clear();
        }
        else tmp += mainCode[i];
    }
}

void MainWindow::previewUpdate(bool noquestion) {

//    // Create variables to check some changes
//    static int      lastWidth           = 0;
//    static int      lastHeight          = 0;
//    static int      lastScale           = 0;
//    static int      lastViewX           = 0;
//    static int      lastViewY           = 0;
//    static int      horizontalBarLast   = 0;
//    static int      verticalBarLast     = 0;
//    static double   lastMainScale       = 0;

//    // Handle mouse events
//    if (mainScene->isMousePressed)  {
//        ui->graphicsView->setCursor(Qt::DragMoveCursor);

//        int posChange = 0, barChange = 0;

//        if (ui->horizontalScrollBar->isEnabled()) {
//            posChange = int(mainScene->mouseCurrentPos.x() - mainScene->mouseStartPos.x());
//            barChange = horizontalBarLast - posChange;
//            if (barChange >= 0 && barChange <= ui->horizontalScrollBar->maximum()) {
//                ui->horizontalScrollBar->setValue(barChange);
//                mainHorizontalSlider = double(barChange) / ui->horizontalScrollBar->maximum();
//            }
//        }

//        if (ui->verticalScrollBar->isEnabled()) {
//            posChange = int(mainScene->mouseCurrentPos.y() - mainScene->mouseStartPos.y());
//            barChange = verticalBarLast - posChange;
//            if (barChange >= 0 && barChange <= ui->verticalScrollBar->maximum()) {
//                ui->verticalScrollBar->setValue(barChange);
//                mainVerticalSlider = double(barChange) / ui->verticalScrollBar->maximum();
//            }
//        }
//    }
//    else {
//        horizontalBarLast   = ui->horizontalScrollBar->value();
//        verticalBarLast     = ui->verticalScrollBar->value();
//        ui->graphicsView->setCursor(Qt::ArrowCursor);
//    }

//    int scroll = mainScene->wheelChanged();
//    if (scroll > 0) {
//        if (ui->verticalSlider->value() + scroll <= ui->verticalSlider->maximum())
//            ui->verticalSlider->setValue(ui->verticalSlider->value() + scroll);
//    } else if (scroll < 0) {
//        if (ui->verticalSlider->value() + scroll > 0)
//            ui->verticalSlider->setValue(ui->verticalSlider->value() + scroll);
//    }

//    // If it is not necessary to perform further
//    if (!noquestion) {
//        if (lastWidth  != ui->graphicsView->width() ||
//            lastHeight != ui->graphicsView->height() ||
//            lastScale  != ui->verticalSlider->value() ||
//            lastViewY  != ui->verticalScrollBar->value() ||
//            lastViewX  != ui->horizontalScrollBar->value()) {

//            lastWidth  = ui->graphicsView->width();
//            lastHeight = ui->graphicsView->height();
//            lastScale  = ui->verticalSlider->value();
//            lastViewY  = ui->verticalScrollBar->value();
//            lastViewX  = ui->horizontalScrollBar->value();
//        } else return;
//    }

//    // Prepare GraphicsScene
    mainScene->clear();
    mainScene->setSceneRect(0, 0, ui->graphicsView->width() - 5, ui->graphicsView->height() - 5);

    QSizeF ppi(qApp->primaryScreen()->physicalDotsPerInchX(), qApp->primaryScreen()->physicalDotsPerInchY());

    mainScene->setTable(*mainTable);
    mainScene->setMainColor(*textColor);
    mainScene->setPPI(ppi);
    mainScene->setScale(0.5);
    mainScene->drawTable(*textColor);

//    // Calculate scale factor
//    double mainScale = double(ui->verticalSlider->value()) / 1000;

//    //
//    int horizontalMax   = int(mainTable->width() * mainScale * 2) - int(mainScene->width());
//    int verticalMax     = int(mainTable->height() * mainScale * 2) - int(mainScene->height());

//    // If the main table is closer than the size of the scene width
//    if (horizontalMax > 10) {
//        ui->horizontalScrollBar->setMaximum(horizontalMax);
//        ui->horizontalScrollBar->setEnabled(1);
//    } else {
//        ui->horizontalScrollBar->setEnabled(0);
//        ui->horizontalScrollBar->setValue(0);
//        ui->horizontalScrollBar->setMaximum(0);
//        mainHorizontalSlider = 0.5;
//    }

//    // If the main table is closer than the size of the scene height
//    if (verticalMax > 10) {
//        ui->verticalScrollBar->setMaximum(verticalMax);
//        ui->verticalScrollBar->setEnabled(1);
//    } else {
//        ui->verticalScrollBar->setEnabled(0);
//        ui->verticalScrollBar->setValue(0);
//        ui->verticalScrollBar->setMaximum(0);
//        mainVerticalSlider = 0.5;
//    }

//    // If the scale has been changed
//    if (lastMainScale != mainScale) {
//        lastMainScale = mainScale;
//        ui->horizontalScrollBar->setValue(int(ui->horizontalScrollBar->maximum() * mainHorizontalSlider));
//        ui->verticalScrollBar->setValue(int(ui->verticalScrollBar->maximum() * mainVerticalSlider));
//    }

//    // Get the value to shift the center of the table
//    int dx, dy;
//    if (horizontalMax > 0)  dx = int(0 + (mainTable->width() * mainScale) / 2) - ui->horizontalScrollBar->value();
//    else                    dx = int(mainScene->width() - mainTable->width() * mainScale) / 2;
//    if (verticalMax > 0)    dy = int(0 + (mainTable->height() * mainScale) / 2) - ui->verticalScrollBar->value();
//    else                    dy = int(mainScene->height() - mainTable->height() * mainScale) / 2;

//    // Create a pen
//    QPen pen(*textColor);
//    if (mainScale > 0) pen.setWidth(int(mainScale) + 1);
//    else               pen.setWidth(int(-mainScale) + 1);

//    // Draw table and content
//    drawPreview(mainScale, dx, dy, mainScene, previewLine, &pen);
//    drawRulers(mainScale, dx, dy, mainScene, mainTable, &pen);

//    // Update the scene
    mainScene->update();
}


void MainWindow::drawPreview(double scale, int dx, int dy, QGraphicsScene* scene, QList<QLineF>* line, QPen* pen) {

    int  x1,  x2,  y1,  y2, _x1 = 0, _x2 = 0, _y1 = 0, _y2 = 0;

    for (int i = 0; i < line->count(); i++) {

        x1 = int(line->at(i).x1() * scale);
        x2 = int(line->at(i).x2() * scale);
        y1 = int(line->at(i).y1() * scale);
        y2 = int(line->at(i).y2() * scale);

        if (i > 0) {
            if (abs(x1 - _x1) > 1 ||
                abs(x2 - _x2) > 1 ||
                abs(y1 - _y1) > 1 ||
                abs(y2 - _y2) > 1) {
            } else continue;
        }

        scene->addLine(dx + x1, dy + y1, dx + x2, dy + y2, *pen);

        _x1 = x1;
        _x2 = x2;
        _y1 = y1;
        _y2 = y2;
    }
}

void MainWindow::drawRulers(double scale, int dx, int dy, QGraphicsScene* scene, QSize* table, QPen* pen) {
    QPen topPen(*pen), downPen(*pen);

    // Set table colors
    if (pen->color().lightness() > 127) {
        downPen.setColor(pen->color().darker(200));
    } else {
        downPen.setColor(pen->color().lighter(250));
    }

    topPen.setWidth(1);
    //downPen.setWidth(int((scale + 1) * 5));
    downPen.setWidth(6);

    double x1, x2, y1, y2;

    x1 = dx + table->width() * scale + topPen.width() / 2 + downPen.width() / 2;
    x2 = x1;
    y1 = dy + topPen.width() / 2 + downPen.width();
    y2 = dy + table->height() * scale + topPen.width() / 2 + downPen.width() / 2;
    scene->addLine(x1, y1, x2, y2, downPen);

    x2 = dx + topPen.width() / 2 + downPen.width();
    y1 = y2;
    scene->addLine(x1, y1, x2, y2, downPen);

    // Draw table borders
    scene->addRect(dx - scale, dy - scale, table->width() * scale + scale, table->height() * scale + scale, topPen);
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
    mainIcon->setThemeStyle(IconEngine::NoStyle);

    QIcon fileIcon;
    if (file.suffix() == "jpg" || file.suffix() == "png")
        fileIcon = QIcon::fromTheme("image-x-generic", mainIcon->getByName("image-x-generic"));
    else if (file.suffix() == "svg")
        fileIcon = QIcon::fromTheme("image-x-svg+xml", mainIcon->getByName("image-x-svg+xml"));
    else
        fileIcon = QIcon::fromTheme("text-x-generic", mainIcon->getByName("text-x-generic"));

    mainIcon->setThemeStyle(IconEngine::SystemColor);

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

    for (int i = 0; i < recentFiles.count(); i++) {
        mainSettingsFile->setArrayIndex(i);
        if (recentFiles[i]->isVisible())    mainSettingsFile->setValue("filepath", recentFiles[i]->whatsThis());
        else                                mainSettingsFile->setValue("filepath", "empty");
    }

    mainSettingsFile->endArray();
    mainSettingsFile->endGroup();
}

/// Load recent files list from the settings file
void MainWindow::loadRecentFilesList() {
    int filesCount = mainSettingsFile->beginReadArray("recentfiles/file");

    for (int i = filesCount - 1; i >= 0; i--) {
        mainSettingsFile->setArrayIndex(i);
        if (mainSettingsFile->value("filepath", "empty").toString() != "empty") {
            addRecentFile(mainSettingsFile->value("filepath").toString());
        }
    }

    mainSettingsFile->endArray();
}

/// Save all window data to files
void MainWindow::saveWindowConfiguration() {

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
void MainWindow::loadWindowConfiguration() {

    // Read window size
    int windowWidth = mainSettingsFile->value("mainwindow/width", 900).toInt();
    int windowHeight = mainSettingsFile->value("mainwindow/height", 600).toInt();
    this->resize(windowWidth, windowHeight);

    // Read window full screen mode
    bool fullScreen = mainSettingsFile->value("mainwindow/fullscreen", 0).toBool();
    if (fullScreen) this->showFullScreen();

    // Read window state
    mainWindowStateFile->open(QFile::ReadOnly);
    this->restoreState(mainWindowStateFile->readAll());
    mainWindowStateFile->close();

}

void MainWindow::clearOutput() {
    ui->textEditOutput->clear();
}

void MainWindow::codeChanged() {
    // Check code for enable/disable "Save" button
    if (savedCode == code->toPlainText())   ui->actionSave->setEnabled(false);
    else if (openedFileName != "")          ui->actionSave->setEnabled(true);

    // Read GCode
    readCode(code, previewLine);

    elapsedTimer->start();

    previewUpdate(true);

    previewUpdateTimer->setInterval(int(elapsedTimer->elapsed()) + 1000/MaxFPS);
    sendMessage("FPS " + QString::number(double(1000) / previewUpdateTimer->interval()));
}

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
        messageColor.setRgb(68, 197, 23); // Green color
    else if (message == Message::Information)
        messageColor = QLabel().palette().color(QPalette::WindowText);
    else if (message == Message::Warning)
        messageColor.setRgb(248, 109, 0); // Orange color
    else if (message == Message::Critical)
        messageColor.setRgb(245, 6, 7); // Red color

    // Show the message in Status Bar and Output dock
    currentStatusLabel->setText("<font color=" + messageColor.name() + '>' + status + "</font>");
    ui->textEditOutput->append('[' + QTime::currentTime().toString() + "] " + status);
}

/// Open raster picture
bool MainWindow::openRasterPicture(const QFileInfo& file) {
    return false;
}

/// Open vector picture
bool MainWindow::openVectorPicture(const QFileInfo& file) {


}

/// Open text file
bool MainWindow::openTextFile(const QFileInfo& file) {
//    QFile file(file.filePath());

//    // Try to open the file
//    if (file.open(QFile::ReadOnly | QFile::Text)) {
//        savedCode = file.readAll().toStdString().c_str();
//        code->setPlainText(savedCode);
//        return true;
//    }
//    // If cannot open, return an error message
//    else {
//        return false;
//    }
    return true;
}

void MainWindow::openRecentFile() {
    // Get an action that called this slot
    QAction *action = qobject_cast<QAction *>(sender());

    // If it's actually QAction
    if (action) {

        // Get full file path from QAction text
        QString filePath;
        int pathStartIndex = action->text().indexOf(' ') + 2;
        int pathEndIndex = action->text().length() - 2;
        filePath = action->text().mid(pathStartIndex, pathEndIndex - pathStartIndex + 1);

        addRecentFile(filePath);
        openFile(filePath);
    }
}

void MainWindow::clearRecentFiles() {
    // Hide all file QActions
    for (int i = 0; i < recentFiles.size(); i++) recentFiles[i]->setVisible(false);
    // Hide button "Clear list"
    ui->menuRecentFiles->actions().last()->setVisible(false);
    // Show label "No files"
    ui->menuRecentFiles->actions()[ui->menuRecentFiles->actions().size() - 3]->setVisible(true);
}

/// Open file
void MainWindow::openFile(const QString &filePath, bool silentMode)
{
    QFileInfo file(filePath);
    bool    fileOpenStatus;

    // If the file is a pixel picture
    if (file.suffix() == "png" || file.suffix() == "jpg")
        fileOpenStatus = openRasterPicture(file);

    // If the file is a vector picture
    else if (file.suffix() == "svg")
        fileOpenStatus = openVectorPicture(file);

    // If it's a text file
    else
        fileOpenStatus = openTextFile(file);

    // If file is opened
    if (fileOpenStatus)
        currentFile->setFile(file.absoluteFilePath());

    // Update user interface
    updateGUI();

    if (silentMode) return;

    // Show message if needed
    if (fileOpenStatus)
        sendMessage(tr("«%1» successfully opened").arg(file.fileName()), Message::Successfully);
    else
        sendMessage(tr("Could not open «%1»").arg(file.fileName()), Message::Warning);
}

/// Save file
void MainWindow::saveFile(const QString& filePath) {
//    // Write code to the file
//    QFile file(filePath);
//    if (file.open(QFile::WriteOnly)) {
//        QByteArray array = code->toPlainText().toLocal8Bit();
//        file.write(array);

//        // Close file
//        file.close();

//        sendMessage(tr("«%1» saved successfully").arg(filePath));
//    } else {
//        sendMessage(tr("Could not save as «%1»").arg(filePath));
//        return;
//    }

//    // Open just created file
//    addRecentFile(filePath);
//    openFile(filePath, true);
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::on_actionAbout_triggered()
{
    AboutWindow about;
    about.setAppDir(applicationDirectory->path());
    about.setModal(true);
    int x = this->geometry().x() + (this->geometry().width() - about.geometry().width()) / 2,
        y = this->geometry().y() + (this->geometry().height() - about.geometry().height()) / 2;
    about.setGeometry(x, y, about.geometry().width(), about.geometry().height());
    about.exec();
}

void MainWindow::on_actionSave_triggered()
{
    QString filePath;

    if (openedFileName.isEmpty() || openedFileType != 0) {
        // Get the file path
        QFileDialog fileName;
        filePath = fileName.getSaveFileName(this, "", "", tr("Text file") + "(*.gcode *.gc *.txt)");

        // If file wasn't selected, just exit
        if (filePath.isEmpty()) return;

    } else filePath = openedFileName;

    saveFile(filePath);
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog getFileName;
    QString filePath = getFileName.getOpenFileName(this);
    if (filePath == "") return;
    addRecentFile(filePath);
    openFile(filePath);
}

void MainWindow::on_actionClose_triggered()
{
    sendMessage(tr("\"%1\" closed").arg(currentFile->fileName()));
    currentFile->setFile(QString());

    savedCode.clear();
    code->clear();

    updateGUI();
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString filePath;

    // Get the file path
    QFileDialog fileName;
    filePath = fileName.getSaveFileName(this, "", "", tr("Text file") + "(*.gcode *.gc *.txt)");

    // If file wasn't selected, just exit
    if (filePath.isEmpty()) return;

    saveFile(filePath);
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

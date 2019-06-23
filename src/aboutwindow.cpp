#include "aboutwindow.h"
#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);

    iconsDirectory = new QDir;

    // Настраиваем надпись с названием приложения
    QFont nameFont(QApplication::font());
    nameFont.setPointSize(int(QApplication::font().pointSize() * 1.6));
    ui->labelApp->setText(QString("<style>p{margin:0; padding:0; font-size:") + QString::number(nameFont.pointSize()) + "pt}</style><p>" + QCoreApplication::applicationName() + "</p>" + tr("Version: %1").arg(AppVersion));

    ui->labelAboutContent->setText(tr("%1 — an application written in C++ using the Qt Framework for working with CNC equipment, such as plotters and drawing machines.")
                                   .arg(QCoreApplication::applicationName()));

    // Указываем версию приложения
    ui->labelLicence->setText("<style> a { text-decoration: none; }</style><a href=#>" + tr("Licence: %1, version 3").arg("GNU General Public License") + "</a>");

    ui->labelProjectPage->setText(QString("<style>a{text-decoration:none;color:") + QLabel().palette().color(QPalette::WindowText).name() +
                                  ";}</style><a href=\"" + ProjectLink + "\">" + tr("Project page on GitHub") + "</a>");
    ui->labelProjectPage->setOpenExternalLinks(true);
    ui->labelProjectPage->setToolTip(tr("Visit %1").arg(ProjectLink));

    ui->labelUsedItems->setOpenExternalLinks(true);
    ui->labelUsedItems->setText(QString("<ul>") +
                                "<li>" + tr("%1Qt%2").arg("<a href=\"https://www.qt.io\">").arg("</a>") + ": LGPLv3 + GPLv3" + "</li>" +
                                "<li>" + tr("%1Papirus icon theme%2").arg("<a href=\"https://github.com/PapirusDevelopmentTeam/papirus-icon-theme\">").arg("</a>") + ": GPLv3" + "</li>" +
                                "<li>" + tr("%1SVG++ library%2").arg("<a href=\"https://github.com/svgpp/svgpp\">").arg("</a>") + ": Boost Software License 1.0" + "</li>" +
                                "</ul>");

    ui->labelQtVersion->setText(tr("Built on version %1").arg(qVersion()));

    ui->label1->setText(tr("%1 is free software; you can distribute and / or modify it according to the terms of the %2GNU General Public License%3.")
                        .arg(QCoreApplication::applicationName())
                        .arg("<i>")
                        .arg("</i>"));

    ui->label2->setText(tr("The program is created on pure enthusiasm in the hope that it will be useful to someone. This means that there are no guarantees for the correct operation of the application."));

    ui->label3->setText(tr("Thank you for using %1!").arg(QCoreApplication::applicationName()));
}

void AboutWindow::setAppDir(const QString &path) {
    iconsDirectory->setPath(path + "/icons");
    updatePixmaps();
}

void AboutWindow::updatePixmaps() {

    // Show Git logo
    QImage gitLogoImage(iconsDirectory->path() + "/GitHubLogo.svg");

    gitLogoImage = gitLogoImage.scaledToHeight(256, Qt::FastTransformation);

    for (int x = 0; x < gitLogoImage.width(); x++)
        for (int y = 0; y < gitLogoImage.height(); y++)
            if ((gitLogoImage.pixel(x, y) & 0xFF000000) >> 6 > 10) gitLogoImage.setPixelColor(x, y, QLabel().palette().color(QPalette::WindowText));

    QPixmap gitLogo(QPixmap::fromImage(gitLogoImage));
    gitLogo.setDevicePixelRatio(qApp->devicePixelRatio());
    gitLogo = gitLogo.scaledToHeight(int(qApp->devicePixelRatio() * 32), Qt::SmoothTransformation);
    ui->labelGitLogo->setPixmap(gitLogo);

    // Show application logo
    QPixmap appLogo(iconsDirectory->path() + "/"+ QCoreApplication::applicationName() + "Logo.svg.png");
    appLogo.setDevicePixelRatio(qApp->devicePixelRatio());
    ui->labelAppLogo->setPixmap(appLogo.scaledToHeight(int(qApp->devicePixelRatio() * 80), Qt::SmoothTransformation));

    // Show Qt logo
    QPixmap qtLogo(iconsDirectory->path() + "/QtLogo.svg");
    qtLogo.setDevicePixelRatio(qApp->devicePixelRatio());
    ui->labelQtLogo->setPixmap(qtLogo.scaledToHeight(int(qApp->devicePixelRatio() * 20), Qt::SmoothTransformation));
}

AboutWindow::~AboutWindow()
{
    delete ui;
}

void AboutWindow::on_pushButtonOk_clicked()
{
    close();
}

void AboutWindow::on_labelLicence_linkActivated(const QString &link)
{
    if (link == "#")
    {
        AboutLicence licence;
        licence.setModal(true);
        licence.exec();
    }
}

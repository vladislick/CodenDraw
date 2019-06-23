#include "aboutlicence.h"
#include "ui_aboutlicence.h"

AboutLicence::AboutLicence(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutLicence)
{
    ui->setupUi(this);

}

AboutLicence::~AboutLicence()
{
    delete ui;
}

void AboutLicence::on_pushButton_clicked()
{
    this->close();
}

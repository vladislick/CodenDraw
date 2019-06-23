#ifndef ABOUTLICENCE_H
#define ABOUTLICENCE_H

#include <QString>
#include <QDialog>

namespace Ui {
class AboutLicence;
}

class AboutLicence : public QDialog
{
    Q_OBJECT

public:
    explicit AboutLicence(QWidget *parent = nullptr);
    ~AboutLicence();

private slots:
    void on_pushButton_clicked();

private:
    Ui::AboutLicence *ui;
};

#endif // ABOUTLICENCE_H


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    qApp->quit();
}

void MainWindow::on_addLocationToolButton_clicked()
{
    goofBox();
}

void MainWindow::on_updateRecordsPushButton_clicked()
{
    goofBox();
}

void goofBox()
{
    QMessageBox msgBox;
    msgBox.setText("Please, do not click me there.");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.exec();

}


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    loadLocales(ui);
}

void loadLocales(Ui::MainWindow *z)
{
    // Check if locale file exists
    QFileInfo checkLocaleFile(LOCALEFILEPATH);
    if(!checkLocaleFile.exists())
    {
        // Yell if it doesn't exist
        QMessageBox msgBox;
        msgBox.setText("The locales file does not exist.\nA generic one will be created for you.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();

        // Create a new generic locales file
        QFile localeFile(LOCALEFILEPATH);
        if(localeFile.open(QIODevice::WriteOnly))
        {
            QTextStream stream( &localeFile );
            stream << "Stavromula Beta" << endl;
        }
        localeFile.close();
    }
    // If it does exist, load it
    QFile localeFile(LOCALEFILEPATH);
    if(localeFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&localeFile);
        while(!in.atEnd())
        {
            QString line = in.readLine();
            z->locationComboBox->addItem(line);
        }
    }
    localeFile.close();
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

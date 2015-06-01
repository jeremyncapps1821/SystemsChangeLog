#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QFileInfo>
#include <QMainWindow>
#include <QMessageBox>
#include <QTextStream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_addLocationToolButton_clicked();

    void on_updateRecordsPushButton_clicked();

private:
    Ui::MainWindow *ui;
};

// Prototypes
void goofBox();
void loadLocales(Ui::MainWindow *z);

// Constants
const QString LOCALEFILEPATH = "data/config/locations";
const QString CHANGELOGDBPATH = "data/sql/changeLog.db";

#endif // MAINWINDOW_H

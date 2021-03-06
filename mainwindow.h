#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "filterbydialog.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    eventLogViewer *eventlogviewer;

    FilterByDialog *filterbydialog;

    BrowseLogsDialog *browselogsdialog;

    Ui::MainWindow *ui;

private slots:
    void on_actionExit_triggered();

    void on_addLocationToolButton_clicked();

    void on_updateRecordsPushButton_clicked();

    void on_actionExport_Records_By_triggered();

    void on_actionBrowse_Logs_triggered();

    void on_actionWindows_Event_Logs_triggered();
    void on_actionAbout_triggered();
    void on_addDeviceToolButton_clicked();
    void on_locationComboBox_currentIndexChanged(const QString &arg1);
};

// Prototypes
void goofBox();
void loadLocales(Ui::MainWindow *z);
void loadDevices(Ui::MainWindow *z);
void buildDatabase();
void openDatabase();
void closeDatabase();

// Constants
const QString CHANGELOGDBPATH = "./data/sql/changeLog.db";

#endif // MAINWINDOW_H

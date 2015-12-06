#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    QStringList args = qApp->arguments();
    openDatabase();
    loadLocales(ui);
    //loadDevices(ui);
}

void loadLocales(Ui::MainWindow *z)
{
    // Connect to db and prepare query
    QSqlDatabase db;
    db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query;
    query.prepare("SELECT * FROM locales");

    // Load locales from db
    if(query.exec())
    {
        while(query.next())
        {
            z->locationComboBox->addItem(query.value(1).toString());
        }
    }
}

void loadDevices(Ui::MainWindow *z)
{
    // Connect to db and prepare query
    QSqlDatabase db;
    db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query;
    QString queryText = QString("SELECT * FROM devices WHERE locale IS '%1'").arg(z->locationComboBox->currentText());
    query.prepare(queryText);

    // Load devices from db
    if(query.exec())
    {
        while(query.next())
        {
            z->deviceComboBox->addItem(query.value(1).toString());
        }
    }
}

MainWindow::~MainWindow()
{
    closeDatabase();
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    closeDatabase();
    qApp->quit();
}

void MainWindow::on_addLocationToolButton_clicked()
{
    // Get New Location Name
    bool ok;
    QString location = QInputDialog::getText(NULL, "New Location",
                                             "Location name:", QLineEdit::Normal,
                                             "Stavromula Beta", &ok);

    // Add to combo box and locales table
    if(ok && !location.isEmpty())
    {
        ui->locationComboBox->addItem(location);
        QSqlDatabase db;
        db = QSqlDatabase::database("QSQLITE");
        QSqlQuery query;
        query.exec(QString("INSERT INTO locales VALUES(NULL, '%1')").arg(location));
    }

}

void MainWindow::on_addDeviceToolButton_clicked()
{
    // Get New Device ID
    bool ok;
    QString device = QInputDialog::getText(NULL, "New Device",
                                           "Device ID:", QLineEdit::Normal,
                                           "42", &ok);

    // Add to combo box and devices table
    if(ok && !device.isEmpty())
    {
        ui->deviceComboBox->addItem(device);
        QSqlDatabase db;
        db = QSqlDatabase::database("QSQLITE");
        QSqlQuery query;
        query.exec(QString("INSERT INTO devices VALUES(NULL, '%1', '%2', NULL, NULL, NULL)").arg(device).arg(ui->locationComboBox->currentText()));
    }
}

void MainWindow::on_updateRecordsPushButton_clicked()
{
    // Connect to Database
    QSqlDatabase db;
    db = QSqlDatabase::database("QSQLITE");

    // Collect Data From Form and Add Record to DB
    QSqlQuery query;
    query.exec(QString("INSERT INTO changes VALUES(NULL,'%1',"
                       "'%2','%3','%4','%5','%6')")
               .arg(ui->dateEdit->date().toString()).arg(ui->locationComboBox->currentText()).arg(ui->deviceComboBox->currentText())
               .arg(ui->notesTextEdit->toPlainText()).arg(ui->timeInTimeEdit->time().msecsSinceStartOfDay())
               .arg(ui->timeOutTimeEdit->time().msecsSinceStartOfDay()));

    // Reset Form
    ui->notesTextEdit->clear();
}

void goofBox()
{
    QMessageBox msgBox;
    msgBox.setText("Please, do not click me there.");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.exec();
}

void buildDatabase()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(CHANGELOGDBPATH);
    db.open();

    // Build the tables
    if(db.isOpen())
    {
        QSqlQuery query;
        query.exec("CREATE table changes "
                   "(id integer primary key, "
                   "date text, "
                   "location text, "
                   "deviceID text, "
                   "notes text, "
                   "timeIn text, "
                   "timeOut text)");
        query.exec("CREATE table locales "
                   "(id integer primary key, "
                   "location text)");
        query.exec("CREATE table eventlogs "
                   "(id integer primary key, "
                   "date text, "
                   "time text, "
                   "hostname text, "
                   "source text, "
                   "eventid text, "
                   "description text)");
        query.exec("CREATE table devices "
                   "(id integer primary key, "
                   "deviceID text, "
                   "locale text, "
                   "hostName text, "
                   "ipAddr text, "
                   "fqdn text)");
    }
}

void openDatabase()
{
    QFileInfo checkChangeLogDB(CHANGELOGDBPATH);
    if (!checkChangeLogDB.exists())
    {
        QMessageBox msgBox;
        msgBox.setText("The database could not be found.\n A new one will be generated.");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();

        buildDatabase();
        return;
    }

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(CHANGELOGDBPATH);
    db.open();
}

void closeDatabase()
{
    QSqlDatabase db;
    db = QSqlDatabase::database("QSQLITE");
    db.close();
}

void MainWindow::on_actionExport_Records_By_triggered()
{
    filterbydialog = new FilterByDialog;
    filterbydialog->show();
}

void MainWindow::on_actionBrowse_Logs_triggered()
{
    browselogsdialog = new BrowseLogsDialog;
    browselogsdialog->show();
}

void MainWindow::on_actionWindows_Event_Logs_triggered()
{
    eventlogviewer = new eventLogViewer;
    eventlogviewer->show();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About", "Systems Change Log v1.2.1\nCopyright (c) 2015 Jeremy N Capps\nFor complete licensing information see the 'LICENSE' file contained in the root of the program folder.");
}

void MainWindow::on_locationComboBox_currentIndexChanged(const QString &arg1)
{
    ui->deviceComboBox->clear();
    loadDevices(ui);
}

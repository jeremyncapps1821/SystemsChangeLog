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

void MainWindow::on_updateRecordsPushButton_clicked()
{
    // Connect to Database
    QSqlDatabase db;
    db = QSqlDatabase::database("QSQLITE");

    // Collect Data From Form and Add Record to DB
    QSqlQuery query;
    query.exec(QString("INSERT INTO changes VALUES(NULL,'%1',"
                       "'%2','%3','%4','%5','%6')")
               .arg(ui->dateEdit->date().toString()).arg(ui->locationComboBox->currentText()).arg(ui->deviceIDLineEdit->text())
               .arg(ui->notesTextEdit->toPlainText()).arg(ui->timeInTimeEdit->time().msecsSinceStartOfDay())
               .arg(ui->timeOutTimeEdit->time().msecsSinceStartOfDay()));

    // Reset Form
    ui->deviceIDLineEdit->clear();
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
        query.exec("CREATE table preferences "
                   "(id integer primary key, "
                   "serverAddress text, "
                   "serverPort text, "
                   "logStyle text)");
        query.exec("CREATE table eventlogs "
                   "(id integer primary key, "
                   "message text, "
                   "eventId text, "
                   "version text, "
                   "qualifiers text, "
                   "level text, "
                   "task text, "
                   "opcode text, "
                   "keywords text, "
                   "recordId text, "
                   "providerName text, "
                   "providerId text, "
                   "logName text, "
                   "processId text, "
                   "threadId text, "
                   "machineName text, "
                   "userId text, "
                   "dateTime text, "
                   "activityId text, "
                   "relatedActivityId text, "
                   "containerLog text, "
                   "matchedQueryIds text, "
                   "bookmark text, "
                   "levelDisplayName text, "
                   "opcodeDisplayName text, "
                   "taskDisplayName text, "
                   "keywordsDisplayNames text, "
                   "properties text)");
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

void MainWindow::on_actionPreferences_triggered()
{
    prefsdialog = new PrefsDialog;
    prefsdialog->show();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About", "Sawmill Viewer v2.0.0-alpha\nCopyright (c) 2015 Jeremy N Capps\nFor complete licensing information see the 'LICENSE' file contained in the root of the program folder.");
}

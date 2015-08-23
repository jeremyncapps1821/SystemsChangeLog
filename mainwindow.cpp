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

    // Build the table
    if(db.isOpen())
    {
        QSqlQuery query;
        query.exec("create table changes "
                   "(id integer primary key, "
                   "date text, "
                   "location text, "
                   "deviceID text, "
                   "notes text, "
                   "timeIn text, "
                   "timeOut text)");
        query.exec("create table locales "
                   "(id integer primary key, "
                   "location text)");
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

void generateReport(int reportType)
{
    // Connect to database and Open our output file
    QSqlDatabase db;
    db = QSqlDatabase::database("QSQLITE");
    QFile reportFile(EXPORTDIRECTORY);

    // Begin SQL Query Prep
    QSqlQuery * query = new QSqlQuery(db);

    // Generate report based on user's selection 'reportType'
    if(reportFile.open(QIODevice::WriteOnly))
    {
        QTextStream stream( &reportFile );
        stream << "<html>\n<head><title>Systems Change Log Report</title></head>\n<body>" << endl;

        // Prepare Query
        if(reportType == 0)
        {
            query->prepare("SELECT * FROM changes");
        }
        else if(reportType == 1)
        {
            bool ok;
            QString devID = QInputDialog::getText(NULL, "Filter by ID",
                                                  "Device ID:", QLineEdit::Normal,
                                                  "42", &ok);

            query->prepare("SELECT * FROM changes WHERE deviceID = '" + devID + "'");
        }
        else if(reportType == 2)
        {
            bool ok;
            QString locName = QInputDialog::getText(NULL, "Filter by Location",
                                                    "Location Name:", QLineEdit::Normal,
                                                    "Stavromuller Beta", &ok);

            query->prepare("SELECT * FROM changes WHERE location = '" + locName + "'");
        }

        // Iterate through Query Results and write to html file
        if(query->exec())
        {
            while(query->next())
            {
                stream << query->value(1).toString() + " " + query->value(2).toString() + " " + query->value(3).toString() + " " << endl;
                stream << query->value(4).toString() + " " + query->value(5).toString() + " " + query->value(6).toString() + "</br>" << endl;
            }
        }

        stream << "</body>\n</html>" << endl;

        reportFile.close();
    }
}

void MainWindow::on_actionExport_All_Records_triggered()
{
    generateReport(0);
}

void MainWindow::on_actionBy_Device_ID_triggered()
{
    generateReport(1);
}

void MainWindow::on_actionBy_Location_triggered()
{
    generateReport(2);
}

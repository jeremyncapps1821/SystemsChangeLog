#include "browselogsdialog.h"
#include "ui_browselogsdialog.h"

BrowseLogsDialog::BrowseLogsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrowseLogsDialog)
{
    ui->setupUi(this);
    buildTable(ui);
}

BrowseLogsDialog::~BrowseLogsDialog()
{
    delete ui;
}

void buildTable(Ui::BrowseLogsDialog *z)
{
    // Connect to Database
    QSqlDatabase db;
    db = QSqlDatabase::database("QSQLITE");

    // Query Database
    QSqlQueryModel * modal = new QSqlQueryModel();
    QSqlQuery * query = new QSqlQuery(db);
    query->prepare("SELECT * FROM changes");
    query->exec();

    // Configure model
    modal->setQuery(*query);
    modal->setHeaderData(1, Qt::Horizontal, QObject::tr("Date"));
    modal->setHeaderData(2, Qt::Horizontal, QObject::tr("Location"));
    modal->setHeaderData(3, Qt::Horizontal, QObject::tr("Device ID"));
    modal->setHeaderData(4, Qt::Horizontal, QObject::tr("Notes"));

    // Configure Table Appearance
    z->browseLogsTableView->setModel(modal);
    z->browseLogsTableView->verticalHeader()->setVisible(false);
    z->browseLogsTableView->hideColumn(0);
    z->browseLogsTableView->hideColumn(5);
    z->browseLogsTableView->hideColumn(6);
    z->browseLogsTableView->setColumnWidth(1, 135);
    z->browseLogsTableView->setColumnWidth(2, 200);
    z->browseLogsTableView->setColumnWidth(3, 80);
    z->browseLogsTableView->horizontalHeader()->setStretchLastSection(true);
    z->browseLogsTableView->resizeRowsToContents();
}

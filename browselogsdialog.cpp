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

    // Configure Tree View Appearance
    z->treeView->setModel(modal);
    z->treeView->setColumnWidth(1, 135);
    z->treeView->setColumnWidth(2, 200);
    z->treeView->setColumnWidth(3, 80);
    z->treeView->hideColumn(0);
    z->treeView->hideColumn(5);
    z->treeView->hideColumn(6);
}

void BrowseLogsDialog::on_treeView_doubleClicked(const QModelIndex &index)
{
    QString id;

    QModelIndex idx = ui->treeView->model()->index(index.row(), 0, index.parent());

    if(idx.isValid())
    {
        id = idx.data(Qt::DisplayRole).toString();
    }
}

#ifndef BROWSELOGSDIALOG_H
#define BROWSELOGSDIALOG_H

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTextStream>

namespace Ui {
class BrowseLogsDialog;
}

class BrowseLogsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrowseLogsDialog(QWidget *parent = 0);
    ~BrowseLogsDialog();

private:
    Ui::BrowseLogsDialog *ui;
};

// Prototypes
void buildTable(Ui::BrowseLogsDialog *z);

#endif // BROWSELOGSDIALOG_H

#ifndef BROWSELOGSDIALOG_H
#define BROWSELOGSDIALOG_H

#include "eventlogviewer.h"

namespace Ui {
class BrowseLogsDialog;
}

class BrowseLogsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrowseLogsDialog(QWidget *parent = 0);

    ~BrowseLogsDialog();

    LogDetailsDialog *logdetailsdialog;

private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_pushButton_clicked();

private:
    Ui::BrowseLogsDialog *ui;
};

// Prototypes
void buildTable(Ui::BrowseLogsDialog *z);

#endif // BROWSELOGSDIALOG_H

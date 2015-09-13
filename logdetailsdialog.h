#ifndef LOGDETAILSDIALOG_H
#define LOGDETAILSDIALOG_H

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTextStream>

namespace Ui {
class LogDetailsDialog;
}

class LogDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogDetailsDialog(QWidget *parent = 0);

    ~LogDetailsDialog();

    Ui::LogDetailsDialog *ui;

public slots:
    void receiveData(QString id, int logType);
};

#endif // LOGDETAILSDIALOG_H

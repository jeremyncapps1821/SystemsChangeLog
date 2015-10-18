#include "logdetailsdialog.h"
#include "ui_logdetailsdialog.h"

LogDetailsDialog::LogDetailsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogDetailsDialog)
{
    ui->setupUi(this);
}

LogDetailsDialog::~LogDetailsDialog()
{
    delete ui;
}

void LogDetailsDialog::receiveData(QString id, int logType)
{
    /*
     * Log Type 0 = Change Log
     * Log Type 1 = Event Log
     */

    // Connect to database and initialize query
    QSqlDatabase db;
    db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query;

    // Build Query
    if(logType == 0)
    {
        query.prepare(QString("SELECT * FROM changes WHERE id=%1").arg(id));
    }
    else if(logType == 1)
    {
        query.prepare(QString("SELECT * FROM eventlogs WHERE id=%1").arg(id));

    }

    // Query Database
    if(query.exec())
    {
        while(query.next())
        {
            if(logType == 0)
            {
                // Convert time to hrs
                bool ok;
                QString end = query.value(6).toString();
                QString start = query.value(5).toString();
                float endTime = (float) end.toInt(&ok,10);
                float startTime = (float) start.toInt(&ok,10);
                float duration = ((endTime - startTime) / 60000) / 60;
                QString time = QString::number(duration, 'f', 2);

                // Edit Layout
                ui->hrsWorkedLabel->setVisible(1);
                ui->hrsWorkedTextLabel->setVisible(1);
                ui->timeLabel->setVisible(0);
                ui->timeTextLabel->setVisible(0);
                ui->sourceLabel->setVisible(0);
                ui->sourceTextLabel->setVisible(0);
                ui->dateTextLabel->setText(QString("%1").arg(query.value(1).toString()));
                ui->locationLabel->setText("Location:");
                ui->deviceIdLabel->setText("Device ID:");
                ui->locationTextLabel->setText(QString("%1").arg(query.value(2).toString()));
                ui->deviceIdTextLabel->setText(QString("%1").arg(query.value(3).toString()));
                ui->descripTextBrowser->setText(QString("%1").arg(query.value(4).toString()));
                ui->hrsWorkedTextLabel->setText(time);
            }
            else if(logType == 1)
            {
                ui->hrsWorkedLabel->setVisible(0);
                ui->hrsWorkedTextLabel->setVisible(0);
                ui->sourceLabel->setVisible(1);
                ui->sourceTextLabel->setVisible(1);
                ui->timeLabel->setVisible(1);
                ui->timeTextLabel->setVisible(1);
                ui->timeTextLabel->setText(QString("%1").arg(query.value(2).toString()));
                ui->dateTextLabel->setText(QString("%1").arg(query.value(1).toString()));
                ui->locationLabel->setText("Hostname:");
                ui->locationTextLabel->setText(QString("%1").arg(query.value(3).toString()));
                ui->deviceIdTextLabel->setText(QString("%1").arg(query.value(5).toString()));
                ui->deviceIdLabel->setText("Event ID:");
                ui->sourceTextLabel->setText(QString("%1").arg(query.value(4).toString()));
                ui->descripTextBrowser->setText(QString("%1").arg(query.value(6).toString()));
            }
        }
    }
}

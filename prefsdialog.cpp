#include "prefsdialog.h"
#include "ui_prefsdialog.h"

PrefsDialog::PrefsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrefsDialog)
{
    ui->setupUi(this);
    QStringList logStylesList;
    logStylesList << "Powershell" << "Eventlog to Syslog";
    ui->logStyleComboBox->addItems(logStylesList);

    QSqlDatabase db;
    db = QSqlDatabase::database("QSQLITE");
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM preferences"));
    if(query.exec())
    {
        while(query.next())
        {
            ui->serverAddressLineEdit->setText(QString("%1").arg(query.value(1).toString()));
            int portNumber = query.value(2).toInt();
            ui->serverPortSpinBox->setValue(portNumber);
            ui->logStyleComboBox->setCurrentText(QString("%1").arg(query.value(3).toString()));
            ui->warningCheckBox->setChecked(query.value(4).toInt());
            ui->errorCheckBox->setChecked(query.value(5).toInt());
            ui->criticalCheckBox->setChecked(query.value(6).toInt());
            ui->eventsLineEdit->setText(query.value(7).toString());
        }
    }
}

PrefsDialog::~PrefsDialog()
{
    delete ui;
}

void PrefsDialog::emptyPreferences()
{
    // Connect to database
    QSqlDatabase db;
    db = QSqlDatabase::database("QSQLITE");

    // Delete Log Entries
    QSqlQuery query;
    query.exec(QString("DELETE FROM preferences"));
}

void PrefsDialog::on_buttonBox_accepted()
{
    emptyPreferences();

    QSqlDatabase db;
    db = QSqlDatabase::database("QSQLITE");

    QSqlQuery query;
    query.exec(QString("INSERT INTO preferences VALUES(NULL,'%1','%2','%3','%4','%5','%6','%7')")
               .arg(ui->serverAddressLineEdit->text()).arg(ui->serverPortSpinBox->value())
               .arg(ui->logStyleComboBox->currentText()).arg(ui->warningCheckBox->isChecked())
               .arg(ui->errorCheckBox->isChecked()).arg(ui->criticalCheckBox->isChecked())
               .arg(ui->eventsLineEdit->text()));
}

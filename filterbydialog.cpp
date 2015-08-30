#include "filterbydialog.h"
#include "ui_filterbydialog.h"

FilterByDialog::FilterByDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterByDialog)
{
    ui->setupUi(this);

    // Connect to database and fill widgets
    QSqlDatabase db;
    db = QSqlDatabase::database("QSQLITE");
    QSqlQuery * query = new QSqlQuery(db);
    query->prepare("SELECT * FROM locales");
    if(query->exec())
    {
        while(query->next())
        {
            ui->locationComboBox->addItem(query->value(1).toString());
        }
    }
}

FilterByDialog::~FilterByDialog()
{
    delete ui;
}

void generateReport(Ui::FilterByDialog *z)
{
    // Connect to database and Open our output file
    QSqlDatabase db;
    db = QSqlDatabase::database("QSQLITE");
    QFile reportFile(EXPORTDIRECTORY);

    // Begin SQL Query Prep
    QSqlQuery * query = new QSqlQuery(db);

    // Generate report
    if(reportFile.open(QIODevice::WriteOnly))
    {
        QTextStream stream( &reportFile );
        // Setup web page
        stream << "<html>\n<head><title>Systems Change Log Report</title></head>\n<body>\n" << endl;
        stream << "<table border='1' style='width:100%'>\n<tr>\n<th>Date</th><th>Locale</th><th>Device ID</th><th>Comments</th><th>Duration (hrs)</th></tr>\n" << endl;

        // Prepare Query
        query->prepare(buildQuery(z));

        // Iterate through Query Results and write to html file
        if(query->exec())
        {
            while(query->next())
            {
                bool ok;
                QString end = query->value(6).toString();
                QString start = query->value(5).toString();
                float endTime = (float) end.toInt(&ok,10);
                float startTime = (float) start.toInt(&ok,10);
                float duration = ((endTime - startTime) / 60000) / 60;
                QString time = QString::number(duration, 'f', 2);
                stream << "<tr><td>" + query->value(1).toString() + "</td><td>" + query->value(2).toString() + "</td><td>" + query->value(3).toString() + "</td><td>" << endl;
                stream << query->value(4).toString() + "</td><td>" + time + "</td></tr>" << endl;
            }
        }

        stream << "</table>\n</body>\n</html>" << endl;

        reportFile.close();
    }
}

QString buildQuery(Ui::FilterByDialog *z)
{
    QString queryPrep = "SELECT * FROM changes";
    bool multiFilter = 0;

    if(!z->locationCheckBox->isChecked() && !z->dateCheckBox->isChecked() && !z->deviceIdCheckBox->isChecked())
    {
        queryPrep = "SELECT * FROM changes";
        return queryPrep;
    }

    queryPrep = queryPrep + " WHERE";

    if(z->locationCheckBox->isChecked())
    {
        queryPrep = queryPrep + " location='" + z->locationComboBox->currentText() + "'";
        multiFilter = 1;
    }

    if(z->deviceIdCheckBox->isChecked())
    {
        if(multiFilter)
        {
            queryPrep = queryPrep + " AND";
        }
        queryPrep = queryPrep + " deviceID='" + z->deviceIdLineEdit->text() + "'";
        multiFilter = 1;
    }

    if(z->dateCheckBox->isChecked())
    {
        if(multiFilter)
        {
            queryPrep = queryPrep + " AND";
        }
        queryPrep = queryPrep + " date='" + z->dateEdit->date().toString() + "'";
        multiFilter = 1;
    }

    return queryPrep;
}

void FilterByDialog::on_buttonBox_accepted()
{
    generateReport(ui);
}

void FilterByDialog::on_locationCheckBox_toggled(bool checked)
{
    if(checked)
    {
        ui->locationComboBox->setEnabled(true);
    }
    else if(!checked)
    {
        ui->locationComboBox->setEnabled(false);
    }
}

void FilterByDialog::on_dateCheckBox_toggled(bool checked)
{
    if(checked)
    {
        ui->dateEdit->setEnabled(true);
    }
    else if(!checked)
    {
        ui->dateEdit->setEnabled(false);
    }
}

void FilterByDialog::on_deviceIdCheckBox_toggled(bool checked)
{
    if(checked)
    {
        ui->deviceIdLineEdit->setEnabled(true);
    }
    else if(!checked)
    {
        ui->deviceIdLineEdit->setEnabled(false);
    }
}

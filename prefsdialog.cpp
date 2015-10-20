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
}

PrefsDialog::~PrefsDialog()
{
    delete ui;
}

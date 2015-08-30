#ifndef FILTERBYDIALOG_H
#define FILTERBYDIALOG_H

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTextStream>

namespace Ui {
class FilterByDialog;
}

class FilterByDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterByDialog(QWidget *parent = 0);
    ~FilterByDialog();

private slots:
    void on_buttonBox_accepted();

    void on_locationCheckBox_toggled(bool checked);

    void on_dateCheckBox_toggled(bool checked);

    void on_deviceIdCheckBox_toggled(bool checked);

private:
    Ui::FilterByDialog *ui;
};

// Prototypes
void generateReport(Ui::FilterByDialog *z);
QString buildQuery(Ui::FilterByDialog *z);

#endif // FILTERBYDIALOG_H

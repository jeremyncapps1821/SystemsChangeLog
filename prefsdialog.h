#ifndef PREFSDIALOG_H
#define PREFSDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class PrefsDialog;
}

class PrefsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrefsDialog(QWidget *parent = 0);
    ~PrefsDialog();
    void emptyPreferences();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::PrefsDialog *ui;
};

#endif // PREFSDIALOG_H

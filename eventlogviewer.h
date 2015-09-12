#ifndef EVENTLOGVIEWER_H
#define EVENTLOGVIEWER_H

#include "browselogsdialog.h"
#include <QMainWindow>

namespace Ui {
class eventLogViewer;
}

class eventLogViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit eventLogViewer(QWidget *parent = 0);
    ~eventLogViewer();

private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);

private:
    Ui::eventLogViewer *ui;
};

// Prototypes
void importEventLog(QString logPath);
void emptyEventLogs();

#endif // EVENTLOGVIEWER_H

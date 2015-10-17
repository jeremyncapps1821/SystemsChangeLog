#ifndef EVENTLOGVIEWER_H
#define EVENTLOGVIEWER_H

#include "logdetailsdialog.h"
#include <QCloseEvent>
#include <QMainWindow>
#include <QMessageBox>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QHostAddress>


namespace Ui {
class eventLogViewer;
}

class eventLogViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit eventLogViewer(QWidget *parent = 0);

    ~eventLogViewer();

    LogDetailsDialog *logdetailsdialog;

private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);

    void connectionInit();

    void handleError(QAbstractSocket::SocketError err);

    void dataArrived();

private:
    Ui::eventLogViewer *ui;

    QTcpSocket tcpSocket;

    void prepNetwork();

    void closeEvent(QCloseEvent *event);
};

// Prototypes
void importEventLog(QString logPath);
void emptyEventLogs();

#endif // EVENTLOGVIEWER_H

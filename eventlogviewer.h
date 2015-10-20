#ifndef EVENTLOGVIEWER_H
#define EVENTLOGVIEWER_H

#include "logdetailsdialog.h"
#include "prefsdialog.h"
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

    void doConnect();

    bool handleIncoming(QByteArray data);
    void saveLog(QByteArray data);

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);
    void on_clrLogsButton_clicked();

private:
    Ui::eventLogViewer *ui;
    QTcpSocket *socket;
    void closeEvent(QCloseEvent *event);
    void setupTable();
    void importEventLog(QString logPath, int logType);
};

// Prototypes

void emptyEventLogs();

#endif // EVENTLOGVIEWER_H

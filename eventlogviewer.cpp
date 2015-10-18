#include "eventlogviewer.h"
#include "ui_eventlogviewer.h"

eventLogViewer::eventLogViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::eventLogViewer)
{
    ui->setupUi(this);

    // Setup our functions to handle the connection to server
    connect(&tcpSocket, SIGNAL(connected()), this, SLOT(connectionInit()));

    connect(&tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));

    connect(&tcpSocket, SIGNAL(readyRead()), this, SLOT(dataArrived()));


    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setWindowTitle("Select the event log you wish to view...");

    QString logFile = "data/new_logs";

    prepNetwork();

    /*if(dialog.exec())
    {
        logFile = dialog.selectedFiles()[0];
    }*/

    // Setup database table and import log file
    emptyEventLogs();
    importEventLog(logFile);
    setupTable();

}

void eventLogViewer::setupTable()
{
    // Connect to database
    QSqlDatabase db;
    db = QSqlDatabase::database("QSQLITE");

    // Query database
    QSqlQueryModel * modal = new QSqlQueryModel();
    QSqlQuery * query = new QSqlQuery(db);
    query->prepare("SELECT id,date,time,eventid,source FROM eventlogs");
    query->exec();

    // Configure model
    modal->setQuery(*query);
    modal->setHeaderData(1, Qt::Horizontal, QObject::tr("Date"));
    modal->setHeaderData(2, Qt::Horizontal, QObject::tr("Time"));
    modal->setHeaderData(4, Qt::Horizontal, QObject::tr("Source"));
    modal->setHeaderData(3, Qt::Horizontal, QObject::tr("Event ID"));

    // Configure List Appearance
    ui->treeView->setModel(modal);
    ui->treeView->setColumnHidden(0,true);
}

eventLogViewer::~eventLogViewer()
{
    tcpSocket.close();
    delete ui;
}

void emptyEventLogs()
{
    // Connect to database
    QSqlDatabase db;
    db = QSqlDatabase::database("QSQLITE");

    // Delete Log Entries
    QSqlQuery query;
    query.exec(QString("DELETE FROM eventlogs"));
}

void importEventLog(QString logPath)
{
    // Connect to database
    QSqlDatabase db;
    db = QSqlDatabase::database("QSQLITE");

    // Open Log File
    QFile logFile(logPath);

    // Parse the log file
    if(logFile.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&logFile);
        while(!stream.atEnd())
        {
            QString date, time, hostName, source, eventId, description;
            // Grab a line and get the length of it
            QString line = stream.readLine();
            int len = line.length();

            // Iterate through and parse
            int counter = 0;

            // Parse Date
            for (counter = 0; counter <= 9; counter++)
            {
                date = date + line[counter];
            }

            // Parse Time
            for (counter = 11; counter <= 18; counter++)
            {
                time = time + line[counter];
            }

            // Parse Hostname
            counter = 26;
            while(line[counter] != ' ')
            {
                hostName = hostName + line[counter];
                counter++;
            }

            // Parse Source
            counter++;
            while(line[counter] != ':')
            {
                if(line[counter] == '.')
                {
                    source = "Flags";
                    break;
                }
                source = source + line[counter];
                counter++;
            }
            if(source == "Flags" || source == "The subscription callback received the following Win32 error")
            {
                continue;
            }

            // Parse Event ID
            counter = counter + 2;
            while(line[counter] != ':')
            {
                eventId = eventId + line[counter];
                counter ++;
            }

            // Parse Description
            counter = counter + 2;
            while(counter < len)
            {
                description = description + line[counter];
                counter++;
            }

            // Insert into database
            QSqlQuery query;
            query.exec(QString("INSERT INTO eventlogs VALUES(NULL, '%1',"
                               "'%2','%3','%4','%5','%6')")
                       .arg(date).arg(time).arg(hostName).arg(source).arg(eventId).arg(description));
        }
    }
    logFile.close();
}

void eventLogViewer::on_treeView_doubleClicked(const QModelIndex &index)
{
    QString id;
    QModelIndex idx = ui->treeView->model()->index(index.row(), 0, index.parent());

    if(idx.isValid())
    {
        id = idx.data(Qt::DisplayRole).toString();
    }

    // Open a dialog with details of selected row
    logdetailsdialog = new LogDetailsDialog;
    logdetailsdialog->receiveData(id, 1);
    logdetailsdialog->show();
}

void eventLogViewer::prepNetwork()
{
    // Connect to server
    QHostAddress addr("127.0.0.1");

    tcpSocket.connectToHost(addr, 65001);

    // Send header to server to notify client type.
    QString netHeader = "sawmill_viewer";
    if(!tcpSocket.isOpen())
    {
        QMessageBox::critical(this, "No Server Connected", "Ya ain't connected to the server.");
        return;
    }
    QByteArray Buffer;
    Buffer.append(netHeader);

    tcpSocket.write(Buffer);

}

void eventLogViewer::connectionInit()
{
    // QMessageBox::information(this, "Connected", "You're hooked up, dude!");
}

void eventLogViewer::handleError(QAbstractSocket::SocketError err)
{
    QMessageBox::critical(this, "Error!", tcpSocket.errorString());
    tcpSocket.close();
}

void eventLogViewer::dataArrived()
{
    QByteArray Buffer;
    Buffer = tcpSocket.readAll();
    incomingLog = QString(Buffer);
    QFile file("data/new_logs");
    if(file.open(QIODevice::Append))
    {
        QTextStream out(&file);
        out << incomingLog;
        // QMessageBox::information(this, "Data Received", "Data Received.");
    }
    file.close();
    emptyEventLogs();
    importEventLog("data/new_logs");
    setupTable();
}

void eventLogViewer::closeEvent(QCloseEvent *event)
{
    tcpSocket.close();
    event->accept();
}

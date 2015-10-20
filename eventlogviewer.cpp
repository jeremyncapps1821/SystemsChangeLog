#include "eventlogviewer.h"
#include "ui_eventlogviewer.h"

eventLogViewer::eventLogViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::eventLogViewer)
{
    ui->setupUi(this);

    QString logFile = "data/new_logs";

    doConnect();

    // Setup database table and import log file
    // emptyEventLogs();
    importEventLog(logFile,1);
    setupTable();

}

void eventLogViewer::doConnect()
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    ui->textBrowser->append("connecting...");

    // this is not blocking call
    socket->connectToHost("localhost", 65001);

    // we need to wait...
    if(!socket->waitForConnected(5000))
    {
        QString errorText = "Error: " + socket->errorString();
        ui->textBrowser->append(errorText);
    }
}

void eventLogViewer::connected()
{
     ui->textBrowser->append("connected...");

    // Hey server.
    socket->write("sawmill_viewer");
}

void eventLogViewer::disconnected()
{
    ui->textBrowser->append("disconnected...");
}

void eventLogViewer::bytesWritten(qint64 bytes)
{
    ui->textBrowser->append("some number of bytes written...");
}

void eventLogViewer::readyRead()
{
    ui->textBrowser->append("reading...");

    // read the data from the socket
    QByteArray data = socket->readAll();
    ui->textBrowser->append("data received...");
    bool result = handleIncoming(data);
}

bool eventLogViewer::handleIncoming(QByteArray data)
{
    if(data == "Data Received")
    {
        ui->textBrowser->append(data);
        return true;
    }
    else if(data == "Failed to receive data")
    {
        ui->textBrowser->append(data);
        return false;
    }
    else
    {
        saveLog(data);
        importEventLog("data/new_logs", 1);
        setupTable();
        return true;
    }
}

void eventLogViewer::saveLog(QByteArray data)
{
    // Open and write out the new log entries
    QFile file("data/new_logs");
    QTextStream out(&file);
    if(!file.open(QIODevice::Append))
    {
        ui->textBrowser->append("Unable to save log file");
    }
    out << data;
    file.close();
    ui->textBrowser->append("Logs received...");
}

void eventLogViewer::setupTable()
{
    // Connect to database
    QSqlDatabase db;
    db = QSqlDatabase::database("QSQLITE");

    // Query database
    QSqlQueryModel * modal = new QSqlQueryModel();
    QSqlQuery * query = new QSqlQuery(db);
    query->prepare("SELECT id,eventId,dateTime,machineName FROM eventlogs");
    query->exec();

    // Configure model
    modal->setQuery(*query);
    modal->setHeaderData(1, Qt::Horizontal, QObject::tr("Event ID"));
    modal->setHeaderData(2, Qt::Horizontal, QObject::tr("Date/Time"));
    modal->setHeaderData(3, Qt::Horizontal, QObject::tr("Source"));

    // Configure List Appearance
    ui->treeView->setModel(modal);
    ui->treeView->setColumnHidden(0,true);
}

eventLogViewer::~eventLogViewer()
{
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

void eventLogViewer::importEventLog(QString logPath, int logType)
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
            if(logType == 0)
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
            else if(logType == 1)
            {
                // event = new winEvent();
                bool eventLoaded = false;
                bool extraLine = false;
                while(!stream.atEnd())
                {
                    QString primaryKey, message, eventId, version, qualifiers;
                    QString level, task, opcode, keywords, recordId, providerName;
                    QString providerId, logName, processId, threadId, machineName;
                    QString userId, dateTime, activityId, relatedActivityId;
                    QString containerLog, matchedQueryIds, bookmark, levelDisplayName;
                    QString opcodeDisplayName, taskDisplayName, keywordsDisplayNames, properties;
                    QString line = stream.readLine();
                    int counter;
                    int len = line.length();

                    // Skip empty lines at the start
                    if(line == "")
                    {
                        continue;
                    }

                    // Parse the message lines
                    for(counter = 23; counter < len; counter++)
                    {
                        message = message + line[counter];
                    }
                    line = stream.readLine();
                    len = line.length();
                    if(line[0] == ' ')
                    {
                        extraLine = true;
                    }
                    while(extraLine)
                    {
                        for(counter = 23; counter < len; counter++)
                        {
                            message = message + line[counter];
                        }
                        line = stream.readLine();
                        len = line.length();
                        if(line[0] == ' ')
                        {
                            extraLine = true;
                        }
                        else
                        {
                            extraLine = false;
                        }
                    }
                    // Parse the event ID
                    for(counter = 23; counter < len; counter++)
                    {
                        eventId = eventId + line[counter];
                    }

                    // Parse version
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        version = version + line[counter];
                    }

                    // Parse qualifiers
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        qualifiers = qualifiers + line[counter];
                    }

                    // Parse level
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        level = level + line[counter];
                    }

                    // Parse task
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        task = task + line[counter];
                    }

                    // Parse opcode
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        opcode = opcode + line[counter];
                    }

                    // Parse keywords
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        keywords = keywords + line[counter];
                    }

                    // Parse recordId
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        recordId = recordId + line[counter];
                    }

                    // Parse providerName
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        providerName = providerName + line[counter];
                    }

                    // Parse providerId
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        providerId = providerId + line[counter];
                    }

                    // Parse logName
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        logName = logName + line[counter];
                    }

                    // Parse processId
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        processId = processId + line[counter];
                    }

                    // Parse threadId
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        threadId = threadId + line[counter];
                    }

                    // Parse machineName
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        machineName = machineName + line[counter];
                    }

                    // Parse userId
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        userId = userId + line[counter];
                    }

                    // Parse dateTime
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        dateTime = dateTime + line[counter];
                    }

                    // Parse activityId
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        activityId = activityId + line[counter];
                    }

                    // Parse relatedActivityId
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        relatedActivityId = relatedActivityId + line[counter];
                    }

                    // Parse containerLog
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        containerLog = containerLog + line[counter];
                    }

                    // Parse matchedQueryIds
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        matchedQueryIds = matchedQueryIds + line[counter];
                    }

                    // Parse bookmark
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        bookmark = bookmark + line[counter];
                    }

                    // Parse levelDisplayName
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        levelDisplayName = levelDisplayName + line[counter];
                    }

                    // Parse opcodeDisplayName
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        opcodeDisplayName = opcodeDisplayName + line[counter];
                    }

                    // Parse taskDisplayName
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        taskDisplayName = taskDisplayName + line[counter];
                    }

                    // Parse keywordsDisplayNames
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        keywordsDisplayNames = keywordsDisplayNames + line[counter];
                    }

                    // Parse properties
                    line = stream.readLine();
                    len = line.length();
                    for(counter = 23; counter < len; counter++)
                    {
                        properties = properties + line[counter];
                    }
                    line = stream.readLine();
                    len = line.length();
                    if(line[0] == ' ')
                    {
                        extraLine = true;
                    }
                    while(extraLine)
                    {
                        for(counter = 23; counter < len; counter++)
                        {
                            properties = properties + line[counter];
                        }
                        line = stream.readLine();
                        len = line.length();
                        if(line[0] == ' ')
                        {
                            extraLine = true;
                        }
                        else
                        {
                            extraLine = false;
                        }
                    }

                    // Insert into database
                    QSqlQuery query;
                    query.exec(QString("INSERT INTO eventlogs VALUES(NULL, '%1',"
                                       "'%2','%3','%4','%5','%6','%7','%8',"
                                       "'%9','%10','%11','%12','%13','%14','%15',"
                                       "'%16','%17','%18','%19','%20','%21','%22',"
                                       "'%23','%24','%25','%26','%27')")
                               .arg(message).arg(eventId).arg(version).arg(qualifiers).arg(level)
                               .arg(task).arg(opcode).arg(keywords).arg(recordId).arg(providerName)
                               .arg(providerId).arg(logName).arg(processId).arg(threadId).arg(machineName)
                               .arg(userId).arg(dateTime).arg(activityId).arg(relatedActivityId).arg(containerLog)
                               .arg(matchedQueryIds).arg(bookmark).arg(levelDisplayName).arg(opcodeDisplayName)
                               .arg(taskDisplayName).arg(keywordsDisplayNames).arg(properties));

                    eventLoaded = true;
                }
            }
        }
    logFile.close();
    QFile::rename("data/new_logs", "data/old_logs");
    }
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

void eventLogViewer::closeEvent(QCloseEvent *event)
{
    socket->close();
    event->accept();
}

void eventLogViewer::on_clrLogsButton_clicked()
{
    emptyEventLogs();
    setupTable();
}

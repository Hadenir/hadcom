#include "main_window.h"

#include <QProgressBar>
#include <QProgressDialog>
#include <QMessageBox>

#include "connection.h"
#include "local_client.h"
#include "server.h"
#include "version.h"

MainWindow::MainWindow(UserInfo userInfo, QWidget* parent)
        : QMainWindow(parent), m_userInfo(std::move(userInfo))
{
    setupUi(this);

    m_progressDialog = new QProgressDialog("Connecting to the server...", "Cancel", 0, 0, this,
                                           Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    QProgressBar* progressBar = new QProgressBar(m_progressDialog);
    progressBar->setTextVisible(false);
    progressBar->setRange(0, 0);
    progressBar->setValue(0);
    m_progressDialog->setBar(progressBar);
    m_progressDialog->setMinimumDuration(0);
    connect(m_progressDialog, SIGNAL(canceled()), this, SLOT(close()));
}

bool MainWindow::setup()
{
    qDebug() << "You are using HadCom v." << VERSION_STRING;

    if(m_userInfo.mode == Mode::SERVER)
    {
        qDebug() << "Starting the server...";

        m_server = new Server(this);
        // connect(m_server, SIGNAL(messageReceived(QString)), this, SLOT(on_messageReceived(QString)));
        // connect(m_server, SIGNAL(clientJoined(LocalClient*)), this, SLOT(on_clientJoined(LocalClient*)));
        // connect(m_server, SIGNAL(clientLeft(LocalClient*)), this, SLOT(on_clientLeft(LocalClient*)));
        if(!m_server->bind(m_userInfo.port))
        {
            QMessageBox::critical(this, "Server error occured", "Error: Couldn't bind to the port!");
            return false;
        }

        qDebug() << "Server started!";
    }

    qDebug() << "Connecting to the server...";

    Connection* connection = new Connection(this);
    connect(connection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_error()));

    qDebug() << "Created connection!";

    m_localClient = new LocalClient(connection, this);
    connect(m_localClient, SIGNAL(connected()), this, SLOT(on_connected()));
    connect(m_localClient, SIGNAL(loggedIn(const QList<QString>&)), this, SLOT(on_loggedIn(const QList<QString>&)));
    connect(m_localClient, SIGNAL(clientJoined(QString)), this, SLOT(on_clientJoined(QString)));
    connect(m_localClient, SIGNAL(clientLeft(QString)), this, SLOT(on_clientLeft(QString)));
    connect(m_localClient, SIGNAL(messageReceived(QString, QString)), this, SLOT(on_messageReceived(QString, QString)));
    m_localClient->setNickname(m_userInfo.nickname);

    qDebug() << "Created local client!";

    connection->connectToHost(m_userInfo.address, m_userInfo.port);

    m_progressDialog->open();

    return true;
}

void MainWindow::on_connected()
{
    accountLabel->setText("Connected to host");
    m_progressDialog->setLabelText("Logging in...");
}

void MainWindow::on_loggedIn(const QList<QString>& nicknames)
{
    accountLabel->setText("Logged");

    for(const auto& nickname : nicknames)
    {
        if(nickname != m_localClient->getNickname())
            clientsList->addItem(nickname);
    }

    m_progressDialog->reset();
}

void MainWindow::on_sendButton_clicked()
{
    QString message = lineInput->text();
    lineInput->clear();

    if(message.isEmpty())
        return;

    m_localClient->sendMessage(message);
}

void MainWindow::on_clientJoined(QString nickname)
{
    qDebug() << "Joined:" << nickname;

    chat->append("-> <i>" + nickname + " joined!</i>");

    clientsList->addItem(nickname);
}

void MainWindow::on_clientLeft(QString nickname)
{
    qDebug() << "Left:" << nickname;

    chat->append("-> <i>" + nickname + " left!</i>");

    QListWidgetItem* item = clientsList->findItems(nickname, Qt::MatchExactly)[0];
    delete item;
}

void MainWindow::on_messageReceived(QString sender, QString message)
{
    qDebug() << "Sender:" << sender;
    qDebug() << "Message:" << message;

    chat->append("&lt;" + sender.toHtmlEscaped() + "&gt; " + message.toHtmlEscaped() + "\n");
}

void MainWindow::on_error()
{
    qDebug() << "Server error!";
    QTcpSocket* socket = (QTcpSocket*) sender();
    QMessageBox::critical(this, "An error occured", "Error: " + socket->errorString() + "!");
    this->close();
}

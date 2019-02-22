#include "application.h"

#include <QMessageBox>

#include "connection.h"
#include "mode_chooser.h"

Application::Application(int argc, char** argv)
{
    auto modeChooser = new ModeChooser;
    connect(modeChooser, SIGNAL(accepted()), this, SLOT(on_accepted()));
    connect(modeChooser, SIGNAL(rejected()), this, SLOT(on_rejected()));
    modeChooser->open();
}

void Application::on_accepted()
{
    auto modeChooser = (ModeChooser*) sender();
    UserInfo m_userInfo = modeChooser->getUserInfo();
    modeChooser->close();

    auto mainWindow = new MainWindow();
    connect(mainWindow, SIGNAL(messageSent(QString)), this, SLOT(on_messageSent(QString)));
    mainWindow->show();

    if(m_userInfo.mode == Mode::SERVER)
    {
        qInfo() << "Starting the server...";

        m_server = new Server(this);
        if(!m_server->bind(m_userInfo.port))
        {
            QMessageBox::critical(modeChooser, "Server error occured", "Error: Couldn't bind to the port!");
            QApplication::quit();
        }

        qInfo() << "Server started!";
    }

    qInfo() << "Connecting to the server...";

    auto connection = new Connection(this);
    connect(connection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_error()));

    qInfo() << "Created connection!";

    m_localClient = new LocalClient(connection, this);
    connect(m_localClient, SIGNAL(connected()), mainWindow, SLOT(on_connected()));
    connect(m_localClient, SIGNAL(loggedIn(const QList<QString>&)), mainWindow, SLOT(on_loggedIn(const QList<QString>&)));
    connect(m_localClient, SIGNAL(clientJoined(QString)), mainWindow, SLOT(on_clientJoined(QString)));
    connect(m_localClient, SIGNAL(clientLeft(QString)), mainWindow, SLOT(on_clientLeft(QString)));
    connect(m_localClient, SIGNAL(messageReceived(QString, QString)), mainWindow, SLOT(on_messageReceived(QString, QString)));
    m_localClient->setNickname(m_userInfo.nickname);

    connection->connectToHost(m_userInfo.address, m_userInfo.port);
}

void Application::on_rejected()
{
    auto modeChooser = (ModeChooser*) sender();
    modeChooser->close();
    QApplication::quit();
}

void Application::on_messageSent(QString message)
{
    m_localClient->sendMessage(message);
}

void Application::on_error()
{
    auto socket = (QTcpSocket*) sender();
    QMessageBox::critical(m_mainWindow, "An error occured", "Error: " + socket->errorString() + "!");
    qInfo() << "Error:" << socket->errorString();
    QApplication::quit();
}
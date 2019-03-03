#include "application.h"

#include <QMessageBox>
#include <QSettings>

#include "connection.h"

Application::Application(int argc, char** argv)
{
    // for(int i = 0; i < argc; ++i)
    // {
    //     if(strcmp(argv[i], "--server") == 0)
    //     {
    //         UserInfo info;
    //         info.nickname = "SERVER";
    //         info.port = 2220;
    //         setup(info);
    //     }
    // }

    loadConfig();

    m_mainWindow = new MainWindow();
    connect(m_mainWindow, SIGNAL(messageSent(QString)), this, SLOT(on_messageSent(QString)));

    m_modeChooser = new ModeChooser;
    connect(m_modeChooser, SIGNAL(continued()), this, SLOT(on_continued()));
    connect(m_modeChooser, SIGNAL(rejected()), this, SLOT(on_rejected()));

    m_modeChooser->setUserInfo(m_userInfo);
    m_modeChooser->open();
}

Application::~Application()
{
    saveConfig();
}

void Application::on_continued()
{
    m_userInfo = m_modeChooser->getUserInfo();

    // m_modeChooser->close();
    // m_mainWindow->show();

    setup();

    m_modeChooser->accept();
    m_mainWindow->show();
}

void Application::on_rejected()
{
    QApplication::quit();
}

void Application::on_connected()
{

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

    m_modeChooser->reset();
    delete m_server;
    delete m_localClient;
}

void Application::setup()
{
    if(m_userInfo.mode == Mode::SERVER)
    {
        qInfo() << "Starting the server...";

        m_server = new Server(this);
        if(!m_server->bind(m_userInfo.port))
        {
            QMessageBox::critical(m_modeChooser, "Server error occured", "Error: Couldn't bind to the port!");
            QApplication::quit();
        }

        qInfo() << "Server started!";
    }

    qInfo() << "Connecting to the server...";

    auto connection = new Connection(this);
    connect(connection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_error()));

    qInfo() << "Created connection!";

    m_localClient = new LocalClient(connection, this);
    connect(m_localClient, SIGNAL(connected()), this, SLOT(on_connected()));
    connect(m_localClient, SIGNAL(loggedIn(const QList<QString>&)), m_mainWindow,
            SLOT(on_loggedIn(const QList<QString>&)));
    connect(m_localClient, SIGNAL(clientJoined(QString)), m_mainWindow, SLOT(on_clientJoined(QString)));
    connect(m_localClient, SIGNAL(clientLeft(QString)), m_mainWindow, SLOT(on_clientLeft(QString)));
    connect(m_localClient, SIGNAL(messageReceived(QString, QString)), m_mainWindow,
            SLOT(on_messageReceived(QString, QString)));
    m_localClient->setNickname(m_userInfo.nickname);

    connection->connectToHost(m_userInfo.address, m_userInfo.port);
    if(!connection->waitForConnected(TIMEOUT_TIME))
        return;
}

void Application::loadConfig()
{
    QSettings config(CONFIG_NAME, QSettings::IniFormat, this);
    QString nickname = config.value("Nickname", "Pl4ceHold3r").toString();
    m_userInfo.nickname = nickname;
    QString address = config.value("IpAddress", "localhost").toString();
    m_userInfo.address = address;
    quint16 port = (quint16) config.value("Port", 1205).toInt();
    m_userInfo.port = port;
    bool serverMode = config.value("ServerMode", false).toBool();
    m_userInfo.mode = serverMode ? Mode::SERVER : Mode::CLIENT;
}

void Application::saveConfig()
{
    QSettings config(CONFIG_NAME, QSettings::IniFormat, this);
    config.setValue("Nickname", m_userInfo.nickname);
    config.setValue("IpAddress", m_userInfo.address);
    config.setValue("Port", m_userInfo.port);
    config.setValue("ServerMode", m_userInfo.mode == Mode::SERVER);
}

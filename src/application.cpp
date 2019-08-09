#include "application.h"

#include <QMessageBox>
#include <QSettings>

#include "connection.h"

Application::Application(int argc, char** argv)
{
    loadConfig();

    for(int i = 0; i < argc; ++i)
    {
        if(strcmp(argv[i], "--server") == 0)
        {
            m_userInfo.server = true;
            m_mode = Mode::SERVER;

            setup();
            return;
        }
    }

    m_mode = m_userInfo.server ? Mode::HYBRID : Mode::CLIENT;

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
    m_mode = m_userInfo.server ? Mode::HYBRID : Mode::CLIENT;

    // m_modeChooser->close();
    // m_mainWindow->show();

    if(!setup())
        QApplication::quit();

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
    if(m_mode == Mode::SERVER || m_mode == Mode::HYBRID)
        delete m_server;
    if(m_mode == Mode::CLIENT || m_mode == Mode::HYBRID)
        delete m_localClient;

    QApplication::quit();
}

bool Application::setup()
{
    if(m_mode == Mode::SERVER || m_mode == Mode::HYBRID)
    {
        qInfo() << "Starting the server...";

        m_server = new Server(this);
        if(!m_server->bind(m_userInfo.port))
        {
            QMessageBox::critical(m_modeChooser, "Server error occured", "Error: Couldn't bind to the port!");
            return false;
        }

        qInfo() << "Server started!";
    }

    if(m_mode == Mode::CLIENT || m_mode == Mode::HYBRID)
    {
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
            return false;
    }

    return true;
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
    bool server = config.value("Server", false).toBool();
    m_userInfo.server = server;
}

void Application::saveConfig()
{
    QSettings config(CONFIG_NAME, QSettings::IniFormat, this);
    config.setValue("Nickname", m_userInfo.nickname);
    config.setValue("IpAddress", m_userInfo.address);
    config.setValue("Port", m_userInfo.port);
    config.setValue("ServerMode", m_userInfo.server);
}

#pragma once

#include <QObject>

#include "local_client.h"
#include "main_window.h"
#include "mode_chooser.h"
#include "server.h"
#include "user_info.h"

class Application : public QObject
{
Q_OBJECT

public:
    Application(int argc, char* argv[]);

    ~Application();

private slots:
    void on_continued();

    void on_rejected();

    void on_connected();

    void on_messageSent(QString message);

    void on_error();

private:
    const QString CONFIG_NAME = "config.ini";

    const int TIMEOUT_TIME = 10000;

    LocalClient* m_localClient;

    Server* m_server;

    ModeChooser* m_modeChooser;

    MainWindow* m_mainWindow;

    UserInfo m_userInfo;

    void setup();

    void loadConfig();

    void saveConfig();
};


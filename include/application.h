#pragma once

#include <QObject>

#include "local_client.h"
#include "main_window.h"
#include "server.h"

class Application : public QObject
{
Q_OBJECT

public:
    Application(int argc, char* argv[]);

private slots:
    void on_accepted();

    void on_rejected();

    void on_messageSent(QString message);

    void on_error();

private:
    LocalClient* m_localClient;

    Server* m_server;

    MainWindow* m_mainWindow;
};


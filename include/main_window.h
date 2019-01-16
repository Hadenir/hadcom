#pragma once

#include "user_info.h"
#include "ui_main_window.h"

class QProgressDialog;
class Server;
class LocalClient;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
Q_OBJECT

public:
    explicit MainWindow(UserInfo userInfo, QWidget* parent = nullptr);

    bool setup();

private slots:
    void on_connected();

    void on_loggedIn(const QList<QString>& nicknames);

    void on_sendButton_clicked();

    void on_clientJoined(QString nickname);

    void on_clientLeft(QString nickname);

    void on_messageReceived(QString sender, QString message);

    void on_error();

private:
    UserInfo m_userInfo;

    LocalClient* m_localClient;

    Server* m_server;

    QProgressDialog* m_progressDialog;
};
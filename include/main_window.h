#pragma once

#include "ui_main_window.h"

#include <QProgressDialog>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

signals:
    void messageSent(QString message);

private slots:
    void on_connected();

    void on_loggedIn(const QList<QString>& nicknames);

    void on_sendButton_clicked();

    void on_clientJoined(QString nickname);

    void on_clientLeft(QString nickname);

    void on_messageReceived(QString sender, QString message);

private:
    QProgressDialog* m_progressDialog;
};
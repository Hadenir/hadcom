#include "main_window.h"

#include <QProgressBar>

#include "local_client.h"

MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
{
    setupUi(this);

    m_progressDialog = new QProgressDialog("Connecting to the server...", "Cancel", 0, 0, this,
                                           Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    auto progressBar = new QProgressBar(m_progressDialog);
    progressBar->setTextVisible(false);
    progressBar->setRange(0, 0);
    progressBar->setValue(0);
    m_progressDialog->setBar(progressBar);
    m_progressDialog->setMinimumDuration(0);
    connect(m_progressDialog, SIGNAL(canceled()), this, SLOT(close()));

    m_progressDialog->open();
}

void MainWindow::on_connected()
{
    accountLabel->setText("Connected to host");
    m_progressDialog->setLabelText("Logging in...");
}

void MainWindow::on_loggedIn(const QList<QString>& nicknames)
{
    auto localClient = (LocalClient*) sender();

    accountLabel->setText("Logged as " + localClient->getNickname());

    for(const auto& nickname : nicknames)
    {
        if(nickname != localClient->getNickname())
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

    emit messageSent(message);
}

void MainWindow::on_clientJoined(QString nickname)
{
    chat->append("-> <i>" + nickname + " joined!</i>");

    clientsList->addItem(nickname);
}

void MainWindow::on_clientLeft(QString nickname)
{
    chat->append("-> <i>" + nickname + " left!</i>");

    QListWidgetItem* item = clientsList->findItems(nickname, Qt::MatchExactly)[0];
    delete item;
}

void MainWindow::on_messageReceived(QString sender, QString message)
{
    QApplication::alert(this);

    chat->append("&lt;" + sender.toHtmlEscaped() + "&gt; " + message.toHtmlEscaped() + "\n");
}

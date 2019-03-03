#include "mode_chooser.h"

#include <QMessageBox>
#include <QIntValidator>

ModeChooser::ModeChooser(QWidget* parent)
        : QDialog(parent)
{
    setupUi(this);

    portInput->setValidator(new QIntValidator(0, 65535, this));
}

void ModeChooser::setUserInfo(UserInfo userInfo)
{
    m_userInfo = userInfo;

    nicknameInput->setText(userInfo.nickname);
    addressInput->setText(userInfo.address);
    portInput->setText(QString::number(userInfo.port));
    serverCheckbox->setChecked(userInfo.mode == Mode::SERVER);
}

UserInfo ModeChooser::getUserInfo() const
{
    return m_userInfo;
}

void ModeChooser::reset()
{
    continueButton->setDisabled(false);
}

void ModeChooser::on_continueButton_clicked()
{
    if(nicknameInput->text().isEmpty())
    {
        QMessageBox::warning(this, "Input error", "Nickname cannot be blank!");
        return;
    }
    m_userInfo.nickname = nicknameInput->text();

    if(addressInput->isEnabled())
    {
        if(addressInput->text().isEmpty())
        {
            QMessageBox::warning(this, "Input error", "Address cannot be blank!");
            return;
        }

        m_userInfo.address = addressInput->text();
    }

    if(portInput->text().isEmpty())
    {
        QMessageBox::warning(this, "Input error", "Port cannot be blank!");
        return;
    }
    m_userInfo.port = portInput->text().toUShort();

    m_userInfo.mode = serverCheckbox->checkState() == Qt::Checked ? Mode::SERVER : Mode::CLIENT;

    continueButton->setDisabled(true);

    emit continued();
}

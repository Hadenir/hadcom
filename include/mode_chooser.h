#pragma once

#include "user_info.h"
#include "ui_mode_chooser.h"

class ModeChooser : public QDialog, private Ui::ModeChooser
{
Q_OBJECT

public:
    explicit ModeChooser(QWidget* parent = nullptr);

    void setUserInfo(UserInfo userInfo);

    UserInfo getUserInfo() const;

    void reset();

signals:
    void continued();

private slots:
    void on_continueButton_clicked();

private:
    UserInfo m_userInfo;
};
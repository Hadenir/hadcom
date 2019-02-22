#pragma once

#include "user_info.h"
#include "ui_mode_chooser.h"

class ModeChooser : public QDialog, private Ui::ModeChooser
{
Q_OBJECT

public:
    explicit ModeChooser(QWidget* parent = nullptr);

    UserInfo getUserInfo() const;

private slots:
    void on_continueButton_clicked();

private:
    UserInfo m_userInfo;
};
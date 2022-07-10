#pragma once

#include "PopWidgetBase.h"
#include "FtpControl.h"
namespace Ui { class FtpClient; }
class FtpClient : public PopWidgetBase
{
    Q_OBJECT

public:
    FtpClient(QWidget *parent = Q_NULLPTR);

private:
    Ui::FtpClient* ui;
	FtpControl * m_ftpControl;
};

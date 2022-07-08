#include "FtpClient.h"
#include "ui_FtpClient.h"
FtpClient::FtpClient(QWidget *parent)
	: PopWidgetBase(parent), ui(new Ui::FtpClient)
{
    ui->setupUi(this);
	m_ftpControl = new FtpControl(this);
}

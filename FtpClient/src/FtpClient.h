#pragma once

#include "PopWidgetBase.h"
#include "FtpControl.h"
#include <QDropEvent>
#include <QDragEnterEvent>
namespace Ui { class FtpClient; }
class FtpClient : public PopWidgetBase
{
    Q_OBJECT

public:
    FtpClient(QWidget *parent = Q_NULLPTR);
	~FtpClient();
protected:
	virtual void dragEnterEvent(QDragEnterEvent *event) override;
	virtual  void dropEvent(QDropEvent *event) override;
	virtual bool eventFilter(QObject * obj, QEvent *event) override;
private:
    Ui::FtpClient* ui;
	FtpControl * m_ftpControl = nullptr;
	bool m_currentTool = false;
	FtpConfig m_currentConfig;
};

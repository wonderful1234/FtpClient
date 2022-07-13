#include "FtpClient.h"
#include "ui_FtpClient.h"
#include <QMimeData>
#include <QFileDialog>
FtpClient::FtpClient(QWidget *parent)
	: PopWidgetBase(parent), ui(new Ui::FtpClient)
{
    ui->setupUi(this);
	ui->toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	setAcceptDrops(true);
	ui->toolButton->setAcceptDrops(true);
	installEventFilter(this);
	ui->toolButton->installEventFilter(this);
	m_ftpControl = new FtpControl(this);
	connect(ui->btnMine, &QPushButton::clicked, this, [&]() {
		showMinimized();
	});

	connect(ui->btnClose, &QPushButton::clicked, this, [&]() {
		close();
	});

	connect(ui->toolButton, &QPushButton::clicked, this, [&]() {
		
		auto filePath=QFileDialog::getOpenFileName(this, u8"请选择要上传的文件");
		if (filePath.isEmpty())
			return;
		ui->stackedWidget->setCurrentIndex(1);
		ui->progressBar->setValue(0);

	});

	connect(m_ftpControl, &FtpControl::signProgress, this, [=](double progress) {

		ui->progressBar->setValue(progress);

	}, Qt::BlockingQueuedConnection);

	connect(m_ftpControl, &FtpControl::signFinished, this, [=](bool success) {

		ui->stackedWidget->setCurrentIndex(0);

	});
}

void FtpClient::dragEnterEvent(QDragEnterEvent * event)
{
	if (event->mimeData()->hasUrls() && event->mimeData()->urls().count() == 1)
	{
		auto filePath = event->mimeData()->urls().last().toLocalFile();
		QFileInfo info(filePath);
		if(!info.isDir())
			event->acceptProposedAction();
	}
}

void FtpClient::dropEvent(QDropEvent * event)
{
	if (m_currentTool)
	{
		auto filePath = event->mimeData()->urls().last().toLocalFile();
		m_currentTool = false;
	}
	
}

bool FtpClient::eventFilter(QObject * obj, QEvent * event)
{
	if (ui->toolButton == obj)
	{
		if (event->type() == QEvent::DragEnter)
		{
			m_currentTool = true;
			return true;
		}
			
	}
	else 
	{
		if (event->type() == QEvent::Enter)
		{
			m_currentTool = false;
			return true;
		}
	}
	return false;
}

#include "FtpControl.h"
FtpControl::FtpControl(QObject *parent)
	: QObject(parent)
{
	m_threadPool = new QThreadPool(this);
	m_threadPool->setMaxThreadCount(MaxThreadCount);
}
FtpControl::~FtpControl()
{

}

void FtpControl::addTask(const FtpConfig & config, const QString & localPath)
{
	
		auto task = new FtpTask(this, config,localPath);
		m_taskLists.append(task);
		m_threadPool->start(task);
	
	
}

void FtpControl::taskFinish(FtpTask * task, bool success)
{
	if (task)
	{
		m_taskLists.removeAll(task);
		delete task;
	}
	emit signFinished(success);
}

void FtpControl::setProgrsee(qint64 bytesDone, qint64 bytesTota)
{
	auto progress = bytesDone * 1.0 / bytesTota * 100;
	emit signProgress(progress);
}

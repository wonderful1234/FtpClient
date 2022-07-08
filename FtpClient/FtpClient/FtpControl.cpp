#include "FtpControl.h"
FtpControl::FtpControl(QObject *parent)
	: QObject(parent)
{
	m_threadPool = new QThreadPool(this);
}
FtpControl::~FtpControl()
{

}

void FtpControl::addTask(const FtpConfig & config, const QString & localPath)
{
	
		auto task = new FtpTask(this, config,localPath);
		m_threadPool->start(task);
	
	
}

void FtpControl::taskFinish(FtpTask * task, bool success)
{
	if (task)
	{
		delete task;
	}
}

void FtpControl::setProgrsee(qint64 bytesDone, qint64 bytesTota)
{

}

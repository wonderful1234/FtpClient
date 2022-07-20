#pragma once
#include <QObject>
#include <QThreadPool>
#include "FtpTask.h"
#define  MaxThreadCount  3
class FtpControl : public QObject
{
	Q_OBJECT
public:
	FtpControl(QObject *parent = Q_NULLPTR);
	~FtpControl();
	void addTask(const FtpConfig& config,const QString & localPath);
	void taskFinish(FtpTask* task, bool success);
	void setProgrsee(qint64 bytesDone, qint64 bytesTota);
signals:
	void signFinished(bool success);
	void signProgress(double progress);
private:
	QThreadPool * m_threadPool;
	QList<FtpTask*>m_taskLists;
};
#pragma once
#include "DataModel.h"
#include <QRunnable>
class FtpControl;
class FtpTask : public QRunnable
{
public:
	FtpTask(FtpControl * control, const FtpConfig& config,const QString & localPath);
protected:
	virtual void run()override;
	inline QString ParseURL(const QString &filePath);
	static int progressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);
	static size_t ReadFromFileCallback(void *ptr, size_t size, size_t nmemb, void *stream);
private:
	FtpConfig m_ftpConfig;
	FtpControl * m_control;
	QString m_localFilePath;
};
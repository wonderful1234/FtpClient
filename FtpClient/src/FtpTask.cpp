#include "FtpTask.h"
#include "FtpControl.h"
#include "curl/curl.h"
#include <QFile>
#include <QFileInfo>
#include <fstream>
FtpTask::FtpTask(FtpControl * control, const FtpConfig& config,  const QString & localPath)
	: m_control(control),m_ftpConfig(config),m_localFilePath(localPath)
{
	setAutoDelete(false);
}


void FtpTask::run()
{
	bool success = false;
	if (!m_ftpConfig.ip.isEmpty() || m_localFilePath.isEmpty())
	{
		if (QFile::exists(m_localFilePath))
		{
			CURL* curl = curl_easy_init();
			if (curl)
			{
				std::ifstream fileStream;
				fileStream.open(m_localFilePath.toStdWString(), std::ifstream::in | std::ifstream::binary);
				QFileInfo info(m_localFilePath);
				QString url=ParseURL(info.fileName());
				curl_easy_setopt(curl, CURLOPT_URL, url.toLocal8Bit().data());
				curl_easy_setopt(curl, CURLOPT_PORT, m_ftpConfig.port.toUInt());
				curl_easy_setopt(curl, CURLOPT_USERPWD, QString("%1:%2").arg(m_ftpConfig.userName).arg(m_ftpConfig.password).toLocal8Bit().data());
				curl_easy_setopt(curl, CURLOPT_READFUNCTION, FtpTask::ReadFromFileCallback);
				curl_easy_setopt(curl, CURLOPT_READDATA, &fileStream);
				curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
				curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, static_cast<curl_off_t>(info.size()));
				curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, FtpTask::progressCallback);
				curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, this);
				curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
				curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
				CURLcode res = curl_easy_perform(curl);
				success = CURLE_OK == res;
				fileStream.close();
			}
			curl_easy_cleanup(curl);
		}
	}
	m_control->taskFinish(this, success);

}

inline QString FtpTask::ParseURL(const QString &filePath)
{
	QString url;
	switch (m_ftpConfig.ftpType)
	{
	case FtpType::FTP:
			url = "ftp://";
			break;
	case FtpType::FTPS:
		url = "ftps://";
		break;
	case FtpType::SFTP:
		url = "sftp://";
	default:
		break;
	}
	url=url + m_ftpConfig.ip + "/" + m_ftpConfig.path + "/"+filePath;
	return url;
}

int FtpTask::progressCallback(void * clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
	auto task = (FtpTask*)clientp;
	task->m_control->setProgrsee(ulnow, ultotal);
	return 0;
}

size_t FtpTask::ReadFromFileCallback(void * ptr, size_t size, size_t nmemb, void * stream)
{

	std::ifstream *pFileStream = reinterpret_cast<std::ifstream *>(stream);
	if (pFileStream->is_open()) {
		pFileStream->read(reinterpret_cast<char *>(ptr), size * nmemb);
		return pFileStream->gcount();
	}
	return 0;
}

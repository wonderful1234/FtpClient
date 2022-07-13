#include "Log.h"
#include <QCoreApplication>

Log* Log::instance = nullptr;

Log::Log()
{
	m_logPath = QCoreApplication::applicationDirPath() + "/FtpLog";
	m_logger = spdlog::basic_logger_mt("basic_logger", m_logPath.toLocal8Bit().data());
	m_logger->set_pattern("%Y-%m-%d %H:%M:%S.%e %l  %v......");
}

Log::~Log()
{

}

Log * Log::getInstance()
{
	if (!instance)
		instance = new Log();
	return instance;
}
void Log::logInfo(const QString & info)
{
	std::string log = info.toLocal8Bit().data();
	m_logger->info(log);
	m_logger->flush();
}

void Log::logError(const QString & error)
{
	std::string log = error.toLocal8Bit().data();
	m_logger->error(log);
	m_logger->flush();
}

void Log::logWarn(const QString & warn)
{
	std::string log = warn.toLocal8Bit().data();
	m_logger->warn(log);
	m_logger->flush();
}

void Log::closeLog()
{
	spdlog::drop_all();
}


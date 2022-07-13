#pragma once
#include <QString>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
class Log
{
public:
	static Log* getInstance();
	void logInfo(const QString& info);
	void logError(const QString& error);
	void logWarn(const QString& warn);
	void closeLog();
	~Log();
private:
	Log();
	static Log* instance;
	std::shared_ptr<spdlog::logger> m_logger;
	QString m_logPath;
};

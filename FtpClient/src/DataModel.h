#pragma once
#include <QString>
enum class FtpType
{
	FTP, SFTP, FTPS
};
struct FtpConfig
{
	FtpType ftpType;
	QString ip;
	QString port;
	QString userName;
	QString password;
	QString path;
	bool operator==(const FtpConfig&config)
	{
		return ip == config.ip && (port == config.port) && (password == config.password) && (userName==config.userName) && (ftpType==config.ftpType);
	}
};
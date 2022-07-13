#pragma once

#include "PopWidgetBase.h"
#include <QSqlDatabase>
#include "DataModel.h"
#include <QMap>
namespace Ui { class SiteManagement; }
#define TableName "ftpConfig"
#define  DBName  "userConfigFtp.db"
#define idRole Qt::UserRole +555
class SiteManagement : public PopWidgetBase
{
	Q_OBJECT

public:
	SiteManagement(QWidget *parent = nullptr);
	~SiteManagement();
	void init();
private:
	void creatTable();
	bool openDatabase(QSqlDatabase& db, const QString & path);
	void initFtp();
	void clearUI();
	void saveConfig();
	bool checkRight();
	void insertConfig();
	void updateConfig();
	bool checkExits();
private:
	Ui::SiteManagement* ui;
	QString m_dbPath;
	QMap<int,FtpConfig> m_listConfig;
	FtpConfig m_currentConfig;
	int m_currentId = 0;
};

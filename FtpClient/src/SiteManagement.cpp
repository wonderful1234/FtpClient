#include "SiteManagement.h"
#include "ui_SiteManagement.h"
#include <QStandardPaths>
#include <QSqlQuery>
#include <QDir>
#include <QSqlError>
SiteManagement::SiteManagement(QWidget *parent)
	: PopWidgetBase(parent), ui(new Ui::SiteManagement)
{
	ui->setupUi(this);
	init();
	initFtp();
	connect(ui->listWidget, &QListWidget::itemClicked, this, [&](QListWidgetItem *current) {
		m_currentId =current->data(idRole).toInt();
		m_currentConfig=m_listConfig.value(m_currentId);
		ui->FtpType->setCurrentIndex(static_cast<int>(m_currentConfig.ftpType));
		ui->lineEdit_ip->setText(m_currentConfig.ip);
		ui->lineEdit_port->setText(m_currentConfig.port);
		ui->lineEdit_username->setText(m_currentConfig.userName);
		ui->lineEdit_password->setText(m_currentConfig.password);

	});

	connect(ui->btnClose, &QPushButton::clicked, this, &QDialog::reject);
	connect(ui->btnClosed, &QPushButton::clicked, this, &QDialog::reject);

	connect(ui->btnSure, &QPushButton::clicked, this, [&]() {

		if (!checkRight())
			return;
		if (!saveConfig())
			return;
		accept();
	});

	connect(ui->btnAdd, &QPushButton::clicked, this, [&]() {
		clearUI();
		m_currentConfig = FtpConfig();
		m_currentId =0;
		ui->listWidget->clearSelection();
	});

	connect(ui->btnDel, &QPushButton::clicked, this, [&]() {
		if (m_listConfig.size() == 0)
			return;
		if (ui->listWidget->selectedItems().size() <= 0)
			return;
		deleteConfig(m_currentId);
		delete ui->listWidget->currentItem();
		ui->listWidget->clearSelection();
		clearUI();
		m_currentId = 0;
		m_currentConfig= FtpConfig();


	});
}

SiteManagement::~SiteManagement()
{
	delete ui;
}

void SiteManagement::init()
{
	QString dir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
	QDir dirPath(dir);
	if (!dirPath.exists())
		dirPath.mkdir(dir);
	m_dbPath= dir + "/"+ DBName;
	creatTable();

}

FtpConfig SiteManagement::getCurrentConfig()
{
	return m_currentConfig;
}

void SiteManagement::creatTable()
{
	QSqlDatabase db;
	if (openDatabase(db, m_dbPath))
	{
		QSqlQuery query(db);
		query.exec(QString("CREATE TABLE IF NOT EXISTS %1 "
			"(id INTEGER , "
			"ip TEXT,"
			"username TEXT,"
			"password TEXT,"
			"port TEXT,"
			"type INTEGER,"
			"PRIMARY KEY (\"id\"));").arg(TableName));
		db.close();
	}
}

bool SiteManagement::openDatabase(QSqlDatabase & db, const QString & path)
{
	if (QSqlDatabase::contains(path))
		db = QSqlDatabase::database(path);
	else
	{
		db = QSqlDatabase::addDatabase("QSQLITE", path);
		db.setDatabaseName(path);
	}
	if (!db.isOpen() && !db.open())
	{
		QSqlDatabase::removeDatabase(path);
		db = QSqlDatabase::addDatabase("QSQLITE", path);
		db.setDatabaseName(path);
		if (!db.open())
		{
			return false;
		}
	}
	return true;
}

void SiteManagement::initFtp()
{
	QSqlDatabase db;
	if (openDatabase(db, m_dbPath))
	{
		QSqlQuery query(db);
		if (query.exec(QString("SELECT * FROM %1 ").arg(TableName)))
		{
			while (query.next())
			{
				FtpConfig config;
				config.ip= query.value("ip").toString();
				config.userName= query.value("username").toString();
				config.password= query.value("password").toString();
				config.port= query.value("port").toString();
				config.ftpType= (FtpType)query.value("type").toInt();
				m_listConfig.insert(query.value("id").toInt(), config);
				QListWidgetItem* item = new QListWidgetItem();
				item->setData(idRole, query.value("id").toInt());
				item->setText(config.ip);
				ui->listWidget->addItem(item);
			}
		}

		db.close();
	}
}

void SiteManagement::clearUI()
{
	ui->FtpType->setCurrentIndex(0);
	ui->lineEdit_ip->clear();
	ui->lineEdit_username->clear();
	ui->lineEdit_password->clear();
	ui->lineEdit_port->clear();
}

bool SiteManagement::saveConfig()
{
	bool success = false;
	m_currentConfig.ftpType = (FtpType)ui->FtpType->currentIndex();
	m_currentConfig.ip = ui->lineEdit_ip->text();
	m_currentConfig.password = ui->lineEdit_password->text();
	m_currentConfig.port = ui->lineEdit_port->text();
	m_currentConfig.userName = ui->lineEdit_username->text();
	if (m_currentId==0)
	{
		if (checkExits())
		{
			
			insertConfig();
			success = true;
		}
		else
		{
			m_currentConfig = FtpConfig();
			success = false;
		}

	}
	else
	{
		if (checkExits())
		{
			updateConfig(m_currentId);
		}
		success = true;
	}
	return success;
}

bool SiteManagement::checkRight()
{
	if (ui->lineEdit_ip->text().trimmed().isEmpty() || ui->lineEdit_password->text().trimmed().isEmpty()
		|| ui->lineEdit_port->text().trimmed().isEmpty() || ui->lineEdit_username->text().trimmed().isEmpty())
		return false;
	else
		return true;
	
}

void SiteManagement::insertConfig()
{
	QSqlDatabase db;
	if (openDatabase(db, m_dbPath))
	{
		QSqlQuery query(db);
		query.prepare(QString(
			"INSERT INTO %1 (ip, username, password, port, type) "
			"values(:ip,:username,:password,:port,:type)")
			.arg(TableName));
		query.bindValue(":ip", m_currentConfig.ip);
		query.bindValue(":username", m_currentConfig.userName);
		query.bindValue(":password", m_currentConfig.password);
		query.bindValue(":port", m_currentConfig.port);
		query.bindValue(":type", static_cast<int>(m_currentConfig.ftpType));
		query.exec();
		db.close();
	}
	
}

void SiteManagement::updateConfig(int id)
{
	QSqlDatabase db;
	if (openDatabase(db, m_dbPath))
	{
		QSqlQuery query(db);
		QString sql = "UPDATE %1 set ip = :ip,username = :username,"
			"password = :password,port = :port,type = :type WHERE id = '%2'";
		query.prepare(sql.arg(TableName, id));
		query.bindValue(":ip", m_currentConfig.ip);
		query.bindValue(":username", m_currentConfig.userName);
		query.bindValue(":password", m_currentConfig.password);
		query.bindValue(":port", m_currentConfig.port);
		query.bindValue(":type", static_cast<int>(m_currentConfig.ftpType));
		if (!query.exec())
		{
			auto a = query.lastError().text();
			auto b = query.lastQuery();
		}
		db.close();
	}
}

bool SiteManagement::checkExits()
{

	bool success = true;
	for (auto it = m_listConfig.begin(); it != m_listConfig.end(); ++it)
	{
		if (m_currentConfig == it.value())
		{
			success = false;
			break;
		}
	}
	return success;
}

void SiteManagement::deleteConfig(int id)
{
	QSqlDatabase db;
	if (openDatabase(db, m_dbPath))
	{
		QSqlQuery query(db);
		QString sql = QString(u8"DELETE FROM %1 WHERE id = %2").arg(TableName).arg(id);
		query.exec(sql);
		db.close();
	}
}


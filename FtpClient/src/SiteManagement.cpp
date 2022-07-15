#include "SiteManagement.h"
#include "ui_SiteManagement.h"
#include <QStandardPaths>
#include <QSqlQuery>
#include <QDir>
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
		saveConfig();
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

void SiteManagement::creatTable()
{
	QSqlDatabase db;
	if (openDatabase(db, m_dbPath))
	{
		QSqlQuery query(db);
		query.exec(QString("CREATE TABLE IF NOT EXISTS %1 "
			"(ip TEXT,"
			"username TEXT,"
			"password TEXT,"
			"port TEXT,"
			"type INTEGER,"
			"id INTEGER,"
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

void SiteManagement::saveConfig()
{
	m_currentConfig.ftpType = (FtpType)ui->FtpType->currentIndex();
	m_currentConfig.ip = ui->lineEdit_ip->text();
	m_currentConfig.password = ui->lineEdit_password->text();
	m_currentConfig.port = ui->lineEdit_port->text();
	m_currentConfig.userName = ui->lineEdit_port->text();
	if (m_currentId==0)
	{
		if (checkExits())
		{
			
			insertConfig();
		}
		else
		{
			m_currentConfig = FtpConfig();
		}

	}
	else
	{
		if (checkExits())
		{
			updateConfig();
		}
	}
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

void SiteManagement::updateConfig()
{
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
}


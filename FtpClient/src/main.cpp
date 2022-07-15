#include "FtpClient.h"
#include <QtWidgets/QApplication>
#include "Log.h"
int main(int argc, char *argv[])
{

	QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);
	Log::getInstance();
    FtpClient w;
    w.show();
    return a.exec();
	Log::getInstance()->closeLog();
}

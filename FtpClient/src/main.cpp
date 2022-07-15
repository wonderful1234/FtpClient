#include "FtpClient.h"
#include <QtWidgets/QApplication>
#include "Log.h"
int main(int argc, char *argv[])
{
	
    QApplication a(argc, argv);
	Log::getInstance();
    FtpClient w;
    w.show();
    return a.exec();
	Log::getInstance()->closeLog();
}

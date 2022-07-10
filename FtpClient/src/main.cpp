#include "FtpClient.h"
#include <QtWidgets/QApplication>
int main(int argc, char *argv[])
{
	
    QApplication a(argc, argv);
    FtpClient w;
    w.show();
    return a.exec();
}

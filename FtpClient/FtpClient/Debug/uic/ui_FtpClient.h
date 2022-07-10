/********************************************************************************
** Form generated from reading UI file 'FtpClient.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FTPCLIENT_H
#define UI_FTPCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FtpClient
{
public:

    void setupUi(QWidget *FtpClient)
    {
        if (FtpClient->objectName().isEmpty())
            FtpClient->setObjectName(QString::fromUtf8("FtpClient"));
        FtpClient->resize(600, 400);

        retranslateUi(FtpClient);

        QMetaObject::connectSlotsByName(FtpClient);
    } // setupUi

    void retranslateUi(QWidget *FtpClient)
    {
        FtpClient->setWindowTitle(QCoreApplication::translate("FtpClient", "FtpClient", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FtpClient: public Ui_FtpClient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FTPCLIENT_H

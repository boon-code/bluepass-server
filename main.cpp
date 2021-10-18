#include "main_app.h"
#include <QApplication>
#include <QDebug>

#include <QBluetoothAddress>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainApp::setOrganizationFields();

    MainApp main_app;
    main_app.start();

    qDebug() << "Address: " << QBluetoothAddress();

    return a.exec();
}

#include "main_app.h"
#include "app_constants.h"
#include <QCoreApplication>

MainApp::MainApp(QObject *parent)
    : QObject(parent),
      settings_(),
      dashboard_(&settings_)
{ }

void MainApp::start()
{
    dashboard_.start();
}


void MainApp::setOrganizationFields()
{
    QCoreApplication::setOrganizationName(ORG_NAME);
    QCoreApplication::setOrganizationDomain(ORG_NAME);
    QCoreApplication::setApplicationName(APP_NAME);
}

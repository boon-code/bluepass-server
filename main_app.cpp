#include "main_app.h"
#include "app_constants.h"
#include <QCoreApplication>
#include <QDebug>


MainApp::MainApp(QObject *parent)
    : QObject(parent),
      settings_(),
      dashboard_(&settings_)
{
    connect(&settings_, &Settings::settingsChanged, this, &MainApp::on_configurationChanged);
}

void MainApp::start()
{
    dashboard_.start();
}

void MainApp::on_configurationChanged(const settings_type &new_settings)
{
    qDebug() << "Changed configuration: "
             << new_settings.adapter_address << ", "
             << new_settings.auto_copy_clipboard;
}


void MainApp::setOrganizationFields()
{
    QCoreApplication::setOrganizationName(ORG_NAME);
    QCoreApplication::setOrganizationDomain(ORG_NAME);
    QCoreApplication::setApplicationName(APP_NAME);
}

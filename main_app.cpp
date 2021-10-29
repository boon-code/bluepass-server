#include "main_app.h"
#include "app_constants.h"
#include <QCoreApplication>
#include <QDebug>


MainApp::MainApp(QObject *parent)
    : QObject(parent),
      settings_(new Settings()),
      dashboard_(new Dashboard(settings_)),
      bt_service_(new BluetoothService())
{
    connect(settings_, &Settings::settingsChanged, this, &MainApp::on_configurationChanged);
    connect(dashboard_, &Dashboard::quit, this, &MainApp::on_quit);
    connect(dashboard_, &Dashboard::discoverableChanged, bt_service_, &BluetoothService::setDiscoverable);
    connect(bt_service_, &BluetoothService::codeReceived, dashboard_, &Dashboard::newCode);
    connect(bt_service_, &BluetoothService::started, dashboard_, &Dashboard::setBtServiceStarted);
    connect(bt_service_, &BluetoothService::stopped, dashboard_, &Dashboard::setBtServiceStopped);
    connect(bt_service_, &BluetoothService::bindFailed, dashboard_, &Dashboard::setBtServiceStopped);
}

void MainApp::start()
{
    dashboard_->start();
    QString addr = settings_->settings().adapter_address;
    if (!addr.isEmpty()) {
        bt_service_->start(addr);
    }
}

void MainApp::on_configurationChanged(const settings_type &new_settings)
{
    qDebug() << "Changed configuration: "
             << new_settings.adapter_address << ", "
             << new_settings.auto_copy_clipboard;

    if (new_settings.adapter_address != bt_service_->adapterAddress()) {
        qDebug() << "Need to restart bt service";
        bt_service_->restart(new_settings.adapter_address);
    }
}

void MainApp::on_quit()
{
    bt_service_->terminate();
    bt_service_->deleteLater();
    dashboard_->deleteLater();
    settings_->deleteLater();
    qApp->quit();
}


void MainApp::setOrganizationFields()
{
    QCoreApplication::setOrganizationName(ORG_NAME);
    QCoreApplication::setOrganizationDomain(ORG_NAME);
    QCoreApplication::setApplicationName(APP_NAME);
}

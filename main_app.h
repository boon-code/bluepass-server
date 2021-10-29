#ifndef MAIN_APP_H
#define MAIN_APP_H

#include <QObject>
#include "dashboard.h"
#include "settings.h"
#include "bluetooth_service.h"

class MainApp : public QObject
{
    Q_OBJECT
public:
    explicit MainApp(QObject *parent = nullptr);

    static void setOrganizationFields();

signals:

public slots:
    void start();

private slots:
    void on_configurationChanged(const settings_type &new_settings);
    void on_quit();

private:
    Settings settings_;
    Dashboard dashboard_;
    BluetoothService bt_service_;
};

#endif // MAIN_APP_H

#ifndef MAIN_APP_H
#define MAIN_APP_H

#include <QObject>
#include "dashboard.h"
#include "settings.h"

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

private:
    Settings settings_;
    Dashboard dashboard_;
};

#endif // MAIN_APP_H

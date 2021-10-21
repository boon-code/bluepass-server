#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include "settings_type.h"

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);
    const QString adapterAddress() const;
    bool autoCopyCode() const;
    const struct settings_type settings() const;

signals:
    void settingsChanged(const settings_type& s);

public slots:
    void setAdapterAddress(const QString& address);
    void setAutoCopyCode(const bool enabled);
    void updateSettings(const settings_type& new_settings);
    void saveSettings();
    void reloadSettingsOrDefaults();

private:
    void loadSettingsOrDefaults();

private:
    QSettings settings_storage_;
    struct settings_type current_settings_;
};

#endif // SETTINGS_H

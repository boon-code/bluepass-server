#include "settings.h"
#include "app_constants.h"
#include <QDebug>

#define KEY_ADAPTER_ADDRESS "adapter-address"
#define KEY_AUTO_COPY "auto-copy-clipboard"


Settings::Settings(QObject *parent)
    : QObject(parent),
      settings_storage_(ORG_NAME, APP_NAME),
      current_settings_()
{
    loadSettingsOrDefaults();
}

void Settings::setAdapterAddress(const QString &address)
{
    settings_storage_.setValue(KEY_ADAPTER_ADDRESS, address);
    if (current_settings_.adapter_address != address) {
        current_settings_.adapter_address = address;
        emit settingsChanged(current_settings_);
    }
}

void Settings::setAutoCopyCode(const bool enabled)
{
    settings_storage_.setValue(KEY_AUTO_COPY, enabled);
    if (current_settings_.auto_copy_clipboard != enabled) {
        current_settings_.auto_copy_clipboard = enabled;
        emit settingsChanged(current_settings_);
    }
}

void Settings::updateSettings(const settings_type &new_settings)
{
     if (new_settings != current_settings_) {
        qDebug() << "Settings have been changed: " << new_settings.adapter_address << ", " << new_settings.auto_copy_clipboard;
        current_settings_ = settings_type(new_settings);
        emit settingsChanged(current_settings_);
     }
}

void Settings::loadSettingsOrDefaults()
{
    struct settings_type new_settings;
    new_settings.adapter_address = settings_storage_.value(KEY_ADAPTER_ADDRESS, "").toString();
    new_settings.auto_copy_clipboard = settings_storage_.value(KEY_AUTO_COPY, false).toBool();

    updateSettings(new_settings);
}

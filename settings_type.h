#ifndef SETTINGS_TYPE_H
#define SETTINGS_TYPE_H

#include <QString>
#include <QMetaType>

struct settings_type {
    QString adapter_address;
    bool auto_copy_clipboard;

    settings_type();
    settings_type(const settings_type& r);
    bool operator== (const settings_type& r) const;
    bool operator!= (const settings_type& r) const;
    ~settings_type();
};

Q_DECLARE_METATYPE(settings_type)

#endif // SETTINGS_TYPE_H

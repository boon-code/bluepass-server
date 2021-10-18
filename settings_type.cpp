#include "settings_type.h"

settings_type::settings_type()
{
    adapter_address = "";
    auto_copy_clipboard = false;
}

settings_type::settings_type(const settings_type& r)
{
    adapter_address = r.adapter_address;
    auto_copy_clipboard = r.auto_copy_clipboard;
}

settings_type::~settings_type()
{}

bool settings_type::operator==(const settings_type &r) const
{
    return (adapter_address == r.adapter_address) &&
            (auto_copy_clipboard == r.auto_copy_clipboard);
}

bool settings_type::operator!=(const settings_type &r) const
{
    return !(*this == r);
}

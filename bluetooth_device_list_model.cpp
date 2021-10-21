#include "bluetooth_device_list_model.h"

#include <QBluetoothLocalDevice>

BluetoothDeviceListModel::BluetoothDeviceListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    update();
}

QVariant BluetoothDeviceListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);

    if (role == Qt::DisplayRole) {
        return QVariant(tr("Device Address"));
    } else {
        return QVariant();
    }
}

int BluetoothDeviceListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return devices_.size();
}

QVariant BluetoothDeviceListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole) {
        return QVariant(devices_[index.row()]);
    } else {
        return QVariant();
    }
}

void BluetoothDeviceListModel::update()
{
    QStringList all;
    foreach (auto adapter, QBluetoothLocalDevice::allDevices()) {
        all << adapter.address().toString();
    }

    QStringList remove_list;
    foreach (auto addr, devices_) {
        if (!all.contains(addr)) {
            remove_list << addr;
        }
    }

    foreach (auto addr, remove_list) {
        int pos = devices_.indexOf(addr);
        beginRemoveRows(QModelIndex(), pos, pos);
        devices_.removeAt(pos);
        endRemoveRows();
    }

    foreach(auto addr, all) {
        if (!devices_.contains(addr)) {
            int pos = devices_.size();
            beginInsertRows(QModelIndex(), pos, pos);
            devices_.append(addr);
            endInsertRows();
        }
    }
}

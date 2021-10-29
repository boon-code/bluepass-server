#ifndef BLUETOOTHDEVICELISTMODEL_H
#define BLUETOOTHDEVICELISTMODEL_H

#include <QAbstractListModel>

class BluetoothDeviceListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit BluetoothDeviceListModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QModelIndex indexOf(const QString& adapter) const;

public slots:
    void update();

private:
    QStringList devices_;
};

#endif // BLUETOOTHDEVICELISTMODEL_H

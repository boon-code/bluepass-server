#ifndef BLUETOOTH_SERVICE_H
#define BLUETOOTH_SERVICE_H

#include <QObject>
#include <QtBluetooth/qbluetoothlocaldevice.h>

class BluetoothService : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothService(QObject *parent = nullptr);
    void start(QBluetoothLocalDevice& adapter);

signals:

public slots:
    void stop();

private slots:
    void connected();

private:


};

#endif // BLUETOOTH_SERVICE_H

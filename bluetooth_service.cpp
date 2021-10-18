#include "bluetooth_service.h"
#include <QtBluetooth/qbluetoothserver.h>
#include <QtBluetooth/qbluetoothsocket.h>

static const QLatin1String BLUEPASS_UUID("e4d56fb3-b86d-4572-9b0d-44d483eb1eee");

BluetoothService::BluetoothService(QObject *parent) : QObject(parent)
{

}

void BluetoothService::start(QBluetoothLocalDevice &adapter)
{

}

void BluetoothService::stop()
{

}

void BluetoothService::connected()
{

}

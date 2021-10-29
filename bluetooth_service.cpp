#include "bluetooth_service.h"
#include <QBluetoothSocket>
#include <QBluetoothLocalDevice>
#include <QBluetoothServer>

#define GET_SOCKET(var) \
    QBluetoothSocket *(var) = qobject_cast<QBluetoothSocket *>(sender()); \
    if (socket == nullptr) { \
        qDebug() << "Failed to retrieve socket"; \
        return; \
    }

#define TRY_RESTART_SERVICE_MS 5000
#define POLL_ADAPTER_STATUS_MS 5000

static const QLatin1String BLUEPASS_UUID("e4d56fb3-b86d-4572-9b0d-44d483eb1eee");

BluetoothService::BluetoothService(QObject *parent)
    : QObject(parent),
      server_(nullptr),
      was_made_discoverable_(false)
{
    connect(&retry_timer_, &QTimer::timeout, this, &BluetoothService::retryStart);
    connect(&poll_status_timer_, &QTimer::timeout, this, &BluetoothService::updateAdapterStatus);
}

const QString BluetoothService::adapterAddress() const
{
    return adapter_address_;
}

void BluetoothService::terminate()
{
    stop();
    retry_timer_.stop();
    poll_status_timer_.stop();
}

BluetoothService::~BluetoothService()
{
    terminate();
}

void BluetoothService::start(const QString& localAdapterAddress)
{
    if (server_ != nullptr) {
        qDebug() << "Server is already running";
        return;
    }
    adapter_address_ = localAdapterAddress;

    if (localAdapterAddress.isEmpty()) {
        qDebug() << "Can't start service with no local adapter address";
        return;
    }

    server_ = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    server_->setSecurityFlags(QBluetooth::Authorization |
                              QBluetooth::Authentication |
                              QBluetooth::Encryption |
                              QBluetooth::Secure);

    connect(server_, &QBluetoothServer::newConnection, this, QOverload<>::of(&BluetoothService::connected));

    bool success = server_->listen(QBluetoothAddress(localAdapterAddress));
    if (!success) {
        qDebug() << "Failed to bind on " << localAdapterAddress;
        serverFailed();
        return;
    }

    connect(server_, QOverload<QBluetoothServer::Error>::of(&QBluetoothServer::error), this, &BluetoothService::listeningSocketError);

    info_.setAttribute(QBluetoothServiceInfo::ServiceName, "BluePassServer");
    info_.setServiceUuid(QBluetoothUuid(BLUEPASS_UUID));

    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::ProtocolUuid::Rfcomm))
             << QVariant::fromValue(quint8(server_->serverPort()));
    //protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocolDescriptorList << QVariant::fromValue(protocol);

    info_.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                       protocolDescriptorList);

    info_.registerService(QBluetoothAddress(localAdapterAddress));

    qDebug() << "Bluetooth service started";
    poll_status_timer_.start(POLL_ADAPTER_STATUS_MS);
    emit started();
}

void BluetoothService::stop()
{
    if (server_ == nullptr) {
        qDebug() << "Not started yet...";
        return;
    }

    info_.unregisterService();
    // disconnect all clients
    qDeleteAll(clients_);
    delete server_;
    server_ = nullptr;

    if (was_made_discoverable_) {
        setDiscoverable(false);
        was_made_discoverable_ = false;
    }

    qDebug() << "Bluetooth service stopped";
    poll_status_timer_.stop();
    emit stopped();
}

void BluetoothService::restart(const QString &localAdapterAddress)
{
    if (localAdapterAddress == adapter_address_) {
        return;
    }

    if (server_ != nullptr) {
        stop();
    }

    start(localAdapterAddress);
}

void BluetoothService::setDiscoverable(bool discoverable)
{
    if (server_ == nullptr) {
        qWarning() << "Service has not yet been started";
        return;
    }

    QBluetoothAddress addr(adapter_address_);
    QBluetoothLocalDevice adapter(addr, nullptr);

    if (discoverable) {
        qDebug() << "Making adapter " << adapter_address_ << " discoverable";
        adapter.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
        was_made_discoverable_ = true;
    } else {
        qDebug() << "Stop ";
        adapter.setHostMode(QBluetoothLocalDevice::HostConnectable);
        was_made_discoverable_ = false;
    }
}


void BluetoothService::connected()
{
    QBluetoothSocket *socket = server_->nextPendingConnection();
    if (!socket) {
        qDebug() << "Failed to get newly connected socket";
        return;
    }

    clients_.append(socket);
    qDebug() << "Connected peer " << socket->peerAddress();

    connect(socket, &QBluetoothSocket::readyRead, this, &BluetoothService::readSocket);
    connect(socket, &QBluetoothSocket::disconnected, this, &BluetoothService::disconnected);
}

void BluetoothService::disconnected()
{
    GET_SOCKET(socket);
    tryReadSocket(socket);
    clients_.removeAll(socket);
    socket->deleteLater();

}

void BluetoothService::readSocket()
{
    GET_SOCKET(socket);
    tryReadSocket(socket);
}

void BluetoothService::listeningSocketError(const QBluetoothServer::Error& error)
{
    qWarning() << "Bluetooth service received an error: " << error;
    serverFailed();
}

void BluetoothService::retryStart()
{
    qDebug() << "retry to start service";
    retry_timer_.stop();

    if (!adapter_address_.isEmpty()) {
        qDebug() << "Try to restart service on " << adapter_address_;
        start(adapter_address_);
    }
}

void BluetoothService::updateAdapterStatus()
{
    if (server_ != nullptr) {
        bool is_available = isAdapterAvailable(adapter_address_);
        if (!is_available) {
            qDebug() << "Adapter " << adapter_address_ << " is not available anymore";
            serverFailed();
        }
    }
}

bool BluetoothService::isAdapterAvailable(const QString &address)
{
    foreach (auto adapter, QBluetoothLocalDevice::allDevices()) {
        if (address == adapter.address().toString()) {
            return true;
        }
    }
    return false;
}

void BluetoothService::tryReadSocket(QBluetoothSocket *socket)
{
    while (socket->canReadLine()) {
        QByteArray line = socket->readLine().trimmed();
        if (!line.isEmpty()) {
            emit codeReceived(QString::fromUtf8(line.constData(), line.length()));
            break;
        }
    }
    socket->close();
}

void BluetoothService::serverFailed()
{
    emit bindFailed();
    stop();
    qDebug() << "Failed to start: retry in " << TRY_RESTART_SERVICE_MS << "ms";
    retry_timer_.start(TRY_RESTART_SERVICE_MS);
}

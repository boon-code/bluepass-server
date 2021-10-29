#ifndef BLUETOOTH_SERVICE_H
#define BLUETOOTH_SERVICE_H

#include <QBluetoothServer>
#include <QObject>
#include <QTimer>

class BluetoothService : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothService(QObject *parent = nullptr);
    const QString adapterAddress() const;
    void terminate();
    ~BluetoothService();

signals:
    void codeReceived(const QString& code);
    void started();
    void stopped();
    void bindFailed();

public slots:
    void start(const QString &localAdapterAddress);
    void stop();
    void restart(const QString& localAdapterAddress);
    void setDiscoverable(bool discoverable);

private slots:
    void connected();
    void disconnected();
    void readSocket();
    void listeningSocketError(const QBluetoothServer::Error &error);
    void retryStart();
    void updateAdapterStatus();

private:
    void tryReadSocket(QBluetoothSocket *socket);
    void serverFailed();
    bool isAdapterAvailable(const QString& address);

private:
    QBluetoothServer *server_;
    QBluetoothServiceInfo info_;
    QList<QBluetoothSocket *> clients_;
    QString adapter_address_;
    bool was_made_discoverable_;
    QTimer retry_timer_;
    QTimer poll_status_timer_;
};

#endif // BLUETOOTH_SERVICE_H

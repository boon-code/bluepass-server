#ifndef CHOOSE_ADAPTER_DIALOG_H
#define CHOOSE_ADAPTER_DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QStringListModel>
#include <QItemSelectionModel>
#include "bluetooth_device_list_model.h"

namespace Ui {
class ChooseAdapterDialog;
}

class ChooseAdapterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseAdapterDialog(const QString& current_adapter, QWidget *parent = nullptr);
    ~ChooseAdapterDialog();

signals:
    void adapterChosen(const QString& adapter);

private slots:
    void on_dbbOkCancel_accepted();

    void on_dbbOkCancel_rejected();

    void showCurrent(const QModelIndex& current, const QModelIndex& previous);

private:
    void showUnselected();

private:
    Ui::ChooseAdapterDialog *ui;
    QTimer refresh_timer_;
    BluetoothDeviceListModel adapters_model_;
    QItemSelectionModel selection_model_;
};

#endif // CHOOSE_ADAPTER_DIALOG_H

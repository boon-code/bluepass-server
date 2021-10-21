#include "choose_adapter_dialog.h"
#include "ui_choose_adapter_dialog.h"

#include <QBluetoothLocalDevice>

ChooseAdapterDialog::ChooseAdapterDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ChooseAdapterDialog),
      adapters_model_(),
      selection_model_(&adapters_model_)
{
    ui->setupUi(this);
    ui->listAdapters->setModel(&adapters_model_);
    ui->listAdapters->setSelectionModel(&selection_model_);
    refresh_timer_.start(2000);
    connect(&refresh_timer_, &QTimer::timeout, &adapters_model_, &BluetoothDeviceListModel::update);
}

ChooseAdapterDialog::~ChooseAdapterDialog()
{
    refresh_timer_.stop();
    delete ui;
}

void ChooseAdapterDialog::on_dbbOkCancel_accepted()
{
    QVariant adapter;

    if (selection_model_.hasSelection()) {
        adapter = adapters_model_.data(selection_model_.currentIndex());
    }

    if (adapter.isNull()) {
        adapter = QVariant("");
    }

    emit adapterChosen(adapter.toString());
    accept();
}

void ChooseAdapterDialog::on_dbbOkCancel_rejected()
{
    reject();
}

#include "choose_adapter_dialog.h"
#include "ui_choose_adapter_dialog.h"

#include <QBluetoothLocalDevice>
#include <QDebug>


ChooseAdapterDialog::ChooseAdapterDialog(const QString &current_adapter, QWidget *parent)
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
    connect(&selection_model_, &QItemSelectionModel::currentChanged, this, &ChooseAdapterDialog::showCurrent);
    QModelIndex current = adapters_model_.indexOf(current_adapter);
    selection_model_.setCurrentIndex(current, QItemSelectionModel::SelectionFlag::SelectCurrent);
    showCurrent(current, QModelIndex());
}

ChooseAdapterDialog::~ChooseAdapterDialog()
{
    refresh_timer_.stop();
    delete ui;
}

void ChooseAdapterDialog::on_dbbOkCancel_accepted()
{
    QVariant adapter;

    const QModelIndex index = selection_model_.currentIndex();
    if (index.isValid()) {
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

void ChooseAdapterDialog::showCurrent(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    if (current.isValid()) {
        qDebug() << "valid index";
        ui->labAdapter->setText(adapters_model_.data(current).toString());
    } else {
        qDebug() << "invalid index";
        ui->labAdapter->setText(tr("No adapter selected"));
    }
}

void ChooseAdapterDialog::showUnselected()
{
    QModelIndex invalid_index;
    showCurrent(invalid_index, invalid_index);
}

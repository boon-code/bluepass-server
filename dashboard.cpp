#include "dashboard.h"
#include "ui_dashboard.h"
#include "choose_adapter_dialog.h"

#include <QCloseEvent>
#include <QMenu>
#include <QDebug>
#include <QBluetoothLocalDevice>
#include <QClipboard>
#ifdef HAS_VERSION
#include "c_git_version.h"
#endif

#define MAX_DISCOVERABLE_TIME_MS 60000


Dashboard::Dashboard(Settings *settings, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Dashboard),
    settings_(settings),
    current_settings_(settings->settings()),
    tray_menu_(nullptr),
    tray_action_open_(nullptr),
    tray_action_copy_(nullptr),
    tray_action_quit_(nullptr),
    tray_action_make_discoverable_(nullptr),
    tray_icon_ok_(":/icons/BluePassServer.ico"),
    tray_icon_discoverable_(":/icons/BluePassServer2.ico"),
    tray_icon_no_adapter_(":/icons/BluePassServer3.ico"),
    tray_(nullptr),
    discoverable_(false),
    bt_service_started_(false)
{
    ui->setupUi(this);
    setTitle();
    updateSettingsView();
    setupTray();
    on_configurationChanged(current_settings_);
    connect(settings, &Settings::settingsChanged, this, &Dashboard::on_configurationChanged);
    connect(&reset_discoverable_timer_, &QTimer::timeout, this, &Dashboard::on_resetDiscoverable);
}

void Dashboard::updateSettingsView()
{
    if (current_settings_.adapter_address.isEmpty()) {
        ui->labAdapter->setText(tr("No bluetooth adapter available"));
    } else {
        ui->labAdapter->setText(current_settings_.adapter_address);
    }

    ui->chkCopyToClipboard->setCheckState(current_settings_.auto_copy_clipboard ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
}

Dashboard::~Dashboard()
{
    delete ui;

    // Automatically deleted:
    tray_ = nullptr;
    tray_menu_ = nullptr;
    tray_action_open_ = nullptr;
    tray_action_copy_ = nullptr;
    tray_action_quit_ = nullptr;
    tray_action_make_discoverable_ = nullptr;
    tray_ = nullptr;
}

void Dashboard::start()
{
    Q_ASSERT(tray_ != nullptr);

    tray_->show();
}

void Dashboard::newCode(const QString &code)
{
    qDebug() << "Code received: " << code;

    if (!code.isEmpty()) {
        last_key_ = code;
        tray_action_copy_->setEnabled(true);
    }

    if (settings_->autoCopyCode()) {
        copyToClipboard(code);
    }

    tray_->showMessage(tr("BluePass"), tr("Received code: %1").arg(code), tray_icon_ok_, 5000);
}

void Dashboard::setBtServiceStarted()
{
    bt_service_started_ = true;
    updateTrayIcon();
}

void Dashboard::setBtServiceStopped()
{
    bt_service_started_ = false;
    discoverable_ = false;
    updateTrayIcon();
    updateDiscoverableText();
}

void Dashboard::trayActivated(const QSystemTrayIcon::ActivationReason &reason)
{
    switch (reason) {
    case QSystemTrayIcon::ActivationReason::Trigger:
        if (isHidden()) {
            show();
        } else {
            hide();
        }
        break;

    default:
        break;
    }
}

void Dashboard::setAdapter(const QString &adapter)
{
    current_settings_.adapter_address = adapter;
    updateSettingsView();
}

void Dashboard::on_toggleDiscoverable()
{
    if (discoverable_) {
        on_resetDiscoverable();
    } else {
        on_setDiscoverable();
    }
}

void Dashboard::on_setDiscoverable()
{
    qDebug() << "Make adapter discoverable";
    reset_discoverable_timer_.stop();
    reset_discoverable_timer_.start(MAX_DISCOVERABLE_TIME_MS);
    emit discoverableChanged(true);
    discoverable_ = true;
    updateDiscoverableText();
    updateTrayIcon();
}

void Dashboard::on_resetDiscoverable()
{
    reset_discoverable_timer_.stop();
    emit discoverableChanged(false);
    discoverable_ = false;
    updateDiscoverableText();
    updateTrayIcon();
}

void Dashboard::on_configurationChanged(const settings_type &new_settings)
{
    tray_action_make_discoverable_->setEnabled(!new_settings.adapter_address.isEmpty());
}

void Dashboard::setupTray()
{
    // Setup actions
    tray_action_open_ = new QAction(tr("Dashboard"), this);
    tray_action_copy_ = new QAction(tr("Copy last"), this);
    tray_action_copy_->setEnabled(false);
    tray_action_quit_ = new QAction(tr("Quit"), this);
    tray_action_make_discoverable_ = new QAction(tr("Make discoverable"), this);

    // Connect actions
    connect(tray_action_open_, &QAction::triggered, this, &Dashboard::show);
    connect(tray_action_copy_, &QAction::triggered, this, &Dashboard::on_copyLastKey);
    connect(tray_action_quit_, &QAction::triggered, this, &Dashboard::quit);
    connect(tray_action_make_discoverable_, &QAction::triggered, this, &Dashboard::on_toggleDiscoverable);

    // Create menu
    tray_menu_ = new QMenu(this);
    tray_menu_->addAction(tray_action_open_);
    tray_menu_->addAction(tray_action_copy_);
    tray_menu_->addSeparator();
    tray_menu_->addAction(tray_action_make_discoverable_);
    tray_menu_->addSeparator();
    tray_menu_->addAction(tray_action_quit_);

    // Create tray
    tray_ = new QSystemTrayIcon(tray_icon_ok_, this);
    tray_->setContextMenu(tray_menu_);
    connect(tray_, &QSystemTrayIcon::activated, this, &Dashboard::trayActivated);
    connect(tray_, &QSystemTrayIcon::messageClicked, this, &Dashboard::on_copyLastKey);
}

void Dashboard::closeEvent(QCloseEvent *event)
{
    event->ignore();
    hide();
}

void Dashboard::copyToClipboard(const QString &code)
{
    QClipboard *clip = QApplication::clipboard();
    if (clip == nullptr) {
        qDebug() << "Couldn't get clipboard instance";
        return;
    }
    if (code.isEmpty()) {
        return;
    }
    clip->setText(code, QClipboard::Mode::Clipboard);
}

void Dashboard::updateDiscoverableText()
{
    if (discoverable_) {
        tray_action_make_discoverable_->setText(tr("Stop discoverable"));
    } else {
        tray_action_make_discoverable_->setText(tr("Make discoverable"));
    }
}

void Dashboard::updateTrayIcon()
{
    if (bt_service_started_) {
        if (discoverable_) {
            tray_->setIcon(tray_icon_discoverable_);
        } else {
            tray_->setIcon(tray_icon_ok_);
        }
    } else {
        tray_->setIcon(tray_icon_no_adapter_);
    }
}

void Dashboard::setTitle()
{
#ifdef HAS_VERSION
    this->setWindowTitle(QString("BluePassServer %1").arg(c_git_version()));
#else
    this->setWindowTitle("BluePassServer");
#endif
}

void Dashboard::on_dbbOkCancel_accepted()
{
    settings_->updateSettings(current_settings_);
    settings_->saveSettings();
    hide();
}

void Dashboard::on_dbbOkCancel_rejected()
{
    settings_->reloadSettingsOrDefaults();
    current_settings_ = settings_->settings();
    updateSettingsView();
    hide();
}

void Dashboard::on_chkCopyToClipboard_toggled(bool checked)
{
    current_settings_.auto_copy_clipboard = checked;
}

void Dashboard::on_pbSelectAdapter_clicked()
{
    ChooseAdapterDialog *dialog = new ChooseAdapterDialog(current_settings_.adapter_address);
    connect(dialog, &ChooseAdapterDialog::adapterChosen, this, &Dashboard::setAdapter);
    dialog->exec();
    dialog->deleteLater();
}

void Dashboard::on_copyLastKey()
{
    if (!last_key_.isEmpty()) {
        copyToClipboard(last_key_);
    }
}

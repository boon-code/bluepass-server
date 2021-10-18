#include "dashboard.h"
#include "ui_dashboard.h"

Dashboard::Dashboard(Settings *settings, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Dashboard),
    settings_(settings),
    tray_menu_(nullptr),
    tray_action_open_(nullptr),
    tray_action_copy_(nullptr),
    tray_action_quit_(nullptr),
    tray_icon_ok_(":/icons/BluePassServer.ico"),
    tray_icon_no_adapter_(":/icons/BluePassServer2.ico"),
    tray_(nullptr),
    dummy_switch_(false)
{
    ui->setupUi(this);
    setupTray();
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
    tray_ = nullptr;
}

void Dashboard::start()
{
    Q_ASSERT(tray_ != nullptr);

    tray_->show();
}

void Dashboard::dummyAction()
{
    Q_ASSERT(tray_ != nullptr);

    QIcon* icon = &tray_icon_ok_;

    dummy_switch_ = !dummy_switch_;
    if (dummy_switch_) {
        tray_->setIcon(tray_icon_no_adapter_);
        tray_->setToolTip(tr("No bluetooth adapater is available"));
        tray_action_copy_->setEnabled(false);
    } else {
        tray_->setIcon(tray_icon_ok_);
        tray_->setToolTip("");
        tray_action_copy_->setEnabled(true);
        icon = &tray_icon_no_adapter_;
    }

    tray_->showMessage(tr("Dummy action"), tr("Triggered dummy action"), *icon);
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
        dummyAction();  // TODO: Remove
        break;

    default:
        break;
    }
}

void Dashboard::setupTray()
{
    // Setup actions
    tray_action_open_ = new QAction(tr("Dashboard"), this);
    tray_action_copy_ = new QAction(tr("Copy last"), this);
    tray_action_quit_ = new QAction(tr("Quit"), this);

    // Connect actions
    connect(tray_action_open_, &QAction::triggered, this, &Dashboard::show);
    connect(tray_action_copy_, &QAction::triggered, this, &Dashboard::dummyAction);
    connect(tray_action_quit_, &QAction::triggered, qApp, &QCoreApplication::quit);

    // Create menu
    tray_menu_ = new QMenu(this);
    tray_menu_->addAction(tray_action_open_);
    tray_menu_->addAction(tray_action_copy_);
    tray_menu_->addSeparator();
    tray_menu_->addAction(tray_action_quit_);

    // Create tray
    tray_ = new QSystemTrayIcon(tray_icon_ok_, this);
    tray_->setContextMenu(tray_menu_);
    connect(tray_, &QSystemTrayIcon::activated, this, &Dashboard::trayActivated);
}

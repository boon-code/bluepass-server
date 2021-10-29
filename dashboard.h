#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QMainWindow>
#include <QAction>
#include <QSystemTrayIcon>
#include <QTimer>
#include <settings.h>

namespace Ui {
class Dashboard;
}

class Dashboard : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Dashboard
     * @param settings Weak reference to settings object
     * @param parent
     */
    explicit Dashboard(Settings *settings, QWidget *parent = nullptr);
    ~Dashboard();

signals:
    void discoverableChanged(bool discoverable);
    void quit();

public slots:
    void start();
    void newCode(const QString& code);
    void setBtServiceStarted();
    void setBtServiceStopped();

private slots:
    void trayActivated(const QSystemTrayIcon::ActivationReason& reason);
    void setAdapter(const QString& adapter);
    void on_setDiscoverable();
    void on_toggleDiscoverable();
    void on_resetDiscoverable();
    void on_configurationChanged(const settings_type &new_settings);
    void on_dbbOkCancel_accepted();
    void on_dbbOkCancel_rejected();
    void on_chkCopyToClipboard_toggled(bool checked);
    void on_pbSelectAdapter_clicked();

private:
    void updateSettingsView();
    void setupTray(void);
    void closeEvent(QCloseEvent *event) override;
    void copyToClipboard(const QString& code);
    void updateDiscoverableText();
    void updateTrayIcon();

private:
    Ui::Dashboard *ui;

    Settings *settings_;
    struct settings_type current_settings_;

    QMenu *tray_menu_;
    QAction *tray_action_open_;
    QAction *tray_action_copy_;
    QAction *tray_action_quit_;
    QAction *tray_action_make_discoverable_;
    QIcon tray_icon_ok_;
    QIcon tray_icon_discoverable_;
    QIcon tray_icon_no_adapter_;
    QSystemTrayIcon *tray_;

    QTimer reset_discoverable_timer_;
    bool discoverable_;
    bool bt_service_started_;
};

#endif // DASHBOARD_H

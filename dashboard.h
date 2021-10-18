#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QMainWindow>
#include <QAction>
#include <QSystemTrayIcon>
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

public slots:
    void start();

private slots:
    void dummyAction(void);
    void trayActivated(const QSystemTrayIcon::ActivationReason& reason);

private:
    void setupTray(void);

private:
    Ui::Dashboard *ui;

    Settings *settings_;

    QMenu *tray_menu_;
    QAction *tray_action_open_;
    QAction *tray_action_copy_;
    QAction *tray_action_quit_;
    QIcon tray_icon_ok_;
    QIcon tray_icon_no_adapter_;
    QSystemTrayIcon *tray_;

    bool dummy_switch_;
};

#endif // DASHBOARD_H

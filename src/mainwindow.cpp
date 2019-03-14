#include <QListView>
#include <QDBusConnection>
#include <QtCore/QDebug>
#include <QtDBus/QtDBus>
#include <QScreen>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "appmodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    appmodel(new AppModel)
{
    ui->setupUi(this);
    resize (860, 500);
    move(QGuiApplication::primaryScreen()->availableGeometry().center() - this->rect().center());
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    ui->centralWidget->setObjectName("centralWidget");

    ui->closeButton->setIcon(QIcon("/usr/share/icons/ukui-icon-theme/24x24/actions/dialog-close.png"));
    ui->closeButton->setObjectName("closeButton");
    ui->verticalLayout->setAlignment(ui->closeButton, Qt::AlignRight);

    ui->titleLabel->setText(tr("You can install the third-party software from kylin software center"));

    initTableView();

    ui->horizontalLayout->setAlignment(ui->checkBox, Qt::AlignLeft);

    ui->installButton->setObjectName("installButton");
    ui->horizontalLayout->setAlignment(ui->installButton, Qt::AlignRight);

}

void MainWindow::initTableView()
{
    ui->tableView->setIconSize(QSize(200, 200));
    ui->tableView->setModel(appmodel);
    ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->horizontalHeader()->hide();
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setShowGrid(false);
//    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setRowHeight(0, 70);
    ui->tableView->setRowHeight(1, 70);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete appmodel;
}

void MainWindow::on_closeButton_clicked()
{
    close();
}

void MainWindow::doInstall()
{
    QDBusConnection bus = QDBusConnection::systemBus();
    if (!bus.isConnected())
    {
        fprintf(stderr, "Cannot connect to the D-Bus session bus.");
        return;
    }
    QDBusInterface dbus_iface("com.ubuntukylin.softwarecenter", "/",
                              "com.ubuntukylin.softwarecenter", bus);
    if (dbus_iface.isValid()) {
        QDBusReply<QString> reply_update = dbus_iface.call("update", true);
        if (reply_update.isValid() && reply_update.value() == "True") {
            qDebug() << "Update success!";
        } else {
            qDebug() << "Update failed!" << reply_update.value();
        }
        for (int i = 0; i < install_list.size(); ++i) {
                QDBusReply<bool> reply_install = dbus_iface.call("install", install_list[i]);
                if (reply_install.isValid() && reply_install.value())
                        qDebug() << "Install " << install_list[i] << " success!";
                else
                        qDebug() << "Install " << install_list[i] << " failed";
        }
    }
    /*
    if (dbus_iface.isValid()) {
        QDBusReply<QStringList> reply = dbus_iface.call("get_sources", QVariant(true));
        if (reply.isValid()) {
            for (int i = 0; i < reply.value().size(); ++i)
                qDebug() << reply.value().at(i);
        } else {
            fprintf(stderr, "Call failed: %s\n", qPrintable(reply.error().message()));
        }
    } else {
        fprintf (stderr, "%s\n",
                 qPrintable(QDBusConnection::systemBus().lastError().message()));
    }
    */
}

void delayMsec(int msec)
{
    QTime timer = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < timer)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::startProgram()
{
    hide();
    QString program = "/usr/bin/ubuntu-kylin-software-center";
    QProcess().startDetached(program);
    // TODO: Delete this delay once find other method.
    delayMsec(3000);
    close();
}

void MainWindow::on_installButton_clicked()
{
    QModelIndexList selection_list = ui->tableView->selectionModel()->selectedIndexes();
    for (int i = 0; i < selection_list.count(); ++i) {
        QString tmp = ui->tableView->model()->data(selection_list.at(i)).toString();
        int pos = tmp.indexOf("\n");
        qDebug() << tmp.mid(0, pos);
        install_list.append(tmp.mid(0, pos));
    }
    doInstall();
    if (ui->checkBox->isChecked())
        startProgram();
}

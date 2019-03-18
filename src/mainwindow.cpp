#include <QListView>
#include <QDBusConnection>
#include <QtCore/QDebug>
#include <QtDBus/QtDBus>
#include <QScreen>
#include <QMessageBox>

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
    ui->centralWidget->layout()->setContentsMargins(60, 0, 0, 0);

    ui->closeButton->setIcon(QIcon(":/icon/data/dialog-close.png"));
    ui->closeButton->setObjectName("closeButton");
    ui->verticalLayout->setAlignment(ui->closeButton, Qt::AlignRight);

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
    ui->tableView->verticalHeader()->setDefaultSectionSize(80);

//    connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
//            SLOT(on_selectionChanged(const QItemSelection &, const QItemSelection &)));
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

int MainWindow::showMessageDialog(const char * message)
{
    QMessageBox msgBox;
    msgBox.setText(tr(message));
    msgBox.setStandardButtons(QMessageBox::Ok);
    int ret = msgBox.exec();
    return ret;
}

void MainWindow::doInstall()
{
    int ret;
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
            QString messages;
            for (int i = 0; i < install_list.size(); ++i) {
                QDBusReply<bool> reply_install = dbus_iface.call("install", install_list[i]);
                if (reply_install.isValid() && reply_install.value()) {
                        messages += "Install " + install_list[i] + " success!\n";
                } else {
                        messages += "Install " + install_list[i] + " success!\n";
                }
            }
            ret = showMessageDialog(messages.toStdString().c_str());
        } else {
            qDebug() << "Update failed!" << reply_update.value();
            QString err_str = tr("Error when install software, please open ubuntu kylin software center to install them manally!");
            ret = showMessageDialog(err_str.toStdString().c_str());
        }
    }

    launchSoftwareCenter();
    QCoreApplication::quit();
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

void MainWindow::launchSoftwareCenter()
{
    if (!ui->checkBox->isChecked())
        return;
    hide();
    QString program = "/usr/bin/ubuntu-kylin-software-center";
    QProcess().startDetached(program);
    // TODO: Delete this delay once find other method.
    delayMsec(3000);
    QCoreApplication::quit();
}

void MainWindow::on_installButton_clicked()
{
    QAbstractItemModel *model = ui->tableView->model();
    int rows = model->rowCount();
    int column = model->columnCount();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < column; ++j) {
            QModelIndex item = model->index(i, j);
            if (model->data(item, Qt::CheckStateRole) == Qt::Checked) {
                QString tmp = model->data(item).toString();
                int pos = tmp.indexOf("\n");
                qDebug() << tmp.mid(0, pos);
                install_list.append(tmp.mid(0, pos));
            }
        }
    }
    if (!install_list.empty())
        doInstall();
    launchSoftwareCenter();
}

void MainWindow::on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QModelIndexList selection_list = selected.indexes();
    for (int i = 0; i < selection_list.count(); ++i) {
        ui->tableView->model()->setData(selection_list.at(i), Qt::Checked, Qt::CheckStateRole);
    }

    QModelIndexList deselection_list = deselected.indexes();
    for (int i = 0; i < deselection_list.count(); ++i) {
        ui->tableView->model()->setData(deselection_list.at(i), Qt::Unchecked, Qt::CheckStateRole);
    }
}

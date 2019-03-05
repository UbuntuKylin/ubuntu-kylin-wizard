#include "mainwindow.h"
#include <QListView>

#include "ui_mainwindow.h"
#include "appmodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    appmodel(new AppModel)
{
    ui->setupUi(this);
    resize (860, 500);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    ui->verticalLayout->setAlignment(ui->closeButton, Qt::AlignRight);
    ui->titleLabel->setText("在这里，你可以安装第三方软件：");
    ui->horizontalLayout->setAlignment(ui->checkBox, Qt::AlignLeft);
    ui->horizontalLayout->setAlignment(ui->installButton, Qt::AlignRight);
    ui->tableView->setIconSize(QSize(200, 200));
    ui->tableView->setModel(appmodel);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->horizontalHeader()->hide();
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->resizeRowsToContents();
    ui->tableView->setShowGrid(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_closeButton_clicked()
{
    close();
}

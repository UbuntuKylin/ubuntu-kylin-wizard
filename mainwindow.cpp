#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize (860, 500);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    ui->verticalLayout->setAlignment(ui->pushButton, Qt::AlignRight);
    ui->horizontalLayout->setAlignment(ui->checkBox, Qt::AlignLeft);
    ui->horizontalLayout->setAlignment(ui->pushButton_2, Qt::AlignRight);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    close();
}
